<?php
// $Id: pageroute_ui.forms.inc,v 1.1.2.11 2009/09/10 10:14:08 fago Exp $

/**
 * @file
 * Adminstration forms, included from pageroute_ui.module
 */

include_once(drupal_get_path('module', 'pageroute') .'/pageroute.route.inc');

function pageroute_ui_route_edit(&$form_state, $op = 'add', $prid = NULL) {
  if ($op != 'add' && isset($prid) && is_numeric($prid)) {
    $route = db_fetch_object(db_query("SELECT * FROM {pageroute_routes} WHERE prid = %d", $prid));
    $route->options = unserialize($route->options);
    drupal_set_title(t('Edit route'));
  }

  $form['path'] = array('#type' => 'textfield',
                        '#title' => t('Path'),
                        '#maxlength' => 127,
                        '#default_value' => isset($route) ? $route->path : '',
                        '#required' => TRUE,
                        '#weight' => -5,
                  );
  $form['options']['#tree'] = TRUE;
  $form['options']['tabs'] = array(
    '#type' => 'radios',
    '#title' => t('Tabs'),
    '#options' => array(
      0 => t('Don\'t show any tabs'),
      PAGEROUTE_MENU_TABS => t('Use the common drupal menu tabs'),
    ),
    '#default_value' => isset($route->options['tabs']) ? $route->options['tabs'] : 0,
    '#description' => t('Note that the commom drupal menu tabs won\'t save the actual form, if they are used. Also any arguments appended to the URL will be lost. They are in particular useful for pageroutes, which focus on displaying content.'),
    '#weight' => -1,
  );
  $form['options']['access'] = array(
    '#type' => 'fieldset',
    '#title' => t('Access control'),
    '#collapsible' => TRUE,
  );
  $form['options']['access']['allowed_roles'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Permit access to the pageroute for only this roles'),
    '#options' => array_map('filter_xss_admin', user_roles()),
    '#default_value' => isset($route->options['access']['allowed_roles']) ? $route->options['access']['allowed_roles'] : array(2),
  );
  $form['options']['access']['#weight'] = 5;

  $form['advanced'] = array(
    '#type' => 'fieldset',
    '#title' => t('Advanced settings'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#tree' => FALSE,
    '#weight' => 10,
  );
  $form['advanced']['options']['#tree'] = TRUE;
  $form['advanced']['options']['redirect_path'] = array(
    '#type' => 'textfield',
    '#title' => t('Customized redirect path'),
    '#default_value' => isset($route) ? $route->options['redirect_path'] : '',
    '#maxlength' => 64,
    '#size' => 45,
    '#description' => t('If entered, a user will be redirected to this path after he has completed the pageroute. Specify an existing path. For example: node/28, user, taxonomy/term/1+2.') .' '.
                      t('You may also use the following replacement variables: !uid (User ID), !nid (Node ID). For example use: node/!nid to redirect to the node with the id taken from the pageroute arguments.'),
    '#field_prefix' => url(NULL, array('absolute' => TRUE)) . (variable_get('clean_url', 0) ? '' : '?q=')
  );
  $form['advanced']['options']['no_messages'] = array(
    '#type' => 'checkbox',
    '#title' => t('Don\'t show drupal messages during this route.'),
    '#default_value' => isset($route->options['no_messages']) ? $route->options['no_messages'] : 1,
  );

  $form['submit'] = array('#type' => 'submit', '#value' => t('Save'), '#weight' => 15 );
  if (isset($route)) {
    $form['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete'),
      '#submit' => array('pageroute_ui_route_delete_submit'),
      '#weight' => 16,
    );
  }
  else {
    $route = (object) array('new' => TRUE);
  }

  $form['route'] = array('#type' => 'value', '#value' => &$route);

  $form['#validate'][] = 'pageroute_ui_route_edit_validate';
  $form['#submit'][] = 'pageroute_ui_route_edit_submit';
  $form['#theme'] = 'pageroute_ui_route';

  return $form;
}

function pageroute_ui_route_edit_validate($form, &$form_state) {
  $path = rtrim(ltrim($form_state['values']['path'], '/'), '/');
  $form_state['values']['path'] = $path;

  if (!valid_url($path)) {
    form_set_error('path', t('The path has to be a valid URL.'));
  }

  if (isset($form_state['values']['route']->path) &&
    ($form_state['values']['route']->path != $path) &&
    gettype(menu_get_item($path)) == 'array') {
    form_set_error('path', t('This drupal path is already in use. Choose another path.'));
  }
}

