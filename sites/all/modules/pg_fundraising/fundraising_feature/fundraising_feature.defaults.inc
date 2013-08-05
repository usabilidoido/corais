<?php

/**
 * Helper to implementation of hook_content_default_fields().
 */
function _fundraising_feature_content_default_fields() {
  $fields = array();

  // Exported field: field_pg_fundraising_amount
  $fields[] = array(
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
  $fields[] = array(
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
  $fields[] = array(
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
      'label' => 'Donation menu image',
      'weight' => '1',
      'description' => '',
      'type' => 'imagefield_widget',
      'module' => 'imagefield',
    ),
  );

  // Translatables
  array(
    t('Donation amount'),
    t('Donation menu image'),
    t('How the money is used'),
  );

  return $fields;
}

/**
 * Helper to implementation of hook_fieldgroup_default_groups().
 */
function _fundraising_feature_fieldgroup_default_groups() {
  $groups = array();

  // Exported group: group_donation_menu
  $groups[] = array(
    'group_type' => 'standard',
    'type_name' => 'pg_fundraising',
    'group_name' => 'group_donation_menu',
    'label' => 'Donation menu',
    'settings' => array(
      'form' => array(
        'style' => 'fieldset',
        'description' => '',
      ),
      'display' => array(
        'weight' => '-2',
        'label' => 'hidden',
        'teaser' => array(
          'format' => 'hidden',
          'exclude' => 0,
        ),
        'full' => array(
          'format' => 'hidden',
          'exclude' => 0,
        ),
        'description' => '',
        '4' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
      ),
    ),
    'weight' => '-2',
    'fields' => array(
      '0' => 'field_pg_fundraising_image',
      '1' => 'field_pg_fundraising_amount',
      '2' => 'field_pg_fundraising_description',
    ),
  );

  // Translatables
  array(
    t('Donation menu'),
  );

  return $groups;
}

/**
 * Helper to implementation of hook_imagecache_default_presets().
 */
function _fundraising_feature_imagecache_default_presets() {
  $items = array(
    'donation_menu_item_220x165' => array(
      'presetname' => 'donation_menu_item_220x165',
      'actions' => array(
        '0' => array(
          'weight' => '0',
          'module' => 'imagecache',
          'action' => 'imagecache_scale_and_crop',
          'data' => array(
            'width' => '220',
            'height' => '165',
          ),
        ),
      ),
    ),
  );
  return $items;
}
