<?php
// $Id: flat_book.admin.inc,v 1.2 2010/07/29 03:47:32 criticalpatch Exp $
/**
 * @file
 * Admin pages for flat_book
 */

/**
 * Form builder for flat_book settings form
 */
function flat_book_admin() {
  $form['flat_book_max_menu_depth'] = array(
    '#type' => 'textfield',
    '#title' => t('Maximum book depth'),
    '#description' => t('The depth beneath which book pages will be flattened. The root is considered depth 1.'),
    '#default_value' => variable_get('flat_book_max_menu_depth', 3),
    '#size' => 5,
    '#maxlength' => 2,
    '#required' => TRUE,
  );
  $form['flat_book_display_book_navigation'] = array(
    '#type' => 'select',
    '#title' => t('Display book navigation'),
    '#default_value' => variable_get('flat_book_display_book_navigation', 0),
    '#options' => array(
      '0' => t('No'),
      '1' => t('Yes'),
    ),
  '#description' => t('Display the book navigation at the bottom of each page in a flattened book'),
  );
  $form['#validate'][] = 'flat_book_admin_validate';
  $form['#submit'][] = 'flat_book_admin_submit';
  return system_settings_form($form);
}

/**
 * Validation function for admin form
 */
function flat_book_admin_validate($form, $form_state) {
  if ($form_state['values']['flat_book_max_menu_depth'] < 1 ||
    $form_state['values']['flat_book_max_menu_depth'] > 99 ) {
    form_set_error('flat_book_max_menu_depth', 'Please enter a valid integer for the maximum menu depth', FALSE);
  }
}

/**
 * Submit handler for admin form
 */
function flat_book_admin_submit() {
  cache_clear_all('flat_book_node_', 'cache', TRUE);
  cache_clear_all('flat_book_node_admin_', 'cache', TRUE);
}
