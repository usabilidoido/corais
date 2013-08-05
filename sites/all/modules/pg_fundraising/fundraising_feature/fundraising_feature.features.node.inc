<?php

/**
 * Helper to implementation of hook_node_info().
 */
function _fundraising_feature_node_info() {
  $items = array(
    'pg_fundraising' => array(
      'name' => t('Fundraising'),
      'module' => 'features',
      'description' => t('By default, this content type is used by Fundraising module. Use this content type to set up donation pages on your website. In order to accept donations using this content type, Fundraising module and modules which it is dependent on need to be enabled and configured accordingly.'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'has_body' => '1',
      'body_label' => t('Description'),
      'min_word_count' => '0',
      'help' => '',
    ),
  );
  return $items;
}
