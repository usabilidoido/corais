<?php

/**
 * Implementation of hook_content_default_fields().
 */
function meetings_content_default_fields() {
  $fields = array();

  // Exported field: field_meeting_agenda
  $fields['meeting-field_meeting_agenda'] = array(
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
  $fields['meeting-field_meeting_attachments'] = array(
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
  $fields['meeting-field_meeting_date'] = array(
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
    'default_format' => 'short',
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
  $fields['meeting-field_meeting_location'] = array(
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
  $fields['meeting-field_meeting_notes'] = array(
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
  $fields['meeting-field_meeting_purpose'] = array(
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
      'description' => 'Complete the sentence "by the end of this meeting we will..." Example: <em>Generate a list of tactics for our campaign</em> or <em>Choose a mission statement for our group</em>',
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
