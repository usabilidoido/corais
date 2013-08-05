<?php

/**
 * Implementation of hook_views_default_views().
 */
function xref_views_default_views() {
  $list = array('xref');
  $views = array();
  foreach ($list as $view_name) {
    $view = call_user_func('_xref_views_'. $view_name);
    $views[$view->name] = $view;
  }
  return $views;
}

function _xref_views_xref() {
$view = new view;
$view->name = 'xref';
$view->description = 'Crossref';
$view->tag = 'crossref';
$view->view_php = '';
$view->base_table = 'node';
$view->is_cacheable = FALSE;
$view->api_version = 2;
$view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */
$handler = $view->new_display('default', 'Defaults', 'default');
$handler->override_option('fields', array(
  'title' => array(
    'label' => 'Referenced by',
    'link_to_node' => 1,
    'exclude' => 0,
    'id' => 'title',
    'table' => 'node',
    'field' => 'title',
    'relationship' => 'none',
    'override' => array(
      'button' => 'Override',
    ),
  ),
  'last_updated' => array(
    'label' => 'Last post',
    'date_format' => 'reldate',
    'custom_date_format' => '',
    'exclude' => 0,
    'id' => 'last_updated',
    'table' => 'node_comment_statistics',
    'field' => 'last_updated',
    'relationship' => 'none',
  ),
  'name' => array(
    'label' => 'Author',
    'link_to_user' => 1,
    'exclude' => 0,
    'id' => 'name',
    'table' => 'users',
    'field' => 'name',
    'relationship' => 'none',
  ),
));
$handler->override_option('sorts', array(
  'created' => array(
    'order' => 'DESC',
    'granularity' => 'second',
    'id' => 'created',
    'table' => 'node',
    'field' => 'created',
    'relationship' => 'none',
  ),
));
$handler->override_option('filters', array(
  'status' => array(
    'operator' => '=',
    'value' => 1,
    'group' => '0',
    'exposed' => FALSE,
    'expose' => array(
      'operator' => FALSE,
      'label' => '',
    ),
    'id' => 'status',
    'table' => 'node',
    'field' => 'status',
    'relationship' => 'none',
  ),
  'current' => array(
    'operator' => 'active',
    'value' => 'all',
    'group' => '0',
    'exposed' => FALSE,
    'expose' => array(
      'operator' => FALSE,
      'label' => '',
    ),
    'id' => 'current',
    'table' => 'spaces',
    'field' => 'current',
    'relationship' => 'none',
  ),
));
$handler->override_option('access', array(
  'type' => 'none',
));
$handler->override_option('cache', array(
  'type' => 'none',
));
$handler->override_option('items_per_page', 5);
$handler->override_option('use_pager', '1');
$handler->override_option('style_plugin', 'table');
$handler->override_option('style_options', array(
  'grouping' => '',
  'override' => 1,
  'sticky' => 0,
  'order' => 'asc',
  'columns' => array(
    'title' => 'title',
    'last_updated' => 'last_updated',
    'name' => 'name',
  ),
  'info' => array(
    'title' => array(
      'sortable' => 0,
      'separator' => '',
    ),
    'last_updated' => array(
      'sortable' => 0,
      'separator' => '',
    ),
    'name' => array(
      'sortable' => 0,
      'separator' => '',
    ),
  ),
  'default' => '-1',
));
$translatables['xref'] = array(
  t('Defaults'),
);
return $view;
}
