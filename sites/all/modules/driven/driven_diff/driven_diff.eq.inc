<?php
// $Id: driven_diff.eq.inc,v 1.3 2010/04/03 23:18:07 arhak Exp $

/**
 * Equality detection is the "Achilles' heel" of this module
 * being the most "per case basis" code and therefore error-prone
 * also knowing 3rd party module's tweaks is needed to support their equality
 */

// helper function to ease driven_diff_form_values invocation
// which receives $form_values instead of $form_state (just to avoid the extra nested level)
function driven_diff_form_state($ned_form, $form_state, $driven_props) {
  return driven_diff_form_values($ned_form, $form_state['values'], $driven_props);
}

function driven_diff_form_values($ned_form, $form_values, $driven_props) {
  if ($ned_form['#access'] === FALSE) driven_assertion_failed('$ned_form[\'#access\']===FALSE (was it deliberately dispatched to driven_diff_form_values?)');
/*  if (FALSE === $ned_form['#access']) {
    // access denied => won't consent any difference
    return array();
  }*/
  
  // IMPORTANT: is responsibility of the caller function
  // to check $ned_form['#access'] (just top level)
  // if desired/intended this function will work beneath a node_form with #access=FALSE
  
  // this is only needed to improve error reporting (warn_property_gone)
  $node_type = $ned_form['#node']->type;
  
  $changes = array();
  $bundled_equal = array();
  // @per-module-discrimination: support content_multigroup
  // IMPORTANT: remember that CCK's multigroup will cause
  // the same property to be repeated under different paths
  $properties_map = driven_build_properties_map($driven_props, $ned_form);
  foreach ($properties_map as $property_path => $prop) {
    $realm = $prop['realm'];
    $target = $prop['target'];
    $property_id = $realm . ':' . $target;
    $meta = $prop['meta'];
    
    // @per-module-discrimination: support content_multigroup
    // test for group_name first, since it will be at least NULL, while group_type index might not exists
    // CCK's meta:
    // $node_type, $realm, $target, $provider,
    // $group_name/type, $field_name/type/module/multiple, $widget_type/module/handle_multiple_values
    // $driven_diff (note that driven_diff was added in driven_diff_driven_properties_meta_extension)
    $multigroup = ($realm == 'cck' && $meta['group_name'] && $meta['group_type'] == 'multigroup');
    // @per-module-discrimination: support workflow
    // in passing by, normalize old body_field's wrapper_joint to the more generic bundle joint
    // (e.g. node:body_field:bundled & workflow:workflow:bundled_schedule)
    $bundle = $meta['bundle'];
    $bundled = !empty($bundle);
    
    $path_parts = explode(':', $property_path);
    $element = $ned_form;
    $ancestor_title = '?';
    foreach ($path_parts as $key) {
      if (empty($element[$key])) {
        // unable to navigate beneath it
        // i.e. property gone (e.g. disabling modules)
        driven_diff_warn_property_gone($node_type, $property_id);
        // comparison is not possible => can't return any difference
        return array();
      }
      
      $element = $element[$key];
      if (!empty($element['#title'])) {
        $ancestor_title = $element['#title'];
      }
    }
    
    // keep track for bundle joint
    if ($bundled) {
      if (!isset($bundled_equal[$property_id])) {
        $bundled_equal[$property_id] = array();
      }
      
      // drop last part, which is only the bundle's holder
      $prefix = explode(':', $target);
      array_pop($prefix);
      $prefix = implode(':', $prefix);
      
      $sub_target = substr($property_path, strlen($prefix . ':'));
      if (strpos($property_path, $prefix . ':') !== 0) driven_assertion_failed('wrong sub-target declaration "' . $property_path . '" bundled under "' . $prefix . '" (property_id: ' . $property_id . ')');
    }
    $diff_values = driven_diff_elem_values($element, $form_values, $meta);
    if (!$diff_values) {
      if (!$bundled) {
        continue; // nothing else to do
      }
      else {
        // property requested to include non changed children
        // if at least one of them changed
        // since all of them will be needed at diff_render time
        //
        // IMPORTANT: bundle joint is one level depth only
        //
        // the only strategy known for bundle joint
        // would be the clasic #default_value vs driven_form_state_value
        // but there is not need to keep track of normalized values
        // since they were declared equal to #default_value
        //
        // IMPORTANT: every children has to have known_default_value
        // (i.e. $element['#default_value'] or a child of #type=radio pointing to it)
        // otherwise they will be ignored (e.g. format tips, body_field:teaser_js)
        if (!driven_elem_has_default_value_key($element)) {
          continue; // nothing else to do (the same as non bundled case)
        }
        $old = $new = driven_elem_default_value($element);
        // flag them as equal for later cleanup
        $bundled_equal[$property_id][$sub_target] = $sub_target;
        // fall through as if they were different
        // they will be cleaned up later if needed
        $diff_values = array($old, $new);
      }
    }
    list($old, $new) = $diff_values;
    $label = empty($prop['label']) ? $ancestor_title : $prop['label'];
    
    // @per-module-discrimination: support workflow
    // in passing by, normalize old body_field's wrapper_joint to the more generic bundle joint
    // (e.g. node:body_field:bundled & workflow:workflow:bundled_schedule)
    if ($bundled) {
      $changes[$property_id]['old'][$sub_target] = $old;
      $changes[$property_id]['new'][$sub_target] = $new;
      // label will be the same every time,
      // and it needs to be set at least once
      $changes[$property_id]['label'] = $label;
      continue; // nothing else to do
    }
    
    // @per-module-discrimination: support content_multigroup
    if ($multigroup) {
      if (isset($changes[$property_id])) {
        // this is another single delta
        $changes[$property_id]['old'][] = $old;
        $changes[$property_id]['new'][] = $new;
        continue; // nothing else to do
      }
      else {
        // this is a single delta of what should be several items
        $old = array($old);
        $new = array($new);
        // fall through usual case bellow
      }
    }
    
    // properties might no longer exist suddenly
    // a backup render has to be made instantly and kept
    // but until this point we might be just validating
    //
    // a default $label will be needed for diff_render time
    // ($old & $new will also be needed for diff_render time, but they storage is unquestionable)
    $changes[$property_id] = compact('label', 'old', 'new');
    // nevertheless, validator ended up rendering diff for preview
    // then $diff_render is being computed on every validation (being save or preview)
    // and not saved for submission, which will have to compute $diff_render again
    // BUT, won't go for a minor performance improvement in detriment of clean logic
    // asking for $changes doesn't involves render
    // beyond easing default label (relying on ancestor title)
    // since validator doesn't requires to preview the diff summary
    // that being optional makes sense to keep diff_render in submission stage
  }
  
  // @per-module-discrimination: support workflow
  // in passing by, normalize old body_field's wrapper_joint to the more generic bundle joint
  // (e.g. node:body_field:bundled & workflow:workflow:bundled_schedule)
  //
  // just right before returning $changes
  // cleanup $bundled_equal which didn't get any diff
  foreach ($bundled_equal as $property_id => $sub_targets) {
    $diff_values = FALSE;
    foreach ($changes[$property_id]['old'] as $sub_target => $old) {
      if (!in_array($sub_target, $sub_targets)) {
        $diff_values = TRUE;
        break;
      }
    }
    if (!$diff_values) {
      unset($changes[$property_id]);
    }
  }
  
  return $changes;
}

