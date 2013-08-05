<?php

/**
 * Implementation of hook_content_default_fields().
 */
function grayside_oa_feeds_opt_content_default_fields() {
  $fields = array();

  // Exported field: field_oa_feeds_allowed
  $fields['group-field_oa_feeds_allowed'] = array(
    'field_name' => 'field_oa_feeds_allowed',
    'type_name' => 'group',
    'display_settings' => array(
      'weight' => '31',
      'parent' => '',
      'label' => array(
        'format' => 'inline',
      ),
      'teaser' => array(
        'format' => 'default',
        'exclude' => 1,
      ),
      'full' => array(
        'format' => 'default',
        'exclude' => 1,
      ),
      '5' => array(
        'format' => 'default',
        'exclude' => 0,
      ),
      '4' => array(
        'format' => 'default',
        'exclude' => 0,
      ),
      '2' => array(
        'format' => 'default',
        'exclude' => 0,
      ),
      '3' => array(
        'format' => 'default',
        'exclude' => 0,
      ),
      'token' => array(
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
    'allowed_values' => 'Do Not Provide RSS Content
Provide RSS Content',
    'allowed_values_php' => '',
    'widget' => array(
      'default_value' => array(
        '0' => array(
          'value' => 'Do Not Provide RSS Content',
        ),
      ),
      'default_value_php' => NULL,
      'label' => 'Use RSS Notifications',
      'weight' => '31',
      'description' => 'Checking this box will allow group content to be pulled into site RSS feeds. RSS feeds are low security but greatly facilitate site involvement for RSS enthusiasts.',
      'type' => 'optionwidgets_onoff',
      'module' => 'optionwidgets',
    ),
  );

  // Translatables
  // Included for use with string extractors like potx.
  t('Use RSS Notifications');

  return $fields;
}
