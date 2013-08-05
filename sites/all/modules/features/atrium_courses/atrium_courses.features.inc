<?php

/**
 * Implementation of hook_ctools_plugin_api().
 */
function atrium_courses_ctools_plugin_api() {
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
function atrium_courses_node_info() {
  $items = array(
    'course' => array(
      'name' => t('Course'),
      'module' => 'features',
      'description' => t('A <em>course</em> containing Drupal learning objects.'),
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

/**
 * Implementation of hook_views_api().
 */
function atrium_courses_views_api() {
  return array(
    'api' => '2',
  );
}
