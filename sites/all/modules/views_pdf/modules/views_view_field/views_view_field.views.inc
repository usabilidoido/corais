<?php

/**
 * @file
 * Views hook implementations
 */

/**
 * Implementation of hook_views_handler()
 */
function views_view_field_views_handlers() {
  return array(
    'info' => array(
      'path' => drupal_get_path('module', 'views_view_field') .'',
    ),
    'handlers' => array(
      'views_view_field_handler_include_view' => array(
        'parent' => 'views_handler_field',
      ),
    ),
  );
}

/**
 * Implemenation of hook_views_data()
 */
function views_view_field_views_data() {


  $data['view']['table']['group'] = t('View');
  $data['view']['table']['join'] = array(
    '#global' => array(),
  );

  $data['view']['include'] = array(
    'title' => t('Include View'),
    'help' => t('Includes a view into this view.'),
    'field' => array(
      'handler' => 'views_view_field_handler_include_view',
      'click sortable' => FALSE,
      'notafield' => TRUE,
    ),
  ); 
  return $data;
}