function driven_diff_elem_values($element, $form_values, $meta) {
  // Access Control Policies SHOULD be already applied using driven_access_control_to_form
  // check #access flag
  if (FALSE === $element['#access']) {
    // access denied => won't consent any difference
    return FALSE;
  }
  
  // wrapper detection
  // if values are not at this level pass through it
  if (!driven_elem_has_default_value_key($element)) {
    $element_children = element_children($element);
    if (!count($element_children)) {
      // having no children is a dead end (e.g. add_more)
      return FALSE; // ignore it
    }
    
    // while being a wrapper, it can hold expanded radios (e.g. format is a fieldset)
    // if so, each of its children is expected to be radio (or wrapper, e.g. format tips)  
    foreach ($element_children as $key) {
      if ($element[$key]['#type'] === 'radio') {
        // then either of its children will suffice to pick up the #default_value
        // and we have one at hand already
        return driven_diff_elem_values($element[$key], $form_values, $meta);
      }
    }
    
    foreach ($element_children as $key) {
      $diff_values = driven_diff_elem_values($element[$key], $form_values, $meta);
      if (!$diff_values) {
        continue;
      }
      
      list($old, $new) = $diff_values;
      // on diff_render symmetric_diff will drop the keys
      // therefore reseted indexes wouldn't harm here
      // but wouldn't be accurate either (e.g. for debugging)
      // in addition, content_taxonomy handles the key 'tids'
      // which this way is persisted into diff_values
      // and later recognized in diff_render time
      $olds[$key] = $old;
      $news[$key] = $new;
    }
    
    if (empty($olds) && empty($news)) {
      return FALSE;
    }
    return array($olds, $news);
  }
  // hitting a non-wrapper beneath the property path
  
  // modules should be aware of mismatching #default_value & $form_state['values']
  // they might differ in other ways beside the actual change
  // e.g.
  // - some arrays are compared by their values
  //   therefore non-assoc arrays vs assoc arrays might be equal despite their keys
  //   differences between their keys should be ignored in some cases      
  // - this becomes worse when #default_value is a non-assoc array
  //   while $form_state['values'] is a plain number
  // - and unleashed when CCK comes in
  // therefore, upon comparison, they have the chance to alter the $value
  // normalizing it to match #default_value format easing diff_render implementation
  // without re-taking into account the same altertanives than diff_values did
  $function = $meta['provider'] . '_diff_values';
  $diff_values = $function($element, $form_values, $meta);
  
  return $diff_values;
}

