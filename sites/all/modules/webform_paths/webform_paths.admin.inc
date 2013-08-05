<?php
/** 
 * @file webform_paths.admin.inc
 * Administrative methods for webform_paths module. 
 *
 * @author Omar Abdel-Wahab <owahab@owahab.com>
 * @copyright: KnowledgeTree Inc
 * @license: GPL 2 or any later version.
 * @date 2009-01-01
 */

/**
 * Menu callback: Confirms deleting a webform path.
 */
function webform_paths_delete_path_confirm($form_state, $pathid) {
  $path = webform_paths_load($pathid);
  if (user_access('administer webform paths')) {
    $form['pathid'] = array('#type' => 'value', '#value' => $path->pathid);
    $output = confirm_form($form,
      t('Are you sure you want to delete webform path %title?', array('%title' => $path->path)),
      isset($_GET['destination']) ? $_GET['destination'] : 'node/' . $path->nid .'/edit/webform_paths' );
  }
  return $output;
}

/** 
 * Executes webform path deletion.
 *
 * @param $form_id form id.
 * @param $form_state form state array.
 */
function webform_paths_delete_path_confirm_submit($form_id, &$form_state) {
  $path = webform_paths_load($form_state['values']['pathid']);
  path_set_alias(NULL, $path->path);
  db_query('DELETE FROM {webform_paths} WHERE pathid = %d', $path->pathid);
  drupal_set_message('Path deleted.');
  $form_state['redirect'] = 'node/' . $path->nid .'/edit/webform_paths';
}

/** 
 * Menu callback: Displays the settings page for a given webform node.
 * 
 * @param $node a node object. 
 * @param $pathid path id.
 * 
 * @return HTML representation of the webform settings page.
 */
function webform_paths_edit_node_settings($node, $pathid = NULL) {
  // Show saved paths
  $output = webform_paths_show_node_paths($node);
  if (is_null($output)) {
    $output = t('No paths defined for this webform.');
  }
  // Append the path form
  $output .= drupal_get_form('webform_paths_path_form', array('nid' => $node->nid, 'pathid' => $pathid));
  return $output;
}

/** 
 * Display a list of declared webform paths for a given node.
 * 
 * @param $node a node object. 
 * 
 * @return HTML table of declared webform paths.
 */
function webform_paths_show_node_paths($node) {
  $output = NULL;
  $header = array(
    'Path',
    'Title',
    array('data' => 'Operations', 'colspan' => 2),
  );
  $rows = array();
  $paths = webform_paths_get_node_paths($node);
  foreach ($paths as $pathid => $path) {
    $rows[] = array(
      l($path['path'], $path['path']),
      $path['title'],
      l('edit', 'node/' . $node->nid . '/edit/webform_paths/' . $path['pathid'] . '/edit'),
      l('delete', 'webform_paths/delete/'. $path['pathid']),
    );
  }
  if (count($rows)) {
    $output = theme('table', $header, $rows);
  }
  return $output;
}

/** 
 * Displays a creation or modification form for a given webform path.
 * 
 * @param $form_state form state array.
 * @param $values list of form values to override default values.
 * 
 * @return a form array. 
 */
function webform_paths_path_form($form_state, $values = array()) {
  if (!empty($values['pathid'])) {
    $path = webform_paths_load($values['pathid']);
    $values['title'] = $path->title;
    $values['path'] = $path->path;
    $values['message'] = $path->message;
  }
  $form = array();

  $form['set'] = array(
    '#type' => 'fieldset',
    '#title' => ($values['pathid'] ? $values['title'] : t('Add a new path')),
    '#collapsible' => TRUE,
    '#collapsed' => (bool) !$values['pathid'],
  );
  $form['set']['pathid'] = array(
    '#type' => 'hidden',
    '#value' => $values['pathid'],
  );
  $form['set']['nid'] = array(
    '#type' => 'hidden',
    '#value' => $values['nid'],
  );
  $form['set']['path'] = array(
    '#type' => 'textfield',
    '#title' => t('Path'),
    '#default_value' => $values['path'],
    '#required' => TRUE,
  );
  $form['set']['title'] = array(
    '#type' => 'textfield',
    '#title' => t('Title'),
    '#default_value' => $values['title'],
  );
  $form['set']['message'] = array(
    '#type' => 'textarea',
    '#title' => t('A message to display in the form page'),
    '#default_value' => $values['thankyou'],
  );
  $form['set']['save'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );
  $form['set']['cancel'] = array(
    '#type' => 'submit',
    '#value' => t('Cancel'),
  );

  return $form;
}

/** 
 * Handles webform path saving.
 * 
 * @param $form_id form id.
 * @param $form_state form state array.
 */
function webform_paths_path_form_submit($form_id, &$form_state) {
  if ($form_state['values']['op'] == t('Save')) {
    $path = $form_state['values'];
    if (webform_paths_path_save($path) !== FALSE) {
      drupal_set_message('Your configuration has been saved.');
    }
    else {
      drupal_set_message('Unable to save path.');
    }
  }
  $form_state['redirect'] = 'node/' . $form_state['values']['nid'] . '/edit/webform_paths';
}
