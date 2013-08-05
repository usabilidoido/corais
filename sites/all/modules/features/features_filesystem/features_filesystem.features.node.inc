<?php

/**
 * Helper to implementation of hook_node_info().
 */
function _features_filesystem_node_info() {
  $items = array(
    'file' => array(
      'name' => t('file'),
      'module' => 'features',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Title'),
      'has_body' => '0',
      'body_label' => '',
      'min_word_count' => '0',
      'help' => '',
    ),
    'folder' => array(
      'name' => t('folder'),
      'module' => 'features',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Folder Name'),
      'has_body' => '1',
      'body_label' => t('Description'),
      'min_word_count' => '0',
      'help' => '',
    ),
  );
  return $items;
}
