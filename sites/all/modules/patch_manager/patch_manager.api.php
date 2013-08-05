<?php

/**
 * @file Document patch_manager API usage
 */

/**
 * Implementation of hook_patch().
 */ 
function hook_patch() {
  $patches = array();
  
  $patches[] = array(
    'title'       => t('Allows mailsystem to be hooked'),
    'patch'       => '112311_core_mailsystem_hook.patch',
    'module'      => 'core',
  );
  
  $patches[] = array(
    'title'       => t('Allows cool things'),
    'patch'       => '123112_core_mailsystem_hook.patch',
    'module'      => 'smtp',
    'issue'       => '6574564/2',
    'description' => t('Patches core to allow the cool things to be hooked.'),
    'patchdir'    => 'patches',
  );
  
  return $patches;
}
