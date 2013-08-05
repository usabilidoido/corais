<?php
// $Id: gb.admin.inc,v 1.1 2009/05/01 00:13:13 gordon Exp $
/**
 * @file
 * Git Browser administration pages
 */

/**
 * Git Browser Settings
 */
function gb_settings() {
  $form = array();
  
  $form['git_cmd'] = array(
    '#type' => 'textfield',
    '#title' => t('Path to git'),
    '#default_value' => variable_get('git_cmd', '/usr/bin/git')
  );
  
  $version = gb_git(NULL, '--version');
  $form['git_version'] = array(
    '#type' => 'item',
    '#title' => t('Git version'),
    '#value' => implode("<br />\n", $version),
  );
  return system_settings_form($form);
}