<?php

/**
 * @file
 * This file contains all the views hooks for the appending of a view
 * to another view.
 */


/**
 * Implementation of hook_views_handler()
 */
function views_append_views_handlers() {
  return array(
    'info' => array(
      'path' => drupal_get_path('module', 'views_append') .'',
    ),
    'handlers' => array(
      'views_append_handler_append_view' => array(
        'parent' => 'views_handler_field',
      ),
    ),
  );
}

/**
 * Implementaion of hook_views_data()
 */
function views_append_views_data() {


  $data['views_append']['table']['group'] = t('View');
  $data['views_append']['table']['join'] = array(
    '#global' => array(),
  );

  $data['views_append']['append'] = array(
    'title' => t('Append View'),
    'help' => t('Appends a view at this view.'),
    'field' => array(
      'handler' => 'views_append_handler_append_view',
      'click sortable' => FALSE,
      'notafield' => TRUE,
    ),
  ); 
  return $data;
}



