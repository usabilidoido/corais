<?php

/**
 * Implementation of hook_ctools_plugin_api().
 */
function atrium_groups_ctools_plugin_api() {
  list($module, $api) = func_get_args();
  if ($module == "context" && $api == "context") {
    return array("version" => 3);
  }
  elseif ($module == "spaces" && $api == "spaces") {
    return array("version" => 3);
  }
  elseif ($module == "strongarm" && $api == "strongarm") {
    return array("version" => 1);
  }
}

/**
 * Implementation of hook_node_info().
 */
function atrium_groups_node_info() {
  $items = array(
    'group' => array(
      'name' => t('Projeto'),
      'module' => 'features',
      'description' => t('A group space for members to coordinate, collaborate and share ideas.'),
      'has_title' => '1',
      'title_label' => t('Título'),
      'has_body' => '1',
      'body_label' => t('Corpo'),
      'min_word_count' => '0',
      'help' => '',
    ),
  );
  return $items;
}

/**
 * Implementation of hook_views_api().
 */
function atrium_groups_views_api() {
  return array(
    'api' => '2',
  );
}
