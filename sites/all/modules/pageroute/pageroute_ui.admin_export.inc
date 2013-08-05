<?php
// $Id: pageroute_ui.admin_export.inc,v 1.1.2.4 2009/08/05 14:36:32 fago Exp $

/**
 * @file
 * Pageroute UI Import/Export
 */


/**
 * Imports a pageroute
 */
function pageroute_ui_form_import(&$form_state) {
  $form = array();
  $form['import'] = array(
    '#type' => 'textarea',
    '#title' => t('Configuration to import'),
    '#description' => t('Just paste your exported pageroute here.'),
    '#rows' => 15,
    '#required' => TRUE,
  );
  $form['button'] = array('#type' => 'submit', '#weight' => 10, '#value' => t('Import'));

  return $form;
}

function pageroute_ui_form_import_submit($form, $form_state) {
  @eval('$route = '. $form_state['values']['import'] .';');
  if (isset($route) && is_array($route) && count($route)) {
    $route->options = serialize($route->options);
    drupal_write_record('pageroute_routes', $route);
    $route->prid = db_last_insert_id('pageroute_routes', 'prid');

    foreach ($route->pages as $page) {
      $page->options = serialize($page->options);
      drupal_write_record('pageroute_pages', $page);
    }

    pageroute_ui_update_neighbours($route);
    cache_clear_all('*', 'cache_menu', TRUE);
    _pageroute_clear_cache();
  }

  drupal_set_message(t('Import failed.'), 'error');
}

/**
 * Exports a rule
 */
function pageroute_ui_form_export(&$form_state, $prid) {
  $form = array();

  $route = pageroute_route::load($prid);

  $form['result'] = array(
    '#type' => 'textarea',
    '#title' => t('Exported rule configurations'),
    '#description' => t('Copy these data and paste them into the import page, to import.'),
    '#rows' => 15,
    '#attributes' => array('readonly' => 'readonly'),
    '#default_value' => var_export($route, TRUE),
  );

  return $form;
}
