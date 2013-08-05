<?php

/**
 * Theme function for spaces_features_form().
 */
function theme_spaces_features_form($form) {
  drupal_add_css(drupal_get_path('module', 'spaces') .'/spaces.css');
  $rows = array();
  foreach (element_children($form['spaces_features']) as $feature) {
    $rows[] = array(
      array('class' => 'label', 'data' => drupal_render($form['labels'][$feature])),
      array('class' => 'option', 'data' => drupal_render($form['spaces_features'][$feature])),
      array('class' => 'actions', 'data' => drupal_render($form['settings'][$feature])),
    );
  }
  $output = theme('table', array(), $rows, array('class' => 'spaces'));
  $output .= drupal_render($form);
  return $output;
}

/**
 * Theme function for spaces_preset_form().
 */
function theme_spaces_preset_form($form) {
  drupal_add_css(drupal_get_path('module', 'spaces') .'/spaces.css');
  $header = array(array('data' => check_plain($form['#title']), 'colspan' => isset($form['storage']) ? 4 : 3));
  $type = $form['#space_type'];
  $rows = array();
  if (!empty($form['#description'])) {
    $rows[] = array(array('data' => "<div class='description'>{$form['#description']}</div>", 'colspan' => isset($form['storage']) ? 4 : 3));
  }
  foreach (element_children($form['labels']) as $preset) {
    $row = array();
    $row[] = array('class' => 'option', 'data' => drupal_render($form["spaces_preset_{$type}"][$preset]));
    $row[] = array('class' => 'label', 'data' => drupal_render($form['labels'][$preset]));
    if (isset($form['storage'][$preset])) {
      $row[] = array('class' => 'storage', 'data' => drupal_render($form['storage'][$preset]));
    }
    $row[] = array('class' => 'actions', 'data' => drupal_render($form['actions'][$preset]));
    $rows[] = array(
      'class' => !empty($form["spaces_preset_{$type}"][$preset]) ? 'enabled' : 'disabled',
      'data' => $row,
    );
  }
  $output = theme('table', $header, $rows, array('class' => 'spaces'));
  $output .= drupal_render($form);
  return $output;
}

/**
 * Theme function for spaces_overrides_form.
 */
function theme_spaces_overrides_form($form) {
  drupal_add_js(drupal_get_path('module', 'spaces') . '/spaces.js');
  drupal_add_css(drupal_get_path('module', 'spaces') .'/spaces.css');
  $header = array(
    theme('table_select_header_cell'),
    array('data' => t('Type'), 'colspan' => 1),
    array('data' => t('Override value'), 'colspan' => 2),
  );
  $rows = array();
  foreach (array_keys(spaces_controllers()) as $controller) {
    if (!empty($form[$controller])) {
      foreach (element_children($form[$controller]) as $key) {
        $label = $form[$controller][$key]['#title'];
        $value = $form[$controller][$key]['#description'];
        $inherited = !empty($form[$controller][$key]['#disabled']);
        unset($form[$controller][$key]['#title'], $form[$controller][$key]['#description']);

        $row = array(
          'data' => array(
            array('class' => 'option', 'data' => drupal_render($form[$controller][$key])),
            array('class' => 'controller', 'data' => $controller),
            array(
              'class' => 'key',
              'data' => l($label, $_GET['q'], array('fragment' => $key)) . '<span class="override-value"><pre>' .  $value . '</pre></span>',
              'colspan' => $inherited ? 1 : 2,
            ),
          ),
        );
        if ($inherited) {
          $row['class'] = 'inherited';
          $row['data'][] = array('class' => 'tag', 'data' => t('inherited'));
        }
        $rows[] = $row;
      }
    }
  }
  if (count($rows)) {
    if (isset($form['preset'])) {
      $preset_label = $form['preset']['#title'];
      unset($form['preset']['#title']);
      $rows[] = array(
        array('data' => $preset_label, 'class' => 'action-label', 'colspan' => 2),
        array('data' => drupal_render($form['preset']), 'class' => 'action-form', 'colspan' => 3),
      );
    }
    if (isset($form['revert'])) {
      $revert_label = $form['revert']['#title'];
      unset($form['revert']['#title']);
      $rows[] = array(
        array('data' => $revert_label, 'class' => 'action-label', 'colspan' => 2),
        array('data' => drupal_render($form['revert']), 'class' => 'action-form', 'colspan' => 3),
      );
    }
  }
  else {
    $rows[] = array(array('data' => t('No overrides present.'), 'colspan' => 5));
  }
  $output = theme('table', $header, $rows, array('class' => 'spaces'));
  $output .= drupal_render($form);
  return $output;
}
