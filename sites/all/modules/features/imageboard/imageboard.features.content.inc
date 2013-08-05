<?php

/**
 * Implementation of hook_content_default_fields().
 */
function imageboard_content_default_fields() {
  $fields = array();

  // Exported field: field_imageboard
  $fields['images-field_imageboard'] = array(
    'field_name' => 'field_imageboard',
    'type_name' => 'images',
    'display_settings' => array(
      'weight' => '31',
      'parent' => '',
      'token' => array(
        'format' => 'imageboard_preview_imagelink',
        'exclude' => 0,
      ),
      'label' => array(
        'format' => 'hidden',
      ),
      'teaser' => array(
        'format' => 'imageboard-thumb_linked',
        'exclude' => 0,
      ),
      'full' => array(
        'format' => 'imageboard_preview_imagelink',
        'exclude' => 0,
      ),
      '5' => array(
        'format' => 'default',
        'exclude' => 0,
      ),
      '4' => array(
        'format' => 'default',
        'exclude' => 0,
      ),
      '2' => array(
        'format' => 'default',
        'exclude' => 0,
      ),
      '3' => array(
        'format' => 'default',
        'exclude' => 0,
      ),
    ),
    'widget_active' => '1',
    'type' => 'filefield',
    'required' => '1',
    'multiple' => '1',
    'module' => 'filefield',
    'active' => '1',
    'list_field' => '0',
    'list_default' => 1,
    'description_field' => '0',
    'widget' => array(
      'file_extensions' => 'png gif jpg jpeg',
      'file_path' => '',
      'progress_indicator' => 'bar',
      'max_filesize_per_file' => '',
      'max_filesize_per_node' => '',
      'max_resolution' => '0',
      'min_resolution' => '0',
      'alt' => '',
      'custom_alt' => 0,
      'title' => '',
      'custom_title' => 1,
      'title_type' => 'textfield',
      'default_image' => NULL,
      'use_default_image' => 0,
      'fupload_mode' => 'multiple',
      'fupload_previewlist_img_attributes' => '',
      'fupload_title_replacements' => '_;{;};-',
      'fupload_previewlist_field_settings' => array(
        'imagefield_title' => 'imagefield_title',
        'node_title' => 0,
        'node_description' => 0,
        'imagefield_alt' => 0,
        'imagefield_description' => 0,
      ),
      'fupload_previewlist_redirecturl' => '',
      'label' => 'Imagens',
      'weight' => '31',
      'description' => '',
      'type' => 'image_fupload_imagefield_widget',
      'module' => 'image_fupload_imagefield',
    ),
  );

  // Translatables
  // Included for use with string extractors like potx.
  t('Imagens');

  return $fields;
}
