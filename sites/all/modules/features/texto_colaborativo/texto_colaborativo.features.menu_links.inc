<?php

/**
 * Implementation of hook_menu_default_menu_links().
 */
function texto_colaborativo_menu_default_menu_links() {
  $menu_links = array();

  // Exported menu link: navigation:node/add/text
  $menu_links['navigation:node/add/text'] = array(
    'menu_name' => 'navigation',
    'link_path' => 'node/add/text',
    'router_path' => 'node/add/text',
    'link_title' => 'Texto Colaborativo',
    'options' => array(
      'attributes' => array(
        'title' => 'Texto que pode ser escrito por várias pessoas ao mesmo tempo utilizando diferentes computadores. O Texto Colaborativo possui um chat no lado para facilitar a colaboração.',
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
  // Translatables
  // Included for use with string extractors like potx.
  t('Texto Colaborativo');


  return $menu_links;
}
