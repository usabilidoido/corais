<?php

/**
 * Implementation of hook_ctools_plugin_api().
 */
function texto_colaborativo_ctools_plugin_api() {
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
function texto_colaborativo_node_info() {
  $items = array(
    'text' => array(
      'name' => t('Texto Colaborativo'),
      'module' => 'features',
      'description' => t('Texto que pode ser escrito por várias pessoas ao mesmo tempo utilizando diferentes computadores. O Texto Colaborativo possui um chat no lado para facilitar a colaboração.'),
      'has_title' => '1',
      'title_label' => t('Título'),
      'has_body' => '1',
      'body_label' => t('Resumo'),
      'min_word_count' => '0',
      'help' => '',
    ),
  );
  return $items;
}

/**
 * Implementation of hook_views_api().
 */
function texto_colaborativo_views_api() {
  return array(
    'api' => '2',
  );
}