function driven_diff_symmetric_diff($arr1, $arr2) {
  // note that symmetric_diff will discard keys for first level
  // first pick up values and then deeply compare them
  // and finally will return arrays with reseted keys
    
  // we are going to iterate n^2
  // but that doesn't make clear at first sight
  // that keys are being discarded
  // so, lets make it clear enough
  $arr1 = array_values($arr1);
  $arr2 = array_values($arr2);
  
  $outgoing = $arr1;
  $incoming = $arr2;
  
  foreach ($arr1 as $index1 => $value1) {
    foreach ($arr2 as $index2 => $value2) {
      if (is_array($value1) && is_array($value2)) {
        if (driven_equal_keyed_array_in_depth($value1, $value2)) {
          unset($outgoing[$index1]);
          unset($incoming[$index2]);
          break;
        }
      }
      elseif (!is_array($value1) && !is_array($value2)) {
        if (_driven_equal_non_array($value1, $value2)) {
          unset($outgoing[$index1]);
          unset($incoming[$index2]);
          break;
        }
      }
      // else, they are not equal, continue looking
    }
  }
  
  // reset keys
  return array(array_values($outgoing), array_values($incoming));
}

function driven_equal_keyed_array_in_depth($arr1, $arr2) {
  if (!driven_equal_array_values(array_keys($arr1), array_keys($arr2))) {
    return FALSE;
  }
  foreach ($arr1 as $key => $value) {
    // $arr1[$key] might be NULL, 
    // and isset($arr2[$key]) checked bellow
    // would return FALSE if it is NULL as well
    if (!isset($arr1[$key])) {
      if (!isset($arr2[$key])) {
        // neither are set
        continue;
      }
      else {
        return FALSE;
      }
    }
    if (!isset($arr2[$key]) || (is_array($arr1[$key]) != is_array($arr2[$key]))) {
      return FALSE;
    }
    if (is_array($arr1[$key])) {
      if (!driven_equal_keyed_array_in_depth($arr1[$key], $arr2[$key])) {
        return FALSE;
      }
    }
    else {
      if (!_driven_equal_non_array($arr1[$key], $arr2[$key])) {
        return FALSE;
      }
    }
  }
  return TRUE;
}

function driven_equal_array_values($arr1, $arr2) {
  // note that this function won't normalize neither of its arguments
  // therefore two array might be considered equal
  // without having exactly the same type on each of their elements
  // e.g. an array of intengers might be equal to an array of strings
  // but overlooking this minor detail won't matter
  // since it will be able to perform the same as the former array
  foreach (array_values($arr1) + array_values($arr2) as $value) { 
    if (is_array($value)) driven_assertion_failed('misusage or driven_equal_array_values comparison: nested array within arguments');
  }
  // using count, since empty won't receive direct result from array_diff
  return (0 == (count(array_diff($arr1, $arr2)) + count(array_diff($arr2, $arr1))));
}

