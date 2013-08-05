<?php

/**
 * Implementation of hook_context_default_contexts().
 */
function atrium_answers_context_default_contexts() {
  module_load_include('inc', 'atrium_answers', 'atrium_answers.defaults');
  $args = func_get_args();
  return call_user_func_array('_atrium_answers_context_default_contexts', $args);
}

/**
 * Implementation of hook_ctools_plugin_api().
 */
function atrium_answers_ctools_plugin_api() {
  module_load_include('inc', 'atrium_answers', 'atrium_answers.defaults');
  $args = func_get_args();
  return call_user_func_array('_atrium_answers_ctools_plugin_api', $args);
}

/**
 * Implementation of hook_menu_default_items().
 */
function atrium_answers_menu_default_items() {
  module_load_include('inc', 'atrium_answers', 'atrium_answers.defaults');
  $args = func_get_args();
  return call_user_func_array('_atrium_answers_menu_default_items', $args);
}

/**
 * Implementation of hook_node_info().
 */
function atrium_answers_node_info() {
  module_load_include('inc', 'atrium_answers', 'atrium_answers.features.node');
  $args = func_get_args();
  return call_user_func_array('_atrium_answers_node_info', $args);
}

/**
 * Implementation of hook_user_default_permissions().
 */
function atrium_answers_user_default_permissions() {
  module_load_include('inc', 'atrium_answers', 'atrium_answers.defaults');
  $args = func_get_args();
  return call_user_func_array('_atrium_answers_user_default_permissions', $args);
}

/**
 * Implementation of hook_strongarm().
 */
function atrium_answers_strongarm() {
  module_load_include('inc', 'atrium_answers', 'atrium_answers.defaults');
  $args = func_get_args();
  return call_user_func_array('_atrium_answers_strongarm', $args);
}

/**
 * Implementation of hook_views_default_views().
 */
function atrium_answers_views_default_views() {
  module_load_include('inc', 'atrium_answers', 'atrium_answers.features.views');
  $args = func_get_args();
  return call_user_func_array('_atrium_answers_views_default_views', $args);
}
