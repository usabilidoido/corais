<?php
// $Id: driven_node.diff.inc,v 1.2 2010/04/01 01:08:01 arhak Exp $

/**
 * Implements hook_diff_values().
 */
function driven_node_diff_values($element, $form_values, $meta) {
  extract($meta); // $node_type, $realm, $target, $provider, [$theme]
  
  // note that teaser_js has #value but no #default_value
  // since it is handled by node_teaser_js on #after_build
  // and saved into $form['body']['#value'] which is already considered
  // nevertheless, it need to be considered within the bundled property
  // to have access to it, but comparison should be bypassed
  if (isset($element['#teaser']) && $element['#teaser'] == 'edit-body') {
    return FALSE;
  }
  
  return driven_diff_values_non_array($element, $form_values);
}

/**
 * Implements hook_diff_render().
 */
function driven_node_diff_render($old, $new, $meta) {
  extract($meta); // $node_type, $realm, $target, $provider, [$theme]
  
  if ($target === 'body_field:bundled') {
    // @TODO: do something to display format changes, 
    // might be achieved having diff_render as a triplet
    // with 3rd item being a flag for
    // potentials visually imperceptible changes
    // which for instance would be true if only a format change happened
    // indicating that it might be no visual difference
    // or FALSE if everything is being rendered (which does NOT mean they can't render the same)
    // @TODO: also consider teaser_include?
    // @TODO: will need to trim the body_field somehow
    // also support other text input for other modules wishing to delegate?
    $outgoing = check_markup($old['body'], $old['format'], TRUE);
    $incoming = check_markup($new['body'], $new['format'], TRUE);
    return array($outgoing, $incoming);
  }
  
  // $theme might be NULL (e.g. title)
  return driven_diff_output_non_array($old, $new, $theme);
}
