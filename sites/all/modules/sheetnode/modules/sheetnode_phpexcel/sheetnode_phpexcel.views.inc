<?php

/**
 * Implementation of hook_views_plugins().
 */
function sheetnode_phpexcel_views_plugins() {
  return array(
    'style' => array(
      'sheet_phpexcel' => array(
        'title' => t('Spreadsheet file'),
        'help' => t('Download the view as a spreadsheet file.'),
        'handler' => 'sheetnode_phpexcel_plugin_style',
        'parent' => 'sheet',
        'uses row plugin' => FALSE,
        'uses fields' => TRUE,
        'uses options' => TRUE,
        'type' => 'feed',
      ),
    ),
  );
}

