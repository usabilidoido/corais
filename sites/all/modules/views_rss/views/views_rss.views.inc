<?php

/**
 * @file
 * Views plugins, handlers and hooks definition for Views RSS module.
 */

/**
 * Implementation of hook_views_style_plugins().
 * Adds view types to views UI interface.
 */
function views_rss_views_plugins() {
  return array(
    'style' => array(
      'rss_fields' => array(
        'type' => 'feed',
        'title' => t('RSS Feed - Fields'),
        'help' => t('Outputs a RSS formatted feed'),
        'handler' => 'views_rss_plugin_style_fields',
        'path' => VIEWS_RSS_PATH . '/views',
        'theme' => 'views_view_views_rss',
        'theme file' => 'theme.inc',
        'theme path' => VIEWS_RSS_PATH . '/theme',
        'uses row plugin' => FALSE,
        'uses fields' => TRUE,
        'uses options' => TRUE,
        'uses grouping' => FALSE,
        'help topic' => 'style-rss-fields',
      ),
    ),
  );
}

/**
 * Implementation of hook_views_handlers().
 */
function views_rss_views_handlers() {
  return array(
    'info' => array(
      'path' => VIEWS_RSS_PATH . '/views',
    ),
    'handlers' => array(
      'views_rss_handler_field_user_mail' => array(
        'parent' => 'views_handler_field_user_mail',
      ),
      'views_rss_handler_field_term_node_tid' => array(
        'parent' => 'views_handler_field_term_node_tid',
      ),
      'views_rss_handler_field_multiple' => array(
        'parent' => 'content_handler_field_multiple',
      ),
    ),
  );
}

/**
 * Implementation of hook_views_data_alter().
 */
function views_rss_views_data_alter(&$data) {
  $data['users']['mail']['field']['handler'] = 'views_rss_handler_field_user_mail';
  $data['term_node']['tid']['field']['handler'] = 'views_rss_handler_field_term_node_tid';
  // Replace content_handler_field_multiple field handler with its extended
  // version so we could use array values returned by Views RSS formatters.
  foreach ($data as $group => $fields) {
    foreach ($fields as $field_id => $field) {
      if (!empty($field['field']['handler']) && $field['field']['handler'] == 'content_handler_field_multiple') {
        $data[$group][$field_id]['field']['handler'] = 'views_rss_handler_field_multiple';
      }
    }
  }
}
