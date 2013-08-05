<?php

/**
 * @file
 * Support file for contrib CCK module.
 */

/**
 * Returns a list of CCK fields.
 *
 * @param $type
 *   Node import type.
 *
 * @param $module
 *   Support module.
 *
 * @param $reset
 *   Reset internal cache.
 *
 * @return
 *   Array of CCK field definitions.
 */
function node_import_cck_fields($type, $module, $reset = FALSE) {
  static $fields;

  if (!isset($fields) || $reset) {
    $fields = array();
  }

  if (!isset($fields[$type])) {
    $fields[$type] = array();

    if (($node_type = node_import_type_is_node($type)) !== FALSE) {
      $node_info = content_types($node_type);
      foreach ($node_info['fields'] as $fieldname => $fieldinfo) {
        if (!isset($fields[$type][$fieldinfo['module']])) {
          $fields[$type][$fieldinfo['module']] = array();
        }
        $fields[$type][$fieldinfo['module']][$fieldname] = $fieldinfo;
      }
    }
  }

  if (!isset($fields[$type][$module])) {
    $fields[$type][$module] = array();
  }

  return $fields[$type][$module];
}

/**
 * Creates a unique field/column name.
 */
function node_import_cck_name($fieldname, $colname) {
  return 'cck:'. $fieldname .':'. $colname;
}

/**
 * Returns an array of default values for the field information array
 * for CCK fields.
 */
function node_import_cck_field_defaults($fieldinfo) {
  return array(
    'group' => (count($fieldinfo['columns']) > 1 ? $fieldinfo['widget']['label'] : ''),
    'tips' => array(
      t('Imports into %name CCK field (%type type).', array('%name' => $fieldinfo['field_name'], '%type' => $fieldinfo['type'])),
    ),
    'has_multiple' => $fieldinfo['multiple'],
    'module' => $fieldinfo['module'],
    'allowed_values' => content_allowed_values($fieldinfo),
    'cck:fieldname' => $fieldinfo['field_name'],
    'cck:fieldinfo' => $fieldinfo,
  );
}

/**
 * Implementation of hook_node_import_values_alter().
 */
function content_node_import_values_alter(&$values, $type, $defaults, $options, $fields, $preview) {
  if (($node_type = node_import_type_is_node($type)) !== FALSE) {
    foreach ($fields as $fieldname => $fieldinfo) {
      if (isset($fieldinfo['cck:fieldname'])) {
        $values[$fieldinfo['cck:fieldname']] = array();

        $num_values = 0;
        foreach ($fieldinfo['cck:fieldinfo']['columns'] as $colname => $colinfo) {
          $cck_fieldname = node_import_cck_name($fieldinfo['cck:fieldname'], $colname);
          if (isset($values[$cck_fieldname])) {
            if (!is_array($values[$cck_fieldname])) {
              $values[$cck_fieldname] = array(0 => $values[$cck_fieldname]);
            }
            $num_values = max(count($values[$cck_fieldname]), $num_values);
          }
        }

        $i = 0;
        while ($i < $num_values) {
          $value = array();
          foreach ($fieldinfo['cck:fieldinfo']['columns'] as $colname => $colinfo) {
            $cck_fieldname = node_import_cck_name($fieldinfo['cck:fieldname'], $colname);
            if (isset($values[$cck_fieldname]) && array_key_exists($i, $values[$cck_fieldname])) {
              $value[$colname] = $values[$cck_fieldname][$i];
            }
            else {
              $value[$colname] = isset($defaults[$cck_fieldname]) ? $defaults[$cck_fieldname] : NULL;
            }
          }
          $values[$fieldinfo['cck:fieldname']][$i] = $value;
          $i++;
        }
/*
        foreach ($fieldinfo['cck:fieldinfo']['columns'] as $colname => $colinfo) {
          $cck_fieldname = node_import_cck_name($fieldinfo['cck:fieldname'], $colname);
          unset($values[$cck_fieldname]);
        }
*/
      }
    }
  }
}

