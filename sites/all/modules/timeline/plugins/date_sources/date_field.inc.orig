<?php

/**
 * @file
 * Definition of the date module integration
 */

$plugin = array(
  'name' => t('Date field'),
  'callback' => 'timeline_date_source_date_field_conversion',
  'handler_name' => 'date_handler_field_multiple',
);

/**
 * Converts different expected date formats to one global date format to talk to the timeline.
 * Respects drupal's timezone handling.
 *
 * @return
 *   date in defined output
 *
 * Notes: -fd
 * 1. Adding "Z" to ISO-Date. This is a "dirty" trick to disable timeline's automatic timezone detection of the browswer
 * 2. Using two date_convert functions to be able to respect timezones. This can be done better!
 * 3. Drupal's gobal timezone settings are taken. Not the date field specific settings.
 */
function timeline_date_source_date_field_conversion($date, $output_format, $options = array()) {

  $input_format = $options['field']['date_format'];

  switch ($input_format) {
    case 'date':
      switch ($output_format) {
        case 'iso8601':
          return date_convert(date_convert($date, DATE_ISO, DATE_UNIX), DATE_UNIX, DATE_ISO, date_get_timezone('date')) . 'Z';
        case 'gregorian':
          if (module_exists('date_api') && variable_get('date_api_version', 0) >= 5.2) {
            return format_date(date_convert($date, DATE_ISO, DATE_UNIX) + timeline_get_timezone(), 'custom', 'M d Y G:i:s', '0', 'en');
          }
          else {
            return format_date(strtotime($date) + timeline_get_timezone(), 'custom', 'M d Y G:i:s', '0', 'en');
          }
        case 'timestamp':
          return date_convert($date, DATE_ISO, DATE_UNIX) + timeline_get_timezone();
      }
    case 'datetime':
      switch ($output_format) {
        case 'iso8601':
          return date_convert(date_convert($date, DATE_DATETIME, DATE_UNIX), DATE_UNIX, DATE_ISO, date_get_timezone('date')) . 'Z';
        case 'gregorian':
          return format_date(date_convert($date, DATE_DATETIME, DATE_UNIX)  + timeline_get_timezone(), 'custom', 'M d Y G:i:s', '0', 'en');
        case 'timestamp':
          return date_convert($date, DATE_DATETIME, DATE_UNIX) + timeline_get_timezone();
      }
    case 'datestamp':
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
}
