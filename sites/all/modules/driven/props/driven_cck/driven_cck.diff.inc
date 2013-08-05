<?php
// $Id: driven_cck.diff.inc,v 1.3 2010/04/16 15:42:15 arhak Exp $

/**
 * Implements hook_diff_values().
 */
function driven_cck_diff_values($element, $form_values, $meta) {
  // $node_type, $realm, $target, $provider,
  // $group_name/type, $field_name/type/module/multiple, $widget_type/module/handle_multiple_values
  // $driven_diff (note that driven_diff was added in driven_diff_driven_properties_meta_extension)
  extract($meta);
  
  $field = driven_cck_get_field($node_type, $field_name);
  if (!$field) {
    // can't guess, can't normalize $value
    // can't afford being asked for diff_render
    return FALSE; // nothing to declare (i.e. return no diff_values)
  }
  
  $default = driven_elem_default_value($element);
  // $value will be normized if needed (per case basis)
  // to match #default_value format instead of $form_state['values']
  $value = driven_elem_state_value($element, $form_values);
  
  
  // @per-module-discrimination
  switch ($field['module']) {
    case 'filefield':
      // drop: UPLOAD_IDENTIFIER, APC_UPLOAD_PROGRESS, filefield_upload, filefield_remove, upload
      // (see filefield_widget_process)
      // none of those indexes are meant to get it
      // since they are not #default_value format
      $keys = array(
        'UPLOAD_IDENTIFIER',
        'APC_UPLOAD_PROGRESS',
        'filefield_upload',
        'filefield_remove',
        'upload',
      );
      driven_diff_drop_keys($value, $keys);
      // on preview $default needs cleanup as well,
      // because it comes from $from_state['values'] (hackish node preview)
      driven_diff_drop_keys($default, $keys);
      break;
      
    case 'date':
      // doc in _date_widget states:
      // It would seem to make sense to do this conversion when the data
      // is loaded instead of when the form is created, but the loaded
      // field data is cached and we can't cache dates that have been converted
      // to the timezone of an individual user, so we cache the UTC values
      // instead and do our conversion to local dates in the form and
      // in the formatters.
// @TODO: temporal testing repeat versions of date widgets
//$value = $value[0];
      $timezone = date_get_timezone($field['tz_handling'], isset($default['timezone']) ? $default['timezone'] : date_default_timezone_name());
      // date_process_values is only a helper function which does
      // return $field['todate'] ? array('value', 'value2') : array('value');
      $process = date_process_values($field);
      foreach ($process as $processed) {
        if (!isset($value[$processed])) {
          $value[$processed] = '';
        }
        $date = date_local_date(NULL, NULL, NULL, $value, $timezone, $field, $processed);
        $value[$processed] = is_object($date) ? date_format($date, DATE_FORMAT_DATETIME) : '';
      }

      // $default has 'timezone_db' & 'date_type' and both have 'timezone',
      // none of those indexes are meant to get it
      // since they were added in validation time,
      // therefore they are not #default_value format
      // thus, prepare them both for comparison
      $keys = array(
        'timezone',
        'timezone_db',
        'date_type',
      );
      driven_diff_drop_keys($default, $keys);
      driven_diff_drop_keys($value, $keys);
      // remove from $value keys which are not present in $default
      // drop: offset, offset2, and also value2 if no $field['todate']
      $keys = array(
        'offset',
        'offset2',
      );
      if (empty($field['todate'])) {
        $keys[] = 'value2';
      }
      driven_diff_drop_keys($value, $keys);
      // on preview $default needs cleanup as well,
      // because it comes from $from_state['values'] (hackish node preview)
      driven_diff_drop_keys($default, $keys);
      break;
      
    // @per-module-discrimination: support taxonomy-related content_taxonomy widgets
    case 'content_taxonomy':
      // if it is a widget known by driven_taxo, then delegate it diff_value
      // otherwise, just delegate it on diff_render
      
      // @per-module-discrimination: support hs_content_taxonomy
      if ($field['widget']['module'] == 'hs_content_taxonomy') {
        $meta += $driven_diff['taxo_meta'];
        return driven_taxo_diff_values($element, $form_values, $meta);
      }
  
      return _driven_cck_content_taxonomy_diff_values($default, $value);
  }

  
  // multiple:
  // 0 = FALSE => single value
  // 1 = TRUE => Ulimited => AHAH add_more
  // 2+ = TRUE => Limited
  //
  // see content_field_form
  //
  // If the widget is handling multiple values (e.g optionwidgets),
  // or selecting an individual element, just get a single form
  // element and make it the $delta value.
  //
  // If we're processing a specific delta value for a field where the
  // content module handles multiples, set the delta in the result.
  // For fields that handle their own processing, we can't make assumptions
  // about how the field is structured, just merge in the returned value.
  //
  // @TODO: @per-module-discrimination: "can't make assumptions about how the field is structured"
  //
  // - if it is a single item then it gets here with its proper array
  // - if it is multiple items then it gets here by pieces
  //   being each single item a respective delta, which need to be wrapped into an array
  // - exception: multiple items handled altogether instead of by pieces (e.g. checkboxes)
  //
  // discriminate based on whether #default_value is an array of delta=>item
  // or just an direct item (without the wrapper parent)
  //
  // deliberately not using neither: driven_elem_has_default_value_key, driven_elem_default_value
  $separated_item = !isset($element['#default_value'][$element['#delta']]);
  
  // wrap separated item into items
  if ($separated_item) {
    $default = array($default);
    $value = array($value);
  }
  
  // @per-module-discrimination: support number (decimal)
  // otherwise "string vs double" comparison won't work
  // having non-significant zeros [#461536-2]
  // then normilizing to $value to meet $default_value format
  // would be converting it to string and pad decimal places
  // with zeros as much as scale
  if ($field['type'] == 'number_decimal') {
    foreach ($value as $delta => $item) {
      $val = &$value[$delta]['value'];
      
      // wrapper detection in unstable6 was doing a special exception for numbers
      // but now (being under a generic/unified wrapper detection)
      // this point is being reached having an empty string $value
      // which compared to $default shouldn't be zero-padded
      // instead it should be normalized to NULL
      // (whether $default might be NULL or not)
      // BUT having a zero value should be normalized to '0' with zero-pads
      if (is_null($val) || $val === '') {
        $val = NULL;
        continue;
      }
      
      // same treatment as proposed at [#461536-7]
      // Note that it should be a double value (or a string if CCK patch was applied)
      // and its decimal separator will be a dot regarless of the field['decimal'] separator
      list($units, $fraction) = explode('.', $val);
      
      // fraction are normally expected to be null if the value is integer
      // but, units are supposed to be something (unless being an empty string)
      // but if it is an empty string, then we got here through a wrong path
      // because the string $value is not normalized to NULL when it should
      //if (empty($units)) {
      //  $units = '0';
      //}
      
      if (is_null($fraction)) {
        $fraction = '';
      }
      $fraction = str_pad($fraction, $field['scale'], '0');
      $val = $units . '.' . $fraction;
    }
    // on preview $default needs cleanup as well,
    // because it comes from $from_state['values'] (hackish node preview)
    // otherwise when previewing it will be double vs string
    foreach ($default as $delta => $item) {
      $val = &$default[$delta]['value'];
      // having a zero value should be normalized to '0' with zero-pads
      if (is_null($val) || $val === '') {
        $val = NULL;
        continue;
      }
      list($units, $fraction) = explode('.', $val);
      if (is_null($fraction)) {
        $fraction = '';
      }
      $fraction = str_pad($fraction, $field['scale'], '0');
      $val = $units . '.' . $fraction;
    }
  }
  
  // filter out empty values
  // @TODO: until now it seems that CCK sub-modules doesn't implement their own presave
  content_field('presave', $node, $field, $default, FALSE, TRUE);
  // $value is passed by ref and cleared through content_set_empty
  content_field('presave', $node, $field, $value, FALSE, TRUE);
  
  // @per-module-discrimination: support text (format)
  // in passing by, fix any other module that might come up with the same issue
  // CCK text:
  // a text field can be set with input format
  // even when choosing a widget that won't support it
  // e.g. checkboxes/radios
  // in that case #default_value contains items with
  // 'value' and uninitialized 'format'
  // and marvelous function content_set_empty
  // won't complete the missing columns with NULL
  // it just handles the missing whole items
  foreach ($default as $delta => $item) {
    // the easiest way to iterate over empty valued keys
    foreach (array_keys($default[$delta]) as $key) {
      if (!isset($default[$delta][$key]) && isset($value[$delta]) && !isset($value[$delta][$key])) {
        // nevermind if it was NULL already
        $value[$delta][$key] = NULL; // make it even to #default_value
      }
    }
  }
  
  // unwrap separated item from items
  if ($separated_item) {
    // pick only the first item
    // since remaning items were filled with equivalent empty CCK values
    $default = array_shift($default);
    $value = array_shift($value);
    
    // dismiss non-value meta
    // '_weight' should come cleared from 'presave' (_content_sort_items)
    // but it won't harm to cover it here as well (just in case)
    // '_remove' shows up when using multigroups
    // (if it would be certain whipping out every underscored key...)
    $keys = array(
      '_weight',
      '_error_element',
      '_remove',
    );
    driven_diff_drop_keys($value, $keys);
    
    // @TODO: every call to driven_assertion_failed will be removed after alpha/beta stage 
    if (isset($default['#_error_element'])) driven_assertion_failed('isset($default[\'#_error_element\']) path: ' . implode(':', $element['#array_parents']));
    // clearing $default shouldn't be needed, but it seems it does
    // when AHAH is involved the form might come back with _error_element
    driven_diff_drop_keys($default, $keys);
  }
  
  // from this point $value is no longer passed by ref
  if (driven_equal_keyed_array_in_depth($default, $value)) {
    return FALSE;
  }
  return array($default, $value);
}

