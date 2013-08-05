<?php
// $Id: comment_driven.tokens.inc,v 1.8 2010/03/27 23:02:25 arhak Exp $

/**
 * Implementation of hook_token_list().
 */
function comment_driven_token_list($type = 'all') {
  if ($type == 'comment' || $type == 'all') {
    $tokens = array();

    $description = t('Diff summary of driven changes made by the comment');
    $tokens['comment'] = array(
      'comment-driven-diff-table' => $description . ' (HTML table)',
      'comment-driven-diff-ulist' => $description . ' (HTML ul/li)',
      'comment-driven-diff-crlf'  => $description . ' (plain text with \\r\\n aka CRLF)',
      'comment-driven-diff-lf'    => $description . ' (plain text with \\n aka LF)',
      'comment-driven-diff-cr'    => $description . ' (plain text with \\r aka CR)',
    );
    
    return $tokens;
  }
}

/**
 * Implementation of hook_token_list().
 */
function comment_driven_token_values($type, $object = NULL) {
  if ($type == 'comment') {
    $comment = (object)$object;
    
    if ($log = comment_driven_load_log($comment->cid)) {
      $node_type = node_get_types('type', node_load($comment->nid))->type;
      $live_render = (!$log->frozen && comment_driven_is_live_render($node_type));
      $diff_render = $live_render ? driven_diff_render($node_type, $log->changes) : $log->diff_render;
      
      $rows = driven_diff_summary_rows_strip_tags($diff_render);
      $tokens['comment-driven-diff-table'] = driven_diff_output('driven_diff_simple_table', array('rows' => $rows));
      $tokens['comment-driven-diff-ulist'] = driven_diff_output('driven_diff_simple_ulist', array('rows' => $rows));
      
      $plain_text = array();
      foreach ($rows as $index => $row) {
        $plain_text[] = implode(' ', $row); // concatenate
      }
      $tokens['comment-driven-diff-crlf']  = implode("\r\n", $plain_text);
      $tokens['comment-driven-diff-lf']    = implode("\n", $plain_text);
      $tokens['comment-driven-diff-cr']    = implode("\r", $plain_text);
    }
    else {
      $tokens['comment-driven-diff-table'] = '';
      $tokens['comment-driven-diff-ulist'] = '';
      $tokens['comment-driven-diff-crlf']  = '';
      $tokens['comment-driven-diff-lf']    = '';
      $tokens['comment-driven-diff-cr']    = '';
    }
    
    return $tokens;
  }
}
