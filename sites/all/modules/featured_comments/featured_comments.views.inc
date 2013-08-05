<?php


/**
 * Implementation of hook_views_api().
 */
function featured_comments_views_api() {
  return array(
    'api' => 2,
    'path' => drupal_get_path('module', 'featured_comments'),
  );
}


/**
 * Implementation of hook_views_handlers().
 */
function featured_comments_views_handlers() {
  return array(
    'info' => array(
      'path' => drupal_get_path('module', 'featured_comments') .'/plugins',
    ),
    'handlers' => array(
      'featured_comments_handler_filter_featured' => array(
        'parent' => 'views_handler_filter',
       ),
    ),
  );
}

function featured_comments_views_data() {
  $data = array();
  $data['featured_comments']['table']['group']  = t('Featured comments');

  $data['featured_comments']['table']['join'] = array(
    'featured_comments' => array(
      'left_field' => 'cid',
      'field' => 'cid',
    ),
    'comments' => array(
      'left_field' => 'cid',
      'field' => 'cid',
    ),
  );

  $data['featured_comments']['status'] = array(
    'title' => t('Featured'),
    'help' => t('Whether or not the comment is featured.'),
    'field' => array(
      'handler' => 'views_handler_field_boolean',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_boolean_operator',
      'label' => t('Featured'),
      'type' => 'yes-no',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );
  
  $data['featured_comments']['weight'] = array(
    'title' => t('Weight in lists'), // The item it appears as on the UI,
    'help' => t('Weight in lists'), // The help that appears on the UI,
     // Information for displaying an input format as a field
    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
    ),
    // Information for sorting on input format
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    // Information for accepting input format as a filter
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
  );

  return $data;
}

