<?php

/**
 * Implementation of hook_menu_default_menu_links().
 */
function atrium_time_tracker_menu_default_menu_links() {
  $menu_links = array();

  // Exported menu link: features:time_sheet/%
  $menu_links['features:time_sheet/%'] = array(
    'menu_name' => 'features',
    'link_path' => 'time_sheet/%',
    'router_path' => 'time_sheet/%',
    'link_title' => 'Time Sheet',
    'options' => array(
      'attributes' => array(
        'class' => 'time_sheet_link',
      ),
    ),
    'module' => 'system',
    'hidden' => '0',
    'external' => '0',
    'has_children' => '0',
    'expanded' => '0',
    'weight' => '0',
  );
  // Translatables
  // Included for use with string extractors like potx.
  t('Time Sheet');


  return $menu_links;
}
