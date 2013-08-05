<?php
/**
 * @file
 *   Link provider for Taskbar.
 */

function taskbar_provider_link_definition() {
  return array(
    'name' => 'link',
    'title' => 'Links',
    'ajax' => FALSE,
    'realtime' => FALSE,
    'callback' => 'taskbar_provider_link',
    'settings form' => 'taskbar_provider_link_settings',
    'settings form submit' => 'taskbar_provider_link_settings_submit',
    'file' => 'taskbar_provider.link.inc',
    'path' => drupal_get_path('module', 'taskbar') . '/providers',
  );
}

function taskbar_provider_link($item) {
  $prefix = 'taskbar-item-' . $item->name;
  $links = array();
  foreach ($item->settings['links'] as $delta => $link) {
    $links[] = '<span class="taskbar-element ' . $prefix . ' ' . $prefix . '-' . $delta . '">' . l($link['title'], $link['url'], $link['options']) . '</span>';
  }

  return array('content' => implode('<span class="taskbar-item-separator"></span>', $links));
}

/**
 * Settings form for the link provider.
 */
function taskbar_provider_link_settings(&$form, &$form_state, $item) {
  $form['provider_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Link settings'),
    '#collapsible' => TRUE,
  );
  
  if (empty($item->settings['links'])) {
    $item->settings['links'] = array();
  }
  $links = $item->settings['links'] + array('link' . (count($item->settings['links']) + 1) => array('title' => '', 'url' => ''));
  foreach ($links as $name => $link) {
    $form['provider_settings']['link_' . $name . '_title'] = array(
     '#type' => 'textfield',
     '#title' => t('Title'),
     '#default_value' => $link['title'],
     '#size' => 20,
    );
    $form['provider_settings']['link_' . $name . '_url'] = array(
     '#type' => 'textfield',
     '#title' => t('URL'),
     '#default_value' => $link['url'],
    );
  }
}

function taskbar_provider_link_settings_submit($form, &$form_state, &$item) {
  foreach ($form_state['values'] as $name => $value) {
    if (!preg_match('/^link_(.+?)_url/', $name, $matches)) {
      continue;
    }
    $name = $matches[1];
    if (empty($form_state['values']['link_' . $name . '_url'])) {
      if (isset($item->settings['links'][$name])) {
        unset($item->settings['links'][$name]);
      }
    }
    else {
      $item->settings['links'][$name] = array(
        'url' => $form_state['values']['link_' . $name . '_url'],
        'title' => $form_state['values']['link_' . $name . '_title'],
        'options' => array(),
      );
    }
  }
}