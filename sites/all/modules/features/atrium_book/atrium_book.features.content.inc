<?php

/**
 * Implementation of hook_content_default_fields().
 */
function atrium_book_content_default_fields() {
  $fields = array();

  // Exported field: field_card_description
  $fields['book-field_card_description'] = array(
    'field_name' => 'field_card_description',
    'type_name' => 'book',
    'display_settings' => array(
      'weight' => '2',
      'parent' => 'group_card',
      'label' => array(
        'format' => 'above',
      ),
      'teaser' => array(
        'format' => 'hidden',
        'exclude' => 0,
      ),
      'full' => array(
        'format' => 'hidden',
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
      'token' => array(
        'format' => 'default',
        'exclude' => 0,
      ),
    ),
    'widget_active' => '1',
    'type' => 'text',
    'required' => '0',
    'multiple' => '0',
    'module' => 'text',
    'active' => '1',
    'text_processing' => '0',
    'max_length' => '400',
    'allowed_values' => '',
    'allowed_values_php' => '',
    'reference_preview' => '',
    'widget' => array(
      'rows' => '3',
      'size' => 60,
      'default_value' => array(
        '0' => array(
          'value' => '',
          '_error_element' => 'default_value_widget][field_card_description][0][value',
        ),
      ),
      'default_value_php' => NULL,
      'label' => 'Card Description',
      'weight' => '2',
      'description' => 'A very synthetic description of the knowledge.',
      'type' => 'text_textarea',
      'module' => 'text',
    ),
  );

  // Exported field: field_images
  $fields['book-field_images'] = array(
    'field_name' => 'field_images',
    'type_name' => 'book',
    'display_settings' => array(
      'weight' => '9',
      'parent' => '',
      'label' => array(
        'format' => 'above',
      ),
      'teaser' => array(
        'format' => 'image_plain',
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
        'imagecache_imageboard-thumb' => 'imagecache_imageboard-thumb',
        'imagecache_imageboard_preview' => 'imagecache_imageboard_preview',
        'auto' => 0,
        'image' => 0,
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
      'weight' => '10',
      'description' => '',
      'type' => 'imagefield_widget',
      'module' => 'imagefield',
    ),
  );

  // Exported field: field_input
  $fields['book-field_input'] = array(
    'field_name' => 'field_input',
    'type_name' => 'book',
    'display_settings' => array(
      'weight' => '1',
      'parent' => 'group_card',
      'label' => array(
        'format' => 'hidden',
      ),
      'teaser' => array(
        'format' => 'hidden',
        'exclude' => 1,
      ),
      'full' => array(
        'format' => 'link',
        'exclude' => 1,
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
      'token' => array(
        'format' => 'default',
        'exclude' => 0,
      ),
    ),
    'widget_active' => '1',
    'type' => 'content_taxonomy',
    'required' => '0',
    'multiple' => '1',
    'module' => 'content_taxonomy',
    'active' => '1',
    'save_term_node' => 1,
    'vid' => '6',
    'parent' => '0',
    'parent_php_code' => '',
    'depth' => '',
    'reference_preview' => '',
    'widget' => array(
      'new_terms' => 'insert',
      'extra_parent' => '0',
      'maxlength' => '255',
      'active_tags' => NULL,
      'default_value' => NULL,
      'default_value_php' => NULL,
      'label' => 'Input',
      'weight' => '1',
      'description' => 'Requirements for this knowledge.',
      'type' => 'content_taxonomy_autocomplete',
      'module' => 'content_taxonomy_autocomplete',
    ),
  );

  // Exported field: field_output
  $fields['book-field_output'] = array(
    'field_name' => 'field_output',
    'type_name' => 'book',
    'display_settings' => array(
      'weight' => '3',
      'parent' => 'group_card',
      'label' => array(
        'format' => 'hidden',
      ),
      'teaser' => array(
        'format' => 'hidden',
        'exclude' => 1,
      ),
      'full' => array(
        'format' => 'link',
        'exclude' => 1,
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
      'token' => array(
        'format' => 'default',
        'exclude' => 0,
      ),
    ),
    'widget_active' => '1',
    'type' => 'content_taxonomy',
    'required' => '0',
    'multiple' => '1',
    'module' => 'content_taxonomy',
    'active' => '1',
    'save_term_node' => 1,
    'vid' => '6',
    'parent' => '0',
    'parent_php_code' => '',
    'depth' => '',
    'reference_preview' => '',
    'widget' => array(
      'new_terms' => 'insert',
      'extra_parent' => '0',
      'maxlength' => '255',
      'active_tags' => NULL,
      'default_value' => NULL,
      'default_value_php' => NULL,
      'label' => 'Ouput',
      'weight' => '3',
      'description' => 'Things you expect to achieve by mean of this knowledge.',
      'type' => 'content_taxonomy_autocomplete',
      'module' => 'content_taxonomy_autocomplete',
    ),
  );

  // Exported field: field_phase
  $fields['book-field_phase'] = array(
    'field_name' => 'field_phase',
    'type_name' => 'book',
    'display_settings' => array(
      'weight' => '4',
      'parent' => 'group_card',
      'label' => array(
        'format' => 'inline',
      ),
      'teaser' => array(
        'format' => 'hidden',
        'exclude' => 0,
      ),
      'full' => array(
        'format' => 'link',
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
      'token' => array(
        'format' => 'default',
        'exclude' => 0,
      ),
    ),
    'widget_active' => '1',
    'type' => 'content_taxonomy',
    'required' => '1',
    'multiple' => '1',
    'module' => 'content_taxonomy',
    'active' => '1',
    'save_term_node' => 0,
    'vid' => '9',
    'parent' => '0',
    'parent_php_code' => '',
    'depth' => '',
    'reference_preview' => '',
    'widget' => array(
      'group_parent' => '0',
      'show_depth' => 1,
      'default_value' => array(
        '0' => array(
          'value' => '',
        ),
      ),
      'default_value_php' => NULL,
      'label' => 'Fases de projeto',
      'weight' => '-1',
      'description' => 'Em que fase de projeto este Conhecimento é útil?',
      'type' => 'content_taxonomy_options',
      'module' => 'content_taxonomy_options',
    ),
  );

  // Exported field: field_referenced_book_page
  $fields['book-field_referenced_book_page'] = array(
    'field_name' => 'field_referenced_book_page',
    'type_name' => 'book',
    'display_settings' => array(
      'weight' => '11',
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
      'shoutbox' => 0,
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
      'size' => 60,
      'default_value' => array(
        '0' => array(
          'nid' => '',
        ),
      ),
      'default_value_php' => NULL,
      'label' => 'Conhecimentos relacionados',
      'weight' => '12',
      'description' => 'Type in the title of another post to add a reference.',
      'type' => 'nodereference_select',
      'module' => 'nodereference',
    ),
  );

  // Exported field: field_referencia
  $fields['book-field_referencia'] = array(
    'field_name' => 'field_referencia',
    'type_name' => 'book',
    'display_settings' => array(
      'weight' => '12',
      'parent' => '',
      'label' => array(
        'format' => 'above',
      ),
      'teaser' => array(
        'format' => 'hidden',
        'exclude' => 0,
      ),
      'full' => array(
        'format' => 'default',
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
      'token' => array(
        'format' => 'default',
        'exclude' => 0,
      ),
    ),
    'widget_active' => '1',
    'type' => 'link',
    'required' => '0',
    'multiple' => '1',
    'module' => 'link',
    'active' => '1',
    'attributes' => array(
      'target' => 'default',
      'rel' => '',
      'class' => '',
      'title' => '',
    ),
    'display' => array(
      'url_cutoff' => '80',
    ),
    'url' => 0,
    'title' => 'optional',
    'title_value' => '',
    'enable_tokens' => 0,
    'validate_url' => 1,
    'reference_preview' => '',
    'widget' => array(
      'default_value' => array(
        '0' => array(
          'title' => '',
          'url' => '',
        ),
      ),
      'default_value_php' => NULL,
      'label' => 'Referências para aprofundamento',
      'weight' => '13',
      'description' => '',
      'type' => 'link',
      'module' => 'link',
    ),
  );

  // Exported field: field_logo
  $fields['service_provider-field_logo'] = array(
    'field_name' => 'field_logo',
    'type_name' => 'service_provider',
    'display_settings' => array(
      'weight' => '-4',
      'parent' => '',
      'label' => array(
        'format' => 'hidden',
      ),
      'teaser' => array(
        'format' => 'imageboard-thumb_default',
        'exclude' => 0,
      ),
      'full' => array(
        'format' => 'imageboard-thumb_default',
        'exclude' => 0,
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
    'multiple' => '0',
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
      'custom_title' => 0,
      'title_type' => 'textfield',
      'default_image' => NULL,
      'use_default_image' => 0,
      'insert' => 0,
      'insert_styles' => array(
        'auto' => 0,
        'link' => 0,
        'image' => 0,
        'imagecache_AttachmentThumbnail' => 0,
        'imagecache_designkit-image-logo' => 0,
        'imagecache_fileview' => 0,
        'imagecache_imageboard-thumb' => 0,
        'imagecache_imageboard_preview' => 0,
        'imagecache_user-m' => 0,
        'imagecache_user-s' => 0,
      ),
      'insert_default' => 'auto',
      'insert_class' => '',
      'insert_width' => '',
      'label' => 'Logo',
      'weight' => '-4',
      'description' => '',
      'type' => 'imagefield_widget',
      'module' => 'imagefield',
    ),
  );

  // Exported field: field_projects
  $fields['service_provider-field_projects'] = array(
    'field_name' => 'field_projects',
    'type_name' => 'service_provider',
    'display_settings' => array(
      'weight' => '7',
      'parent' => '',
      'label' => array(
        'format' => 'above',
      ),
      'teaser' => array(
        'format' => 'default',
        'exclude' => 0,
      ),
      'full' => array(
        'format' => 'default',
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
      'token' => array(
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
      'group' => 'group',
      'shoutbox' => 0,
      'book' => 0,
      'milestone' => 0,
      'event' => 0,
      'images' => 0,
      'graphmind' => 0,
      'patch' => 0,
      'profile' => 0,
      'sheetnode' => 0,
      'blog' => 0,
      'page' => 0,
      'webform' => 0,
      'service_provider' => 0,
      'question' => 0,
      'task' => 0,
      'text' => 0,
      'discussion' => 0,
      'poll' => 0,
      'feed_ical_item' => 0,
      'feed_ical' => 0,
    ),
    'advanced_view' => '--',
    'advanced_view_args' => '',
    'widget' => array(
      'default_value' => array(
        '0' => array(
          'nid' => '',
        ),
      ),
      'default_value_php' => NULL,
      'label' => 'Projetos no Corais',
      'weight' => '7',
      'description' => '',
      'type' => 'nodereference_autocomplete',
      'module' => 'nodereference',
    ),
  );

  // Exported field: field_referenced_book_page
  $fields['service_provider-field_referenced_book_page'] = array(
    'field_name' => 'field_referenced_book_page',
    'type_name' => 'service_provider',
    'display_settings' => array(
      'weight' => '6',
      'parent' => '',
      'label' => array(
        'format' => 'inline',
      ),
      'teaser' => array(
        'format' => 'hidden',
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
      'shoutbox' => 0,
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
      'autocomplete_match' => 'starts_with',
      'size' => '60',
      'default_value' => array(
        '0' => array(
          'nid' => NULL,
          '_error_element' => 'default_value_widget][field_referenced_book_page][0][nid][nid',
        ),
      ),
      'default_value_php' => NULL,
      'node_link' => array(
        'teaser' => 0,
        'full' => 1,
        'title' => 'Add Service Provider',
        'hover_title' => '',
        'destination' => 'default',
      ),
      'fallback' => 'select',
      'edit_fallback' => 1,
      'label' => 'Know-how',
      'weight' => '6',
      'description' => '',
      'type' => 'nodereference_autocomplete',
      'module' => 'nodereference',
    ),
  );

  // Exported field: field_website
  $fields['service_provider-field_website'] = array(
    'field_name' => 'field_website',
    'type_name' => 'service_provider',
    'display_settings' => array(
      'weight' => '9',
      'parent' => '',
      'label' => array(
        'format' => 'above',
      ),
      'teaser' => array(
        'format' => 'hidden',
        'exclude' => 0,
      ),
      'full' => array(
        'format' => 'default',
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
      'token' => array(
        'format' => 'default',
        'exclude' => 0,
      ),
    ),
    'widget_active' => '1',
    'type' => 'link',
    'required' => '0',
    'multiple' => '0',
    'module' => 'link',
    'active' => '1',
    'attributes' => array(
      'target' => 'default',
      'rel' => '',
      'class' => '',
      'title' => '',
    ),
    'display' => array(
      'url_cutoff' => '80',
    ),
    'url' => 0,
    'title' => 'none',
    'title_value' => '',
    'enable_tokens' => 0,
    'validate_url' => 1,
    'widget' => array(
      'default_value' => array(
        '0' => array(
          'title' => '',
          'url' => '',
        ),
      ),
      'default_value_php' => NULL,
      'label' => 'Website',
      'weight' => '9',
      'description' => '',
      'type' => 'link',
      'module' => 'link',
    ),
  );

  // Exported field: field_referenced_book_page
  $fields['toolkit-field_referenced_book_page'] = array(
    'field_name' => 'field_referenced_book_page',
    'type_name' => 'toolkit',
    'display_settings' => array(
      'weight' => 0,
      'parent' => '',
      'label' => array(
        'format' => 'inline',
      ),
      'teaser' => array(
        'format' => 'hidden',
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
      'shoutbox' => 0,
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
      'autocomplete_match' => 'starts_with',
      'size' => '90',
      'default_value' => array(
        '0' => array(
          'nid' => NULL,
          '_error_element' => 'default_value_widget][field_referenced_book_page][0][nid][nid',
        ),
      ),
      'default_value_php' => NULL,
      'label' => 'Toolkit',
      'weight' => '8',
      'description' => 'Type in the title of another post to add a reference.',
      'type' => 'nodereference_autocomplete',
      'module' => 'nodereference',
    ),
  );

  // Translatables
  // Included for use with string extractors like potx.
  t('Card Description');
  t('Conhecimentos relacionados');
  t('Fases de projeto');
  t('Imagens');
  t('Input');
  t('Know-how');
  t('Logo');
  t('Ouput');
  t('Projetos no Corais');
  t('Referências para aprofundamento');
  t('Toolkit');
  t('Website');

  return $fields;
}
