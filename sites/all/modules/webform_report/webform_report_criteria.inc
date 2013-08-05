<?php

// $Id:

/**
 * @file
 * This file contains the functions required to implement the
 * Report Criteria form for the webform report module
 */
 
/**
 * Generate a form for specifying report criteria.
 *
 * @param form_state
 *   drupal form state
 * *param node
 *   current node object
 * @return 
 *   an array of form elements
 */
function webform_report_criteria_form($form_state, $node) {
  
  global $user;

  module_load_include('inc', 'webform_report', 'webform_report');

  $mod_path = drupal_get_path('module','webform_report');
  $img_path = $mod_path . "/images/";

  if (isset($node->wnid)) {  
  
    $webform_components = _webform_report_get_components($node->wnid);
    
    $form['nid'] = array(
      '#type' => 'hidden',
      '#value' => $node->nid
    );
  
    $comp_options = array();
    $comp_options[0] = t('Select a field');
    foreach($webform_components as $cid => $comp) {
      $comp_options[$cid] = $comp['name'];
    }
  
    $component_date_format_options = array(
      '' => '', 
      'm/d/Y' => 'mm/dd/yyyy', 
      'd/m/Y' => 'dd/mm/yyyy', 
      'Y/m/d' => 'yyyy/mm/dd'
    );
    $component_time_format_options = array(
      '' => '', 
      'H:i' => 'hh:mm', 
      'h:i a' => 'hh:mm am/pm'
    );
    $component_select_format_options = array(
      '' => '', 
      'KEY' => 'Display Key', 
      'VALUE' => 'Display Value'
    );
    $component_select_options = array(
      '' => '', 
      'KEY' => 'Sort By Key', 
      'VALUE' => 'Sort By Value'
    );
  
    $columns = _webform_report_get_columns($node, $webform_components);
  
    // column fieldset  
    $form['columns'] = array(
      '#type' => 'fieldset', 
      '#title' => t('Report Columns'),
      '#description' => t('Columns are listed in same order as this list.'),
      '#tree' => TRUE
    );
  
    // form list of report columns
    $cc = count($columns);
    $i = 0;
    
    for ($i=0; $i<$cc; $i++) {
    
      $cid = $columns[$i]['cid'];
  
      $form['columns'][$i]['name'] = array(
        '#type' => 'markup',
        '#value' => $columns[$i]['name']
      );
      
      $form['columns'][$i]['type'] = array(
        '#type' => 'markup',
        '#value' => $columns[$i]['type']
      );
  
      if ($columns[$i]['type'] == 'date') {
        $form['columns'][$i]['format'] = array(
          '#type' => 'select',
          '#default_value' => $columns[$i]['format'],
          '#options' => $component_date_format_options
        );
      }
      if ($columns[$i]['type'] == 'time') {
        $form['columns'][$i]['format'] = array(
          '#type' => 'select',
          '#default_value' => $columns[$i]['format'],
          '#options' => $component_time_format_options
        );
      }
      if ($columns[$i]['type'] == 'select') {
        $form['columns'][$i]['format'] = array(
          '#type' => 'select',
          '#default_value' => $columns[$i]['format'],
          '#options' => $component_select_format_options
        );
        $form['columns'][$i]['option'] = array(
          '#type' => 'select',
          '#default_value' => $columns[$i]['option'],
          '#options' => $component_select_options
        );
      }
  
      $form['columns'][$i]['hidden'] = array(
        '#type' => 'checkbox',
        '#default_value' => $columns[$i]['hidden']
      );
  
      webform_report_criteria_form_list_buttons($form, $img_path, 'columns', $cid, $i, $cc);
    }

    $form['columns']['addcomp'] = array(
      '#type' => 'select',
      '#options' => $comp_options,
      '#required' => FALSE
    );
  
    $form['columns']['add'] = array(
      '#type' => 'submit',
      '#value' => t('Add Column'),
      '#return_value' => 'columns_add',
      '#submit' => array('webform_report_criteria_form_list_submit')
    );

    // make sure we get report values  
    unset($_SESSION['search_form'][$node->nid]);
    
    $filters = _webform_report_get_filters($node, $webform_components);

    $filter_options = _webform_report_get_filter_options();
    
    $filter_options_datetime = _webform_report_get_filter_options(TRUE);
    
    // filters fieldset  
    $form['filters'] = array(
      '#type' => 'fieldset', 
      '#title' => t('Filters'),
      '#description' => t('All filters are ANDed together.'),
      '#tree' => TRUE
    );
  
    // form list of report filters
    $cc = count($filters);
    $i = 0;

    for ($i=0; $i<$cc; $i++) {
  
      $cid = $filters[$i]['cid'];
      $type = $filters[$i]['type'];
      $ftype = $filters[$i]['ftype'];
      $value = $filters[$i]['value'];
  
      $form['filters'][$i]['name'] = array(
        '#type' => 'markup',
        '#value' => $filters[$i]['name']
      );
      $form['filters'][$i]['type'] = array(
        '#type' => 'markup',
        '#value' => $type
      );
  
      $form['filters'][$i]['ftype'] = array(
        '#type' => 'select',
        '#default_value' => $ftype
      );

      // use reduced options for date/time
      if ($type == 'date' || $type == 'time') {
        $form['filters'][$i]['ftype']['#options'] = $filter_options_datetime;
      }
      else {
        $form['filters'][$i]['ftype']['#options'] = $filter_options;
      }

      // insert value field(s)      
      webform_report_criteria_filter_value_field($form, $i, $type, $ftype, $value);
      
      webform_report_criteria_form_list_buttons($form, $img_path, 'filters', $cid, $i, $cc);
    }
    
    $form['filters']['addcomp'] = array(
      '#type' => 'select',
      '#options' => $comp_options,
      '#required' => FALSE
    );
  
    $form['filters']['add'] = array(
      '#type' => 'submit',
      '#value' => t('Add Filter'),
      '#return_value' => 'filters_add',
      '#submit' => array('webform_report_criteria_form_list_submit')
    );
  
    $sorton = _webform_report_get_sorton($node, $webform_components);
  
    $sort_options = array(
      SORT_ASC => t('Ascending'), 
      SORT_DESC => t('Descending')
    );
    
    // sort fieldset  
    $form['sorton'] = array(
      '#type' => 'fieldset', 
      '#title' => t('Sort By'),
      '#description' => t('Sorting is applied in same order as this list.'),
      '#tree' => TRUE
    );
  
    // form list of report sort options
    $cc = count($sorton);
    $i = 0;
    
    for ($i=0; $i<$cc; $i++) {
  
      $cid = $sorton[$i]['cid'];
  
      $form['sorton'][$i]['name'] = array(
        '#type' => 'markup',
        '#value' => $sorton[$i]['name']
      );
      $form['sorton'][$i]['type'] = array(
        '#type' => 'markup',
        '#value' => $sorton[$i]['type']
      );
  
      $form['sorton'][$i]['order'] = array(
        '#type' => 'select',
        '#options' => $sort_options,
        '#default_value' => $sorton[$i]['order']
      );
  
      webform_report_criteria_form_list_buttons($form, $img_path, 'sorton', $cid, $i, $cc);
    }
    
    // only allow sorts on report columns
    $options = array();
    $options[0] = t('Select a field');
    foreach ($columns as $index => $col) {
      $options[$col['cid']] = $col['name'];
    }
    
    $form['sorton']['addcomp'] = array(
      '#type' => 'select',
      '#options' => $options,
      '#required' => FALSE
    );
  
    $form['sorton']['add'] = array(
      '#type' => 'submit',
      '#value' => t('Add Sort'),
      '#return_value' => 'sorton_add',
      '#submit' => array('webform_report_criteria_form_list_submit')
    );
  
    // options fieldset  
    $form['options'] = array(
      '#type' => 'fieldset', 
      '#title' => t('Options'),
      '#description' => t('Report options.')
    );
  
    $results_per_page_options = array(
      0 => 'All', 
      20 => '20', 
      40 => '40', 
      60 => '60', 
      80 => '80', 
      100 => '100',
      200 => '200',
      300 => '300',
      400 => '400',
      500 => '500'
    );
    $default = 20;
    if (array_key_exists('results_per_page', $node->options)) {
      $default = $node->options['results_per_page'];
    }
    $form['options']['results_per_page'] = array(
      '#type' => 'select',
      '#title' => t('Results per page'),
      '#options' => $results_per_page_options,
      '#default_value' => $default,
    );

    $default = FALSE;  
    if (array_key_exists('hide_csv', $node->options)) {
      $default = $node->options['hide_csv'];
    }
    $form['options']['hide_csv'] = array(
      '#type' => 'checkbox',
      '#title' => t('Hide CSV Download option'),
      '#default_value' => $default,
    );
    
    $default = FALSE;  
    if (array_key_exists('search_form', $node->options)) {
      $default = $node->options['search_form'];
    }
    $form['options']['search_form'] = array(
      '#type' => 'checkbox',
      '#title' => t('Display Search Form'),
      '#default_value' => $default,
    );
    
    $default = FALSE;  
    if (array_key_exists('filter_form', $node->options)) {
      $default = $node->options['filter_form'];
    }
    $form['options']['filter_form'] = array(
      '#type' => 'checkbox',
      '#title' => t('Display Report Filter Form'),
      '#default_value' => $default,
    );

    // PHP code fieldset - only shown if user has permission
    if (user_access('use PHP code in webform reports')) {
    
      $code = isset($node->options['php_code']) ? $node->options['php_code'] : '';
      
      $form['php'] = array(
        '#type' => 'fieldset', 
        '#title' => t('PHP Code'),
        '#collapsible' => TRUE,
        '#collapsed' => empty($code),
        '#description' => t('PHP processing code can be used to modify report values before the report is displayed.  <strong>This option should be used with <u>caution</u> as improper code can crash or damage a site.</strong>')
      );

      $form['php']['variables'] = array(
        '#type' => 'markup',
        '#value' => '<p>' . t('The following variables are available to thie code:') . '</p>' . theme('item_list', array(
          '$node: ' . t('The webform report node object.'),
          '$headers: ' . t('The report header array as input to <a href="http://api.drupal.org/api/drupal/includes--theme.inc/function/theme_table/6">theme_table</a>.'),
          '$rows: ' . t('The report row array as input to <a href="http://api.drupal.org/api/drupal/includes--theme.inc/function/theme_table/6">theme_table</a> with additional keys <i>field</i> (field id), <i>sort</i> (value used to sort the column), and <i>hidden</i> (indicates if value is to be displayed on report).'),
          )) . '<p>The <i>$headers</i> and <i>$rows</i> may be modifed in order to alter the report output.'
      );
      
      $form['php']['php_code'] = array(
        '#type' => 'textarea',
        '#title' => t('PHP Processing Code'),
        '#default_value' => $code,
        '#cols' => 40,
        '#rows' => 10,
        '#description' => t('The PHP code must start with &lt;?php in order to be executed.')
      );
    }
    
    $form['update'] = array(
      '#type' => 'submit',
      '#value' => t('Update'),
    );
  
    return $form;
  }
  else {
    drupal_set_message(t('Error - Webform is not set for this report - cannot proceed.  Please delete and re-create this report.'), 'error');
    return NULL;
  }
}

