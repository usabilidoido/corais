<?php

/**
 * Implementation of hook_ctools_plugin_api().
 */
function meeting_planner_ctools_plugin_api() {
  list($module, $api) = func_get_args();
  if ($module == "context" && $api == "context") {
    return array("version" => 3);
  }
  elseif ($module == "strongarm" && $api == "strongarm") {
    return array("version" => 1);
  }
}

/**
 * Implementation of hook_node_info().
 */
function meeting_planner_node_info() {
  $items = array(
    'meeting_planner' => array(
      'name' => t('Reunião'),
      'module' => 'features',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Título'),
      'has_body' => '1',
      'body_label' => t('Pauta'),
      'min_word_count' => '0',
      'help' => '',
    ),
  );
  return $items;
}

/**
 * Implementation of hook_views_api().
 */
function meeting_planner_views_api() {
  return array(
    'api' => '2',
  );
}
