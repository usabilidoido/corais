<?php

/**
 * @file
 * Implementation of the theme plugin for the classic theme
 */
$path = drupal_get_path('module', 'timeline') .'/plugins/themes/';
$plugin = array(
  'name' => t('Dark Theme'),
  'files' => array(
    'css' => $path . 'DarkTheme/theme.css',
    'js' => $path . 'DarkTheme/theme.js',
  ),
);
