<?php

/**
 * @file
 * Support file for the Moderate module (modr8).
 */

/**
 * Implementation of hook_node_import_fields().
 */
function modr8_node_import_fields($type) {
  $fields = array();

  if (($node_type = node_import_type_is_node($type)) !== FALSE) {
    $node_options = variable_get('node_options_'. $node_type, array('status', 'promote'));

    $fields['moderate'] = array(
      'title' => t('In moderation queue'),
      'group' => t('Publishing options'),
      'module' => 'modr8',
      'weight' => 30,
      'is_mappable' => user_access('administer content') || user_access('moderate content'),
      'input_format' => 'boolean',
      'default_value' => in_array('moderate', $node_options) ? '1' : '0',
    );
  }

  return $fields;
}

/**
 * Implementation of hook_node_import_defaults().
 */
function modr8_node_import_defaults($type, $defaults, $fields, $map) {
  $form = array();

  if (($node_type = node_import_type_is_node($type)) !== FALSE) {
    $node_options = variable_get('node_options_'. $node_type, array('status', 'promote'));

    $form['moderate'] = array(
      '#title' => t('In moderation queue'),
      '#type' => 'radios',
      '#options' => array(0 => t('No'), 1 => t('Yes')),
      '#default_value' => isset($defaults['moderate']) ? $defaults['moderate'] : (in_array('moderate', $node_options) ? 1 : 0),
    );
  }

  return $form;
}
