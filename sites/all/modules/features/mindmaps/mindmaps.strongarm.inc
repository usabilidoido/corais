<?php

/**
 * Implementation of hook_strongarm().
 */
function mindmaps_strongarm() {
  $export = array();
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'og_content_type_usage_graphmind';
  $strongarm->value = 'group_post_standard';

  $export['og_content_type_usage_graphmind'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'og_max_groups_graphmind';
  $strongarm->value = '';

  $export['og_max_groups_graphmind'] = $strongarm;
  return $export;
}
