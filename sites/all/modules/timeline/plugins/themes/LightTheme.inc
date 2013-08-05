<?php

/**
 * @file
 * Implementation of the theme plugin for the classic theme
 */
$path = drupal_get_path('module', 'timeline') .'/plugins/themes/';
$plugin = array(
  'name' => t('Light Theme'),
  'files' => array(
    'css' => $path . 'LightTheme/theme.css',
    'js' => $path . 'LightTheme/theme.js',
  ),
);
