<?php
// $Id: urlfill.admin.inc,v 1.4.2.1 2010/10/07 19:38:05 phayes Exp $ 

/**
 * @file
 * Admin / config
 * 
 * This file holds all the helper functions for administering urlfill.
 * It contains functions for altering the node admin form, the field admin forms etc.
 * It also contains functions for saving data to the database etc.
 * 
 */


/**
 * Alter the node admin form
 *
 * Alters the node admin form found at admin/content/node-type/mynodetype
 * and adds extra URL-fill options
 *
 * @param $form
 *   Node admin form to be altered
 * @return
 *   None. Form is passed by reference and therefore is altered in situ.
 */
function urlfill_node_admin (&$form) {
  
  // Load urlfill field data.
  $title_data = urlfill_load_data('node', 'title', $form['#node_type']->type);
  $body_data = urlfill_load_data('node', 'body', $form['#node_type']->type);
  
  // Store some data about this
  $form['#urlfill_type'] = 'node';
  
  // Create config form
  
  $form['urlfill'] = array(
    '#type' => 'fieldset', 
    '#title' => t('URL Fill'), 
    '#collapsible' => TRUE, 
    '#collapsed' => TRUE,
  );
  
  urlfill_config_node_form($form, $title_data, 'title');
  urlfill_config_node_form($form, $body_data, 'body');

  // Add extra submission function
  $form['#submit'][] = 'urlfill_data_submit';
  
  // Add hide / show javascript;
  drupal_add_js(drupal_get_path('module', 'urlfill').'/urlfill.js');

  // Adding javascript settings
  $urlfill_settings = array (
    'urlfill_fields' => array ('title','body'),
  ); 
  drupal_add_js($urlfill_settings,'setting');
  
}

/**
 * Alter the field admin form
 *
 * Alters the field admin form found at admin/content/node-type/mynodetype/fields/field_myfield
 * and adds extra Auto-fill from URL options
 *
 * @param $form
 *   Node admin form to be altered
 * @return
 *   None. Form is passed by reference and therefore is altered in situ.
 */
 function urlfill_content_admin_field(&$form) {
  
  $type = array();
  $type = content_types($form['type_name']['#value']);
  
  // Load urlfill field data.
  $urlfill_data = urlfill_load_data('field', $form['#field']['field_name']);
  
  // Create config form
  module_load_include('inc','urlfill','urlfill.form');
  urlfill_config_field_form($form, $urlfill_data);

  // Add extra submission function
  $form['#submit'][] = 'urlfill_data_submit';
  
  // Add hide / show javascript;
  drupal_add_js(drupal_get_path('module', 'urlfill').'/urlfill.js');

  // Adding javascript settings
  $urlfill_settings = array (
    'urlfill_fields' => array ($form['#field']['field_name']),
  ); 
  drupal_add_js($urlfill_settings,'setting');
}

/**
 * urlfill Data Submit
 *
 * Submit urlfill data from the field editing forms. Basically just takes
 * the form submitted, parses out urlfill data and updates the database.
 * 
 * @param $form
 *   Node admin form
 * @return
 *   None. Data is entered in the database.
 */
