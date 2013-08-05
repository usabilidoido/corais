<?php

/**
 * System settings form for custom text strings.
 */
function spaces_customtext_settings_form($form_state, $feature) {
  global $language;
  $langcode = $language->language;
  $custom_strings = variable_get("locale_custom_strings_{$langcode}", array());

  $strings = spaces_customtext_feature_strings($feature);
  if (!empty($strings)) {
    foreach ($strings as $original) {
      // Start with the feature name.
      $form[$original] = array(
        '#type' => 'textfield',
        '#title' => check_plain($original),
        '#default_value' => isset($custom_strings[$original]) ? $custom_strings[$original] : '',
      );
    }
    $master["locale_custom_strings_{$langcode}"] = $form;
    $master["locale_custom_strings_{$langcode}"]['#tree'] = TRUE;
    $master["locale_custom_strings_{$langcode}"]['#theme'] = 'spaces_customtext_settings_form';
    $master["locale_custom_strings_{$langcode}"]['#element_validate'] = array('spaces_customtext_settings_validate');
    $master["locale_custom_strings_{$langcode}"]['#custom_strings'] = $custom_strings;
    $master = system_settings_form($master);
    return $master;
  }
  drupal_not_found();
  exit;
}

/**
 * Validation handler for custom strings. Actually acts as a form processor.
 */
function spaces_customtext_settings_validate(&$element, &$form_state) {
  $name = end($element['#parents']);

  // If "Reset" was hit, we only want to reset values for this particular
  // feature. Yank this feature's strings and preserve the rest by switching
  // $op to 'Save configuration'.
  $op = isset($form_state['values']['op']) ? $form_state['values']['op'] : '';
  if ($op == t('Reset to defaults')) {
    $form_state['values'][$name] = array_diff_key($element['#custom_strings'], $form_state['values'][$name]);
    $form_state['values']['op'] = t('Save configuration');
  }
  // Merge this feature's string customizations into the existing string overrides.
  else {
    $form_state['values'][$name] = array_merge($element['#custom_strings'], $form_state['values'][$name]);
    $form_state['values'][$name] = array_filter($form_state['values'][$name], 'trim');
  }
}

/**
 * Theme function for customtext fields.
 */
function theme_spaces_customtext_settings_form($element) {
  drupal_add_css(drupal_get_path('module', 'spaces_customtext') .'/spaces_customtext.css');
  $rows = array();
  foreach (element_children($element) as $key) {
    $label = $element[$key]['#title'];
    unset($element[$key]['#title']);
    $rows[] = array($label, drupal_render($element[$key]));
  }
  $output = theme('table', array(t('Original'), t('Customized')), $rows, array('class' => 'spaces-customtext'));
  $output .= drupal_render($element);
  return $output;
}

/**
 * Retrieve strings related to the given feature.
 */
function spaces_customtext_feature_strings($feature) {
  global $language;
  $langcode = $language->language;

  $features = spaces_features(spaces_get_space() ? spaces_get_space()->type : 'site');
  $strings = array();

  if (isset($features[$feature])) {
    // We want to get the original string values for any object retrieval we
    // do, so use a fake language code temporarily. Note that any
    // retrieval must clear any caches (static or db) that may have t()'d
    // the original source string.
    $language->language = 'spaces_customtext';

    $feature = $features[$feature];

    // Feature name.
    $strings[] = $feature->info['name'];

    // Node types.
    if ($map = features_get_component_map('node')) {
      foreach ($map as $node_type => $component_features) {
        if (in_array($feature->name, $component_features)) {
          $strings[] = node_get_types('name', $node_type, TRUE);
        }
      }
    }

    // Views.
    if ($map = features_get_component_map('views')) {
      foreach ($map as $view_name => $component_features) {
        if (in_array($feature->name, $component_features) && $view = views_get_view($view_name, TRUE)) {
          foreach (array_keys($view->display) as $display_id) {
            $view->set_display($display_id);
            $strings[] = $view->display_handler->get_option('title');
          }
          $view->destroy();
        }
      }
    }

    // hook_menu().
    if (module_hook($feature->name, 'menu')) {
      $items = module_invoke($feature->name, 'menu');
      foreach ($items as $path => $item) {
        // Don't pollute options with administrative page strings.
        if (isset($item['title']) && (strpos($path, 'features/') === FALSE) && (strpos($path, 'admin/') === FALSE)) {
          $strings[] = $item['title'];
        }
      }
    }

    // Localize the original strings. This will use translations of the
    // original strings if available.
    $localized = array();
    foreach ($strings as $string) {
      $localized[] = module_exists('locale') ? locale($string, $langcode) : $string;
    }

    // Put the actual language code back.
    $language->language = $langcode;
  }
  return $strings;
}
