<?php

/**
 * @file
 * Documentation of hooks.
 */

/**
 * Invoked after a data record has been inserted.
 */
function hook_data_insert($record, $table_name) {
}

/**
 * Invoked after a data record has been updated.
 */
function hook_data_update($record, $table_name) {
}

/**
 * Invoked before a delete. Add to/remove from a delete query before deleting.
 */
function hook_data_delete_query_alter(&$query, $table_name) {
}

/**
 * Expose default tables.
 *
 * Note:
 *
 * - Implementor is responsible for creating this table on installation and for
 *   proper updates in case of schema changes (hook_install(), hook_update_N())
 * - Implement hook_ctools_plugin_api() to make this hook discoverable by
 *   CTools - see below.
 */
function hook_data_default() {
  $export = array();
  $data_table = new stdClass;
  $data_table->disabled = FALSE; /* Edit this to true to make a default data_table disabled initially */
  $data_table->api_version = 1;
  $data_table->title = 'Example';
  $data_table->name = 'data_table_example';
  $data_table->table_schema = array(
    'fields' => array(
      'id' => array(
        'type' => 'int',
        'size' => 'normal',
        'unsigned' => TRUE,
      ),
    ),
    'primary key' => array(
      '0' => 'id',
    ),
  );
  $data_table->meta = array(
    'fields' => array(
      'id' => array(
        'label' => 'Identifier',
      ),
    ),
  );

  $export['data_table_example'] = $data_table;
  return $export;
}

/**
 * Example for a CTools Plugin API implementation for hook_data_default().
 */
function hook_ctools_plugin_api() {
  $args = func_get_args();
  $module = array_shift($args);
  $api = array_shift($args);
  if ($module == "data" && $api == "data_default") {
    return array("version" => 1);
  }
}