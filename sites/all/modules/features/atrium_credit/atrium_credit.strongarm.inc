<?php

/**
 * Implementation of hook_strongarm().
 */
function atrium_credit_strongarm() {
  $export = array();

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_options_currency';
  $strongarm->value = array(
    0 => 'status',
    1 => 'promote',
  );
  $export['node_options_currency'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_options_exchange';
  $strongarm->value = array(
    0 => 'status',
  );
  $export['node_options_exchange'] = $strongarm;

  return $export;
}
