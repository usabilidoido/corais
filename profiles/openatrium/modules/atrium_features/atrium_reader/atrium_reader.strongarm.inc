<?php

/**
 * Implementation of hook_strongarm().
 */
function atrium_reader_strongarm() {
  $export = array();

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_feed_reader';
  $strongarm->value = '0';
  $export['comment_feed_reader'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_options_feed_reader';
  $strongarm->value = array(
    0 => 'status',
  );
  $export['node_options_feed_reader'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'og_content_type_usage_feed_reader';
  $strongarm->value = 'group_post_standard';
  $export['og_content_type_usage_feed_reader'] = $strongarm;

  return $export;
}
