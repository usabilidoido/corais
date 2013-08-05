<?php

/**
 * @file
 * Support file for the CCK Node reference module.
 */

/**
 * Implementation of hook_node_import_fields().
 */
function nodereference_node_import_fields($type) {
  $fields = array();

  foreach (node_import_cck_fields($type, 'nodereference') as $fieldname => $fieldinfo) {
    foreach ($fieldinfo['columns'] as $colname => $colinfo) {
      $cck_fieldname = node_import_cck_name($fieldname, $colname);

      $fields[$cck_fieldname] = node_import_cck_field_defaults($fieldinfo);

      switch ($colname) {
        case 'nid':
          $fields[$cck_fieldname]['title'] = $fieldinfo['widget']['label'];
          $fields[$cck_fieldname]['input_format'] = 'node_reference';
          $fields[$cck_fieldname]['output_format'] = ($fieldinfo['widget']['type'] == 'nodereference_autocomplete' ? 'name' : 'nid');
          $fields[$cck_fieldname]['map_required'] = $fieldinfo['required'];
          break;

        default:
          $fields[$cck_fieldname]['title'] = t('Unsupported: ') . $fieldinfo['widget']['label'] .' - '. $colname;
          break;
      }
    }
  }

  return $fields;
}

/**
 * Implementation of hook_node_import_values_alter().
 */
function nodereference_node_import_values_alter(&$values, $type, $defaults, $options, $fields, $preview) {
  foreach (node_import_cck_fields($type, 'nodereference') as $fieldname => $fieldinfo) {
    if ($fieldinfo['widget']['type'] == 'nodereference_autocomplete') {
      foreach ($values[$fieldname] as $i => $value) {
        // This is just braindead - because of some wrapper, the value
        // needs to be:
        // array(0 => array('nid' => array('nid' => 'name')))
        // instead of:
        // array(0 => array('nid' => 'name')).
        $values[$fieldname][$i]['nid'] = array('nid' => $values[$fieldname][$i]['nid']);
      }
    }
  }
}

