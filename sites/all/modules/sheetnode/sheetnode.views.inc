<?php

/**
 * Implementation of hook_views_plugins().
 */
function sheetnode_views_plugins() {
  $path = drupal_get_path('module', 'sheetnode');
  return array(
    'style' => array(
      'sheet' => array(
        'title' => t('Spreadsheet'),
        'help' => t('Displays results in a browser spreadsheet.'),
        'handler' => 'sheetnode_plugin_style',
        'uses row plugin' => FALSE,
        'uses fields' => TRUE,
        'uses options' => TRUE,
        'type' => 'normal',
        'theme' => 'views_view_sheet',
        'path' => $path . '/views',
        'js' => array(
          $path . '/socialcalc/socialcalcconstants.js',
          $path . '/socialcalc/socialcalc-3.js',
          $path . '/socialcalc/socialcalctableeditor.js',
          $path . '/socialcalc/formatnumber2.js',
          $path . '/socialcalc/formula1.js',
          $path . '/socialcalc/socialcalcpopup.js',
          $path . '/socialcalc/socialcalcspreadsheetcontrol.js',
          $path . '/socialcalc/socialcalcviewer.js',
          $path . '/sheetnode.js',
        ),
      ),
      'sheet_raw' => array(
        'title' => t('SocialCalc format'),
        'help' => t('Render the view as a raw SocialCalc spreadsheet.'),
        'handler' => 'sheetnode_raw_plugin_style',
        'parent' => 'sheet',
        'uses row plugin' => FALSE,
        'uses fields' => TRUE,
        'uses options' => TRUE,
        'type' => 'feed',
        'path' => $path . '/views',
      ),
    ),
  );
}

/**
 * Implementation of hook_views_handlers().
 */
function sheetnode_views_handlers() {
  return array(
    'info' => array(
      'path' => drupal_get_path('module', 'sheetnode') . '/views',
    ),
    'handlers' => array(
      'sheetnode_handler_field_cell' => array(
        'parent' => 'views_handler_field',
      ),
      'sheetnode_handler_field_range' => array(
        'parent' => 'views_handler_field',
      ),
      'sheetnode_handler_field_date' => array(
        'parent' => 'views_handler_field',
      ),
      'sheetnode_handler_field_named_range' => array(
        'parent' => 'views_handler_field',
      ),
    ),
  );
}

/**
 * Implementation of hook_views_data().
 */
function sheetnode_views_data() {
  // Basic table information.
  $data['sheetnode']['table']['group']  = t('Sheetnode');

  // Explain how we join.
  $data['sheetnode']['table']['join'] = array(
    // Directly links to node table.
    'node' => array(
      'left_field' => 'vid',
      'field' => 'vid',
    ),
  );

  // Sheetnode field.
  $data['sheetnode']['content'] = array(
    'title' => t('Content'),
    'help' => t('The raw content of the sheetnode.'),
    'field' => array(
      'field' => 'value',
      'handler' => 'views_handler_field',
    ),
  );

  // Cell field.
  $data['sheetnode']['cell'] = array(
    'title' => t('Cell'),
    'help' => t('The value stored in a specific sheetnode cell.'),
    'field' => array(
      'field' => 'value',
      'handler' => 'sheetnode_handler_field_cell',
    ),
  );

  // Range field.
  $data['sheetnode']['range'] = array(
    'title' => t('Range'),
    'help' => t('The values stored in a range of cells, including whole columns or whole rows.'),
    'field' => array(
      'field' => 'value',
      'handler' => 'sheetnode_handler_field_range',
    ),
  );

  // Variable name.
  $data['sheetnode']['named_range'] = array(
    'title' => t('Named range'),
    'help' => t('The cell or range values referred to by a named variable.'),
    'field' => array(
      'field' => 'value',
      'handler' => 'sheetnode_handler_field_named_range',
    ),
  );

  return $data;
}

/**
 * Implementation of hook_views_data_alter().
 */
function sheetnode_views_data_alter(&$data) {
  $cck_info = _content_type_info();
  foreach ($data as $table_name => $table_info) {
    foreach ($table_info as $field_name => $field_info) {
      // Add a SocialCalc date converter for each date field.
      if (!empty($field_info['field']['handler']) && $field_info['field']['handler'] == 'views_handler_field_date') {
        $date = $field_info;
        $date['real field'] = $field_name;
        $date['title'] = $field_info['title'] . ' (SocialCalc)';
        $date['field']['handler'] = 'sheetnode_handler_field_date';
        unset($date['filter'], $date['argument'], $date['relationship'], $date['sort']);
        $data[$table_name][$field_name . '_socialcalc'] = $date;
      }

      // Add sheetnode handlers for each sheet field.
      if (!empty($field_info['field']['content_field_name']) && $cck_info['fields'][ $field_info['field']['content_field_name'] ]['type'] == 'sheetfield') {
        $field = $cck_info['fields'][ $field_info['field']['content_field_name'] ];
        $label_truncated = truncate_utf8(t($field['widget']['label']), 10, TRUE);
        $title = t('@label (!name)', array('@label' => t($field['widget']['label']), '!name' => $field['field_name']));
        $title_short = check_plain($label_truncated);
        $types = array();
        foreach ($cck_info['content types'] as $type) {
          if (isset($type['fields'][$field['field_name']])) {
            $types[] = $type['name'];
          }
        }

        $cell = $field_info;
        $cell['real field'] = $field_name;
        $cell['title'] = $title . ' - ' . t('cell');
        $cell['title short'] = $title_short . ' - ' . t('cell');
        $cell['help'] = t('Cell reference - Appears in: @types', array('@types' => implode(', ', $types)));
        $cell['field']['handler'] = 'sheetnode_handler_field_cell';
        unset($cell['filter'], $cell['argument'], $cell['relationship'], $cell['sort'], $cell['field']['title'], $cell['field']['title short']);
        $data[$table_name][$field_name . '_cell'] = $cell;

        $range = $cell;
        $range['title'] = $title . ' - ' . t('range');
        $range['title short'] = $title_short . ' - ' . t('range');
        $range['help'] = t('Range reference - Appears in: @types', array('@types' => implode(', ', $types)));
        $range['field']['handler'] = 'sheetnode_handler_field_range';
        $data[$table_name][$field_name . '_range'] = $range;

        $named_range = $cell;
        $named_range['title'] = $title . ' - ' . t('named range');
        $named_range['title short'] = $title_short . ' - ' . t('named range');
        $named_range['help'] = t('Named range - Appears in: @types', array('@types' => implode(', ', $types)));
        $named_range['field']['handler'] = 'sheetnode_handler_field_named_range';
        $data[$table_name][$field_name . '_named_range'] = $named_range;
      }
    }
  }
}

