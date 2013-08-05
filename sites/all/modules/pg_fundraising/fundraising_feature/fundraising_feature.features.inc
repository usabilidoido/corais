<?php

/**
 * Implementation of hook_content_default_fields().
 */
function fundraising_feature_content_default_fields() {
  module_load_include('inc', 'fundraising_feature', 'fundraising_feature.defaults');
  $args = func_get_args();
  return call_user_func_array('_fundraising_feature_content_default_fields', $args);
}

/**
 * Implementation of hook_fieldgroup_default_groups().
 */
function fundraising_feature_fieldgroup_default_groups() {
  module_load_include('inc', 'fundraising_feature', 'fundraising_feature.defaults');
  $args = func_get_args();
  return call_user_func_array('_fundraising_feature_fieldgroup_default_groups', $args);
}

/**
 * Implementation of hook_imagecache_default_presets().
 */
function fundraising_feature_imagecache_default_presets() {
  module_load_include('inc', 'fundraising_feature', 'fundraising_feature.defaults');
  $args = func_get_args();
  return call_user_func_array('_fundraising_feature_imagecache_default_presets', $args);
}

/**
 * Implementation of hook_node_info().
 */
function fundraising_feature_node_info() {
  module_load_include('inc', 'fundraising_feature', 'fundraising_feature.features.node');
  $args = func_get_args();
  return call_user_func_array('_fundraising_feature_node_info', $args);
}
