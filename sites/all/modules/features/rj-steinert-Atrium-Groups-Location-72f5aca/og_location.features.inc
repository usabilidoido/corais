<?php

/**
 * Implementation of hook_content_default_fields().
 */
function og_location_content_default_fields() {
  module_load_include('inc', 'og_location', 'og_location.defaults');
  $args = func_get_args();
  return call_user_func_array('_og_location_content_default_fields', $args);
}

/**
 * Implementation of hook_context_default_contexts().
 */
function og_location_context_default_contexts() {
  module_load_include('inc', 'og_location', 'og_location.defaults');
  $args = func_get_args();
  return call_user_func_array('_og_location_context_default_contexts', $args);
}

/**
 * Implementation of hook_views_default_views().
 */
function og_location_views_default_views() {
  module_load_include('inc', 'og_location', 'og_location.features.views');
  $args = func_get_args();
  return call_user_func_array('_og_location_views_default_views', $args);
}
