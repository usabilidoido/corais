<?php

/**
 * Helper to implementation of hook_node_info().
 */
function _atrium_answers_node_info() {
  $items = array(
    'question' => array(
      'name' => t('Question'),
      'module' => 'features',
      'description' => t('A question that can be answered, not just discussed.'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'has_body' => '1',
      'body_label' => t('Body'),
      'min_word_count' => '0',
      'help' => '',
    ),
  );
  return $items;
}