/**
 * Insert filter value field(s)
 *
 * @param form
 *   the form to add the fields to
 * @param index
 *   the filter index
 * @param type
 *   the filter column type
 * @param ftype
 *   the filter type
 * @param value
 *   the filter value
 */
function webform_report_criteria_filter_value_field(&$form, $index, $type, $ftype, $value) {
  
  // use a date popup for dates if date_popup module is enabled
  $fieldtype = 'textfield';
  if ($type == 'date') {
    if (module_exists('date_popup')) {
      $fieldtype = 'date_popup';
    }
  }
  
  // if filter type is 'between', extract the two values
  if ($ftype == 13) {
    $value = explode('|', $value);
    if (count($value) == 1) {
      $value[] = '';
    }
  }
  
  // if between, add two fields
  if (is_array($value)) {
    $form['filters'][$index]['value1'] = array(
      '#type' => $fieldtype,
      '#default_value' => $value[0]
    );
    if ($fieldtype == 'date_popup') {
      $form['filters'][$index]['value1']['#date_format'] = 'm/d/Y';
    }
    // add a size if a textbox
    else {
      $form['filters'][$index]['value1']['#size'] = 15;
    }
    $form['filters'][$index]['value2'] = array(
      '#type' => $fieldtype,
      '#default_value' => $value[1]
    );
          
    // if using popup, format the value correctly
    if ($fieldtype == 'date_popup') {
      $form['filters'][$index]['value2']['#date_format'] = 'm/d/Y';
    }
    // add a size if a textbox
    else {
      $form['filters'][$index]['value2']['#size'] = 15;
    }
  }
  // otherwise, just add one field
  else {
    $form['filters'][$index]['value'] = array(
      '#type' => $fieldtype,
      '#default_value' => $value
    );
        
    // if using popup, format the value correctly
    if ($fieldtype == 'date_popup') {
      $form['filters'][$index]['value']['#date_format'] = 'm/d/Y';
    }
    // add a size if a textbox
    else {
      $form['filters'][$index]['value']['#size'] = 40;
    }
  }   // end - if (is_array($value))...
        
}        

