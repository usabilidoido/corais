<?php
// $Id: driven_cck_producer_consumer.inc,v 1.1 2010/04/03 23:20:47 arhak Exp $

function _driven_cck_producer_consumer_to_form(&$ned_form, $driven_props) {
  // each property may have #producer_consumer NULL, 'producer', 'consumer', or not set
  $restricted_props = array(
    // initialize for properties_map to receive proper arrays
    'producer' => array(),
    'consumer' => array(),
  );
  foreach ($driven_props as $property_id => $prop) {
    $model = $prop['#producer_consumer'];
    if (isset($model)) {
      $restricted_props[$model][$property_id] = $prop;
    }
  }
  
  // don't want the previewing node
  // instead we need the actual saved node
  // (otherwise incoming/outgoing will be applied to new deltas)
  $node = $ned_form['#node'];
  $node = node_load($node->nid);
  $deny_map = array();
  foreach (array('producer', 'consumer') as $model) {
    $properties_map = driven_build_properties_map($restricted_props[$model], $ned_form, TRUE);
    foreach ($properties_map as $property_path => $prop) {
      $path_parts = explode(':', $property_path);
      $element = &$ned_form;
      foreach ($path_parts as $key) {
        $element = &$element[$key];
      }
      $element['#element_validate'][] = 'driven_cck_' . $model . '_validate';
      // additions (e.g. _weight) are beneath the property path (for non-multigroup)
      // but theme_content_multiple_values is disregarding
      // #access=FALSE for individual deltas
      $element['#theme'] = 'driven_cck_producer_consumer_multiple_values';
      
      $meta = $prop['meta'];
      // $node_type, $realm, $target, $provider,
      // $group_name/type, $field_name/type/module/multiple, $widget_type/module/handle_multiple_values
      // $driven_diff (note that driven_diff was added in driven_diff_driven_properties_meta_extension)
      extract($meta);
      // just using a helper function
      // (there shouldn't be any property gone, since this is expected to be used with fresh read properties)
      $field = driven_cck_get_field($node_type, $field_name);
      if ($field === FALSE) driven_assertion_failed('Couldn\'t fetch field: ' . $field_name . ' for content type: ' . $node_type);
      $function_is_empty = $field_module . '_content_is_empty';
      
      // work around hackish content_add_more_js (only if it has add_more button)
      // (if it has a fixed multiplicity it won't get an add_more button)
      if ($model == 'producer' && $field_multiple == 1) {
        $node_type_url_str = str_replace('_', '-', $node_type);
        $json_callback = 'driven_cck_producer/js_add_more/' . $node_type_url_str . '/' . $field_name;
        $element[$field_name . '_add_more']['#ahah']['path'] = $json_callback;
      }
      
      foreach (element_children($element) as $delta) {
        if ($delta === $field_name . '_add_more') {
          continue;
        }
        
        // don't want the previewing node
        // instead we need the actual saved node
        // (otherwise incoming/outgoing will be applied to new deltas)
        //$item = driven_elem_default_value($element[$delta]);
        $items = $node->$field_name;
        $item = $items[$delta];
        
        // Bitwise Xor successful on either:
        // - $model is producer and not is_empty
        // - $model is consumer and is_empty
        if (($model == 'producer') ^ $function_is_empty($item, $field)) {
          // just flag the map's leaf (with anything) for further property_paths_tree & access_modify
          $deny_map[$property_path . ':' . $delta] = $model;
        }
        
        // @per-module-discrimination
        if ($field_module == 'filefield' && $model == 'consumer') {
          $element[$delta]['#process'][] = 'driven_cck_consumer_filefield_widget_process';
        }
      }
      // also remove any extra button that might remain
      if ($model == 'consumer') {
        // consumer will have add_more button
        // since behavior_form was restricted to fields with
        // $widget_handle_multiple_values == CONTENT_HANDLE_CORE
        $extra_button = $field_name . '_add_more';
        $deny_map[$property_path . ':' . $extra_button] = $model;
      }
    }
  }
  
  $deny_tree = driven_build_property_paths_tree($deny_map);
  driven_access_modify($ned_form, $deny_tree, FALSE);  
  // seal parents which no child left granted
  driven_access_cleanup($ned_form, $deny_tree);
}

