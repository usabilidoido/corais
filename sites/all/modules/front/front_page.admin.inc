<?php

/**
 * @file
 * Admin page callbacks for the front page module.
 */

/**
 * Form for configuring front page settings.
 */
function front_page_admin() {

  $form['front_page_enable'] = array(
    '#type' => 'checkbox',
    '#title' => t('Front Page Override'),
    '#description' => t('Enable this if you want the front page module to manage the home page.'),
    '#default_value' => variable_get('front_page_enable', 0),
  );

  // Load any existing settings and build the by redirect by role form
  $form['roles'] = array(
    '#tree' => TRUE,
    '#type' => 'fieldset',
    '#title' => t('Roles'),
    '#description' => t('These are the settings for each role. If Front Page Override is enabled when a user reaches the home page the site will iterate through the roles below from top to bottom until firstly the user has the role and secondly the role is not set to SKIP. If no roles get selected the site front page will be shown. To rearrange the order in which the roles are processed you may do this at the !link.', array('!link' => l(t('Arrange tab'), 'admin/config/front/arrange'))),
    '#collapsible' => FALSE,
  );

  // build the form for roles
  $roles = user_roles();
  $front_page_data = front_page_get_all();

  // Set the type options common for all roles.
  $options = array(
    '' => t('skip'),
    'themed' => t('themed'),
    'full' => t('full'),
    'redirect' => t('redirect'),
    'alias' => t('alias'),
  );

  // Set the description common for all roles.
  $description = '<dl><dt>' . t('themed') . '</dt>
      <dd>' . t('means your default layout, theme and stylesheet will be loaded with your custom front_page.') . '</dd></dl>';
  $description .= '<dl><dt>' . t('full') . '</dt>
      <dd>' . t('allows you to have a completely different layout, style sheet etc.') . '</dd></dl>';
  $description .= '<dl><dt>' . t('redirect') . '</dt>
      <dd>' . t('will automatically redirect visitors already logged in to a specific page specified in the REDIRECT TO box.') . '</dd></dl>';
  $description .= '<dl><dt>' . t('alias') . '</dt>
      <dd>' . t('will display the page listed in path as if it were the home page. This option does not redirect.') . '</dd></dl>';

  foreach ($roles as $rid => $role) {

    $form['roles'][$rid] = array(
      '#type' => 'fieldset',
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
      '#title' => t('Front Page for !rolename.', array('!rolename' => $role)),
      '#weight' => isset($front_page_data[$rid]['weight']) ? $front_page_data[$rid]['weight'] : 0,
    );

    $form['roles'][$rid]['mode'] = array(
      '#type' => 'select',
      '#title' => t('Select mode'),
      '#default_value' => isset($front_page_data[$rid]['mode']) ? $front_page_data[$rid]['mode'] : '',
      '#options' => $options,
      '#description' => '<p>' . $description . '</p>',
    );

    $form['roles'][$rid]['data'] = array(
      '#type' => 'textarea',
      '#title' => t('Data'),
      '#default_value' => (isset($front_page_data[$rid]['data']) && isset($front_page_data[$rid]['mode']) && ($front_page_data[$rid]['mode'] == 'themed' || $front_page_data[$rid]['mode'] == 'full')) ? $front_page_data[$rid]['data'] : NULL,
      '#description' => t('Paste your HTML or TEXT here.') . '<br /><br />' . t('You can paste in the full HTML code for a complete page and include a different style sheet in the HEAD of the document if you want a completely different layout and style to the rest of your site.'),
    );
    $format = !empty($front_page_data[$rid]['filter_format']) ? $front_page_data[$rid]['filter_format'] : NULL;
    $form['roles'][$rid]['filter_format'] = filter_form($format, NULL, array('roles', $rid, 'filter_format'));

    $form['roles'][$rid]['path'] = array(
      '#type' => 'textfield',
      '#title' => t('Path'),
      '#default_value' => (isset($front_page_data[$rid]['data']) && isset($front_page_data[$rid]['mode']) && ($front_page_data[$rid]['mode'] == 'redirect' || $front_page_data[$rid]['mode'] == 'alias')) ? $front_page_data[$rid]['data'] : NULL,
      '#cols' => 20,
      '#rows' => 1,
      '#description' => t('If you are using <strong>Redirect</strong> or <strong>Alias</strong> you need to specify the path. An alias path should only include the URL part of a URL (eg "node/51"). A redirect path can contain a full URL including get parameters and fragment string (eg "node/51?page=5#anchor").'),
    );
  }

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save Settings'),
  );


  return $form;
}

/**
 * Validation hook for front_page_admin.
 */
function front_page_admin_validate($form, &$form_state) {
  if (is_array($form_state['values']['roles'])) {
    foreach ($form_state['values']['roles'] as $rid => $role) {
      switch ($role['mode']) {
        case 'themed':
        case 'full':
          if (empty($role['data']['value'])) {
            form_set_error('roles][' . $rid . '][data][value', 'You must set the data field for ' . $role['mode'] . ' mode.');
          }
          break;
        case 'redirect':
          if (empty($role['path'])) {
            form_set_error('roles][' . $rid . '][path', 'You must set the path field for redirect mode.');
          }
          break;
        case 'alias':
          if (empty($role['path'])) {
            form_set_error('roles][' . $rid . '][path', 'You must set the path field for alias mode.');
          }
          elseif (!preg_match('@^[^?#]+$@', $role['path'])) {
            form_set_error('roles][' . $rid . '][path', 'You must set only the URI part of a URL in alias mode.');
          }
          break;
      }
    }
  }
}

/**
 * Submit hook for front_page_admin.
 */