/**
 * Add list buttons to a form
 *
 * @param form
 *   the form to add the components
 * *param img_path
 *   path to button image files
 * *param list
 *   the list name
 * *param cid
 *   the current item cid
 * *param index
 *   the current item index
 * *param count
 *   the number of items in the list
 */
function webform_report_criteria_form_list_buttons(&$form, $img_path, $list, $cid, $index, $count) {

  $form[$list][$index]['delete'] = array(
    '#type' => 'image_button',
    '#src' => $img_path . 'trash.png',
    '#return_value' => $list . '_delete_' . $index . '_' . $cid,
    '#attributes' => array('alt' => t('Delete this item')),
    '#submit' => array('webform_report_criteria_form_list_submit')
  );
  
  if ($index > 0) {
    $form[$list][$index]['top'] = array(
      '#type' => 'image_button',
      '#src' => $img_path . 'toparrow.png',
      '#return_value' => $list . '_top_' . $index . '_' . $cid,
      '#attributes' => array('alt' => t('Move item to the top')),
      '#submit' => array('webform_report_criteria_form_list_submit')
    );
    $form[$list][$index]['up'] = array(
      '#type' => 'image_button',
      '#src' => $img_path . 'uparrow.png',
      '#return_value' => $list . '_up_' . $index . '_' . $cid,
      '#attributes' => array('alt' => t('Move item up')),
      '#submit' => array('webform_report_criteria_form_list_submit')
    );
  }
  else {
    $form[$list][$index]['top'] = array(
      '#type' => 'markup'
    );
    $form[$list][$index]['up'] = array(
      '#type' => 'markup'
    );
  }
  if ($index < $count-1) {
    $form[$list][$index]['down'] = array(
      '#type' => 'image_button',
      '#src' => $img_path . 'downarrow.png',
      '#return_value' => $list . '_down_' . $index . '_' . $cid,
      '#attributes' => array('alt' => t('Move item down')),
      '#submit' => array('webform_report_criteria_form_list_submit')
    );
    $form[$list][$index]['end'] = array(
      '#type' => 'image_button',
      '#src' => $img_path . 'endarrow.png',
      '#return_value' => $list . '_end_' . $index . '_' . $cid,
      '#attributes' => array('alt' => t('Move item to the end')),
      '#submit' => array('webform_report_criteria_form_list_submit')
    );
  }
  else {
    $form[$list][$index]['down'] = array(
      '#type' => 'markup'
    );
    $form[$list][$index]['end'] = array(
      '#type' => 'markup'
    );
  }
}

