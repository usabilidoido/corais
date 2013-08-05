<?php
// $Id: stickers.cck_import.inc,v 1.1.2.2 2010/10/16 07:40:36 Countzero Exp $

/**
 * @file
 * A CCK content type for the Stickers module
 * This file will be used by install_profile_api to create the basic content type
 */

$content['type'] = array(
  'name' => 'Sticker',
  'type' => 'sticker',
  'description' => '',
  'title_label' => 'Name',
  'body_label' => '',
  'min_word_count' => '0',
  'help' => '',
  'node_options' =>
  array(
    'status' => TRUE,
    'promote' => FALSE,
    'sticky' => FALSE,
    'revision' => FALSE,
  ),
  'language_content_type' => '0',
  'upload' => '0',
  'old_type' => 'sticker',
  'orig_type' => '',
  'module' => 'node',
  'custom' => '1',
  'modified' => '1',
  'locked' => '1',
  'comment' => '0',
  'comment_default_mode' => '4',
  'comment_default_order' => '1',
  'comment_default_per_page' => '50',
  'comment_controls' => '3',
  'comment_anonymous' => 0,
  'comment_subject_field' => '1',
  'comment_preview' => '1',
  'comment_form_location' => '0',
);
$content['fields']  = array(
  0 =>
  array(
    'label' => 'Image',
    'field_name' => 'field_article_image',
    'type' => 'filefield',
    'widget_type' => 'imagefield_widget',
    'change' => 'Change basic information',
    'weight' => '-4',
    'file_extensions' => 'png gif jpg jpeg',
    'progress_indicator' => 'bar',
    'file_path' => '',
    'max_filesize_per_file' => '10M',
    'max_filesize_per_node' => '',
    'max_resolution' => 0,
    'min_resolution' => 0,
    'custom_alt' => 0,
    'alt' => '',
    'custom_title' => 0,
    'title_type' => 'textfield',
    'title' => '',
    'use_default_image' => 0,
    'default_image_upload' => '',
    'default_image' => '',
    'description' => '',
    'group' => FALSE,
    'required' => 0,
    'multiple' => '0',
    'list_field' => '0',
    'list_default' => 1,
    'description_field' => '0',
    'op' => 'Save field settings',
    'module' => 'filefield',
    'widget_module' => 'imagefield',
    'columns' =>
    array(
      'fid' =>
      array(
        'type' => 'int',
        'not null' => FALSE,
        'views' => TRUE,
      ),
      'list' =>
      array(
        'type' => 'int',
        'size' => 'tiny',
        'not null' => FALSE,
        'views' => TRUE,
      ),
      'data' =>
      array(
        'type' => 'text',
        'serialize' => TRUE,
        'views' => TRUE,
      ),
    ),
    'display_settings' =>
    array(
      'weight' => '-4',
      'parent' => '',
      'label' =>
      array(
        'format' => 'above',
      ),
      'teaser' =>
      array(
        'format' => 'sticker_default',
        'exclude' => 0,
      ),
      'full' =>
      array(
        'format' => 'sticker_default',
        'exclude' => 0,
      ),
      4 =>
      array(
        'format' => 'image_plain',
        'exclude' => 0,
      ),
      2 =>
      array(
        'format' => 'image_plain',
        'exclude' => 0,
      ),
      3 =>
      array(
        'format' => 'image_plain',
        'exclude' => 0,
      ),
      'token' =>
      array(
        'format' => 'image_plain',
        'exclude' => 0,
      ),
    ),
  ),
  1 =>
  array(
    'label' => 'Preset',
    'field_name' => 'field_preset',
    'type' => 'text',
    'widget_type' => 'optionwidgets_select',
    'change' => 'Change basic information',
    'weight' => '-3',
    'description' => '',
    'default_value' =>
    array(
      0 =>
      array(
        'value' => '',
      ),
    ),
    'default_value_php' => '',
    'default_value_widget' =>
    array(
      'field_preset' =>
      array(
        'value' => '',
      ),
    ),
    'group' => FALSE,
    'required' => 1,
    'multiple' => '0',
    'text_processing' => '0',
    'max_length' => '',
    'allowed_values' => '',
    'allowed_values_php' => '
        $presets = imagecache_presets();
        foreach ($presets as $pid => $preset) {
            if (!(strpos($preset["presetname"], "sticker") === false) )  {
                $list_presets[$preset["presetname"]] = $preset["presetname"];
            }
        }
        return $list_presets;',
    'op' => 'Save field settings',
    'module' => 'text',
    'widget_module' => 'optionwidgets',
    'columns' =>
    array(
      'value' =>
      array(
        'type' => 'text',
        'size' => 'big',
        'not null' => FALSE,
        'sortable' => TRUE,
        'views' => TRUE,
      ),
    ),
    'display_settings' =>
    array(
      'label' =>
      array(
        'format' => 'above',
        'exclude' => 0,
      ),
      'teaser' =>
      array(
        'format' => 'default',
        'exclude' => 0,
      ),
      'full' =>
      array(
        'format' => 'default',
        'exclude' => 0,
      ),
      4 =>
      array(
        'format' => 'default',
        'exclude' => 0,
      ),
      'email_plain' =>
      array(
        'format' => 'default',
        'exclude' => 0,
      ),
      'email_html' =>
      array(
        'format' => 'default',
        'exclude' => 0,
      ),
      'token' =>
      array(
        'format' => 'default',
        'exclude' => 0,
      ),
    ),
  ),
  2 =>
  array(
    'label' => 'Horizontal position',
    'field_name' => 'field_x',
    'type' => 'number_integer',
    'widget_type' => 'number',
    'change' => 'Change basic information',
    'weight' => '-2',
    'description' => '',
    'default_value' =>
    array(
      0 =>
      array(
        'value' => '0',
        '_error_element' => 'default_value_widget][field_x][0][value',
      ),
    ),
    'default_value_php' => '',
    'default_value_widget' =>
    array(
      'field_x' =>
      array(
        0 =>
        array(
          'value' => '0',
          '_error_element' => 'default_value_widget][field_x][0][value',
        ),
      ),
    ),
    'group' => FALSE,
    'required' => 1,
    'multiple' => '0',
    'min' => '0',
    'max' => '900',
    'prefix' => '',
    'suffix' => '',
    'allowed_values' => '',
    'allowed_values_php' => '',
    'op' => 'Save field settings',
    'module' => 'number',
    'widget_module' => 'number',
    'columns' =>
    array(
      'value' =>
      array(
        'type' => 'int',
        'not null' => FALSE,
        'sortable' => TRUE,
      ),
    ),
    'display_settings' =>
    array(
      'weight' => '-3',
      'parent' => '',
      'label' =>
      array(
        'format' => 'above',
      ),
      'teaser' =>
      array(
        'format' => 'default',
        'exclude' => 0,
      ),
      'full' =>
      array(
        'format' => 'default',
        'exclude' => 0,
      ),
      4 =>
      array(
        'format' => 'default',
        'exclude' => 0,
      ),
      'email_plain' =>
      array(
        'format' => 'default',
        'exclude' => 0,
      ),
      'email_html' =>
      array(
        'format' => 'default',
        'exclude' => 0,
      ),
      'token' =>
      array(
        'format' => 'default',
        'exclude' => 0,
      ),
    ),
  ),
  3 =>
  array(
    'label' => 'Vertical position',
    'field_name' => 'field_y',
    'type' => 'number_integer',
    'widget_type' => 'number',
    'change' => 'Change basic information',
    'weight' => '-1',
    'description' => '',
    'default_value' =>
    array(
      0 =>
      array(
        'value' => '0',
        '_error_element' => 'default_value_widget][field_y][0][value',
      ),
    ),
    'default_value_php' => '',
    'default_value_widget' =>
    array(
      'field_y' =>
      array(
        0 =>
        array(
          'value' => '0',
          '_error_element' => 'default_value_widget][field_y][0][value',
        ),
      ),
    ),
    'group' => FALSE,
    'required' => 1,
    'multiple' => '0',
    'min' => '0',
    'max' => '900',
    'prefix' => '',
    'suffix' => '',
    'allowed_values' => '',
    'allowed_values_php' => '',
    'op' => 'Save field settings',
    'module' => 'number',
    'widget_module' => 'number',
    'columns' =>
    array(
      'value' =>
      array(
        'type' => 'int',
        'not null' => FALSE,
        'sortable' => TRUE,
      ),
    ),
    'display_settings' =>
    array(
      'weight' => '-2',
      'parent' => '',
      'label' =>
      array(
        'format' => 'above',
      ),
      'teaser' =>
      array(
        'format' => 'default',
        'exclude' => 0,
      ),
      'full' =>
      array(
        'format' => 'default',
        'exclude' => 0,
      ),
      4 =>
      array(
        'format' => 'default',
        'exclude' => 0,
      ),
      'email_plain' =>
      array(
        'format' => 'default',
        'exclude' => 0,
      ),
      'email_html' =>
      array(
        'format' => 'default',
        'exclude' => 0,
      ),
      'token' =>
      array(
        'format' => 'default',
        'exclude' => 0,
      ),
    ),
  ),
);
$content['extra']  = array(
  'title' => '-5',
  'revision_information' => '1',
  'author' => '2',
  'options' => '3',
  'comment_settings' => '4',
  'menu' => '0',
  'path' => '5',
);