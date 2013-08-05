<?php
// $Id: driven_path.diff.inc,v 1.1 2010/03/27 22:56:52 arhak Exp $

/**
 * Implements hook_diff_values().
 */
function driven_path_diff_values($element, $form_values, $meta) {
  extract($meta); // $node_type, $realm, $target, $provider, [$theme]
  
  return driven_diff_values_non_array($element, $form_values);
}

/**
 * Implements hook_diff_render().
 */
function driven_path_diff_render($old, $new, $meta) {
  extract($meta); // $node_type, $realm, $target, $provider, [$theme]
  
  // there are contrib modules which add more stuff under "URL path" fieldset
  // but they will need to request to be supported here
  // @TODO: review pathauto & other modules acting on path
  
  // $theme might be 'driven_diff_link'
  // or empty, which would cause rendering plain text instead of links
  return driven_diff_output_non_array($old, $new, $theme);
}
