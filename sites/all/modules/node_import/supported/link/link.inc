<?php

/**
 * @file
 * Support file for the CCK Link module.
 */

/**
 * Implementation of hook_node_import_fields().
 */
function link_node_import_fields($type) {
  $fields = array();

  foreach (node_import_cck_fields($type, 'link') as $fieldname => $fieldinfo) {
    foreach ($fieldinfo['columns'] as $colname => $colinfo) {
      $cck_fieldname = node_import_cck_name($fieldname, $colname);

      switch ($colname) {
        case 'title':
          if ($fieldinfo['title'] !== 'none' && $fieldinfo['title'] !== 'value') {
            $fields[$cck_fieldname] = node_import_cck_field_defaults($fieldinfo);
            $fields[$cck_fieldname]['title'] = $fieldinfo['widget']['label'] .' - '. t('Title');
            $fields[$cck_fieldname]['map_required'] = ($fieldinfo['title'] === 'required' ? $fieldinfo['required'] : FALSE); //TODO
          }
          break;

        case 'url':
          $fields[$cck_fieldname] = node_import_cck_field_defaults($fieldinfo);
          $fields[$cck_fieldname]['title'] = $fieldinfo['widget']['label'] .' - '. t('URL');
          $fields[$cck_fieldname]['map_required'] = ($fieldinfo['url'] === 'optional' ? $fieldinfo['required'] : FALSE); //TODO
          break;

        case 'attributes':
          // We don't allow users to import attributes (as it is not shown
          // on the edit form either).
          $fields[$cck_fieldname] = node_import_cck_field_defaults($fieldinfo);
          $fields[$cck_fieldname]['is_mappable'] = FALSE;
          //TODO: users can set the target attribute if so configured.
          break;

        default:
          $fields[$cck_fieldname] = node_import_cck_field_defaults($fieldinfo);
          $fields[$cck_fieldname]['title'] = t('Unsupported: ') . $fieldinfo['widget']['label'] .' - '. $colname;
          break;
      }
    }
  }

  if (count($fields) <= 1) {
    foreach ($fields as $fieldname => $fieldinfo) {
      unset($fields[$fieldname]['group']);
    }
  }

  return $fields;
}

