<?php
/**
 * @file
 * This include processes Soundcloud audio files for use by emaudio.module.
 */

define('EMAUDIO_SOUNDCLOUD_MAIN_URL', 'http://soundcloud.com/');

/**
 * Implementation of hook_emaudio_PROVIDER_info().
 *
 * This returns information relevant to a specific 3rd party audio provider.
 *
 * @return
 *   An array of strings requested by various admin and other forms.
 *   'name' => The translated name of the provider.
 *   'url' => The url to the main page for the provider.
 *   'settings_description' => A description of the provider that will be posted in the admin settings form.
 *   'supported_features' => An array of rows describing the state of certain supported features by the provider.
 *      These will be rendered in a table, with the columns being 'Feature', 'Supported', 'Notes'.
 */
function emaudio_soundcloud_info() {
  $features = array(
    array(t('Autoplay'), t('Yes'), ''),
    array(t('RSS Attachment'), t('No'), ''),
    array(t('Thumbnails'), t('No'), t('')),
    array(t('Custom player colors'), t('Yes'), t("You may customize the player's play button color.")),
    array(t('Show SoundCloud comments'), t('Yes'), t("You may choose whether to show timed comments on the player.")),
  );
  return array(
    'provider' => 'soundcloud',
    'name' => t('SoundCloud'),
    'url' => EMAUDIO_SOUNDCLOUD_MAIN_URL,
    'settings_description' => t('These settings specifically affect audio displayed from <a href="@soundcloud" target="_blank">SoundCloud</a>.', array('@soundcloud' => EMAUDIO_SOUNDCLOUD_MAIN_URL)),
    'supported_features' => $features,
  );
}

/**
 * Implementation of hook_emaudio_PROVIDER_settings().
 *
 * This should return a subform to be added to the emaudio_settings() admin
 * settings page.
 * Note that a form fieldset will already be provided, at $form['odeo'] (such as
 * $form['podomatic']).
 * So if you want specific provider settings within that field, you can add the
 * elements to that form field.
 */
function emaudio_soundcloud_settings() {
  $form = array();

  $form['soundcloud']['media_soundcloud_color_play'] = array(
    '#type' => 'textfield',
    '#title' => t('Play button color'),
    '#size' => 8,
    '#default_value' => variable_get('media_soundcloud_color_play', ''),
  );
  if (module_exists('colorpicker')) {
    $form['soundcloud']['media_soundcloud_color_play']['#type'] = 'colorpicker_textfield';
  }

  $form['soundcloud']['media_soundcloud_comments'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show timed comments'),
    '#size' => 8,
    '#default_value' => variable_get('media_soundcloud_comments', TRUE),
  );

  return $form;
}

/**
 * Implementation of hook_emaudio_PROVIDER_extract().
 *
 * This is called to extract the video code from a pasted URL or embed code.
 *
 * @param $embed
 *   An optional string with the pasted URL or embed code.
 * @return
 *   Either an array of regex expressions to be tested, or a string with the
 *   audio code to be used. If the hook tests the code itself, it should return
 *   either the string of the audio code (if matched), or an empty array.
 *   Otherwise, the calling function will handle testing the embed code against
 *   each regex string in the returned array.
 */
function emaudio_soundcloud_extract($embed = '') {
  return array(
    '@soundcloud\.com/([^"\&]+)@i',
  );
}

/**
 * Implementation of hook_emaudio_PROVIDER_embedded_link().
 */
function emaudio_soundcloud_embedded_link($code, $data = array()) {
  return EMAUDIO_SOUNDCLOUD_MAIN_URL . $code;
}

/**
 * Implementation of hook_PROVIDER_data().
 */
function emaudio_soundcloud_data($field, $item) {
  // Create the initial data for the enclosure with the thumbnail and asset.
  $data = array(
    'emaudio_soundcloud_version' => 1,
  );

  // If the given code is between <object> tags, assume it's an actual embed
  // code rather than a URL, and return it directly.
  if (preg_match('/^<object.*<\/object>.*/i', $item['embed'])) {
    $data['embed'] = TRUE;
    return $data;
  }

  $soundcloud_id = $item['value'];
  $data['id'] = $soundcloud_id;

  // Build up the parameters for the player.
  // See http://developers.soundcloud.com/docs/oembed.
  // Note that booleans have to passed as string representations.
  $oembed_parameters = array(
    'maxwidth' => $field['widget']['audio_width'],
    'maxheight' => $field['widget']['audio_height'],
    'show_comments' => variable_get('media_soundcloud_comments', TRUE) ? 'true' : 'false',
  );
  if ($field['widget']['audio_autoplay']) {
    $oembed_parameters['auto_play'] = 'true';
  }
  $color_play = variable_get('media_soundcloud_color_play', '');
  if ($color_play) {
    $oembed_parameters['color'] = $color_play;
  }

  $data['player'] = media_soundcloud_get_oembed_player($soundcloud_id, $oembed_parameters);

  // If any of the preview settings differ from the main ones, get a second
  // player for the preview.
  if ($field['widget']['preview_width'] != $field['widget']['audio_width']
    || $field['widget']['preview_height'] != $field['widget']['audio_height']
    || $field['widget']['preview_autoplay'] != $field['widget']['audio_autoplay']
  ) {
    $oembed_parameters['maxwidth'] = $field['widget']['preview_width'];
    $oembed_parameters['maxheight'] = $field['widget']['preview_height'];
    // Have to explicitly set this in case it was earlier set to non-default.
    $oembed_parameters['auto_play'] = $field['widget']['preview_autoplay'] ? 'true' : 'false';

    $data['preview'] = media_soundcloud_get_oembed_player($soundcloud_id, $oembed_parameters);
  }

  return $data;
}

