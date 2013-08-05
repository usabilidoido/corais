<?php

/**
 * @file
 * Support file for the CCK ImageField module.
 */

/**
 * Implementation of hook_node_import_fields_alter().
 */
function imagefield_node_import_fields_alter(&$fields, $type) {
  foreach (node_import_cck_fields($type, 'filefield') as $fieldname => $fieldinfo) {
    if ($fieldinfo['widget']['module'] == 'imagefield') {
      $cck_fieldname = node_import_cck_name($fieldname, 'data');
      foreach (imagefield_filefield_data_info() as $key => $datainfo) {
        $data_fieldname = $cck_fieldname . ':' . $key;
        if (isset($fields[$data_fieldname])) {
          switch ($key) {
            case 'alt':
              $fields[$data_fieldname]['title'] = $fieldinfo['widget']['label'] .' - '. $datainfo['title'];
              $fields[$data_fieldname]['is_mappable'] = $fieldinfo['widget']['custom_alt'];
              $fields[$data_fieldname]['default_value'] = $fieldinfo['widget']['alt'];
              break;

            case 'title':
              $fields[$data_fieldname]['title'] = $fieldinfo['widget']['label'] .' - '. $datainfo['title'];
              $fields[$data_fieldname]['is_mappable'] = $fieldinfo['widget']['custom_title'];
              $fields[$data_fieldname]['default_value'] = $fieldinfo['widget']['title'];
              break;
          }
        }
      }
    }
  }
}

/**
 * Implementation of hook_node_import_defaults().
 */
function imagefield_node_import_defaults($type, $defaults, $fields, $map) {
  $form = array();

  foreach (node_import_cck_fields($type, 'filefield') as $fieldname => $fieldinfo) {
    if ($fieldinfo['widget']['module'] == 'imagefield') {
      $cck_fieldname = node_import_cck_name($fieldname, 'data');
      foreach (imagefield_filefield_data_info() as $key => $datainfo) {
        $data_fieldname = $cck_fieldname . ':' . $key;
        if (isset($fields[$data_fieldname]) && $fields[$data_fieldname]['is_mappable']) {
          switch ($key) {
            case 'alt':
              $form[$data_fieldname] = array(
                '#title' => $fields[$data_fieldname]['title'],
                '#type' => 'textfield',
                '#default_value' => isset($defaults[$data_fieldname]) ? $defaults[$data_fieldname] : $fields[$data_fieldname]['default_value'],
              );
              break;

            case 'title':
              $form[$data_fieldname] = array(
                '#title' => $fields[$data_fieldname]['title'],
                '#type' => 'textfield',
                '#default_value' => isset($defaults[$data_fieldname]) ? $defaults[$data_fieldname] : $fields[$data_fieldname]['default_value'],
              );
              break;
          }
        }
      }
    }
  }

  return $form;
}

