<?php
/**
 * @file
 * Support file for the contrib scheduler module.
 */

/**
 * Implementation of hook_node_import_fields().
 */
function scheduler_node_import_fields($type) {
  $fields = array();

  if (($node_type = node_import_type_is_node($type)) !== FALSE) {
    if (variable_get('scheduler_publish_enable_' . $node_type, 0) == 1) {
      $fields['publish_on'] = array(
        'title' => t('Publish on'),
        'group' => t('Scheduling options'),
        'module' => 'scheduler',
        'weight' => 35,
        'map_required' => variable_get('scheduler_publish_required_' . $node_type, 0),
        'is_mappable' => user_access('schedule (un)publishing of nodes'),
        'input_format' => 'date',
        'default_value' => '',
      );
    }

    if (variable_get('scheduler_unpublish_enable_' . $node_type, 0) == 1) {
      $fields['unpublish_on'] = array(
        'title' => t('Unpublish on'),
        'group' => t('Scheduling options'),
        'module' => 'scheduler',
        'weight' => 35,
        'map_required' => variable_get('scheduler_unpublish_required_' . $node_type, 0),
        'is_mappable' => user_access('schedule (un)publishing of nodes'),
        'input_format' => 'date',
        'default_value' => '',
      );
    }
  }

  return $fields;
}

/**
 * Implementation of hook_node_import_defaults().
 */
function scheduler_node_import_defaults($type, $defaults, $fields, $map) {
  $form = array();

  if (($node_type = node_import_type_is_node($type)) !== FALSE && user_access('schedule (un)publishing of nodes')) {
    if (variable_get('scheduler_publish_enable_' . $node_type, 0) == 1) {
      $form['publish_on'] =  array(
        '#type' => 'textfield',
        '#title' => t('Publish on'),
        '#maxlength' => 25,
        '#default_value' => isset($defaults['publish_on']) ? $defaults['publish_on'] : '',
      );
    }

    if (variable_get('scheduler_unpublish_enable_' . $node_type, 0) == 1) {
      $form['unpublish_on'] =  array(
        '#type' => 'textfield',
        '#title' => t('Unpublish on'),
        '#maxlength' => 25,
        '#default_value' => isset($defaults['unpublish_on']) ? $defaults['unpublish_on'] : '',
      );
    }
  }

  return $form;
}