/**
 * Helper function to get an embedded player from SoundCloud.
 *
 * @param $soundcloud_id
 *  The SoundCloud id.
 * @param $oembed_parameters
 *  An array of parameters to pass in the oEmbed URL, in addition to the URL
 *  and format which are added in for you.
 *  For possible keys see http://developers.soundcloud.com/docs/oembed.
 */
function media_soundcloud_get_oembed_player($soundcloud_id, $oembed_parameters) {
  $oembed_parameters += array(
    'url' => EMAUDIO_SOUNDCLOUD_MAIN_URL . urldecode($soundcloud_id),
    'format' => 'json',
  );

  $url = url(EMAUDIO_SOUNDCLOUD_MAIN_URL . 'oembed', array('query' => $oembed_parameters));
  $response = drupal_http_request($url);
  // Check the return code for the request.
  if ($response->code == 200) {
    // Code 200 assumes successful query of the soundcloud servers.
    $json = json_decode($response->data);
    $html = (string) $json->html;
    return $html;
  }
  else {
    // Any other code we'll assume is an error.
    drupal_set_message(t('SoundCloud server returned an error: @code @status', array(
      '@code' => $response->code,
      '@status' => $response->status_message,
    )));
  }
}

/**
 * Implementation of hook_emaudio_PROVIDER_thumbnail().
 *
 * Returns the external url for a thumbnail of a specific audio.
 * TODO: make the args: ($embed, $field, $item), with $field/$item provided if we need it, but otherwise simplifying things.
 *
 * @param $field
 *   The field of the requesting node.
 * @param $item
 *   The actual content of the field from the requesting node.
 * @return
 *   A URL pointing to the thumbnail.
 */
function emaudio_soundcloud_thumbnail($field, $item, $formatter, $node, $width, $height) {
  return $tn;
}

/**
 * Implementation of hook_emaudio_PROVIDER_audio().
 *
 * This actually displays the full/normal-sized video we want, usually on the default page view.
 *
 * @param $embed
 *   The video code for the audio to embed.
 * @param $width
 *   The width to display the audio.
 * @param $height
 *   The height to display the audio.
 * @param $field
 *   The field info from the requesting node.
 * @param $item
 *   The actual content from the field.
 * @return
 *   The html of the embedded audio.
 */
function emaudio_soundcloud_audio($embed, $width, $height, $field, $item, $node, $autoplay) {
  return theme('emaudio_soundcloud_flash', $item, $embed, $width, $height, $autoplay);
}

/**
 * Implementation of hook_emaudio_PROVIDER_preview().
 *
 * This actually displays the preview-sized video we want, commonly for the teaser.
 *
 * @param $embed
 *   The video code for the audio to embed.
 * @param $width
 *   The width to display the audio.
 * @param $height
 *   The height to display the audio.
 * @param $field
 *   The field info from the requesting node.
 * @param $item
 *   The actual content from the field.
 * @return
 *   The html of the embedded audio.
 */
function emaudio_soundcloud_preview($embed, $width, $height, $field, $item, $node, $autoplay) {
  return theme('emaudio_soundcloud_flash', $item, $embed, $width, $height, $autoplay, TRUE);
}

/**
 * The embedded flash displaying the SoundCloud audio.
 */
function theme_emaudio_soundcloud_flash($item, $embed, $width, $height, $autoplay, $preview = FALSE) {
  $output = '';

  if ($item['data']['emaudio_soundcloud_version'] >= 1) {
    if ($item['data']['embed']) {
      // Full embed code.
      $output = $item['embed'];
    }
    else {
      // SoundCloud ID: return the cached HTML.
      if ($preview && isset($item['data']['preview'])) {
        $output = $item['data']['preview'];
      }
      else {
        $output = $item['data']['player'];
      }
    }
  }

  return $output;
}

/**
 * Implementation of hook_emfield_subtheme.
 */
function emaudio_soundcloud_emfield_subtheme() {
  return array(
    'emaudio_soundcloud_flash'  => array(
      'arguments' => array('item' => NULL, 'embed' => NULL, 'width' => NULL, 'height' => NULL, 'autoplay' => NULL, 'preview' => FALSE),
      'file' => 'providers/soundcloud.inc'
    )
  );
}
