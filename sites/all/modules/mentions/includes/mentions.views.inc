<?php
// $Id: mentions.views.inc,v 1.3 2009/07/23 05:20:03 deciphered Exp $
/**
 * @file
 * Contains views data and handlers for the Mentions module.
 */

/**
 * Implementation of hook_views_handlers().
 */
function mentions_views_handlers() {
  return array(
    'info' => array(
      'path' => drupal_get_path('module', 'mentions') .'/includes',
    ),
    'handlers' => array(
      'mentions_views_handler_field_content' => array(
        'parent' => 'views_handler_field',
      ),
      'mentions_views_handler_field_link' => array(
        'parent' => 'views_handler_field',
      ),
      'mentions_views_handler_field_title' => array(
        'parent' => 'views_handler_field',
      ),
      'mentions_views_handler_field_user_name' => array(
        'parent' => 'views_handler_field_user_name',
      ),
    ),
  );
}

/**
 * Implementation of hook_views_data().
 */
function mentions_views_data() {
  $data['mentions']['table']['group']  = t('Mentions');

  $data['mentions']['table']['base'] = array(
    'field' => 'uid',
    'title' => t('Mentions'),
    'help' => t('References made to @username.'),
    'weight' => 10,
  );

  // Content.
  $data['mentions']['content'] = array(
    'title' => t('Content'),
    'help' => t('The contents of the mention.'),
    'field' => array(
      'handler' => 'mentions_views_handler_field_content',
    ),
  );

  // Link.
  $data['mentions']['link'] = array(
    'title' => t('Link'),
    'help' => t('The link to the mention.'),
    'field' => array(
      'handler' => 'mentions_views_handler_field_link',
    ),
  );

  // Timestamp.
  $data['mentions']['timestamp'] = array(
    'title' => t('Date'),
    'help' => t("The date of the mention."),
    'field' => array(
      'handler' => 'views_handler_field_date',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort_date',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_date',
    ),
  );

  // Title.
  $data['mentions']['title'] = array(
    'title' => t('Title'),
    'help' => t('The title of the mention.'),
    'field' => array(
      'handler' => 'mentions_views_handler_field_title',
    ),
  );

  // User name - Author.
  $data['mentions']['auid'] = array(
    'title' => t('Author'),
    'help' => t("The name of the mention's author."),
    'field' => array(
      'handler' => 'mentions_views_handler_field_user_name',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  // User name - Mentioned.
  $data['mentions']['uid'] = array(
    'title' => t('User'),
    'help' => t('The user mentioned.'),
    'field' => array(
      'handler' => 'mentions_views_handler_field_user_name',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  return $data;
}

function mentions_views_data_alter(&$data) {
  foreach (module_list() as $module) {
    if (function_exists($function = 'mentions_'. $module .'_views_data_alter')) {
      $function($data);
    }
  }
}
