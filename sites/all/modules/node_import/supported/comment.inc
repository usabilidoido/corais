<?php

/**
 * @file
 * Support file for the core comment module.
 */

/**
 * Implementation of hook_node_import_fields().
 */
function comment_node_import_fields($type) {
  $fields = array();

  if (($node_type = node_import_type_is_node($type)) !== FALSE) {
    $fields['comment'] = array(
      'title' => t('Comment settings'),
      'group' => t('Comment settings'),
      'module' => 'comment',
      'weight' => 30,
      'is_mappable' => user_access('administer comments'),
      'allowed_values' => array(
        COMMENT_NODE_DISABLED => t('Disabled'),
        COMMENT_NODE_READ_ONLY => t('Read only'),
        COMMENT_NODE_READ_WRITE => t('Read/Write'),
      ),
      'default_value' => variable_get('comment_'. $node_type, COMMENT_NODE_READ_WRITE),
    );
  }

  return $fields;
}

/**
 * Implementation of hook_node_import_defaults().
 */
function comment_node_import_defaults($type, $defaults, $fields, $map) {
  $form = array();

  if (($node_type = node_import_type_is_node($type)) !== FALSE) {
    if (user_access('administer comments')) {
      $form['comment'] = array(
        '#title' => t('Comment settings'),
        '#type' => 'radios',
        '#options' => array(
          COMMENT_NODE_DISABLED => t('Disabled'),
          COMMENT_NODE_READ_ONLY => t('Read only'),
          COMMENT_NODE_READ_WRITE => t('Read/Write'),
        ),
        '#default_value' => isset($defaults['comment']) ? $defaults['comment'] : variable_get('comment_'. $node_type, COMMENT_NODE_READ_WRITE),
      );
    }
  }

  return $form;
}