/**
 * Theme the report criteria form
 *
 * @param form
 *   the form to theme
 * @return
 *   the rendered form
 */
function theme_webform_report_criteria_form($form) {

  if (array_key_exists('columns', $form)) {
  
    // theme column list
    $header = array(
      t('Name'), 
      t('Type'), 
      t('Format'), 
      t('Option'), 
      t('Hidden'),
      array('data' => t('Operations'), 'colspan' => '5')
    );
    
    $rows = array();
    
    foreach (element_children($form['columns']) as $index) {
  
      $rows[] = array(
        drupal_render($form['columns'][$index]['name']),
        drupal_render($form['columns'][$index]['type']),
        drupal_render($form['columns'][$index]['format']),
        drupal_render($form['columns'][$index]['option']),
        drupal_render($form['columns'][$index]['hidden']),
        drupal_render($form['columns'][$index]['delete']),    
        drupal_render($form['columns'][$index]['top']), 
        drupal_render($form['columns'][$index]['up']),
        drupal_render($form['columns'][$index]['down']),
        drupal_render($form['columns'][$index]['end'])    
      );
    }
  
    $form['columns']['table'] = array(
      '#value' => theme('table', $header, $rows)
    );
  
    $header = array(
      array('data' => t('Add Column'), 'colspan' => '2')
    );
  
    $rows = array();
    $rows[] = array(
      drupal_render($form['columns']['addcomp']), 
      drupal_render($form['columns']['add'])
    );
    
    $form['columns']['addtable'] = array(
      '#value' => theme('table', $header, $rows)
    );
  }
  
  if (array_key_exists('filters', $form)) {
  
    // see if an extra value column is present
    $hasextra = FALSE;
    foreach (element_children($form['filters']) as $index) {
      if (array_key_exists('value1', $form['filters'][$index])) {
        $hasextra = TRUE;
        break;
      }
    }
    // theme filter list
    $rows = array();
    foreach (element_children($form['filters']) as $index) {
  
      $row = array();
      $row[] = drupal_render($form['filters'][$index]['name']);
      $row[] = drupal_render($form['filters'][$index]['type']);
      $row[] = drupal_render($form['filters'][$index]['ftype']);
      if (array_key_exists('value1', $form['filters'][$index])) {
         $row[] = drupal_render($form['filters'][$index]['value1']);
         $row[] = drupal_render($form['filters'][$index]['value2']);
      }
      else {
        if ($hasextra) {
          $row[] = array('data' => drupal_render($form['filters'][$index]['value']), 'colspan' => '2');
        }
        else {
          $row[] = drupal_render($form['filters'][$index]['value']);
        }
      }
      $row[] = drupal_render($form['filters'][$index]['delete']);
      $row[] = drupal_render($form['filters'][$index]['top']);
      $row[] = drupal_render($form['filters'][$index]['up']);
      $row[] = drupal_render($form['filters'][$index]['down']);
      $row[] = drupal_render($form['filters'][$index]['end']);  
      
      $rows[] = $row;
    }

    $header = array();
    $header[] = t('Name');
    $header[] = t('Type');
    $header[] = t('Filter Type');
    if ($hasextra) {  
      $header[] = array('data' => t('Filter Value(s)'), 'colspan' => '2');
    }
    else {
      $header[] = t('Filter Value');
    }
    $header[] = array('data' => t('Operations'), 'colspan' => '5');
    
    $form['filters']['table'] = array(
      '#value' => theme('table', $header, $rows)
    );
  
    $header = array(
      array('data' => t('Add Filter'), 'colspan' => '2')
    );
  
    $rows = array();
    $rows[] = array(
      drupal_render($form['filters']['addcomp']),
      drupal_render($form['filters']['add'])
    );
    
    $form['filters']['addtable'] = array(
      '#value' => theme('table', $header, $rows)
    );
  }
   
  if (array_key_exists('sorton', $form)) {
  
    // theme sort list
    $header = array(
      t('Name'), 
      t('Type'), 
      t('Order'), 
      array('data' => t('Operations'), 'colspan' => '5')
    );
    
    $rows = array();
    
    foreach (element_children($form['sorton']) as $index) {
  
      $rows[] = array(
        drupal_render($form['sorton'][$index]['name']),
        drupal_render($form['sorton'][$index]['type']),
        drupal_render($form['sorton'][$index]['order']),
        drupal_render($form['sorton'][$index]['delete']),    
        drupal_render($form['sorton'][$index]['top']), 
        drupal_render($form['sorton'][$index]['up']),
        drupal_render($form['sorton'][$index]['down']),
        drupal_render($form['sorton'][$index]['end'])    
      );
    }
  
    $form['sorton']['table'] = array(
      '#value' => theme('table', $header, $rows)
    );
  
    $header = array(
      array('data' => t('Add Sort'), 'colspan' => '2')
    );
  
    $rows = array();
    $rows[] = array(
      drupal_render($form['sorton']['addcomp']),
      drupal_render($form['sorton']['add'])
    );
  
    $form['sorton']['addtable'] = array(
      '#value' => theme('table', $header, $rows)
    );
  }
  
  return drupal_render($form);
}

