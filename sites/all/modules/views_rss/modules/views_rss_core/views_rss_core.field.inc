<?php

/**
 * @file
 * Field formatters for Views RSS: Core Elements module.
 */

/**
 * Implementation of hook_field_formatter_info().
 */
function views_rss_core_field_formatter_info() {
  $formatters = array(
    // Generic image formatter for <enclosure> element.
    'enclosure_image' => array(
      'label' => t('RSS <enclosure> element: image: original size'),
      'field types' => array('image', 'filefield'),
    ),
  );
  // Additional formatters for imagecache module.
  if (module_exists('imagecache')) {
    foreach (imagecache_presets() as $preset) {
      // Imagecache image formatter for <enclosure> element.
      $formatters['enclosure_image' . $preset['presetname']] = array(
        'label' => t('RSS <enclosure> element: image: @preset', array('@preset' => $preset['presetname'])),
        'field types' => array('image', 'filefield'),
      );
    }
  }
  // Generic file formatter for <enclosure> element.
  $formatters['enclosure_file'] = array(
    'label' => t('RSS <enclosure> element: file'),
    'field types' => array('filefield'),
  );
  return $formatters;
}

/**
 * No, this is not an implementation of hook_field_formatter_view().
 * This hook does not exist in D6. This function only more or less simulates it,
 * but in reality it is just a standard theme callback.
 */
function views_rss_core_field_formatter_view($variables) {

  // Inside a view $element may contain NULL data. In that case, just return.
  if (empty($variables['#item']['fid'])) {
    return;
  }

  // Is this an image formatter?
  $uri = NULL;
  // #formatter could have values like "enclosure_image<preset_name>".
  if (strpos($variables['#formatter'], 'enclosure_image') !== FALSE) {
    // Get full image URL based on provided imagecache preset.
    $preset_name = str_replace('enclosure_image', '', $variables['#formatter']);
    if (!empty($preset_name) && $preset = imagecache_preset_by_name($preset_name)) {
      $uri = imagecache_create_url($preset_name, $variables['#item']['filepath']);
      // Get file size of image preset file (if it has already been created,
      // otherwise just create it first and then get file size).
      $path = imagecache_create_path($preset_name, $variables['#item']['filepath']);
      $real_path = realpath($path);
      if (file_exists($real_path) || imagecache_build_derivative($preset['actions'], $variables['#item']['filepath'], $path)) {
        $variables['#item']['filesize'] = filesize($real_path);
      }
    }
  }
  if (empty($uri)) {
    $uri = url($variables['#item']['filepath'], array('absolute' => TRUE));
  }

  // XML element array in format_xml_elements() format.
  $rss_element = array(
    'key' => 'enclosure',
    'attributes' => array(
      'url' => $uri,
      'length' => $variables['#item']['filesize'],
      'type'   => $variables['#item']['filemime'],
    ),
  );
  
  // Views do not pass #type variable to this theme function,
  // so we should be safe to assume that if this variable exists,
  // we are displaying the field on a normal page, and in such case 
  // return only XML markup instead of extended element array.
  if (isset($variables['#type']) && $variables['#type'] == 'markup') {
    return format_xml_elements(array($rss_element));
  }

  // In all other cases return fully-fledged element array. Drupal by default
  // still will not be able to parse it properly (even with help of Views),
  // but this will be managed by our views_rss_handler_field_multiple
  // extending standard CCK's content_handler_field_multiple.
  $element = array(
    '#item' => $variables['#item'],
    '#markup' => format_xml_elements(array($rss_element)),
    '#rss_element' => $rss_element,
  );
  return $element;
}