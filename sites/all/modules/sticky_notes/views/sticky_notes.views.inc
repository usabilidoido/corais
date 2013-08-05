<?php

/**
 * @file
 * Views integration for the sticky_notes module.
 */

/**
 * Implementation of hook_views_data().
 *
 * @return void
 * @author berliner
 */

function sticky_notes_views_data() {

  $data = array();
  $data['node_sticky_note']['table']['group'] = 'Sticky Notes';
  $data['node_sticky_note']['table']['join'] = array(
    'node' => array(
      'field' => 'nid',
      'left_field' => 'nid',
    ),
    'users_roles' => array(
      'left_table' => 'node',
      'field' => 'uid',
      'left_field' => 'uid',
    ),
  );
  
  $data['node_sticky_note']['table']['join'] = array(
    'node' => array(
      'field' => 'nid',
      'left_field' => 'nid',
    ),
  );
  
  $data['node_sticky_note']['link_path'] = array(
    'title' => t('Path'),
    'help' => t('A link to the page that the note is attached to'),
    'field' => array(
      'handler' => 'views_handler_field_link_path',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_equality',
      'allow empty' => FALSE,
    ),
  );
  $data['node_sticky_note']['origin_title'] = array(
    'title' => t('Page title'),
    'help' => t('The title of the page that the note is´attached to'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_equality',
      'allow empty' => FALSE,
    ),
  );
  $data['node_sticky_note']['priority'] = array(
    'title' => t('Priority'),
    'help' => t('The notes priority'),
    'field' => array(
      'handler' => 'views_handler_field_sticky_notes_priority',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_sticky_notes_priority',
      'allow empty' => FALSE,
    ),
  );
  $data['node_sticky_note']['access'] = array(
    'title' => t('Access'),
    'help' => t('Filter by access.'),
    'filter' => array(
      'handler' => 'views_handler_filter_sticky_notes_access',
      'allow empty' => FALSE,
    ),
  );
  return $data;
}

/**
 * Implementation of hook_views_handlers().
 *
 * @return void
 * @author berliner
 */

function sticky_notes_views_handlers() {
  return array(
    'info' => array(
      'path' => drupal_get_path('module', 'sticky_notes') . '/views',
    ),
    'handlers' => array(
      'views_handler_field_sticky_notes_priority' => array('parent' => 'views_handler_field'),
      'views_handler_filter_sticky_notes_priority' => array('parent' => 'views_handler_filter'),
      'views_handler_field_link_path' => array('parent' => 'views_handler_field'),
      'views_handler_filter_sticky_notes_access' => array('parent' => 'views_handler_filter'),
    ),
  );
}
