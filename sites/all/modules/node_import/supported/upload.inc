<?php

/**
 * @file
 * Support file for the core upload module.
 */

/**
 * Implementation of hook_node_import_fields().
 */
function upload_node_import_fields($type) {
  $fields = array();
  return $fields; //TODO: reenable this

  if (($node_type = node_import_type_is_node($type)) !== FALSE && variable_get('upload_'. $node_type, TRUE)) {
    $fields['files'] = array(
      'title' => t('File attachments'),
      'group' => t('File attachments'),
      'is_mappable' => user_access('upload files'),
      'columns' => array(
        'filename' => array(
          'title' => t('Filename'),
          'has_multiple' => TRUE,
        ),
        'description' => array(
          'title' => t('Description'),
          'has_multiple' => TRUE,
        ),
        'list' => array(
          'title' => t('List'),
          'has_multiple' => TRUE,
        ),
        'weight' => array(
          'title' => t('Weight'),
          'has_multiple' => TRUE,
        ),
      ),
    );
  }

  return $fields;
}

