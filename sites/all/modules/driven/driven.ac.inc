<?php
// $Id: driven.ac.inc,v 1.1 2010/03/27 22:56:51 arhak Exp $

/**
 * Deferred function driven_access_control_to_form().
 */
function _driven_access_control_to_form(&$ned_form, $driven_props, $apply_mask = TRUE) {
  // each property may have #access NULL, TRUE, FALSE, or not set
  
  $properties_map = driven_build_properties_map($driven_props, $ned_form, TRUE);
  $tree = driven_build_property_paths_tree($properties_map);
  
  // access_mask MUST be the first thing to do
  // to avoid granting access to unrelated elements
  if ($apply_mask) {
    driven_access_mask($ned_form, $tree);
  }
  
  $grant_props = array();
  $deny_props = array();
  foreach ($driven_props as $property_id => $prop) {
    if (!isset($prop['#access'])) {
      continue;
    }
    if ($prop['#access']) {
      $grant_props[$property_id] = $prop;
    }
    else {
      $deny_props[$property_id] = $prop;
    }
  }
  $grant_map = driven_build_properties_map($grant_props, $ned_form, TRUE);
  $grant_tree = driven_build_property_paths_tree($grant_map);
  driven_access_modify($ned_form, $grant_tree, TRUE);
  $deny_map = driven_build_properties_map($deny_props, $ned_form, TRUE);
  $deny_tree = driven_build_property_paths_tree($deny_map);
  driven_access_modify($ned_form, $deny_tree, FALSE);
  
  // seal parents which no child left granted
  driven_access_cleanup($ned_form, $tree);
  
  // if there is nothing allowed left,
  // make the caller aware to avoid working in vain
  // the way to know it, is if cleanup revoked access to the top level
  // (note that top level access is NEVER granted by any means) 
  return (!isset($ned_form['#access']) || $ned_form['#access']);
}

function driven_access_cleanup(&$ned_form, $tree) {
  // Access Control Policies were already applied
  // seek for elements included in property paths which got all forbidden
  // i.e. if it is an ancestor of driven properties,
  // but none of them remained allowed/accessible
  $children_keys = element_children($tree);
  foreach ($children_keys as $key) {
    // first go in depth (bottom-up)
    driven_access_cleanup($ned_form[$key], $tree[$key]);
  }

  // now check whether it still has at least an allowed child
  foreach (element_children($ned_form) as $key) {
    if (!isset($ned_form[$key]['#access']) || $ned_form[$key]['#access']) {
      // found an accesible child
      return; // it doesn't requires cleanup
    }
  }
  // didn't find any accessible child
  
  $is_top_level = empty($tree['#path']);
  $has_children = !empty($children_keys);
  
  if ($is_top_level || $has_children) {
    // not yet at property path level
    //
    // didn't find any accessible child
    // therefore it does requires cleanup
    // even if it is the top level (the whole form itself)
    $ned_form['#access'] = FALSE;
  }
}

function driven_access_modify(&$ned_form, $tree, $allowed) {
  // (note that top level access is NEVER granted by any means)
  if ($allowed) { 
    $in_depth = driven_is_grant_in_depth();
  }
  else {
    $in_depth = driven_is_deny_in_depth();
  }
  
  $children_keys = element_children($tree);
  foreach ($children_keys as $key) {
    if ($allowed) {
      // grant access to every property and its ancestors (otherwise it wouldn't be reachable)
      $ned_form[$key]['#access'] = TRUE;
    }
    // if denying it will be handled by cleanup
    driven_access_modify($ned_form[$key], $tree[$key], $allowed);
  }
  
  // using very explicit variable names,
  // since this is a sensible matter
  $is_top_level = empty($tree['#path']);
  $has_children = !empty($children_keys);
  
  // note that top level access is NEVER granted by any means
  // and only denied by cleanup 
  if ($is_top_level || $has_children) {
    // not yet at property path level
    return; // don't fall through leaf treatment (bellow)
  }
  
  // found a leaf
  // therefore we are at a property path level
  $ned_form['#access'] = $allowed;
  // if in_depth then also grant/deny access to every element beneath it
  if ($in_depth) {
    _driven_access_in_depth($ned_form, $allowed);
  }
}

function driven_access_mask(&$ned_form, $tree) {
  // forbid access to anything not included in at least a property path
  // i.e. if it is an ancestor of at least a driven property
  // then don't mess with its #access (i.e. don't even touch it)
  
  $children_keys = element_children($tree);
  $has_children = !empty($children_keys);
  if (!$has_children) {
    // found a leaf
    // therefore we are at a property path level
    // don't touch this level neither beneath it
    return;
  }
  
  foreach (element_children($ned_form) as $key) {
    // avoid in_array, which succeeds comparing string vs zero
    // i.e. in_array($key, $children_keys) when children contain a zero delta
    if (array_key_exists($key, $tree)) {
      // it is in a property path, seek beneath it
      driven_access_mask($ned_form[$key], $tree[$key]);
      // don't touch this level
      continue;
    }
    // it is not in the way to any property path => seal it
    $ned_form[$key]['#access'] = FALSE;
    // be paranoiac (in case of bugs in FAPI or custom element types),
    // seal everything beneath it as well
    _driven_access_in_depth($ned_form[$key], FALSE);
  }
}
