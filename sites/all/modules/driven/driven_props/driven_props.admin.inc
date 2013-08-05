<?php
// $Id: driven_props.admin.inc,v 1.2 2010/04/03 23:18:07 arhak Exp $

function driven_props_overview_form(&$form_state, $driver_mod, $node_type) {
  $form = array(
    '#node_type' => $node_type,
    '#driver_mod' => $driver_mod,
  );
  
  $hook = 'driven_props_type_settings';
  $type_settings = module_invoke($driver_mod, $hook);

  $args_what = array('@what' => $type_settings['description_tail']);
  
  $form['driven_props'] = array(
    '#type' => 'fieldset',
    '#title' => t('Driven properties'),
    '#description' => t('Select which of the available properties will be <strong>@what</strong>.', $args_what),
    '#collapsible' => TRUE,
    '#tree' => TRUE,
  );
  
  $form['driven_props']['enabled'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable driven properties'),
    '#description' => t('Disabling this option would be enough to make all properties bellow ignored.'),
    '#default_value' => FALSE,
  );
  
  $available_props = driven_properties_available($node_type);
  $unsupported_props = array(); 
  $hook = 'unsupported_driven_props';
  if (module_hook($driver_mod, $hook)) {
    $unsupported_props = module_invoke($driver_mod, $hook, $available_props);
  }
  
  if (empty($available_props)) {
    $form['driven_props']['no_props'] = array(
      '#type' => 'item',
      '#value' => t('There are no driven properties exposed. You have to enable some property provider modules.'),
      '#prefix' => '<div class="warning">',
      '#suffix' => '</div>',
    );
  }
  foreach ($available_props as $property_id => $prop) {
    $realm = $prop['realm'];
    $target = $prop['target'];
    $elem = &$form['driven_props'][$realm];
    if (!isset($elem)) {
      $elem = array(
        '#type' => 'fieldset',
        '#title' => $prop['realm_title'],
        '#collapsible' => TRUE,
      );
    }
    $elem[$target]['enabled'] = array(
      '#type' => 'checkbox',
      '#title' => $prop['target_title'],
      '#default_value' => FALSE,
    );
    if (isset($unsupported_props[$property_id])) {
      $elem[$target]['enabled']['#disabled'] = TRUE;
      $elem[$target]['enabled']['#description'] = $unsupported_props[$property_id];
    }
    else {
      $elem[$target]['behaviors'] = _driven_props_retrieve_behaviors_form($driver_mod, $prop);
    }
  }
  $varname = $driver_mod . ':type:' . $node_type . ':driven_props';
  $settings = variable_get($varname, array());
  _driven_props_load_settings($form['driven_props'], $settings);
  
  $hook = 'driven_props_type_settings_form';
  if (module_hook($driver_mod, $hook)) {
    $form['driver_mod'] = module_invoke($driver_mod, $hook, $node_type);
    $form['driver_mod']['#tree'] = TRUE;
  }
  $varname = $driver_mod . ':type:' . $node_type . ':settings';
  $settings = variable_get($varname, array());
  _driven_props_load_settings($form['driver_mod'], $settings);
  
  $form += driven_form_signature();
  $form['buttons']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save configuration'),
  );
  $form['buttons']['reset'] = array(
    '#type' => 'submit',
    '#value' => t('Reset to defaults'),
  );
  $form['#theme'] = 'system_settings_form';
  
  return $form;
}

function _driven_props_load_settings(&$form, $settings) {
  foreach (element_children($form) as $key) {
    if (!isset($settings[$key])) {
      // stored settings are #tree
      // if there is nothing at this level then this is a dead end
      continue; 
    } 
    if (array_key_exists('#default_value', $form[$key])) {
      // replace the default if a stored value exists
      $form[$key]['#default_value'] = $settings[$key];
    }
    else {
      _driven_props_load_settings($form[$key], $settings[$key]);
    }
  }
}

