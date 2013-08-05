<?php

/**
 * Implementation of hook_ctools_plugin_api().
 */
function atrium_blog_ctools_plugin_api() {
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
function atrium_blog_node_info() {
  $items = array(
    'blog' => array(
      'name' => t('Post de blog'),
      'module' => 'features',
      'description' => t('Um blog é um diário virtual constituído de vários posts exibidos em ordem cronológica decrescente. Cada usuário registrado do seu site pode criar e manter seu próprio blog.'),
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
function atrium_blog_views_api() {
  return array(
    'api' => '2',
  );
}
