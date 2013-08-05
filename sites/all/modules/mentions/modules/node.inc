<?php
// $Id: node.inc,v 1.1 2009/07/23 05:20:03 deciphered Exp $
/**
 * @file
 * Contains Node module integration for the Mentions module.
 */

/**
 * Implementation of hook_nodeapi().
 */
function mentions_nodeapi(&$node, $op, $a3 = NULL, $a4 = NULL) {
  switch ($op) {
    case 'insert':
    case 'update':
      _mentions_update('node', mentions_find_users($node->body), $node->nid, $node->uid);
      break;

    case 'delete':
      _mentions_delete('node', $node->nid);

      // Flush Comment Mentions for deleted Comments link to deleted Node.
      if (function_exists('_mentions_comment_flush')) {
        _mentions_comment_flush();
      }
      break;
  }
}

/**
 * Implementation of mentions_hook_views_data_alter().
 */
function mentions_node_views_data_alter(&$data) {
  $data['node_revisions']['table']['join']['mentions'] = array(
    'left_table' => 'mentions',
    'left_field' => 'mid',
    'field' => 'nid',
  );
}

/**
 * Implementation of mentions_hook_views_handler_field_content_fields().
 */
function mentions_node_views_handler_field_content_fields($view) {
  $view->additional_fields['body'] = array('table' => 'node_revisions', 'field' => 'body');
}

/**
 * Implementation of mentions_hook_views_handler_field_content().
 */
function mentions_node_views_handler_field_content($view, $values) {
  return $values->{$view->aliases['body']};
}

/**
 * Implementation of mentions_hook_views_handler_field_link().
 */
function mentions_node_views_handler_field_link($view, $values, &$link) {
  $link['url'] = 'node/'. $values->{$view->aliases['mid']};
}

/**
 * Implementation of mentions_hook_views_handler_field_title_fields().
 */
function mentions_node_views_handler_field_title_fields($view) {
  $view->additional_fields['title'] = array('table' => 'node_revisions', 'field' => 'title');
}

/**
 * Implementation of mentions_hook_views_handler_field_title().
 */
function mentions_node_views_handler_field_title($view, $values) {
  return $values->{$view->aliases['title']};
}
