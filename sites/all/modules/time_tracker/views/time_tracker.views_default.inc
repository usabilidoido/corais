<?php

/**
 * @file
 * Implementation of hook_views_default_views().
 */
function time_tracker_views_default_views() {
  $list = array(
    'time_tracker_reporting',
  );
  $views = array();
  foreach ($list as $view_name) {
    $view = call_user_func('_view_'. $view_name);
    $views[$view->name] = $view;
  }
  return $views;
}

/**
 * The main time tracker reporting view.
 */
function _view_time_tracker_reporting() {
  $view = new view;
  $view->name = 'time_tracker_reports';
  $view->description = 'A view for reporting on time tracker data.';
  $view->tag = '';
  $view->view_php = '';
  $view->base_table = 'time_tracker_entry';
  $view->is_cacheable = FALSE;
  $view->api_version = 2;
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */
  $handler = $view->new_display('default', 'Defaults', 'default');
  $handler->override_option('relationships', array(
    'uid' => array(
      'label' => 'User',
      'required' => 0,
      'id' => 'uid',
      'table' => 'time_tracker_entry',
      'field' => 'uid',
      'relationship' => 'none',
    ),
    'cid' => array(
      'label' => 'Comment',
      'required' => 0,
      'id' => 'cid',
      'table' => 'time_tracker_entry',
      'field' => 'cid',
      'relationship' => 'none',
    ),
    'nid' => array(
      'label' => 'Node',
      'required' => 0,
      'id' => 'nid',
      'table' => 'time_tracker_entry',
      'field' => 'nid',
      'relationship' => 'none',
    ),
    'activity' => array(
      'label' => 'Activity',
      'required' => 0,
      'id' => 'activity',
      'table' => 'time_tracker_entry',
      'field' => 'activity',
      'override' => array(
        'button' => 'Override',
      ),
      'relationship' => 'none',
    ),
    'pid' => array(
      'label' => 'project',
      'required' => 0,
      'id' => 'pid',
      'table' => 'casetracker_case',
      'field' => 'pid',
      'relationship' => 'nid',
    ),
  ));
  $handler->override_option('fields', array(
    'name' => array(
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
      'link_to_user' => 1,
      'overwrite_anonymous' => 0,
      'anonymous_text' => '',
      'exclude' => 0,
      'id' => 'name',
      'table' => 'users',
      'field' => 'name',
      'relationship' => 'uid',
      'override' => array(
        'button' => 'Override',
      ),
    ),
    'title_1' => array(
      'label' => 'Project',
      'alter' => array(
        'alter_text' => 0,
        'text' => '',
        'make_link' => 0,
        'path' => '',
        'link_class' => '',
        'alt' => '',
        'prefix' => '',
        'suffix' => '',
        'help' => '',
        'trim' => 0,
        'max_length' => '',
        'word_boundary' => 1,
        'ellipsis' => 1,
        'strip_tags' => 0,
        'html' => 0,
      ),
      'link_to_node' => 1,
      'exclude' => 0,
      'id' => 'title_1',
      'table' => 'node',
      'field' => 'title',
      'relationship' => 'nid',
    ),
    'title' => array(
      'label' => 'Title',
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
        'strip_tags' => 0,
        'html' => 0,
      ),
      'empty' => '',
      'hide_empty' => 0,
      'empty_zero' => 0,
      'link_to_node' => 1,
      'exclude' => 0,
      'id' => 'title',
      'table' => 'node',
      'field' => 'title',
      'relationship' => 'nid',
    ),
    'comment' => array(
      'label' => 'Body',
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
        'trim' => 1,
        'max_length' => '200',
        'word_boundary' => 1,
        'ellipsis' => 1,
        'html' => 0,
        'strip_tags' => 0,
      ),
      'empty' => '',
      'hide_empty' => 0,
      'empty_zero' => 0,
      'exclude' => 0,
      'id' => 'comment',
      'table' => 'comments',
      'field' => 'comment',
      'relationship' => 'cid',
    ),
    'note' => array(
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
      'exclude' => 0,
      'id' => 'note',
      'table' => 'time_tracker_entry',
      'field' => 'note',
      'override' => array(
        'button' => 'Override',
      ),
      'relationship' => 'none',
    ),
    'timestamp' => array(
      'label' => 'Timestamp',
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
      'custom_date_format' => 'M jS, Y',
      'reldate' => '0',
      'exclude' => 0,
      'id' => 'timestamp',
      'table' => 'time_tracker_entry',
      'field' => 'timestamp',
      'relationship' => 'none',
    ),
    'duration' => array(
      'label' => 'Duration',
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
        'strip_tags' => 0,
        'html' => 0,
      ),
      'empty' => '',
      'hide_empty' => 0,
      'empty_zero' => 0,
      'set_precision' => FALSE,
      'precision' => 0,
      'decimal' => '.',
      'separator' => ',',
      'prefix' => '',
      'suffix' => ' hours',
      'exclude' => 0,
      'id' => 'duration',
      'table' => 'time_tracker_entry',
      'field' => 'duration',
      'relationship' => 'none',
    ),
  ));
  $handler->override_option('sorts', array(
    'timestamp' => array(
      'id' => 'timestamp',
      'table' => 'time_tracker_entry',
      'field' => 'timestamp',
    ),
  ));
  $handler->override_option('filters', array(
    'uid' => array(
      'operator' => 'in',
      'value' => '',
      'group' => '0',
      'exposed' => TRUE,
      'expose' => array(
        'use_operator' => 0,
        'operator' => 'uid_op',
        'identifier' => 'uid',
        'label' => 'Name',
        'optional' => 1,
        'remember' => 0,
        'reduce' => 0,
      ),
      'id' => 'uid',
      'table' => 'users',
      'field' => 'uid',
      'relationship' => 'uid',
    ),
    'timestamp' => array(
      'operator' => 'between',
      'value' => array(
        'type' => 'relative',
        'value' => '',
        'min' => 'First Of This Month',
        'max' => 'Last Of This Month',
      ),
      'group' => '0',
      'exposed' => TRUE,
      'expose' => array(
        'use_operator' => 0,
        'operator' => 'timestamp_op',
        'identifier' => 'timestamp',
        'label' => 'Entered Between',
        'optional' => 1,
        'remember' => 0,
      ),
      'id' => 'timestamp',
      'table' => 'time_tracker_entry',
      'field' => 'timestamp',
      'relationship' => 'none',
      'override' => array(
        'button' => 'Override',
      ),
    ),
    'pid' => array(
      'operator' => 'or',
      'value' => array(),
      'group' => '0',
      'exposed' => TRUE,
      'expose' => array(
        'use_operator' => 0,
        'operator' => 'pid_op',
        'identifier' => 'pid',
        'label' => 'Project',
        'optional' => 1,
        'single' => 1,
        'remember' => 0,
        'reduce' => 0,
      ),
      'id' => 'pid',
      'table' => 'casetracker_case',
      'field' => 'pid',
      'relationship' => 'nid',
      'reduce_duplicates' => 0,
    ),
  ));
  $handler->override_option('access', array(
    'type' => 'role',
    'role' => array(
      '4' => 4,
    ),
  ));
  $handler->override_option('cache', array(
    'type' => 'none',
  ));
  $handler->override_option('items_per_page', 0);
  $handler->override_option('style_plugin', 'views_calc');
  $handler->override_option('style_options', array(
    'grouping' => '',
    'override' => 1,
    'sticky' => 0,
    'order' => 'asc',
    'columns' => array(
      'name' => 'name',
      'title' => 'title',
      'comment' => 'comment',
      'note' => 'comment',
      'timestamp' => 'timestamp',
      'duration' => 'duration',
    ),
    'info' => array(
      'name' => array(
        'sortable' => 0,
        'separator' => '<br/>',
        'justification' => 'views_calc_justify_none',
        'has_calc' => 0,
        'calc' => array(),
      ),
      'title' => array(
        'sortable' => 0,
        'separator' => '',
        'justification' => 'views_calc_justify_none',
        'has_calc' => 0,
        'calc' => array(),
      ),
      'comment' => array(
        'separator' => '',
        'justification' => 'views_calc_justify_none',
        'has_calc' => 0,
        'calc' => array(),
      ),
      'note' => array(
        'separator' => '',
        'justification' => 'views_calc_justify_none',
        'has_calc' => 0,
        'calc' => array(),
      ),
      'timestamp' => array(
        'separator' => '',
        'justification' => 'views_calc_justify_none',
        'has_calc' => 0,
        'calc' => array(),
      ),
      'duration' => array(
        'separator' => '',
        'justification' => 'views_calc_justify_none',
        'has_calc' => 1,
        'calc' => array(
          'SUM' => 'SUM',
        ),
      ),
    ),
    'default' => '-1',
    'detailed_values' => '0',
  ));
  $handler = $view->new_display('page', 'Page w/Casetracker', 'page_1');
  $handler->override_option('path', 'time_tracker/casetracker_reports');
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
  $handler = $view->new_display('page', 'Page', 'page_2');
  $handler->override_option('relationships', array(
    'uid' => array(
      'label' => 'User',
      'required' => 0,
      'id' => 'uid',
      'table' => 'time_tracker_entry',
      'field' => 'uid',
      'relationship' => 'none',
    ),
    'cid' => array(
      'label' => 'Comment',
      'required' => 0,
      'id' => 'cid',
      'table' => 'time_tracker_entry',
      'field' => 'cid',
      'relationship' => 'none',
    ),
    'nid' => array(
      'label' => 'Node',
      'required' => 0,
      'id' => 'nid',
      'table' => 'time_tracker_entry',
      'field' => 'nid',
      'relationship' => 'none',
    ),
    'activity' => array(
      'label' => 'Activity',
      'required' => 0,
      'id' => 'activity',
      'table' => 'time_tracker_entry',
      'field' => 'activity',
      'override' => array(
        'button' => 'Override',
      ),
      'relationship' => 'none',
    ),
  ));
  $handler->override_option('fields', array(
    'name' => array(
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
      'link_to_user' => 1,
      'overwrite_anonymous' => 0,
      'anonymous_text' => '',
      'exclude' => 0,
      'id' => 'name',
      'table' => 'users',
      'field' => 'name',
      'relationship' => 'uid',
      'override' => array(
        'button' => 'Override',
      ),
    ),
    'title' => array(
      'label' => 'Title',
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
        'strip_tags' => 0,
        'html' => 0,
      ),
      'empty' => '',
      'hide_empty' => 0,
      'empty_zero' => 0,
      'link_to_node' => 1,
      'exclude' => 0,
      'id' => 'title',
      'table' => 'node',
      'field' => 'title',
      'relationship' => 'nid',
    ),
    'comment' => array(
      'label' => 'Body',
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
        'trim' => 1,
        'max_length' => '200',
        'word_boundary' => 1,
        'ellipsis' => 1,
        'html' => 0,
        'strip_tags' => 0,
      ),
      'empty' => '',
      'hide_empty' => 0,
      'empty_zero' => 0,
      'exclude' => 0,
      'id' => 'comment',
      'table' => 'comments',
      'field' => 'comment',
      'relationship' => 'cid',
    ),
    'note' => array(
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
      'exclude' => 0,
      'id' => 'note',
      'table' => 'time_tracker_entry',
      'field' => 'note',
      'override' => array(
        'button' => 'Use default',
      ),
      'relationship' => 'none',
    ),
    'timestamp' => array(
      'label' => 'Timestamp',
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
      'custom_date_format' => 'M jS, Y',
      'reldate' => '0',
      'exclude' => 0,
      'id' => 'timestamp',
      'table' => 'time_tracker_entry',
      'field' => 'timestamp',
      'relationship' => 'none',
    ),
    'duration' => array(
      'label' => 'Duration',
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
        'strip_tags' => 0,
        'html' => 0,
      ),
      'empty' => '',
      'hide_empty' => 0,
      'empty_zero' => 0,
      'set_precision' => FALSE,
      'precision' => 0,
      'decimal' => '.',
      'separator' => ',',
      'prefix' => '',
      'suffix' => ' hours',
      'exclude' => 0,
      'id' => 'duration',
      'table' => 'time_tracker_entry',
      'field' => 'duration',
      'relationship' => 'none',
    ),
  ));
  $handler->override_option('filters', array(
    'uid' => array(
      'operator' => 'in',
      'value' => '',
      'group' => '0',
      'exposed' => TRUE,
      'expose' => array(
        'use_operator' => 0,
        'operator' => 'uid_op',
        'identifier' => 'uid',
        'label' => 'Name',
        'optional' => 1,
        'remember' => 0,
        'reduce' => 0,
      ),
      'id' => 'uid',
      'table' => 'users',
      'field' => 'uid',
      'relationship' => 'uid',
    ),
    'timestamp' => array(
      'operator' => 'between',
      'value' => array(
        'type' => 'relative',
        'value' => '',
        'min' => 'First Of This Month',
        'max' => 'Last Of This Month',
      ),
      'group' => '0',
      'exposed' => TRUE,
      'expose' => array(
        'use_operator' => 0,
        'operator' => 'timestamp_op',
        'identifier' => 'timestamp',
        'label' => 'Entered Between',
        'optional' => 1,
        'remember' => 0,
      ),
      'id' => 'timestamp',
      'table' => 'time_tracker_entry',
      'field' => 'timestamp',
      'relationship' => 'none',
      'override' => array(
        'button' => 'Override',
      ),
    ),
  ));
  $handler->override_option('path', 'time_tracker/reports');
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
  $translatables['time_tracker_reports'] = array(
    t('Defaults'),
    t('Page'),
    t('Page w/Casetracker'),
  );

  return $view;
}