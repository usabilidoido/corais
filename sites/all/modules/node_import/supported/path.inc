<?php

/**
 * @file
 * Support file for the path core module.
 */

/**
 * Implementation of hook_node_import_fields().
 */
function path_node_import_fields($type) {
  $fields = array();

  if (($node_type = node_import_type_is_node($type)) !== FALSE) {
    $fields['path'] = array(
      'title' => t('URL path settings'),
      'group' => t('URL path settings'),
      'module' => 'path',
      'weight' => 30,
      'is_mappable' => user_access('create url aliases'),
      'default_value' => '',
    );
  }

  return $fields;
}

/**
 * Implementation of hook_node_import_fields_alter().
 */
function path_node_import_fields_alter(&$fields, $type) {
  foreach ($fields as $fieldname => $fieldinfo) {
    if ($fieldinfo['input_format'] == 'node') {
      $fields[$fieldname]['preprocess'] = array('node_import_check_node_by_path') + $fieldinfo['preprocess'];
    }
  }
}

/**
 * Check a node by path alias.
 *
 * @ingroup node_import_preprocess
 */
function node_import_check_node_by_path(&$value, $field, $options, $preview) {
  // Value is either a path ("path/to/page") or an array (array("path", "to", "page")).
  $url = is_array($value) ? implode('/', $value) : $value;
  if (($result = db_result(db_query("SELECT src FROM {url_alias} WHERE dst = '%s' AND src LIKE 'node/%'", $url)))) {
    $value = substr($result, strlen('node/')); //TODO: what if an alias like 'node/XXX/...'
    return TRUE;
  }
}

