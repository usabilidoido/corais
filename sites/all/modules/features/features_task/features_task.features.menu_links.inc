<?php

/**
 * Implementation of hook_menu_default_menu_links().
 */
function features_task_menu_default_menu_links() {
  $menu_links = array();

  // Exported menu link: admin:admin/settings/time_tracker
  $menu_links['admin:admin/settings/time_tracker'] = array(
    'menu_name' => 'admin',
    'link_path' => 'admin/settings/time_tracker',
    'router_path' => 'admin/settings/time_tracker',
    'link_title' => 'Time Tracker',
    'options' => array(
      'attributes' => array(
        'title' => 'Adjust time tracker settings.',
      ),
    ),
    'module' => 'system',
    'hidden' => '0',
    'external' => '0',
    'has_children' => '1',
    'expanded' => '0',
    'weight' => '0',
    'parent_path' => 'admin/settings',
  );
  // Exported menu link: features:tasks
  $menu_links['features:tasks'] = array(
    'menu_name' => 'features',
    'link_path' => 'tasks',
    'router_path' => 'tasks',
    'link_title' => 'Tasks',
    'options' => array(),
    'module' => 'system',
    'hidden' => '0',
    'external' => '0',
    'has_children' => '0',
    'expanded' => '0',
    'weight' => '-10',
  );
  // Exported menu link: navigation:node/add/milestone
  $menu_links['navigation:node/add/milestone'] = array(
    'menu_name' => 'navigation',
    'link_path' => 'node/add/milestone',
    'router_path' => 'node/add/milestone',
    'link_title' => 'Etapa',
    'options' => array(),
    'module' => 'system',
    'hidden' => '0',
    'external' => '0',
    'has_children' => '0',
    'expanded' => '0',
    'weight' => '0',
    'parent_path' => 'node/add',
  );
  // Exported menu link: navigation:node/add/task
  $menu_links['navigation:node/add/task'] = array(
    'menu_name' => 'navigation',
    'link_path' => 'node/add/task',
    'router_path' => 'node/add/task',
    'link_title' => 'Tarefa',
    'options' => array(
      'attributes' => array(
        'title' => 'These are OA Group tasks',
      ),
    ),
    'module' => 'system',
    'hidden' => '0',
    'external' => '0',
    'has_children' => '0',
    'expanded' => '0',
    'weight' => '0',
    'parent_path' => 'node/add',
  );
  // Translatables
  // Included for use with string extractors like potx.
  t('Etapa');
  t('Tarefa');
  t('Tasks');
  t('Time Tracker');


  return $menu_links;
}
