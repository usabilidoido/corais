<?php

/**
 * Implementation of hook_ctools_plugin_api().
 */
function meeting_planner_ctools_plugin_api() {
  list($module, $api) = func_get_args();
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => 1);
  }
}

/**
 * Implementation of hook_node_info().
 */
function meeting_planner_node_info() {
  $items = array(
    'meeting_planner' => array(
      'name' => t('Meeting planner'),
      'module' => 'features',
      'description' => t('A poll with multiple dates to find the best moment for a meeting.'),
      'has_title' => '1',
      'title_label' => t('For'),
      'has_body' => '1',
      'body_label' => t('Details'),
      'min_word_count' => '0',
      'help' => '',
    ),
  );
  return $items;
}
