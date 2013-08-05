<?php

/**
 * Implementation of hook_strongarm().
 */
function spaces_test_strongarm() {
  $export = array();
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'og_content_type_usage_features_test';
  $strongarm->value = 'group_post_standard';

  $export['og_content_type_usage_features_test'] = $strongarm;
  return $export;
}
