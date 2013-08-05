<?php

/**
 * Implementation of hook_content_default_fields().
 */
function fundraising_feature_content_default_fields() {
  $fields = array();

  // Exported field: field_pg_fundraising_amount
  $fields['pg_fundraising-field_pg_fundraising_amount'] = array(
    'field_name' => 'field_pg_fundraising_amount',
    'type_name' => 'pg_fundraising',
    'display_settings' => array(
      'weight' => '2',
      'parent' => 'group_donation_menu',
      'label' => array(
        'format' => 'hidden',
      ),
      'teaser' => array(
        'format' => 'hidden',
        'exclude' => 0,
      ),
      'full' => array(
        'format' => 'hidden',
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
    'multiple' => '3',
    'module' => 'text',
    'active' => '1',
    'text_processing' => '0',
    'max_length' => '',
    'allowed_values' => '',
    'allowed_values_php' => '',
    'reference_preview' => '',
    'widget' => array(
      'rows' => 5,
      'size' => '5',
      'default_value' => array(
        '0' => array(
          'value' => '',
          '_error_element' => 'default_value_widget][field_pg_fundraising_amount][0][value',
        ),
      ),
      'default_value_php' => NULL,
      'label' => 'Donation amount',
      'weight' => '2',
      'description' => 'Add donation amount and currency symbol',
      'type' => 'text_textfield',
      'module' => 'text',
    ),
  );

  // Exported field: field_pg_fundraising_description
  $fields['pg_fundraising-field_pg_fundraising_description'] = array(
    'field_name' => 'field_pg_fundraising_description',
    'type_name' => 'pg_fundraising',
    'display_settings' => array(
      'weight' => '3',
      'parent' => 'group_donation_menu',
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
    'required' => '1',
    'multiple' => '3',
    'module' => 'text',
    'active' => '1',
    'text_processing' => '0',
    'max_length' => '200',
    'allowed_values' => '',
    'allowed_values_php' => '',
    'reference_preview' => '',
    'widget' => array(
      'rows' => '5',
      'size' => 60,
      'default_value' => array(
        '0' => array(
          'value' => '',
          '_error_element' => 'default_value_widget][field_pg_fundraising_description][0][value',
        ),
      ),
      'default_value_php' => NULL,
      'label' => 'How the money is used',
      'weight' => '3',
      'description' => '',
      'type' => 'text_textarea',
      'module' => 'text',
    ),
  );

  // Exported field: field_pg_fundraising_image
  $fields['pg_fundraising-field_pg_fundraising_image'] = array(
    'field_name' => 'field_pg_fundraising_image',
    'type_name' => 'pg_fundraising',
    'display_settings' => array(
      'weight' => '1',
      'parent' => 'group_donation_menu',
      'label' => array(
        'format' => 'hidden',
      ),
      'teaser' => array(
        'format' => 'hidden',
        'exclude' => 0,
      ),
      'full' => array(
        'format' => 'hidden',
        'exclude' => 0,
      ),
      '4' => array(
        'format' => 'image_plain',
        'exclude' => 0,
      ),
    ),
    'widget_active' => '1',
    'type' => 'filefield',
    'required' => '1',
    'multiple' => '3',
    'module' => 'filefield',
    'active' => '1',
    'list_field' => '0',
    'list_default' => 1,
    'description_field' => '0',
    'reference_preview' => '',
    'widget' => array(
      'file_extensions' => 'png gif jpg jpeg',
      'file_path' => 'fundraising_module/images',
      'progress_indicator' => 'bar',
      'max_filesize_per_file' => '2MB',
      'max_filesize_per_node' => '',
      'max_resolution' => '',
      'min_resolution' => '640x480',
      'alt' => '',
      'custom_alt' => 1,
      'title' => '',
      'custom_title' => 0,
      'title_type' => 'textfield',
      'default_image' => NULL,
      'use_default_image' => 0,
      'insert' => NULL,
      'insert_absolute' => NULL,
      'insert_styles' => NULL,
      'insert_default' => NULL,
      'insert_class' => NULL,
      'insert_width' => NULL,
      'label' => 'Donation menu image',
      'weight' => '1',
      'description' => '',
      'type' => 'imagefield_widget',
      'module' => 'imagefield',
    ),
  );

  // Translatables
  // Included for use with string extractors like potx.
  t('Donation amount');
  t('Donation menu image');
  t('How the money is used');

  return $fields;
}
