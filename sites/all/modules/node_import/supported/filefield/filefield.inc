<?php

/**
 * @file
 * Support file for the CCK FileField module.
 */

/**
 * Implementation of hook_node_import_fields().
 */
function filefield_node_import_fields($type) {
  $fields = array();

  foreach (node_import_cck_fields($type, 'filefield') as $fieldname => $fieldinfo) {
    foreach ($fieldinfo['columns'] as $colname => $colinfo) {
      $cck_fieldname = node_import_cck_name($fieldname, $colname);

      $fields[$cck_fieldname] = node_import_cck_field_defaults($fieldinfo);

      switch ($colname) {
        case 'fid':
          $fields[$cck_fieldname]['title'] = $fieldinfo['widget']['label'];
          $fields[$cck_fieldname]['input_format'] = 'filepath';
          $fields[$cck_fieldname]['to_directory'] = $fieldinfo['widget']['file_path'];
          $fields[$cck_fieldname]['map_required'] = $fieldinfo['required'];
          break;

        case 'list':
          $fields[$cck_fieldname]['title'] = $fieldinfo['widget']['label'] .' - '. t('List');
          $fields[$cck_fieldname]['is_mappable'] = $fieldinfo['list_field'];
          $fields[$cck_fieldname]['input_format'] = 'boolean';
          $fields[$cck_fieldname]['default_value'] = $fieldinfo['list_default'];
          break;

        case 'data':
          // The data column stores a number of values, each needing a separate
          // field! Only 'description' is supported by filefield module, we
          // make the others unmappable by default.
          foreach (filefield_data_info() as $key => $datainfo) {
            $data_fieldname = $cck_fieldname . ':' . $key;
            $fields[$data_fieldname] = $fields[$cck_fieldname];

            switch ($key) {
              case 'description':
                $fields[$data_fieldname]['title'] = $fieldinfo['widget']['label'] .' - '. t('Description');
                $fields[$data_fieldname]['is_mappable'] = $fieldinfo['description_field'];
                $fields[$data_fieldname]['default_value'] = '';
                break;

              default:
                $fields[$data_fieldname]['title'] = t('Unsupported: ') . $fieldinfo['widget']['label'] .' - '. $datainfo['title'];
                $fields[$data_fieldname]['is_mappable'] = FALSE;
                break;
            }
          }

          // Hide the data field.
          $fields[$cck_fieldname]['is_mappable'] = FALSE;
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
function filefield_node_import_defaults($type, $defaults, $fields, $map) {
  $form = array();

  foreach (node_import_cck_fields($type, 'filefield') as $fieldname => $fieldinfo) {
    $data_fieldname = node_import_cck_name($fieldname, 'data') . ':description';
    if (isset($fields[$data_fieldname]) && $fields[$data_fieldname]['is_mappable']) {
      $form[$data_fieldname] = array(
        '#title' => $fieldinfo['widget']['label'] .' - '. t('Description'),
        '#type' => 'textfield',
        '#maxlength' => 128,
        '#size' => 60,
        '#default_value' => isset($defaults[$data_fieldname]) ? $defaults[$data_fieldname] : $fields[$data_fieldname]['default_value'],
      );
    }

    $list_fieldname = node_import_cck_name($fieldname, 'list');
    if (isset($fields[$list_fieldname]) && $fields[$list_fieldname]['is_mappable']) {
      $form[$list_fieldname] = array(
        '#title' => $fieldinfo['widget']['label'] .' - '. t('List'),
        '#type' => 'radios',
        '#options' => array(
          '0' => t('No'),
          '1' => t('Yes'),
        ),
        '#default_value' => isset($defaults[$list_fieldname]) ? $defaults[$list_fieldname] : $fields[$list_fieldname]['default_value'],
      );
    }
  }

  return $form;
}

/**
 * Implementation of hook_node_import_values_alter().
 */
function filefield_node_import_values_alter(&$values, $type, $defaults, $options, $fields, $preview) {
  foreach (node_import_cck_fields($type, 'filefield') as $fieldname => $fieldinfo) {
    $num_values = count($values[$fieldname]);
    if ($num_values > 0) {
      foreach (filefield_data_info() as $key => $datainfo) {
        $data_fieldname = node_import_cck_name($fieldname, 'data') . ':' . $key;
        $data_values = isset($values[$data_fieldname]) ? $values[$data_fieldname] : '';
        $data_values = is_array($data_values) ? $data_values : array($data_values);
        $data_values = $data_values + array_fill(count($data_values), $num_values, isset($defaults[$data_fieldname]) ? $defaults[$data_fieldname] : $fields[$data_fieldname]['default_value']);
        $data_values = array_slice($data_values, 0, $num_values);

        foreach ($values[$fieldname] as $i => $value) {
          if (!isset($values[$fieldname][$i]['data']) || !is_array($values[$fieldname][$i]['data'])) {
            $values[$fieldname][$i]['data'] = array();
          }

          $values[$fieldname][$i]['data'][$key] = $data_values[$i];
        }
      }
    }
  }
}

