<?php
// $Id: purl.api.php,v 1.1.2.2 2010/03/17 22:51:54 yhahn Exp $

/**
 * @file
 * Hooks provided by PURL.
 */

/**
 * Registry hook for PURL providers. Should return a keyed array where each key
 * is a provider identifier and each value is a sub-array of information for
 * the the provider. Possible keys:
 *    "name": the human-readable name of the provider. Should be wrapped in
 *      t() if localization is desired.
 *    "description": description of the provider. Should be wrapped in t()
 *      if localization is desired.
 *    "callback": String. Callback function to be called when a modifier for
 *      this provider is found in a page request.
 *    "callback arguments": Optional. Array of arguments to supply to the
 *      callback function. Note that the modifier ID is always provided as the
 *      *last* argument to the callback function.
 *    "example": String. Example modifier to be displayed to administrators.
 */
function hook_purl_provider() {
  return array(
    'example_provider' => array(
      'name' => t('Example provider'),
      'description' => t('Sets a message for the the retrieved ID.'),
      'callback' => 'drupal_set_message',
      'example' => 'foobar',
    ),
  );
}

/**
 * Optional hook for providers who use custom storage for modifiers. Should
 * return a keyed array where each key is a provider identifier and each value
 * is a sub-array of modifiers with modifier value and ID.
 */
function hook_purl_modifiers() {
  return array(
    'example_provider' => array(
      array('value' => 'foo', 'id' => 1),
      array('value' => 'bar', 'id' => 2),
    ),
  );
}

/**
 * CTools plugin API hook for PURL. Note that a proper entry in
 * hook_ctools_plugin_api() must exist for this hook to be called.
 */
function hook_purl_processor() {
  $plugins = array();
  $plugins['cookie'] = array(
    'handler' => array(
      'path' => drupal_get_path('module', 'foo') .'/plugins',
      'file' => 'purl_cookie.inc',
      'class' => 'purl_cookie',
    ),
  );
  return $plugins;
}