function _driven_equal_non_array($default, &$value) {
  // $value will be normized if needed (per case basis)
  // to match #default_value format instead of $form_state['values']
  
  // when #default_value is NULL the equivalent submitted value
  // should be an empty string (e.g. path)
  //
  // if $value is NULL while $default isn't
  // then the caller function must be reponsible of normalization
  // (e.g. case 'select' in driven_taxo_diff_values)
  // since we can't figure how it should be normalized (zero or empty string/array?)
  // (remember to be careful with values nested within arrays being compared in depth)
  if (is_null($default) || is_null($value)) {
    if ($value === '') {
      $value = NULL; 
    }
    return (is_null($default) && is_null($value));
  }
  
  if (is_bool($default) && is_int($value)) { 
    $value = (bool)$value; 
  }
  elseif (is_int($default) && (is_bool($value) || is_numeric($value))) {
    $value = (int)$value;
  }
  elseif (is_string($default) && (is_bool($value) || is_int($value))) {
    // not considering is_float($value) because
    // in some cases driven properties would need to take care
    // of converting decimal values to string to match
    // decimal zeros on the extreme right (e.g. CCK's decimal)
    // which would be very tricky for elements nested into arrays
    // therefore, this function doesn't support float value
    // which would need to reach this point already converted to string
    $value = (string)$value;
  }
  
  if (gettype($default) != gettype($value)) driven_assertion_failed('gettype($default) != gettype($value): ' . gettype($default) . ' vs ' . gettype($value));
  
  // using === to avoid comparing zero vs string which gives TRUE
  return ($default === $value || (empty($default) && empty($value)));
}


// helper function to pick up the default value
// ONCE KNOWN: it exists or one of its children is #type=radio pointing to it
// i.e. $element['#default_value'] or from a child of #type=radio with #parents having the path of $element
function driven_elem_default_value($element) {
  if (driven_elem_has_default_value_key($element)) {
    return $element['#default_value'];
  }
  // still, while being a wrapper, it can hold expanded radios (e.g. format is a fieldset)
  // if so, each of its children is expected to be radio (or wrapper, e.g. format tips)  
  foreach (element_children($element) as $key) {
    // finding children with no defualt value isn't enough for a negative
    // since they can be wrappers (e.g. format tips)
    if (driven_elem_has_default_value_key($element[$key])) {
      // found a non-wrapper => its type being radio would be a positive (while negative otherwise)
      if ($element[$key]['#type'] === 'radio') {
        // ensure we're not picking up a wrong radio's value (brought by workflow's driven properties [#758086])
        if (!driven_equal_array_values($element[$key]['#parents'], $element['#parents'])) driven_assertion_failed('improper form structure: $element[' . $key . '][\'#parents\'] differs from $element[\'#parents\'] for a radio child (child\'s #parents: ' . var_export($element[$key]['#parents'], TRUE) . ', element\'s #parents=' . var_export($element['#parents'], TRUE) . ')');
        // then either of its children will suffice to pick up the #default_value
        // and we have one at hand already
        return $element[$key]['#default_value'];
      }
      // found a children with default value but it isn't a radio
      // then stop looping through its children, it is already a negative
      //driven_assertion_failed('misusage of driven_elem_default_value: $element has no #default_value and one of its children is non-radio (child_key: ' . $key . ', #type=' . $element[$key]['#type'] . ')');
      //return FALSE;
      // (brought by workflow's driven properties [#758086])
      // no, workflow module has a fieldset wrapping radios and elements of other types
      // therefore, finding a non-radio wouldn't be a negative
    }
  }
  
  // if this point is reached then either
  // - it had no children
  // - its children didn't had default value
  driven_assertion_failed('misusage of driven_elem_default_value: $element has no #default_value and neither of its children is #type=radio');
  return FALSE;
} 

// helper function for wrapper detection
function driven_elem_has_default_value_key($element) {
  // this is the heart of the so called "wrapper detection"
  // it is not all of it, since it might be needed
  // to check for count(element_children($element) as well
  //
  // we only care if #default_value exists as a key of $element
  // not whether it is empty/NULL or anything, it doesn't matters
  //
  // careful, isset is not enough, since it can be NULL (e.g. number)
  // in_array is not enough, since it compares and matches zero with string
  // therefore, discard any zero key (or empty keys in general)
  // (there are not keys being FALSE/NULL/'', but if so, they are intended to be removed as well)
  // $keys = array_filter(array_keys($element));
  // return (in_array('#default_value', $keys));
  //
  // having this simplicity wrapped in a function
  // will help avoiding typo bugs
  return array_key_exists('#default_value', $element);
}

function driven_elem_state_value($element, $form_values) {
  // having this simplicity wrapped in a function
  // will help avoiding typo bugs
  return driven_form_state_value($element['#parents'], $form_values);
}
