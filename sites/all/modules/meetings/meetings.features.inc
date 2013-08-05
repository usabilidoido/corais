<?php

/**
 * Implementation of hook_ctools_plugin_api().
 */
function meetings_ctools_plugin_api() {
  list($module, $api) = func_get_args();
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => 1);
  }
}

/**
 * Implementation of hook_node_info().
 */
function meetings_node_info() {
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

/**
 * Implementation of hook_views_api().
 */
function meetings_views_api() {
  return array(
    'api' => '2',
  );
}