function front_page_admin_submit($form, &$form_state) {
  variable_set('front_page_enable', $form_state['values']['front_page_enable']);

  $front_page_data = front_page_get_all();

  db_query("UPDATE {front_page} SET mode=''");
  if (is_array($form_state['values']['roles'])) {
    foreach ($form_state['values']['roles'] as $rid => $role) {
      if (!isset($front_page_data[$rid]['weight'])) {
        $front_page_data[$rid]['weight'] = 0;
      }
      switch ($role['mode']) {
        case 'themed':
        case 'full':
          db_query("REPLACE INTO {front_page} (rid, mode, data, filter_format, weight) VALUES (%d, '%s', '%s', '%s', %d)", $rid, $role['mode'], $role['data'], $role['filter_format'], $front_page_data[$rid]['weight']);
          break;
        case 'redirect':
        case 'alias':
          db_query("REPLACE INTO {front_page} (rid, mode, data, filter_format, weight) VALUES (%d, '%s', '%s', '', %d)", $rid, $role['mode'], $role['path'], $front_page_data[$rid]['weight']);
          break;
        default:
          db_query("REPLACE INTO {front_page} (rid, mode, data, filter_format, weight) VALUES (%d, '', '', '', %d)", $rid, $front_page_data[$rid]['weight']);
      }
    }
  }
  drupal_set_message(t('Your settings have been saved.'));
}

/**
 * Form for arranging the roles according to what order the roles should be processed.
 */
function front_page_admin_arrange_form() {
  $roles = user_roles();
  $front_page_data = front_page_get_all();
  foreach ($roles as $rid => $role) {
    $front_page_data[$rid]['name'] = $role;
  }

  $form['roles'] = array(
    '#tree' => TRUE,
  );
  foreach ($front_page_data as $role_id => $role) {
    $form['roles'][$role_id]['title']['#value'] = $role['name'];
    $form['roles'][$role_id]['mode']['#value'] = !empty($role['mode']) ? $role['mode'] : 'skip';
    $form['roles'][$role_id]['preview']['#value'] = !empty($role['mode']) ? l(t('preview'), 'front_page/preview/' . $role_id, array('attributes' => array('target' => '_blank'))) : '';
    if (!empty($role['mode'])) {
      $form['roles'][$role_id]['enabled'] = array(
        '#type' => 'checkbox',
        '#title' => t('Enable'),
        '#title_display' => 'invisible',
        '#default_value' => TRUE,
      );
    }
    else {
      $form['roles'][$role_id]['enabled']['#value'] = 'disabled';
    }
    $form['roles'][$role_id]['weight'] = array(
      '#type' => 'weight',
      '#delta' => 10,
      '#default_value' => isset($role['weight']) ? $role['weight'] : 0,
    );
  }

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save Order'),
  );
  return $form;
}

/**
 * Submit hook for front_page_admin_arrange_form.
 */
function front_page_admin_arrange_form_submit($form, &$form_state) {
  $front_page_data = front_page_get_all();
  foreach ($form_state['values']['roles'] as $rid => $role) {
    $count = db_result(db_query("SELECT COUNT(*) FROM {front_page} WHERE rid = %d", $rid));
    if (!isset($front_page_data[$rid])) {
      db_query("INSERT INTO {front_page} (rid, mode, data, filter_format, weight) VALUES (%d, '', '', '', %d)", $rid, $role['weight']);
    }
    elseif (isset($role['enabled']) && !$role['enabled']) {
      db_query("UPDATE {front_page} SET mode = '', data = '', filter_format = '', weight = %d WHERE rid = %d", $role['weight'], $rid);
    }
    else {
      db_query("UPDATE {front_page} SET weight = %d WHERE rid = %d", $role['weight'], $rid);
    }
  }
}

/**
 * Form for setting the <front> placeholder to another path.
 */
function front_page_admin_home_links() {
  global $base_url;
  $form['front_page_home_link_path'] = array(
    '#type' => 'textfield',
    '#title' => t('Redirect your site HOME links to'),
    '#default_value' => variable_get('front_page_home_link_path', ''),
    '#cols' => 20, '#rows' => 1,
    '#description' => t('Specify where the user should be redirected to. An example would be <em>node/12</em>. Leave blank when you\'re not using HOME redirect.'),
    '#field_prefix' => $base_url . '/' . (variable_get('clean_url', 0) ? '' : '?q=')
  );

  return system_settings_form($form);
}

/**
 * Returns HTML for the front page arrange form into a table.
 *
 * @param $variables
 *   An associative array containing:
 *   - form: A render element representing the form.
 */
function theme_front_page_admin_arrange_form($form) {

  drupal_add_tabledrag('front-page-arrange', 'order', 'sibling', 'front-page-weight');

  $header = array(
    t('Role'),
    t('Mole'),
    t('Preview'),
    t('Enabled'),
    t('Weight'),
  );

  $rows = array();
  foreach (element_children($form['roles']) as $rid) {
    $element = &$form['roles'][$rid];

    // Add special classes to be used for tabledrag.js.
    $element['weight']['#attributes']['class'] .= ' front-page-weight';

    $row = array();
    $row[] = drupal_render($element['title']);
    $row[] = drupal_render($element['mode']);
    $row[] = drupal_render($element['preview']);
    $row[] = drupal_render($element['enabled']);
    $row[] = drupal_render($element['weight']);

    $row = array('data' => $row);
    $row['class'] .= ' draggable';
    $rows[] = $row;
  }
  $output = '';
  if (empty($rows)) {
    $rows[] = array(array('data' => 'no roles', 'colspan' => '5'));
  }
  $output .= theme('table', $header, $rows, array('id' => 'front-page-arrange'));
  $output .= drupal_render($form);
  return $output;
}