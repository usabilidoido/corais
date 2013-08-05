<?php

/**
 * Form for adding/editing custom Dashboards.
 */
function spaces_dashboard_add() {
  $custom = variable_get('spaces_dashboard_custom', array('custom-1' => t('Dashboard')));
  $form = array();
  $form['spaces_dashboard_custom'] = array(
    '#description' => t('Enter a label for each dashboard you would like to use. To remove a custom dashboard clear its field.'),
    '#title' => t('Custom dashboards'),
    '#tree' => TRUE,
    '#type' => 'fieldset',
  );
  for ($i = 1; $i < 6; $i++) {
    $form['spaces_dashboard_custom']["custom-{$i}"] = array(
      '#type' => 'textfield',
      '#default_value' => isset($custom["custom-{$i}"]) ? $custom["custom-{$i}"] : '',
      '#element_validate' => array('spaces_dashboard_add_validate'),
    );
  }
  return system_settings_form($form);
}

/**
 * Element validator for custom Dashboards.
 */
function spaces_dashboard_add_validate($element, &$form_state) {
  $value = trim($element['#value']);
  $key = end($element['#parents']);
  if ($key === 'custom-1' && empty($value)) {
    $form_state['values']['spaces_dashboard_custom'][$key] = t('Dashboard');
  }
  else {
    $form_state['values']['spaces_dashboard_custom'][$key] = $value;
  }
}

/**
 * Provide a form that allows administrators to select which blcoks are
 * available in which dashboards.
 */
function spaces_dashboard_admin_form() {
  $blocks = context_get_plugin('reaction', 'block')->get_blocks(NULL, NULL, TRUE);
  $default = variable_get('spaces_dashboard_blocks', array());
  $form['spaces_dashboard_blocks'] = array(
    '#tree' => TRUE,
    '#theme' => 'spaces_dashboard_admin_form',
  );
  ksort($blocks);
  foreach ($blocks as $k => $block) {
    $module = spaces_dashboard_get_module($block);
    $info = context_get_info('module', $module);
    $form['spaces_dashboard_blocks'][$k] = array(
      '#type' => 'checkbox',
      '#title' => $block->info,
      '#grouping' => $info['name'],
      '#grouping_id' => $module,
    );
    if (isset($default[$k])) {
      $form['spaces_dashboard_blocks'][$k]['#default_value'] = $default[$k];
    }
  }
  return system_settings_form($form);
}

/**
 * Validation for spaces_dashboard_admin_form, actually just limits what is
 * saved to what needs to be saved.
 */
function spaces_dashboard_admin_form_validate($form, &$form_state) {
  $form_state['values']['spaces_dashboard_blocks'] = array_filter($form_state['values']['spaces_dashboard_blocks']);
}

/**
 * Allow administrators to select which regions and layouts are available
 * in dashboards.
 */
function spaces_dashboard_admin_region_form($form_state) {
  $theme_key = variable_get('theme_default', 'garland');
  $layouts = module_exists('context_layouts') ? context_layouts_get_layouts($theme_key) : array();
  if (empty($layouts)) {
    $layouts['default'] = array(
      'title' => t('Default'),
      'description' => t('Default layout'),
      'regions' => array_keys(system_region_list($theme_key))
    );
  }

  $region_labels = system_region_list($theme_key);

  $form = array();
  $form['spaces_dashboard_layouts'] = array('#tree' => TRUE);
  $form['spaces_dashboard_regions'] = array('#tree' => TRUE);

  $default_layouts = variable_get('spaces_dashboard_layouts', array());
  $default_regions = variable_get('spaces_dashboard_regions', array());

  foreach ($layouts as $k => $v) {
    if (!empty($v['regions'])) {
      $form['spaces_dashboard_layouts'][$k] = array(
        '#type' => 'checkbox',
        '#title' => isset($v['name']) ? $v['name'] : $k,
        '#description' => isset($v['description']) ? $v['description'] : NULL,
        '#default_value' => isset($default_layouts[$k]) ? $default_layouts[$k] : TRUE,
      );
      foreach ($v['regions'] as $region) {
        $form['spaces_dashboard_regions'][$k][$region] = array(
          '#type' => 'checkbox',
          '#title' => check_plain($region_labels[$region]),
          '#grouping' => isset($v['name']) ? check_plain($v['name']) : $k,
          '#grouping_id' => $k,
          '#default_value' => isset($default_regions[$k][$region]) ? $default_regions[$k][$region] : TRUE,
        );
      }
    }
  }
  $form = system_settings_form($form);
  $form['#theme'] = 'spaces_dashboard_admin_region_form';
  return $form;
}

