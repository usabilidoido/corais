<?php
// $Id:

/**
 * @file
 * The bestreplyviews.inc views support for best reply.
 */

/**
 * Implementation of hook_views_data().
 */
function bestreply_views_data() {

// Basic table information.

// Define the base group of this table. Fields that don't
// have a group defined will go into this field by default.
  $data['bestreply']['table']['group']  = t('Bestreply');
  // For base tables, explain how we join 
  $data['bestreply']['table']['join'] = array(
   'node' => array(
      'left_field' => 'nid',
      'field' => 'nid',
    ),
    'comments' => array(
      'left_field' => 'cid',
      'field' => 'cid',
    ),
    'users' => array(
      'left_field' => 'uid',
      'field' => 'aid',
    ),
  );
  $data['bestreply']['nid'] = array(
    'title' => t('Bestreply node id'),
    'help' => t('The nid of a node that has a best reply.'),
     'relationship' => array(
       'base' => 'node',
       'field' => 'nid',
       'handler' => 'views_handler_relationship',
       'label' => t('bestreply node'),
       'skip base' => array('node', 'node_revisions'),

     ),
   'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
   'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
    ),
  );

  $data['bestreply']['cid'] = array(
    'title' => t('Bestreply comment id'),
    'help' => t('The comment id of the best reply'),
     'relationship' => array(
       'base' => 'comments',
       'field' => 'cid',
       'handler' => 'views_handler_relationship',
       'label' => t('bestreply comment'),
     ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
   'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
    ),
  );
  $data['bestreply']['aid'] = array(
    'title' => t('User id'),
    'help' => t('The user id of the bestreply author.'),
     'relationship' => array(
       'base' => 'users',
       'field' => 'nid',
       'handler' => 'views_handler_relationship',
       'label' => t('bestreply user'),
     ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
   'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
    ),
  ); 
  return $data;
}