/**
 * Implementation of hook_validate
 */
function webform_report_criteria_form_validate($form_id, $form_state) {
  
}

/**
 * Implementation of hook_submit for the criteria form list buttons
 *
 * @param form_id
 *   drupal form id
 * @param form_state
 *   drupal form state and values
 */
function webform_report_criteria_form_list_submit($form_id, $form_state) {

  // load the node
  $nid = $form_state['values']['nid'];
  $node = node_load($nid);

  // get the return value of the button that was pressed
  $return = $form_state['clicked_button']['#return_value'];
  // extract the list, operation, index, and cid
  $values = explode('_', $return);
  $list = $values[0];
  $op = $values[1];
  if (count($values) >= 3) {
    $index = (int) $values[2];
  }
  if (count($values) >= 4) {
    $cid = (int) $values[3];
  }

  $upd = FALSE;
  
  // perform the operation    
  if ($op == 'add') {
    $cid = (int) $form_state['values'][$list]['addcomp'];
    if ($cid != 0) {
      if ($list == 'columns') {
        $node->columns[] = array(
          'cid' => $cid,
          'format' => '',
          'option' => '',
          'hidden' => FALSE
        );
        $upd = TRUE;
      }
      else if ($list == 'sorton') {
        $node->sorton[] = array(
          'cid' => $cid,
          'order' => SORT_ASC
        );
        $upd = TRUE;
      }
      else if ($list == 'filters') {
        $node->filters[] = array(
          'field' => $cid,
          'type' => 0,
          'value' => ''
        );
        $upd = TRUE;
      }
    }
  }
  
  else if ($op == 'delete' && $index >= 0) {
    if ($list == 'columns') {
      // remove any sort columns that are based on deleted column
      $list = array();
      foreach($node->sorton as $si => $sort) {
        if ($sort['cid'] == $node->columns[$index]['cid']) {
          $list[$si] = $si;
        }
      }
      if (count($list) > 0) {
        $node->sorton = array_values(array_diff_key($node->sorton, $list));
      }
      $node->columns = array_values(array_diff_key($node->columns, array($index => $index)));
      $upd = TRUE;
    }
    else if ($list == 'sorton') {
      $node->sorton = array_values(array_diff_key($node->sorton, array($index => $index)));
      $upd = TRUE;
    }
    else if ($list == 'filters') {
      $node->filters = array_values(array_diff_key($node->filters, array($index => $index)));
      $upd = TRUE;
    }
  }
  
  else {
    
    if ($list == 'columns') {
      $clist = $node->columns;
    }
    else if ($list == 'sorton') {
      $clist = $node->sorton;
    }
    else if ($list == 'filters') {
      $clist = $node->filters;
    }
    
    if ($op == 'top' && $cid != 0) {
      if ($index > 0) {
        for ($i = $index; $i > 0; $i--) {
          $tmp = $clist[$i];
          $clist[$i] = $clist[$i-1];
          $clist[$i-1] = $tmp;
        }
        $upd = TRUE;
      }
    }
  
    else if ($op == 'up' && $cid != 0) {
      if ($index > 0) {
          $tmp = $clist[$index];
          $clist[$index] = $clist[$index-1];
          $clist[$index-1] = $tmp;
      }
      $upd = TRUE;
    }
  
    else if ($op == 'down' && $cid != 0) {
      if ($index >= 0 && $index < count($clist)) {
          $tmp = $clist[$index];
          $clist[$index] = $clist[$index+1];
          $clist[$index+1] = $tmp;
      }
      $upd = TRUE;
    }
  
    else if ($op == 'end' && $cid != 0) {
      if ($index >= 0 && $index < count($clist)) {
        for ($i = $index; $i < count($clist) - 1; $i++) {
          $tmp = $clist[$i];
          $clist[$i] = $clist[$i+1];
          $clist[$i+1] = $tmp;
        }
        $upd = TRUE;
      }
    }
    
    if ($upd) {
      if ($list == 'columns') {
        $node->columns = $clist;
      }
      else if ($list == 'sorton') {
        $node->sorton = $clist;
      }
      else if ($list == 'filters') {
        $node->filters = $clist;
      }
    }
  }

  // update the node  
  if ($upd) {
    node_save($node);
  }
}