function urlfill_data_submit($form, &$form_state) {
  $type = $form['#urlfill_type'];
  
  // Build out the fields/data we wish to submit / delete in the database
  $fields = array();
  $delete_fields = array();
  
  //Build out data for title / body entry
  if ($type == 'node'){
    foreach (array('title','body') as $field_name) {
      if ($form['urlfill'][$field_name]['urlfill_onoff_'.$field_name]['#value']) {
        // Do this twice, one for title, once for body
        $fields[$field_name] = array(
          'type' => 'node',
          'field_name' => $field_name,
          'multiple' => '0',
          'node_type' => $form['#node_type']->type,
          'variable' => $form['urlfill'][$field_name]['urlfill_variable_'.$field_name]['#value'],
          'override' => $form['urlfill'][$field_name]['urlfill_override_'.$field_name]['#value'],
          'editable' => $form['urlfill'][$field_name]['urlfill_editable_'.$field_name]['#value'], 
          'nourl' => $form['urlfill'][$field_name]['urlfill_nourl_'.$field_name]['#value'],
        );
      }
      else {
        // Delete record from database
        $delete_fields[$field_name] = array(
          'type' => 'node',
          'field_name' => $field_name,
          'node_type' => $form['#node_type']->type,
        );
      }
    }
  }
  
  //Build out data for CCK field entry
  if ($type == 'field'){
    $field_name = $form['#field']['field_name'];
    if ($form['field']['urlfill']['urlfill_onoff_'.$field_name]['#value']) {
      $fields[$field_name] = array(
        'type' => 'field',
        'field_name' => $field_name,
        'multiple' => $form['field']['multiple']['#value'] != '0' ? '1' : '0',
        'node_type' => '',
        'variable' => $form['field']['urlfill']['urlfill_variable_'.$field_name]['#value'],
        'override' => $form['field']['urlfill']['urlfill_override_'.$field_name]['#value'],
        'editable' => $form['field']['urlfill']['urlfill_editable_'.$field_name]['#value'],
        'nourl' => $form['field']['urlfill']['urlfill_nourl_'.$field_name]['#value'],
      );
    }
    else {
      // Delete record from database
      $delete_fields[$field_name] = array(
        'type' => 'field',
        'field_name' => $field_name,
        'node_type' => '',
      );
    }
  }
  
  // Done building out data. Time to do actual queries. 
  // Go through fields and add / modify them in the database using the data we built above.
  foreach ($fields as $field) {
    
    // Add or alter record for urlfill
    $countrows = db_result(db_query("SELECT count(*) FROM {urlfill} WHERE field_name = '%s' AND type ='%s' AND node_type = '%s'",$field['field_name'],$field['type'],$field['node_type']));
    if ($countrows){
      // Update record.
      db_query(
        "UPDATE {urlfill} SET variable = '%s', multiple =  %d, override = '%s', editable = '%s', nourl = '%s' WHERE field_name = '%s' AND type = '%s' AND node_type = '%s'",
        $field['variable'], $field['multiple'], $field['override'], $field['editable'], $field['nourl'], $field['field_name'], $field['type'], $field['node_type']
      );
    }
    else {
      // Add record.
      db_query(
        "INSERT INTO {urlfill} (type, field_name, node_type, variable, multiple, override, editable, nourl) VALUES ('%s','%s','%s','%s',%d,%d,'%s','%s')",
        $field['type'], $field['field_name'], $field['node_type'], $field['variable'], $field['multiple'], $field['override'], $field['editable'], $field['nourl']
      );
    }
  }
  
  // Go through fields and delete ones marked for deletion.
  foreach ($delete_fields as $field) {
    // Remove urlfill for this field
    db_query("DELETE FROM {urlfill} WHERE field_name = '%s' AND type = '%s' AND node_type = '%s' ",$field['field_name'], $field['type'], $field['node_type']);
  }
}

/**
 * Find urlfill fields and mark them.
 * 
 * Adds a little helpful example of how to fill in this field
 * 
 * @param $form
 *   Field overiew form.
 * @return
 *   None. Form is passed by reference and modified in situ.
 */
function urlfill_field_overview_form(&$form) {
  // Check if we have conditional data
  if (!$data = urlfill_load_data('field')) {
    return;
  }
  foreach ($data as $field) {
    if (in_array($field->field_name, $form['#fields'])) {
      $form[$field->field_name]['field_name']['#value'] .= theme('urlfill_manage_marker', $field);
    }
  }
}

/**
 * Delete urlfill data
 * 
 * Removes urlfill data when a field is deleted
 * 
 * @param $form
 * @param $form_state
 * @return
 *   None. Changes are made to the database.
 */
function urlfill_content_admin_field_remove_submit ($form, $form_state) {
  // Clean the urlfill database table of this field.
  db_query("DELETE FROM {urlfill} WHERE field_name = '%s' AND type = 'field'", $form_state['values']['field_name']);
}

/**
 * Config Node Form
 * 
 * Alters the form for the node admin page, adding form elements
 * for urlfill config for this node type
 * 
 * @param $form
 *  The node admin form we wish add items to
 * @param $urlfill_data
 *  The urlfill data for this particular field. Will be FALSE if there is
 *  no data.
 * @param $field_name
 *  The field name we are adding these admin items for. Will either be
 *  'title' or 'body'
 * 
 * @return
 *   None. Form is passed by reference and modified in situ.
 */
