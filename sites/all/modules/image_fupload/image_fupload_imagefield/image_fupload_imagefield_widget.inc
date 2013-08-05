<?php

/**
 * @file
 * Image Fupload ImageField widget hooks and callbacks.
 *
 * This file is loaded by filefield.module/imagefield.module and the callbacks it contains are
 * invoked by filefield.module. This happens because imagefield delegates it's
 * CCK hooks and callbacks to filefield in imagefield.module.
 *
 * This file is mainly based on imagefield_widget.inc (D6 3.0-rc1)
 */
 
module_load_include('inc', 'imagefield', 'imagefield_widget');

/**
 * Implementation of hook_widget_settings($op = 'form')
 */
function image_fupload_imagefield_widget_settings_form($widget) {
  $form = module_invoke('imagefield', 'widget_settings_form', $widget);
  $node_type = str_replace("-", "_", arg(3));
  
  // warn user to only use this widget once per node type  
  if (!isset($widget['fupload_mode']))
    drupal_set_message(t("Image FUpload widget can only be used <u>once</u> per node type.<br />If this is a second field using image fupload based on the same node type, it's adviced to delete this field, reconfigure the other one and create a new one without image fupload widget."), 'warning');
    
  $form['fupload_mode'] = array(
    '#type' => 'radios',
    '#title' => t('Storage mode'),
    '#description' => t("First option creates one complete node per uploaded image. This is useful if you have to store a lot of other information beside the image (for example other CCK fields or several Taxonomy items per image).<br /> Second option stores multiple images per node. This is useful if you want to create simple galleries.<br /><b>Don't forget</b> to set <i>'Number of values'</i> to a sufficient value in <i>'Global settings'</i>. This will define the number of images which can be uploaded per upload session."),
    '#default_value' => !empty($widget['fupload_mode']) ? $widget['fupload_mode'] : '',
    '#options' => array('single' => t('One image per node'), 'multiple' => t('Multiple images per node')),
    '#weight' => -10,
    '#required' => TRUE,
  );
  
  $form['fupload_previewlist'] = array(
    '#type' => 'fieldset',
    '#title' => t('Images preview list'),
    '#description' => t('This feature adds the ability to show a list of all images after having uploaded them. At the same time, all title and body fields can be edited all at once.<br /> In order to use it, it has to be activated separately per !admin-permissions.', array('!admin-permissions' => l(t('user role'), 'admin/user/permissions'))),
    '#collapsible' => TRUE,
    '#weight' => 3,
  );
  $form['fupload_previewlist']['fupload_previewlist_img'] = array(
    '#type' => 'select',
    '#title' => t('Preview Image Preset'),
    '#description' => t("This setting is responsible for the way of displaying and handling the preview image which is generated out of the original image. ImageCache module has to be installed and a preset has to be created which can be selected right here, in order to make it work."),
    '#options' => _fupload_imagepreview_settings('list', $node_type), // @str_replace: returns correct node type
    '#default_value' => _fupload_imagepreview_settings('read', $node_type),
    '#required' => TRUE,
  );
  $form['fupload_previewlist']['fupload_previewlist_img_attributes'] = array(
    '#type' => 'textfield',
    '#title' => t('Image Attributes'),
    '#description' => t('Provide some additional attributes to be integrated in image (preview) tag, for example: class="my_custom_css"'),
    '#default_value' => !empty($widget['fupload_previewlist_img_attributes']) ? $widget['fupload_previewlist_img_attributes'] : '',
  );
  $form['fupload_previewlist']['field_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Field settings'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['fupload_previewlist']['field_settings']['fupload_previewlist_field_settings'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Editable fields'),
    '#description' => t('Choose the fields which should be editable by the uploader. Note that depending on the storage mode, it is not possible to choose some fields.') .'<p>'.t('<strong>Important information:</strong><br /> If the body is not editable by the user, <em>"minimum number of words"</em> !setting for the body field has to be <strong>0</strong>.', array('!setting' => l(t('setting'), 'admin/content/node-type/image'))).'</p>',
    '#options' => _image_fupload_previewlist_captions($node_type, arg(5)), 
    '#default_value' => !empty($widget['fupload_previewlist_field_settings']) ? $widget['fupload_previewlist_field_settings'] : array(),
  );
  $form['fupload_previewlist_redirecturl'] = array(
    '#type' => 'textfield',
    '#title' => t('Redirect url'),
    '#description' => t("After having uploaded some images, the user will redirected to newly created node (multiple storage mode) or node creation page (single storage mode) by default.<br />Providing an alternative url, user will be redirected to the entered url. Syntax (Drupals url function): \"node/add\""),
    '#default_value' => !empty($widget['fupload_previewlist_redirecturl']) ? $widget['fupload_previewlist_redirecturl'] : '',
  );
  $form['fupload_title_replacements'] = array(
    '#type' => 'textfield',
    '#title' => t('Image title processor'),
    '#description' => t('All entered elements which have to be separated by a semicolon (";"), are replaced by a whitespace when the node title is created out of the original image filename.')
      .'<p>'. t('<em>Note:</em> The theme function "fupload_create_filename" can be overwritten to provide a customised title creation.') .'</p>',
    '#default_value' => !empty($widget['fupload_title_replacements']) ? $widget['fupload_title_replacements'] : '_;{;};-',
    '#weight' => 3,
    '#required' => TRUE,
  ); 
  
  return $form;
}

/**
 * Implementation of hook_widget_settings($op = 'save').
 */
function image_fupload_imagefield_widget_settings_save($widget) {
  // elements which should be saved for this field (settings)
  $settings = array('fupload_mode', 'fupload_previewlist_img_attributes', 'fupload_title_replacements', 'fupload_previewlist_field_settings', 'fupload_previewlist_redirecturl');
  _fupload_imagepreview_settings('write', $widget['type_name'], array('fieldname' => $widget['field_name'], 'preview_preset' => $widget['fupload_previewlist_img']));
  
  return array_merge(module_invoke('imagefield', 'widget_settings_save', $widget), $settings);  
}

/**
* Implementation of hook_widget_settings($op = 'validate').
 */
function image_fupload_imagefield_widget_settings_validate($widget) {
  module_invoke('imagefield', 'widget_settings_validate', $widget);
  // check some dependencies
  if (isset($widget['fupload_mode']) && $widget['fupload_mode'] == "multiple") {
    $allowed_fields = array('imagefield_title', 'imagefield_alt', 'imagefield_description');
    foreach ($widget['fupload_previewlist_field_settings'] as $key) {
      if (!in_array($key, $allowed_fields))
        form_set_error('fupload_previewlist_field_settings_' .$key, t('It is not possible to support a "@field" field (Field settings) in current storage mode. Disable this checkbox in order to continue.', array('@field' => $widget['fupload_previewlist_field_settings'][$key])));
    }
  } 
  return array();
}

/**
 * @} End defgroup "FileField widget element callbacks."
 */

/**
* Helper function
* Creates a list of CCK fields depending on node type
*/
 
function _image_fupload_previewlist_captions($node_type, $field_name) {
  $options = array('node_title' => t('Title (Node)'), 'node_description' => t('Description (Node)'), 'imagefield_title' => t('Title (ImageField)'), 'imagefield_alt' => t('Alt (ImageField)'), 'imagefield_description' => t('Description (ImageField)'));
  if (module_exists('taxonomy')) {
    // also include taxonomy in list
    foreach (taxonomy_get_vocabularies($node_type) as $term)    
      $options['taxonomy_' .$term->vid] = $term->name .' (Taxonomy)';
  }
  
  $fields = content_types($node_type);  
  foreach ($fields['fields'] as $field) {
    if ($field['field_name'] != $field_name)
      $options['cck_' .$field['field_name']] = $field['widget']['label'] .' (CCK)';
  }
  return $options;
}
 
/**
 * FormAPI theme function. Theme the output of an image field.
 */
function theme_image_fupload_imagefield_widget($element) {
  return theme('form_element', $element, $element['#children']);
}