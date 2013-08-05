<?php
/**
 * @file patch_manager.views_default.inc
 * Provide default views to be used with the patch manager
 */

/**
 * Implementation of hook_views_default_views().
 */
function patch_manager_views_default_views() {
  $views = array();
    
  $view = new view;
  $view->name = 'patches';
  $view->description = 'View patches stored with the patch manager.';
  $view->tag = '';
  $view->view_php = '';
  $view->base_table = 'node';
  $view->is_cacheable = FALSE;
  $view->api_version = 2;
  $view->disabled = FALSE;
  $handler = $view->new_display('default', 'Defaults', 'default');
  $handler->override_option('fields', array(
    'nid' => array(
      'label' => '',
      'alter' => array(
        'alter_text' => 0,
        'text' => '',
        'make_link' => 0,
        'path' => '',
        'link_class' => '',
        'alt' => '',
        'prefix' => '',
        'suffix' => '',
        'target' => '',
        'help' => '',
        'trim' => 0,
        'max_length' => '',
        'word_boundary' => 1,
        'ellipsis' => 1,
        'html' => 0,
        'strip_tags' => 0,
      ),
      'empty' => '',
      'hide_empty' => 0,
      'empty_zero' => 0,
      'link_to_node' => 0,
      'exclude' => 1,
      'id' => 'nid',
      'table' => 'node',
      'field' => 'nid',
      'relationship' => 'none',
    ),
    'created' => array(
      'label' => 'Created',
      'alter' => array(
        'alter_text' => 0,
        'text' => '',
        'make_link' => 0,
        'path' => '',
        'link_class' => '',
        'alt' => '',
        'prefix' => '',
        'suffix' => '',
        'target' => '',
        'help' => '',
        'trim' => 0,
        'max_length' => '',
        'word_boundary' => 1,
        'ellipsis' => 1,
        'html' => 0,
        'strip_tags' => 0,
      ),
      'empty' => '',
      'hide_empty' => 0,
      'empty_zero' => 0,
      'date_format' => 'custom',
      'custom_date_format' => 'Y-m-d',
      'exclude' => 0,
      'id' => 'created',
      'table' => 'node',
      'field' => 'created',
      'relationship' => 'none',
    ),
    'field_module_value' => array(
      'label' => 'Module',
      'alter' => array(
        'alter_text' => 0,
        'text' => '',
        'make_link' => 0,
        'path' => '',
        'link_class' => '',
        'alt' => '',
        'prefix' => '',
        'suffix' => '',
        'target' => '',
        'help' => '',
        'trim' => 0,
        'max_length' => '',
        'word_boundary' => 0,
        'ellipsis' => 0,
        'html' => 0,
        'strip_tags' => 0,
      ),
      'empty' => '',
      'hide_empty' => 0,
      'empty_zero' => 0,
      'link_to_node' => 0,
      'label_type' => 'widget',
      'format' => 'default',
      'multiple' => array(
        'group' => TRUE,
        'multiple_number' => '',
        'multiple_from' => '',
        'multiple_reversed' => FALSE,
      ),
      'exclude' => 0,
      'id' => 'field_module_value',
      'table' => 'node_data_field_module',
      'field' => 'field_module_value',
      'override' => array(
        'button' => 'Override',
      ),
      'relationship' => 'none',
    ),
    'title' => array(
      'label' => 'Name',
      'alter' => array(
        'alter_text' => 0,
        'text' => '',
        'make_link' => 0,
        'path' => '',
        'link_class' => '',
        'alt' => '',
        'prefix' => '',
        'suffix' => '',
        'target' => '',
        'help' => '',
        'trim' => 0,
        'max_length' => '',
        'word_boundary' => 1,
        'ellipsis' => 1,
        'html' => 0,
        'strip_tags' => 0,
      ),
      'empty' => '',
      'hide_empty' => 0,
      'empty_zero' => 0,
      'link_to_node' => 1,
      'exclude' => 0,
      'id' => 'title',
      'table' => 'node',
      'field' => 'title',
      'relationship' => 'none',
    ),
    'field_drupal_issue_value' => array(
      'label' => 'Issue #',
      'alter' => array(
        'alter_text' => 0,
        'text' => '',
        'make_link' => 0,
        'path' => '',
        'link_class' => '',
        'alt' => '',
        'prefix' => '',
        'suffix' => '',
        'target' => '',
        'help' => '',
        'trim' => 0,
        'max_length' => '',
        'word_boundary' => 1,
        'ellipsis' => 1,
        'html' => 0,
        'strip_tags' => 0,
      ),
      'empty' => '<em>N/A</em>',
      'hide_empty' => 1,
      'empty_zero' => 0,
      'link_to_node' => 0,
      'label_type' => 'custom',
      'format' => 'issuelink',
      'multiple' => array(
        'group' => TRUE,
        'multiple_number' => '',
        'multiple_from' => '',
        'multiple_reversed' => FALSE,
      ),
      'exclude' => 0,
      'id' => 'field_drupal_issue_value',
      'table' => 'node_data_field_drupal_issue',
      'field' => 'field_drupal_issue_value',
      'override' => array(
        'button' => 'Override',
      ),
      'relationship' => 'none',
    ),
    'field_patch_fid' => array(
      'label' => 'Patch file',
      'alter' => array(
        'alter_text' => 0,
        'text' => '',
        'make_link' => 0,
        'path' => '',
        'link_class' => '',
        'alt' => '',
        'prefix' => '',
        'suffix' => '',
        'target' => '',
        'help' => '',
        'trim' => 0,
        'max_length' => '',
        'word_boundary' => 0,
        'ellipsis' => 0,
        'html' => 0,
        'strip_tags' => 0,
      ),
      'empty' => '',
      'hide_empty' => 0,
      'empty_zero' => 0,
      'link_to_node' => 1,
      'label_type' => 'widget',
      'format' => 'default',
      'multiple' => array(
        'group' => TRUE,
        'multiple_number' => '',
        'multiple_from' => '',
        'multiple_reversed' => FALSE,
      ),
      'exclude' => 0,
      'id' => 'field_patch_fid',
      'table' => 'node_data_field_patch',
      'field' => 'field_patch_fid',
      'override' => array(
        'button' => 'Override',
      ),
      'relationship' => 'none',
    ),
  ));
  $handler->override_option('sorts', array(
    'last_updated' => array(
      'order' => 'ASC',
      'granularity' => 'second',
      'id' => 'last_updated',
      'table' => 'node_comment_statistics',
      'field' => 'last_updated',
      'relationship' => 'none',
    ),
  ));
  $handler->override_option('filters', array(
    'type' => array(
      'operator' => 'in',
      'value' => array(
        'patch' => 'patch',
      ),
      'group' => '0',
      'exposed' => FALSE,
      'expose' => array(
        'operator' => 'type_op',
        'label' => 'Node: Type',
        'use_operator' => FALSE,
        'identifier' => 'type',
        'remember' => FALSE,
        'single' => TRUE,
        'optional' => TRUE,
        'reduce' => FALSE,
      ),
      'id' => 'type',
      'table' => 'node',
      'field' => 'type',
      'relationship' => 'none',
    ),
  ));
  $handler->override_option('access', array(
    'type' => 'perm',
    'perm' => 'administer patch manager',
  ));
  $handler->override_option('cache', array(
    'type' => 'none',
  ));
  $handler->override_option('title', 'Patches');
  $handler->override_option('style_plugin', 'table');
  $handler->override_option('style_options', array(
    'grouping' => '',
    'override' => 1,
    'sticky' => 0,
    'order' => 'asc',
    'columns' => array(
      'nid' => 'nid',
      'created' => 'created',
      'field_module_value' => 'field_module_value',
      'title' => 'title',
      'field_drupal_issue_value' => 'field_drupal_issue_value',
      'field_patch_fid' => 'field_patch_fid',
    ),
    'info' => array(
      'nid' => array(
        'sortable' => 0,
        'separator' => '',
      ),
      'created' => array(
        'sortable' => 1,
        'separator' => '',
      ),
      'field_module_value' => array(
        'sortable' => 1,
        'separator' => '',
      ),
      'title' => array(
        'sortable' => 1,
        'separator' => '',
      ),
      'field_drupal_issue_value' => array(
        'sortable' => 0,
        'separator' => '',
      ),
      'field_patch_fid' => array(
        'separator' => '',
      ),
    ),
    'default' => 'field_module_value',
  ));
  $handler = $view->new_display('page', 'Simple List', 'simplelist');
  $handler->override_option('empty', '<p>There are no patches being managed by the patch manager.</p>');
  $handler->override_option('empty_format', '1');
  $handler->override_option('path', 'admin/build/patch/simplelist');
  $handler->override_option('menu', array(
    'type' => 'none',
    'title' => 'List',
    'description' => 'List patches stored with the patch manager.',
    'weight' => '0',
    'name' => 'navigation',
  ));
  $handler->override_option('tab_options', array(
    'type' => 'none',
    'title' => 'Patches',
    'description' => 'Patch management of core and contributed modules.',
    'weight' => '0',
    'name' => 'navigation',
  ));
  
  // If we have views_bulk_operations enabled, then we add another view
  if (module_exists('views_bulk_operations')) {
    $handler = $view->new_display('page', 'Bulk List', 'bulklist');
    $handler->override_option('style_plugin', 'bulk');
    $handler->override_option('style_options', array(
      'grouping' => '',
      'override' => 1,
      'sticky' => 0,
      'order' => 'asc',
      'columns' => array(
        'nid' => 'nid',
        'created' => 'created',
        'field_module_value' => 'field_module_value',
        'title' => 'title',
        'field_drupal_issue_value' => 'field_drupal_issue_value',
        'field_patch_fid' => 'field_patch_fid',
      ),
      'info' => array(
        'nid' => array(
          'sortable' => 1,
          'separator' => '',
        ),
        'created' => array(
          'sortable' => 1,
          'separator' => '',
        ),
        'field_module_value' => array(
          'sortable' => 1,
          'separator' => '',
        ),
        'title' => array(
          'sortable' => 1,
          'separator' => '',
        ),
        'field_drupal_issue_value' => array(
          'sortable' => 0,
          'separator' => '',
        ),
        'field_patch_fid' => array(
          'separator' => '',
        ),
      ),
      'default' => 'field_module_value',
      'execution_type' => '1',
      'display_type' => '1',
      'hide_select_all' => 0,
      'skip_confirmation' => 0,
      'display_result' => 0,
      'merge_single_action' => 1,
      'selected_operations' => array(
        'patch_manager_apply_action' => 'patch_manager_apply_action',
        'patch_manager_delete_action' => 'patch_manager_delete_action',
        'patch_manager_revert_action' => 'patch_manager_revert_action',
      ),
      'views_bulk_operations_fields_action' => array(
        'php_code' => 0,
        'display_fields' => array(
          'field_module' => 'field_module',
          'field_patch' => 'field_patch',
          'field_drupal_issue' => 'field_drupal_issue',
        ),
      ),
    ));
    $handler->override_option('path', 'admin/build/patch/bulklist');
    $handler->override_option('menu', array(
      'type' => 'none',
      'title' => '',
      'description' => '',
      'weight' => 0,
      'name' => 'navigation',
    ));
    $handler->override_option('tab_options', array(
      'type' => 'none',
      'title' => '',
      'description' => '',
      'weight' => 0,
      'name' => 'navigation',
    ));
  }
  
  $views['patches'] = $view;
  
  return $views;
}

