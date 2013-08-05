<?php

/**
 * @file
 * Support file for the CCK Embedded Video module.
 */

/**
 * Implementation of hook_node_import_fields().
 */
function emvideo_node_import_fields($type) {
  $fields = array();

  foreach (node_import_cck_fields($type, 'emvideo') as $fieldname => $fieldinfo) {
    foreach ($fieldinfo['columns'] as $colname => $colinfo) {
      $cck_fieldname = node_import_cck_name($fieldname, $colname);

      $fields[$cck_fieldname] = node_import_cck_field_defaults($fieldinfo);
      switch ($colname) {
        case 'embed':
          $fields[$cck_fieldname]['title'] = $fieldinfo['widget']['label'];
          break;

        default:
          // as in the node editing form, we only set the embed field, and the
          // rest happens automatically
          unset($fields[$cck_fieldname]);
          break;
      }
    }
  }

  return $fields;
}
