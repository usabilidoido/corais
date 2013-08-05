<?php

/**
 * CTools export UI extending class.
 */
class spaces_presets_export_ui extends ctools_export_ui {
  function list_page($js, $input) {
    return drupal_get_form('spaces_preset_list', $this);
  }

  /**
   * Override of edit_form_submit().
   * Don't copy values from $form_state['values'].
   */
  function edit_form_submit(&$form, &$form_state) {
    if (!empty($this->plugin['form']['submit'])) {
      $this->plugin['form']['submit']($form, $form_state);
    }
  }
}

/**
 * Presets form.
 */
function spaces_preset_list($form_state, $export_ui) {
  $form = array();

  $types = array();
  foreach (spaces_types(TRUE) as $type => $info) {
    $types[$type] = $info['title'];
  }

  $form['new'] = array(
    '#tree' => FALSE,
    '#theme' => 'spaces_preset_list_new',
    'space_type' => array(
      '#title' => t('Type'),
      '#type' => 'select',
      '#options' => $types,
    ),
    'name' => array(
      '#type' => 'textfield',
      '#maxlength' => 64,
      '#size' => 32,
      '#title' => t('Machine ID'),
      '#element_validate' => array('spaces_preset_name_validate'),
    ),
    'title' => array(
      '#type' => 'textfield',
      '#maxlength' => 64,
      '#size' => 32,
      '#title' => t('Name'),
    ),
    'submit' => array(
      '#type' => 'submit',
      '#value' => t('Create new preset'),
      '#submit' => array('spaces_preset_list_new'),
    ),
  );

  // Generate preset options.
  foreach ($types as $type => $title) {
    module_load_include('inc', 'spaces', 'spaces.admin');
    $presets = spaces_preset_load(NULL, $type, TRUE);
    ksort($presets);
    $form[$type] = spaces_preset_form($presets, $type, TRUE);
    $form[$type]['#title'] = t('@spacetype presets', array('@spacetype' => $title));
    $form[$type]['#description'] = t('Select a default preset for each new @spacetype.', array('@spacetype' => $title));

    $plugin = $export_ui->plugin;
    foreach ($presets as $name => $item) {
      // Note: Creating this list seems a little clumsy, but can't think of
      // better ways to do this.
      $allowed_operations = drupal_map_assoc(array_keys($plugin['allowed operations']));
      $not_allowed_operations = array('import');

      if ($item->type == t('Normal')) {
        $not_allowed_operations[] = 'revert';
      }
      elseif ($item->type == t('Overridden')) {
        $not_allowed_operations[] = 'delete';
      }
      else {
        $not_allowed_operations[] = 'revert';
        $not_allowed_operations[] = 'delete';
      }
      $not_allowed_operations[] = empty($item->disabled) ? 'enable' : 'disable';
      foreach ($not_allowed_operations as $op) {
        // Remove the operations that are not allowed for the specific
        // exportable.
        unset($allowed_operations[$op]);
      }

      $operations = array();
      foreach ($allowed_operations as $op) {
        $operations[$op] = array(
          'title' => $plugin['allowed operations'][$op]['title'],
          'href' => ctools_export_ui_plugin_menu_path($plugin, $op, $name),
        );
        if (!empty($plugin['allowed operations'][$op]['token'])) {
          $operations[$op]['query'] = array('token' => drupal_get_token($op));
        }
      }
      $form[$type]['storage'][$item->name] = array('#type' => 'markup', '#value' => check_plain($item->type));
      $form[$type]['actions'][$item->name] = array('#type' => 'markup', '#value' => theme('links', $operations));
    }
  }
  $form = system_settings_form($form);
  return $form;
}

/**
 * Submit handler for preset creation.
 */
function spaces_preset_list_new(&$form, &$form_state) {
  $preset = ctools_export_new_object('spaces_presets');
  $preset->name = $form_state['values']['name'];
  $preset->title = $form_state['values']['title'];
  $preset->space_type = $form_state['values']['space_type'];
  spaces_preset_save($preset);
}

