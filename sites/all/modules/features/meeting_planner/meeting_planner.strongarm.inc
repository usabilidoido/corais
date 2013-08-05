<?php

/**
 * Implementation of hook_strongarm().
 */
function meeting_planner_strongarm() {
  $export = array();

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_anonymous_meeting_planner';
  $strongarm->value = 0;
  $export['comment_anonymous_meeting_planner'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_controls_meeting_planner';
  $strongarm->value = '3';
  $export['comment_controls_meeting_planner'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_default_mode_meeting_planner';
  $strongarm->value = '4';
  $export['comment_default_mode_meeting_planner'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_default_order_meeting_planner';
  $strongarm->value = '2';
  $export['comment_default_order_meeting_planner'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_default_per_page_meeting_planner';
  $strongarm->value = '50';
  $export['comment_default_per_page_meeting_planner'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_form_location_meeting_planner';
  $strongarm->value = '1';
  $export['comment_form_location_meeting_planner'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_meeting_planner';
  $strongarm->value = '2';
  $export['comment_meeting_planner'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_preview_meeting_planner';
  $strongarm->value = '0';
  $export['comment_preview_meeting_planner'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_subject_field_meeting_planner';
  $strongarm->value = '0';
  $export['comment_subject_field_meeting_planner'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'content_extra_weights_meeting_planner';
  $strongarm->value = array(
    'title' => '-5',
    'body_field' => '-4',
    'revision_information' => '5',
    'author' => '4',
    'options' => '6',
    'comment_settings' => '7',
    'menu' => '0',
    'book' => '3',
    'path' => '8',
    'creativecommons' => '9',
    'locations' => '2',
    'og_nodeapi' => '1',
  );
  $export['content_extra_weights_meeting_planner'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'language_content_type_meeting_planner';
  $strongarm->value = '0';
  $export['language_content_type_meeting_planner'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_options_meeting_planner';
  $strongarm->value = array(
    0 => 'status',
    1 => 'promote',
  );
  $export['node_options_meeting_planner'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'og_content_type_usage_meeting_planner';
  $strongarm->value = 'group_post_wiki';
  $export['og_content_type_usage_meeting_planner'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'upload_meeting_planner';
  $strongarm->value = '0';
  $export['upload_meeting_planner'] = $strongarm;

  return $export;
}
