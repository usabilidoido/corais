<?php

/**
 * Helper to implementation of hook_content_default_fields().
 */
function _meetings_content_default_fields() {
  $fields = array();

  // Exported field: field_meeting_agenda
  $fields[] = array(
    'field_name' => 'field_meeting_agenda',
    'type_name' => 'meeting',
    'display_settings' => array(
      'weight' => '-1',
      'parent' => '',
      'label' => array(
        'format' => 'above',
      ),
      'teaser' => array(
        'format' => 'default',
        'exclude' => 0,
      ),
      'full' => array(
        'format' => 'default',
        'exclude' => 0,
      ),
      '4' => array(
        'format' => 'default',
        'exclude' => 0,
      ),
    ),
    'widget_active' => '1',
    'type' => 'text',
    'required' => '0',
    'multiple' => '0',
    'module' => 'text',
    'active' => '1',
    'text_processing' => '1',
    'max_length' => '',
    'allowed_values' => '',
    'allowed_values_php' => '',
    'widget' => array(
      'rows' => '5',
      'size' => 60,
      'default_value' => array(
        '0' => array(
          'value' => '* Intros
* Old Business
* New Business
* Next Steps',
          'format' => '1',
          '_error_element' => 'default_value_widget][field_meeting_agenda][0][value',
        ),
      ),
      'default_value_php' => NULL,
      'label' => 'Agenda',
      'weight' => '-1',
      'description' => 'Items to be covered at the meeting. Should be built up in advance of the meeting.',
      'type' => 'text_textarea',
      'module' => 'text',
    ),
  );

  // Exported field: field_meeting_attachments
  $fields[] = array(
    'field_name' => 'field_meeting_attachments',
    'type_name' => 'meeting',
    'display_settings' => array(
      'weight' => '1',
      'parent' => '',
      'label' => array(
        'format' => 'above',
      ),
      'teaser' => array(
        'format' => 'default',
        'exclude' => 1,
      ),
      'full' => array(
        'format' => 'default',
        'exclude' => 0,
      ),
      '4' => array(
        'format' => 'default',
        'exclude' => 0,
      ),
    ),
    'widget_active' => '1',
    'type' => 'filefield',
    'required' => '0',
    'multiple' => '1',
    'module' => 'filefield',
    'active' => '1',
    'list_field' => '0',
    'list_default' => 1,
    'description_field' => '1',
    'widget' => array(
      'file_extensions' => '',
      'file_path' => 'meeting-attachments',
      'progress_indicator' => 'bar',
      'max_filesize_per_file' => '',
      'max_filesize_per_node' => '',
      'label' => 'Attachments',
      'weight' => '1',
      'description' => 'Upload documents and files related to this meeting',
      'type' => 'filefield_widget',
      'module' => 'filefield',
    ),
  );

  // Exported field: field_meeting_date
  $fields[] = array(
    'field_name' => 'field_meeting_date',
    'type_name' => 'meeting',
    'display_settings' => array(
      'weight' => '-3',
      'parent' => '',
      'label' => array(
        'format' => 'inline',
      ),
      'teaser' => array(
        'format' => 'default',
        'exclude' => 0,
      ),
      'full' => array(
        'format' => 'default',
        'exclude' => 0,
      ),
      '4' => array(
        'format' => 'default',
        'exclude' => 0,
      ),
      'token' => array(
        'format' => 'default',
        'exclude' => 0,
      ),
    ),
    'widget_active' => '1',
    'type' => 'date',
    'required' => '0',
    'multiple' => '0',
    'module' => 'date',
    'active' => '1',
    'granularity' => array(
      'year' => 'year',
      'month' => 'month',
      'day' => 'day',
      'hour' => 'hour',
      'minute' => 'minute',
    ),
    'timezone_db' => 'UTC',
    'tz_handling' => 'site',
    'todate' => 'optional',
    'repeat' => 0,
    'repeat_collapsed' => '',
    'default_format' => 'long',
    'widget' => array(
      'default_value' => 'strtotime',
      'default_value_code' => '+1 week',
      'default_value2' => 'strtotime',
      'default_value_code2' => '+1 week 1 hour',
      'input_format' => 'm/d/Y - g:ia',
      'input_format_custom' => '',
      'increment' => '5',
      'text_parts' => array(),
      'year_range' => '-3:+3',
      'label_position' => 'above',
      'label' => 'Date',
      'weight' => '-3',
      'description' => 'Enter the date & time when this meeting will occur. ',
      'type' => 'date_popup',
      'module' => 'date',
    ),
  );

  // Exported field: field_meeting_location
  $fields[] = array(
    'field_name' => 'field_meeting_location',
    'type_name' => 'meeting',
    'display_settings' => array(
      'weight' => '-2',
      'parent' => '',
      'label' => array(
        'format' => 'inline',
      ),
      'teaser' => array(
        'format' => 'default',
        'exclude' => 0,
      ),
      'full' => array(
        'format' => 'default',
        'exclude' => 0,
      ),
      '4' => array(
        'format' => 'default',
        'exclude' => 0,
      ),
    ),
    'widget_active' => '1',
    'type' => 'text',
    'required' => '0',
    'multiple' => '0',
    'module' => 'text',
    'active' => '1',
    'text_processing' => '0',
    'max_length' => '',
    'allowed_values' => '',
    'allowed_values_php' => '',
    'widget' => array(
      'rows' => 5,
      'size' => '60',
      'default_value' => array(
        '0' => array(
          'value' => '',
          '_error_element' => 'default_value_widget][field_meeting_location][0][value',
        ),
      ),
      'default_value_php' => NULL,
      'label' => 'Location',
      'weight' => '-2',
      'description' => '',
      'type' => 'text_textfield',
      'module' => 'text',
    ),
  );

  // Exported field: field_meeting_notes
  $fields[] = array(
    'field_name' => 'field_meeting_notes',
    'type_name' => 'meeting',
    'display_settings' => array(
      'weight' => 0,
      'parent' => '',
      'label' => array(
        'format' => 'above',
      ),
      'teaser' => array(
        'format' => 'default',
        'exclude' => 1,
      ),
      'full' => array(
        'format' => 'default',
        'exclude' => 0,
      ),
      '4' => array(
        'format' => 'default',
        'exclude' => 0,
      ),
    ),
    'widget_active' => '1',
    'type' => 'text',
    'required' => '0',
    'multiple' => '0',
    'module' => 'text',
    'active' => '1',
    'text_processing' => '1',
    'max_length' => '',
    'allowed_values' => '',
    'allowed_values_php' => '',
    'widget' => array(
      'rows' => '10',
      'size' => 60,
      'default_value' => array(
        '0' => array(
          'value' => '',
          'format' => '1',
          '_error_element' => 'default_value_widget][field_meeting_notes][0][value',
        ),
      ),
      'default_value_php' => NULL,
      'label' => 'Notes',
      'weight' => 0,
      'description' => 'Record of items discussed, decisions made, and action items. Should be taken during the meeting.',
      'type' => 'text_textarea',
      'module' => 'text',
    ),
  );

  // Exported field: field_meeting_purpose
  $fields[] = array(
    'field_name' => 'field_meeting_purpose',
    'type_name' => 'meeting',
    'display_settings' => array(
      'weight' => '-4',
      'parent' => '',
      'label' => array(
        'format' => 'inline',
      ),
      'teaser' => array(
        'format' => 'default',
        'exclude' => 0,
      ),
      'full' => array(
        'format' => 'default',
        'exclude' => 0,
      ),
      '4' => array(
        'format' => 'default',
        'exclude' => 0,
      ),
    ),
    'widget_active' => '1',
    'type' => 'text',
    'required' => '1',
    'multiple' => '0',
    'module' => 'text',
    'active' => '1',
    'text_processing' => '0',
    'max_length' => '',
    'allowed_values' => '',
    'allowed_values_php' => '',
    'widget' => array(
      'rows' => 5,
      'size' => '60',
      'default_value' => array(
        '0' => array(
          'value' => '',
          '_error_element' => 'default_value_widget][field_meeting_purpose][0][value',
        ),
      ),
      'default_value_php' => NULL,
      'label' => 'Purpose',
      'weight' => '-4',
      'description' => 'Reason for having the meeting',
      'type' => 'text_textfield',
      'module' => 'text',
    ),
  );

  // Translatables
  array(
    t('Agenda'),
    t('Attachments'),
    t('Date'),
    t('Location'),
    t('Notes'),
    t('Purpose'),
  );

  return $fields;
}

