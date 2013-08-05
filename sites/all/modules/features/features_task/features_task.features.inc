<?php

/**
 * Implementation of hook_ctools_plugin_api().
 */
function features_task_ctools_plugin_api() {
  list($module, $api) = func_get_args();
  if ($module == "context" && $api == "context") {
    return array("version" => 3);
  }
  elseif ($module == "strongarm" && $api == "strongarm") {
    return array("version" => 1);
  }
}

/**
 * Implementation of hook_flag_default_flags().
 */
function features_task_flag_default_flags() {
  $flags = array();
  // Exported flag: "Andamento da tarefa".
  $flags['completed'] = array(
    'content_type' => 'node',
    'title' => 'Andamento da tarefa',
    'global' => '1',
    'types' => array(
      '0' => 'milestone',
      '1' => 'task',
    ),
    'flag_short' => 'Em andamento',
    'flag_long' => 'Tarefa em andamento',
    'flag_message' => '',
    'unflag_short' => 'Completada',
    'unflag_long' => 'Tarefa concluída',
    'unflag_message' => '',
    'unflag_denied_text' => '',
    'link_type' => 'toggle',
    'roles' => array(
      'flag' => array(
        '0' => 3,
        '1' => 4,
        '2' => 5,
      ),
      'unflag' => array(
        '0' => 3,
        '1' => 4,
        '2' => 5,
      ),
    ),
    'weight' => 0,
    'anon_display' => '',
    'anon_message' => '',
    'show_on_page' => 1,
    'show_on_teaser' => 1,
    'show_on_form' => 0,
    'access_author' => '',
    'i18n' => '1',
    'module' => 'features_task',
    'locked' => array(
      '0' => 'name',
    ),
    'api_version' => 2,
  );
  return $flags;

}

/**
 * Implementation of hook_node_info().
 */
function features_task_node_info() {
  $items = array(
    'milestone' => array(
      'name' => t('Etapa'),
      'module' => 'features',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Título'),
      'has_body' => '1',
      'body_label' => t('Descrição'),
      'min_word_count' => '0',
      'help' => '',
    ),
    'task' => array(
      'name' => t('Tarefa'),
      'module' => 'features',
      'description' => t('These are OA Group tasks'),
      'has_title' => '1',
      'title_label' => t('Título da tarefa'),
      'has_body' => '1',
      'body_label' => t('Descrição'),
      'min_word_count' => '0',
      'help' => '',
    ),
  );
  return $items;
}

/**
 * Implementation of hook_views_api().
 */
function features_task_views_api() {
  return array(
    'api' => '2',
  );
}