function driven_cck_producer_add_more_js($type_name_url, $field_name) {
  // instead of mimicking content_add_more_js
  // pick up the cached form (form_get_cache) in the same fashion
  // check deltas with #access=FALSE and copy their #default_value to $_POST
  // then just delegate to content_add_more_js (which will print JSON and exit)
  
  // guided by content_add_more_js
  $type = content_types($type_name_url);
  $field = content_fields($field_name, $type['type']);
  // Retrieve the cached form.
  $form_state = array('submitted' => FALSE);
  $form_build_id = $_POST['form_build_id'];
  $form = form_get_cache($form_build_id, $form_state);
  // if a form wasn't picked up let content_add_more_js take care of complaining
  if ($form) {
    // pick up the old element at the right place in the (original, unbuilt) form.
    if (module_exists('fieldgroup') && ($group_name = _fieldgroup_field_get_group($type['type'], $field_name))) {
      $element = $form[$group_name][$field_name];
    }
    else {
      $element = $form[$field_name];
    }
    // save original element for later use in form_alter
    _driven_cck_producers_add_more($field_name, $element); 
    // update post info
    foreach (element_children($element) as $delta) {
      if ($delta === $field_name . '_add_more') {
        continue;
      }
      if (isset($element[$delta]['#access']) && !$element[$delta]['#access']) {
        $_POST[$field_name][$delta] = $element[$delta]['#default_value'];
        $_POST[$field_name][$delta]['_weight'] = $element[$delta]['_weight']['#default_value'];
      }
    }
  }
  module_load_include('inc', 'content', 'includes/content.node_form');
  content_add_more_js($type_name_url, $field_name);
}

/**
 * Deferred implementation of hook_form_alter() for content_add_more_js. 
 */
function _driven_cck_producer_consumer_add_more_js_form_alter(&$form, &$form_state) {
  // the only element to be dispatched by content_add_more_js
  // avoid messing up the $form when no action will be taken
  //$new_element = array_shift($form);
  $children_keys = element_children($form);
  if (count($children_keys) != 1) driven_assertion_failed('count($children_keys) != 1, $children_keys: ' . var_export($children_keys, TRUE));
  $new_element = &$form[$children_keys[0]];
  
  $field_name = $new_element['#field_name'];
  $orig_element = _driven_cck_producers_add_more($field_name);
  if (!isset($orig_element)) {
    // not ours
    return; // nothing to do
  }
  
  // reapply modificatinos performed at _driven_cck_producer_consumer_to_form
  $keys = array(
    '#element_validate',
    '#theme',
    $field_name . '_add_more',
  );
  foreach ($keys as $key) {
    // this is a simple way to check if the add_more button exists
    // to be copied, otherwise avoid creating a NULL children
    // in passing by, its healthy to protect every index against NULL children creation
    if (isset($orig_element[$key])) {
      $new_element[$key] = $orig_element[$key];
    }
  }
  
  foreach (element_children($orig_element) as $delta) {
    if ($delta === $field_name . '_add_more') {
      continue;
    }
    if (isset($orig_element[$delta]['#access']) && !$orig_element[$$delta]['#access']) {
      // copy #access (for #access=FALSE not to be rendered by content_add_more_js)
      $new_element[$delta]['#access'] = $orig_element[$delta]['#access'];
    }
  }
}

function driven_cck_consumer_filefield_widget_process($element, $edit, &$form_state, $form) {
  // delegate to filefield
  module_load_include('inc', 'filefield', 'filefield_widget');
  $element = filefield_widget_process($element, $edit, $form_state, $form);
  // cleanup forbidden parts
  $forbidden_keys = array(
    'upload',
    'filefield_upload',
    'data',
  );
  foreach ($forbidden_keys as $key) {
    $element[$key]['#access'] = FALSE;
  }
  return $element;
}

