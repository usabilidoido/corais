<?php
// $Id: used_modules.admin.inc,v 1.1.2.2 2009/05/17 00:09:05 narno Exp $

/**
 * @file
 * Admin page callbacks for the used_modules module.
 */

/**
 * Administration settings form.
 *
 * @see system_settings_form()
 */
function used_modules_admin_settings() {
  $form = array();
  // page settings
  $form['page'] = array(
    '#type'  => 'fieldset',
    '#title' => t('Page settings'),
  );
  $form['page']['used_modules_page_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Title'),
    '#default_value' => variable_get('used_modules_page_title', t('Used modules')),
    '#size' => 60,
    '#maxlength' => 128,
    '#required' => TRUE,
  );
  $form['page']['used_modules_page_description'] = array(
    '#type' => 'textarea',
    '#title' => t('Page description'),
    '#default_value' => variable_get('used_modules_page_description', t('List of modules are used on my website.')),
    '#cols' => 72,
    '#rows' => 3,
  );
  $form['page']['used_modules_page_display_description'] = array(
    '#type' => 'checkbox',
    '#title' => t('Display modules description'),
    '#default_value' => variable_get('used_modules_page_display_description', TRUE),
  );
  $form['page']['used_modules_page_display_version'] = array(
    '#type' => 'checkbox',
    '#title' => t('Display modules version'),
    '#default_value' => variable_get('used_modules_page_display_version', FALSE),
  );
  $form['page']['used_modules_page_add_link'] = array(
    '#type' => 'checkbox',
    '#title' => t('Link to the module page (drupal.org)'),
    '#default_value' => variable_get('used_modules_page_add_link', TRUE),
  );
  $form['page']['information'] = array(
    '#value' => l(t('Access to the <em>Used modules</em> page'), 'modules/used', array('attributes' => array('target' => '_blank'), 'html' => TRUE)),
  );
  return system_settings_form($form);
}
