<?php
// $Id: nodequeuenode.views.inc,v 1.2 2009/09/08 09:39:49 daniboy Exp $

/**
 * @file nodequeuenode.views.inc
 * Nodequeuenode's Views integration.
 */


/**
 * Implementation of hook_views_data()
 */
function nodequeuenode_views_data() {
  $data = array();
  
  // Description of the nodequeuenode relations table and it's relationship to other tables
  $data['nodequeuenode_relation']['table'] = array(
    'group' => t('Nodequeuenode'),
    'join' => array(
      'node' => array(
        'left_field' => 'nid',
        'field' => 'nid',
      ),
    ),
  );
  
  // The "nid" field is a foreign key to the {node} table.
  $data['nodequeuenode_relation']['nid'] = array(
    'title' => 'nid',
    'help' => t('The ID of Meta Nodes.'),
  );
  
  // The "sqid" field is a foreign key to the {nodequeue_subqueue} table.
  $data['nodequeuenode_relation']['sqid'] = array(
    'title' => t('Related subqueue ID'),
    'help' => t('The ID of the subqueue the node relates to.'),
    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
     ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'nodequeue_handler_argument_subqueue_qid',
      'name field' => 'title',
      'name table' => 'nodequeue_subqueue',
      'empty field name' => t('No queue'),
    ),
  );
  
  return $data;
}

/**
 * Implementation of hook_views_data_alter().
 */
function nodequeuenode_views_data_alter(&$data) {
  
  // Create a new join to relate the smartqueue_users's privacy table to the
  // node table through nodequeuenode's relations table
  if (module_exists('smartqueue_users')) {
    $data['smartqueue_users_subqueue']['table']['join']['node'] = array(
      'left_table' => 'nodequeuenode_relation',
      'left_field' => 'sqid',
      'field' => 'sqid',
    );
  }
  
  // Create new joins to relate nodequeue's queue table to the node table
  // through nodequeuenode's relations table
  $data['nodequeue_queue']['table']['join']['node'] = array(
      'left_table' => 'nodequeue_subqueue',
      'left_field' => 'qid',
      'field' => 'qid',
    );
  $data['nodequeue_subqueue']['table']['join']['node'] = array(
    'left_table' => 'nodequeuenode_relation',
    'left_field' => 'sqid',
    'field' => 'sqid',
  );
  
}

/**
 * Implementation of hook_views_handlers().
 */
function nodequeuenode_views_handlers() {
  return array(
    'info' => array(
      'path' => drupal_get_path('module', 'nodequeuenode') .'/includes/views',
    ),
    'handlers' => array(
      'nodequeuenode_handler_relationship_nodequeue' => array(
        'parent' => 'views_handler_relationship',
      ),
    ),
  );
}

/**
 * Implementation of hook_views_plugins().
 */
function nodequeuenode_views_plugins() {
  $path = drupal_get_path('module', 'nodequeuenode');
  $views_path = drupal_get_path('module', 'views');
  return array(
    'module' => 'nodequeuenode', // This just tells our themes are elsewhere.
    'display' => array(
      // Parents are not really displays, just needed so the files can
      // be included.
      'parent' => array(
        'no ui' => TRUE,
        'handler' => 'views_plugin_display',
        'path' => "$views_path/plugins",
        'parent' => '',
      ),
      'attachment' => array(
        'no ui' => TRUE,
        'handler' => 'views_plugin_display_attachment',
        'path' => "$views_path/plugins",
        'parent' => 'parent',
      ),
      // Display plugin for nodequeuenodes.
      'nodequeuenode' => array(
        'title' => t('Nodequeuenode'),
        'help' => t('Meta-Node Attachments added to other displays.'),
        'handler' => 'nodequeuenode_plugin_display_attachment',
        'parent' => 'attachment',
        'path' => "$path/includes/views",
        'theme' => 'views_view',
        'use ajax' => TRUE,
        'admin' => t('Nodequeuenode'),
      ),
    ),
  );
}

