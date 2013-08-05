<?php

/**
 * Implementation of hook_fieldgroup_default_groups().
 */
function atrium_book_fieldgroup_default_groups() {
  $groups = array();

  // Exported group: group_card
  $groups['book-group_card'] = array(
    'group_type' => 'standard',
    'type_name' => 'book',
    'group_name' => 'group_card',
    'label' => 'Knowledge Card',
    'settings' => array(
      'form' => array(
        'style' => 'fieldset',
        'description' => '',
      ),
      'display' => array(
        'weight' => '2',
        'label' => 'above',
        'teaser' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        'full' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        'description' => '',
        '5' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        '4' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        '2' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        '3' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
        'token' => array(
          'format' => 'fieldset',
          'exclude' => 0,
        ),
      ),
    ),
    'weight' => '3',
    'fields' => array(
      '0' => 'field_input',
      '1' => 'field_card_description',
      '2' => 'field_output',
    ),
  );

  // Translatables
  // Included for use with string extractors like potx.
  t('Knowledge Card');

  return $groups;
}
