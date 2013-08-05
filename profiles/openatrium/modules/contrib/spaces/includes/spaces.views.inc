<?php

/**
 * Implementation of hook_views_plugins().
 */
function spaces_views_plugins() {
  return array(
    'module' => 'spaces',
    'access' => array(
      'spaces_feature' => array(
        'title' => t('Spaces feature'),
        'help' => t('Access will be granted if the given feature is enabled in the current space.'),
        'handler' => 'spaces_plugin_access_spaces_feature',
        'path' => drupal_get_path('module', 'spaces') .'/includes',
        'uses options' => TRUE,
      ),
    ),
  );
}

/**
 * Implementation of hook_views_handlers().
 */
function spaces_views_handlers() {
  return array(
    'info' => array(
      'path' => drupal_get_path('module', 'spaces') .'/includes',
    ),
    'handlers' => array(
      'spaces_handler_filter_spaces_current' => array('parent' => 'views_handler_filter'),
      'spaces_handler_filter_spaces_feature' => array('parent' => 'views_handler_filter'),
      'spaces_handler_field_spaces_feature' => array('parent' => 'views_handler_field_node_type'),
      'spaces_handler_field_node' => array('parent' => 'views_handler_field_node'),
    ),
  );
}

/**
 * Implementation of hook_views_data().
 * Adds a meta-filter that provides a layer of abstraction that
 * delegates actual filtering to the implementing space type modules.
 */
function spaces_views_data() {
  $data = array();
  // This defines a fake "pseudo" table.
  $data['spaces']['table']['group']  = t('Spaces');
  $data['spaces']['table']['join'] = array(
    'node' => array(
      'left_field' => 'nid',
      'field' => 'id',
    ),
    'users' => array(
      'left_field' => 'uid',
      'field' => 'id',
    ),
  );
  $data['spaces']['current'] = array(
    'title' => t('Content in current space'),
    'help' => t('Filters content to only those that belong to the current space.'),
    'filter' => array(
      'handler' => 'spaces_handler_filter_spaces_current',
    ),
  );

  // Pseudo node table
  $data['spaces_node']['table']['group']  = t('Spaces');
  $data['spaces_node']['table']['join'] = array(
    'node' => array(
      'table' => 'node',
      'left_field' => 'nid',
      'field' => 'nid',
    ),
  );
  $data['spaces_node']['feature'] = array(
    'title' => t('Spaces feature'),
    'help' => t('Spaces feature associated with the current node.'),
    'field' => array(
      'field' => 'type',
      'handler' => 'spaces_handler_field_spaces_feature',
    ),
  );
  return $data;
}

/**
 * Implementation of hook_views_data_alter().
 */
function spaces_views_data_alter(&$data) {
  // Add type filter by current feature.
  $data['node']['feature'] = array(
    'group' => t('Spaces'),
    'title' => t('Node type in current feature'),
    'help' => t('Filters on node types associated with the current spaces feature.'),
    'filter' => array(
      'field' => 'type',
      'handler' => 'spaces_handler_filter_spaces_feature',
    ),
  );
  // Iterate through Views data and replace instances of
  // views_handler_field_node with spaces node handler.
  foreach ($data as $table => $data_table) {
    foreach ($data_table as $field => $data_field) {
      if (isset($data_field['field']['handler']) && $data_field['field']['handler'] === 'views_handler_field_node') {
        $data[$table][$field]['field']['handler'] = 'spaces_handler_field_node';
      }
    }
  }
}
