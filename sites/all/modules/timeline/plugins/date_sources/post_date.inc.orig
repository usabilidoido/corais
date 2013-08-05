<?php
/**
 * @file
 * Implementation of the post date plugin
 */


$plugin = array(
  'name' => t('Post date'),
  'callback' => 'timeline_date_source_post_date_conversion',
  'handler_name' => 'views_handler_field_date',
    'field_type' => 'date',
);


function timeline_date_source_post_date_conversion($date, $output_format, $options = array()) {

  $input_format = 'timestamp';
  switch ($output_format) {
    case 'iso8601':
      if (module_exists('date_api') && variable_get('date_api_version', 0) >= 5.2) {
        return date_convert($date, DATE_UNIX, DATE_ISO, date_get_timezone('date')) . 'Z';
      }
      else {
        return format_date($date + timeline_get_timezone(), 'custom', 'Y-m-d\TH:i:s\Z', '0', 'en');
      }
    case 'gregorian':
      return format_date($date + timeline_get_timezone(), 'custom', 'M d Y G:i:s', '0', 'en');
    case 'timestamp':
      return $date + timeline_get_timezone();
  }
}


