<?php
// $Id: image_fupload_image.admin.inc,v 1.7 2009/01/31 00:13:38 grandcat Exp $

function image_fupload_image_admin() {
  $form['info'] = array(
    '#value' => t('To be able to mass upload images using image_fupload, the corresponding permissions have to be set per !admin-permissions. To provide a preview page of all uploaded images and the ability to edit its fields (title and body), have a look at the category "Images preview list".', array('!admin-permissions' => l(t('user role'), 'admin/user/permissions'))),
    '#weight' => -12,
  );
  $form['previewlist'] = array(
    '#type' => 'fieldset',
    '#title' => t('Images preview list'),
    '#description' => t('This feature adds the ability to show a list of all images after having uploaded them. At the same time, all title and body fields can be edited all at once.<br /> In order to use it, it has to be activated separately per !admin-permissions.', array('!admin-permissions' => l(t('user role'), 'admin/user/permissions'))),
    '#collapsible' => TRUE,
    '#weight' => -10,
  );
  $form['previewlist']['fupload_previewlist_img'] = array(
    '#type' => 'select',
    '#title' => t('Preview Image Preset'),
    '#description' => t('This setting is responsible for the way of displaying and handling the preview image which is generated out of the original image. Between different image sizes can be chosen.<br /> If <em>imagecache module</em> is installed and enabled, also its presets (if available) can be used to display the preview image.'),
    '#options' => _fupload_imagepreview_settings('list', 'image'),
    '#default_value' => _fupload_imagepreview_settings('read', 'image'),
    '#required' => TRUE,
  );
  $form['previewlist']['fupload_previewlist_img_attributes'] = array(
    '#type' => 'textfield',
    '#title' => t('Image Attributes'),
    '#description' => t('Provide some additional attributes to be integrated in image (preview) tag, for example: class="my_custom_css"'),
    '#default_value' => variable_get('fupload_previewlist_img_attributes', ''),
  );
  $form['previewlist']['field_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Field settings'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['previewlist']['field_settings']['fupload_previewlist_field_settings'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Editable fields'),
    '#description' => t('Choose the fields which should be editable by the uploader.') .'<p>'.t('<strong>Important information:</strong><br /> If the body is not editable by the user, <em>"minimum number of words"</em> !setting for the body field has to be <strong>0</strong>.', array('!setting' => l(t('setting'), 'admin/content/node-type/image'))).'</p>',
    '#options' => array(
      'title' => t('Title'),
      'body' => t('Body'),    
    ),
    '#default_value' => variable_get('fupload_previewlist_field_settings', array('title', 'body')),
  );
  
  $form['fupload_title_replacements'] = array(
    '#type' => 'textfield',
    '#title' => t('Image Title Processor'),
    '#description' => t('All entered elements which have to be separated by a semicolon (";"), are replaced by a whitespace when the node title is created out of the original image filename.')
      .'<p>'. t('<em>Note:</em> The theme function "fupload_create_filename" can be overwritten to provide a customised title creation.') .'</p>',
    '#default_value' => variable_get('fupload_title_replacements', '_;{;}'),
    '#weight' => -5,
    '#required' => TRUE,
  );
  $form['#submit'][] = 'image_fupload_image_admin_submit';

  return system_settings_form($form);
}

function image_fupload_image_admin_submit($form, &$form_state) {
  _fupload_imagepreview_settings('write', 'image', array('preview_preset' => $form_state['values']['fupload_previewlist_img']));     
}