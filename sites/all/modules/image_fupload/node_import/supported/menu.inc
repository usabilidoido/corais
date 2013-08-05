<?php
// $Id: menu.inc,v 1.1.2.8 2009/01/15 20:05:52 robrechtj Exp $

/**
 * @file
 * Support file for the core menu module.
 */

/**
 * Implementation of hook_node_import_fields().
 */
function menu_node_import_fields($type) {
  $fields = array();
  return $fields; //TODO: reenable

  if (($node_type = node_import_type_is_node($type)) !== FALSE) {
    $fields['menu:link_title'] = array(
      'title' => t('Menu link title'),
      'group' => t('Menu settings'),
      'module' => 'menu',
      'is_mappable' => user_access('administer menu'),
    );

    $fields['menu:parent'] = array(
      'title' => t('Parent item'),
      'group' => t('Menu settings'),
      'module' => 'menu',
      'is_mappable' => user_access('administer menu'),
    );

    $fields['menu:weight'] = array(
      'title' => t('Weight'),
      'group' => t('Menu settings'),
      'module' => 'menu',
      'input_format' => 'weight',
      'delta' => 50,
      'is_mappable' => user_access('administer menu'),
    );
  }

  return $fields;
}

/**
 * Implementation of hook_node_import_defaults().
 */
function menu_node_import_defaults($type, $defaults, $fields, $map) {
  $form = array();
  return $form; //TODO: reenable

  if (($node_type = node_import_type_is_node($type)) !== FALSE) {
    if (user_access('administer menu')) {
      $menu_options = menu_parent_options(menu_get_menus(), array('mlid' => 0));

      $form['menu:parent'] = array(
        '#title' => t('Parent'),
        '#type' => 'select',
        '#options' => $menu_options,
        '#default_value' => isset($defaults['menu:parent']) ? $defaults['menu:parent'] : 'primary-links:0',
      );

      $form['menu:weight'] = array(
        '#title' => t('Weight'),
        '#type' => 'weight',
        '#delta' => 50,
        '#default_value' => isset($default['menu:weight']) ? $defaults['menu:weight'] : 0,
      );
    }
  }

  return $form;
}