/**
 * Helper to implementation of hook_ctools_plugin_api().
 */
function _meetings_ctools_plugin_api() {
  $args = func_get_args();
  $module = array_shift($args);
  $api = array_shift($args);
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => 1);
  }
}

/**
 * Helper to implementation of hook_user_default_permissions().
 */
function _meetings_user_default_permissions() {
  $permissions = array();

  // Exported permission: edit all meeting fields
  $permissions[] = array(
    'name' => 'edit all meeting fields',
    'roles' => array(),
  );

  // Exported permission: edit any meeting content
  $permissions[] = array(
    'name' => 'edit any meeting content',
    'roles' => array(
      '0' => 'authenticated user',
    ),
  );

  // Exported permission: edit meeting agenda
  $permissions[] = array(
    'name' => 'edit meeting agenda',
    'roles' => array(
      '0' => 'authenticated user',
    ),
  );

  // Exported permission: edit meeting attachments
  $permissions[] = array(
    'name' => 'edit meeting attachments',
    'roles' => array(),
  );

  // Exported permission: edit meeting date
  $permissions[] = array(
    'name' => 'edit meeting date',
    'roles' => array(),
  );

  // Exported permission: edit meeting location
  $permissions[] = array(
    'name' => 'edit meeting location',
    'roles' => array(),
  );

  // Exported permission: edit meeting notes
  $permissions[] = array(
    'name' => 'edit meeting notes',
    'roles' => array(
      '0' => 'authenticated user',
    ),
  );

  // Exported permission: edit meeting purpose
  $permissions[] = array(
    'name' => 'edit meeting purpose',
    'roles' => array(),
  );

  // Exported permission: edit own meeting content
  $permissions[] = array(
    'name' => 'edit own meeting content',
    'roles' => array(
      '0' => 'authenticated user',
    ),
  );

  // Exported permission: subscribe other users
  $permissions[] = array(
    'name' => 'subscribe other users',
    'roles' => array(
      '0' => 'authenticated user',
    ),
  );

  // Exported permission: subscribe to content
  $permissions[] = array(
    'name' => 'subscribe to content',
    'roles' => array(
      '0' => 'authenticated user',
    ),
  );

  // Exported permission: take attendance for meetings
  $permissions[] = array(
    'name' => 'take attendance for meetings',
    'roles' => array(),
  );

  // Exported permission: view attendance for meetings
  $permissions[] = array(
    'name' => 'view attendance for meetings',
    'roles' => array(
      '0' => 'authenticated user',
    ),
  );

  return $permissions;
}

/**
 * Helper to implementation of hook_strongarm().
 */
function _meetings_strongarm() {
  $export = array();
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'content_extra_weights_meeting';
  $strongarm->value = array(
    'title' => '-5',
    'revision_information' => '4',
    'author' => '3',
    'options' => '5',
    'comment_settings' => '6',
    'menu' => '2',
  );

  $export['content_extra_weights_meeting'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_repeat_type_meeting';
  $strongarm->value = 'field_meeting_date';

  $export['node_repeat_type_meeting'] = $strongarm;
  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'og_content_type_usage_meeting';
  $strongarm->value = 'group_post_standard';

  $export['og_content_type_usage_meeting'] = $strongarm;
  return $export;
}
