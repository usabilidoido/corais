<?php

/**
 * @file
 * Support file for optionwidgets module.
 */

/**
 * Implementation of hook_node_import_values_alter().
 */
function optionwidgets_node_import_values_alter(&$values, $type, $defaults, $options, $fields, $preview) {
  //TODO: is this all that is needed? Is the column always called 'value'? NEEDS MORE TESTING.
  foreach ($fields as $fieldname => $fieldinfo) {
    if (isset($fieldinfo['cck:fieldname'])) {
      if ($fieldinfo['cck:fieldinfo']['widget']['type'] == 'optionwidgets_buttons') {
        // Instead of
        //
        //   fieldname => array(
        //     0 => array(
        //       value => 3,
        //     ),
        //     1 => array(
        //       value => 4,
        //     ),
        //   ),
        //
        // we need:
        //
        //   fieldname => array(
        //     value => array(
        //       3 => 3,
        //       4 => 4,
        //     ),
        //   ),
        //
        $new_value = array();
        foreach ($values[$fieldinfo['cck:fieldname']] as $key => $value) {
          $new_value[(string)$value['value']] = (string)$value['value'];
        }
        $values[$fieldinfo['cck:fieldname']] = array(
          'value' => $new_value,
        );
      }
    }
  }
}

