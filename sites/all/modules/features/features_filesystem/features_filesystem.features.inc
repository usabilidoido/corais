<?php

/**
 * Implementation of hook_content_default_fields().
 */
function features_filesystem_content_default_fields() {
  module_load_include('inc', 'features_filesystem', 'features_filesystem.defaults');
  $args = func_get_args();
  return call_user_func_array('_features_filesystem_content_default_fields', $args);
}

/**
 * Implementation of hook_context_default_contexts().
 */
function features_filesystem_context_default_contexts() {
  module_load_include('inc', 'features_filesystem', 'features_filesystem.defaults');
  $args = func_get_args();
  return call_user_func_array('_features_filesystem_context_default_contexts', $args);
}

/**
 * Implementation of hook_menu_default_items().
 */
function features_filesystem_menu_default_items() {
  module_load_include('inc', 'features_filesystem', 'features_filesystem.defaults');
  $args = func_get_args();
  return call_user_func_array('_features_filesystem_menu_default_items', $args);
}

/**
 * Implementation of hook_node_info().
 */
function features_filesystem_node_info() {
  module_load_include('inc', 'features_filesystem', 'features_filesystem.features.node');
  $args = func_get_args();
  return call_user_func_array('_features_filesystem_node_info', $args);
}

/**
 * Implementation of hook_rules_defaults().
 */
function features_filesystem_rules_defaults() {
  module_load_include('inc', 'features_filesystem', 'features_filesystem.defaults');
  $args = func_get_args();
  return call_user_func_array('_features_filesystem_rules_defaults', $args);
}

/**
 * Implementation of hook_user_default_permissions().
 */
function features_filesystem_user_default_permissions() {
  module_load_include('inc', 'features_filesystem', 'features_filesystem.defaults');
  $args = func_get_args();
  return call_user_func_array('_features_filesystem_user_default_permissions', $args);
}

/**
 * Implementation of hook_views_default_views().
 */
function features_filesystem_views_default_views() {
  module_load_include('inc', 'features_filesystem', 'features_filesystem.features.views');
  $args = func_get_args();
  return call_user_func_array('_features_filesystem_views_default_views', $args);
}
