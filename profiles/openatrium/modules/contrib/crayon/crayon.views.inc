<?php

/**
 * Implementation of hook_views_handlers().
 */
function crayon_views_handlers() {
  $handlers = array();
  $handlers['handlers'] = array();
  $handlers['info']['path'] = drupal_get_path('module', 'crayon') .'/views';
  // OG: Group.
  if (module_exists('og_views')) {
    if (module_exists('spaces_og')) {
      $handlers['handlers']['crayon_views_handler_field_spaces_og'] = array(
        'parent' => 'spaces_og_handler_field_og_group_nids',
      );
    }
    else {
      $handlers['handlers']['crayon_views_handler_field_og'] = array(
        'parent' => 'og_views_handler_field_og_group_nids',
      );
    }
  }
  // Casetracker: Project.
  if (module_exists('casetracker')) {
    $handlers['handlers']['crayon_views_handler_field_project'] = array(
      'parent' => 'casetracker_views_handler_field_project_name',
    );
  }
  return $handlers;
}

/**
 * Implementation of hook_views_data_alter().
 */
function crayon_views_data_alter(&$data) {
  // OG: Group.
  if (module_exists('og_views') && isset($data['og_ancestry']['group_nid']['field']['handler'])) {
    if (module_exists('spaces_og')) {
      $data['og_ancestry']['group_nid']['field']['handler'] = 'crayon_views_handler_field_spaces_og';
    }
    else {
      $data['og_ancestry']['group_nid']['field']['handler'] = 'crayon_views_handler_field_og';
    }
  }
  // Casetracker: Project.
  if (module_exists('casetracker') && isset($data['casetracker_case']['pid']['field']['handler'])) {
    $data['casetracker_case']['pid']['field']['handler'] = 'crayon_views_handler_field_project';
  }
}
