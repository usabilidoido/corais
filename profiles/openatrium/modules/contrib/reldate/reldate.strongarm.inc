<?php

/**
 * Implementation of hook_strongarm().
 */
function reldate_strongarm() {
  $export = array();
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'date_format_day';
  $strongarm->value = 'l, M j';

  $export['date_format_day'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'date_format_month_after';
  $strongarm->value = 'g:ia M j, Y';

  $export['date_format_month_after'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'date_format_month_within';
  $strongarm->value = 'g:ia D M j';

  $export['date_format_month_within'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'date_format_time';
  $strongarm->value = 'g:ia';

  $export['date_format_time'] = $strongarm;
  return $export;
}
