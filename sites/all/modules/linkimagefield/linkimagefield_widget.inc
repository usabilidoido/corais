<?php
// $Id: linkimagefield_widget.inc,v 1.5 2009/09/04 18:32:34 johnfyoung Exp $
/**
  * @file
  *
  * linkimagefield widget code
  * @author John Young <john@codeandcreative.com>
  */

/**
 * Implementation of CCK's hook_widget_settings($op = 'form')
 *
 * @param array $widget
 * @return array
 */
function linkimagefield_widget_settings_form($widget) {
  $form = module_invoke('imagefield', 'widget_settings', 'form', $widget);
  
  $form['url_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('URL Link settings'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#weight' => 0,
  );
  
  $form['url_settings']['url'] = array(
    '#type'          => 'textfield',
    '#maxlength'     => '255',
    '#title'         => t('Default URL'),
    '#description'   => t('Provide a well-formed URL.  This will be the default url linked to by provided images.'),
    '#default_value' => !empty($widget['url']) ? $widget['url'] : '',
  );
  
  $target_options = _linkimagefield_widget_url_target_options();

  $form['url_settings']['custom_target'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable custom target'),
    '#default_value' =>  !empty($widget['custom_target']) ? $widget['custom_target'] : 0,
    '#description' => t('Enable user to provide alternate target frame for link.'),
  );
  
  $form['url_settings']['target'] = array(
    '#type'          => 'select',
    '#maxlength'     => '255',
    '#title'         => t('Default Target'),
    '#description'   => t('Select a default target type.'),
    '#default_value' => !empty($widget['target']) ? $widget['target'] : '_self',
  	'#options'			 => $target_options,
  );
  
  $form['url_settings']['nofollow'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable nofollow'),
    '#default_value' =>  !empty($widget['nofollow']) ? $widget['nofollow'] : 0,
    '#description' => t('Allow links to have "nofollow" rel tags'),
  );
  
  return $form;
}

/**
 * Implementation of CCK's hook_widget_settings($op = 'save').
 *
 * @param array $widget
 * @return array
 */
function linkimagefield_widget_settings_save($widget) {
  $imagefield_settings = module_invoke('imagefield', 'widget_settings', 'save', $widget);
  return array_merge($imagefield_settings, array('url', 'custom_target', 'target', 'nofollow'));
}

/**
 * Implementation of CCK's hook_widget_settings($op = 'validate').
 *
 * @param array $widget
 */
function linkimagefield_widget_settings_validate($widget) {  
  module_invoke('imagefield', 'widget_settings', 'validate', $widget);
  
  // Check the URL
  if (!empty($widget['url'])) {
    if (!valid_url($widget['url'])) {
      form_set_error('url', t('Please provide a valid url.'));
    } else {
      $widget['url'] = check_url($widget['url']);
    }
  }
  
  // Check the target
  if (!empty($widget['target'])) {
    $options = array('_self', '_blank', '_parent', '_top');
    
    if(!in_array($widget['target'], $options)) {
      $widget['target'] = '_self';
    }
  }
}

/**
 * Element #value_callback function.
 *
 * @param array $element
 * @param mixed $edit
 * @return array
 */
function linkimagefield_widget_value($element, $edit = FALSE) {
  $item = imagefield_widget_value($element, $edit);
  if ($edit) {
    $item['url'] = isset($edit['data']['url']) ? $edit['data']['url'] : '';
    $item['target'] = isset($edit['data']['target']) ? $edit['data']['target'] : '_self';
    $item['nofollow'] = isset($edit['data']['nofollow']) ? $edit['data']['nofollow'] : FALSE;
  }
  else {
    $item['url'] = '';
    $item['target'] = '_self';
    $item['nofollow'] = TRUE;
  }
  return $item;
}

/**
 * Element #process callback function.
 *
 * @param array $element
 * @param array $edit
 * @param arrayreference $form_state
 * @param array $form
 * @return array
 */
function linkimagefield_widget_process($element, $edit, &$form_state, $form) {
  $file = $element['#value'];
  $element = imagefield_widget_process($element, $edit, $form_state, $form);

  // add some help text, telling user the URL field will be viewable after the file is supplied 
  $element['upload']['#description'] = t('Once an Image is uploaded, you will ba able to supply a URL for the link.') . '<br />' . $element['upload']['#description'];
   
  $field = content_fields($element['#field_name'], $element['#type_name']);
  $element['#theme'] = 'linkimagefield_widget_item';
  
  $element['data']['url'] = array(
    '#title' => t('URL'),
    '#type' => 'textfield',
    '#default_value' => $file['data']['url'] ? $file['data']['url'] : $field['widget']['url'],
    '#description' => t('This URL will be loaded when the image is clicked'),
    '#maxlength' => 255,
  );
  
  $default_target = !$field['widget']['custom_target'];
  
  $element['data']['target'] = array(
    '#title'         => t('Target'),
    '#type'          => $field['widget']['custom_target'] ? 'select' : 'value',
    '#default_value' => $default_target ? $field['widget']['target'] : $file['data']['target'],
    '#description'   => t('Window/Frame Target for the URL'),
  );
  
  if($field['widget']['custom_target']) {
    $element['data']['target']['#options'] = array_merge(array('default' => t('Use Default').' ('.$field['widget']['target'].')'),_linkimagefield_widget_url_target_options());
  }
  
  if($field['widget']['nofollow']) {
    $element['data']['nofollow'] = array(
      '#title'         => t('Mark as "nofollow"'),
      '#type'          => 'checkbox',
      '#default_value' => $file['data']['nofollow'],
      '#description'   => t('Add a relation tag telling bots not to follow this link'),      
    );
  }
  return $element;
}

/**
 * FormAPI theme function. Theme the output of an image field.
 *
 * @param arrayreference $element
 * @return string
 */
function theme_linkimagefield_widget(&$element) {
  drupal_add_css(drupal_get_path('module', 'imagefield') .'/imagefield.css');
  return theme('form_element', $element, $element['#children']);
}

/**
 * the list of URL frame targets
 *
 * @return array
 */
function _linkimagefield_widget_url_target_options(){
  return array(
    '_self'				  => t('Same Window (_self)'),
    '_blank'				=> t('New Window (_blank)'),
    '_parent'				=> t('Parent Frame (_parent)'),
    '_top'				  => t('Top Frame (_top)'),
  );
}
