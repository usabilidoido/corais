<?php
/**
 * @file
 * Support file for the CCK Date module.
 */

/**
 * Implementation of hook_node_import_fields().
 */
function date_node_import_fields($type) {
  $fields = array();

  foreach (node_import_cck_fields($type, 'date') as $fieldname => $fieldinfo) {
    foreach ($fieldinfo['columns'] as $colname => $colinfo) {
      $cck_fieldname = node_import_cck_name($fieldname, $colname);

      $fields[$cck_fieldname] = node_import_cck_field_defaults($fieldinfo);

      switch ($colname) {
        case 'value':
          $fields[$cck_fieldname]['title'] = $fieldinfo['widget']['label'];
          $fields[$cck_fieldname]['input_format'] = 'date';
          $fields[$cck_fieldname]['date_format'] = $fieldinfo['widget']['type'] == 'date_select' ? DATE_ARRAY : DATE_ISO;
          $fields[$cck_fieldname]['output_format'] = DATE_ISO;
          $fields[$cck_fieldname]['map_required'] = $fieldinfo['required'];
          break;

        case 'value2':
          $fields[$cck_fieldname]['title'] = $fieldinfo['widget']['label'] .' - '. t('To date');
          $fields[$cck_fieldname]['input_format'] = 'date';
          $fields[$cck_fieldname]['date_format'] = $fieldinfo['widget']['type'] == 'date_select' ? DATE_ARRAY : DATE_ISO;
          $fields[$cck_fieldname]['output_format'] = DATE_ISO;
          $fields[$cck_fieldname]['map_required'] = $fieldinfo['required'] && ($fieldinfo['todate'] == 'required');
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
function date_node_import_values_alter(&$values, $type, $defaults, $options, $fields, $preview) {
  foreach (node_import_cck_fields($type, 'date') as $fieldname => $fieldinfo) {  
    switch ($fieldinfo['widget']['type']) {
      // Select List
      default:
      case 'date_select':
        date_node_import_values_alter_date_select($values, $fields, $fieldname, $fieldinfo);
        break;
      
      // Text Field with custom input format
      case 'date_text':
        date_node_import_values_alter_date_text($values, $fields, $fieldname, $fieldinfo);
        break;
        
      // Text Field with Date Pop-up calendar
      case 'date_popup':
        date_node_import_values_alter_date_popup($values, $fields, $fieldname, $fieldinfo);
        break;
        
    }
  }
}

function date_node_import_values_alter_date_text(&$values, $fields, $fieldname, $fieldinfo) {
  foreach ($fieldinfo['columns'] as $colname => $colinfo) {  
    // Provide default value if the field has no values.
    if (count($values[$fieldname]) == 0) {
      $values[$fieldname] = array();
      $values[$fieldname][0] = array();
      $values[$fieldname][0][$colname] = array('date' => null);
    }
    else {
      foreach ($values[$fieldname] as $i => $value) {
        // Get the format that is expected from the pop-up calendar.
        // If a custom format was entered, use it instead.
        if ($fieldinfo['widget']['input_format_custom']) {
          $widgetFormat = $fieldinfo['widget']['input_format_custom'];
        }
        else {
          $widgetFormat = $fieldinfo['widget']['input_format'];
        }

        // Remove parts that are not valid for the selected granularity
        $widgetFormat = date_limit_format($widgetFormat, $fieldinfo['granularity']);
  
        // Get the original value format we need to convert
        $time = strtotime($value[$colname]);
        
        /* $colname is 'value' in most cases.
         * We trash the original 'value' field and replace it with an array containing the various pieces entered
         * from the 'pop-up calendar'.
         */
        $values[$fieldname][$i][$colname] = array();
        if ($time == FALSE) {
          $values[$fieldname][$i][$colname]['date'] = $value[$colname];
        }
        else {
          $values[$fieldname][$i][$colname]['date'] = date($widgetFormat, $time);
        }
      }
    }
  }
}

function date_node_import_values_alter_date_popup(&$values, $fields, $fieldname, $fieldinfo) {
  foreach ($fieldinfo['columns'] as $colname => $colinfo) {  
    // Provide default value if the field has no values.
    if (count($values[$fieldname]) == 0) {
      $values[$fieldname] = array();
      $values[$fieldname][0] = array();
      $values[$fieldname][0][$colname] = array(
        'date' => null,
        'time' => null,
      );
    }    
    else {
      foreach ($values[$fieldname] as $i => $value) {
        //Timezone is handled separately - just wrap in an array('timezone' => $timezone)
        if ($colname == 'timezone') {
          $values[$fieldname][$i]['timezone'] = array('timezone' => $values[$fieldname][$i]['timezone']);
          continue;
        }
        // Get the format that is expected from the pop-up calendar.
        // If a custom format was entered, use it instead.
        if ($fieldinfo['widget']['input_format_custom']) {
          $widgetFormat = $fieldinfo['widget']['input_format_custom'];
        }
        else {
          $widgetFormat = $fieldinfo['widget']['input_format'];
        }

        // Remove parts that are not valid for the selected granularity
        $widgetFormat = date_limit_format($widgetFormat, $fieldinfo['granularity']);
        
        // Separate the widget format into a date part and a time part
        $widgetFormatPieces  = explode(' - ', $widgetFormat, 2);
        $dateFormat = $widgetFormatPieces[0];
        $timeFormat = $widgetFormatPieces[1];
  
        // Get the original value format we need to convert
        $time = strtotime($value[$colname]);
        
        /* $colname is equal to 'value' in most cases.
         * We trash the original 'value' field and replace it with an array containing the various pieces
         * entered from the 'pop-up calendar'.
         */
        $values[$fieldname][$i][$colname] = array();
        if ($time == FALSE) {
          $values[$fieldname][$i][$colname]['date'] = '';
        }
        else {
          $values[$fieldname][$i][$colname]['date'] = date($dateFormat, $time);
        }
        
        // AM/PM are case sensitive...
        $values[$fieldname][$i][$colname]['time'] = strtoupper(date($timeFormat, $time));
      }
    }
  }
}

function date_node_import_values_alter_date_select(&$values, $fields, $fieldname, $fieldinfo) {
  foreach ($fieldinfo['columns'] as $colname => $colinfo) {  
    // Provide default value if the field has no values.
    if (count($values[$fieldname]) == 0) {
      $values[$fieldname] = array();
      $values[$fieldname][0] = array();
      $values[$fieldname][0][$colname] = array(
        'year' => null,
        'hour' => 0,
        'minute' => 0,
        'second' => 0,
        'month' => null,
        'day' => null,
      );
    }    
    else {
      $cck_fieldname = node_import_cck_name($fieldname, $colname);
    
      foreach ($values[$fieldname] as $i => $value) {
        // TODO: Take into account granularity!
        $values[$fieldname][$i][$colname] = date_convert($value[$colname], $fields[$cck_fieldname]['output_format'], $fields[$cck_fieldname]['date_format']);   

        // Strangely, hours don't appear to be padded the same way for 12-hour...
        // $values[$fieldname][$i][$colname]['hour']  = str_pad($values[$fieldname][$i][$colname]['hour'], 2, "0", STR_PAD_LEFT);
    
        $values[$fieldname][$i][$colname]['minute']  = str_pad($values[$fieldname][$i][$colname]['minute'], 2, "0", STR_PAD_LEFT);

        $values[$fieldname][$i][$colname]['second']  = str_pad($values[$fieldname][$i][$colname]['second'], 2, "0", STR_PAD_LEFT);
      }
    }
  }
}
