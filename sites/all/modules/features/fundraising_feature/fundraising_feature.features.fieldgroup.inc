<?php

/**
 * Implementation of hook_fieldgroup_default_groups().
 */
function fundraising_feature_fieldgroup_default_groups() {
  $groups = array();

  // Exported group: group_donation_menu
  $groups['pg_fundraising-group_donation_menu'] = array(
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
  // Included for use with string extractors like potx.
  t('Donation menu');

  return $groups;
}
