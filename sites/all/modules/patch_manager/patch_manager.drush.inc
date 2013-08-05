<?php


/**
 * Implementation of hook_drush_command().
 */
function patch_manager_drush_command() {
  $items = array();

  $items['patch-list'] = array(
    'callback' => 'patch_manager_command_list',
    'description' => 'List all patches that patch manager knows about.',
    'arguments' => array(),
    'aliases' => array(),
    'options' => array(),
    'examples' => array(),
  );
  
  return $items;
}


/**
 * Implementation of hook_drush_help().
 */
function patch_manager_drush_help($section) {
  switch ($section) {
    case 'drush:pm-list':
      return dt('List all patches that patch manager knows about.');
  }
}


/**
 * Patch manager command: list patches
 */
function patch_manager_command_list() {
  $res = db_query('SELECT nid, title FROM {node} WHERE type="patch"');
  
  $rows = array();
  while ($node = db_fetch_object($res)) {
    $rows[] = array(
      $node->nid,
      $node->title);
  }
  
  $header = array(
    t('#'),
    t('Title')
  );
  
  $tbl = new Console_Table(CONSOLE_TABLE_ALIGN_LEFT , '');    
  $out = $tbl->fromArray($header, $rows);
  
  drush_print($out);
}
