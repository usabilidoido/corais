<?php

/**
 * @file
 * Support file for the CCK Text module.
 */

/**
 * Implementation of hook_node_import_fields().
 */
function text_node_import_fields($type) {
  $fields = array();

  foreach (node_import_cck_fields($type, 'text') as $fieldname => $fieldinfo) {
    foreach ($fieldinfo['columns'] as $colname => $colinfo) {
      $cck_fieldname = node_import_cck_name($fieldname, $colname);

      $fields[$cck_fieldname] = node_import_cck_field_defaults($fieldinfo);

      switch ($colname) {
        case 'value':
          $fields[$cck_fieldname]['title'] = $fieldinfo['widget']['label'];
          $fields[$cck_fieldname]['map_required'] = $fieldinfo['required'];
          break;

        case 'format':
          $fields[$cck_fieldname]['title'] = $fieldinfo['widget']['label'] .' - '. t('Input format');
          $fields[$cck_fieldname]['allowed_values'] = node_import_extract_property(filter_formats(), 'name');
          $fields[$cck_fieldname]['default_value'] = filter_resolve_format(FILTER_FORMAT_DEFAULT);
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
 * Implementation of hook_node_import_defaults().
 */
function text_node_import_defaults($type, $defaults, $fields, $map) {
  $form = array();

  foreach (node_import_cck_fields($type, 'text') as $fieldname => $fieldinfo) {
    foreach ($fieldinfo['columns'] as $colname => $colinfo) {
      $cck_fieldname = node_import_cck_name($fieldname, $colname);

      switch ($colname) {
        case 'format':
          $form[$cck_fieldname] = array(
            '#title' => t('Input format'),
            '#type' => 'radios',
            '#options' => node_import_extract_property(filter_formats(), 'name'),
            '#default_value' => isset($defaults[$cck_fieldname]) ? $defaults[$cck_fieldname] : filter_resolve_format(FILTER_FORMAT_DEFAULT),
          );
          break;
      }
    }
  }

  return $form;
}

