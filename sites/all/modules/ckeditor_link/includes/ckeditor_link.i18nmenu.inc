<?php
/**
 * @file
 * Written by Henri MEDOT <henri.medot[AT]absyx[DOT]fr>
 * http://www.absyx.fr
 */

/**
 * Implementation of hook_ckeditor_link_TYPE_autocomplete().
 */
function ckeditor_link_ckeditor_link_i18nmenu_autocomplete($string, $limit) {
  // Currently, this function only supports MySQL.
  // TODO: Add support for pgsql.
  if (!in_array($GLOBALS['db_type'], array('mysql', 'mysqli'))) {
    return array();
  }

  $matches = array();

  $menus = array_keys(array_filter(variable_get('ckeditor_link_autocomplete_menus', array())));
  if (count($menus)) {
    $sql = "SELECT ml.link_path, CONVERT(lt.translation USING utf8) link_title, lt.language FROM {menu_links} ml
      INNER JOIN {locales_source} ls ON ls.location = CONCAT('item:', ml.mlid, ':title')
      INNER JOIN {locales_target} lt ON lt.lid = ls.lid
      WHERE CONVERT(lt.translation USING utf8) LIKE '%%%s%%' AND ml.hidden = 0 AND ml.external = 0";
    $args = array($string);
    if (!in_array('- any -', $menus)) {
      $sql .= ' AND ml.menu_name IN ('. db_placeholders($menus, 'text') .')';
      $args = array_merge($args, $menus);
    }
    $sql .= ' ORDER BY link_title';
    $result = db_query_range($sql, $args, 0, $limit);
    while ($item = db_fetch_object($result)) {
      if (_ckeditor_link_check_path($item->link_path)) {
        $router_item = menu_get_item($item->link_path);
        if ($router_item && $router_item['access']) {
          $path = ckeditor_link_path_prefix_language($item->link_path, $item->language);
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
function ckeditor_link_ckeditor_link_i18nmenu_revert($path, &$langcode) {
  $router_item = menu_get_item($path);
  if ($router_item) {
    if (!$router_item['access']) {
      return FALSE;
    }
    $result = db_query("SELECT mlid, link_title, options FROM {menu_links} WHERE link_path = '%s' AND hidden = 0 ORDER BY customized DESC", $path);
    $default_langcode = language_default('language');
    $link_title = NULL;
    while ($item = db_fetch_object($result)) {
      $options = unserialize($item->options);
      $item_langcode = (isset($options['langcode'])) ? $options['langcode'] : '';
      if ($item_langcode == $langcode) {
        $link_title = $item->link_title;
        break;
      }
      elseif (($item_langcode == $default_langcode) && empty($langcode)) {
        $langcode = $default_langcode;
        $link_title = $item->link_title;
        break;
      }
      elseif (!$link_title && empty($item_langcode)) {
        $link_title = i18nstrings('menu:item:'. $item->mlid .':title', $item->link_title, $langcode);
      }
    }
    return ($link_title) ? $link_title : NULL;
  }
}
