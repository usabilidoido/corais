<?php

/**
 * @file
 *
 * @author greenSkin
 */

/**
 * A router theme function.
 *
 * Appropriately themes the system modules page with alterations and based on
 * set variables
 *
 * @param $form
 *   An associative array containing the structure of the form.
 *
 * @return
 *   An output string.
 */
function theme_module_filter_system_modules($form) {
  if (isset($form['confirm'])) {
    return drupal_render($form);
  }

  drupal_add_css(drupal_get_path('module', 'module_filter') .'/css/module_filter.css');

  $output = '';
  if (variable_get('module_filter_tabs', 1)) {
    drupal_add_css(drupal_get_path('module', 'module_filter') .'/css/module_filter_tab.css');
    drupal_add_js(drupal_get_path('module', 'module_filter') .'/js/module_filter_tab.js');

    drupal_add_js(array('moduleFilter' => array('visualAid' => variable_get('module_filter_visual_aid', 1))), 'setting');

    if (variable_get('module_filter_dynamic_save_position', 0)) {
      drupal_add_js(drupal_get_path('module', 'module_filter') .'/js/dynamic_position.js');
    }

    $form['module_filter']['#size'] = 45;
    $output .= theme('module_filter_system_modules_tabs', $form);
  }
  else {
    drupal_add_js(drupal_get_path('module', 'module_filter') .'/js/module_filter.js');

    $form['module_filter']['#prefix'] = '<div id="module-filter-wrapper" style="display: none;">';
    $form['module_filter']['#suffix'] = '</div>';
    $output = drupal_render($form['module_filter']);
    $form['#theme'] = 'system_modules';
    $output .= theme('system_modules', $form);
  }
  return $output;
}

/**
 * Theme callback for the modules form.
 *
 * @param $form
 *   An associative array containing the structure of the form.
 *
 * @return
 *   An output string.
 */
function theme_module_filter_system_modules_tabs($form) {
  if (isset($form['confirm'])) {
    return drupal_render($form);
  }

  // Individual table headers.
  $header = array();
  $header[] = array('data' => t('Enabled'), 'class' => 'checkbox');
  if (module_exists('throttle')) {
    $header[] = array('data' => t('Throttle'), 'class' => 'checkbox');
  }
  $header[] = t('Name');
  $header[] = t('Version');
  $header[] = t('Description');

  // Pull package information from module list and start grouping modules.
  $modules = $form['validation_modules']['#value'];
  foreach ($modules as $module) {
    if (!isset($module->info['package']) || !$module->info['package']) {
      $module->info['package'] = t('Other');
    }
    $packages[$module->info['package']][$module->name] = $module->info;
  }
  ksort($packages);

  // Display packages.
  $tab_counts = array(t('All') => array('id' => 'all', 'enabled' => 0, 'total' => 0));
  $rows = array();
  foreach ($packages as $package => $modules) {
    $id = strtolower($package);
    // $id = preg_replace('/[^a-z ]\//', '', $id);
    $id = preg_replace('/([^a-z])([\/(  )])*/', '-', $id);

    if (!isset($tab_counts[$package])) {
      $tab_counts[$package] = array('id' => $id, 'enabled' => 0, 'total' => 0);
    }

    foreach ($modules as $key => $module) {
      $tab_counts[t('All')]['total']++;
      $tab_counts[$package]['total']++;
      if ($form['status'][$key]['#default_value']) {
        $tab_counts[$package]['enabled']++;
        $tab_counts[t('All')]['enabled']++;
      }

      $row = array();
      $description = drupal_render($form['description'][$key]);
      if (isset($form['status']['#incompatible_modules_core'][$key])) {
        unset($form['status'][$key]);
        $status = theme('image', 'misc/watchdog-error.png', t('incompatible'), t('Incompatible with this version of Drupal core'));
        $description .= '<div class="incompatible">'. t('This version is incompatible with the !core_version version of Drupal core.', array('!core_version' => VERSION)) .'</div>';
      }
      elseif (isset($form['status']['#incompatible_modules_php'][$key])) {
        unset($form['status'][$key]);
        $status = theme('image', 'misc/watchdog-error.png', t('incompatible'), t('Incompatible with this version of PHP'));
        $php_required = $form['status']['#incompatible_modules_php'][$key];
        if (substr_count($php_required, '.') < 2) {
          $php_required .= '.*';
        }
        $description .= '<div class="incompatible">'. t('This module requires PHP version @php_required and is incompatible with PHP version !php_version.', array('@php_required' => $php_required, '!php_version' => phpversion())) .'</div>';
      }
      else {
        $status = drupal_render($form['status'][$key]);
      }
      $row[] = array('data' => $status, 'class' => 'checkbox');
      if (module_exists('throttle')) {
        $row[] = array('data' => drupal_render($form['throttle'][$key]), 'class' => 'checkbox');
      }

      // Add labels only when there is also a checkbox.
      if (isset($form['status'][$key])) {
        $row[] = '<strong class="project-name"><label for="'. $form['status'][$key]['#id'] .'">'. drupal_render($form['name'][$key]) .'</label></strong>';
      }
      else {
        $row[] = '<strong class="project-name">'. drupal_render($form['name'][$key]) .'</strong>';
      }

      $row[] = array('data' => drupal_render($form['version'][$key]), 'class' => 'version');
      $row[] = array('data' => $description, 'class' => 'description');

      // Set the key for the row to the module name so we can easily sort it.
      // Append the $key variable to the end of the row key to ensure a unique
      // key. http://drupal.org/node/840324
      $rows[strtolower($form['name'][$key]['#value'] . $key)] = array(
        'data' => $row,
        'class' => $id .'-tab-content'
      );
    }
  }

  $tabs = array();
  foreach ($tab_counts as $package => $value) {
    $count = (variable_get('module_filter_count_enabled', 1)) ? '<span class="counts">'. t('!enabled of !total', array('!enabled' => $value['enabled'], '!total' => $value['total'])) .'</span>' : '';
    $tabs[strtolower($package)] = '<li'. (($value['id'] == 'all') ? ' class="active"' : '') .'><a id="'. $value['id'] .'-tab" class="project-tab" href="#'. str_replace('-', '_', $value['id']) .'">'. $package . $count .'</a></li>';
  }

  $all_tab = array_shift($tabs); // Shifts the 'All' tab off so we can prepend it back on after sorting.
  ksort($tabs);
  $tabs = array_merge(array(t('All') => $all_tab), $tabs);

  ksort($rows);
  $output = '<div id="module-filter-wrapper">';
  $output .= '<div id="module-filter-left">';
  $output .= '<div id="module-filter-tabs"><ul>'. implode($tabs) .'</ul></div>';
  $output .= '<div id="module-filter-submit">'. drupal_render($form['buttons']) .'</div></div>';
  $output .= '<div id="module-filter-right"><div id="module-filter-squeeze">'. drupal_render($form['module_filter']);
  $output .= theme('table', $header, $rows, array('id' => 'projects')) .'</div></div>';
  $output .= '<div class="clear-block"></div>';
  $output .= '</div>';

  $output .= drupal_render($form);
  return $output;
}
