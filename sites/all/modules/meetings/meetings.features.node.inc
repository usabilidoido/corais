<?php

/**
 * Helper to implementation of hook_node_info().
 */
function _meetings_node_info() {
  $items = array(
    'meeting' => array(
      'name' => t('Meeting'),
      'module' => 'features',
      'description' => t('Stores data related to a meeting including agendas, notes, invitations, attendance, time, location, and more.'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'has_body' => '0',
      'body_label' => '',
      'min_word_count' => '0',
      'help' => '',
    ),
  );
  return $items;
}
