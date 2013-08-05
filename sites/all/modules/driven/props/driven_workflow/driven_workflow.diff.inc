<?php
// $Id: driven_workflow.diff.inc,v 1.2 2010/04/03 23:18:08 arhak Exp $

/**
 * Implements hook_diff_values().
 */
function driven_workflow_diff_values($element, $form_values, $meta) {
  extract($meta); // $node_type, $realm, $target, $provider, $wid
  
  $workflow = driven_workflow_get_workflow($wid, $node_type);
  if (!$workflow) {
    // can't guess, can't normalize $value
    // can't afford being asked for diff_render
    return FALSE; // nothing to declare (i.e. return no diff_values)
  }
  
  if ($element['#type'] === 'date') {
    return driven_diff_values_keyed_array_in_depth($element, $form_values);
  }
  
  return driven_diff_values_non_array($element, $form_values);
}

/**
 * Implements hook_diff_render().
 */
function driven_workflow_diff_render($old, $new, $meta) {
  extract($meta); // $node_type, $realm, $target, $provider, $wid

  $workflow = driven_workflow_get_workflow($wid, $node_type);
  if (!$workflow) {
    // can't say, but if we got here was because diff_values delivered something (?!)
    driven_assertion_failed('Can\'t figure diff_render of a gone property: ' . $realm . ':' . $target);
    return array('?', '?');
  }

  // the parent to which the state's radios point
  // (which is an empty string passed from workflow_form_alter to workflow_node_form)
  if ($target == 'workflow:') {
    $outgoing = driven_workflow_get_state_name($old, $wid);
    if (!$outgoing) {
      // value gone was already reported by driven_workflow_get_state_name
      // (avoid rendering empty sides if value is gone)
      $outgoing = '?';
    }
    $incoming = driven_workflow_get_state_name($new, $wid);
    if (!$incoming) {
      // value gone was already reported by driven_workflow_get_state_name
      // (avoid rendering empty sides if value is gone)
      $incoming = '?';
    }
  }
  elseif ($target == 'workflow:bundled_schedule') {
    $outgoing = driven_diff_output('driven_diff_workflow_schedule', array('schedule' => $old));
    $incoming = driven_diff_output('driven_diff_workflow_schedule', array('schedule' => $new));
  }
  else {
    $outgoing = $old;
    $incoming = $new;
  }
  
  // $theme might be NULL
  return driven_diff_output_non_array($outgoing, $incoming, $theme);
}
