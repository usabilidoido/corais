<?php

/**
 * @file Helper file for Theming date_picker_formatter module output
 */

/**
 * Theming function for date element formatter
 */
function theme_date_picker_formatter_formatter_default($element) {
  // We're going to build a form to get user votes
  return drupal_get_form('date_picker_formatter_element_form', $element);
}

/**
 * Format the voting users count
 */
function theme_date_picker_formatter_stats_count($count) {
  return format_plural($count, '1 User votes', '@count Users votes');
}

/**
 * Format a voting stats cell 
 */
function theme_date_picker_formatter_stats_cell($info) {
  // dpm($info);
  $vote_options = _date_picker_formatter_options();
  $items = array();
  foreach ($info as $id => $count) {
    $items[] =  format_plural($count, "@votelabel : 1 Vote", "@votelabel : @count Votes", array('@votelabel' => $vote_options[$id]));
  }

  return theme('item_list', $items);
}


/**
 * Output a header cell
 */
function theme_date_picker_formatter_date_cell($info) {
  extract($info);
  $output = '';
  
  if ($date != $date2) {
    $output .= '<span class="date first">'. $date . ($time ? ' <span class="time">'. $time .'</span>' : '') .'</span> ';
    $output .= '<span class="separator">-</span> ';
    $output .= '<span class="date second">'. $date2 . ($time2 ? ' <span class="time">'. $time2 .'</span>' : '') .'</span>';
  }
  else {
    $output .= '<span class="date">'. $date .'</span>';
  }
  
  return $output;
}

function theme_date_picker_formatter_time_cell($info) {
  extract($info);
  $output = '';
  
  if ($date == $date2) {
    if ($time != $time2) {
      $output .= '<span class="time first">'. $time .'</span> ';
      $output .= '<span class="separator">-</span> ';
      $output .= '<span class="time second">'. $time2 .'</span>';
    }
    else {
      $output .= '<span class="time">'. $time .'</span>';
    }
  }
  
  return $output;
}

/**
 * Theming function for "User" cell
 */
function theme_date_picker_formatter_user_cell($uid) {
  global $user;
  $account = user_load($uid);
  $output = '';
  if (variable_get('date_picker_formatter_display_user_picture', 1)) {
    $output .= theme('user_picture', $account);
  }
 
  // Use the default theme_username to show voting user
  $output .= theme('username', $account);
  
  if ($account->uid == $user->uid) {
    $output .= ' (' . t('You') . ')';
  }
  return $output;
}

/**
 * Theming function for the "Vote" table cell ($vote['label'] and $vote['value']
 */
function theme_date_picker_formatter_vote_cell($vote) {
  return '<span class="vote vote-' . $vote['value'] . '">' . $vote['label'] . '</span>';
}

/**
 * Theme function for  the date_picker_formatter form
 * @param $form
 * @return string
 */
function theme_date_picker_formatter_element_form($form) {
  global $user;
  
  // Don't display form is no #infos provided (something went wrong)
  if (empty($form['dates']['#infos']))
    return '';
  
  drupal_add_css(drupal_get_path('module', 'date_picker_formatter') . '/date_picker_formatter.css');
  
  $output = '';
  $rows = array();
  // $children = element_children($form['dates']);
  // $first_child = array_shift($children);
  
  $header = array();
  $header[] = array('data' => t('Users'), 'rowspan' => 2, 'class' => 'user-header');
  
  $second_row = array(
    array(
      'data' => '<!-- top left corner time -->',
      'class' => 'header'
    ),
  );

  $infos = $form['dates']['#infos'];
  $prev_key = array_shift(array_keys($infos));
  $prev_date = NULL;
  
  foreach ($infos as $key => $info) {
    if ($info['date'] != $info['date2'] || $prev_date != $info['date']) {
      $header[$key] = array(
        'class' => 'date-header',
        'data' => theme('date_picker_formatter_date_cell', $info),
      );
      $prev_key = $key;
    }
    else {
      $header[$prev_key]['colspan'] = !$header[$prev_key]['colspan'] ? 1 : $header[$prev_key]['colspan'];
      $header[$prev_key]['colspan']++;
    }
    $prev_date = $info['date'];

    $second_row[$key] = array(
      'class' => 'time',
      'data' => theme('date_picker_formatter_time_cell', $info),    
    );
    
  }
  
  $rows['times'] = array('class' => 'time header', 'data' => $second_row);
  // $header = array($header, $second_row);
  
  foreach (element_children($form['dates']) as $uid) {
    $row = array(
      'user' => array(
        'class' => 'user header',
        'data' => theme('date_picker_formatter_user_cell', $uid),
      )
    );
    foreach (element_children($form['dates'][$uid]) as $key) {
      unset($form['dates'][$uid][$key]['#title']);

      $row[$key] = array(
        'class' => 'answer ' . @$form['dates'][$uid][$key]['#attributes']['class'],
        'data' => drupal_render($form['dates'][$uid][$key]),
      );
    }
    $rows[$uid] = array('class' => 'result', 'data' => $row);
  }
  
  // Add to the current user row the "Submit" button
  if (isset($rows[$user->uid])) {
    $rows['current_user'] = $rows[$user->uid];
    unset($rows[$user->uid]);
    $rows['current_user']['data']['user']['data'] .= ' '. drupal_render($form['submit']);
    
    // Adding CSS for current-user row theming
    $rows['current_user']['class'] .= ' current-user';
  }
  
  // Adding stats
  $stats = date_picker_formatter_get_stats($form['#date_picker_formatter_votes'], array_keys($infos));
  // dvm($stats);
  
  $stats_row = array(
    'class' => 'stats',
    'data' => array(
      'count' => theme('date_picker_formatter_stats_count', $stats['voting_users']),
    ),
  );

  foreach ($stats['votes'] as $date_votes) {
    $stats_row['data'][] = array(
      'class' => 'answer-stats ' . _date_picker_formatter_get_winner_class($date_votes) ,
      'data' => theme('date_picker_formatter_stats_cell', $date_votes)
    );
  }
  
  $rows[] = $stats_row;
  
  $output .= '<div class="date-picker-formatter-table-wrapper">';
  $output .= theme('table', $header, $rows, array(
    'id' => 'date-picker-formatter-table-' . $form['#node']->nid . '-' . str_replace('_', '-', $form['#field']['field_name']),
    'class' => 'date-picker-formatter-table'
  ));
  
  $output .= '</div>';
  
  return $output . drupal_render($form);
}
