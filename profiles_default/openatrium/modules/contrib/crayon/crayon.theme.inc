<?php

/**
 * Preprocessor for theme('crayon_popup').
 */
function template_preprocess_crayon_popup(&$vars) {
  drupal_add_css(drupal_get_path('module', 'crayon') .'/crayon.css');

  $vars['label'] = filter_xss_admin($vars['label']);
  $vars['label_attr'] = array('class' => 'crayon-popup-label');

  if (!empty($vars['color'])) {
    $vars['attr'] = array('class' => 'crayon-swatch');
    if (crayon_is_color($vars['color'])) {
      $vars['attr']['style'] = 'background-color:' . $vars['color'];
    }
    else {
      $vars['attr']['class'] .= ' crayon-' . crayon_generate_index($vars['color']);
    }

    // Indicate that this popup includes a swatch.
    $vars['label_attr']['class'] .= ' with-swatch';
  }
}

/**
 * Preprocessor for theme('crayon_swatch').
 */
function template_preprocess_crayon_swatch(&$vars) {
  drupal_add_css(drupal_get_path('module', 'crayon') .'/crayon.css');

  $vars['attr'] = array('class' => 'crayon-anchor crayon-swatch');
  if (crayon_is_color($vars['color'])) {
    $vars['attr']['style'] = 'background-color:' . $vars['color'];
  }
  else {
    $vars['attr']['class'] .= ' crayon-' . crayon_generate_index($vars['color']);
  }

  $vars['content'] = "<span class='crayon-swatch-label'>" . filter_xss_admin($vars['label']) ."</span>";
  $vars['content'] .= theme('crayon_popup', (empty($vars['popup']) ? $vars['label'] : $vars['popup']), $vars['color']);

  if (!empty($vars['path'])) {
    $vars['options'] = array('html' => TRUE, 'attributes' => $vars['attr']) + $vars['options'];
    $vars['link'] = l($vars['content'], $vars['path'], $vars['options']);
  }
}
