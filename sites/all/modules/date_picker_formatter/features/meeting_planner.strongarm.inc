<?php

/**
 * Implementation of hook_strongarm().
 */
function meeting_planner_strongarm() {
  $export = array();

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'atrium_activity_update_type_meeting_planner';
  $strongarm->value = 1;
  $export['atrium_activity_update_type_meeting_planner'] = $strongarm;

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
  $strongarm->value = '1';
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
  $strongarm->value = '0';
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
  $strongarm->value = '1';
  $export['comment_preview_meeting_planner'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_subject_field_meeting_planner';
  $strongarm->value = '1';
  $export['comment_subject_field_meeting_planner'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_upload_images_meeting_planner';
  $strongarm->value = 'none';
  $export['comment_upload_images_meeting_planner'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'comment_upload_meeting_planner';
  $strongarm->value = '1';
  $export['comment_upload_meeting_planner'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'content_extra_weights_meeting_planner';
  $strongarm->value = array(
    'title' => '-5',
    'body_field' => '-1',
    'revision_information' => '2',
    'author' => '1',
    'options' => '3',
    'comment_settings' => '4',
    'menu' => '-3',
    'book' => '0',
    'attachments' => '5',
    'og_nodeapi' => '-2',
  );
  $export['content_extra_weights_meeting_planner'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'enable_revisions_page_meeting_planner';
  $strongarm->value = 1;
  $export['enable_revisions_page_meeting_planner'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'language_content_type_meeting_planner';
  $strongarm->value = '0';
  $export['language_content_type_meeting_planner'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'location_defaultnum_meeting_planner';
  $strongarm->value = '1';
  $export['location_defaultnum_meeting_planner'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'location_maxnum_meeting_planner';
  $strongarm->value = '1';
  $export['location_maxnum_meeting_planner'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'location_settings_node_meeting_planner';
  $strongarm->value = array(
    'multiple' => array(
      'min' => '0',
      'max' => '1',
      'add' => '1',
    ),
    'form' => array(
      'weight' => 0,
      'collapsible' => 1,
      'collapsed' => 0,
      'fields' => array(
        'name' => array(
          'collect' => '1',
          'default' => '',
          'weight' => '2',
        ),
        'street' => array(
          'collect' => '1',
          'default' => '',
          'weight' => '4',
        ),
        'additional' => array(
          'collect' => '1',
          'default' => '',
          'weight' => '6',
        ),
        'city' => array(
          'collect' => '2',
          'default' => '',
          'weight' => '8',
        ),
        'province' => array(
          'collect' => '1',
          'default' => '',
          'weight' => '10',
        ),
        'postal_code' => array(
          'collect' => '1',
          'default' => '',
          'weight' => '12',
        ),
        'country' => array(
          'collect' => '1',
          'default' => 'de',
          'weight' => '14',
        ),
        'locpick' => array(
          'collect' => '0',
          'weight' => '20',
        ),
        'phone' => array(
          'collect' => '1',
          'default' => '',
          'weight' => '25',
        ),
        'fax' => array(
          'collect' => '1',
          'default' => '',
          'weight' => '30',
        ),
      ),
    ),
    'display' => array(
      'weight' => 0,
      'hide' => array(
        'locpick' => 'locpick',
        'province_name' => 'province_name',
        'country_name' => 'country_name',
        'coords' => 'coords',
        'name' => 0,
        'street' => 0,
        'additional' => 0,
        'city' => 0,
        'province' => 0,
        'postal_code' => 0,
        'country' => 0,
        'fax' => 0,
        'phone' => 0,
        'map_link' => 0,
      ),
      'teaser' => 1,
      'full' => 1,
    ),
    'rss' => array(
      'mode' => 'simple',
    ),
  );
  $export['location_settings_node_meeting_planner'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_options_meeting_planner';
  $strongarm->value = array(
    0 => 'status',
    1 => 'revision',
  );
  $export['node_options_meeting_planner'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'og_content_type_usage_meeting_planner';
  $strongarm->value = 'group_post_standard';
  $export['og_content_type_usage_meeting_planner'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'upload_meeting_planner';
  $strongarm->value = '1';
  $export['upload_meeting_planner'] = $strongarm;

  return $export;
}
