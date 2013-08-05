<?php

/**
 * @file
 * Definition of
 */

$plugin = array(
  'name' => t('Taxonomy image'),
  'handler_name' => 'views_handler_field_taxonomy_image',
  'callback' => 'timeline_image_source_taxonomy_image_path',
);

function timeline_image_source_taxonomy_image_path($field, $view) {

  if (module_exists('taxonomy_image')) {
    $path = file_directory_path() . '/' . variable_get('taxonomy_image_path', 'category_pictures') .'/';
    return $path . taxonomy_image_get_object($field['fid'])->path;
  }
}

