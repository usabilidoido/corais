<?php
// $Id$

/**
 * Implementation of hook_views_handlers().
 */
function grayside_oa_feeds_views_handlers() {
  return array(
    'info' => array(
      'path' => drupal_get_path('module', 'grayside_oa_feeds') .'/includes',
    ),
    'handlers' => array(
      'grayside_oa_feeds_handler_field_activity_id' => array(
        'parent' => 'atrium_activity_handler_field_activity',
      ),
      'grayside_oa_feeds_handler_field_activity_description' => array(
        'parent' => 'views_handler_field_markup',
      ),
    ),
  ); 
}

/**
 * Implementation of hook_views_data().
 */
function grayside_oa_feeds_views_data() {
  $data = array();
  $data['comments']['atrium_activity_id'] = array(
    'real field' => 'cid',
    'title' => t('Activity ID'),
    'help' => t("Unique ID for a poor man's activity."),
    'field' => array('handler' => 'grayside_oa_feeds_handler_field_activity_id'),
  );
  $data['node_revisions']['atrium_activity_teaser'] = array(
    'group' => t('Comment'),
    'real field' => 'teaser',
    'title' => t('Activity description'),
    'help' => t("Description of a poor man's activity."),
    'field' => array(
      'handler' => 'grayside_oa_feeds_handler_field_activity_description',
      'format' => 'format',
    ),
  ); 

  return $data;
}