function pageroute_ui_route_delete_submit($form, &$form_state) {
  $form_state['redirect'] = 'admin/build/pageroute/'. $form_state['values']['route']->prid .'/delete';
}

function pageroute_ui_route_edit_submit($form, &$form_state) {
  $record->path = $form_state['values']['path'];
  $record->options = serialize($form_state['values']['options']);

  if (!isset($form_state['values']['route']->new)) {
    $record->prid = $form_state['values']['route']->prid;
    drupal_write_record('pageroute_routes', $record, array('prid'));
  }
  else {
    drupal_write_record('pageroute_routes', $record);
    $form_state['values']['route']->prid = db_last_insert_id('pageroute_routes', 'prid');
  }
  _pageroute_clear_cache();
  menu_rebuild();
  $form_state['redirect'] = array('admin/build/pageroute', drupal_get_destination());
}

function pageroute_ui_route_delete_confirm(&$form_state, $prid) {
  if (is_numeric($prid)) {
    $route = db_fetch_object(db_query("SELECT * FROM {pageroute_routes} WHERE prid = %d", $prid));
    $route->options = unserialize($route->options);
  }
  if (!isset($route)) {
    drupal_not_found();
    exit;
  }

  $form['route'] = array('#type' => 'value', '#value' => $route);

  return confirm_form($form,
    t('Are you sure you want to delete the route %path?', array('%path' => $route->path)),
    'admin/build/pageroute',
    t('Deleting a route will delete all the pages you created in it. This action cannot be undone.'),
    t('Delete'), t('Cancel')
  );
}

function pageroute_ui_route_delete_confirm_submit($form, &$form_state) {
  db_query("DELETE FROM {pageroute_pages} WHERE prid = %d", $form_state['values']['route']->prid);
  db_query("DELETE FROM {pageroute_routes} WHERE prid = %d", $form_state['values']['route']->prid);
  _pageroute_clear_cache();
  menu_rebuild();
  drupal_set_message(t('Your route has been deleted.'));
  $form_state['redirect'] = 'admin/build/pageroute';
}

function pageroute_ui_page_edit($op, $prid, $page_name = NULL) {
  $page = NULL;
  $page_data = NULL;

  if (is_numeric($prid)) {
    $route = PagerouteRoute::load($prid);
  }

  if (!isset($route)) {
    drupal_not_found();
    exit;
  }

  if ($op == 'add') {
    drupal_set_title(check_plain($route->path));

    if (isset($page_name)) {
      list($module, $type) = split('[.]', $page_name);
    }
    else {
      return drupal_get_form('pageroute_ui_page_add_type', $prid);
    }
  }
  elseif ($op != 'edit') {
    return pageroute_ui_route_overview($route);
  }
  elseif ($page_name) {
    $route->start($page_name);
    $page_data = &$route->pageDataCurrent;

    if (!isset($page_data)) {
      drupal_not_found();
      exit;
    }
    drupal_set_title(check_plain($page_data->name));
    $type = $page_data->type;
    $module = $page_data->module;
  }

  return drupal_get_form('pageroute_ui_page_edit_page', $route, $module, $type, $page_data);
}

