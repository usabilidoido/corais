<?php

/**
 * @file
 * Hooks provided by Spaces.
 */

/**
 * CTools plugin API hook for Spaces. Note that a proper entry in
 * hook_ctools_plugin_api() must exist for this hook to be called.
 */
function hook_spaces_plugins() {
  $plugins = array();
  $plugins['space_foo'] = array(
    'handler' => array(
      'path' => drupal_get_path('module', 'foo') .'/plugins',
      'file' => 'space_foo.inc',
      'class' => 'space_foo',
      'parent' => 'space_type',
    ),
  );
  return $plugins;
}

/**
 * Registry hook for controllers, space types.
 *
 * Each entry associates a controller or space type with the CTools plugin to
 * be used as its plugin class. For space types,  a path may be provided where
 * spaces-specific menu items can be automatically grafted in as local tasks.
 */
function hook_spaces_registry() {
  return array(
    'controllers' => array(
      'object_type' => array(
        'title' => t('Name of the object type'),
        'plugin' => 'spaces_controller_object_type',
      ),
    ),
    'types' => array(
      'space_foo' => array(
        'title' => t('Name of the space type'),
        'plugin' => 'space_foo',
        'path' => 'foo/%foo',
      ),
    ),
  );
}

/**
 * Alter the registry.
 *
 * Allows modules to alter the registry. Default plugins can be replaced by
 * custom ones declared in hook_spaces_plugins().
 *
 * @param $registry
 *   The registry, passed by reference.
 */
function hook_spaces_registry_alter(&$registry) {
  if (isset($registry['controllers']['variable'])) {
    $registry['controllers']['variable']['plugin'] = 'custom_controller_plugin';
  }
}