function urlfill_config_node_form (&$form, $urlfill_data, $field_name){
  
  $form['urlfill'][$field_name]['urlfill_onoff_'.$field_name] = array(
    '#type' => 'checkbox', 
    '#title' => t("Enable auto-fill from URL for $field_name"),
    '#default_value' => $urlfill_data ? TRUE : FALSE,
    '#attributes'  => array('onclick' => "urlfillHideUnhide('$field_name');"),
  );
  $form['urlfill'][$field_name]['start_div'] = array(
    '#value' => "<div id='urlfill_form_div_$field_name'>",
  );
  $form['urlfill'][$field_name]['urlfill_variable_'.$field_name] = array(
    '#type' => 'textfield', 
    '#title' => t('URL variable'),
    '#description' => t('The URL variable that we will take the value from for auto-filling this field.'),
    '#default_value' => $urlfill_data->variable ? $urlfill_data->variable : $field_name,
  );
  $form['urlfill'][$field_name]['urlfill_override_'.$field_name] = array(
    '#type' => 'checkbox', 
    '#title' => t('Override pre-existing values'),
    '#default_value' => $urlfill_data ? $urlfill_data->override : TRUE,
  );
  $form['urlfill'][$field_name]['urlfill_editable_'.$field_name] = array(
    '#type' => 'radios', 
    '#title' => t('When a field is auto-filled from the URL'), 
    '#default_value' => $urlfill_data ? $urlfill_data->editable : 'edit', 
    '#options' => array(
      'edit' => t('Let field be edited'), 
      #'lock' => t('Lock field  -- <b>Does not yet work</b>'), //@@TODO: Implement this
      'hide' => t('Hide field'),
    ),
  );
  $form['urlfill'][$field_name]['urlfill_nourl_'.$field_name] = array (
    '#type' => 'radios', 
    '#title' => t(' When URL value is not present'), 
    '#default_value' => $urlfill_data ? $urlfill_data->nourl : 'nothing', 
    '#options' => array(
      'nothing' => t('Do nothing. Let field be edited as is'),
      'notfound' => t('Display \'page not found\' error'), 
    ),
  );
  $form['urlfill'][$field_name]['end_div'] = array(
    '#value' => "</div>",
  ); 
  
}

/**
 * Config Field Form
 * 
 * Alters the form for the field admin page, adding form elements
 * for urlfill config for this field.
 * 
 * @param $form
 *  The field admin form we wish add items to
 * @param $urlfill_data
 *  The urlfill data for this particular field. Will be FALSE if there is
 *  no data.
 * @return
 *   None. Form is passed by reference and modified in situ.
 */
function urlfill_config_field_form(&$form, $urlfill_data){
  
  if (urlfill_get_path($form['#field']['widget']['type'])) {
     $form['#urlfill_type'] = 'field';
     $field_name = $form['#field']['field_name'];
     
     $form['field']['urlfill'] = array(
      '#type' => 'fieldset', 
      '#title' => t('URL Fill'), 
      '#collapsible' => TRUE, 
      '#collapsed' => FALSE,
    );
    $form['field']['urlfill']['urlfill_onoff_'.$field_name] = array(
      '#type' => 'checkbox', 
      '#title' => t('Enable auto-fill from URL for this field'),
      '#default_value' => $urlfill_data ? TRUE : FALSE,
      '#attributes'  => array('onclick' => "urlfillHideUnhide('$field_name');"),
    );
    $form['field']['urlfill']['start_div'] = array(
      '#value' => "<div id='urlfill_form_div_$field_name'>",
    );
    $form['field']['urlfill']['urlfill_variable_'.$field_name] = array(
      '#type' => 'textfield', 
      '#title' => t('URL variable'),
      '#description' => t('The URL variable that we will take the value from for auto-filling this field.'),
      '#default_value' => $urlfill_data->variable ? $urlfill_data->variable : $form['#field']['field_name'],
    );
    $form['field']['urlfill']['urlfill_override_'.$field_name] = array(
      '#type' => 'checkbox', 
      '#title' => t('Override pre-existing values'),
      '#default_value' => $urlfill_data ? $urlfill_data->override : TRUE,
    );
    $form['field']['urlfill']['urlfill_editable_'.$field_name] = array(
      '#type' => 'radios', 
      '#title' => t('When a field is auto-filled from the URL'), 
      '#default_value' => $urlfill_data ? $urlfill_data->editable : 'edit', 
      '#options' => array(
        'edit' => t('Let field be edited'), 
        #'lock' => t('Lock field  -- <b>Does not yet work</b>'), //@@TODO: Implement this
        'hide' => t('Hide field'),
      ),
    );
    $form['field']['urlfill']['urlfill_nourl_'.$field_name] = array (
      '#type' => 'radios', 
      '#title' => t(' When URL value is not present'), 
      '#default_value' => $urlfill_data ? $urlfill_data->nourl : 'nothing', 
      '#options' => array(
        'nothing' => t('Do nothing. Let field be edited as is'),
        'notfound' => t('Display \'page not found\' error'), 
      ),
    );
    $form['field']['urlfill']['end_div'] = array(
      '#value' => "</div>",
    ); 
  }
  
}

?>