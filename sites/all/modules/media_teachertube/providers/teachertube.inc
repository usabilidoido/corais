<?php
// $Id: teachertube.inc,v 1.1 2010/05/08 01:41:43 dragonwize Exp $
/**
 * @file
 *   This include processes teachertube.com media files for use by emfield.module.
 */

define('EMVIDEO_TEACHERTUBE_MAIN_URL', 'http://teachertube.com/');

/**
 *  This defines the version of the content data array that we serialize
 *  in emvideo_teachertube_data(). If we change the expected keys of that array,
 *  we must increment this value, which will allow older content to be updated
 *  to the new version automatically.
 */
define('EMVIDEO_TEACHERTUBE_DATA_VERSION', 1);

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
function emvideo_teachertube_info() {
  $features = array(
    array(t('Autoplay'), t('No'), ''),
    array(t('RSS Attachment'), t('No'), ''),
    array(t('Thumbnails'), t('Yes'), t('')),
    array(t('Full screen mode'), t('Yes'), t('You may customize the player to enable or disable full screen playback. Full screen mode is enabled by default.')),
  );
  return array(
    'provider' => 'teachertube',
    'name' => t('TeacherTube'),
    'url' => EMVIDEO_TEACHERTUBE_MAIN_URL,
    'settings_description' => t('These settings specifically affect videos displayed from !teachertube.', array('!teachertube' => l(t('TeacherTube.com'), EMVIDEO_TEACHERTUBE_MAIN_URL))),
    'supported_features' => $features,
  );
}

/**
 * hook emvideo_PROVIDER_settings
 * this should return a subform to be added to the emvideo_settings() admin settings page.
 * note that a form field will already be provided, at $form['PROVIDER'] (such as $form['teachertube'])
 * so if you want specific provider settings within that field, you can add the elements to that form field.
 */
function emvideo_teachertube_settings() {
}

/**
 *  Implement hook emvideo_PROVIDER_data_version().
 */
function emvideo_teachertube_data_version() {
  return EMVIDEO_TEACHERTUBE_DATA_VERSION;
}

/**
 * hook emfield_PROVIDER_data
 *
 * provides an array to be serialised and made available with $item elsewhere
 */
function emvideo_teachertube_data($field, $item) {
  $data = array();
  // Create some 'field' version control.
  $data['emvideo_teachertube_version'] = EMVIDEO_TEACHERTUBE_DATA_VERSION;

  $video_id = emvideo_teachertube_extract($item['embed']);

  // Be nice to make this an array for changing media:thumbnail?
  $data['thumbnail'] = 'http://www.teachertube.com/files/videos/thumbnails/' . $video_id . 'T.jpg';
  $data['video_id']  = $video_id;

  return $data;
}

/**
 *
 */
function emvideo_teachertube_rss($item, $teaser = NULL) {
}

/**
 * hook emvideo_PROVIDER_extract
 * this is called to extract the video code from a pasted URL or embed code.
 * @param $embed
 *   an optional string with the pasted URL or embed code
 * @return
 *   either an array of regex expressions to be tested, or a string with the video code to be used
 *   if the hook tests the code itself, it should return either the string of the video code (if matched), or an empty array.
 *   otherwise, the calling function will handle testing the embed code against each regex string in the returned array.
 */
function emvideo_teachertube_extract($embed = '') {
  // http://www.teachertube.com/viewVideo.php?video_id=131919&title=Embed_code_procedure
  preg_match('@^.*\?video_id=([\d]+)@i', $embed, $matches);
  return $matches[1];
}

/**
 * hook emvideo_PROVIDER_embedded_link($video_code)
 * returns a link to view the video at the provider's site
 *  @param $video_id
 *    the string containing the video to watch
 *  @return
 *    a string containing the URL to view the video at the original provider's site
 */
function emvideo_teachertube_embedded_link($video_id) {
  return 'http://www.teachertube.com/viewVideo.php?video_id=' . $video_id;
}

/**
 * The embedded flash displaying the teachertube video.
 */
function theme_emvideo_teachertube_flash($video_id, $width, $height, $autoplay) {
  $output = '';
  if ($video_id) {
    $autoplay = $autoplay ? 'true' : 'false';
    $fullscreen = variable_get('emvideo_teachertube_full_screen', 1) ? 'true' : 'false';

    $output = '<embed src="http://www.teachertube.com/embed/player.swf"';
    $output .= ' width="' . $width . '"';
    $output .= ' height="' . $height . '"';
    $output .= ' bgcolor="transparent"';
    $output .= ' wmode="opaque"';
    $output .= ' allowscriptaccess="always"';
    $output .= ' allowfullscreen="' . $fullscreen . '"';
    $output .= ' flashvars="file=http://www.teachertube.com/embedFLV.php?pg=video_' . $video_id . '&menu=false&frontcolor=ffffff&lightcolor=FF0000&logo=http://www.teachertube.com/www3/images/greylogo.swf&skin=http://www.teachertube.com/embed/overlay.swf&volume=80&controlbar=over&displayclick=link&viral.link=http://www.teachertube.com/viewVideo.php?video_id=' . $video_id . '&stretching=exactfit&plugins=viral-2&viral.callout=none&viral.onpause=false" />';
  }
  return $output;
}

/**
 * hook emvideo_PROVIDER_thumbnail
 * returns the external url for a thumbnail of a specific video
 * TODO: make the args: ($embed, $field, $item), with $field/$item provided if we need it, but otherwise simplifying things
 *  @param $field
 *    the field of the requesting node
 *  @param $item
 *    the actual content of the field from the requesting node
 *  @return
 *    a URL pointing to the thumbnail
 */
function emvideo_teachertube_thumbnail($field, $item, $formatter, $node, $width, $height) {
  return 'http://www.teachertube.com/files/videos/thumbnails/' . $item['data']['video_id'] . 'T.jpg';
}

/**
 * hook emvideo_PROVIDER_video
 * this actually displays the full/normal-sized video we want, usually on the default page view
 *  @param $embed
 *    the video code for the video to embed
 *  @param $width
 *    the width to display the video
 *  @param $height
 *    the height to display the video
 *  @param $field
 *    the field info from the requesting node
 *  @param $item
 *    the actual content from the field
 *  @return
 *    the html of the embedded video
 */
function emvideo_teachertube_video($embed, $width, $height, $field, $item, &$node, $autoplay) {
  $output = theme('emvideo_teachertube_flash', $item['data']['video_id'], $width, $height, $autoplay);
  return $output;
}

/**
 * hook emvideo_PROVIDER_video
 * this actually displays the preview-sized video we want, commonly for the teaser
 *  @param $embed
 *    the video code for the video to embed
 *  @param $width
 *    the width to display the video
 *  @param $height
 *    the height to display the video
 *  @param $field
 *    the field info from the requesting node
 *  @param $item
 *    the actual content from the field
 *  @return
 *    the html of the embedded video
 */
function emvideo_teachertube_preview($embed, $width, $height, $field, $item, &$node, $autoplay) {
  $output = theme('emvideo_teachertube_flash', $item['data']['video_id'], $width, $height, $autoplay);
  return $output;
}

/**
 * Implementation of hook_emfield_subtheme.
 */
function emvideo_teachertube_emfield_subtheme() {
  $themes = array(
      'emvideo_teachertube_flash'  => array(
          'arguments' => array('video_id' => NULL, 'width' => NULL, 'height' => NULL, 'autoplay' => NULL),
          'file' => 'providers/teachertube.inc',
          'path' => drupal_get_path('module', 'media_teachertube'),
      )
  );
  return $themes;
}
