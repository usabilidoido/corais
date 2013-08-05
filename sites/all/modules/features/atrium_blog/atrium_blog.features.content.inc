<?php

/**
 * Implementation of hook_content_default_fields().
 */
function atrium_blog_content_default_fields() {
  $fields = array();

  // Exported field: field_images
  $fields['blog-field_images'] = array(
    'field_name' => 'field_images',
    'type_name' => 'blog',
    'display_settings' => array(
      'weight' => '31',
      'parent' => '',
      'label' => array(
        'format' => 'above',
      ),
      'teaser' => array(
        'format' => 'imageboard-thumb_linked',
        'exclude' => 1,
      ),
      'full' => array(
        'format' => 'image_plain',
        'exclude' => 1,
      ),
      '5' => array(
        'format' => 'image_plain',
        'exclude' => 0,
      ),
      '4' => array(
        'format' => 'image_plain',
        'exclude' => 0,
      ),
      '2' => array(
        'format' => 'image_plain',
        'exclude' => 0,
      ),
      '3' => array(
        'format' => 'image_plain',
        'exclude' => 0,
      ),
      'token' => array(
        'format' => 'image_plain',
        'exclude' => 0,
      ),
    ),
    'widget_active' => '1',
    'type' => 'filefield',
    'required' => '0',
    'multiple' => '1',
    'module' => 'filefield',
    'active' => '1',
    'list_field' => '0',
    'list_default' => 1,
    'description_field' => '0',
    'reference_preview' => '',
    'widget' => array(
      'file_extensions' => 'png gif jpg jpeg',
      'file_path' => '',
      'progress_indicator' => 'bar',
      'max_filesize_per_file' => '',
      'max_filesize_per_node' => '',
      'max_resolution' => '0',
      'min_resolution' => '0',
      'alt' => '',
      'custom_alt' => 1,
      'title' => '',
      'custom_title' => 0,
      'title_type' => 'textfield',
      'default_image' => NULL,
      'use_default_image' => 0,
      'insert' => 1,
      'insert_styles' => array(
        'link' => 'link',
        'image' => 'image',
        'imagecache_imageboard-thumb' => 'imagecache_imageboard-thumb',
        'imagecache_imageboard_preview' => 'imagecache_imageboard_preview',
        'auto' => 0,
        'imagecache_AttachmentThumbnail' => 0,
        'imagecache_designkit-image-logo' => 0,
        'imagecache_fileview' => 0,
        'imagecache_user-m' => 0,
        'imagecache_user-s' => 0,
      ),
      'insert_default' => 'imagecache_imageboard_preview',
      'insert_class' => '',
      'insert_width' => '',
      'label' => 'Imagens',
      'weight' => '6',
      'description' => '',
      'type' => 'imagefield_widget',
      'module' => 'imagefield',
    ),
  );

  // Exported field: field_referenced_book_page
  $fields['blog-field_referenced_book_page'] = array(
    'field_name' => 'field_referenced_book_page',
    'type_name' => 'blog',
    'display_settings' => array(
      'weight' => 0,
      'parent' => '',
      'label' => array(
        'format' => 'inline',
      ),
      'teaser' => array(
        'format' => 'default',
        'exclude' => 0,
      ),
      'full' => array(
        'format' => 'default',
        'exclude' => 0,
      ),
    ),
    'widget_active' => '1',
    'type' => 'nodereference',
    'required' => '0',
    'multiple' => '1',
    'module' => 'nodereference',
    'active' => '1',
    'referenceable_types' => array(
      'book' => 'book',
      'milestone' => 0,
      'images' => 0,
      'graphmind' => 0,
      'mass_contact' => 0,
      'board' => 0,
      'patch' => 0,
      'profile' => 0,
      'sheetnode' => 0,
      'blog' => 0,
      'service_provider' => 0,
      'group' => 0,
      'page' => 0,
      'webform' => 0,
      'event' => 0,
      'question' => 0,
      'task' => 0,
      'text' => 0,
      'toolkit' => 0,
      'poll' => 0,
      'currency' => 0,
      'exchange' => 0,
      'feed_ical_item' => 0,
      'feed_ical' => 0,
    ),
    'advanced_view' => '--',
    'advanced_view_args' => '',
    'reference_preview' => array(
      'enable' => 1,
      'type' => 'node-teaser',
      'links' => 0,
    ),
    'widget' => array(
      'autocomplete_match' => 'contains',
      'size' => NULL,
      'default_value' => array(
        '0' => array(
          'nid' => NULL,
          '_error_element' => 'default_value_widget][field_referenced_book_page][0][nid][nid',
        ),
      ),
      'default_value_php' => NULL,
      'label' => 'Related Knowledge',
      'weight' => 0,
      'description' => 'Type in the title of another post to add a reference.',
      'type' => 'nodereference_autocomplete',
      'module' => 'nodereference',
    ),
  );

  // Translatables
  // Included for use with string extractors like potx.
  t('Imagens');
  t('Related Knowledge');

  return $fields;
}
