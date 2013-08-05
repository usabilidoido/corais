<?php

/**
 * @file nodequeue_annotate.views.inc
 * Nodequeue Annotate's Views integration.
 */


/**
 * Implementation of hook_views_handlers().
 */
function nodequeue_annotate_views_handlers() {
  return array(
  'info' => array(
    'path' => drupal_get_path('module', 'nodequeue_annotate') .'/includes/views',
    ),
    'handlers' => array(
      'nodequeue_annotate_handler_field_links' => array(
        'parent' => 'views_handler_field_node_link',
      ),
    ),
  );
}

/**
 * Implementation of hook_views_data().
 */
function nodequeue_annotate_views_data() {
  $data = array();

  $data['nodequeue_nodes']['annotation'] = array(
    'title' => t('Annotation'),
    'help' => t('The annotation of the node within a subqueue.'),
    'group' => t('Nodequeue Annotate'),
    'field' => array(
      'handler' => 'views_handler_field_xss',
      'click sortable' => TRUE,
     ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );
  
  return $data;
}

/**
 * Implementation of hook_views_data_alter().
 */
function nodequeue_annotate_views_data_alter(&$data) {
  
  $data['node']['nodequeue_annotate_links'] = array(
    'group' => t('Nodequeue Annotate'),
    'title' => t('Add/remove links'),
    'help' => t('Display add/remove links for all eligible nodequeues. Note that this performs a node_load so could add many extra queries to a view.'),
    'real field' => 'nid',
    'field' => array(
      'handler' => 'nodequeue_annotate_handler_field_links',
    ),
  );
  
}
