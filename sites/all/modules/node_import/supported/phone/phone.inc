<?php

/**
 * @file
 * Support file for the CCK Phone module.
 */

/**
 * Implementation of hook_node_import_fields().
 */
function phone_node_import_fields($type) {
  $fields = array();

  foreach (node_import_cck_fields($type, 'phone') as $fieldname => $fieldinfo) {
    foreach ($fieldinfo['columns'] as $colname => $colinfo) {
      $cck_fieldname = node_import_cck_name($fieldname, $colname);

      $fields[$cck_fieldname] = node_import_cck_field_defaults($fieldinfo);

      switch ($colname) {
        case 'value':
          $fields[$cck_fieldname]['title'] = $fieldinfo['widget']['label'];
          $fields[$cck_fieldname]['is_required'] = $fieldinfo['required'];
          break;

        default:
          $fields[$cck_fieldname]['title'] = t('Unsupported: ') . $fieldinfo['widget']['label'] .' - '. $colname;
          break;
      }
    }
  }

  return $fields;
}

