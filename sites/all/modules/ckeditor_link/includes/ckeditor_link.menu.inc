<?php
/**
 * @file
 * Written by Henri MEDOT <henri.medot[AT]absyx[DOT]fr>
 * http://www.absyx.fr
 */

/**
 * Implementation of hook_ckeditor_link_TYPE_autocomplete().
 */
function ckeditor_link_ckeditor_link_menu_autocomplete($string, $limit) {
  $matches = array();

  $menus = array_keys(array_filter(variable_get('ckeditor_link_autocomplete_menus', array())));
  if (count($menus)) {
    $sql = "SELECT link_path, link_title, options FROM {menu_links}
      WHERE link_title LIKE '%%%s%%' AND hidden = 0 AND external = 0";
    $args = array($string);
    if (!in_array('- any -', $menus)) {
      $sql .= ' AND menu_name IN ('. db_placeholders($menus, 'text') .')';
      $args = array_merge($args, $menus);
    }
    $sql .= ' ORDER BY link_title';
    $result = db_query_range($sql, $args, 0, $limit);
    while ($item = db_fetch_object($result)) {
      if (_ckeditor_link_check_path($item->link_path)) {
        $router_item = menu_get_item($item->link_path);
        if ($router_item && $router_item['access']) {
          $options = unserialize($item->options);
          $langcode = (isset($options['langcode'])) ? $options['langcode'] : '';
          $path = ckeditor_link_path_prefix_language($item->link_path, $langcode);
          $matches[$path] = $item->link_title;
        }
      }
    }
  }

  return $matches;
}

/**
 * Implementation of hook_ckeditor_link_TYPE_revert().
 */
function ckeditor_link_ckeditor_link_menu_revert($path, &$langcode) {
  if (function_exists('ckeditor_link_ckeditor_link_i18nmenu_revert')) {
    return;
  }

  $router_item = menu_get_item($path);
  if ($router_item) {
    if (!$router_item['access']) {
      return FALSE;
    }
    $link_title = db_result(db_query("SELECT link_title FROM {menu_links} WHERE link_path = '%s' AND hidden = 0 ORDER BY customized DESC", $path));
    return ($link_title) ? $link_title : NULL;
  }
}

/**
 * Implementation of hook_ckeditor_link_TYPE_settings().
 */
function ckeditor_link_ckeditor_link_menu_settings() {
  $form = array();

  if (module_exists('menu')) {
    $form['menu'] = array(
      '#type' => 'fieldset',
      '#title' => t('Menu items'),
    );
    $form['menu']['ckeditor_link_autocomplete_menus'] = array(
      '#type' => 'checkboxes',
      '#title' => t('Menus'),
      '#options' => array('- any -' => t('<em>Any menu</em>')) + array_map('check_plain', menu_get_menus()),
      '#default_value' => variable_get('ckeditor_link_autocomplete_menus', array()),
      '#description' => t('Select the menus to be available as autocomplete suggestions.'),
    );
  }

  return $form;
}
