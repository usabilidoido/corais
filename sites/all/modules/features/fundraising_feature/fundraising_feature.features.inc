<?php

/**
 * Implementation of hook_ctools_plugin_api().
 */
function fundraising_feature_ctools_plugin_api() {
  list($module, $api) = func_get_args();
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => 1);
  }
}

/**
 * Implementation of hook_imagecache_default_presets().
 */
function fundraising_feature_imagecache_default_presets() {
  $items = array(
    'donation_menu_item_220x165' => array(
      'presetname' => 'donation_menu_item_220x165',
      'actions' => array(
        '0' => array(
          'weight' => '0',
          'module' => 'imagecache',
          'action' => 'imagecache_scale_and_crop',
          'data' => array(
            'width' => '220',
            'height' => '165',
          ),
        ),
      ),
    ),
  );
  return $items;
}

/**
 * Implementation of hook_node_info().
 */
function fundraising_feature_node_info() {
  $items = array(
    'pg_fundraising' => array(
      'name' => t('Fundraising'),
      'module' => 'features',
      'description' => t('By default, this content type is used by Fundraising module. Use this content type to set up donation pages on your website. In order to accept donations using this content type, Fundraising module and modules which it is dependent on need to be enabled and configured accordingly.'),
      'has_title' => '1',
      'title_label' => t('Título'),
      'has_body' => '1',
      'body_label' => t('Descrição'),
      'min_word_count' => '0',
      'help' => '',
    ),
  );
  return $items;
}
