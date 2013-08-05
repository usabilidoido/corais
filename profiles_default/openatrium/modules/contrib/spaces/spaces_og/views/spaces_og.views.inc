<?php

/**
 * Implementation of hook_views_plugins().
 */
function spaces_og_views_plugins() {
  return array(
    'argument validator' => array(
      'useringroups' => array(
        'title' => t("User in current user's groups"),
        'handler' => 'spaces_og_plugin_argument_validate_useringroups',
        'path' => drupal_get_path('module', 'spaces_og') . '/views',
      ),
    ),
  );
}

/**
 * Implementation of hook_views_handlers().
 */
function spaces_og_views_handlers() {
  return array(
    'info' => array(
      'path' => drupal_get_path('module', 'spaces_og') .'/views',
    ),
    'handlers' => array(
      'spaces_og_handler_field_og_group_nids' => array(
        'parent' => 'og_views_handler_field_og_group_nids',
      ),
    ),
  );
}

/**
 * Implementation of hook_views_data_alter().
 */
function spaces_og_views_data_alter(&$data) {
  if (isset($data['og_ancestry']['group_nid']['field']['handler'])) {
    $data['og_ancestry']['group_nid']['field']['handler'] = 'spaces_og_handler_field_og_group_nids';
  }
}
