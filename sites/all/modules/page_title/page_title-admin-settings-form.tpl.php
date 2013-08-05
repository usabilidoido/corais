<?php

/**
 * @file
 * Template file for the admin settings form. Displays configuration in a neat table
 */

$rows = array();

foreach (element_children($form['pattern']) as $key) {
  $title = array(
    '#type' => 'item',
    '#title' => $form['pattern'][$key]['#title'],
    '#required' => $form['pattern'][$key]['#required'],
  );
  unset($form['pattern'][$key]['#title']);

  $row = array(
    array('data' => drupal_render($title), 'class' => 'page-type'),
    array('data' => drupal_render($form['scope'][$key]), 'class' => 'scope'),
  );
  if (isset($form['showfield'][$key .'_showfield'])) {
    $row[] = array('data' => drupal_render($form['pattern'][$key]), 'class' => 'pattern');
    $row[] = array('data' => drupal_render($form['showfield'][$key .'_showfield']), 'class' => 'showfield');
  }
  else {
    $row[] = array('data' => drupal_render($form['pattern'][$key]), 'colspan' => 2, 'class' => 'pattern');
  }

  $rows[] = $row;
}

$headers = array(
  array('data' => t('Page Type'),   'class' => 'page-type'),
  array('data' => t('Token Scope'), 'class' => 'scope'),
  array('data' => t('Pattern'),     'class' => 'pattern'),
  array('data' => t('Show Field'),  'class' => 'showfield'),
);


drupal_add_css(drupal_get_path('module', 'page_title') .'/page_title.admin.css', 'module', 'all', FALSE);
print theme('table', $headers, $rows, array('id' => 'page-title-settings'));

print drupal_render($form);
