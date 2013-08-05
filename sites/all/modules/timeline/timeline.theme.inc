<?php
/**
 * @file
 * Theme functions for timeline.
 */

/**
 * Implementation of template_process for views-view-timeline.
 */
function template_preprocess_views_view_timeline(&$vars) {
  if (isset($vars['options']['debug'])) {
    $vars['debug'] = theme('timeline_debug', $vars['options']['debug']);
  }
  // check if data is available
  if ($vars['options']['timeline']['data_count'] > 0) {
    // create actual timeline
    if (_timeline_include($vars['options']['timeline']['theme'])) {

      // preprocess css information for the template
      $vars['id'] = $vars['options']['timeline']['class'];
      $vars['class'] = $vars['options']['timeline']['class'];
      $vars['height'] = $vars['options']['timeline']['height'];
      $vars['width'] = $vars['options']['timeline']['width'];
      $vars['controls'] = $vars['options']['misc']['controls'];
      $vars['timeline_theme'] = $vars['options']['timeline']['theme'];
    }
  }
  else {
    theme('timeline_nodata');
  }
}


/**
 * Overridable theme function.
 * This functions prints a warning when no data is available.
 */
function theme_timeline_nodata() {
  drupal_set_message(t('There is no data available to show on the timeline'), 'warning');
}

/**
 * Overridable theme function.
 * This returns the title of each event bubble on the timeline.
 */
function theme_timeline_bubble_title($row, $alias) {
  return $row->$alias;
}


/**
 * Overridable theme function.
 * This creates the url that the title of each event bubble is linked to.
 */
function theme_timeline_bubble_url($row, $link_enabled) {
  if ($link_enabled) {
    return base_path() . drupal_get_path_alias('node/' . $row->nid);
  }
  return FALSE;
}

/*
 * Function to theme a textarea with a array inside
 */
function theme_timeline_debug($debug_array) {
  $output = '<div id="timeline_debug">';
  $output .= '<label><strong>DEBUG:</strong></label>';
  $output .= '<div class="resizable-textarea"><textarea id="edit-code" class="form-textarea resizable textarea-processed" name="code" rows ="10" cols="60">';
  $output .= var_export($debug_array, TRUE);
  $output .= '</textarea></div></div>';
  return $output;
}

/**
 * Inlude all js and css files that needs the timeline.
 *
 * @return: TRUE if everything could be included properly
 *          FALSE if Simile Exhibit Libraries couldn't be included properly
 */
function _timeline_include($timeline_theme) {
  static $loaded = FALSE;
  $timeline_mod_path = drupal_get_path('module', 'timeline');

  if (!$loaded) {

    // Check correct inclusion of local timeline library
    if ($timeline_library_status = timeline_validate_library()) {

      // some error messages are available, because of detected errors during validation
      if (is_string($timeline_library_status)) {
        drupal_set_message(check_plain($timeline_library_status));
        return FALSE;
      }

      // timeline library has been validated and will be added
      elseif ($timeline_library_status == TRUE) {

        $timeline_lib_path = libraries_get_path('simile_timeline');
        drupal_add_js(file_directory_path() . '/timeline/local_variables.js');
        drupal_add_js($timeline_lib_path . '/timeline_js/timeline-api.js');

        // include custom theme files (if activated)
        if ($timeline_theme != 'classic_theme' && $timeline_theme != FALSE) {
          ctools_include('plugins');
          $timeline_themes = ctools_get_plugins('timeline', 'themes');
          if (isset($timeline_themes[$timeline_theme])) {
            drupal_add_js($timeline_themes[$timeline_theme]['files']['js']);
            drupal_add_css($timeline_themes[$timeline_theme]['files']['css']);
          }
        }

        ctools_add_css('timeline', 'timeline');
        ctools_add_js('timeline', 'timeline');
        $loaded = TRUE;

        return TRUE;
      }
    }
  drupal_set_message(t('Simile Timeline Libraries not found'), 'error');
  return FALSE;
  }
}