function driven_props_overview_form_submit($form, $form_state) {
  $node_type = $form['#node_type'];
  $driver_mod = $form['#driver_mod'];
  $values = $form_state['values'];
  $op = isset($values['op']) ? $values['op'] : '';
  
  if ($op == t('Reset to defaults')) {
    driven_props_delete_settings($driver_mod, $node_type);
  }
  else {
    // IMPORTANT: modules are responsible of
    // deleting their variables on uninstall
  
    // at least we have the 'enabled' key
    $varname = $driver_mod . ':type:' . $node_type . ':driven_props';
    variable_set($varname, $values['driven_props']);

    $varname = $driver_mod . ':type:' . $node_type . ':settings';
    if (isset($values['driver_mod'])) {
      variable_set($varname, $values['driver_mod']);
    }
    else {
      // if the module is not exposing settings
      // avoid old garbage settings from being picked up later
      variable_del($varname);
    }
  }
}

/**
 * Deferred function driven_props_types_overview_alter().
 */
function _driven_props_types_overview_alter(&$header, &$rows) {
  $type_settings = driven_props_invoke_type_settings(); 
  if (empty($type_settings)) {
    return; // nothing to do
  }
  
  // point to the default local task,
  $uri = 'driven-props';
  if (count($type_settings) == 1) {
    // there is only one driver_mod
    $driver_mod = array_pop(array_keys($type_settings));
    // don't point to the default local task,
    // use the full task's path instead
    //$uri = 'driven-props/' . str_replace('_', '-', $driver_mod);
    $settings = array_pop($type_settings);
    $link_text = $settings['task_title'];
  }
  else {
    $link_text = t('driven properties');
  }
  
  // @d6
  $types = node_get_types();
  $row_index = -1;
  $header[3]['colspan']++;
  foreach ($types as $key => $type) {
    // mimic CCK behavior,
    // they check node_hook in content types page,
    // but not in hook_menu
    if (node_hook($type, 'form')) {
      $type_url_str = str_replace('_', '-', $type->type);
      $row = &$rows[++$row_index];
      // keep delete operation last
      $delete = array_pop($row);
      $row[] = array('data' => l($link_text, 'admin/content/node-type/' . $type_url_str . '/' . $uri));
      $row[] = $delete;
    }
  }
}

/**
 * Menu callback: replacement for node_overview_types().
 */
function driven_props_types_overview() {
  // @d6
  // mimics content_types_overview()
  // except for:
  // - colspan numbers
  // - not providing hook_content_types_overview_alter
  $types = node_get_types();
  $names = node_get_types('names');
  $header = array(t('Name'), t('Type'), t('Description'), array('data' => t('Operations'), 'colspan' => '2'),);
  $rows = array();

  foreach ($names as $key => $name) {
    $type = $types[$key];
    if (node_hook($type, 'form')) {
      $type_url_str = str_replace('_', '-', $type->type);
      $row = array(
        check_plain($name),
        check_plain($type->type),
      );
      // Make the description smaller
      $row[] = array('data' => filter_xss_admin($type->description), 'class' => 'description');
      // Set the edit column.
      $row[] = array('data' => l(t('edit'), 'admin/content/node-type/'. $type_url_str));
      
      // Set the delete column.
      if ($type->custom) {
        $row[] = array('data' => l(t('delete'), 'admin/content/node-type/'. $type_url_str .'/delete'));
      }
      else {
        $row[] = array('data' => '');
      }

      $rows[] = $row;
    }
  }

  // Allow external modules alter the table headers and rows.
  //foreach (module_implements('content_types_overview_alter') as $module) {
  //  $function = $module .'_content_types_overview_alter';
  //  $function($header, $rows);
  //}
  _driven_props_types_overview_alter($header, $rows);

  if (empty($rows)) {
    // if there were no resulting rows then header shouldn't have more columns/colspans
    $rows[] = array(array('data' => t('No content types available.'), 'colspan' => '5', 'class' => 'message'));
  }

  return theme('table', $header, $rows) .theme('content_overview_links');
}
