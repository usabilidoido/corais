<?php

/**
 * Implementation of hook_features_export_options().
 */
function urlfill_features_export_options() {
  $query = db_query("SELECT type, node_type, field_name FROM {urlfill}");
  while ($result = db_fetch_array($query)) {
    $machine_name = $result['type'].'_'.$result['node_type'].'_'.$result['field_name'];
    $human_name = $result['node_type'] ? $result['node_type'].': '.$result['field_name'] : $result['field_name'];
    $options[$machine_name] = $human_name;
  }
  return $options;
}

/**
 * Implementation of hook_features_export()
 */
function urlfill_features_export($data, &$export, $module_name = '') {
  $export['dependencies'][] = 'urlfill';
  
  foreach ($data as $key => $title) {
    $export['features']['urlfill'][$key] = $title;
  }
}

/**
 * Implmentation of hook_features_export_render()
 */
function urlfill_features_export_render($module_name = '', $data) {
  $code[] = '  $items = array();';
  foreach ($data as $machine_name) {
    $urlfill = urlfill_features_get_urlfill($machine_name);
    $serialized = serialize($urlfill);
    $code[] = '  $items["'.$machine_name.'"] = array(';
    $code[] = "    '$machine_name' => '$serialized',";
    $code[] = '  );';
    $i++;
  }
  $code[] = '  return $items;';
  $code = implode("\n", $code);
  return array('urlfill_defaults' => $code);
}

/**
 * Implmentation of hook_features_rebuild()
 */
function urlfill_features_rebuild($module) {
  $defaults = module_invoke($module, 'urlfill_defaults');
  if (!empty($defaults)) {
    $types = array();
    foreach ($defaults as $item) {
      $field = unserialize(array_pop($item));
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
  }
}

/**
 * Implementation of hook_features_revert().
 */
function urlfill_features_revert($module) {
  return urlfill_features_rebuild($module);
}

function urlfill_features_get_urlfill($machine_name) {
  // First decode the machine name
  if (substr($machine_name,0,4) == 'node') {
    $type = 'node';
    if (substr($machine_name,-4) == 'body') {
      $field_name = 'body';
      $node_type = substr($machine_name,5,-5);
    }
    elseif (substr($machine_name,-5) == 'title') {
      $field_name = 'title';
      $node_type = substr($machine_name,5,-6);
    }
    
  }
  elseif (substr($machine_name,0,5) == 'field') {
    $node_type = '';
    $type = 'field';
    $field_name = substr($machine_name,7);
  }
  
  $db_result = db_query("SELECT * FROM {urlfill} WHERE field_name = '%s' AND type ='%s' AND node_type = '%s'", $field_name, $type, $node_type);
  
  return db_fetch_array($db_result);
  
}
