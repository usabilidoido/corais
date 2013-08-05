<?php

/**
 * Implementation of hook_context_load_alter().
 */
function spaces_context_load_alter(&$context) {
  $space = spaces_get_space();
  if ($space) {
    foreach (array_keys(context_reactions()) as $plugin) {
      $override = $space->controllers->context->get("{$context->name}:reaction:{$plugin}");
      if (!is_null($override)) {
        $context->reactions[$plugin] = $override;
      }
    }
  }
}

/**
 * Implementation of hook_form_alter() for system_site_information_settings.
 */
function spaces_form_system_site_information_settings_alter(&$form, &$form_state) {
  $form['site_frontpage']['#disabled'] = TRUE;
  $form['site_frontpage']['#value'] = $form['site_frontpage']['#default_value'];
  $form['site_frontpage']['#description'] = t('The front page is controlled by the spaces module and can be set !link.', array('!link' => l(t('here'), 'features')));
}

/**
 * Implementation of hook_form_alter() for context_ui_context_editor.
 */
function spaces_form_context_ui_editor_alter(&$form, &$form_state) {
  $space = spaces_get_space();
  if ($space && isset($form['buttons']['save']['#submit'])) {
    $form['buttons']['save']['#submit'] = array('spaces_context_ui_editor_submit');
    // Context layouts adds another button which needs treatment.
    if (isset($form['contexts'])) {
      foreach (element_children($form['contexts']) as $context) {
        if (isset($form['contexts'][$context]['reaction-block']['layout'])) {
          $form['contexts'][$context]['reaction-block']['layout']['update']['#submit'] = array(
            'spaces_context_ui_editor_submit',
          );
        }
      }
    }
  }
}

/**
 * Save handler for context_block_editor().
 */
function spaces_context_ui_editor_submit(&$form, &$form_state) {
  $space = spaces_get_space();
  if ($space) {
    $saved = FALSE;
    foreach ($form_state['values']['contexts'] as $name => $values) {
      $original = drupal_clone($values['context']);
      $context = context_ui_editor_process($values);
      foreach (array_keys(context_conditions(TRUE)) as $condition) {
        if ($context->conditions[$condition] != $original->conditions[$condition]) {
          $space->controllers->context->set("$context->name:condition:$condition", $context->conditions[$condition]);
          $saved = TRUE;
        }
      }
      foreach (array_keys(context_reactions(TRUE)) as $reaction) {
        if ($context->reactions[$reaction] != $original->reactions[$reaction]) {
          $space->controllers->context->set("$context->name:reaction:$reaction", $context->reactions[$reaction]);
          $saved = TRUE;
        }
      }
    }
    if ($saved) {
      drupal_set_message(t('Saved %title for %space.', array(
        '%title' =>  (!empty($context->description) ? $context->description : $context->name),
        '%space' => $space->title()
      )));
    }
  }
}

/**
 * Form alter for the system_module form that adds a submit handler to clear
 * the space registry.
 */
function spaces_form_system_modules_alter(&$form, &$form_state) {
  $form['#submit'][] = 'spaces_form_system_modules_submit';
}

/**
 * Submit handler for clearing the space registry.
 */
function spaces_form_system_modules_submit($form, &$form_state) {
  cache_clear_all('spaces_types', 'cache');
}

/**
 * Implementation of hook_form_alter().
 */
function spaces_form_alter(&$form, &$form_state, $form_id) {
  if ($space = spaces_get_space()){

    // Add our submit handler to system settings forms
    if (!empty($form['#submit'])) {
      $position = array_search('system_settings_form_submit', $form['#submit'], TRUE);
      if ($position !== FALSE) {
        $form['#submit'][$position] = 'spaces_system_settings_form_submit';

        // For administrators, let them save to either the space or the preset,
        // unless it looks like the buttons may have already been altered.
        if (count(element_children($form['buttons'])) == 2 &&
            spaces_access_admin_perms(array('administer spaces'))
        ) {
          if ($preset = $space->controllers->variable->get('spaces_preset_' . $space->type)) {
            $preset = spaces_preset_load($preset, $space->type);
            $form['buttons']['preset'] = array(
              '#type' => 'submit',
              '#value' => t('Save to @title', array('@title' => $preset->title)),
            );
            $form['buttons']['submit']['#value'] = t('Save for @name', array(
              '@name' => $space->title()));
            $form['buttons']['reset']['#weight'] = 100;
          }
        }
      }
    }

    // Add the #space value to all forms generated in a space to enable non-
    // purl space-types to handle AHAH callbacks. @see spaces_ahah_check()
    $form['#space'] = array(
      '#type' => 'value',
      '#value' => $space->type .':'. $space->id,
    );
  }
}

/**
 * Form submit handler for system settings form for space-specific overrides.
 * Similar to system_settings_form_submit() but saves override values to space.
 */
function spaces_system_settings_form_submit($form, &$form_state) {
  if ($space = spaces_get_space()) {
    $op = isset($form_state['values']['op']) ? $form_state['values']['op'] : '';

    // Exclude unnecessary elements.
    unset($form_state['values']['submit'], $form_state['values']['reset'], $form_state['values']['preset'], $form_state['values']['form_id'], $form_state['values']['op'], $form_state['values']['form_token'], $form_state['values']['form_build_id']);


    if ($preset = $space->controllers->variable->get('spaces_preset_' . $space->type)) {
      $preset = spaces_preset_load($preset, $space->type);
    }

    foreach ($form_state['values'] as $key => $value) {
      if ($op == t('Reset to defaults')) {
        // variable_del($key);
        $space->controllers->variable->del($key);
      }
      elseif($preset && $op == t('Save to @title', array('@title' => $preset->title))) {
        $space->controllers->variable->del($key);
        $preset->value['variable'][$key] = $value;
      }
      else {
        if (is_array($value) && isset($form_state['values']['array_filter'])) {
          $value = array_keys(array_filter($value));
        }
        // variable_set($key, $value);
        // @TODO: Implement selective saving -- e.g. only save if
        // differs from parent value (preset first, then check original).
        $space->controllers->variable->set($key, $value);
      }
    }
    if ($op == t('Reset to defaults')) {
      drupal_set_message(t('The configuration options have been reset to their default values for %space.', array('%space' => $space->title())));
    }
    elseif($preset && $op == t('Save to @title', array('@title' => $preset->title))) {
      drupal_set_message(t('The configuration options have been saved for %title.', array('%title' => $preset->title)));
      spaces_preset_save($preset);
    }
    else {
      drupal_set_message(t('The configuration options have been saved for %space.', array('%space' => $space->title())));
    }
  }
}