/**
 * Implementation of hook_submit for the criteria form update button
 *
 * @param form_id
 *   drupal form id
 * @param form_state
 *   drupal form state and values
 */
function webform_report_criteria_form_submit($form_id, $form_state) {

  // load the node
  $nid = $form_state['values']['nid'];
  $node = node_load($nid);

  // update lists
  foreach (element_children($form_state['values']['columns']) as $index) {
    if (is_numeric($index)) {
      if (array_key_exists($index, $node->columns)) {
        if (isset($form_state['values']['columns'][$index]['format'])) {
          $node->columns[$index]['format'] = $form_state['values']['columns'][$index]['format'];
        }
        if (isset($form_state['values']['columns'][$index]['option'])) {
          $node->columns[$index]['option'] = $form_state['values']['columns'][$index]['option'];
        }
        if (isset($form_state['values']['columns'][$index]['hidden'])) {
          $node->columns[$index]['hidden'] = $form_state['values']['columns'][$index]['hidden'];
        }
      }
    }
  }

  foreach (element_children($form_state['values']['filters']) as $index) {
    if (is_numeric($index)) {
      if (array_key_exists($index, $node->filters)) {
        $node->filters[$index]['type'] = $form_state['values']['filters'][$index]['ftype'];
        // see if we have two values (between)
        if (array_key_exists('value1', $form_state['values']['filters'][$index])) {
          $v = array();
          $v[] = $form_state['values']['filters'][$index]['value1'];
          $v[] = $form_state['values']['filters'][$index]['value2'];
          // combine the values
          $value = implode('|', $v);
        }
        // just one value
        else {
          $value = $form_state['values']['filters'][$index]['value'];
        }
        $node->filters[$index]['value'] = $value;
      }
    }
  }

  foreach (element_children($form_state['values']['sorton']) as $index) {
    if (is_numeric($index)) {
      if (array_key_exists($index, $node->sorton)) {
        $node->sorton[$index]['order'] = $form_state['values']['sorton'][$index]['order'];
      }
    }
  }

  // update options
  $node->options['results_per_page'] = $form_state['values']['results_per_page'];
  $node->options['hide_csv'] = $form_state['values']['hide_csv'];
  $node->options['search_form'] = $form_state['values']['search_form'];
  if (user_access('use PHP code in webform reports') && isset($form_state['values']['php_code'])) {
    $node->options['php_code'] = $form_state['values']['php_code'];
  }
  
  // update the node  
  node_save($node);

  drupal_set_message(t('Webform Report has been updated.'));
}

