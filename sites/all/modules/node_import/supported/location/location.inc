<?php

/**
 * @file
 * Support file for contrib Location module.
 */

/**
 * Implementation of hook_node_import_fields().
 */
function location_node_import_fields($type) {
  $fields = array();

  if (($node_type = node_import_type_is_node($type)) !== FALSE) {
    $settings = variable_get('location_settings_node_'. $node_type, array());

    if (isset($settings['multiple']['max']) && $settings['multiple']['max'] > 0) {
      $fields['location:latitude'] = array(
        'title' => t('Latitude'),
        'group' => t('Location'),
        'has_multiple' => $settings['multiple']['max'] > 1,
      );

      $fields['location:longitude'] = array(
        'title' => t('Longitude'),
        'group' => t('Location'),
        'has_multiple' => $settings['multiple']['max'] > 1,
      );

      foreach (location_field_names() as $fieldname => $fieldtitle) {
        if ($settings['form']['fields'][$fieldname]['collect']) {
          $fields['location:'. $fieldname] = array(
            'title' => $fieldtitle,
            'group' => t('Location'),
            'has_multiple' => $settings['multiple']['max'] > 1,
            'map_required' => $settings['multiple']['min'] > 0,
            'default_value' => $settings['form']['fields'][$fieldname]['default'],
            'weight' => $settings['form']['fields'][$fieldname]['weight'],
          );

          switch ($fieldname) {
            case 'name':
            case 'street':
            case 'additional':
            case 'city':
            case 'province':
            case 'postal_code':
              break;

            case 'country':
              $fields['location:'. $fieldname]['allowed_values'] = array_merge(array('xx' => 'NOT LISTED'), location_get_iso3166_list());
              break;
          }
        }
      }
    }
  }

  return $fields;
}

/**
 * Implementation of hook_node_import_defaults().
 */
function location_node_import_defaults($type, $defaults, $fields, $map) {
  $form = array();

  if (($node_type = node_import_type_is_node($type)) !== FALSE) {
    $settings = variable_get('location_settings_node_'. $node_type, array());

    if (isset($settings['multiple']['max']) && $settings['multiple']['max'] > 0) {
      foreach (location_field_names() as $fieldname => $fieldtitle) {
        if ($settings['form']['fields'][$fieldname]['collect']) {
          $defaults['location:'. $fieldname] = isset($defaults['location:'. $fieldname]) ? $defaults['location:'. $fieldname] : '';
          $form['location:'. $fieldname] = location_invoke_locationapi($defaults['location:'. $fieldname], 'field_expand', $fieldname, $settings['form']['fields'][$fieldname]['collect'], array());
        }
      }
    }
  }

  return $form;
}

/**
 * Implementation of hook_node_import_values_alter().
 */
function location_node_import_values_alter(&$values, $type, $defaults, $options, $fields, $preview) {
  if (($node_type = node_import_type_is_node($type)) !== FALSE) {
    $settings = variable_get('location_settings_node_'. $node_type, array());

    if (isset($settings['multiple']['max']) && $settings['multiple']['max'] > 0) {
      $values['locations'] = array();

      $all_location_field_names = array();
      foreach (array_keys(location_field_names()) as $fieldname) {
        if (isset($settings['form']['fields'][$fieldname]) && $settings['form']['fields'][$fieldname]['collect']) {
          $all_location_field_names[] = $fieldname;
        }
      }
      $all_location_field_names[] = 'latitude';
      $all_location_field_names[] = 'longitude';

      $num_values = 0;
      foreach ($all_location_field_names as $fieldname) {
        if (!is_array($values['location:'. $fieldname])) {
          $values['location:'. $fieldname] = array(0 => $values['location:'. $fieldname]);
        }
        $num_values = max(count($values['location:'. $fieldname]), $num_values);
      }

      $i = 0;
      while ($i < $num_values && $i <= $settings['multiple']['max']) {
        $value = array();

        foreach ($all_location_field_names as $fieldname) {
          if (array_key_exists($i, $values['location:'. $fieldname])) {
            $value[$fieldname] = $values['location:'. $fieldname][$i];
          }
          else {
            $value[$fieldname] = $defaults['location:'. $fieldname];
          }
        }

        $value['locpick'] = array(
          'user_latitude' => isset($value['latitude']) ? $value['latitude'] : '',
          'user_longitude' => isset($value['longitude']) ? $value['longitude'] : '',
        );

        $value['location_settings'] = $settings;
        $values['locations'][] = $value;
        $i++;
      }

      foreach ($all_location_field_names as $fieldname) {
        if (isset($values['location:'. $fieldname])) {
          unset($values['location:'. $fieldname]);
        }
      }
    }
  }
}