function _driven_cck_producer_consumer_validate($element, &$form_state, $model) {
  // $model must be 'producer' or 'consumer' (nothing else)
  // both validations share most of their code
  // (they only differ on the last is_empty verification)
  //
  // to test this function comment out the driven_access_modify at apply_to_form (last step)
  // that way every delta will have #access=TRUE
  // and this validator would be able to be exercised
  // - producer model
  // -- modifying an existent value should NOT be allowed
  // -- new values are allowed
  // -- new values can be modified on re-preview
  // - consumer model
  // -- entering new values should NOT be allowed
  // -- existing values can be emptied, but not modified
  //    (i.e. not a different non-empty value)
  
  $field_name = $element['#field_name'];
  $form_values = $form_state['values'];
  $node_type = $form_values['type'];
  $available_props = driven_properties_available($node_type);
  $prop = $available_props['cck:' . $field_name];
  if (!isset($prop)) {
    driven_assertion_failed('consumer validation failed to fetch the property: cck:' . $field_name . ' for content type: ' . $node_type);
    return;
  }
  // don't want the previewing node
  // instead we need the actual saved node
  // (otherwise incoming/outgoing will be applied to new deltas)
  $nid = $form_values['nid'];
  $node = node_load($nid);
  $items = $node->$field_name;
  foreach ($items as $delta => $item) {
    $element[$delta]['#default_value'] = $item;
  }
  
  $meta = $prop['meta'];
  $diff_values = driven_diff_elem_values($element, $form_values, $meta);
  if (!$diff_values) {
    // there were no changes at all
    return;
  }
  list($old, $new) = $diff_values;
  
  // @TODO: right now not so sure, but it seems to me that widgets with core's add_more will always have its #title
  // nevertheless it might be empty due to user's configuration, right?
  $label = empty($element['#title']) ? $field_name : $element['#title'];
  $function_is_empty = $meta['field_module'] . '_content_is_empty';
  if ($model == 'producer') {
    $offender = $new;
    $validate_empty = array();
    foreach ($old as $delta => $item) {
      if ($delta === $field_name . '_add_more') {
        continue;
      }
      // don't want the previewing node
      // instead we need the actual saved node
      // (otherwise incoming constraint will be applied to new deltas)
      $item = $items[$delta];
      // - $model is producer and not is_empty
      if (!$function_is_empty($item, $field)) {
        $validate_empty[$delta] = $old[$delta];
      }
    }
    $error_msg = t('You are only allowed to <em>produce</em> new values for %label. Not allowed to <em>consume</em> nor <em>modify</em> existing values.', array('%label' => $label));
  }
  else {
    $offender = $new; // seems that offender will be always the new value (user supplied data)
    $validate_empty = $new;
    $error_msg = t('You are only allowed to <em>consume</em> existent values of %label. Not allowed to <em>produce</em> new values nor <em>modify</em> existing ones.', array('%label' => $label));
  }
  
  foreach ($validate_empty as $index => $item) {
    if (!$function_is_empty($item, $field)) {
      $invalid_items = array(0 => $offender[$index]);
      // just using a helper function
      // (there shouldn't be any property gone, since this is expected to be used with fresh read properties)
      $field = driven_cck_get_field($node_type, $field_name);
      if ($field === FALSE) driven_assertion_failed('Couldn\'t fetch field: ' . $field_name . ' for content type: ' . $node_type);
      $node = (object)array(
        'type' => $node_type, 
        'build_mode' => NODE_BUILD_NORMAL
      );
      $function = $meta['field_module'] . '_field';
      if (function_exists($function)) {
        $function('sanitize', $node, $field, $invalid_items, FALSE, TRUE);
        $invalid_view = $function('view', $node, $field, $invalid_items, FALSE, TRUE);
      }
      // the module might implement hook_field but not necessarily the 'view' operation 
      if (!isset($invalid_view)) {
        $invalid_view = content_field('view', $node, $field, $invalid_items, FALSE, TRUE);
      }
      $invalid_render = array_shift($invalid_view[$field_name]['field']['items']);
      $invalid_render = drupal_render($invalid_render);
      form_error($element, t('Invalid value !value.', array('!value' => $invalid_render)) . ' ' . $error_msg);
      break;
    }
  }
}

function theme_driven_cck_producer_consumer_multiple_values($element) {
  // additions (e.g. _weight) are beneath the property path (for non-multigroup)
  // but theme_content_multiple_values is disregarding
  // #access=FALSE for individual deltas
  //
  // prepare to delegate to theme_content_multiple_values
  $field_name = $element['#field_name'];
  foreach (element_children($element) as $delta) {
    if ($delta === $field_name . '_add_more') {
      continue;
    }
    if (isset($element[$delta]['#access']) && !$element[$$delta]['#access']) {
      // modify our local copy (received by val)
      unset($element[$delta]);
    }
  }
   
  // @d6
  // delegate to theme_content_multiple_values
  return theme('content_multiple_values', $element);
}
