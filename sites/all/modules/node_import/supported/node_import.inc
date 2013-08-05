<?php

/**
 * @file
 * Support file for node_import module.
 */

/**
 * Implementation of hook_node_import_values().
 */
function node_import_node_import_values($type, $defaults, $options, $fields, $preview) {
  return array(
    'node_import_build_mode' => $preview ? NODE_BUILD_PREVIEW : NODE_BUILD_NORMAL,
  );
}

/**
 * Implementation of hook_node_import_format_options().
 */
function node_import_node_import_format_options($op) {
  switch ($op) {
    case 'date input formats':
      // Copy from modules/system/system.admin.inc (admin/settings/date-time).
      $date_short = array(
        'Y-m-d H:i', 'm/d/Y - H:i', 'd/m/Y - H:i', 'Y/m/d - H:i',
         'd.m.Y - H:i', 'm/d/Y - g:ia', 'd/m/Y - g:ia', 'Y/m/d - g:ia',
         'M j Y - H:i', 'j M Y - H:i', 'Y M j - H:i',
         'M j Y - g:ia', 'j M Y - g:ia', 'Y M j - g:ia');
      $date_formats = array();
      foreach ($date_short as $f) {
        $date_formats[$f] = format_date(time(), 'custom', $f);
      }
      return $date_formats;

    case 'record separators':
      return array(
        '<newline>' => t('Newline'),
      );

    case 'field separators':
      return array(
        ':' => t('Colon (:)'),
        ',' => t('Comma (,)'),
        '|' => t('Pipe (|)'),
        ';' => t('Semicolon (;)'),
        ' ' => t('Space ( )'),
        '<tab>' => t('Tab'),
      );

    case 'text delimiters':
      return array(
        '<none>' => t('None'),
        '"' => t('Double quote (")'),
        "'" => t("Single quote (')"),
      );

    case 'escape characters':
      return array(
        '<none>' => t('None'),
        '\\' => t('Backslash (\\)'),
        '"' => t('Double quote (")'),
        "'" => t("Single quote (')"),
      );

    case 'file formats':
      return array(
        'csv' => array(
          'title' => t('Comma Separated Values'),
          'record separator' => '<newline>',
          'field separator' => ',',
          'text delimiter' => '"',
          'escape character' => '"',
          'mime' => 'text/comma-separated-values',
        ),
        'csv-excel' => array(
          'title' => t('Semicolon Separated Values'),
          'record separator' => '<newline>',
          'field separator' => ';',
          'text delimiter' => '"',
          'escape character' => '"',
        ),
        'tsv' => array(
          'title' => t('Tab Separated Values'),
          'record separator' => '<newline>',
          'field separator' => '<tab>',
          'text delimiter' => '<none>',
          'escape character' => '<none>',
          'mime' => 'text/tab-separated-values',
        ),
      );
  }

  return array();
}

