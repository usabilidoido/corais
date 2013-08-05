<?php
// $Id: vimeotube.admin.inc,v 1.1 2010/07/13 21:41:28 bonked Exp $

/**
 * @file
 * Administration page callbacks for the vimeotube module
 */
 
/**
 * Form builder. Configure vimeotube.
 *
 * @ingroup forms
 * @see system_settings_form().
 */
function vimeotube_admin_settings() {
  $form['vimeotube_default_width'] = array(
    '#type' => 'textfield',
	'#title' => t('Default Width for Videos'),
	'#default_value' => variable_get('vimeotube_default_width', 400),
	'#description' => t('Enter the default width vimeotube will use for Vimeo videos.'),
  );
  $form['vimeotube_default_height'] = array(
    '#type' => 'textfield',
	'#title' => t('Default Height for Videos'),
	'#default_value' => variable_get('vimeotube_default_height', 225),
	'#description' => t('Enter the default height vimeotube will use for Vimeo videos.'),
  );
  
  return system_settings_form($form);
  
}
