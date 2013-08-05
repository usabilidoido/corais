<?php

/**
 * Implementation of hook_strongarm().
 */
function grayside_oa_feeds_strongarm() {
  $export = array();
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'tokenauth_length';
  $strongarm->value = '23';

  $export['tokenauth_length'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'tokenauth_pages';
  $strongarm->value = 'dashboard/feed*
reader/feed*
';

  $export['tokenauth_pages'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'tokenauth_text';
  $strongarm->value = array(
    'body' => 'These feeds use part of the URL to log you into your account for limited purposes.
If your URL \'token\' has been stolen press the reset button to get a new token. ',
  );

  $export['tokenauth_text'] = $strongarm;
  return $export;
}
