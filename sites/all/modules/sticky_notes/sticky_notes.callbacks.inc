<?php

/**
 * @file
 * Ajax callbacks for the sticky_notes module.
 */

function sticky_notes_info_box_js() {

  if (isset($_REQUEST['path'])) {
    $path = $_REQUEST['path'];
  }
  
  if (!isset($path)) {
    $menu_item = menu_get_item();
    $path = $menu_item['href'];
  }

  // prepare add link if the user has the permission
  if (user_access('create sticky notes') || user_access('administer sticky notes')) {
    $link = l(t('+ Add note'), 'sticky-notes/add');
  }

  // prepare some info box properties
  if (sticky_notes_check_interface_access()) {

    $classes = variable_get('sticky_notes_info_box_corner', STICKY_NOTES_INFO_BOX_CORNER_BOTTOM_RIGHT);
    if (variable_get('sticky_notes_info_box_hide', STICKY_NOTES_DEFAULT_INFO_BOX_HIDE)) {
      $classes .= ' hide-info-box';
    }

    if (user_access('view sticky notes overview')) {
      $menu_item = menu_get_item();
      $overview_link = module_exists('views') && $menu_item['href'] != 'sticky-notes/list' ? l(t('Sticky Notes overview'), 'sticky-notes/list') : NULL;
    }
    
    drupal_json(theme(
      'sticky_notes_info_box',
      $classes,
      $link,
      isset($overview_link) ? $overview_link : NULL,
      variable_get('sticky_notes_toggle_visibility_state', STICKY_NOTES_DEFAULT_TOGGLE_VISIBILITY_STATE)
    ));
    return;
  }
  else{
    drupal_json();
    return;
  }

}

/**
 * Ajax callback: Load notes for the current page
 *
 * @return void
 * @author berliner
 */

function sticky_notes_load_js() {
  global $user;
  $menu_item = menu_get_item();
  $pattern = !isset($_REQUEST['pattern']) ? $menu_item['path'] : $_REQUEST['pattern'];
  $path = !isset($_REQUEST['path']) ? $menu_item['href'] : $_REQUEST['path'];

  $nodes = sticky_notes_load_by_path($pattern, $path);

  $items = array();
  foreach ($nodes as $node) {
    // update the node access history
    node_tag_new($node->nid);
    // build the themed output for this node
    $items[] = theme('sticky_notes_note', $node);
  }

  drupal_json(theme('sticky_notes_wrapper', implode('', $items)));
  exit();

}

/**
 * Ajax callback: Delete the given node
 *
 * @param object $node
 * @return void
 * @author berliner
 */

function sticky_notes_delete_js($node) {

  global $user;
  if (!node_access('delete', $node, $user)) {
    return;
  }

  if ($node->nid) {
    node_delete($node->nid);
    drupal_get_messages();
    $result = TRUE;
  }
  else {
    $result = FALSE;
  }

  drupal_json($result);
  exit();

}

/**
 * Ajax Callback: Save the position of a sticky note relative to the document
 *
 * @param object $node
 * @param int $x
 * @param int $y
 * @return void
 * @author berliner
 */

function sticky_notes_save_position_js($node, $x, $y) {

  // check if the user has permission to modify the node
  global $user;

  if (!node_access('update', $node, $user)) {
    return;
  }
  else {

    // reestablish order of the notes on the page and retrieve the highest z_index
    $z_index = sticky_notes_reorder_page_notes($node->link_path, $node->link_pattern);

    // update the notes position in the database and increase the z-index so
    // that this note will be on top of all the others
    db_query("UPDATE {node_sticky_note}
              SET position_x = %d, position_y = %d, position_z = %d
              WHERE nid = %d", $x, $y, $z_index++, $node->nid);

  }

}

/**
 * Ajax Callback: Save the position of a sticky note relative to the document
 *
 * @param object $node
 * @param int $x
 * @param int $y
 * @return void
 * @author berliner
 */

function sticky_notes_save_size_js($node, $width, $height) {

  // check if the user has permission to modify the node
  global $user;

  if (!node_access('update', $node, $user)) {
    return;
  }
  else {

    // update the notes position in the database and increase the z-index so
    // that this note will be on top of all the others
    db_query("UPDATE {node_sticky_note}
              SET width = %d, height = %d
              WHERE nid = %d", $width, $height, $node->nid);

  }

}

/**
 * Show the node add form
 *
 * @return tring
 * @author berliner
 */

function sticky_notes_dialog_add_js() {
  // Send the Modal Frame javascript for child windows to the page.
  modalframe_child_js();
  module_load_include('inc', 'node', 'node.pages');
  return node_add('sticky_notes');

}

/**
 * Show the node edit form
 *
 * @param object $node
 * @return string
 * @author berliner
 */

function sticky_notes_dialog_edit_js($node) {
  // Send the Modal Frame javascript for child windows to the page.
  modalframe_child_js();
  module_load_include('inc', 'node', 'node.pages');
  return drupal_get_form($node->type .'_node_form', $node);
}

/**
 * Show the node delete confirmation form
 *
 * @param object $node
 * @return string
 * @author berliner
 */

function sticky_notes_dialog_delete_js($node) {
  // Send the Modal Frame javascript for child windows to the page.
  modalframe_child_js();
  module_load_include('inc', 'node', 'node.pages');
  return drupal_get_form('node_delete_confirm', $node);
}