function _driven_cck_content_taxonomy_diff_values($default, $value) {
  // most of the cases it will hold arrays of pairs 'value'=>tid
  // if empty, $default will have 'value'=>NULL (according to content_taxonomy_autocomplete_widget)
  // if empty, $value will be an empty array
  // when new tags are forbidden and validation fails, then $value will be 'value'=>'imploded tags' (i.e. comma separated list)
  // if new terms are allowed tids will always exists, since content_taxonomy_autocomplete creates terms even on preview
  //
  // we are going to compare tids, but $value will be normalized to #default_value format
  $default_tids = array();
  $value_tids = array();
  foreach ($default as $pair) {
    $tid = $pair['value'];
    if (!is_null($tid)) {
      $default_tids[] = $tid;
    }
  }
  if (empty($value)) {
    $value = array('value' => NULL); // normalized to #default_value
  }
  else {
    foreach ($value as $pair) {
      $tid = $pair['value'];
      // note that being imploded tags means validation failed,
      // and therefore there is no need to proceed
      $tid = (int)$tid; // imploded tags will evaluate to zero (a non-valid tid)
      if (empty($tid)) {
        // lets do anyway the best we can (i.e. ensure no diff)
        $value_tids = $default_tids;
        break;
      }
      $value_tids[] = $tid;
    }
  }
  if (driven_equal_array_values($default_tids, $value_tids)) {
    return FALSE;
  }
  return array($default, $value);
}

