<?php

/**
 * @file
 * Support file for the Domain access.
 */

/**
 * Implementation of hook_node_import_fields().
 */
function domain_node_import_fields($type) {
  $fields = array();

  if (($node_type = node_import_type_is_node($type)) !== FALSE) {
    global $_domain;
  
    $fields['domain_site'] = array(
      'title' => t('Send to all affiliates'),
      'group' => t('Domain access options'),
      'module' => 'domain',
      'is_mappable' => user_access('administer content') || user_access('moderate content'),
      'default_value' => 0,
    );
    $options = array();
    foreach (domain_domains() as $data) {
      // Cannot pass zero in checkboxes.
      ($data['domain_id'] == 0) ? $key = -1 : $key = $data['domain_id'];
      // The domain must be valid.
      if ($data['valid'] || user_access('administer domains')) {
        $options[$key] = $data['sitename'];
      }
    }
    $fields['domains'] = array(
      'title' => t('Publish to'),
      'group' => t('Domain access options'),
      'module' => 'domain',
      'is_mappable' => user_access('administer content') || user_access('moderate content'),
      'default_value' => array(($_domain['domain_id'] == 0) ? -1 : $_domain['domain_id']), // Can't use 0 as a checkbox value.
      'has_multiple' => TRUE,
      'is_checkboxes' => TRUE,
      'allowed_values' => $options,
    );
  }

  return $fields;
}

/**
 * Implementation of hook_node_import_defaults().
 */
function domain_node_import_defaults($type, $defaults, $fields, $map) {
  $form = array();

  if (($node_type = node_import_type_is_node($type)) !== FALSE) {
    global $_domain;

    $options = array();
    foreach (domain_domains() as $data) {
      // Cannot pass zero in checkboxes.
      ($data['domain_id'] == 0) ? $key = -1 : $key = $data['domain_id'];
      // The domain must be valid.
      if ($data['valid'] || user_access('administer domains')) {
        $options[$key] = $data['sitename'];
      }
    }
  
    $form['domain_site'] = array(
      '#type' => 'checkbox',
      '#title' => t('Send to all affiliates'),
      '#required' => FALSE,
      '#description' => t('Select if this content can be shown to all affiliates.  This setting will override the options below.'),
      '#default_value' => 0,
    );
    $form['domains'] = array(
      '#type' => 'checkboxes',
      '#title' => t('Publish to'),
      '#options' => $options,
      '#required' => FALSE,
      '#description' => t('Select which affiliates can access this content.'),
      '#default_value' => array(($_domain['domain_id'] == 0) ? -1 : $_domain['domain_id']), // Can't use 0 as a checkbox value.
    );
  }

  return $form;
}

