<?php

/**
 * @file
 * Support file for the CCK User reference module.
 */

/**
 * Implementation of hook_node_import_fields().
 */
function userreference_node_import_fields($type) {
  $fields = array();

  foreach (node_import_cck_fields($type, 'userreference') as $fieldname => $fieldinfo) {
    foreach ($fieldinfo['columns'] as $colname => $colinfo) {
      $cck_fieldname = node_import_cck_name($fieldname, $colname);

      $fields[$cck_fieldname] = node_import_cck_field_defaults($fieldinfo);

      switch ($colname) {
        case 'uid':
          $fields[$cck_fieldname]['title'] = $fieldinfo['widget']['label'];
          $fields[$cck_fieldname]['input_format'] = 'user_reference';
          $fields[$cck_fieldname]['output_format'] = ($fieldinfo['widget']['type'] == 'userreference_autocomplete' ? 'name' : 'uid');
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
function userreference_node_import_values_alter(&$values, $type, $defaults, $options, $fields, $preview) {
  foreach (node_import_cck_fields($type, 'userreference') as $fieldname => $fieldinfo) {
    if ($fieldinfo['widget']['type'] == 'userreference_autocomplete') {
      foreach ($values[$fieldname] as $i => $value) {
        // This is just braindead - because of some wrapper, the value
        // needs to be:
        // array(0 => array('uid' => array('uid' => 'name')))
        // instead of:
        // array(0 => array('uid' => 'name')).
        $values[$fieldname][$i]['uid'] = array('uid' => $values[$fieldname][$i]['uid']);
      }
    }
  }
}

