<?php

/**
 * @file
 * Definition of
 */

$plugin = array(
  'name' => t('Imagecache field'),
  'handler_name' => 'content_handler_field_multiple',
  'callback' => 'timeline_image_source_imagefield_path',
);

function timeline_image_source_imagefield_path($field, $view) {
  if ($filepath = db_fetch_object(db_query('SELECT filepath FROM {files} WHERE fid = %d', $field['fid']))->filepath) {
    return $filepath;
  }
  elseif (isset($view->field[$field['name']]->content_field['widget']['default_image']['filepath'])) {
    return $view->field[$field['name']]->content_field['widget']['default_image']['filepath'];
  }
}
