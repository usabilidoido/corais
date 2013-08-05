<?php
// $Id: comment.inc,v 1.1 2009/07/23 05:20:03 deciphered Exp $
/**
 * @file
 * Contains Comment module integration for the Mentions module.
 */

/**
 * Implementation of hook_comment().
 */
function mentions_comment(&$a1, $op) {
  switch ($op) {
    case 'publish':
      _mentions_update('comment', mentions_find_users($a1['comment']), $a1['cid'], $a1['uid']);
      break;

    case 'delete':
      _mentions_delete('comment', $a1->cid);
      break;
  }
}

/**
 * Flush Comment Mentions if linked Comment no longer exists.
 */
function _mentions_comment_flush() {
  db_query("DELETE {mentions} m FROM {mentions} m LEFT JOIN {comments} c ON m.mid = c.cid WHERE m.type = 'comment' AND c.cid IS NULL");
}

/**
 * Implementation of mentions_hook_views_data_alter().
 */
function mentions_comment_views_data_alter(&$data) {
  $data['comments']['table']['join']['mentions'] = array(
    'left_table' => 'mentions',
    'left_field' => 'mid',
    'field' => 'cid',
  );
}

/**
 * Implementation of mentions_hook_views_handler_field_content_fields().
 */
function mentions_comment_views_handler_field_content_fields($view) {
  $view->additional_fields['comment'] = array('table' => 'comments', 'field' => 'comment');
}

/**
 * Implementation of mentions_hook_views_handler_field_content().
 */
function mentions_comment_views_handler_field_content($view, $values) {
  return $values->{$view->aliases['comment']};
}

/**
 * Implementation of mentions_hook_views_handler_field_link_fields().
 */
function mentions_comment_views_handler_field_link_fields($view) {
  $view->additional_fields['comment_nid'] = array('table' => 'comments', 'field' => 'nid');
}

/**
 * Implementation of mentions_hook_views_handler_field_link().
 */
function mentions_comment_views_handler_field_link($view, $values, &$link) {
  $link['url'] = 'node/'. $values->{$view->aliases['comment_nid']};
  $link['options']['fragment'] = 'comment-'. $values->{$view->aliases['mid']};
}

/**
 * Implementation of mentions_hook_views_handler_field_title_fields().
 */
function mentions_comment_views_handler_field_title_fields($view) {
  $view->additional_fields['subject'] = array('table' => 'comments', 'field' => 'subject');
}

/**
 * Implementation of mentions_hook_views_handler_field_title().
 */
function mentions_comment_views_handler_field_title($view, $values) {
  return $values->{$view->aliases['subject']};
}
