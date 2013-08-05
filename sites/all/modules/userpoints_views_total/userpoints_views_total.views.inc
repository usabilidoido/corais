<?php
// $Id$

/**
 * @file Expose the userpoints total value to Views.
 */

/**
 * Implementation of hook_views_data().
 */
function userpoints_views_total_views_data() {
  //User points total
  $data['userpoints_total']['table']['group']  = t('Userpoints');

  $data['userpoints_total']['table']['base'] = array(
    'field' => 'uid',
    'title' => t('Userpoints Total'),
    'help' => t('Total !Points accumulated by users on your site.', userpoints_translation()),
  );

  $data['userpoints_total']['table']['join'] = array(
    'users' => array(
      'left_field' => 'uid',
      'field' => 'uid',
    ),
    'node' => array(
      'left_field' => 'uid',
      'field' => 'uid',
    ),
    // This goes to the node so that we have consistent authorship.
    'node_revisions' => array(
      'left_table' => 'node',
      'left_field' => 'uid',
      'field' => 'uid',
    ),
  );

  // Describe the total_points column of the userpoints_total table.
  $data['userpoints_total']['total_points'] = array(
    'title' => t('Total !Points', userpoints_translation()),
    'help' => t("A User's current total !points.", userpoints_translation()), // The help that appears on the UI,
    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
      'numeric' => TRUE,
      'name field' => 'total_points', // display this field in the summary
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  // Add relationship to user table.
  $data['userpoints_total']['uid'] = array(
    'title' => t('User'),
    'help' => t('Relate the total userpoints table to the user table.'),
    'relationship' => array(
      'base' => 'users',
      'field' => 'uid',
      'label' => t('Users'),
      'handler' => 'views_handler_relationship',
    ),
  );

  return $data;
}