<?php
// $Id: casetracker_admin.inc,v 1.2.2.10 2010/06/23 18:32:08 jmiccolis Exp $

/**
 * Configures the various Case Tracker options; system_settings_form().
 */
function casetracker_settings() {
  $form = array();

  $form['casetracker_general'] = array(
    '#type' => 'fieldset',
    '#title' => t('General settings'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['casetracker_general']['casetracker_default_assign_to'] = array(
    '#type' => 'textfield',
    '#title' => t('Default assigned user'),
    '#autocomplete_path' => 'casetracker_autocomplete',
    '#required' => TRUE,
    '#default_value' => casetracker_default_assign_to(),
    '#description' => t('User to be assigned the case if one is not explicitly defined.'),
    '#element_validate' => array('casetracker_default_assign_to_validate'),
  );
  $form['casetracker_general']['casetracker_assign_to_widget'] = array(
    '#type' => 'select',
    '#title' => t('Assign to user widget'),
    '#options' => array(
      'flexible' => t('Flexible'),
      'select' => t('Select'),
      'radios' => t('Radios'),
      'autocomplete' => t('Autocomplete'),
    ),
    '#description' => t('Define which widget use to select the assigned user for a case. If you choose <strong>Flexible</strong> Case Tracker will use radios, select and autocomplete as widget depending of how many users can be assigned to the case.'),
    '#default_value' => variable_get('casetracker_assign_to_widget', 0),
  );

  foreach (array('priority', 'status', 'type') as $realm) {
    $options = casetracker_realm_load($realm);
    $temp_keys = array_keys($options);
    $form['casetracker_general']['casetracker_default_case_'. $realm] = array(
      '#type' => 'select',
      '#options' => $options,
      '#title' => t('Default case %realm', array('%realm' => $realm)),
      '#default_value' => variable_get('casetracker_default_case_'. $realm, array_shift($temp_keys)),
      '#description' => t('%realm to be assigned the case if one is not explicitly defined.', array('%realm' => ucfirst($realm))),
    );
  };

  $node_types = node_get_types('names');

  $project_types = $node_types; unset($project_types['casetracker_basic_case']);
  $form['casetracker_general']['casetracker_project_node_types'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Project node types'),
    '#options' => $project_types,
    '#default_value' => variable_get('casetracker_project_node_types', array('casetracker_basic_project')),
    '#description' => t('Select the node types that will be considered Case Tracker projects.'),
  );

  $case_types = $node_types; unset($case_types['casetracker_basic_project']);
  $form['casetracker_general']['casetracker_case_node_types'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Case node types'),
    '#options' => $case_types,
    '#default_value' => variable_get('casetracker_case_node_types', array('casetracker_basic_case')),
    '#description' => t('Select the node types that will be considered Case Tracker cases.'),
  );
  
  $all_views = views_get_all_views();
  $node_views = array();
  $user_views = array();
  foreach($all_views AS $view) {
    if($view->base_table == 'node') {
      $node_views[$view->name] = $view->name;
    } else if($view->base_table = 'users') {
      $user_views[$view->name] = $view->name;
    }
  }
  $form['casetracker_general']['casetracker_view_project_options'] = array(
    '#type' => 'select',
    '#title' => t('Project options view'),
    '#options' => $node_views,
    '#default_value' => variable_get('casetracker_view_project_options', 'casetracker_project_options'),
    '#description' => t('Select the view from which to fill the project selector on the add case page.'),
  );
  
  $form['casetracker_general']['casetracker_view_assignee_options'] = array(
    '#type' => 'select',
    '#title' => t('Assignee options view'),
    '#options' => $user_views,
    '#default_value' => variable_get('casetracker_view_assignee_options', 'casetracker_assignee_options'),
    '#description' => t('Select the view from which to fill the user assignment options on the add case page.'),
  );
  
  $form['#submit'][] = 'casetracker_settings_submit';
  return system_settings_form($form);
}

/**
 * Element validate for the default assign to value.
 */
function casetracker_default_assign_to_validate($element, &$form_state) {
  // Switch Unassigned to Anonymous so things are saved properly.
  if ($form_state['values']['casetracker_default_assign_to'] == t('Unassigned')) {
    $form_state['values']['casetracker_default_assign_to'] = variable_get('anonymous', t('Anonymous'));
  }
  else {
    if (!casetracker_get_uid($element['#value'])) {
      form_error($element, t('"Default assigned user" must be "Unassigned" or an existing user.'));
    }
  }
}

/**
 * Submit handler for casetracker_settings.
 *
 * Clear the views so project drop downs will be properly populated after
 * settings changes.
 */
function casetracker_settings_submit($form, &$form_state) {
  views_invalidate_cache();
}

/**
 * Displays a form for adding or editing a case state.
 */
function casetracker_case_state_edit($form_state, $case_state = NULL) {

  $form = array();
  $form['case_state'] = array(
    '#type' => 'fieldset',
    '#title' => t('Case state'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['case_state']['name'] = array(
    '#type' => 'textfield',
    '#title' => t('State name'),
    '#required' => TRUE,
    '#default_value' => isset($case_state) ? $case_state->name : NULL,
    '#description' => t('The name for this case state. Example: "Resolved".'),
  );
  $form['case_state']['realm'] = array(
    '#type' => 'select',
    '#title' => t('State realm'),
    '#required'  => TRUE,
    '#default_value' => isset($case_state) ? $case_state->realm : NULL,
    '#description' => t('The realm in which this case state will appear.'),
    '#options' => array('priority' => t('priority'), 'status' => t('status'), 'type' => t('type')),
  );
  $form['case_state']['weight'] = array(
    '#type' => 'weight',
    '#title' => t('Weight'),
    '#default_value' => isset($case_state) ? $case_state->weight : 0,
    '#description' => t('States are ordered first by weight and then by state name.'),
  );

  if ($case_state) { 
    $form['csid'] = array(
      '#type' => 'value', 
      '#value' => $case_state->csid,
    ); 
  }
  $form['submit'] = array(
    '#type' => 'submit', 
    '#value' => t('Submit'), // this text is an easter egg.
  ); 

  return $form;
}

/**
 * Displays an administrative overview of all case states available.
 */
function casetracker_case_state_overview() {
  $rows = array(); 
  $headers = array(
    t('Name'), 
    t('Realm'), 
    array(
      'data' => t('Operations'), 
      'colspan' => 2)
    );
  foreach (array('priority', 'status', 'type') as $realm) {
    foreach (casetracker_realm_load($realm, FALSE) as $csid => $name) {
      $rows[] = array(
        check_plain($name),
        $realm,
        l(t('edit'), 'admin/settings/casetracker/states/edit/'. $csid),
        l(t('delete'), 'admin/settings/casetracker/states/delete/'. $csid), );
    }
  }

  return theme('table', $headers, $rows);
}

/**
 * Processes the submitted results of our case state addition or editing.
 */
function casetracker_case_state_edit_submit($form, &$form_state) {
  $case_state = array(
    'name' => $form_state['values']['name'], 
    'realm' => $form_state['values']['realm'],
    'weight' => $form_state['values']['weight'],
  );

  if (!empty($form_state['values']['csid'])) {
    // Update
    $case_state['csid'] = $form_state['values']['csid'];
    drupal_set_message(t('The case state %name has been updated.', array('%name' =>  $form_state['values']['name'])));
  }
  else {
    // Create
    drupal_set_message(t('The case state %name has been created.', array('%name' =>  $form_state['values']['name'])));
  }
  casetracker_case_state_save($case_state);
  $form_state['redirect'] = 'admin/settings/casetracker/states';
}

/**
 * If the user has asked to delete a case state, we'll double-check.
 */
function casetracker_case_state_confirm_delete(&$form_state, $case_state) {
  if (!empty($case_state)) {
    $form = array(
      'csid' => array('#type' => 'value', '#value' => $case_state->csid),
      'name' => array('#type' => 'value', '#value' => $case_state->name),
    );
    return confirm_form($form,
                        t('Are you sure you want to delete the case state %name?', array('%name' => $case_state->name)),
                        'admin/settings/casetracker/states', t('This action can not be undone.'), t('Delete'), t('Cancel'));
  }
}

/**
 * Ayup, the user definitely wants to delete this case state.
 */
function casetracker_case_state_confirm_delete_submit($form, &$form_state) {
  drupal_set_message(t('Deleted case state %name.', array('%name' =>  $form_state['values']['name'])));
  casetracker_case_state_delete($form_state['values']['csid']);
  $form_state['redirect'] = 'admin/settings/casetracker/states';
}