/**
 * Implements hook_diff_render().
 */
function driven_cck_diff_render($old, $new, $meta) {
  // $node_type, $realm, $target, $provider,
  // $group_name/type, $field_name/type/module/multiple, $widget_type/module/handle_multiple_values
  // $driven_diff (note that driven_diff was added in driven_diff_driven_properties_meta_extension)
  extract($meta);
  
  $field = driven_cck_get_field($node_type, $field_name);
  if (!$field) {
    // can't say, but if we got here was because diff_values delivered something (?!)
    driven_assertion_failed('Can\'t figure diff_render of a gone property: ' . $realm . ':' . $target);
    return array('?', '?');
  }
  
  // @per-module-discrimination: support taxonomy-related content_taxonomy widgets
  if ($field['module'] == 'content_taxonomy') {
    // if it is a widget known by driven_taxo, then delegate it diff_value
    // otherwise, just delegate it on diff_render
    $meta += $driven_diff['taxo_meta'];
    
    // @per-module-discrimination: support hs_content_taxonomy
    if ($widget_module == 'hs_content_taxonomy') {
      // pass through tids arrays,
      // since driven_diff_elem_values took care of the wrapper
      // but driven_taxo won't be aware of it
      $old = $old['tids']; 
      $new = $new['tids'];
      // fall through driven_taxo_diff_render delegation
    }
    else {
      // diff_values delivered arrays of 'value'=>tid pairs
      $old_pairs = $old;
      $old = array();
      foreach ($old_pairs as $pair) {
        $old[] = $pair['value'];
      }
      $new_pairs = $new;
      $new = array();
      foreach ($new_pairs as $pair) {
        $new[] = $pair['value'];
      }
      // prepare to fall through driven_taxo_diff_render delegation
      // note that a pair (at most) could be 'value'=>NULL
      // and therefore we need to discard a possible NULL tid with array_filter
      $old = array_filter($old);
      $new = array_filter($new);
    }
    return driven_taxo_diff_render($old, $new, $meta);
  }
  
  $node = (object)array(
    'type' => $node_type, 
    'build_mode' => NODE_BUILD_NORMAL
  );

  list($old, $new) = driven_diff_symmetric_diff($old, $new);
  
  $function = $field['module'] . '_field';
  if (function_exists($function)) {
    $function('sanitize', $node, $field, $old, FALSE, TRUE);
    $old_view = $function('view', $node, $field, $old, FALSE, TRUE);
    
    $function('sanitize', $node, $field, $new, FALSE, TRUE);
    $new_view = $function('view', $node, $field, $new, FALSE, TRUE);
  }
  // the module might implement hook_field but not necessarily the 'view' operation 
  if (!isset($old_view)) {
    $old_view = content_field('view', $node, $field, $old, FALSE, TRUE);
    $new_view = content_field('view', $node, $field, $new, FALSE, TRUE);
  }
  
  $outgoing = array();
  $incoming = array();
  $classes = array($field['type'], $field['field_name']);
  // old/new deltas don't have to match
  
  foreach ($old_view[$field_name]['field']['items'] as $delta => $item) {
    $rendered = drupal_render($item);
    if (!empty($rendered)) {
      if ($field['multiple']) {
        $rendered = driven_diff_output('driven_diff_outgoing', array(
          'content' => $rendered,
          'classes' => $classes,
        ));
      }
      $outgoing[] = $rendered;
    }
  }
  foreach ($new_view[$field_name]['field']['items'] as $delta => $item) {
    $rendered = drupal_render($item);
    if (!empty($rendered)) {
      if ($field['multiple']) {
        $rendered = driven_diff_output('driven_diff_incoming', array(
          'content' => $rendered,
          'classes' => $classes,
        ));
      }
      $incoming[] = $rendered;
    }
  }
  
  return array($outgoing, $incoming);
}
