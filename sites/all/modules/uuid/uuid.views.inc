<?php

/**
 * @file
 * Views Implementation for UUID.
 */

/**
 * Implementation of hook_views_data().
 */
function uuid_views_data() {
  $data = array();

  module_load_include('install', 'uuid');
  $tables = uuid_schema();

  foreach ($tables as $table => $schema) {
    $base_table = str_replace('uuid_', '', $table);

    $field_keys = array_keys($schema['fields']);
    $field = array_shift($field_keys);

    $data[$table]['table'] = array(
      'group' => 'UUID',
      'join' => array(
        $base_table => array(
          'left_field' => $field,
          'field' => $field,
        ),
      )
    );

    $data[$table]['uuid'] = array(
      'title' => t('@table UUID', array('@table' => ucwords($base_table))),
      'help' => t(
        'Universally Unique Identifier for @table objects.',
        array('@table' => ucwords($base_table))
      ),
      'field' => array(
        'handler' => 'views_handler_field',
        'click sortable' => TRUE,
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_string',
        'allow empty' => TRUE,
        'help' => 'Filter on a UUID',
      ),
      'argument' => array(
        'handler' => 'views_handler_argument_string',
        'help' => 'Argument on a UUID',
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
        'help' => 'Sort on UUID',
      ),
    );
  }

  return $data;
}