/**
 * Generate a form for searching report data
 *
 * @param form_state
 *   drupal form state
 * *param node
 *   current node object
 * @return 
 *   an array of form elements
 */
 function webform_report_search_form($form_state, $node) {
  
  $form = array();
  
  if (isset($node->wnid)) {  
  
    $webform_components = _webform_report_get_components($node->wnid);

    $columns = _webform_report_get_columns($node, $webform_components);

    $comp_options = array();
    $comp_options[-1] = t('Search All Columns');
    foreach($columns as $index => $comp) {
      // don't include links in options
      if ($index > -5) {
        $comp_options[$index] = $comp['name'];
      }
    }
  
    // filters fieldset  
    $form['search'] = array(
      '#type' => 'fieldset', 
      '#title' => t('Search Report'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
      '#tree' => TRUE
    );
    
    $form['search']['nid'] = array(
      '#type' => 'hidden',
      '#value' => $node->nid
    );
  
    // get current session search values
    $search = &$_SESSION['search_form'][$node->nid];
    
    $default = '';
    if (is_array($search)) {
      if (array_key_exists('column', $search)) {
        $default = $search['column'];
      }
    }
    $form['search']['column'] = array(
      '#type' => 'select',
      '#title' => t('Column to search'),
      '#options' => $comp_options,
      '#default_value' => $default,
      '#required' => FALSE
    );
  
    $default = '';
    if (is_array($search)) {
      if (array_key_exists('value', $search)) {
        $default = $search['value'];
      }
    }
    $form['search']['value'] = array(
      '#type' => 'textfield',
      '#title' => t('Value to search for'),
      '#default_value' => $default,
      '#required' => FALSE
    );

    $form['search']['search'] = array(
      '#type' => 'submit',
      '#value' => t('Search'),
      '#weight' => 1
    );
  
    $form['search']['reset'] = array(
      '#type' => 'submit',
      '#value' => t('Reset'),
      '#weight' => 2
    );
    
  }   // end -   if (isset($node->wnid))...
  
  return $form;
}

/**
 * Implementation of hook_submit for the report search form
 *
 * @param form_id
 *   drupal form id
 * @param form_state
 *   drupal form state and values
 */
function webform_report_search_form_submit($form_id, $form_state) {

  // get current node id
  $nid = $form_state['values']['search']['nid'];
  
  // refresh
  if ($form_state['values']['op'] == 'Search') {
  
    // save search values in session variable
    // include node id so values will not be 
    // used in other nodes
    $_SESSION['search_form'][$nid] = array(
      'column' => $form_state['values']['search']['column'],
      'value' => $form_state['values']['search']['value']
    );
  }
  
  // reset
  else {
    unset($_SESSION['search_form'][$nid]);
  }
  
}
