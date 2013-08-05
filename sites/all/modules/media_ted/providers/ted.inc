<?php

/**
 * @file
 *   This include processes TED video for use by the emfield.module
 *
 *  TODO parse default language attributes for the video embed
 *  TODO return proper original URL
 */

define('EMVIDEO_TED_MAIN_URL', 'http://ted.com/');
define('EMVIDEO_TED_PLAYER_URL', 'http://video.ted.com/assets/player/swf/EmbedPlayer.swf');
define('EMVIDEO_TED_ID_EMBED_URL', 'http://www.ted.com/talks/embed/id/');
define('EMVIDEO_TED_ID_VIEW_URL', 'http://ted.com/talks/view/id/');
define('EMVIDEO_TED_VIDEO_BASE', 'http://video.ted.com/');

define('EMVIDEO_TED_DATA_VERSION', 3);

/**
 * FIXME avoid using the global variable
 * This is a global variable used to keep the extra info (incl. thumbnail url).
 * Cannot keep the full embed string because of the way TED video URLs work.
 * Otherwise there are conflicts with modal display of the video, inline view or
 * emthumb
 */
global $_media_ted_extra;

/**
 * hook emvideo_PROVIDER_info
 * this returns information relevant to a specific 3rd party video provider
 * @return
 *   an array of strings requested by various admin and other forms
 *   'name' => the translated name of the provider
 *   'url' => the url to the main page for the provider
 *   'settings_description' => a description of the provider that will be posted in the admin settings form
 *   'supported_features' => an array of rows describing the state of certain supported features by the provider.
 *      These will be rendered in a table, with the columns being 'Feature', 'Supported', 'Notes'.
 */
function emvideo_ted_info() {
  $features = array(
    array(t('Autoplay'), t('Yes'), ''),
    //array(t('RSS Attachment'), t('Yes'), ''),
    array(t('Thumbnails'), t('Yes'), t('')),
    array(
      t('Full screen mode'),
      t('Yes'),
      t('You may customize the player to enable or disable full screen playback. Full screen mode is enabled by default.')),
  );
  return array(
    'provider' => 'ted',
    'name' => t('TED'),
    'url' => EMVIDEO_TED_MAIN_URL,
    'settings_description' => t('These settings specifically affect videos displayed from !provider.', array('!provider' => l(t('TED.com'), EMVIDEO_TED_MAIN_URL))),
    'supported_features' => $features,
  );
}

/**
 *  hook emvideo_PROVIDER_settings
 */
