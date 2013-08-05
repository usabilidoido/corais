<?php

/**
 * Implementation of hook_ctools_plugin_api().
 */
function features_webform_ctools_plugin_api() {
  module_load_include('inc', 'features_webform', 'features_webform.defaults');
  $args = func_get_args();
  return call_user_func_array('_features_webform_ctools_plugin_api', $args);
}

/**
 * Implementation of hook_menu_default_items().
 */
function features_webform_menu_default_items() {
  module_load_include('inc', 'features_webform', 'features_webform.defaults');
  $args = func_get_args();
  return call_user_func_array('_features_webform_menu_default_items', $args);
}

/**
 * Implementation of hook_node_info().
 */
function features_webform_node_info() {
  module_load_include('inc', 'features_webform', 'features_webform.features.node');
  $args = func_get_args();
  return call_user_func_array('_features_webform_node_info', $args);
}

/**
 * Implementation of hook_user_default_permissions().
 */
function features_webform_user_default_permissions() {
  module_load_include('inc', 'features_webform', 'features_webform.defaults');
  $args = func_get_args();
  return call_user_func_array('_features_webform_user_default_permissions', $args);
}

/**
 * Implementation of hook_strongarm().
 */
function features_webform_strongarm() {
  module_load_include('inc', 'features_webform', 'features_webform.defaults');
  $args = func_get_args();
  return call_user_func_array('_features_webform_strongarm', $args);
}

/**
 * Implementation of hook_views_default_views().
 */
function features_webform_views_default_views() {
  module_load_include('inc', 'features_webform', 'features_webform.features.views');
  $args = func_get_args();
  return call_user_func_array('_features_webform_views_default_views', $args);
}
