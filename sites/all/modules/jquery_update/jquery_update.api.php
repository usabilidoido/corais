<?php

/**
 * Perform necessary alterations to the JavaScript before it is presented on
 * the page.
 *
 * @param $javascript
 *   An array of all JavaScript being presented on the page. An associative
 *   array of scopes, file paths, and then options associated with the files.
 *
 * @see drupal_add_js()
 * @see drupal_get_js()
 * @see hook_js_alter()
 */
function hook_jquery_update_alter(&$javascript) {
  // Retrieve the attributes about jQuery.
  $jquery = $javascript['core']['misc/drupal.js'];
  // Remove the old one.
  unset($javascript['core']['misc/drupal.js']);

  // Swap in the new one.
  $path = drupal_get_path('module', 'jquery_update');
  $scripts['core'][$path] = $jquery;
}
