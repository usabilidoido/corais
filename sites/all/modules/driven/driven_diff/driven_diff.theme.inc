<?php
// $Id: driven_diff.theme.inc,v 1.2 2010/04/01 01:08:01 arhak Exp $

// it is aimed that every call to driven_diff_output
// can be replaced with a direct theme invocation
// therefore, its signature matches core's theme function
// (D7 fashion, the same as functions declared by this module)
function driven_diff_output($theme, $variables) {
  if (!empty($theme)) {
    // proxy function to avoid rendering nothing
    // for instance, when invoking a non-existent theme function
    // (which was already addressed in D7 with a watchdog warning)
    $themed = theme($theme, $variables);
    if (!empty($themed)) {
      return $themed;
    }
  }
  // if it is a misnamed variable
  // then print whatever received on front
  $value = isset($variables['value']) ? $variables['value'] : array_shift($variables);
  if (is_array($value)) {
    // do the best we can to display an array through check_plain
    $value = implode(', ', $value);
  }
  return check_plain($value);
}

/**
 * Implements hook_theme().
 */
function driven_diff_theme() {
  // @d6 'arguments' => array('variables' => array(...))
  // @d7 'variables' => array(...)
  
  $theme['driven_diff_simple_table'] = array(
    'arguments' => array('variables' => array('rows' => '')),
  );
  $theme['driven_diff_simple_ulist'] = array(
    'arguments' => array('variables' => array('rows' => '')),
  );
  $theme['driven_diff_link'] = array(
    'arguments' => array('variables' => array('value' => '')),
  );
  $theme['driven_diff_boolean'] = array(
    'arguments' => array('variables' => array('value' => '', 'options' => NULL)),
  );
  $theme['driven_diff_yes_no'] = array(
    'arguments' => array('variables' => array('value' => '')),
  );
  $theme['driven_diff_comment_settings'] = array(
    'arguments' => array('variables' => array('value' => '')),
  );
  $theme['driven_diff_username'] = array(
    'arguments' => array('variables' => array('value' => '')),
  );
  $theme['driven_diff_term'] = array(
    'arguments' => array('variables' => array('value' => '')),
  );
  $theme['driven_diff_workflow_schedule'] = array(
    'arguments' => array('variables' => array('schedule' => '')),
  );
  $theme['driven_diff_outgoing'] = array(
    'arguments' => array('variables' => array('content' => '', 'classes' => array())),
  );
  $theme['driven_diff_incoming'] = array(
    'arguments' => array('variables' => array('content' => '', 'classes' => array())),
  );
  return $theme;
}

// this way it can receive rows already sanitized or not
function theme_driven_diff_simple_table($variables) {
  $rows = $variables['rows'];
  // avoid printing tags for nothing
  if (empty($rows)) return '';
  
  $output = '<table><tbody>';
  foreach ($rows as $row) {
    $output .= '<tr>';
    foreach ($row as $cell) {
      $output .= '<td>' . $cell . '</td>';
    }
    $output .= '</tr>';
  }
  $output .= '</tbody></table>';
  return $output;
}

// this way it can receive rows already sanitized or not
function theme_driven_diff_simple_ulist($variables) {
  $rows = $variables['rows'];
  // avoid printing tags for nothing
  if (empty($rows)) return '';
  
  $output = '<ul>';
  foreach ($rows as $row) {
    $output .= '<li>' . implode(' ', $row) . '</li>';
  }
  $output .= '</ul>';
  return $output;
}

function theme_driven_diff_link($variables) {
  $path = $variables['value'];
  if (empty($path)) {
    return '';
  }
  return l($path, $path);
}

function theme_driven_diff_boolean($variables) {
  $boolean = $variables['value'];
  if ($boolean === '') {
    return '';
  }
  // options should come untranslated
  $options = $variables['options'];
  if (empty($options)) {
    $options = array(1 => 'True', 0 => 'False');
  }
  $boolean = (bool)$boolean; // careful, and integer might get out of bounds
  $value = $options[$boolean];
  // this is the reason to have value not translated yet
  $class = strtolower(str_replace(' ', '-', $value));
  // translating a dinamic value,
  // but it will be a well known translation to be provided
  // (see theme_driven_diff_comment_settings to notice the difference between class/meaning)
  $value = t($value);
  return '<span class="boolean ' . $class . ' ' . ($boolean ? 'true' : 'false') . '">' . check_plain($value) . '</span>';
}

function theme_driven_diff_yes_no($variables) {
  $variables['options'] = array(1 => 'Yes', 0 => 'No');
  return theme_driven_diff_boolean($variables);
}

function theme_driven_diff_comment_settings($variables) {
  $flag = $variables['value'];
  switch ($flag) {
    case 0:
      $meaning = t('Disabled');
      $class = 'disabled';
      break;
    case 1:
      $meaning = t('Read only');
      $class = 'read-only';
      break;
    case 2:
      $meaning = t('Read/Write');
      $class = 'read-write';
      break;
  }
  return '<span class="comment-settings ' . $class . '">' . $meaning . '</span>';
}

function theme_driven_diff_username($variables) {
  $username = $variables['value'];
  return theme('username', user_load(array('name' => $username)));
}

function theme_driven_diff_term($variables) {
  $term = $variables['value'];
  return l($term->name, taxonomy_term_path($term));
}

function theme_driven_diff_workflow_schedule($variables) {
  $schedule = $variables['schedule'];
  if ($schedule['workflow_scheduled']) {
    // see how $scheduled is built at workflow_transition (workflow-6.x-1.4)
    // and how format_date handles timezone
    // to know why this was the approach taken 
    //
    // take advantage of reverse alphabetical order of indexes: year/month/day
    $date = $schedule['workflow_scheduled_date'];
    // careful with single digits for month/day
    // (why isn't workflow using a hyphen separator?)
    $date = implode('-', $date);
    $time = $schedule['workflow_scheduled_hour'];
    // workflow substracts timezone before evaluating transitions
    // which is added by format_date on display time
    // and which timezone is time() delivering for workflow_cron?
    $scheduled = $date . ' ' . $time;
    $timestamp = strtotime($scheduled);
    $datetime = format_date($timestamp);
  }
  else {
    $datetime = 'Immediately';
  }
  return check_plain($datetime);
}

function theme_driven_diff_outgoing($variables) {
  $content = $variables['content'];
  $classes = $variables['classes'];
  return _theme_driven_diff_multiple($content, TRUE, $classes);
}

function theme_driven_diff_incoming($variables) {
  $content = $variables['content'];
  $classes = $variables['classes'];
  return _theme_driven_diff_multiple($content, FALSE, $classes);
}

function _theme_driven_diff_multiple($content, $outgoing, $classes) {
  $wrapper = 'diff ' . ($outgoing ? 'outgoing' : 'incoming') . '-diff';
  if (empty($content)) {
    $classes[] = 'empty';
  } 
  $classes = implode(' ', $classes);
  $mark = ($outgoing ? '-' : '+');
  // @TODO: needs theme review this "mark" thing
  // (e.g. numeric values still get the sign in tokens, which are text plain, i.e. strip_tags)
  return '<span class="' . $wrapper . '"><span class="' . $classes . '"><span class="mark">' . $mark . '</span>' . $content . '</span></span>';
}
