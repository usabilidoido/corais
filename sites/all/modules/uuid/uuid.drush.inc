<?php

/**
 * @file
 * Drush implementation for the uuid module.
 */

/**
 * Implementation of hook_drush_command().
 */
function uuid_drush_command() {
  $items = array();
  $items['uuid-create-missing'] = array(
    'description' => 'Create missing UUIDs for node types specified for automatic generation',
    'arguments' => array(
      'type' => 'Constrain creation of UUIDs to nodes of a certain type.',
    ),
    'aliases' => array('uuid-create'),
  );
  return $items;
}

/**
 * Implementation of hook_drush_help().
 */
function uuid_drush_help($section) {
  switch ($section) {
    case 'drush:uuid-create-missing':
      return dt("This command will create missing UUIDs for those content types specified in the module settings for automatic generation.");
  }
}

/**
 * Drush command callback.
 */
function drush_uuid_create_missing() {
  if (!drush_confirm(dt('Are you sure?'))) {
    return drush_user_abort();
  }
  module_load_include('inc', 'uuid', 'uuid.admin');

  if ($args = func_get_args()) {
    $validate_result = _uuid_validate_specified_types($args);
    if (!$validate_result[0]) {
      drush_log(dt($validate_result[1]), 'error');
      return;
    }
  }
  drush_log(dt('Beginning bulk creation of UUIDs.'), 'ok');
  uuid_sync($args);
}

function _uuid_validate_specified_types($args) {
  $types = variable_get('uuid_automatic_for_nodes', array());
  // Remove disabled node types.
  $types = array_filter($types);

  foreach($args as $arg) {
    if (!in_array($arg, $types)) {
      return array(FALSE, 'One or more of the content types you specified are not selected for automatic UUID generation.');
    }
  }
  return array(TRUE);
}