function pageroute_ui_page_edit_page(&$form_state, $route, $module, $type, $page = NULL) {

  if (!$page) {
    $page->name = '';
    $page->title = '';
    $page->options['activated'] = 1;
    $page->options['no_tab'] = 0;
    $page->options['forward'] = t('Forward');
    $page->options['back'] = t('Back');
    $page->options['content-type'] = '';
    $page->weight = '';
    $new = TRUE;
  }

  $page->route = $route;
  pageroute_include_page_type($module, $type);
  $page_class = pageroute_get_page_class($module, $type);
  $help = call_user_func(array($page_class, 'help'));

  $form['module'] = array(
    '#type' => 'hidden',
    '#value' => $module,
  );

  if ($help) {
    $form['help'] = array(
      '#type' => 'fieldset',
      '#title' => t('Help'),
      '#collapsible' => TRUE,
      '#description' => $help,
    );
  }

  $form['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Name'),
    '#maxlength' => 63,
    '#default_value' => $page->name,
    '#required' => TRUE,
    '#description' => t('Last part of the page\'s URL. Used for identifing the page.'),
  );
  $form['title'] = array(
    '#type' => 'textfield',
    '#title' => t('Title'),
    '#maxlength' => 255,
    '#default_value' => $page->title,
    '#description' => t('An optional title which will be set when the page is viewed.'),
    '#weight' => 1,
  );
  $form['options']['activated'] = array(
    '#type' => 'checkbox',
    '#title' => t('Activated'),
    '#default_value' => $page->options['activated'],
    '#description' => t('When the pageroute is generated, deactivated pages will be ignored.'),
    '#weight' => 0,
  );
  if ($page->route->options['tabs']) {
    $form['options']['no_tab'] = array(
      '#type' => 'checkbox',
      '#title' => t('Don\'t show a tab for this page'),
      '#default_value' => $page->options['no_tab'],
      '#weight' => 1,
    );
  }
  $form['options']['forward'] = array(
    '#type' => 'textfield',
    '#title' => t('Forward button label'),
    '#maxlength' => 32,
    '#default_value' => $page->options['forward'],
    '#description' => t('The label of the forward button. Leave it empty to hide the button.'),
    '#weight' => 3,
  );
  $form['options']['back'] = array(
    '#type' => 'textfield',
    '#title' => t('Back button label'),
    '#maxlength' => 32,
    '#default_value' => $page->options['back'],
    '#description' => t('The label of the back button. Leave it empty to hide the button.'),
    '#weight' => 4,
  );
  $form['options']['cancel'] = array(
    '#type' => 'textfield',
    '#title' => t('Cancel link label'),
    '#maxlength' => 32,
    '#default_value' => isset($page->options['cancel']) ? $page->options['cancel'] : NULL,
    '#description' => t('The label of the cancel link. Leave it empty to hide the link, but note that the link is the only possibility for the user to not save the form while staying in the route.'),
    '#weight' => 3,
  );
  $form['options']['#tree'] = TRUE;
  $form['options']['#weight'] = 3;

  $form['weight'] = array(
    '#type' => 'weight',
    '#title' => t('Weight'),
    '#default_value' => $page->weight,
    '#description' => t('Used for ordering the pages. Pages with lower weights are used first.'),
    '#weight' => 7,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
    '#weight' => 8,
  );

  $form['cancel'] = array(
    '#type' => 'submit',
    '#value' => t('Cancel'),
    '#submit' => array('pageroute_ui_page_cancel_submit'),
    '#weight' => 9,
  );


  if (!isset($new)) {
    $form['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete'),
      '#submit' => array('pageroute_ui_page_delete_submit'),
      '#weight' => 10,
      );
    $form['page'] = array('#type' => 'value', '#value' => $page);
  }

  $form['page_type'] = array('#type' => 'value', '#value' => $type);
  $form['route'] = array('#type' => 'value', '#value' => $route);

  $form['#validate'][] = 'pageroute_ui_page_edit_validate';
  $form['#submit'][] = 'pageroute_ui_page_edit_submit';

  //let the page type add further form items
  call_user_func_array(array($page_class, 'getAdminForm'), array($page, &$form));

  return $form;
}

function pageroute_ui_page_delete_submit($form, &$form_state) {
  drupal_redirect_form($form, 'admin/build/pageroute/'. $form_state['values']['route']->prid .'/page-delete/'. $form_state['values']['page']->name);
}

function pageroute_ui_page_cancel_submit($form, &$form_state) {
  drupal_redirect_form($form, 'admin/build/pageroute/'. $form_state['values']['route']->prid);
}

function pageroute_ui_page_edit_validate($form, &$form_state) {

  $name = rtrim(ltrim($form_state['values']['name'], '/'), '/');

  $form_state['values']['name'] = $name;

  if (strpos($name, '/') !== FALSE) {
    form_set_error('name', t('The page name must not contain a slash "/".'));
  }
  if (!valid_url($name)) {
    form_set_error('name', t('The page name has to be a valid URL.'));
  }

  if ((!isset($form_state['values']['page']) || $form_state['values']['page']->name != $name) &&
      db_result(db_query("SELECT * FROM {pageroute_pages} WHERE prid = %d AND name = '%s'",
      $form_state['values']['route']->prid, $name))) {

    form_set_error('name', t('A page with this name already exists. Choose another name.'));
  }
}

function pageroute_ui_page_edit_submit($form, &$form_state) {

  if (isset($form_state['values']['page'])) {
    $edit_page = (object)$form_state['values'];
    pageroute_ui_update_page($form_state['values']['route'], $edit_page, $form_state['values']['page']->name);
    pageroute_ui_update_neighbours($form_state['values']['route']);
  }
  else {
    //add a new page
    $edit_page = (object)$form_state['values'];
    db_query("INSERT INTO {pageroute_pages} (prid, name, title, weight, module, type, options) VALUES(%d, '%s', '%s', %d, '%s', '%s', '%s')",
      $form_state['values']['route']->prid, $form_state['values']['name'], $form_state['values']['title'], $form_state['values']['weight'], $form_state['values']['module'], $form_state['values']['page_type'], serialize($edit_page->options));
    pageroute_ui_update_neighbours($form_state['values']['route']);
  }
  _pageroute_clear_cache();
  menu_rebuild();
  $form_state['redirect'] = array('admin/build/pageroute/'. $form_state['values']['route']->prid, drupal_get_destination());
}

