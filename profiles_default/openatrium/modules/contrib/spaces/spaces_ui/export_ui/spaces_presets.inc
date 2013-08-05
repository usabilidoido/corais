<?php

$plugin = array(
  'schema' => 'spaces_presets',
  'menu' => array(
    'menu prefix' => 'admin/build',
    'menu item' => 'spaces',
    'menu title' => 'Spaces',
    'menu description' => 'Settings for Spaces.',
  ),
  'access' => 'administer spaces',
  'title singular' => t('spaces preset'),
  'title singular proper' => t('Spaces preset'),
  'title plural' => t('spaces presets'),
  'title plural proper' => t('Spaces presets'),
  'form' => array(
    'settings' => 'spaces_preset_editor',
    'submit' => 'spaces_preset_editor_submit',
  ),
  'handler' => array(
    'class' => 'spaces_presets_export_ui',
    'parent' => 'ctools_export_ui',
  ),
);
