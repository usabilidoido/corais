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

  if (($node_type = node_import_type_is_node($type)) !== FALSE) {
    $fields['menu:link_title'] = array(
      'title' => t('Menu link title'),
      'group' => t('Menu settings'),
      'module' => 'menu',
      'is_mappable' => user_access('administer menu'),
    );
    $fields['menu:path'] = array(
      'title' => t('Alias for node which should be used as parent menu'),
      'group' => t('Menu settings'),
      'module' => 'menu',
      'is_mappable' => user_access('administer menu'),
    );
    $fields['menu:parent'] = array(
      'title' => t('Parent Menu'),
      'group' => t('Menu settings'),
      'module' => 'menu',
      'is_mappable' => FALSE, #set this in defaults
    );
  }

  return $fields;
}

/**
 * Implementation of hook_node_import_defaults().
 * 
 * Global defaults for menus would be confusing. Select only the parent menu.
 */
function menu_node_import_defaults($type, $defaults, $fields, $map) {
  $form = array();

  // If we are importing nodes and user can administer menus... 
  if ((($node_type = node_import_type_is_node($type)) !== FALSE) && (user_access('administer menu'))) {
    // Which main menu will these items be imported under.
    $result = db_query("SELECT * FROM {menu_custom} ORDER BY title");
    $menu_options = array();
    while ($menu = db_fetch_array($result)) {
      $menu_options[$menu['menu_name']] = $menu['menu_name'];
    }

    $menu_options = menu_parent_options(menu_get_menus(), array('mlid' => 0));

    $form['menu:parent'] = array(
      '#title' => t('Parent Menu'),
      '#type' => 'select',
      '#options' => $menu_options,
      '#default_value' => isset($defaults['menu:parent']) ? $defaults['menu:parent'] : 'primary-links:0',
    );
  }
  return $form;
}


/**
 * Implementation of hook_node_import_values_alter().
 * 
 * Look up the menu item using the provided path alias for the node's chosen
 * menu parent.
 * 
 * Here we restructure the values back from the menu: format back into an array
 * for saving.
 */
function menu_node_import_values_alter(&$values, $type, $defaults, $options, $fields, $preview) {
  static $first_run = TRUE;

  // If we are importing nodes and user can administer menus... 
  if ((($node_type = node_import_type_is_node($type)) !== FALSE)  && (user_access('administer menu'))) {
    if ($first_run) {
      $menu_weight = -50;
      $first_run = FALSE;
    }
    else {
      $menu_weight = variable_get('node_import_menu_weight', -51);
      $menu_weight++;
      
      // If the menu weight gets above the max, roll it over...
      if ($menu_weight > 50) {
        $menu_weight = -50;
      }
    }
    variable_set('node_import_menu_weight', $menu_weight);

    // If the user is trying to configure menu path...
    if (isset($values['menu:path']) && drupal_strlen($values['menu:path']) > 0) {
      // dpm("Set menu based on path '".$values['menu:path']."' under parent: '".$values['menu:parent']."'");

      // Get the menu id using the provided parent node alias
      if (($parent = node_import_get_object('menu:path', $values['menu:path'])) || ($parent = menu_node_import_get_parent_from_path($values['menu:path']) ))  {
         $values['menu:parent'] = $parent;
      }
      else {
         // otherwise drop it into the configured menu root.
         drupal_set_message("Failed to find an existing menu parent of ". $values['menu:path'] . " - just dropping it in the root of the menu.");
      }
    
      // Caching the parent will avoid looking it up the next time
      node_import_set_object('menu:path', $values['menu:parent'], $values['menu:path']);

      // Every menu needs a title, even if one wasn't provided
      $values['menu:link_title'] = isset($values['menu:link_title']) ? $values['menu:link_title'] : $values['title'];
    
      // dpm("Parent of '".$values['title']."' will be '".$values['menu:parent']."'");

      $values['menu'] = array(
        'parent' => $values['menu:parent'],
        'weight' => $menu_weight,
        'link_title' => $values['menu:link_title'],
      );
    }
    else if (isset($values['menu:link_title']) && drupal_strlen($values['menu:link_title']) > 0) {
      // If user configured menu link title, just drop it in the root of the menu
      //dpm("Parent of '".$values['menu:link_title']."' will be '".$values['menu:parent']."'");

      $values['menu'] = array(
        'parent' => $values['menu:parent'],
        'weight' => $menu_weight,
        'link_title' => $values['menu:link_title'],
      );
    }    
  }
}

/**
 * Given an url alias, look to see if a menu item already exists
 */
function menu_node_import_get_parent_from_path($parent_path) {
  if (! $parent_path) return NULL;
  
  // look up appropriate menu item identifier
  drupal_lookup_path('wipe'); // batch jobbing this needs to flush the cache
  if ($parent_system_path = drupal_get_normal_path($parent_path)) {

    // menu doesn't provide a get-menu item by name. Do it myself. This also helps avoid menu caching issues.
    $result = db_query("SELECT mlid,plid,menu_name FROM {menu_links} WHERE link_path = '%s' LIMIT 1", $parent_system_path);
    while ($menu_item = db_fetch_array($result)) {
      $parent = $menu_item['menu_name'] .':'. $menu_item['mlid'];
    }

    // However later processes (node_save validate) are not up to date with the just-created menu items
    // and don't believe the parent exists :-(
    // Do I really have to:
    // menu_rebuild(); // each time?
    // This slows everything down HEAPS
    // so really should check if it's needed first
    menu_rebuild(); 
    
    // menu.module appears to provide an out? - beware - this setting messes up menu selector functions
    #variable_set('menu_override_parent_selector', TRUE);

    return $parent;
  }
}