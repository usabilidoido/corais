<?php
// $Id: facebook.inc,v 1.1 2010/12/06 13:45:46 njt1982 Exp $

/**
 * Implement hook_PROVIDER_info()
 */
function emimage_facebook_info() {
  // Define the provider
  return array(
    'provider' => 'facebook',
    'name' => t('Facebook'),
    'url' => 'http://www.facebook.com/',
    'settings_description' => t('These settings affect the Facebook images displayed on this site'),
    'supported_features' => array(),
  );
}


/**
 * Implement hook_PROVIDER_settings()
 */
function emimage_facebook_settings() {
  $form = array();
  // TODO: If needed?
  return $form;
}


/**
 * Implement hook_PROVIDER_data()
 */
function emimage_facebook_data($field, $item) {
  // Query facebook for the photo
  $result = fbconnect_graph_query($item['value']);

  // TODO: Check that the result actually IS a photo - not an ID for a profile or something
  
  // Define data to be stored
  $data = array(
    'id' => $result['id'],
    'owner' => $result['from']['id'],
    'title' => $result['name'],
    'images' => $result['images'],
  );

  return $data;
}


/**
 * Implement hook_PROVIDER_extract()
 */
function emimage_facebook_extract($embed = '') {
  // Example: http://www.facebook.com/photo.php?fbid=10150355982925301&set=a.103760510300.188470.508160300
  // TODO: Whats about pid/id URL's?
  return array('@photo\.php\?.*fbid=(\d+)@i');
}


/**
 * Implement hook_PROVIDER_embedded_link()
 */
function emimage_facebook_embedded_link($code, $data = array()) {
  return 'http://www.facebook.com/photo.php?fbid='. $data['id'];
}

/**
 * Implement hook_PROVIDER_image_url()
 */
function emimage_facebook_image_url($code, $width, $height, $formatter = NULL, $field = NULL, $item = NULL, $node = NULL) {
  // TODO: Get a better system for finding an appropriate photo size.
  return $item['data']['images'][1]['source'];
}

/**
 * Implement hook_PROVIDER_image_title()
 */
function emimage_facebook_image_title($code, $data) {
  return $data['title']; 
}