/**
 * Theme function to display the dashboar block selector as a table.
 */
function theme_spaces_dashboard_admin_form($form) {
  $links = array();
  $headers = array();
  $tables = array();
  foreach (element_children($form) as $i) {
    $grouping = $form[$i]['#grouping'];
    $grouping_id = $form[$i]['#grouping_id'];
    if (!isset($links[$grouping_id])) {
      $links[$grouping_id] = array(
        'title' => $grouping,
        'href' => '',
        'attributes' => array('class' => "dashboard-blocks-{$grouping_id}"),
      );
      $tables[$grouping_id] = array();
      $headers[$grouping_id] = array(theme('table_select_header_cell'), t('Block'));
    }

    $row = array();
    $title = $form[$i]['#title'];
    unset($form[$i]['#title']);
    $row[] = drupal_render($form[$i]);
    $row[] = check_plain($title);
    $tables[$grouping_id][] = $row;
  }
  ksort($links);
  ksort($tables);
  ksort($headers);
  foreach ($tables as $grouping_id => $rows) {
    $output .= theme('table', $headers[$grouping_id], $tables[$grouping_id], array('id' => "dashboard-blocks-{$grouping_id}"));
  }

  $output = '<div id="dashboard-form-content"><div class="table-wrapper">'. $output .'</div></div>';
  $output .= '<div id="dashboard-form-links">'.
    theme('links', $links, array('id' => 'dashboard-blocks-links')).
    '</div><div class="clear-block"></div>';

  drupal_add_js(drupal_get_path('module', 'spaces_dashboard') . '/spaces_dashboard.admin.js');
  drupal_add_css(drupal_get_path('module', 'spaces_dashboard') . '/spaces_dashboard.admin.css');
  return $output;
}

/**
 * Theme function to display the dashboar block selector as a table.
 */
function theme_spaces_dashboard_admin_region_form($form) {
  $rows = array();
  foreach (element_children($form['spaces_dashboard_layouts']) as $layout) {
    $title = check_plain($form['spaces_dashboard_layouts'][$layout]['#title']);
    $description = check_plain($form['spaces_dashboard_layouts'][$layout]['#description']);
    $help = t('Enable this layout for use with the dashboard. Select the regions that can be edited below.');
    $label = "<strong>{$title}</strong> &mdash; <em>{$description}</em><div class='description'>{$help}</div>";
    unset($form['spaces_dashboard_layouts'][$layout]['#title']);
    unset($form['spaces_dashboard_layouts'][$layout]['#description']);

    $rows[] = array(
      array('data' => drupal_render($form['spaces_dashboard_layouts'][$layout]), 'header' => TRUE),
      array('data' => $label, 'header' => TRUE, 'colspan' => 2, 'class' => 'label'),
    );

    foreach (element_children($form['spaces_dashboard_regions'][$layout]) as $region) {
      $title = $form['spaces_dashboard_regions'][$layout][$region]['#title'];
      unset($form['spaces_dashboard_regions'][$layout][$region]['#title']);
      $rows[] = array(
        array('data' => ''),
        array('data' => drupal_render($form['spaces_dashboard_regions'][$layout][$region])),
        array('data' => check_plain($title), 'class' => 'label'),
      );
    }
  }

  drupal_add_js(drupal_get_path('module', 'spaces_dashboard') . '/spaces_dashboard.admin.js');
  drupal_add_css(drupal_get_path('module', 'spaces_dashboard') . '/spaces_dashboard.admin.css');
  $output = theme('table', array(), $rows, array('class' => 'spaces-dashboard-regions'));
  $output .= drupal_render($form);
  return $output;
}