/**
 * Name validator for preset creation.
 */
function spaces_preset_name_validate($element, &$form_state) {
  if (isset($form_state['clicked_button']['#submit']) && in_array('spaces_preset_list_new', $form_state['clicked_button']['#submit'])) {
    ctools_export_ui_edit_name_validate($element, $form_state);
  }
}

/**
 * Preset editor form.
 */
function spaces_preset_editor(&$form, &$form_state) {
  $preset = $form_state['item'];

  $form['info']['#type'] = 'fieldset';

  $form['info']['title']= array(
    '#type' => 'textfield',
    '#title' => t('Title'),
    '#default_value' => $preset->title,
    '#maxlength' => 64,
  );
  $form['info']['description']= array(
    '#type' => 'textfield',
    '#title' => t('Description'),
    '#default_value' => $preset->description,
  );
  $types = array();
  foreach (spaces_types(TRUE) as $type => $info) {
    $types[$type] = $info['title'];
  }
  $form['info']['space_type'] = array(
    '#title' => t('Type'),
    '#type' => 'select',
    '#options' => $types,
    '#default_value' => $preset->space_type,
    '#disabled' => ($form_state['op'] === 'edit')
  );

  $space = spaces_load($preset->space_type, 0);
  $stack = array('preset');
  if ($space && !empty($preset->value)) {
    foreach ($preset->value as $controller => $overrides) {
      if (!empty($overrides) && is_array($overrides)) {
        foreach ($overrides as $key => $value) {
          $form['revert'][$controller][$key] = array(
            '#type' => 'checkbox',
            '#title' => $key,
            '#disabled' => FALSE,
            '#description' => $space->controllers->{$controller}->summary($key, $value),
          );
        }
      }
    }
    if (element_children($form['revert'])) {
      $form['revert']['#tree'] = TRUE;
      $form['revert']['#theme'] = 'spaces_overrides_form';
      $form['revert']['revert'] = array(
        '#type' => 'item',
        '#title' => t('Remove'),
        '#description' => t('Remove the selected overrides from this preset.'),
      );
      $form['revert']['revert']['revert'] = array(
        '#type' => 'submit',
        '#value' => t('Remove overrides'),
        '#submit' => array('spaces_preset_editor_remove_overrides'),
      );
    }
  }
  return $form;
}

/**
 * Submit handler for removing specific preset overrides.
 */
function spaces_preset_editor_remove_overrides(&$form, &$form_state) {
  foreach (array_keys(spaces_controllers()) as $controller) {
    if (!empty($form_state['item']->value[$controller])) {
      $remove = !empty($form_state['values']['revert'][$controller]) ? array_filter($form_state['values']['revert'][$controller]) : array();
      $form_state['item']->value[$controller] = array_diff_key($form_state['item']->value[$controller], $remove);
    }
  }
}

/**
 * Submit handler for preset editor.
 */
function spaces_preset_editor_submit(&$form, &$form_state) {
  $form_state['item']->name = $form_state['values']['name'];
  $form_state['item']->title = $form_state['values']['title'];
  $form_state['item']->description = $form_state['values']['description'];
  $form_state['item']->space_type = $form_state['values']['space_type'];
}

/**
 * Theme function for spaces_presets_list_new().
 */
function theme_spaces_preset_list_new($form) {
  drupal_add_css(drupal_get_path('module', 'spaces') .'/spaces.css');
  $row = $header = array();
  foreach (element_children($form) as $key) {
    if (isset($form[$key]['#title'])) {
      $header[] = $form[$key]['#title'];
      unset($form[$key]['#title']);
    }
    else {
      $header[] = '';
    }
    $row[] = drupal_render($form[$key]);
  }
  $output = theme('table', $header, array($row));
  $output .= drupal_render($form);
  return $output;
}
