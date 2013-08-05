<?php
// $Id$
/**
 * @file
 */

/**
 * Configures basic settings to connect to Gitosis
 */
function gitosis_admin_form() {
  $form = array();

  $form['gitosis_repository'] = array(
    '#type' => 'textfield',
    '#title' => t('Full URL of gitosis-admin repository'),
    '#default_value' => variable_get('gitosis_repository', ''),
    '#description' => t("The url of the gitosis-admin repository, in a format like 
      'git@domain.to.my.repo:gitosis-admin.git'."),
    '#required' => TRUE,
  );

  $form['gitosis_main_config'] = array(
    '#type' => 'textarea',
    '#title' => t('Main configuration for gitosis.conf'),
    '#default_value' => variable_get('gitosis_main_config', ''),
    '#description' => t("This will be the main configutarion at the top of the gitosis.conf file."),
    '#required' => FALSE,
    '#wysiwyg'  => FALSE,
  );

  $form['gitosis_extra_config'] = array(
    '#type' => 'textarea',
    '#title' => t('Additional configuration for gitosis.conf'),
    '#default_value' => variable_get('gitosis_extra_config', ''),
    '#description' => t("This will be appended at the end of the generated gitosis.conf."),
    '#required' => FALSE,
    '#wysiwyg'  => FALSE,
  );

  $form['gitosis_main_config_help'] = array(
    '#value' => t("<b>Your main gitosis configuration should look at least as the following:</b> <br/>
                          [gitosis] <br/><br/> 
                          [group gitosis-admin] <br/>
                          writable = gitosis-admin <br/>
                          members = name_of@your_key_at_keydir<br/>"),
  );
   

  $form['#submit'][] = 'gitosis_admin_form_submit';

  return system_settings_form($form);
}
