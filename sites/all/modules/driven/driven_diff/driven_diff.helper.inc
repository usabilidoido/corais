<?php
// $Id: driven_diff.helper.inc,v 1.3 2010/04/03 23:18:07 arhak Exp $

// helper functions for property provider modules

function driven_diff_values_non_array($element, $form_values) {
  $default = driven_elem_default_value($element);
  // $value will be normized if needed (per case basis)
  // to match #default_value format instead of $form_state['values']
  $value = driven_elem_state_value($element, $form_values);
  
  if (_driven_equal_non_array($default, $value)) {
    return FALSE;
  }
  return array($default, $value);
}

function driven_diff_values_keyed_array_in_depth($element, $form_values) {
  $default = driven_elem_default_value($element);
  // $value will be normized if needed (per case basis)
  // to match #default_value format instead of $form_state['values']
  $value = driven_elem_state_value($element, $form_values);
  
  if (driven_equal_keyed_array_in_depth($default, $value)) {
    return FALSE;
  }
  return array($default, $value);
}

function driven_diff_output_non_array($outgoing, $incoming, $theme = '', $theme_varname = 'value') {
  $outgoing = driven_diff_output($theme, array($theme_varname => $outgoing));
  $incoming = driven_diff_output($theme, array($theme_varname => $incoming));
  return array($outgoing, $incoming);
}

function driven_diff_drop_keys(&$arr, $keys) {
  foreach ($keys as $key) {
    unset($arr[$key]);
  }
}
