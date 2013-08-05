<?php
// $Id: reldate.views.inc,v 1.1.2.1 2010/08/10 16:22:51 yhahn Exp $

/**
 * Implementation of hook_views_handlers().
 */
function reldate_views_handlers() {
  return array(
    'info' => array('path' => drupal_get_path('module', 'reldate') .'/views'),
    'handlers' => array(
      'reldate_handler_field_date' => array('parent' => 'views_handler_field_date'),
      'reldate_handler_field_ncs_last_updated' => array('parent' => 'views_handler_field_ncs_last_updated')
    ),
  );
}

/**
 * Implementation of hook_views_data_alter().
 * Replaces default handlers with custom handlers.
 */
function reldate_views_data_alter(&$data) {
  foreach ($data as $table => $data_table) {
    foreach ($data_table as $field => $data_field) {
      if (!empty($data_field['field']['handler']) && $data_field['field']['handler'] === 'views_handler_field_date') {
        $data[$table][$field]['field']['handler'] = 'reldate_handler_field_date';
      }
    }
  }
  if (isset($data['node_comment_statistics']['last_updated']['field']['handler'])) {
    $data['node_comment_statistics']['last_updated']['field']['handler'] = 'reldate_handler_field_ncs_last_updated';
  }
}
