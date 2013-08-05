<?php

/**
 * Implementation of hook_data_default().
 */
function atrium_reader_data_default() {
  $export = array();

  $data_table = new stdClass;
  $data_table->disabled = FALSE; /* Edit this to true to make a default data_table disabled initially */
  $data_table->api_version = 1;
  $data_table->title = 'Reader feed';
  $data_table->name = 'feeds_data_feed_reader';
  $data_table->table_schema = array(
    'fields' => array(
      'feed_nid' => array(
        'type' => 'int',
        'size' => 'normal',
        'unsigned' => TRUE,
        'not null' => TRUE,
      ),
      'id' => array(
        'type' => 'serial',
        'size' => 'normal',
        'unsigned' => TRUE,
        'not null' => TRUE,
      ),
      'timestamp' => array(
        'type' => 'int',
        'unsigned' => TRUE,
        'not null' => FALSE,
      ),
      'title' => array(
        'type' => 'varchar',
        'size' => 'normal',
        'length' => '255',
        'not null' => FALSE,
      ),
      'description' => array(
        'type' => 'text',
        'size' => 'normal',
        'not null' => FALSE,
      ),
      'options_original_url' => array(
        'type' => 'text',
        'size' => 'normal',
        'not null' => FALSE,
      ),
      'options_raw_guid' => array(
        'type' => 'text',
        'size' => 'normal',
        'not null' => FALSE,
      ),
      'author' => array(
        'type' => 'text',
        'size' => 'normal',
      ),
    ),
    'name' => 'feeds_data_feed_reader',
    'indexes' => array(
      'feed_nid' => array(
        0 => 'feed_nid',
      ),
      'id' => array(
        0 => 'id',
      ),
      'timestamp' => array(
        0 => 'timestamp',
      ),
      'options_original_url' => array(
        0 => array(
          0 => 'options_original_url',
          1 => 255,
        ),
      ),
      'options_raw_guid' => array(
        0 => array(
          0 => 'options_raw_guid',
          1 => 255,
        ),
      ),
    ),
    'primary key' => array(
      0 => 'id',
    ),
  );
  $data_table->meta = array(
    'join' => array(
      'node' => array(
        'left_field' => 'nid',
        'field' => 'feed_nid',
        'inner_join' => TRUE,
      ),
    ),
    'fields' => array(
      'feed_nid' => array(
        'label' => '',
        'search' => 0,
        'views_field_handler' => 'views_handler_field_numeric',
        'views_filter_handler' => 'views_handler_filter_numeric',
        'views_argument_handler' => 'views_handler_argument_numeric',
        'views_sort_handler' => 'views_handler_sort',
      ),
      'id' => array(
        'label' => '',
        'search' => 0,
        'views_field_handler' => 'views_handler_field_numeric',
        'views_filter_handler' => 'views_handler_filter_numeric',
        'views_argument_handler' => 'views_handler_argument_numeric',
        'views_sort_handler' => 'views_handler_sort',
      ),
      'timestamp' => array(
        'label' => '',
        'views_field_handler' => 'views_handler_field_date',
        'views_filter_handler' => 'views_handler_filter_date',
        'views_argument_handler' => 'views_handler_argument_date',
        'views_sort_handler' => 'views_handler_sort_date',
      ),
      'title' => array(
        'label' => '',
        'search' => 1,
        'views_field_handler' => 'atrium_reader_handler_field_title',
        'views_filter_handler' => 'views_handler_filter_string',
        'views_argument_handler' => 'views_handler_argument',
        'views_sort_handler' => 'views_handler_sort',
      ),
      'description' => array(
        'label' => '',
        'search' => 1,
        'views_field_handler' => 'atrium_reader_handler_field_description',
        'views_filter_handler' => 'views_handler_filter_string',
        'views_argument_handler' => 'views_handler_argument',
        'views_sort_handler' => 'views_handler_sort',
      ),
      'options_original_url' => array(
        'label' => '',
        'search' => 0,
        'views_field_handler' => 'views_handler_field',
        'views_filter_handler' => 'views_handler_filter_string',
        'views_argument_handler' => 'views_handler_argument',
        'views_sort_handler' => 'views_handler_sort',
      ),
      'options_raw_guid' => array(
        'label' => '',
        'search' => 0,
        'views_field_handler' => 'views_handler_field',
        'views_filter_handler' => 'views_handler_filter_string',
        'views_argument_handler' => 'views_handler_argument',
        'views_sort_handler' => 'views_handler_sort',
      ),
      'author' => array(
        'label' => '',
        'views_field_handler' => 'views_handler_field',
        'views_filter_handler' => 'views_handler_filter_string',
        'views_argument_handler' => 'views_handler_argument',
        'views_sort_handler' => 'views_handler_sort',
      ),
    ),
  );
  $export['feeds_data_feed_reader'] = $data_table;

  return $export;
}
