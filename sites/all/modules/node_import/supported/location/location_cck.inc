<?php

/**
 * @file
 * Support file for contrib Location CCK Field module.
 */

/**
 * Implementation of hook_node_import_fields().
 */
function location_cck_node_import_fields($type) {
  $fields = array();

  // Get location_cck fields for this node type
  foreach (node_import_cck_fields($type, 'location_cck') as $field => $settings) {  
    $cck_field = 'location_cck:' . $field;
    $fields[$cck_field . ':latitude'] = array(
      'title' => t('Latitude'),
      'group' => t('Location CCK Field'),
      'has_multiple' => $settings['multiple'] > 1,
    );

    $fields[$cck_field . ':longitude'] = array(
      'title' => t('Longitude'),
      'group' => t('Location CCK Field'),
      'has_multiple' => $settings['multiple'] > 1,
    );
    foreach (location_field_names() as $fieldname => $fieldtitle) {
      if ($settings['location_settings']['form']['fields'][$fieldname]['collect']) {
        $fields[$cck_field . ':' . $fieldname] = array(
          'title' => $fieldtitle,
          'group' => t('Location CCK Field'),
          'has_multiple' => $settings['multiple'] > 1,
          'is_required' => $settings['required'],
          'default_value' => $settings['location_settings']['form']['fields'][$fieldname]['default'],
          'weight' => $settings['location_settings']['form']['fields'][$fieldname]['weight'],
        );

        // Set allowed values for Country field
        if ($fieldname == 'country') {
          $fields[$cck_field . ':' . $fieldname]['allowed_values'] = array_merge(array('xx' => 'NOT LISTED'), location_get_iso3166_list());
        }
        
      }
    }  
  }
  return $fields;
}

/**
 * Implementation of hook_node_import_defaults().
 */
function location_cck_node_import_defaults($type, $defaults, $fields, $map) {
  $form = array();

  // Get location_cck fields for this node type
  foreach (node_import_cck_fields($type, 'location_cck') as $field => $settings) {  
    $settings = content_fields($fieldname, $node_type);
    $cck_field = 'location_cck:' . $field;

    if (isset($settings['multiple']) && $settings['multiple'] > 0) {
      foreach (location_field_names() as $fieldname => $fieldtitle) {
        if ($settings['location_settings']['form']['fields'][$fieldname]['collect']) {
          $defaults[$cck_field . ':' . $fieldname] = isset($defaults[$cck_field . ':' . $fieldname]) ? $defaults[$cck_field . ':' . $fieldname] : '';
          $form[$cck_field . ':' . $fieldname] = location_invoke_locationapi($defaults[$cck_field . ':' . $fieldname], 'field_expand', $fieldname, $settings['location_settings']['form']['fields'][$fieldname]['collect'], array());
        }
      }
    }
  }

  return $form;
}

/**
 * Implementation of hook_node_import_values_alter().
 */
function location_cck_node_import_values_alter(&$values, $type, $defaults, $options, $fields, $preview) {
  // Get location_cck fields for this node type
  foreach (node_import_cck_fields($type, 'location_cck') as $field => $settings) {  
//dsm($values);
    //$settings = content_fields('field_location', $node_type);
//    $values['locations_cck'] = array();
    $cck_field = 'location_cck:' . $field;

    $all_location_field_names = array();
    foreach (array_keys(location_field_names()) as $fieldname) {
      if (isset($settings['location_settings']['form']['fields'][$fieldname]) && $settings['location_settings']['form']['fields'][$fieldname]['collect']) {
        $all_location_field_names[] = $fieldname;
      }
    }
    $all_location_field_names[] = 'latitude';
    $all_location_field_names[] = 'longitude';

    $num_values = 0;
    foreach ($all_location_field_names as $fieldname) {
      if (!is_array($values[$cck_field . ':' . $fieldname])) {
        $values[$cck_field . ':' . $fieldname] = array(0 => $values[$cck_field . ':' . $fieldname]);
      }
      $num_values = max(count($values[$cck_field . ':' . $fieldname]), $num_values);
    }

    $i = 0;
    while ($i < $num_values && $i <= $settings['multiple']) {
      $value = array();
      foreach ($all_location_field_names as $fieldname) {
        if (array_key_exists($i, $values[$cck_field . ':' . $fieldname])) {
          $value[$fieldname] = $values[$cck_field . ':' . $fieldname][$i];
        }
        else {
          $value[$fieldname] = $defaults[$cck_field . ':' . $fieldname];
        }
      }

      $value['locpick'] = array(
        'user_latitude' => isset($value['latitude']) ? $value['latitude'] : '',
        'user_longitude' => isset($value['longitude']) ? $value['longitude'] : '',
      );

      $value['location_settings'] = $settings;
      $values[$field][] = $value;
      $i++;
    }

    foreach ($all_location_field_names as $fieldname) {
      if (isset($values[$cck_field . ':' . $fieldname])) {
        unset($values[$cck_field . ':' . $fieldname]);
      }
    }
  }
}