function emvideo_ted_settings() {
  $form['ted']['player_options'] = array(
    '#type' => 'fieldset',
    '#title' => t('Embedded video player options'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  // This is an option to set the video to full screen.
  $form['ted']['player_options']['emvideo_ted_full_screen'] = array(
    '#type' => 'checkbox',
    '#title' => t('Allow fullscreen'),
    '#default_value' => variable_get('emvideo_ted_full_screen', 1),
    '#description' => t('Allow users to view video using the entire computer screen.'),
  );

  return $form;
}

/**
 * hook emvideo_PROVIDER_extract
 * this is called to extract the video code from a pasted URL or embed code.
 * @param $code
 *   an optional string with the pasted URL or embed code
 * @return
 *   either an array of regex expressions to be tested, or a string with the video code to be used
 *   if the hook tests the code itself, it should return either the string of the video code (if matched), or an empty array.
 *   otherwise, the calling function will handle testing the embed code against each regex string in the returned array.
 */
function emvideo_ted_extract($embed = '') {

  // [ted id=319] --> http://www.ted.com/talks/embed/id/319
  if ($embed && preg_match('@\[ted id=([0-9]+)\]@i', $embed, $matches)) {
    $result = drupal_http_request(EMVIDEO_TED_ID_EMBED_URL . $matches[1]);
    if ($result->code == 200) {
      $embed = $result->data;
    }
  }
  else if (strpos($embed, 'http://www.ted.com/talks/') === 0 && valid_url($embed, TRUE)) {
  // The user provided the direct url to the page.
  // The embed code resides in an input with id = embedCode
    $result = drupal_http_request($embed);
    if ($result->code == 200) {
      $pattern = '@<input id=[\"\']embedCode[\"\'].*value=[\"\'](.*?[^\"\'])[\"\']@i';

      // Do not update the embed code if the embedCode input is not found,
      if (preg_match($pattern, $result->data, $matches) && $matches[1]) {
        $embed = html_entity_decode($matches[1]);
      }
    }
  }

  if ($embed && preg_match('@video\.ted\.com\/(talk.*?[^&])\&([^\"\']+)@i', $embed, $matches)) {
    global $_media_ted_extra;
    $_media_ted_extra = $matches[2];

    return $matches[1];
  }

  return FALSE;
}

/**
 * hook emfield_PROVIDER_data
 *
 * provides an array to be serialised and made available with $item elsewhere
 */
function emvideo_ted_data($field, $item) {
  // Initialize the data array.
  $data = array();
  $data['emvideo_ted_version'] = EMVIDEO_TED_DATA_VERSION;

  // Gather info about the item's raw flash video.
  $url = EMVIDEO_TED_PLAYER_URL . EMVIDEO_TED_VIDEO_BASE . $item['value'];

  // add the extra bits to get a proper response
  global $_media_ted_extra;
  if (!empty($_media_ted_extra)) {
    $url .= "&$_media_ted_extra";
  }

  $response = emfield_request_header('ted', $url);

  if ($response->code == 200) {
    // Don't give the 303 path.
    $data['flash']['url'] = $url;
    $data['flash']['size'] = $response->headers['Content-Length'];
    $data['flash']['mime'] = $response->headers['Content-Type'];
  }

  // Save the thumbnail URL, if set + the extra bits
  if (!empty($_media_ted_extra) &&
      preg_match('@su=(http[s]?://.*\.(gif|png|jpg))(.*)@i', $_media_ted_extra, $matches)) {
    $data['thumbnail']['url'] = $matches[1];
    $data['extra'] = $matches[3];
  }

  unset($_media_ted_extra);
  return $data;
}

/**
 *  hook emvideo_PROVIDER_rss
 *
 *  This attaches a file to an RSS feed.
 */
/*/
function emvideo_ted_rss($item, $teaser = NULL) {

  if ($item['value']) {
    $file['thumbnail']['filepath'] = $item['data']['thumbnail']['url'];

    return $file;
  }
}
//*/

/**
 * hook emvideo_PROVIDER_embedded_link($video_code)
 * returns a link to view the video at the provider's site.
 *  @param $video_code
 *    The string containing the video to watch.
 *  @return
 *    A string containing the URL to view the video @ the original provider site.
 */
function emvideo_ted_embedded_link($video_code, $data) {
  if (isset($data['extra']) && preg_match('/ti=([0-9]+)/', $data['extra'], $matches)) {
    return EMVIDEO_TED_ID_VIEW_URL . $matches[1];
  }
}

/**
 * hook emvideo_PROVIDER_thumbnail
 * Returns the external url for a thumbnail of a specific video.
 *  @param $field
 *    The field of the requesting node.
 *  @param $item
 *    The actual content of the field from the requesting node.
 *  @return
 *    A URL pointing to the thumbnail.
 */
function emvideo_ted_thumbnail($field, $item, $formatter, $node, $width, $height) {
  if (isset($item['data']['thumbnail']['url']))  {
    return $item['data']['thumbnail']['url'];
  }
}

/**
 *  hook emvideo_PROVIDER_video
 *  This actually displays the full/normal-sized video we want, usually on the
 *  default page view.
 *  @param $embed
 *    The video code for the video to embed.
 *  @param $width
 *    The width to display the video.
 *  @param $height
 *    The height to display the video.
 *  @param $field
 *    The field info from the requesting node.
 *  @param $item
 *    The actual content from the field.
 *  @return
 *    The html of the embedded video.
 */
function emvideo_ted_video($embed, $width, $height, $field, $item, $node, $autoplay) {
  $output = theme('emvideo_ted_flash', $item, $width, $height, $autoplay);
  return $output;
}

/**
 *  hook emvideo_PROVIDER_video
 *
 *  This actually displays the preview-sized video we want, commonly for the
 *  teaser.
 *  @param $embed
 *    The video code for the video to embed.
 *  @param $width
 *    The width to display the video.
 *  @param $height
 *    The height to display the video.
 *  @param $field
 *    The field info from the requesting node.
 *  @param $item
 *    The actual content from the field.
 *  @return
 *    The html of the embedded video.
 */
function emvideo_ted_preview($embed, $width, $height, $field, $item, $node, $autoplay) {
  $output = theme('emvideo_ted_flash', $item, $width, $height, $autoplay);
  return $output;
}

/**
 * The embedded flash displaying the TED video.
 */
function theme_emvideo_ted_flash($item, $width, $height, $autoplay) {
  $output = '';

  if ($item['embed']) {
    $thumbnail  = isset($item['data']['thumbnail']['url']) ?
                  '&su=' . $item['data']['thumbnail']['url'] : '';
    $extra      = isset($item['data']['extra']) ? $item['data']['extra'] : '';

    // a bit of backwards compatibility here
    $vars        = $item['value'];

    if (strpos($vars, '.flv') === FALSE && strpos($vars, 'mp4') === FALSE) {
      $vars = 'vu='. EMVIDEO_TED_VIDEO_BASE .'talks/dynamic/'
              . $item['value'] .'.flv'. $thumbnail . $extra;
    }
    else if (strpos($item['value'], 'vu=') === FALSE) {
      $vars = 'vu='. EMVIDEO_TED_VIDEO_BASE . $item['value'] . $thumbnail . $extra;
    }

    $movie_url  = EMVIDEO_TED_PLAYER_URL;

    $autoplay   = $autoplay ? 'true' : 'false';
    $fullscreen = variable_get('emvideo_ted_full_screen', 1) ? 'true' : 'false';

    // Let's resize properly and account for smaller player sizes
    // TODO keep aspect ratio
    $w    = round($width * 0.975);
    $h    = $height - 85;
    $vars = preg_replace('/sz=[0-9]{2,4}x[0-9]{2-4};/', "sz={$w}x{$h};", $vars);
    $vars = preg_replace('/vw=[0-9]{2,4}/', "vw=$w", $vars);
    $vars = preg_replace('/vh=[0-9]{2,4}/', "vh=$h", $vars);

    $output = <<<EOD
<object width="$w" height="$height">
  <param name="movie" value="$movie_url"></param>
  <param name="allowFullScreen" value="$fullscreen" />
  <param name="allowScriptAccess" value="always"/>
  <param name="wmode" value="transparent"></param>
  <param name="bgColor" value="#ffffff"></param>
  <param name="flashvars" value="$vars" />
  <embed src="$movie_url" pluginspace="http://www.macromedia.com/go/getflashplayer" type="application/x-shockwave-flash" wmode="transparent" bgColor="#ffffff" width="$width" height="$height" allowFullScreen="$fullscreen" allowScriptAccess="always" flashvars="$vars"></embed>
</object>
EOD;
  }

  return $output;
}

/**
 * Implementation of hook_emfield_subtheme().
 * This returns any theme functions defined by this provider.
 */
function emvideo_ted_emfield_subtheme() {
  $themes = array(
    'emvideo_ted_flash'  => array(
      'arguments' => array('item' => NULL, 'width' => NULL, 'height' => NULL,
        'autoplay' => NULL),
        'file' => 'providers/ted.inc',
        'path' => drupal_get_path('module', 'media_ted'),
      )
  );
  return $themes;
}

/**
 * Implementation of hook_emvideo_PROVIDER_content_generate().
 */
function emvideo_ted_content_generate() {
  return array(
  // flv
    '[ted id=2]',
    '[ted id=219]',
    '[ted id=316]',
    '[ted id=65]',
  // mp4
    '[ted id=1110]',
    '[ted id=1135]',
    '[ted id=1142]'
  );
}