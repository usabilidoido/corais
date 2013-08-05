<?php

/**
 * Implementation of hook_menu_default_menu_links().
 */
function atrium_credit_menu_default_menu_links() {
  $menu_links = array();

  // Exported menu link: features:exchanges
  $menu_links['features:exchanges'] = array(
    'menu_name' => 'features',
    'link_path' => 'exchanges',
    'router_path' => 'exchanges',
    'link_title' => 'Mutual Credit',
    'options' => array(
      'attributes' => array(
        'title' => 'Member-to-member exchanges',
      ),
    ),
    'module' => 'system',
    'hidden' => '0',
    'external' => '0',
    'has_children' => '0',
    'expanded' => '0',
    'weight' => '20',
  );
  // Exported menu link: navigation:exchange
  $menu_links['navigation:exchange'] = array(
    'menu_name' => 'navigation',
    'link_path' => 'exchange',
    'router_path' => 'exchange',
    'link_title' => 'Record exchange',
    'options' => array(),
    'module' => 'system',
    'hidden' => '0',
    'external' => '0',
    'has_children' => '0',
    'expanded' => '0',
    'weight' => '-21',
    'parent_path' => 'node/add',
  );
  // Exported menu link: navigation:node/add/currency
  $menu_links['navigation:node/add/currency'] = array(
    'menu_name' => 'navigation',
    'link_path' => 'node/add/currency',
    'router_path' => 'node/add/currency',
    'link_title' => 'Currency',
    'options' => array(
      'attributes' => array(
        'title' => 'A unit of value',
      ),
    ),
    'module' => 'system',
    'hidden' => '0',
    'external' => '0',
    'has_children' => '0',
    'expanded' => '0',
    'weight' => '-28',
  );
  // Translatables
  // Included for use with string extractors like potx.
  t('Currency');
  t('Mutual Credit');
  t('Record exchange');


  return $menu_links;
}
