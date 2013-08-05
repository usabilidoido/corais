<?php
// $Id: content_complete.views.inc,v 1.1.2.7 2010/10/08 13:55:11 pvhee Exp $

/**
 *  @file
 *  Views integration.
 */

/**
 * Implementation of hook_views_data()
 */
function content_complete_views_data() {
  $data['content_complete']['table']['group']  = t('Content Complete');
  
  $data['content_complete']['table']['join'] = array(
    'node' => array(
      'left_field' => 'nid',
      'field' => 'nid',
    ),
  );

  $data['content_complete']['completeness'] = array(
    'title' => t('Completeness'),
    'help' => t('The completeness of the node.'),
    'field' => array(
      'handler' => 'content_complete_views_handler_field_completeness',
      'click sortable' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
      'numeric' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  $data['content_complete']['completed'] = array(
    'title' => t('Completed'),
    'help' => t('Filter to view whether or not a node is 100% completed.'),
    'filter' => array(
      'real field' => 'completeness',
      'title' => t('Completed'),
      'handler' => 'content_complete_views_handler_filter_complete_nodes',
      'type' => 'yes-no',
    ),
  );

  $data['content_complete']['data'] = array(
    'title' => t('Completeness Data'),
    'help' => t('The completeness data of the node.'),
    'field' => array(
      'handler' => 'content_complete_views_handler_field_data',
      'click sortable' => FALSE,
    ),
  );
  
  $data['content_complete']['content_type'] = array(
    'title' => t('Content type'),
    'help' => t('Whether or not the content type is used for Content Complete.'),
    'filter' => array(
      'handler' => 'content_complete_views_handler_filter_content_type',
      'label' => t('Is Content Complete enabled content type'),
    ),
  );
  
  return $data;
}

/**
 * Implementation of hook_views_handlers().
 */
function content_complete_views_handlers() {
  return array(
    'info' => array(
      'path' => drupal_get_path('module', 'content_complete') .'/views/handlers',
    ),
    'handlers' => array(
      // field handlers
      'content_complete_views_handler_field_completeness' => array(
        'parent' => 'views_handler_field_custom',
      ),
      'content_complete_views_handler_field_data' => array(
        'parent' => 'views_handler_field_custom',
      ),
      
      // filter handlers
      'content_complete_views_handler_filter_content_type' => array(
        'parent' => 'views_handler_filter_boolean_operator',
      ),
      'content_complete_views_handler_filter_complete_nodes' => array(
        'parent' => 'views_handler_filter_boolean_operator',
      ),
    ),
  );
}

/**
  * Implementation of hook_views_plugin().
  */
function content_complete_views_plugins() {
  return array(
    'module' => 'content_complete',
    'style' => array(
      'content_complete' => array(
        'path' => drupal_get_path('module', 'content_complete') . '/views/plugins',
        'title' => t('Content Complete'),
        'help' => t('Display completeness for a number of rows.'),
        'handler' => 'content_complete_style_plugin',
        'uses row plugin' => TRUE,
        'uses options' => TRUE,
        'type' => 'normal',
      ),
    ),
  );
}