/*
 * Shows a form for choosing the inital type of the page
 */
function pageroute_ui_page_add_type(&$form_state, $prid) {
  $options = pageroute_get_types('options');

  $form['type'] = array(
    '#type' => 'radios',
    '#title' => t('Choose a page type'),
    '#options' => $options,
    '#required' => TRUE
    );

  $form['prid'] = array(
    '#type' => 'hidden',
    '#value' => $prid
    );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Forward')
    );

  $form['#submit'][] = 'pageroute_ui_page_add_type_submit';

  return $form;
}

function pageroute_ui_page_add_type_submit($form, &$form_state) {
  $form_state['redirect'] = array('admin/build/pageroute/'. $form_state['values']['prid'] .'/page-add/'. $form_state['values']['type']);
}


function pageroute_ui_page_delete_confirm(&$form_state, $prid, $page_name) {
  $route = PagerouteRoute::load($prid);

  $page = $route->pages[$route->pageIndex[$page_name]];

  if (!$page) {
    drupal_not_found();
    exit;
  }

  $form['page'] = array('#type' => 'value', '#value' => $page);
  $form['route'] = array('#type' => 'value', '#value' => $route);

  return confirm_form($form,
    t('Are you sure you want to delete the page %name?', array('%name' => $page->name)),
    'admin/build/pageroute/'. $page->route->prid,
    t('This action cannot be undone.'), t('Delete'), t('Cancel')
  );
}

function pageroute_ui_page_delete_confirm_submit($form, &$form_state) {
  db_query("DELETE FROM {pageroute_pages} WHERE prid = %d AND name ='%s'",
    $form_state['values']['page']->prid, $form_state['values']['page']->name);
  pageroute_ui_update_neighbours($route = (object)array('prid' => $form_state['values']['page']->prid));
  _pageroute_clear_cache();
  menu_rebuild();
  drupal_set_message(t('Your page has been deleted.'));
  $form_state['redirect'] = array('admin/build/pageroute/'. $form_state['values']['page']->prid, drupal_get_destination());
}

/**
 * Imports a route
 */
function pageroute_ui_import(&$form_state) {
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

function pageroute_ui_import_validate($form, &$form_state) {
  include_once(drupal_get_path('module', 'pageroute') .'/pageroute.route.inc');
  eval('$route = '. $form_state['values']['import'] .';');

  if (!isset($route) || !is_array($route) || !count($route)) {
    drupal_set_message(t('Import failed.'), 'error');
    return FALSE;
  }

  if (menu_get_item($route['path']) != FALSE) {
    form_set_error('path', t('This drupal path is already in use.'));
    return FALSE;
  }

  $form_state['route'] = (object)$route;
  return TRUE;
}

function pageroute_ui_import_submit($form, &$form_state) {

  $route = &$form_state['route'];

  $route->options = serialize($route->options);
  drupal_write_record('pageroute_routes', $route);
  $route_prid = db_last_insert_id('pageroute_routes', 'prid');

  foreach ($route->pages as &$page) {
    $page = (object)$page;
    $page->prid = $route_prid;
    $page->options = serialize($page->options);
    drupal_write_record('pageroute_pages', $page);
  }

  pageroute_ui_update_neighbours($route);
  _pageroute_clear_cache();
  menu_rebuild();
  $form_state['redirect'] = 'admin/build/pageroute';
}

/**
 * Exports a route
 */
function pageroute_ui_export(&$form_state, $prid) {
  $form = array();

  include_once(drupal_get_path('module', 'pageroute') .'/pageroute.route.inc');
  $route = PagerouteRoute::load($prid);
  unset($route->prid);

  foreach ($route->pages as &$page) {
    unset($page->route);
    unset($page->prid);
    unset($page->options['neighbours']);
    $page = (array)$page;
  }

  $form['result'] = array(
    '#type' => 'textarea',
    '#title' => t('Exported rule configurations'),
    '#description' => t('Copy these data and paste them into the import page, to import.'),
    '#rows' => 15,
    '#attributes' => array('readonly' => 'readonly'),
    '#default_value' => var_export((array)$route, TRUE),
  );

  return $form;
}
