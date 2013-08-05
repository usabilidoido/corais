<?php

/**
 * Implementation of hook_menu_default_menu_links().
 */
function spreadsheets_menu_default_menu_links() {
  $menu_links = array();

  // Exported menu link: admin:admin/settings/sheetnode
  $menu_links['admin:admin/settings/sheetnode'] = array(
    'menu_name' => 'admin',
    'link_path' => 'admin/settings/sheetnode',
    'router_path' => 'admin/settings/sheetnode',
    'link_title' => 'Sheetnode',
    'options' => array(
      'attributes' => array(
        'title' => 'Sheetnode module settings',
      ),
    ),
    'module' => 'system',
    'hidden' => '0',
    'external' => '0',
    'has_children' => '1',
    'expanded' => '0',
    'weight' => '0',
    'parent_path' => 'admin/settings',
  );
  // Exported menu link: admin:admin/settings/sheetnode/phpexcel
  $menu_links['admin:admin/settings/sheetnode/phpexcel'] = array(
    'menu_name' => 'admin',
    'link_path' => 'admin/settings/sheetnode/phpexcel',
    'router_path' => 'admin/settings/sheetnode/phpexcel',
    'link_title' => 'Sheetnode PHPExcel',
    'options' => array(
      'attributes' => array(
        'title' => 'Administer settings for Sheetnode PHPExcel.',
      ),
    ),
    'module' => 'system',
    'hidden' => '0',
    'external' => '0',
    'has_children' => '0',
    'expanded' => '0',
    'weight' => '0',
    'parent_path' => 'admin/settings/sheetnode',
  );
  // Exported menu link: features:spreadsheets
  $menu_links['features:spreadsheets'] = array(
    'menu_name' => 'features',
    'link_path' => 'spreadsheets',
    'router_path' => 'spreadsheets',
    'link_title' => 'Spreadsheets',
    'options' => array(),
    'module' => 'system',
    'hidden' => '0',
    'external' => '0',
    'has_children' => '0',
    'expanded' => '0',
    'weight' => '0',
  );
  // Exported menu link: navigation:node/add/ods
  $menu_links['navigation:node/add/ods'] = array(
    'menu_name' => 'navigation',
    'link_path' => 'node/add/ods',
    'router_path' => 'node/add/ods',
    'link_title' => 'Sheetnode import from !format',
    'options' => array(
      'attributes' => array(
        'title' => 'Create a new sheetnode with content from an existing spreadsheet.',
      ),
    ),
    'module' => 'system',
    'hidden' => '1',
    'external' => '0',
    'has_children' => '0',
    'expanded' => '0',
    'weight' => '0',
    'parent_path' => 'node/add',
  );
  // Exported menu link: navigation:node/add/sheetnode
  $menu_links['navigation:node/add/sheetnode'] = array(
    'menu_name' => 'navigation',
    'link_path' => 'node/add/sheetnode',
    'router_path' => 'node/add/sheetnode',
    'link_title' => 'Planilha',
    'options' => array(
      'attributes' => array(
        'title' => 'A spreadsheet node.',
      ),
    ),
    'module' => 'system',
    'hidden' => '0',
    'external' => '0',
    'has_children' => '0',
    'expanded' => '0',
    'weight' => '0',
    'parent_path' => 'node/add',
  );
  // Exported menu link: navigation:node/add/xls
  $menu_links['navigation:node/add/xls'] = array(
    'menu_name' => 'navigation',
    'link_path' => 'node/add/xls',
    'router_path' => 'node/add/xls',
    'link_title' => 'Sheetnode import from !format',
    'options' => array(
      'attributes' => array(
        'title' => 'Create a new sheetnode with content from an existing spreadsheet.',
      ),
    ),
    'module' => 'system',
    'hidden' => '1',
    'external' => '0',
    'has_children' => '0',
    'expanded' => '0',
    'weight' => '0',
    'parent_path' => 'node/add',
  );
  // Exported menu link: navigation:node/add/xlsx
  $menu_links['navigation:node/add/xlsx'] = array(
    'menu_name' => 'navigation',
    'link_path' => 'node/add/xlsx',
    'router_path' => 'node/add/xlsx',
    'link_title' => 'Sheetnode import from !format',
    'options' => array(
      'attributes' => array(
        'title' => 'Create a new sheetnode with content from an existing spreadsheet.',
      ),
    ),
    'module' => 'system',
    'hidden' => '1',
    'external' => '0',
    'has_children' => '0',
    'expanded' => '0',
    'weight' => '0',
    'parent_path' => 'node/add',
  );
  // Translatables
  // Included for use with string extractors like potx.
  t('Planilha');
  t('Sheetnode');
  t('Sheetnode PHPExcel');
  t('Sheetnode import from !format');
  t('Spreadsheets');


  return $menu_links;
}
