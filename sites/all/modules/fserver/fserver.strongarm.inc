<?php

/**
 * Implementation of hook_strongarm().
 */
function fserver_strongarm() {
  $export = array();
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'content_extra_weights_fserver_project';
  $strongarm->value = array(
    'title' => '-5',
    'body_field' => '4',
    'revision_information' => '5',
    'author' => '7',
    'options' => '8',
    'comment_settings' => '10',
    'menu' => '-1',
    'taxonomy' => '-3',
    'path' => '9',
    'attachments' => '6',
  );

  $export['content_extra_weights_fserver_project'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'content_extra_weights_fserver_release';
  $strongarm->value = array(
    'title' => '-2',
    'body_field' => '0',
    'revision_information' => '2',
    'author' => '1',
    'options' => '3',
    'comment_settings' => '6',
    'menu' => '-1',
    'path' => '4',
    'attachments' => '5',
  );

  $export['content_extra_weights_fserver_release'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_options_fserver_release';
  $strongarm->value = array(
    0 => 'status',
  );

  $export['node_options_fserver_release'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'upload_fserver_release';
  $strongarm->value = '1';

  $export['upload_fserver_release'] = $strongarm;
  return $export;
}
