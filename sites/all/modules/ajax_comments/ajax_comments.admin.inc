<?php
// $Id: ajax_comments.admin.inc,v 1.1.2.2 2009/07/16 09:34:49 neochief Exp $

/**
 * @file
 * AJAX comments settings form.
 */

/**
 * AJAX comments settings form.
 */
function ajax_comments_settings() {
  $form['settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Settings'),
  );
  $form['settings']['ajax_comments_always_expand_form'] = array(
    '#type' => 'checkbox',
    '#title' => t('Form always expanded'),
    '#description' => t('This setting allows to render comments as collapsed link by default.'),
    '#default_value' => variable_get('ajax_comments_always_expand_form', FALSE),
  );
  $form['settings']['ajax_comments_blink_new'] = array(
    '#type' => 'checkbox',
    '#title' => t('Blink new comment'),
    '#description' => t('This effect helps to find new comment after replying to long threads.'),
    '#default_value' => variable_get('ajax_comments_blink_new', TRUE),
  );
  $form['settings']['ajax_comments_default_row_count'] = array(
    '#type' => 'textfield',
    '#title' => t('Default row count'),
    '#description' => t('Allows to control comment forms\' textarea size. Leave blank to use system defined.'),
    '#default_value' => variable_get('ajax_comments_default_row_count', 5),
  );
  $form['settings']['ajax_comments_reply_row_count'] = array(
    '#type' => 'textfield',
    '#title' => t('Row count in replies'),
    '#description' => t('Usually, there is no need in big forms for replies, so this setting allows to reduce their size.'),
    '#default_value' => variable_get('ajax_comments_reply_row_count', 3),
  );

  $form['node_types'] = array(
    '#type' => 'fieldset',
    '#title' => t('Content types'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['node_types']['ajax_comments_node_types'] = array(
    '#type' => 'checkboxes',
    '#description' => t('Select node types you want to activate ajax comments on. If you select nothing, AJAX Comments will be enabled everywhere.'),
    '#default_value' => variable_get('ajax_comments_node_types', array()),
    '#options' => node_get_types('names'),
  );
  return system_settings_form($form);
}

