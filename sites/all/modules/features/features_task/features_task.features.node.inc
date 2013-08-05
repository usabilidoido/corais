<?php

/**
 * Helper to implementation of hook_node_info().
 */
function _features_task_node_info() {
  $items = array(
    'milestone' => array(
      'name' => t('milestone'),
      'module' => 'features',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Title'),
      'has_body' => '1',
      'body_label' => t('Description'),
      'min_word_count' => '0',
      'help' => '',
    ),
    'task' => array(
      'name' => t('task'),
      'module' => 'features',
      'description' => t('These are OA Group tasks'),
      'has_title' => '1',
      'title_label' => t('Task Title'),
      'has_body' => '1',
      'body_label' => t('Description'),
      'min_word_count' => '0',
      'help' => '',
    ),
  );
  return $items;
}
