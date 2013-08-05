<?php

/**
 * Implementation of hook_views_default_views().
 */
function atrium_book_views_default_views() {
  $views = array();

  // Exported view: book_current
  $view = new view;
  $view->name = 'book_current';
  $view->description = 'Atrium Book: helper view to look up root book pages';
  $view->tag = 'atrium_book';
  $view->base_table = 'node';
  $view->core = 0;
  $view->api_version = '2';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */
  $handler = $view->new_display('default', 'Defaults', 'default');
  $handler->override_option('fields', array(
    'title' => array(
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
      'spaces' => array(
        'frontpage' => 0,
        'type' => 'spaces_og',
      ),
      'exclude' => 0,
      'id' => 'title',
      'table' => 'node',
      'field' => 'title',
      'relationship' => 'none',
    ),
  ));
  $handler->override_option('sorts', array(
    'title' => array(
      'order' => 'ASC',
      'id' => 'title',
      'table' => 'node',
      'field' => 'title',
      'relationship' => 'none',
    ),
  ));
  $handler->override_option('arguments', array(
    'type' => array(
      'default_action' => 'ignore',
      'style_plugin' => 'default_summary',
      'style_options' => array(),
      'wildcard' => 'all',
      'wildcard_substitution' => 'All',
      'title' => '',
      'breadcrumb' => '',
      'default_argument_type' => 'fixed',
      'default_argument' => '',
      'validate_type' => 'none',
      'validate_fail' => 'not found',
      'id' => 'type',
      'table' => 'node',
      'field' => 'type',
      'validate_user_argument_type' => 'uid',
      'validate_user_roles' => array(
        2 => 0,
        3 => 0,
        4 => 0,
      ),
      'relationship' => 'none',
      'default_options_div_prefix' => '',
      'default_argument_user' => 0,
      'default_argument_fixed' => '',
      'default_argument_php' => '',
      'validate_argument_node_type' => array(
        'blog' => 0,
        'book' => 0,
        'event' => 0,
        'feed_ical' => 0,
        'feed_ical_item' => 0,
        'casetracker_basic_case' => 0,
        'casetracker_basic_project' => 0,
        'group' => 0,
        'profile' => 0,
        'shoutbox' => 0,
        'admin_message' => 0,
        'feedapi_node' => 0,
        'feed_reader' => 0,
        'imageset' => 0,
        'page' => 0,
      ),
      'validate_argument_node_access' => 0,
      'validate_argument_nid_type' => 'nid',
      'validate_argument_vocabulary' => array(
        5 => 0,
        1 => 0,
        2 => 0,
        6 => 0,
        4 => 0,
      ),
      'validate_argument_type' => 'tid',
      'validate_argument_transform' => 0,
      'validate_user_restrict_roles' => 0,
      'validate_argument_is_member' => 0,
      'validate_argument_php' => '',
    ),
  ));
  $handler->override_option('filters', array(
    'depth' => array(
      'operator' => '=',
      'value' => array(
        'value' => '1',
        'min' => '',
        'max' => '',
      ),
      'group' => '0',
      'exposed' => FALSE,
      'expose' => array(
        'operator' => FALSE,
        'label' => '',
      ),
      'id' => 'depth',
      'table' => 'book_menu_links',
      'field' => 'depth',
      'relationship' => 'none',
    ),
    'status' => array(
      'operator' => '=',
      'value' => '1',
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
    'book_type' => array(
      'operator' => '=',
      'value' => '',
      'group' => '0',
      'exposed' => FALSE,
      'expose' => array(
        'operator' => FALSE,
        'label' => '',
      ),
      'id' => 'book_type',
      'table' => 'node',
      'field' => 'book_type',
      'relationship' => 'none',
    ),
  ));
  $handler->override_option('access', array(
    'type' => 'none',
  ));
  $handler->override_option('cache', array(
    'type' => 'none',
  ));
  $handler->override_option('items_per_page', 0);
  $translatables['book_current'] = array(
    t('Defaults'),
  );

  $views[$view->name] = $view;

  // Exported view: book_flag
  $view = new view;
  $view->name = 'book_flag';
  $view->description = '';
  $view->tag = '';
  $view->base_table = 'node';
  $view->core = 6;
  $view->api_version = '2';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */
  $handler = $view->new_display('default', 'Defaults', 'default');
  $handler->override_option('relationships', array(
    'nid' => array(
      'label' => 'Book parent',
      'required' => 0,
      'id' => 'nid',
      'table' => 'book_parent',
      'field' => 'nid',
      'override' => array(
        'button' => 'Override',
      ),
      'relationship' => 'none',
    ),
  ));
  $handler->override_option('fields', array(
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
        'help' => '',
        'trim' => 0,
        'max_length' => '',
        'word_boundary' => 1,
        'ellipsis' => 1,
        'strip_tags' => 0,
        'html' => 0,
      ),
      'link_to_node' => 0,
      'exclude' => 0,
      'id' => 'title',
      'table' => 'node',
      'field' => 'title',
      'override' => array(
        'button' => 'Override',
      ),
      'relationship' => 'none',
    ),
    'nid' => array(
      'label' => '',
      'alter' => array(
        'alter_text' => 1,
        'text' => 'Jump into',
        'make_link' => 1,
        'path' => 'node/[nid]/rearrange',
        'link_class' => '',
        'alt' => 'Jump to the rearrange page of this book page.',
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
      'link_to_node' => 0,
      'exclude' => 0,
      'id' => 'nid',
      'table' => 'node',
      'field' => 'nid',
      'override' => array(
        'button' => 'Override',
      ),
      'relationship' => 'none',
    ),
    'weight' => array(
      'label' => 'Weight',
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
      'set_precision' => FALSE,
      'precision' => 0,
      'decimal' => '.',
      'separator' => ',',
      'prefix' => '',
      'suffix' => '',
      'exclude' => 0,
      'id' => 'weight',
      'table' => 'book_menu_links',
      'field' => 'weight',
      'relationship' => 'none',
    ),
    'nid_1' => array(
      'label' => 'Parent Nid',
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
      'link_to_node' => 0,
      'exclude' => 0,
      'id' => 'nid_1',
      'table' => 'node',
      'field' => 'nid',
      'override' => array(
        'button' => 'Override',
      ),
      'relationship' => 'nid',
    ),
    'phpcode' => array(
      'label' => 'Announce your knowledge level',
      'alter' => array(
        'alter_text' => 0,
        'text' => '',
        'make_link' => 0,
        'path' => '',
        'absolute' => 0,
        'link_class' => '',
        'alt' => '',
        'rel' => '',
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
      'hide_alter_empty' => 1,
      'value' => '<?php 
  $node = node_load($data->nid);
  print flag_create_link(\'studying\', $node->nid);
?>',
      'exclude' => 0,
      'id' => 'phpcode',
      'table' => 'customfield',
      'field' => 'phpcode',
      'relationship' => 'none',
    ),
    'phpcode_1' => array(
      'label' => '',
      'alter' => array(
        'alter_text' => 0,
        'text' => '',
        'make_link' => 0,
        'path' => '',
        'absolute' => 0,
        'link_class' => '',
        'alt' => '',
        'rel' => '',
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
      'hide_alter_empty' => 1,
      'value' => '<?php 
  $node = node_load($data->nid);
  print flag_create_link(\'used\', $node->nid);
?>',
      'exclude' => 0,
      'id' => 'phpcode_1',
      'table' => 'customfield',
      'field' => 'phpcode',
      'relationship' => 'none',
    ),
    'phpcode_2' => array(
      'label' => '',
      'alter' => array(
        'alter_text' => 0,
        'text' => '',
        'make_link' => 0,
        'path' => '',
        'absolute' => 0,
        'link_class' => '',
        'alt' => '',
        'rel' => '',
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
      'hide_alter_empty' => 1,
      'value' => '<?php 
  $node = node_load($data->nid);
  print flag_create_link(\'expert\', $node->nid);
?>',
      'exclude' => 0,
      'id' => 'phpcode_2',
      'table' => 'customfield',
      'field' => 'phpcode',
      'relationship' => 'none',
    ),
  ));
  $handler->override_option('sorts', array(
    'weight_1' => array(
      'order' => 'ASC',
      'id' => 'weight_1',
      'table' => 'book_menu_links',
      'field' => 'weight',
      'override' => array(
        'button' => 'Override',
      ),
      'relationship' => 'none',
    ),
    'p' => array(
      'order' => 'ASC',
      'id' => 'p',
      'table' => 'book_menu_links',
      'field' => 'p',
      'override' => array(
        'button' => 'Override',
      ),
      'relationship' => 'none',
    ),
  ));
  $handler->override_option('filters', array(
    'type' => array(
      'operator' => 'in',
      'value' => array(
        'book' => 'book',
      ),
      'group' => '0',
      'exposed' => FALSE,
      'expose' => array(
        'operator' => FALSE,
        'label' => '',
      ),
      'id' => 'type',
      'table' => 'node',
      'field' => 'type',
      'relationship' => 'none',
    ),
    'nid' => array(
      'operator' => '!=',
      'value' => array(
        'value' => '1665',
        'min' => '',
        'max' => '',
      ),
      'group' => '0',
      'exposed' => FALSE,
      'expose' => array(
        'operator' => FALSE,
        'label' => '',
      ),
      'id' => 'nid',
      'table' => 'node',
      'field' => 'nid',
      'relationship' => 'none',
    ),
    'status' => array(
      'operator' => '=',
      'value' => '1',
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
  ));
  $handler->override_option('access', array(
    'type' => 'perm',
    'perm' => 'access content',
  ));
  $handler->override_option('cache', array(
    'type' => 'none',
  ));
  $handler->override_option('title', 'My Knowledge');
  $handler->override_option('header', '<?php
print t(\'In this page you can announce your knowledge level regarding every knowledge in our tree. When you announce it, you will subscribe to new comment this knowledge.\');
?>');
  $handler->override_option('header_format', '7');
  $handler->override_option('header_empty', 0);
  $handler->override_option('empty', 'This book page doesn\'t contain any sub pages.');
  $handler->override_option('empty_format', '1');
  $handler->override_option('items_per_page', 0);
  $handler->override_option('use_pager', '1');
  $handler->override_option('style_plugin', 'draggabletable');
  $handler->override_option('style_options', array(
    'override' => 1,
    'sticky' => 0,
    'order' => 'asc',
    'summary' => '',
    'columns' => array(
      'title' => 'title',
      'nid' => 'title',
      'weight' => 'weight',
      'nid_1' => 'nid_1',
      'phpcode' => 'phpcode',
      'phpcode_1' => 'phpcode_1',
      'phpcode_2' => 'phpcode_2',
    ),
    'info' => array(
      'title' => array(
        'sortable' => 0,
        'separator' => ' - ',
      ),
      'nid' => array(
        'sortable' => 0,
        'separator' => '',
      ),
      'weight' => array(
        'sortable' => 0,
        'separator' => '',
      ),
      'nid_1' => array(
        'sortable' => 0,
        'separator' => '',
      ),
      'phpcode' => array(
        'separator' => '',
      ),
      'phpcode_1' => array(
        'separator' => '',
      ),
      'phpcode_2' => array(
        'separator' => '',
      ),
    ),
    'default' => -1,
    'tabledrag_order' => array(
      'field' => 'weight',
      'handler' => 'book',
    ),
    'tabledrag_order_visible' => array(
      'visible' => 0,
    ),
    'tabledrag_hierarchy' => array(
      'field' => 'nid_1',
      'handler' => 'book',
    ),
    'tabledrag_hierarchy_visible' => array(
      'visible' => 0,
    ),
    'draggableviews_depth_limit' => '-1',
    'draggableviews_repair' => array(
      'repair' => 'repair',
    ),
    'tabledrag_types_add' => 'Add type',
    'tabledrag_expand' => array(
      'expand_links' => 'expand_links',
      'collapsed' => 0,
      'by_uid' => 0,
    ),
    'draggableviews_extensions' => array(
      'extension_top' => '3',
      'extension_bottom' => '3',
    ),
    'tabledrag_lock' => array(
      'lock' => 'lock',
    ),
    'draggableviews_default_on_top' => '1',
    'draggableviews_button_text' => 'Save order',
    'draggableviews_arguments' => array(
      'use_args' => 0,
    ),
    'draggableviews_book_radios' => '2',
    'draggableviews_book_argument' => 'none',
    'draggableviews_book_filter' => 'type',
  ));
  $handler = $view->new_display('page', 'Page', 'page_1');
  $handler->override_option('path', 'knowledge/me');
  $handler->override_option('menu', array(
    'type' => 'tab',
    'title' => 'Me',
    'description' => 'Organize your book pages.',
    'weight' => '10',
    'name' => 'primary-links',
  ));
  $handler->override_option('tab_options', array(
    'type' => 'none',
    'title' => '',
    'description' => '',
    'weight' => 0,
    'name' => 'navigation',
  ));
  $translatables['book_flag'] = array(
    t('<?php
print t(\'In this page you can announce your knowledge level regarding every knowledge in our tree. When you announce it, you will subscribe to new comment this knowledge.\');
?>'),
    t('Defaults'),
    t('My Knowledge'),
    t('Page'),
    t('This book page doesn\'t contain any sub pages.'),
  );

  $views[$view->name] = $view;

  // Exported view: book_listing
  $view = new view;
  $view->name = 'book_listing';
  $view->description = 'Atrium > Book';
  $view->tag = 'atrium';
  $view->base_table = 'node';
  $view->core = 0;
  $view->api_version = '2';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */
  $handler = $view->new_display('default', 'Defaults', 'default');
  $handler->override_option('relationships', array(
    'bid' => array(
      'label' => 'Book',
      'required' => 0,
      'id' => 'bid',
      'table' => 'book',
      'field' => 'bid',
      'relationship' => 'none',
    ),
  ));
  $handler->override_option('fields', array(
    'feature' => array(
      'label' => '',
      'alter' => array(
        'alter_text' => 0,
        'text' => '',
        'make_link' => 0,
        'path' => '',
        'alt' => '',
        'prefix' => '',
        'suffix' => '',
        'help' => '',
        'trim' => 0,
        'max_length' => '',
        'word_boundary' => 1,
        'ellipsis' => 1,
        'html' => 0,
      ),
      'link_to_node' => 0,
      'exclude' => 0,
      'id' => 'feature',
      'table' => 'spaces_node',
      'field' => 'feature',
      'relationship' => 'none',
    ),
    'atrium_activity_timestamp' => array(
      'label' => '',
      'alter' => array(
        'alter_text' => FALSE,
        'text' => '',
        'make_link' => FALSE,
        'path' => '',
        'alt' => '',
        'link_class' => '',
        'prefix' => '',
        'suffix' => '',
        'trim' => FALSE,
        'max_length' => '',
        'word_boundary' => TRUE,
        'ellipsis' => TRUE,
        'strip_tags' => FALSE,
        'html' => FALSE,
      ),
      'time_horizon' => 30,
      'exclude' => 0,
      'id' => 'atrium_activity_timestamp',
      'table' => 'comments',
      'field' => 'atrium_activity_timestamp',
      'override' => array(
        'button' => 'Override',
      ),
      'relationship' => 'none',
      'date_format' => 'small',
      'custom_date_format' => '',
      'reldate' => 'reldate',
    ),
    'atrium_activity' => array(
      'label' => '',
      'alter' => array(
        'alter_text' => FALSE,
        'text' => '',
        'make_link' => FALSE,
        'path' => '',
        'alt' => '',
        'link_class' => '',
        'prefix' => '',
        'suffix' => '',
        'trim' => FALSE,
        'max_length' => '',
        'word_boundary' => TRUE,
        'ellipsis' => TRUE,
        'strip_tags' => FALSE,
        'html' => FALSE,
      ),
      'time_horizon' => '90',
      'exclude' => 0,
      'id' => 'atrium_activity',
      'table' => 'comments',
      'field' => 'atrium_activity',
      'override' => array(
        'button' => 'Override',
      ),
      'relationship' => 'none',
    ),
    'log' => array(
      'label' => '',
      'alter' => array(
        'alter_text' => 1,
        'text' => '<div class=\'description\'>[log]</div>',
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
      'exclude' => 0,
      'id' => 'log',
      'table' => 'node_revisions',
      'field' => 'log',
      'relationship' => 'none',
      'override' => array(
        'button' => 'Override',
      ),
    ),
  ));
  $handler->override_option('filters', array(
    'status' => array(
      'operator' => '=',
      'value' => '1',
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
    'type' => array(
      'operator' => 'in',
      'value' => array(
        'book' => 'book',
        'service_provider' => 'service_provider',
        'toolkit' => 'toolkit',
      ),
      'group' => '0',
      'exposed' => FALSE,
      'expose' => array(
        'operator' => FALSE,
        'label' => '',
      ),
      'id' => 'type',
      'table' => 'node',
      'field' => 'type',
      'relationship' => 'none',
      'override' => array(
        'button' => 'Sobrescrever',
      ),
    ),
    'current' => array(
      'operator' => '=',
      'value' => '',
      'group' => '0',
      'exposed' => FALSE,
      'expose' => array(
        'operator' => FALSE,
        'label' => '',
      ),
      'id' => 'current',
      'table' => 'spaces',
      'field' => 'current',
      'override' => array(
        'button' => 'Sobrescrever',
      ),
      'relationship' => 'none',
    ),
  ));
  $handler->override_option('access', array(
    'type' => 'spaces_feature',
    'spaces_feature' => 'atrium_book',
  ));
  $handler->override_option('cache', array(
    'type' => 'none',
  ));
  $handler->override_option('title', 'Árvore do Conhecimento');
  $handler->override_option('header_format', '4');
  $handler->override_option('header_empty', 0);
  $handler->override_option('empty', 'Nenhuma atualização recente encontrada.');
  $handler->override_option('items_per_page', 0);
  $handler->override_option('use_pager', '1');
  $handler->override_option('style_plugin', 'table');
  $handler->override_option('style_options', array(
    'grouping' => '',
    'override' => 1,
    'sticky' => 0,
    'order' => 'asc',
    'columns' => array(
      'feature' => 'feature',
      'atrium_activity_timestamp' => 'atrium_activity_timestamp',
      'atrium_activity' => 'atrium_activity',
      'log' => 'atrium_activity',
    ),
    'info' => array(
      'feature' => array(
        'separator' => '',
      ),
      'atrium_activity_timestamp' => array(
        'separator' => '',
      ),
      'atrium_activity' => array(
        'separator' => '',
      ),
      'log' => array(
        'separator' => '',
      ),
    ),
    'default' => '-1',
  ));
  $handler = $view->new_display('page', 'Page: Latest changes', 'page_1');
  $handler->override_option('fields', array(
    'atrium_activity' => array(
      'label' => '',
      'alter' => array(
        'alter_text' => FALSE,
        'text' => '',
        'make_link' => FALSE,
        'path' => '',
        'alt' => '',
        'link_class' => '',
        'prefix' => '',
        'suffix' => '',
        'trim' => FALSE,
        'max_length' => '',
        'word_boundary' => TRUE,
        'ellipsis' => TRUE,
        'strip_tags' => FALSE,
        'html' => FALSE,
      ),
      'time_horizon' => '90',
      'exclude' => 0,
      'id' => 'atrium_activity',
      'table' => 'comments',
      'field' => 'atrium_activity',
      'override' => array(
        'button' => 'Override',
      ),
      'relationship' => 'none',
    ),
    'log' => array(
      'label' => '',
      'alter' => array(
        'alter_text' => 1,
        'text' => '<div class=\'description\'>[log]</div>',
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
      'exclude' => 0,
      'id' => 'log',
      'table' => 'node_revisions',
      'field' => 'log',
      'relationship' => 'none',
      'override' => array(
        'button' => 'Override',
      ),
    ),
  ));
  $handler->override_option('items_per_page', 20);
  $handler->override_option('path', 'knowledge');
  $handler->override_option('menu', array(
    'type' => 'normal',
    'title' => 'Knowledge Tree',
    'description' => '',
    'weight' => '0',
    'name' => 'features',
  ));
  $handler->override_option('tab_options', array(
    'type' => 'tab',
    'title' => 'Notebook',
    'description' => '',
    'weight' => '0',
    'name' => 'navigation',
  ));
  $handler = $view->new_display('page', 'Page: Archived books', 'page_2');
  $handler->override_option('fields', array(
    'title' => array(
      'label' => '',
      'alter' => array(
        'alter_text' => 0,
        'text' => '',
        'make_link' => 0,
        'path' => '',
        'alt' => '',
        'prefix' => '',
        'suffix' => '',
        'help' => '',
        'trim' => 0,
        'max_length' => '',
        'word_boundary' => 1,
        'ellipsis' => 1,
        'html' => 0,
      ),
      'link_to_node' => 1,
      'exclude' => 0,
      'id' => 'title',
      'table' => 'node',
      'field' => 'title',
      'relationship' => 'none',
      'override' => array(
        'button' => 'Use default',
      ),
    ),
    'changed' => array(
      'label' => '',
      'alter' => array(
        'alter_text' => 0,
        'text' => '',
        'make_link' => 0,
        'path' => '',
        'alt' => '',
        'prefix' => '',
        'suffix' => '',
        'help' => '',
        'trim' => 0,
        'max_length' => '',
        'word_boundary' => 1,
        'ellipsis' => 1,
        'html' => 0,
      ),
      'date_format' => 'small',
      'custom_date_format' => '',
      'reldate' => 'reldate',
      'exclude' => 0,
      'id' => 'changed',
      'table' => 'node',
      'field' => 'changed',
      'override' => array(
        'button' => 'Use default',
      ),
      'relationship' => 'none',
    ),
  ));
  $handler->override_option('sorts', array(
    'title' => array(
      'order' => 'ASC',
      'id' => 'title',
      'table' => 'node',
      'field' => 'title',
      'override' => array(
        'button' => 'Use default',
      ),
      'relationship' => 'none',
    ),
  ));
  $handler->override_option('filters', array(
    'status' => array(
      'operator' => '=',
      'value' => '0',
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
      'override' => array(
        'button' => 'Use default',
      ),
    ),
    'type' => array(
      'operator' => 'in',
      'value' => array(
        'book' => 'book',
      ),
      'group' => '0',
      'exposed' => FALSE,
      'expose' => array(
        'operator' => FALSE,
        'label' => '',
      ),
      'id' => 'type',
      'table' => 'node',
      'field' => 'type',
      'relationship' => 'none',
    ),
    'current' => array(
      'operator' => 'all',
      'value' => '',
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
    'depth' => array(
      'operator' => '=',
      'value' => array(
        'value' => '1',
        'min' => '',
        'max' => '',
      ),
      'group' => '0',
      'exposed' => FALSE,
      'expose' => array(
        'operator' => FALSE,
        'label' => '',
      ),
      'id' => 'depth',
      'table' => 'book_menu_links',
      'field' => 'depth',
      'override' => array(
        'button' => 'Use default',
      ),
      'relationship' => 'none',
    ),
    'feature' => array(
      'operator' => '=',
      'value' => '',
      'group' => '0',
      'exposed' => FALSE,
      'expose' => array(
        'operator' => FALSE,
        'label' => '',
      ),
      'id' => 'feature',
      'table' => 'node',
      'field' => 'feature',
      'relationship' => 'bid',
    ),
  ));
  $handler->override_option('empty', 'Nenhum livro arquivado encontrado.');
  $handler->override_option('use_pager', '0');
  $handler->override_option('style_plugin', 'grid');
  $handler->override_option('style_options', array(
    'grouping' => '',
    'columns' => '2',
    'alignment' => 'vertical',
  ));
  $handler->override_option('path', 'knowledge/archived');
  $handler->override_option('menu', array(
    'type' => 'none',
    'title' => 'Archived',
    'description' => '',
    'weight' => '1',
    'name' => 'navigation',
  ));
  $handler->override_option('tab_options', array(
    'type' => 'none',
    'title' => '',
    'description' => '',
    'weight' => 0,
    'name' => 'navigation',
  ));
  $handler = $view->new_display('page', 'Top pages', 'page_3');
  $handler->override_option('fields', array(
    'feature' => array(
      'label' => '',
      'alter' => array(
        'alter_text' => 0,
        'text' => '',
        'make_link' => 0,
        'path' => '',
        'alt' => '',
        'prefix' => '',
        'suffix' => '',
        'help' => '',
        'trim' => 0,
        'max_length' => '',
        'word_boundary' => 1,
        'ellipsis' => 1,
        'html' => 0,
      ),
      'link_to_node' => 0,
      'exclude' => 0,
      'id' => 'feature',
      'table' => 'spaces_node',
      'field' => 'feature',
      'relationship' => 'none',
    ),
    'title' => array(
      'label' => 'Título',
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
      'spaces' => array(
        'frontpage' => 0,
        'type' => 'spaces_og',
      ),
      'exclude' => 0,
      'id' => 'title',
      'table' => 'node',
      'field' => 'title',
      'override' => array(
        'button' => 'Usar o padrão',
      ),
      'relationship' => 'none',
    ),
    'totalcount' => array(
      'label' => 'Visualizações',
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
      'set_precision' => FALSE,
      'precision' => 0,
      'decimal' => '.',
      'separator' => '.',
      'prefix' => '',
      'suffix' => '',
      'exclude' => 0,
      'id' => 'totalcount',
      'table' => 'node_counter',
      'field' => 'totalcount',
      'override' => array(
        'button' => 'Usar o padrão',
      ),
      'relationship' => 'none',
    ),
  ));
  $handler->override_option('sorts', array(
    'totalcount' => array(
      'order' => 'DESC',
      'id' => 'totalcount',
      'table' => 'node_counter',
      'field' => 'totalcount',
      'override' => array(
        'button' => 'Usar o padrão',
      ),
      'relationship' => 'none',
    ),
  ));
  $handler->override_option('path', 'knowledge/top');
  $handler->override_option('menu', array(
    'type' => 'tab',
    'title' => 'Top pages',
    'description' => '',
    'weight' => '0',
    'name' => 'navigation',
  ));
  $handler->override_option('tab_options', array(
    'type' => 'none',
    'title' => '',
    'description' => '',
    'weight' => 0,
    'name' => 'navigation',
  ));
  $handler = $view->new_display('feed', 'Knowledge Feed', 'feed_1');
  $handler->override_option('relationships', array());
  $handler->override_option('fields', array(
    'atrium_activity_timestamp' => array(
      'label' => '',
      'alter' => array(
        'alter_text' => FALSE,
        'text' => '',
        'make_link' => FALSE,
        'path' => '',
        'alt' => '',
        'link_class' => '',
        'prefix' => '',
        'suffix' => '',
        'trim' => FALSE,
        'max_length' => '',
        'word_boundary' => TRUE,
        'ellipsis' => TRUE,
        'strip_tags' => FALSE,
        'html' => FALSE,
      ),
      'time_horizon' => 30,
      'exclude' => 0,
      'id' => 'atrium_activity_timestamp',
      'table' => 'comments',
      'field' => 'atrium_activity_timestamp',
      'override' => array(
        'button' => 'Override',
      ),
      'relationship' => 'none',
      'date_format' => 'small',
      'custom_date_format' => '',
      'reldate' => 'reldate',
    ),
    'atrium_activity' => array(
      'label' => '',
      'alter' => array(
        'alter_text' => FALSE,
        'text' => '',
        'make_link' => FALSE,
        'path' => '',
        'alt' => '',
        'link_class' => '',
        'prefix' => '',
        'suffix' => '',
        'trim' => FALSE,
        'max_length' => '',
        'word_boundary' => TRUE,
        'ellipsis' => TRUE,
        'strip_tags' => FALSE,
        'html' => FALSE,
      ),
      'time_horizon' => '90',
      'exclude' => 0,
      'id' => 'atrium_activity',
      'table' => 'comments',
      'field' => 'atrium_activity',
      'override' => array(
        'button' => 'Override',
      ),
      'relationship' => 'none',
    ),
    'log' => array(
      'label' => '',
      'alter' => array(
        'alter_text' => 1,
        'text' => '<div class=\'description\'>[log]</div>',
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
      'exclude' => 0,
      'id' => 'log',
      'table' => 'node_revisions',
      'field' => 'log',
      'relationship' => 'none',
      'override' => array(
        'button' => 'Override',
      ),
    ),
    'body' => array(
      'label' => 'Corpo',
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
      'id' => 'body',
      'table' => 'node_revisions',
      'field' => 'body',
      'override' => array(
        'button' => 'Sobrescrever',
      ),
      'relationship' => 'none',
    ),
    'title' => array(
      'label' => 'Título',
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
      'spaces' => array(
        'frontpage' => 0,
        'type' => 'spaces_og',
      ),
      'exclude' => 0,
      'id' => 'title',
      'table' => 'node',
      'field' => 'title',
      'override' => array(
        'button' => 'Sobrescrever',
      ),
      'relationship' => 'none',
    ),
    'view_node' => array(
      'label' => 'Link',
      'alter' => array(
        'alter_text' => 0,
        'text' => '',
        'make_link' => 0,
        'path' => '',
        'absolute' => 0,
        'link_class' => '',
        'alt' => '',
        'rel' => '',
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
      'hide_alter_empty' => 1,
      'text' => '',
      'exclude' => 0,
      'id' => 'view_node',
      'table' => 'node',
      'field' => 'view_node',
      'override' => array(
        'button' => 'Usar o padrão',
      ),
      'relationship' => 'none',
    ),
    'view_node_1' => array(
      'label' => 'Talk Link',
      'alter' => array(
        'alter_text' => 1,
        'text' => '[view_node]/talk',
        'make_link' => 0,
        'path' => '',
        'absolute' => 0,
        'link_class' => '',
        'alt' => '',
        'rel' => '',
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
      'hide_alter_empty' => 1,
      'text' => '',
      'exclude' => 0,
      'id' => 'view_node_1',
      'table' => 'node',
      'field' => 'view_node',
      'override' => array(
        'button' => 'Usar o padrão',
      ),
      'relationship' => 'none',
    ),
  ));
  $handler->override_option('filters', array(
    'status' => array(
      'operator' => '=',
      'value' => '1',
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
    'type' => array(
      'operator' => 'in',
      'value' => array(
        'book' => 'book',
      ),
      'group' => '0',
      'exposed' => FALSE,
      'expose' => array(
        'operator' => FALSE,
        'label' => '',
      ),
      'id' => 'type',
      'table' => 'node',
      'field' => 'type',
      'relationship' => 'none',
    ),
  ));
  $handler->override_option('style_plugin', 'rss_fields');
  $handler->override_option('style_options', array(
    'mission_description' => FALSE,
    'description' => '',
    'channel' => array(
      'core' => array(
        'views_rss_core' => array(
          'description' => '',
          'language' => '',
          'category' => '',
          'image' => '',
          'copyright' => '',
          'managingEditor' => '',
          'webMaster' => '',
          'generator' => '',
          'docs' => '',
          'cloud' => '',
          'ttl' => '',
          'skipHours' => '',
          'skipDays' => '',
        ),
      ),
    ),
    'item' => array(
      'core' => array(
        'views_rss_core' => array(
          'title' => 'title',
          'link' => 'view_node',
          'description' => '',
          'author' => '',
          'category' => '',
          'comments' => '',
          'enclosure' => '',
          'guid' => '',
          'pubDate' => '',
        ),
      ),
      'dc' => array(
        'views_rss_dc' => array(
          'title' => '',
          'creator' => '',
          'subject' => '',
          'description' => '',
          'publisher' => '',
          'contributor' => '',
          'date' => '',
          'type' => '',
          'format' => '',
          'identifier' => '',
          'source' => '',
          'language' => '',
          'relation' => '',
          'coverage' => '',
          'rights' => '',
        ),
      ),
    ),
    'feed_settings' => array(
      'absolute_paths' => 1,
      'feed_in_links' => 0,
    ),
  ));
  $handler->override_option('row_plugin', 'node_rss');
  $handler->override_option('row_options', array(
    'relationship' => 'none',
    'item_length' => 'default',
  ));
  $handler->override_option('path', 'knowledge/feed');
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
  $handler->override_option('displays', array());
  $handler->override_option('sitename_title', FALSE);
  $translatables['book_listing'] = array(
    t('Defaults'),
    t('Knowledge Base'),
    t('Knowledge Feed'),
    t('No archived books found.'),
    t('No recent updates found.'),
    t('Page: Archived books'),
    t('Page: Latest changes'),
    t('Top pages'),
  );

  $views[$view->name] = $view;

  // Exported view: book_users
  $view = new view;
  $view->name = 'book_users';
  $view->description = 'Display user ranks';
  $view->tag = 'atrium';
  $view->base_table = 'users';
  $view->core = 0;
  $view->api_version = '2';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */
  $handler = $view->new_display('default', 'Defaults', 'default');
  $handler->override_option('relationships', array(
    'flag_user_content_rel' => array(
      'label' => 'user flagged content',
      'required' => 1,
      'flag' => 'studying',
      'id' => 'flag_user_content_rel',
      'table' => 'users',
      'field' => 'flag_user_content_rel',
      'override' => array(
        'button' => 'Sobrescrever',
      ),
      'relationship' => 'none',
    ),
  ));
  $handler->override_option('fields', array(
    'name' => array(
      'label' => '',
      'alter' => array(
        'alter_text' => 0,
        'text' => '',
        'make_link' => 0,
        'path' => '',
        'absolute' => 0,
        'link_class' => '',
        'alt' => '',
        'rel' => '',
        'prefix' => '',
        'suffix' => '',
        'target' => '',
        'help' => '',
        'trim' => 0,
        'max_length' => '30',
        'word_boundary' => 0,
        'ellipsis' => 0,
        'html' => 0,
        'strip_tags' => 0,
      ),
      'empty' => '',
      'hide_empty' => 0,
      'empty_zero' => 0,
      'hide_alter_empty' => 1,
      'link_to_user' => 1,
      'overwrite_anonymous' => 0,
      'anonymous_text' => '',
      'exclude' => 0,
      'id' => 'name',
      'table' => 'users',
      'field' => 'name',
      'relationship' => 'none',
    ),
    'picture' => array(
      'label' => '',
      'alter' => array(
        'alter_text' => 0,
        'text' => '',
        'make_link' => 0,
        'path' => 'node',
        'absolute' => 0,
        'link_class' => '',
        'alt' => '',
        'rel' => '',
        'prefix' => '',
        'suffix' => '',
        'target' => '',
        'help' => '',
        'trim' => 0,
        'max_length' => '15',
        'word_boundary' => 1,
        'ellipsis' => 1,
        'html' => 0,
        'strip_tags' => 0,
      ),
      'empty' => '',
      'hide_empty' => 0,
      'empty_zero' => 0,
      'hide_alter_empty' => 1,
      'imagecache_preset' => 'user-s',
      'exclude' => 0,
      'id' => 'picture',
      'table' => 'users',
      'field' => 'picture',
      'relationship' => 'none',
    ),
  ));
  $handler->override_option('sorts', array(
    'timestamp' => array(
      'order' => 'DESC',
      'granularity' => 'minute',
      'id' => 'timestamp',
      'table' => 'flag_content',
      'field' => 'timestamp',
      'relationship' => 'flag_user_content_rel',
      'override' => array(
        'button' => 'Sobrescrever',
      ),
    ),
  ));
  $handler->override_option('arguments', array(
    'content_id' => array(
      'default_action' => 'default',
      'style_plugin' => 'default_summary',
      'style_options' => array(),
      'wildcard' => 'all',
      'wildcard_substitution' => 'Tudo',
      'title' => '',
      'breadcrumb' => '',
      'default_argument_type' => 'node',
      'default_argument' => '',
      'validate_type' => 'none',
      'validate_fail' => 'not found',
      'break_phrase' => 0,
      'not' => 0,
      'id' => 'content_id',
      'table' => 'flag_content',
      'field' => 'content_id',
      'relationship' => 'flag_user_content_rel',
      'validate_user_argument_type' => 'uid',
      'validate_user_roles' => array(
        2 => 0,
        3 => 0,
        4 => 0,
        5 => 0,
        6 => 0,
      ),
      'default_options_div_prefix' => '',
      'default_taxonomy_tid_term_page' => 0,
      'default_taxonomy_tid_node' => 0,
      'default_taxonomy_tid_limit' => 0,
      'default_taxonomy_tid_vids' => array(
        5 => 0,
        3 => 0,
        1 => 0,
        2 => 0,
        4 => 0,
      ),
      'default_argument_user' => 0,
      'default_argument_fixed' => '',
      'default_argument_php' => '',
      'validate_argument_node_type' => array(
        'poll' => 0,
        'blog' => 0,
        'group' => 0,
        'profile' => 0,
        'shoutbox' => 0,
        'question' => 0,
        'book' => 0,
        'event' => 0,
        'feed_ical' => 0,
        'feed_ical_item' => 0,
        'discussion' => 0,
        'milestone' => 0,
        'task' => 0,
        'webform' => 0,
        'images' => 0,
        'text' => 0,
        'graphmind' => 0,
        'sheetnode' => 0,
        'page' => 0,
      ),
      'validate_argument_node_access' => 0,
      'validate_argument_nid_type' => 'nid',
      'validate_argument_vocabulary' => array(
        5 => 0,
        3 => 0,
        1 => 0,
        2 => 0,
        4 => 0,
      ),
      'validate_argument_type' => 'tid',
      'validate_argument_transform' => 0,
      'validate_user_restrict_roles' => 0,
      'validate_argument_node_flag_name' => '*relationship*',
      'validate_argument_node_flag_test' => 'flaggable',
      'validate_argument_node_flag_id_type' => 'id',
      'validate_argument_user_flag_name' => '*relationship*',
      'validate_argument_user_flag_test' => 'flaggable',
      'validate_argument_user_flag_id_type' => 'id',
      'validate_argument_is_member' => '0',
      'validate_argument_group_node_type' => array(
        'group' => 0,
      ),
      'validate_argument_php' => '',
      'override' => array(
        'button' => 'Sobrescrever',
      ),
    ),
  ));
  $handler->override_option('filters', array(
    'status' => array(
      'operator' => '=',
      'value' => '1',
      'group' => '0',
      'exposed' => FALSE,
      'expose' => array(
        'operator' => FALSE,
        'label' => '',
      ),
      'id' => 'status',
      'table' => 'users',
      'field' => 'status',
      'relationship' => 'none',
    ),
    'flagged' => array(
      'operator' => '=',
      'value' => '1',
      'group' => '0',
      'exposed' => FALSE,
      'expose' => array(
        'operator' => FALSE,
        'label' => '',
      ),
      'id' => 'flagged',
      'table' => 'flag_content',
      'field' => 'flagged',
      'relationship' => 'flag_user_content_rel',
      'override' => array(
        'button' => 'Sobrescrever',
      ),
    ),
  ));
  $handler->override_option('access', array(
    'type' => 'none',
  ));
  $handler->override_option('cache', array(
    'type' => 'none',
  ));
  $handler->override_option('header_format', '4');
  $handler->override_option('header_empty', 0);
  $handler->override_option('footer_format', '7');
  $handler->override_option('footer_empty', 0);
  $handler->override_option('empty', ' ');
  $handler->override_option('empty_format', '4');
  $handler->override_option('use_ajax', TRUE);
  $handler->override_option('items_per_page', 8);
  $handler->override_option('use_pager', '0');
  $handler->override_option('use_more', 1);
  $handler->override_option('use_more_always', 0);
  $handler->override_option('use_more_text', 'todos');
  $handler->override_option('style_plugin', 'fluid_grid');
  $handler->override_option('style_options', array(
    'grouping' => '',
    'advanced_layout' => array(
      'margins' => TRUE,
    ),
    'items_width' => '',
    'items_height' => '',
    'list_alignment' => '',
    'items_alignment' => '',
    'items_h_margin' => '6px',
    'items_v_margin' => '6px',
    'box_shadow' => '0',
    'border_radius' => '0',
  ));
  $handler->override_option('row_options', array(
    'inline' => array(
      'picture' => 'picture',
      'name' => 'name',
    ),
    'separator' => '',
    'hide_empty' => 0,
  ));
  $handler = $view->new_display('block', 'Studying Knowledge', 'block_4');
  $handler->override_option('relationships', array(
    'flag_user_content_rel' => array(
      'label' => 'user flagged content',
      'required' => 1,
      'flag' => 'studying',
      'id' => 'flag_user_content_rel',
      'table' => 'users',
      'field' => 'flag_user_content_rel',
      'override' => array(
        'button' => 'Usar o padrão',
      ),
      'relationship' => 'none',
    ),
    'uid' => array(
      'id' => 'uid',
      'table' => 'flag_content',
      'field' => 'uid',
      'relationship' => 'flag_user_content_rel',
    ),
  ));
  $handler->override_option('title', 'Comunidade');
  $handler->override_option('header', '<?php 
if ( arg(0) == \'node\' && is_numeric(arg(1)) ) {
  $node = node_load(arg(1));
  //print \'<a href="\'. $node->nid .\'/talk#comment-form">\'.t(\'Ask a question\') . \'</a> \' . t(\'or announce\');
  print t(\'Studying\') . \': \';
  print flag_create_link(\'studying\', $node->nid);
}
?>');
  $handler->override_option('header_format', '7');
  $handler->override_option('header_empty', 1);
  $handler->override_option('block_description', 'Estudando esse conhecimento');
  $handler->override_option('block_caching', -1);
  $handler = $view->new_display('attachment', 'Used', 'attachment_3');
  $handler->override_option('relationships', array(
    'flag_user_content_rel' => array(
      'label' => 'user flagged content',
      'required' => 1,
      'flag' => 'used',
      'id' => 'flag_user_content_rel',
      'table' => 'users',
      'field' => 'flag_user_content_rel',
      'override' => array(
        'button' => 'Usar o padrão',
      ),
      'relationship' => 'none',
    ),
  ));
  $handler->override_option('title', 'Usaram');
  $handler->override_option('header', '<?php 
if ( arg(0) == \'node\' && is_numeric(arg(1)) ) {
  $node = node_load(arg(1));
  //print \'<a href="\'. $node->nid .\'/talk#comment-form">\'.t(\'Ask a question\') . \'</a> \' . t(\'or announce\');
  print t(\'Used\') . \': \';
  print flag_create_link(\'used\', $node->nid);
}
?>');
  $handler->override_option('header_format', '7');
  $handler->override_option('header_empty', 1);
  $handler->override_option('attachment_position', 'after');
  $handler->override_option('inherit_arguments', TRUE);
  $handler->override_option('inherit_exposed_filters', FALSE);
  $handler->override_option('inherit_pager', FALSE);
  $handler->override_option('render_pager', TRUE);
  $handler->override_option('displays', array(
    'block_4' => 'block_4',
    'default' => 0,
    'block_2' => 0,
    'block_3' => 0,
  ));
  $handler = $view->new_display('attachment', 'Experts', 'attachment_4');
  $handler->override_option('relationships', array(
    'flag_user_content_rel' => array(
      'label' => 'user flagged content',
      'required' => 1,
      'flag' => 'expert',
      'id' => 'flag_user_content_rel',
      'table' => 'users',
      'field' => 'flag_user_content_rel',
      'override' => array(
        'button' => 'Usar o padrão',
      ),
      'relationship' => 'none',
    ),
  ));
  $handler->override_option('header', '<?php 
if ( arg(0) == \'node\' && is_numeric(arg(1)) ) {
  $node = node_load(arg(1));
  //print \'<a href="\'. $node->nid .\'/talk#comment-form">\'.t(\'Ask a question\') . \'</a> \' . t(\'or announce\');
  print t(\'Experts\') . \': \';
  print flag_create_link(\'expert\', $node->nid);
}
?>');
  $handler->override_option('header_format', '7');
  $handler->override_option('header_empty', 1);
  $handler->override_option('attachment_position', 'after');
  $handler->override_option('inherit_arguments', TRUE);
  $handler->override_option('inherit_exposed_filters', FALSE);
  $handler->override_option('inherit_pager', FALSE);
  $handler->override_option('render_pager', TRUE);
  $handler->override_option('displays', array(
    'block_4' => 'block_4',
    'default' => 0,
    'block_2' => 0,
    'block_3' => 0,
  ));
  $translatables['book_users'] = array(
    t(' '),
    t('<?php 
if ( arg(0) == \'node\' && is_numeric(arg(1)) ) {
  $node = node_load(arg(1));
  //print \'<a href="\'. $node->nid .\'/talk#comment-form">\'.t(\'Ask a question\') . \'</a> \' . t(\'or announce\');
  print t(\'Experts\') . \': \';
  print flag_create_link(\'expert\', $node->nid);
}
?>'),
    t('<?php 
if ( arg(0) == \'node\' && is_numeric(arg(1)) ) {
  $node = node_load(arg(1));
  //print \'<a href="\'. $node->nid .\'/talk#comment-form">\'.t(\'Ask a question\') . \'</a> \' . t(\'or announce\');
  print t(\'Studying\') . \': \';
  print flag_create_link(\'studying\', $node->nid);
}
?>'),
    t('<?php 
if ( arg(0) == \'node\' && is_numeric(arg(1)) ) {
  $node = node_load(arg(1));
  //print \'<a href="\'. $node->nid .\'/talk#comment-form">\'.t(\'Ask a question\') . \'</a> \' . t(\'or announce\');
  print t(\'Used\') . \': \';
  print flag_create_link(\'used\', $node->nid);
}
?>'),
    t('Community'),
    t('Defaults'),
    t('Estudando esse conhecimento'),
    t('Experts'),
    t('Studying Knowledge'),
    t('Used'),
    t('todos'),
  );

  $views[$view->name] = $view;

  // Exported view: cards
  $view = new view;
  $view->name = 'cards';
  $view->description = 'Display the knowledge card';
  $view->tag = '';
  $view->base_table = 'node';
  $view->core = 6;
  $view->api_version = '2';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */
  $handler = $view->new_display('default', 'Padrões', 'default');
  $handler->override_option('fields', array(
    'field_input_value' => array(
      'label' => 'Input',
      'alter' => array(
        'alter_text' => 0,
        'text' => '',
        'make_link' => 0,
        'path' => '',
        'absolute' => 0,
        'link_class' => '',
        'alt' => '',
        'rel' => '',
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
      'hide_alter_empty' => 1,
      'link_to_node' => 0,
      'label_type' => 'widget',
      'format' => 'link',
      'multiple' => array(
        'group' => 1,
        'multiple_number' => '',
        'multiple_from' => '',
        'multiple_reversed' => 0,
      ),
      'exclude' => 0,
      'id' => 'field_input_value',
      'table' => 'node_data_field_input',
      'field' => 'field_input_value',
      'relationship' => 'none',
    ),
    'title_1' => array(
      'label' => '',
      'alter' => array(
        'alter_text' => 1,
        'text' => '<div class="titulo_verso">[title_1]</div>',
        'make_link' => 0,
        'path' => '',
        'absolute' => 0,
        'link_class' => '',
        'alt' => '',
        'rel' => '',
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
      'hide_alter_empty' => 1,
      'link_to_node' => 0,
      'spaces' => array(
        'frontpage' => 0,
        'type' => 'spaces_og',
      ),
      'exclude' => 0,
      'id' => 'title_1',
      'table' => 'node',
      'field' => 'title',
      'relationship' => 'none',
      'override' => array(
        'button' => 'Sobrescrever',
      ),
    ),
    'title' => array(
      'label' => '',
      'alter' => array(
        'alter_text' => 1,
        'text' => '<div class="titulo_frente">[title]</div>',
        'make_link' => 0,
        'path' => '',
        'absolute' => 0,
        'link_class' => '',
        'alt' => '',
        'rel' => '',
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
      'hide_alter_empty' => 1,
      'link_to_node' => 0,
      'spaces' => array(
        'frontpage' => 0,
        'type' => 'spaces_og',
      ),
      'exclude' => 0,
      'id' => 'title',
      'table' => 'node',
      'field' => 'title',
      'relationship' => 'none',
    ),
    'field_card_description_value' => array(
      'label' => '',
      'alter' => array(
        'alter_text' => 0,
        'text' => '',
        'make_link' => 0,
        'path' => '',
        'absolute' => 0,
        'link_class' => '',
        'alt' => '',
        'rel' => '',
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
      'hide_alter_empty' => 1,
      'link_to_node' => 0,
      'label_type' => 'none',
      'format' => 'default',
      'multiple' => array(
        'group' => TRUE,
        'multiple_number' => '',
        'multiple_from' => '',
        'multiple_reversed' => FALSE,
      ),
      'exclude' => 0,
      'id' => 'field_card_description_value',
      'table' => 'node_data_field_card_description',
      'field' => 'field_card_description_value',
      'relationship' => 'none',
    ),
    'field_output_value' => array(
      'label' => 'Ouput',
      'alter' => array(
        'alter_text' => 0,
        'text' => '',
        'make_link' => 0,
        'path' => '',
        'absolute' => 0,
        'link_class' => '',
        'alt' => '',
        'rel' => '',
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
      'hide_alter_empty' => 1,
      'link_to_node' => 0,
      'label_type' => 'widget',
      'format' => 'link',
      'multiple' => array(
        'group' => 1,
        'multiple_number' => '',
        'multiple_from' => '',
        'multiple_reversed' => 0,
      ),
      'exclude' => 0,
      'id' => 'field_output_value',
      'table' => 'node_data_field_output',
      'field' => 'field_output_value',
      'relationship' => 'none',
    ),
    'nid' => array(
      'label' => 'Nid',
      'alter' => array(
        'alter_text' => 0,
        'text' => '',
        'make_link' => 0,
        'path' => '',
        'absolute' => 0,
        'link_class' => '',
        'alt' => '',
        'rel' => '',
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
      'hide_alter_empty' => 1,
      'link_to_node' => 0,
      'spaces' => array(
        'frontpage' => 0,
        'type' => 'spaces_og',
      ),
      'exclude' => 1,
      'id' => 'nid',
      'table' => 'node',
      'field' => 'nid',
      'override' => array(
        'button' => 'Sobrescrever',
      ),
      'relationship' => 'none',
    ),
    'phpcode' => array(
      'label' => '',
      'alter' => array(
        'alter_text' => 0,
        'text' => '',
        'make_link' => 0,
        'path' => '',
        'absolute' => 0,
        'link_class' => '',
        'alt' => '',
        'rel' => '',
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
      'hide_alter_empty' => 1,
      'value' => '<?php 
$space = spaces_get_space();
if ($space) {
if ($space->access_feature(\'create\', \'features_task\')) {
    echo \'<p><a href="../node/add/task?edit%5Bfield_referenced_book_page%5D%5B0%5D%5Bnid%5D%5Bnid%5D=%5Bnid:\'.$data->nid.\'%5D"><img src="/sites/all/modules/features/features_task/icon-small.png" /> Criar tarefa</a></p>\';
  }
}
?>
<p><a href="/node/<?php echo $data->nid; ?>">Ver detalhes</a></p>',
      'exclude' => 0,
      'id' => 'phpcode',
      'table' => 'customfield',
      'field' => 'phpcode',
      'relationship' => 'none',
    ),
  ));
  $handler->override_option('sorts', array(
    'title' => array(
      'order' => 'ASC',
      'id' => 'title',
      'table' => 'node',
      'field' => 'title',
      'relationship' => 'none',
    ),
  ));
  $handler->override_option('filters', array(
    'type' => array(
      'operator' => 'in',
      'value' => array(
        'book' => 'book',
      ),
      'group' => '0',
      'exposed' => FALSE,
      'expose' => array(
        'operator' => FALSE,
        'label' => '',
      ),
      'id' => 'type',
      'table' => 'node',
      'field' => 'type',
      'relationship' => 'none',
    ),
    'field_card_description_value' => array(
      'operator' => 'not empty',
      'value' => '',
      'group' => '0',
      'exposed' => FALSE,
      'expose' => array(
        'operator' => FALSE,
        'label' => '',
        'optional' => FALSE,
      ),
      'case' => 1,
      'id' => 'field_card_description_value',
      'table' => 'node_data_field_card_description',
      'field' => 'field_card_description_value',
      'relationship' => 'none',
    ),
    'tid' => array(
      'operator' => 'not empty',
      'value' => '',
      'group' => '0',
      'exposed' => FALSE,
      'expose' => array(
        'operator' => FALSE,
        'label' => '',
      ),
      'type' => 'textfield',
      'limit' => TRUE,
      'limit_by' => 'vid',
      'vid' => '6',
      'module' => 'features_atrium_scrum_stories_role',
      'id' => 'tid',
      'table' => 'term_node',
      'field' => 'tid',
      'hierarchy' => 0,
      'relationship' => 'none',
      'reduce_duplicates' => 0,
    ),
  ));
  $handler->override_option('access', array(
    'type' => 'none',
  ));
  $handler->override_option('cache', array(
    'type' => 'none',
  ));
  $handler->override_option('title', 'Cartão do Conhecimento');
  $handler->override_option('header_format', '4');
  $handler->override_option('header_empty', 0);
  $handler->override_option('footer_format', '4');
  $handler->override_option('footer_empty', 0);
  $handler->override_option('row_options', array(
    'inline' => array(),
    'separator' => '',
    'hide_empty' => 0,
  ));
  $handler = $view->new_display('block', 'Card', 'block_1');
  $handler->override_option('arguments', array(
    'nid' => array(
      'default_action' => 'default',
      'style_plugin' => 'default_summary',
      'style_options' => array(),
      'wildcard' => 'all',
      'wildcard_substitution' => 'Tudo',
      'title' => '',
      'breadcrumb' => '',
      'default_argument_type' => 'node',
      'default_argument' => '',
      'validate_type' => 'none',
      'validate_fail' => 'not found',
      'break_phrase' => 0,
      'not' => 0,
      'id' => 'nid',
      'table' => 'node',
      'field' => 'nid',
      'validate_user_argument_type' => 'uid',
      'validate_user_roles' => array(
        2 => 0,
        3 => 0,
        4 => 0,
        5 => 0,
        6 => 0,
      ),
      'relationship' => 'none',
      'default_options_div_prefix' => '',
      'default_taxonomy_tid_term_page' => 0,
      'default_taxonomy_tid_node' => 0,
      'default_taxonomy_tid_limit' => 0,
      'default_taxonomy_tid_vids' => array(
        5 => 0,
        3 => 0,
        6 => 0,
        1 => 0,
        2 => 0,
        8 => 0,
        4 => 0,
      ),
      'default_argument_user' => 0,
      'default_argument_fixed' => '',
      'default_argument_php' => '',
      'validate_argument_node_type' => array(
        'poll' => 0,
        'blog' => 0,
        'group' => 0,
        'profile' => 0,
        'shoutbox' => 0,
        'question' => 0,
        'book' => 0,
        'event' => 0,
        'feed_ical' => 0,
        'feed_ical_item' => 0,
        'discussion' => 0,
        'milestone' => 0,
        'task' => 0,
        'webform' => 0,
        'images' => 0,
        'text' => 0,
        'graphmind' => 0,
        'sheetnode' => 0,
        'page' => 0,
      ),
      'validate_argument_node_access' => 0,
      'validate_argument_nid_type' => 'nid',
      'validate_argument_vocabulary' => array(
        5 => 0,
        3 => 0,
        6 => 0,
        1 => 0,
        2 => 0,
        8 => 0,
        4 => 0,
      ),
      'validate_argument_type' => 'tid',
      'validate_argument_transform' => 0,
      'validate_user_restrict_roles' => 0,
      'validate_argument_node_flag_name' => '*relationship*',
      'validate_argument_node_flag_test' => 'flaggable',
      'validate_argument_node_flag_id_type' => 'id',
      'validate_argument_user_flag_name' => '*relationship*',
      'validate_argument_user_flag_test' => 'flaggable',
      'validate_argument_user_flag_id_type' => 'id',
      'validate_argument_is_member' => '0',
      'validate_argument_group_node_type' => array(
        'group' => 0,
      ),
      'validate_argument_php' => '',
    ),
  ));
  $handler->override_option('block_description', 'Cartão do Conhecimento');
  $handler->override_option('block_caching', -1);
  $handler = $view->new_display('page', 'Página', 'page_1');
  $handler->override_option('filters', array(
    'type' => array(
      'operator' => 'in',
      'value' => array(
        'book' => 'book',
      ),
      'group' => '0',
      'exposed' => FALSE,
      'expose' => array(
        'operator' => FALSE,
        'label' => '',
      ),
      'id' => 'type',
      'table' => 'node',
      'field' => 'type',
      'relationship' => 'none',
    ),
    'field_card_description_value' => array(
      'operator' => 'not empty',
      'value' => '',
      'group' => '0',
      'exposed' => FALSE,
      'expose' => array(
        'operator' => FALSE,
        'label' => '',
        'optional' => FALSE,
      ),
      'case' => 1,
      'id' => 'field_card_description_value',
      'table' => 'node_data_field_card_description',
      'field' => 'field_card_description_value',
      'relationship' => 'none',
    ),
  ));
  $handler->override_option('css_class', 'knowledgecards');
  $handler->override_option('header', '<p>Estes cartões foram criados no projeto <a href="/cards">UXCards</a>.</p>');
  $handler->override_option('items_per_page', 0);
  $handler->override_option('style_plugin', 'grid');
  $handler->override_option('style_options', array(
    'grouping' => '',
    'columns' => '3',
    'alignment' => 'horizontal',
    'fill_single_line' => 1,
    'summary' => '',
  ));
  $handler->override_option('path', 'knowledge/cards');
  $handler->override_option('menu', array(
    'type' => 'tab',
    'title' => 'Knowledge Cards',
    'description' => '',
    'weight' => '0',
    'name' => 'navigation',
  ));
  $handler->override_option('tab_options', array(
    'type' => 'none',
    'title' => '',
    'description' => '',
    'weight' => 0,
    'name' => 'navigation',
  ));
  $handler = $view->new_display('attachment', 'Anexo', 'attachment_1');
  $handler->override_option('relationships', array(
    'term_node_tid' => array(
      'label' => 'term',
      'required' => 0,
      'vids' => array(
        6 => 6,
        5 => 0,
        3 => 0,
        1 => 0,
        2 => 0,
        8 => 0,
        4 => 0,
      ),
      'id' => 'term_node_tid',
      'table' => 'node',
      'field' => 'term_node_tid',
      'override' => array(
        'button' => 'Usar o padrão',
      ),
      'relationship' => 'none',
    ),
    'vid' => array(
      'label' => 'node',
      'required' => 0,
      'id' => 'vid',
      'table' => 'term_node',
      'field' => 'vid',
      'override' => array(
        'button' => 'Usar o padrão',
      ),
      'relationship' => 'term_node_tid',
    ),
  ));
  $handler->override_option('arguments', array(
    'nid' => array(
      'default_action' => 'default',
      'style_plugin' => 'default_summary',
      'style_options' => array(),
      'wildcard' => 'all',
      'wildcard_substitution' => 'Tudo',
      'title' => '',
      'breadcrumb' => '',
      'default_argument_type' => 'node',
      'default_argument' => '',
      'validate_type' => 'none',
      'validate_fail' => 'not found',
      'break_phrase' => 0,
      'not' => 0,
      'id' => 'nid',
      'table' => 'node',
      'field' => 'nid',
      'validate_user_argument_type' => 'uid',
      'validate_user_roles' => array(
        2 => 0,
        3 => 0,
        4 => 0,
        5 => 0,
        6 => 0,
      ),
      'override' => array(
        'button' => 'Usar o padrão',
      ),
      'relationship' => 'vid',
      'default_options_div_prefix' => '',
      'default_taxonomy_tid_term_page' => 0,
      'default_taxonomy_tid_node' => 0,
      'default_taxonomy_tid_limit' => 0,
      'default_taxonomy_tid_vids' => array(
        5 => 0,
        3 => 0,
        6 => 0,
        1 => 0,
        2 => 0,
        8 => 0,
        4 => 0,
      ),
      'default_argument_user' => 0,
      'default_argument_fixed' => '',
      'default_argument_php' => '',
      'validate_argument_node_type' => array(
        'poll' => 0,
        'blog' => 0,
        'group' => 0,
        'profile' => 0,
        'shoutbox' => 0,
        'question' => 0,
        'book' => 0,
        'event' => 0,
        'feed_ical' => 0,
        'feed_ical_item' => 0,
        'discussion' => 0,
        'milestone' => 0,
        'task' => 0,
        'webform' => 0,
        'images' => 0,
        'text' => 0,
        'graphmind' => 0,
        'patch' => 0,
        'sheetnode' => 0,
        'page' => 0,
        'service_provider' => 0,
        'toolkit' => 0,
      ),
      'validate_argument_node_access' => 0,
      'validate_argument_nid_type' => 'nid',
      'validate_argument_vocabulary' => array(
        5 => 0,
        3 => 0,
        6 => 0,
        1 => 0,
        2 => 0,
        8 => 0,
        4 => 0,
      ),
      'validate_argument_type' => 'tid',
      'validate_argument_transform' => 0,
      'validate_user_restrict_roles' => 0,
      'validate_argument_node_flag_name' => '*relationship*',
      'validate_argument_node_flag_test' => 'flaggable',
      'validate_argument_node_flag_id_type' => 'id',
      'validate_argument_user_flag_name' => '*relationship*',
      'validate_argument_user_flag_test' => 'flaggable',
      'validate_argument_user_flag_id_type' => 'id',
      'validate_argument_is_member' => '0',
      'validate_argument_group_node_type' => array(
        'group' => 0,
      ),
      'validate_argument_php' => '',
    ),
  ));
  $handler->override_option('filters', array(
    'type' => array(
      'operator' => 'in',
      'value' => array(
        'book' => 'book',
      ),
      'group' => '0',
      'exposed' => FALSE,
      'expose' => array(
        'operator' => FALSE,
        'label' => '',
      ),
      'id' => 'type',
      'table' => 'node',
      'field' => 'type',
      'relationship' => 'none',
    ),
    'field_card_description_value' => array(
      'operator' => 'not empty',
      'value' => '',
      'group' => '0',
      'exposed' => FALSE,
      'expose' => array(
        'operator' => FALSE,
        'label' => '',
        'optional' => FALSE,
      ),
      'case' => 1,
      'id' => 'field_card_description_value',
      'table' => 'node_data_field_card_description',
      'field' => 'field_card_description_value',
      'relationship' => 'none',
    ),
    'field_output_value' => array(
      'operator' => 'or',
      'value' => array(),
      'group' => '0',
      'exposed' => FALSE,
      'expose' => array(
        'operator' => FALSE,
        'label' => '',
      ),
      'id' => 'field_output_value',
      'table' => 'node_data_field_output',
      'field' => 'field_output_value',
      'override' => array(
        'button' => 'Usar o padrão',
      ),
      'relationship' => 'vid',
      'reduce_duplicates' => 0,
    ),
  ));
  $handler->override_option('attachment_position', 'before');
  $handler->override_option('inherit_arguments', TRUE);
  $handler->override_option('inherit_exposed_filters', FALSE);
  $handler->override_option('inherit_pager', FALSE);
  $handler->override_option('render_pager', TRUE);
  $handler->override_option('displays', array());
  $translatables['cards'] = array(
    t('<p>Estes cartões foram criados no projeto <a href="/cards">UXCards</a>.</p>'),
    t('Anexo'),
    t('Card'),
    t('Knowledge Card'),
    t('Padrões'),
    t('Página'),
  );

  $views[$view->name] = $view;

  // Exported view: knowledge_locations
  $view = new view;
  $view->name = 'knowledge_locations';
  $view->description = 'Show a map with location associated to a knowledge';
  $view->tag = 'Book';
  $view->base_table = 'node';
  $view->core = 6;
  $view->api_version = '2';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */
  $handler = $view->new_display('default', 'Padrões', 'default');
  $handler->override_option('fields', array(
    'title' => array(
      'label' => 'Título',
      'alter' => array(
        'alter_text' => 0,
        'text' => '',
        'make_link' => 0,
        'path' => '',
        'absolute' => 0,
        'link_class' => '',
        'alt' => '',
        'rel' => '',
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
      'hide_alter_empty' => 1,
      'link_to_node' => 1,
      'spaces' => array(
        'frontpage' => 0,
        'type' => 'spaces_og',
      ),
      'exclude' => 0,
      'id' => 'title',
      'table' => 'node',
      'field' => 'title',
      'override' => array(
        'button' => 'Sobrescrever',
      ),
      'relationship' => 'none',
    ),
  ));
  $handler->override_option('arguments', array(
    'nid' => array(
      'id' => 'nid',
      'table' => 'node',
      'field' => 'nid',
    ),
  ));
  $handler->override_option('access', array(
    'type' => 'none',
  ));
  $handler->override_option('cache', array(
    'type' => 'none',
  ));
  $handler->override_option('style_plugin', 'gmap');
  $handler->override_option('style_options', array(
    'macro' => '[gmap ]',
    'datasource' => 'location',
    'latfield' => '',
    'lonfield' => '',
    'markers' => 'static',
    'markerfield' => '',
    'markertype' => '',
    'center_on_nodearg' => 1,
    'center_on_nodearg_arg' => 'nid',
    'highlight_nodearg' => 0,
    'highlight_nodearg_arg' => 'nid',
    'highlight_nodearg_color' => '#FF0000',
    'tooltipenabled' => 0,
    'tooltipfield' => '',
  ));
  $handler = $view->new_display('block', 'Bloco', 'block_1');
  $handler->override_option('block_description', '');
  $handler->override_option('block_caching', -1);
  $translatables['knowledge_locations'] = array(
    t('Bloco'),
    t('Padrões'),
  );

  $views[$view->name] = $view;

  // Exported view: knowledge_select
  $view = new view;
  $view->name = 'knowledge_select';
  $view->description = 'Display the knowledge list for related content';
  $view->tag = '';
  $view->base_table = 'node';
  $view->core = 6;
  $view->api_version = '2';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */
  $handler = $view->new_display('default', 'Padrões', 'default');
  $handler->override_option('fields', array(
    'nid' => array(
      'label' => 'Nid',
      'alter' => array(
        'alter_text' => 0,
        'text' => '',
        'make_link' => 0,
        'path' => '',
        'absolute' => 0,
        'link_class' => '',
        'alt' => '',
        'rel' => '',
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
      'hide_alter_empty' => 1,
      'link_to_node' => 0,
      'spaces' => array(
        'frontpage' => 0,
        'type' => 'spaces_og',
      ),
      'exclude' => 1,
      'id' => 'nid',
      'table' => 'node',
      'field' => 'nid',
      'relationship' => 'none',
    ),
    'title' => array(
      'label' => '',
      'alter' => array(
        'alter_text' => 0,
        'text' => '',
        'make_link' => 1,
        'path' => 'http://corais.org/node/[nid]',
        'absolute' => 1,
        'link_class' => 'title3',
        'alt' => '',
        'rel' => '',
        'prefix' => '',
        'suffix' => '',
        'target' => '_blank',
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
      'hide_alter_empty' => 1,
      'link_to_node' => 0,
      'spaces' => array(
        'frontpage' => 0,
        'type' => 'spaces_og',
      ),
      'exclude' => 0,
      'id' => 'title',
      'table' => 'node',
      'field' => 'title',
      'relationship' => 'none',
    ),
    'field_card_description_value' => array(
      'label' => '',
      'alter' => array(
        'alter_text' => 0,
        'text' => '',
        'make_link' => 0,
        'path' => '',
        'absolute' => 0,
        'link_class' => '',
        'alt' => '',
        'rel' => '',
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
      'hide_alter_empty' => 1,
      'link_to_node' => 0,
      'label_type' => 'none',
      'format' => 'default',
      'multiple' => array(
        'group' => TRUE,
        'multiple_number' => '',
        'multiple_from' => '',
        'multiple_reversed' => FALSE,
      ),
      'exclude' => 0,
      'id' => 'field_card_description_value',
      'table' => 'node_data_field_card_description',
      'field' => 'field_card_description_value',
      'relationship' => 'none',
    ),
  ));
  $handler->override_option('sorts', array(
    'title' => array(
      'order' => 'ASC',
      'id' => 'title',
      'table' => 'node',
      'field' => 'title',
      'relationship' => 'none',
    ),
  ));
  $handler->override_option('filters', array(
    'type' => array(
      'operator' => 'in',
      'value' => array(
        'book' => 'book',
      ),
      'group' => '0',
      'exposed' => FALSE,
      'expose' => array(
        'operator' => FALSE,
        'label' => '',
      ),
      'id' => 'type',
      'table' => 'node',
      'field' => 'type',
      'relationship' => 'none',
    ),
  ));
  $handler->override_option('access', array(
    'type' => 'none',
  ));
  $handler->override_option('cache', array(
    'type' => 'none',
  ));
  $handler->override_option('title', 'Cartão do Conhecimento');
  $handler->override_option('footer_format', '4');
  $handler->override_option('footer_empty', 0);
  $handler->override_option('items_per_page', 0);
  $handler->override_option('style_plugin', 'list');
  $handler->override_option('style_options', array(
    'grouping' => '',
    'type' => 'ul',
  ));
  $handler->override_option('row_options', array(
    'inline' => array(
      'title_1' => 'title_1',
      'view_comment' => 'view_comment',
    ),
    'separator' => ' ',
    'hide_empty' => 0,
  ));
  $translatables['knowledge_select'] = array(
    t('Cartão do Conhecimento'),
    t('Padrões'),
  );

  $views[$view->name] = $view;

  // Exported view: knowledge_service_provider
  $view = new view;
  $view->name = 'knowledge_service_provider';
  $view->description = 'Display service providers for a specific knowledge';
  $view->tag = '';
  $view->base_table = 'node';
  $view->core = 6;
  $view->api_version = '2';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */
  $handler = $view->new_display('default', 'Padrões', 'default');
  $handler->override_option('fields', array(
    'field_logo_fid' => array(
      'label' => '',
      'alter' => array(
        'alter_text' => 0,
        'text' => '',
        'make_link' => 0,
        'path' => '',
        'absolute' => 0,
        'link_class' => '',
        'alt' => '',
        'rel' => '',
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
      'hide_alter_empty' => 1,
      'link_to_node' => 0,
      'label_type' => 'none',
      'format' => 'imageboard-thumb_linked',
      'multiple' => array(
        'group' => TRUE,
        'multiple_number' => '',
        'multiple_from' => '',
        'multiple_reversed' => FALSE,
      ),
      'exclude' => 0,
      'id' => 'field_logo_fid',
      'table' => 'node_data_field_logo',
      'field' => 'field_logo_fid',
      'relationship' => 'none',
    ),
    'title' => array(
      'label' => '',
      'alter' => array(
        'alter_text' => 0,
        'text' => '',
        'make_link' => 0,
        'path' => '',
        'absolute' => 0,
        'link_class' => '',
        'alt' => '',
        'rel' => '',
        'prefix' => '',
        'suffix' => '',
        'target' => '',
        'help' => '',
        'trim' => 1,
        'max_length' => '40',
        'word_boundary' => 1,
        'ellipsis' => 1,
        'html' => 0,
        'strip_tags' => 0,
      ),
      'empty' => '',
      'hide_empty' => 0,
      'empty_zero' => 0,
      'hide_alter_empty' => 1,
      'link_to_node' => 1,
      'spaces' => array(
        'frontpage' => 0,
        'type' => 'spaces_og',
      ),
      'exclude' => 0,
      'id' => 'title',
      'table' => 'node',
      'field' => 'title',
      'override' => array(
        'button' => 'Sobrescrever',
      ),
      'relationship' => 'none',
    ),
    'city' => array(
      'label' => '',
      'alter' => array(
        'alter_text' => 0,
        'text' => '',
        'make_link' => 0,
        'path' => '',
        'absolute' => 0,
        'link_class' => '',
        'alt' => '',
        'rel' => '',
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
      'hide_alter_empty' => 1,
      'exclude' => 0,
      'id' => 'city',
      'table' => 'location',
      'field' => 'city',
      'relationship' => 'none',
    ),
    'view_node' => array(
      'label' => '',
      'alter' => array(
        'alter_text' => 0,
        'text' => '',
        'make_link' => 0,
        'path' => '',
        'absolute' => 0,
        'link_class' => '',
        'alt' => '',
        'rel' => '',
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
      'hide_alter_empty' => 1,
      'text' => 'ver detalhes',
      'exclude' => 0,
      'id' => 'view_node',
      'table' => 'node',
      'field' => 'view_node',
      'relationship' => 'none',
    ),
    'edit_node' => array(
      'label' => '',
      'alter' => array(
        'alter_text' => 0,
        'text' => '',
        'make_link' => 0,
        'path' => '',
        'absolute' => 0,
        'link_class' => '',
        'alt' => '',
        'rel' => '',
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
      'hide_alter_empty' => 1,
      'text' => 'editar',
      'exclude' => 0,
      'id' => 'edit_node',
      'table' => 'node',
      'field' => 'edit_node',
      'relationship' => 'none',
    ),
  ));
  $handler->override_option('arguments', array(
    'field_referenced_book_page_nid' => array(
      'default_action' => 'default',
      'style_plugin' => 'default_summary',
      'style_options' => array(),
      'wildcard' => 'all',
      'wildcard_substitution' => 'Tudo',
      'title' => '',
      'breadcrumb' => '',
      'default_argument_type' => 'node',
      'default_argument' => '',
      'validate_type' => 'none',
      'validate_fail' => 'not found',
      'break_phrase' => 0,
      'not' => 0,
      'id' => 'field_referenced_book_page_nid',
      'table' => 'node_data_field_referenced_book_page',
      'field' => 'field_referenced_book_page_nid',
      'validate_user_argument_type' => 'uid',
      'validate_user_roles' => array(
        2 => 0,
        3 => 0,
        4 => 0,
        5 => 0,
        6 => 0,
      ),
      'override' => array(
        'button' => 'Sobrescrever',
      ),
      'relationship' => 'none',
      'default_options_div_prefix' => '',
      'default_taxonomy_tid_term_page' => 0,
      'default_taxonomy_tid_node' => 0,
      'default_taxonomy_tid_limit' => 0,
      'default_taxonomy_tid_vids' => array(
        5 => 0,
        3 => 0,
        6 => 0,
        1 => 0,
        2 => 0,
        8 => 0,
        4 => 0,
      ),
      'default_argument_user' => 0,
      'default_argument_fixed' => '',
      'default_argument_php' => '',
      'validate_argument_node_type' => array(
        'poll' => 0,
        'blog' => 0,
        'group' => 0,
        'profile' => 0,
        'shoutbox' => 0,
        'question' => 0,
        'book' => 0,
        'event' => 0,
        'feed_ical' => 0,
        'feed_ical_item' => 0,
        'discussion' => 0,
        'milestone' => 0,
        'task' => 0,
        'webform' => 0,
        'images' => 0,
        'text' => 0,
        'graphmind' => 0,
        'patch' => 0,
        'sheetnode' => 0,
        'page' => 0,
        'service_provider' => 0,
      ),
      'validate_argument_node_access' => 0,
      'validate_argument_nid_type' => 'nid',
      'validate_argument_vocabulary' => array(
        5 => 0,
        3 => 0,
        6 => 0,
        1 => 0,
        2 => 0,
        8 => 0,
        4 => 0,
      ),
      'validate_argument_type' => 'tid',
      'validate_argument_transform' => 0,
      'validate_user_restrict_roles' => 0,
      'validate_argument_node_flag_name' => '*relationship*',
      'validate_argument_node_flag_test' => 'flaggable',
      'validate_argument_node_flag_id_type' => 'id',
      'validate_argument_user_flag_name' => '*relationship*',
      'validate_argument_user_flag_test' => 'flaggable',
      'validate_argument_user_flag_id_type' => 'id',
      'validate_argument_is_member' => '0',
      'validate_argument_group_node_type' => array(
        'group' => 0,
      ),
      'validate_argument_php' => '',
    ),
  ));
  $handler->override_option('filters', array(
    'type' => array(
      'operator' => 'in',
      'value' => array(
        'profile' => 'profile',
        'service_provider' => 'service_provider',
      ),
      'group' => '0',
      'exposed' => FALSE,
      'expose' => array(
        'operator' => FALSE,
        'label' => '',
      ),
      'id' => 'type',
      'table' => 'node',
      'field' => 'type',
      'override' => array(
        'button' => 'Sobrescrever',
      ),
      'relationship' => 'none',
    ),
  ));
  $handler->override_option('access', array(
    'type' => 'none',
  ));
  $handler->override_option('cache', array(
    'type' => 'none',
  ));
  $handler->override_option('title', 'Prestadores de Serviços');
  $handler->override_option('footer', '<?php
print l(t(\'Add new service provider\'), \'node/add/service-provider\', array(\'attributes\' => array(\'class\' => \'flag-action\'))  ); 
?>');
  $handler->override_option('footer_format', '7');
  $handler->override_option('footer_empty', 1);
  $handler->override_option('empty', 'Nenhum ainda...');
  $handler->override_option('empty_format', '4');
  $handler->override_option('items_per_page', 0);
  $handler->override_option('style_plugin', 'gmap');
  $handler->override_option('style_options', array(
    'grouping' => '',
    'macro' => '[gmap | width = 100% ]',
    'datasource' => 'location',
    'latfield' => 'title',
    'lonfield' => 'title',
    'markers' => 'nodetype',
    'markerfield' => 'title',
    'markertype' => 'yellow',
    'center_on_nodearg' => 0,
    'center_on_nodearg_arg' => 'field_referenced_book_page_nid',
    'highlight_nodearg' => 0,
    'highlight_nodearg_arg' => 'field_referenced_book_page_nid',
    'highlight_nodearg_color' => '#FF0000',
    'tooltipenabled' => 1,
    'tooltipfield' => 'title',
  ));
  $handler->override_option('row_options', array(
    'inline' => array(
      'view_node' => 'view_node',
      'edit_node' => 'edit_node',
    ),
    'separator' => ' | ',
    'hide_empty' => 0,
  ));
  $handler = $view->new_display('block', 'Bloco', 'block_1');
  $handler->override_option('block_description', 'Provedores de serviço para este conhecimento');
  $handler->override_option('block_caching', -1);
  $translatables['knowledge_service_provider'] = array(
    t('<?php
print l(t(\'Add new service provider\'), \'node/add/service-provider\', array(\'attributes\' => array(\'class\' => \'flag-action\'))  ); 
?>'),
    t('Bloco'),
    t('None yet...'),
    t('Padrões'),
    t('Service Providers'),
    t('Service providers for this knowledge'),
  );

  $views[$view->name] = $view;

  // Exported view: my_flagged
  $view = new view;
  $view->name = 'my_flagged';
  $view->description = 'Display flagged content in my profile';
  $view->tag = '';
  $view->base_table = 'node';
  $view->core = 6;
  $view->api_version = '2';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */
  $handler = $view->new_display('default', 'Padrões', 'default');
  $handler->override_option('relationships', array(
    'flag_content_rel' => array(
      'label' => 'flag',
      'required' => 1,
      'flag' => 'used',
      'user_scope' => 'any',
      'id' => 'flag_content_rel',
      'table' => 'node',
      'field' => 'flag_content_rel',
      'relationship' => 'none',
    ),
  ));
  $handler->override_option('fields', array(
    'title' => array(
      'label' => '',
      'alter' => array(
        'alter_text' => 0,
        'text' => '',
        'make_link' => 0,
        'path' => '',
        'absolute' => 0,
        'link_class' => '',
        'alt' => '',
        'rel' => '',
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
      'hide_alter_empty' => 1,
      'link_to_node' => 1,
      'spaces' => array(
        'frontpage' => 0,
        'type' => 'spaces_og',
      ),
      'exclude' => 0,
      'id' => 'title',
      'table' => 'node',
      'field' => 'title',
      'override' => array(
        'button' => 'Sobrescrever',
      ),
      'relationship' => 'none',
    ),
  ));
  $handler->override_option('sorts', array(
    'title' => array(
      'order' => 'ASC',
      'id' => 'title',
      'table' => 'node',
      'field' => 'title',
      'relationship' => 'none',
    ),
  ));
  $handler->override_option('arguments', array(
    'uid' => array(
      'default_action' => 'default',
      'style_plugin' => 'default_summary',
      'style_options' => array(),
      'wildcard' => 'all',
      'wildcard_substitution' => 'Tudo',
      'title' => '',
      'breadcrumb' => '',
      'default_argument_type' => 'user',
      'default_argument' => '',
      'validate_type' => 'none',
      'validate_fail' => 'ignore',
      'break_phrase' => 0,
      'not' => 0,
      'id' => 'uid',
      'table' => 'users',
      'field' => 'uid',
      'validate_user_argument_type' => 'uid',
      'validate_user_roles' => array(
        2 => 0,
        3 => 0,
        4 => 0,
        5 => 0,
        6 => 0,
      ),
      'override' => array(
        'button' => 'Sobrescrever',
      ),
      'relationship' => 'uid',
      'default_options_div_prefix' => '',
      'default_taxonomy_tid_term_page' => 0,
      'default_taxonomy_tid_node' => 0,
      'default_taxonomy_tid_limit' => 0,
      'default_taxonomy_tid_vids' => array(
        5 => 0,
        3 => 0,
        1 => 0,
        2 => 0,
        4 => 0,
      ),
      'default_argument_user' => 0,
      'default_argument_fixed' => '',
      'default_argument_php' => '',
      'validate_argument_node_type' => array(
        'poll' => 0,
        'blog' => 0,
        'group' => 0,
        'profile' => 0,
        'shoutbox' => 0,
        'question' => 0,
        'book' => 0,
        'event' => 0,
        'feed_ical' => 0,
        'feed_ical_item' => 0,
        'discussion' => 0,
        'milestone' => 0,
        'task' => 0,
        'webform' => 0,
        'images' => 0,
        'text' => 0,
        'graphmind' => 0,
        'sheetnode' => 0,
        'page' => 0,
      ),
      'validate_argument_node_access' => 0,
      'validate_argument_nid_type' => 'nid',
      'validate_argument_vocabulary' => array(
        5 => 0,
        3 => 0,
        1 => 0,
        2 => 0,
        4 => 0,
      ),
      'validate_argument_type' => 'tid',
      'validate_argument_transform' => 0,
      'validate_user_restrict_roles' => 0,
      'validate_argument_node_flag_name' => '*relationship*',
      'validate_argument_node_flag_test' => 'flagged',
      'validate_argument_node_flag_id_type' => 'id',
      'validate_argument_user_flag_name' => '*relationship*',
      'validate_argument_user_flag_test' => 'flaggable',
      'validate_argument_user_flag_id_type' => 'id',
      'validate_argument_is_member' => '0',
      'validate_argument_group_node_type' => array(
        'group' => 0,
      ),
      'validate_argument_php' => '',
    ),
  ));
  $handler->override_option('access', array(
    'type' => 'none',
  ));
  $handler->override_option('cache', array(
    'type' => 'none',
  ));
  $handler->override_option('items_per_page', 0);
  $handler->override_option('style_plugin', 'list');
  $handler->override_option('style_options', array(
    'grouping' => '',
    'type' => 'ul',
  ));
  $handler->override_option('row_options', array(
    'inline' => array(
      'title' => 'title',
      'timestamp' => 'timestamp',
    ),
    'separator' => ' ',
    'hide_empty' => 0,
  ));
  $handler = $view->new_display('block', 'Knowledge Used', 'block_1');
  $handler->override_option('relationships', array(
    'flag_content_rel' => array(
      'label' => 'flag',
      'required' => 1,
      'flag' => 'used',
      'user_scope' => 'any',
      'id' => 'flag_content_rel',
      'table' => 'node',
      'field' => 'flag_content_rel',
      'override' => array(
        'button' => 'Usar o padrão',
      ),
      'relationship' => 'none',
    ),
    'uid' => array(
      'label' => 'Flag user',
      'required' => 0,
      'id' => 'uid',
      'table' => 'flag_content',
      'field' => 'uid',
      'relationship' => 'flag_content_rel',
      'override' => array(
        'button' => 'Usar o padrão',
      ),
    ),
  ));
  $handler->override_option('arguments', array(
    'uid' => array(
      'default_action' => 'default',
      'style_plugin' => 'default_summary',
      'style_options' => array(),
      'wildcard' => 'all',
      'wildcard_substitution' => 'Tudo',
      'title' => '',
      'breadcrumb' => '',
      'default_argument_type' => 'user',
      'default_argument' => '',
      'validate_type' => 'none',
      'validate_fail' => 'not found',
      'break_phrase' => 0,
      'not' => 0,
      'id' => 'uid',
      'table' => 'users',
      'field' => 'uid',
      'validate_user_argument_type' => 'uid',
      'validate_user_roles' => array(
        2 => 0,
        3 => 0,
        4 => 0,
        5 => 0,
        6 => 0,
      ),
      'override' => array(
        'button' => 'Usar o padrão',
      ),
      'relationship' => 'uid',
      'default_options_div_prefix' => '',
      'default_taxonomy_tid_term_page' => 0,
      'default_taxonomy_tid_node' => 0,
      'default_taxonomy_tid_limit' => 0,
      'default_taxonomy_tid_vids' => array(
        5 => 0,
        3 => 0,
        1 => 0,
        2 => 0,
        4 => 0,
      ),
      'default_argument_user' => 0,
      'default_argument_fixed' => '',
      'default_argument_php' => '',
      'validate_argument_node_type' => array(
        'poll' => 0,
        'blog' => 0,
        'group' => 0,
        'profile' => 0,
        'shoutbox' => 0,
        'question' => 0,
        'book' => 0,
        'event' => 0,
        'feed_ical' => 0,
        'feed_ical_item' => 0,
        'discussion' => 0,
        'milestone' => 0,
        'task' => 0,
        'webform' => 0,
        'images' => 0,
        'text' => 0,
        'graphmind' => 0,
        'sheetnode' => 0,
        'page' => 0,
      ),
      'validate_argument_node_access' => 0,
      'validate_argument_nid_type' => 'nid',
      'validate_argument_vocabulary' => array(
        5 => 0,
        3 => 0,
        1 => 0,
        2 => 0,
        4 => 0,
      ),
      'validate_argument_type' => 'tid',
      'validate_argument_transform' => 0,
      'validate_user_restrict_roles' => 0,
      'validate_argument_node_flag_name' => '*relationship*',
      'validate_argument_node_flag_test' => 'flagged',
      'validate_argument_node_flag_id_type' => 'id',
      'validate_argument_user_flag_name' => '*relationship*',
      'validate_argument_user_flag_test' => 'flaggable',
      'validate_argument_user_flag_id_type' => 'id',
      'validate_argument_is_member' => '0',
      'validate_argument_group_node_type' => array(
        'group' => 0,
      ),
      'validate_argument_php' => '',
    ),
  ));
  $handler->override_option('title', 'Conhecimentos que já usei');
  $handler->override_option('block_description', 'Knowledge Used');
  $handler->override_option('block_caching', -1);
  $handler = $view->new_display('block', 'Knowledge Expert', 'block_2');
  $handler->override_option('relationships', array(
    'flag_content_rel' => array(
      'label' => 'flag2',
      'required' => 1,
      'flag' => 'expert',
      'user_scope' => 'any',
      'id' => 'flag_content_rel',
      'table' => 'node',
      'field' => 'flag_content_rel',
      'override' => array(
        'button' => 'Usar o padrão',
      ),
      'relationship' => 'none',
    ),
    'uid' => array(
      'label' => 'Flag user',
      'required' => 0,
      'id' => 'uid',
      'table' => 'flag_content',
      'field' => 'uid',
      'relationship' => 'flag_content_rel',
      'override' => array(
        'button' => 'Usar o padrão',
      ),
    ),
  ));
  $handler->override_option('arguments', array(
    'uid' => array(
      'default_action' => 'default',
      'style_plugin' => 'default_summary',
      'style_options' => array(),
      'wildcard' => 'all',
      'wildcard_substitution' => 'Tudo',
      'title' => '',
      'breadcrumb' => '',
      'default_argument_type' => 'user',
      'default_argument' => '',
      'validate_type' => 'none',
      'validate_fail' => 'not found',
      'break_phrase' => 0,
      'not' => 0,
      'id' => 'uid',
      'table' => 'users',
      'field' => 'uid',
      'validate_user_argument_type' => 'uid',
      'validate_user_roles' => array(
        2 => 0,
        3 => 0,
        4 => 0,
        5 => 0,
        6 => 0,
      ),
      'override' => array(
        'button' => 'Usar o padrão',
      ),
      'relationship' => 'uid',
      'default_options_div_prefix' => '',
      'default_taxonomy_tid_term_page' => 0,
      'default_taxonomy_tid_node' => 0,
      'default_taxonomy_tid_limit' => 0,
      'default_taxonomy_tid_vids' => array(
        5 => 0,
        3 => 0,
        1 => 0,
        2 => 0,
        4 => 0,
      ),
      'default_argument_user' => 0,
      'default_argument_fixed' => '',
      'default_argument_php' => '',
      'validate_argument_node_type' => array(
        'poll' => 0,
        'blog' => 0,
        'group' => 0,
        'profile' => 0,
        'shoutbox' => 0,
        'question' => 0,
        'book' => 0,
        'event' => 0,
        'feed_ical' => 0,
        'feed_ical_item' => 0,
        'discussion' => 0,
        'milestone' => 0,
        'task' => 0,
        'webform' => 0,
        'images' => 0,
        'text' => 0,
        'graphmind' => 0,
        'sheetnode' => 0,
        'page' => 0,
      ),
      'validate_argument_node_access' => 0,
      'validate_argument_nid_type' => 'nid',
      'validate_argument_vocabulary' => array(
        5 => 0,
        3 => 0,
        1 => 0,
        2 => 0,
        4 => 0,
      ),
      'validate_argument_type' => 'tid',
      'validate_argument_transform' => 0,
      'validate_user_restrict_roles' => 0,
      'validate_argument_node_flag_name' => '*relationship*',
      'validate_argument_node_flag_test' => 'flagged',
      'validate_argument_node_flag_id_type' => 'id',
      'validate_argument_user_flag_name' => '*relationship*',
      'validate_argument_user_flag_test' => 'flaggable',
      'validate_argument_user_flag_id_type' => 'id',
      'validate_argument_is_member' => '0',
      'validate_argument_group_node_type' => array(
        'group' => 0,
      ),
      'validate_argument_php' => '',
    ),
  ));
  $handler->override_option('title', 'Conhecimentos que sou especialista');
  $handler->override_option('block_description', 'Knowledge Expert');
  $handler->override_option('block_caching', -1);
  $handler = $view->new_display('block', 'Knowledge Studying', 'block_3');
  $handler->override_option('relationships', array(
    'flag_content_rel' => array(
      'label' => 'flag2',
      'required' => 1,
      'flag' => 'studying',
      'user_scope' => 'any',
      'id' => 'flag_content_rel',
      'table' => 'node',
      'field' => 'flag_content_rel',
      'override' => array(
        'button' => 'Usar o padrão',
      ),
      'relationship' => 'none',
    ),
    'uid' => array(
      'label' => 'Flag user',
      'required' => 0,
      'id' => 'uid',
      'table' => 'flag_content',
      'field' => 'uid',
      'relationship' => 'flag_content_rel',
      'override' => array(
        'button' => 'Usar o padrão',
      ),
    ),
  ));
  $handler->override_option('arguments', array(
    'uid' => array(
      'default_action' => 'default',
      'style_plugin' => 'default_summary',
      'style_options' => array(),
      'wildcard' => 'all',
      'wildcard_substitution' => 'Tudo',
      'title' => '',
      'breadcrumb' => '',
      'default_argument_type' => 'user',
      'default_argument' => '',
      'validate_type' => 'none',
      'validate_fail' => 'not found',
      'break_phrase' => 0,
      'not' => 0,
      'id' => 'uid',
      'table' => 'users',
      'field' => 'uid',
      'validate_user_argument_type' => 'uid',
      'validate_user_roles' => array(
        2 => 0,
        3 => 0,
        4 => 0,
        5 => 0,
        6 => 0,
      ),
      'override' => array(
        'button' => 'Usar o padrão',
      ),
      'relationship' => 'uid',
      'default_options_div_prefix' => '',
      'default_taxonomy_tid_term_page' => 0,
      'default_taxonomy_tid_node' => 0,
      'default_taxonomy_tid_limit' => 0,
      'default_taxonomy_tid_vids' => array(
        5 => 0,
        3 => 0,
        6 => 0,
        1 => 0,
        2 => 0,
        8 => 0,
        4 => 0,
      ),
      'default_argument_user' => 0,
      'default_argument_fixed' => '',
      'default_argument_php' => '',
      'validate_argument_node_type' => array(
        'poll' => 0,
        'blog' => 0,
        'group' => 0,
        'profile' => 0,
        'question' => 0,
        'book' => 0,
        'event' => 0,
        'feed_ical' => 0,
        'feed_ical_item' => 0,
        'discussion' => 0,
        'milestone' => 0,
        'task' => 0,
        'webform' => 0,
        'images' => 0,
        'text' => 0,
        'graphmind' => 0,
        'patch' => 0,
        'sheetnode' => 0,
        'board' => 0,
        'page' => 0,
        'service_provider' => 0,
        'toolkit' => 0,
      ),
      'validate_argument_node_access' => 0,
      'validate_argument_nid_type' => 'nid',
      'validate_argument_vocabulary' => array(
        5 => 0,
        3 => 0,
        6 => 0,
        1 => 0,
        2 => 0,
        8 => 0,
        4 => 0,
      ),
      'validate_argument_type' => 'tid',
      'validate_argument_transform' => 0,
      'validate_user_restrict_roles' => 0,
      'validate_argument_node_flag_name' => '*relationship*',
      'validate_argument_node_flag_test' => 'flagged',
      'validate_argument_node_flag_id_type' => 'id',
      'validate_argument_user_flag_name' => '*relationship*',
      'validate_argument_user_flag_test' => 'flaggable',
      'validate_argument_user_flag_id_type' => 'id',
      'validate_argument_is_member' => '0',
      'validate_argument_group_node_type' => array(
        'group' => 0,
      ),
      'validate_argument_php' => '',
    ),
  ));
  $handler->override_option('title', 'Conhecimentos que estou estudando');
  $handler->override_option('block_description', 'Knowledge Studying');
  $handler->override_option('block_caching', -1);
  $handler = $view->new_display('block', 'My studies', 'block_4');
  $handler->override_option('relationships', array(
    'flag_content_rel' => array(
      'label' => 'flag2',
      'required' => 1,
      'flag' => 'studying',
      'user_scope' => 'any',
      'id' => 'flag_content_rel',
      'table' => 'node',
      'field' => 'flag_content_rel',
      'override' => array(
        'button' => 'Usar o padrão',
      ),
      'relationship' => 'none',
    ),
    'uid' => array(
      'label' => 'Flag user',
      'required' => 0,
      'id' => 'uid',
      'table' => 'flag_content',
      'field' => 'uid',
      'relationship' => 'flag_content_rel',
      'override' => array(
        'button' => 'Usar o padrão',
      ),
    ),
  ));
  $handler->override_option('arguments', array(
    'uid' => array(
      'default_action' => 'default',
      'style_plugin' => 'default_summary',
      'style_options' => array(),
      'wildcard' => 'all',
      'wildcard_substitution' => 'Tudo',
      'title' => '',
      'breadcrumb' => '',
      'default_argument_type' => 'current_user',
      'default_argument' => '',
      'validate_type' => 'none',
      'validate_fail' => 'not found',
      'break_phrase' => 0,
      'not' => 0,
      'id' => 'uid',
      'table' => 'users',
      'field' => 'uid',
      'validate_user_argument_type' => 'uid',
      'validate_user_roles' => array(
        2 => 0,
        3 => 0,
        4 => 0,
        5 => 0,
        6 => 0,
      ),
      'override' => array(
        'button' => 'Usar o padrão',
      ),
      'relationship' => 'uid',
      'default_options_div_prefix' => '',
      'default_taxonomy_tid_term_page' => 0,
      'default_taxonomy_tid_node' => 0,
      'default_taxonomy_tid_limit' => 0,
      'default_taxonomy_tid_vids' => array(
        5 => 0,
        3 => 0,
        6 => 0,
        1 => 0,
        2 => 0,
        8 => 0,
        4 => 0,
      ),
      'default_argument_user' => 0,
      'default_argument_fixed' => '',
      'default_argument_php' => '',
      'validate_argument_node_type' => array(
        'poll' => 0,
        'blog' => 0,
        'group' => 0,
        'profile' => 0,
        'question' => 0,
        'book' => 0,
        'event' => 0,
        'feed_ical' => 0,
        'feed_ical_item' => 0,
        'discussion' => 0,
        'milestone' => 0,
        'task' => 0,
        'webform' => 0,
        'images' => 0,
        'text' => 0,
        'graphmind' => 0,
        'patch' => 0,
        'sheetnode' => 0,
        'board' => 0,
        'page' => 0,
        'service_provider' => 0,
        'toolkit' => 0,
      ),
      'validate_argument_node_access' => 0,
      'validate_argument_nid_type' => 'nid',
      'validate_argument_vocabulary' => array(
        5 => 0,
        3 => 0,
        6 => 0,
        1 => 0,
        2 => 0,
        8 => 0,
        4 => 0,
      ),
      'validate_argument_type' => 'tid',
      'validate_argument_transform' => 0,
      'validate_user_restrict_roles' => 0,
      'validate_argument_node_flag_name' => '*relationship*',
      'validate_argument_node_flag_test' => 'flagged',
      'validate_argument_node_flag_id_type' => 'id',
      'validate_argument_user_flag_name' => '*relationship*',
      'validate_argument_user_flag_test' => 'flaggable',
      'validate_argument_user_flag_id_type' => 'id',
      'validate_argument_is_member' => '0',
      'validate_argument_group_node_type' => array(
        'group' => 0,
      ),
      'validate_argument_php' => '',
    ),
  ));
  $handler->override_option('title', 'Conhecimentos que estou estudando');
  $handler->override_option('block_description', 'Meus estudos');
  $handler->override_option('block_caching', -1);
  $handler = $view->new_display('pdf', 'PDF Page', 'pdf_1');
  $handler->override_option('relationships', array(
    'flag_content_rel' => array(
      'label' => 'flag',
      'required' => 1,
      'flag' => 'studying',
      'user_scope' => 'current',
      'id' => 'flag_content_rel',
      'table' => 'node',
      'field' => 'flag_content_rel',
      'relationship' => 'none',
      'override' => array(
        'button' => 'Usar o padrão',
      ),
    ),
  ));
  $handler->override_option('fields', array(
    'title' => array(
      'label' => '',
      'alter' => array(
        'alter_text' => 0,
        'text' => '',
        'make_link' => 0,
        'path' => '',
        'absolute' => 0,
        'link_class' => '',
        'alt' => '',
        'rel' => '',
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
      'hide_alter_empty' => 1,
      'link_to_node' => 1,
      'spaces' => array(
        'frontpage' => 0,
        'type' => 'spaces_og',
      ),
      'exclude' => 0,
      'id' => 'title',
      'table' => 'node',
      'field' => 'title',
      'override' => array(
        'button' => 'Sobrescrever',
      ),
      'relationship' => 'none',
    ),
    'field_card_description_value' => array(
      'id' => 'field_card_description_value',
      'table' => 'node_data_field_card_description',
      'field' => 'field_card_description_value',
    ),
    'field_input_value' => array(
      'label' => 'Input',
      'alter' => array(
        'alter_text' => 0,
        'text' => '',
        'make_link' => 0,
        'path' => '',
        'absolute' => 0,
        'link_class' => '',
        'alt' => '',
        'rel' => '',
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
      'hide_alter_empty' => 1,
      'link_to_node' => 0,
      'label_type' => 'widget',
      'format' => 'default',
      'multiple' => array(
        'group' => 1,
        'multiple_number' => '',
        'multiple_from' => '',
        'multiple_reversed' => 0,
      ),
      'exclude' => 0,
      'id' => 'field_input_value',
      'table' => 'node_data_field_input',
      'field' => 'field_input_value',
      'override' => array(
        'button' => 'Usar o padrão',
      ),
      'relationship' => 'none',
    ),
    'field_output_value' => array(
      'label' => 'Ouput',
      'alter' => array(
        'alter_text' => 0,
        'text' => '',
        'make_link' => 0,
        'path' => '',
        'absolute' => 0,
        'link_class' => '',
        'alt' => '',
        'rel' => '',
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
      'hide_alter_empty' => 1,
      'link_to_node' => 0,
      'label_type' => 'widget',
      'format' => 'default',
      'multiple' => array(
        'group' => 1,
        'multiple_number' => '',
        'multiple_from' => '',
        'multiple_reversed' => 0,
      ),
      'exclude' => 0,
      'id' => 'field_output_value',
      'table' => 'node_data_field_output',
      'field' => 'field_output_value',
      'override' => array(
        'button' => 'Usar o padrão',
      ),
      'relationship' => 'none',
    ),
    'body' => array(
      'label' => 'Corpo',
      'alter' => array(
        'alter_text' => 0,
        'text' => '',
        'make_link' => 0,
        'path' => '',
        'absolute' => 0,
        'link_class' => '',
        'alt' => '',
        'rel' => '',
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
      'hide_alter_empty' => 1,
      'exclude' => 0,
      'id' => 'body',
      'table' => 'node_revisions',
      'field' => 'body',
      'override' => array(
        'button' => 'Usar o padrão',
      ),
      'relationship' => 'none',
    ),
  ));
  $handler->override_option('filters', array(
    'field_card_description_value' => array(
      'operator' => 'not empty',
      'value' => '',
      'group' => '0',
      'exposed' => FALSE,
      'expose' => array(
        'operator' => FALSE,
        'label' => '',
        'optional' => FALSE,
      ),
      'case' => 1,
      'id' => 'field_card_description_value',
      'table' => 'node_data_field_card_description',
      'field' => 'field_card_description_value',
      'override' => array(
        'button' => 'Usar o padrão',
      ),
      'relationship' => 'none',
    ),
  ));
  $handler->override_option('style_plugin', 'pdf_unformatted');
  $handler->override_option('style_options', array(
    'mission_description' => FALSE,
    'description' => '',
  ));
  $handler->override_option('row_plugin', 'pdf_fields');
  $handler->override_option('row_options', array(
    'formats' => array(
      'title' => array(
        'position' => array(
          'object' => 'last_position',
          'corner' => 'top_left',
          'x' => '',
          'y' => '0',
          'width' => '',
          'height' => '',
        ),
        'text' => array(
          'font_size' => '14',
          'font_family' => 'default',
          'font_style' => array(
            'b' => 0,
            'i' => 0,
            'u' => 0,
            'd' => 0,
            'o' => 0,
          ),
          'align' => NULL,
          'hyphenate' => NULL,
          'color' => '',
        ),
        'render' => array(
          'is_html' => 1,
          'minimal_space' => '1',
          'eval_before' => '',
          'eval_after' => '',
        ),
      ),
      'field_card_description_value' => array(
        'position' => array(
          'object' => 'field_title',
          'corner' => 'top_left',
          'x' => '',
          'y' => '100',
          'width' => '',
          'height' => '',
        ),
        'text' => array(
          'font_size' => '10',
          'font_family' => 'times',
          'font_style' => array(
            'b' => 0,
            'i' => 0,
            'u' => 0,
            'd' => 0,
            'o' => 0,
          ),
          'align' => NULL,
          'hyphenate' => NULL,
          'color' => '',
        ),
        'render' => array(
          'is_html' => 1,
          'minimal_space' => '1',
          'eval_before' => '',
          'eval_after' => '',
        ),
      ),
      'field_input_value' => array(
        'position' => array(
          'object' => 'self',
          'corner' => 'top_left',
          'x' => '',
          'y' => '300',
          'width' => '',
          'height' => '',
        ),
        'text' => array(
          'font_size' => '20',
          'font_family' => 'default',
          'font_style' => array(
            'b' => 0,
            'i' => 0,
            'u' => 0,
            'd' => 0,
            'o' => 0,
          ),
          'align' => NULL,
          'hyphenate' => NULL,
          'color' => '',
        ),
        'render' => array(
          'is_html' => 1,
          'minimal_space' => '1',
          'eval_before' => '',
          'eval_after' => '',
        ),
      ),
      'field_output_value' => array(
        'position' => array(
          'object' => 'self',
          'corner' => 'top_left',
          'x' => '400',
          'y' => '200',
          'width' => '',
          'height' => '',
        ),
        'text' => array(
          'font_size' => '20',
          'font_family' => 'default',
          'font_style' => array(
            'b' => 0,
            'i' => 0,
            'u' => 0,
            'd' => 0,
            'o' => 0,
          ),
          'align' => NULL,
          'hyphenate' => NULL,
          'color' => '',
        ),
        'render' => array(
          'is_html' => 1,
          'minimal_space' => '1',
          'eval_before' => '',
          'eval_after' => '',
        ),
      ),
      'body' => array(
        'position' => array(
          'object' => 'field_title',
          'corner' => 'top_left',
          'x' => '',
          'y' => '',
          'width' => '',
          'height' => '',
        ),
        'text' => array(
          'font_size' => '10',
          'font_family' => 'default',
          'font_style' => array(
            'b' => 0,
            'i' => 0,
            'u' => 0,
            'd' => 0,
            'o' => 0,
          ),
          'align' => NULL,
          'hyphenate' => NULL,
          'color' => '',
        ),
        'render' => array(
          'is_html' => 1,
          'minimal_space' => '1',
          'eval_before' => '',
          'eval_after' => '',
        ),
      ),
    ),
    'leading_template' => '0',
    'template' => '0',
    'succeed_template' => '0',
    'template_file' => '',
  ));
  $handler->override_option('path', 'studying_cards');
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
  $handler->override_option('displays', array());
  $handler->override_option('sitename_title', FALSE);
  $handler->override_option('default_page_format', 'A4');
  $handler->override_option('default_page_format_custom', '');
  $handler->override_option('default_page_orientation', 'L');
  $handler->override_option('unit', 'mm');
  $handler->override_option('margin_left', '15');
  $handler->override_option('margin_right', '15');
  $handler->override_option('margin_top', '20');
  $handler->override_option('margin_bottom', '30');
  $handler->override_option('leading_template', '');
  $handler->override_option('template', '');
  $handler->override_option('succeed_template', '');
  $handler->override_option('default_font_family', 'helvetica');
  $handler->override_option('default_font_style', array());
  $handler->override_option('default_font_size', '11');
  $handler->override_option('default_text_align', 'L');
  $handler->override_option('default_font_color', '000000');
  $handler->override_option('default_text_hyphenate', 'none');
  $handler->override_option('css_file', '');
  $translatables['my_flagged'] = array(
    t('Knowledge Expert'),
    t('Knowledge Studying'),
    t('Knowledge Used'),
    t('Knowledge that I have used'),
    t('Knowledge that I\'m an expert in'),
    t('My studies'),
    t('PDF Page'),
    t('Padrões'),
    t('What I\'m studying now'),
  );

  $views[$view->name] = $view;

  // Exported view: service_providers
  $view = new view;
  $view->name = 'service_providers';
  $view->description = '';
  $view->tag = '';
  $view->base_table = 'location';
  $view->core = 6;
  $view->api_version = '2';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */
  $handler = $view->new_display('default', 'Padrões', 'default');
  $handler->override_option('fields', array(
    'title' => array(
      'label' => '',
      'alter' => array(
        'alter_text' => 0,
        'text' => '',
        'make_link' => 0,
        'path' => '',
        'absolute' => 0,
        'link_class' => '',
        'alt' => '',
        'rel' => '',
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
      'hide_alter_empty' => 1,
      'link_to_node' => 1,
      'spaces' => array(
        'frontpage' => 0,
        'type' => 'spaces_og',
      ),
      'exclude' => 0,
      'id' => 'title',
      'table' => 'node',
      'field' => 'title',
      'override' => array(
        'button' => 'Sobrescrever',
      ),
      'relationship' => 'none',
    ),
    'type' => array(
      'label' => '',
      'alter' => array(
        'alter_text' => 0,
        'text' => '',
        'make_link' => 0,
        'path' => '',
        'absolute' => 0,
        'link_class' => '',
        'alt' => '',
        'rel' => '',
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
      'hide_alter_empty' => 1,
      'link_to_node' => 0,
      'machine_name' => 0,
      'exclude' => 0,
      'id' => 'type',
      'table' => 'node',
      'field' => 'type',
      'relationship' => 'none',
    ),
    'name' => array(
      'label' => '',
      'alter' => array(
        'alter_text' => 0,
        'text' => '',
        'make_link' => 0,
        'path' => '',
        'absolute' => 0,
        'link_class' => '',
        'alt' => '',
        'rel' => '',
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
      'hide_empty' => 1,
      'empty_zero' => 0,
      'hide_alter_empty' => 1,
      'link_to_user' => 1,
      'overwrite_anonymous' => 1,
      'anonymous_text' => '',
      'exclude' => 0,
      'id' => 'name',
      'table' => 'users',
      'field' => 'name',
      'relationship' => 'none',
    ),
  ));
  $handler->override_option('filters', array(
    'type' => array(
      'operator' => 'in',
      'value' => array(
        'profile' => 'profile',
        'service_provider' => 'service_provider',
      ),
      'group' => '0',
      'exposed' => FALSE,
      'expose' => array(
        'operator' => 'type_op',
        'label' => 'Filtrar',
        'use_operator' => 0,
        'identifier' => 'type',
        'optional' => 1,
        'single' => 1,
        'remember' => 0,
        'reduce' => 1,
      ),
      'id' => 'type',
      'table' => 'node',
      'field' => 'type',
      'relationship' => 'none',
    ),
  ));
  $handler->override_option('access', array(
    'type' => 'none',
  ));
  $handler->override_option('cache', array(
    'type' => 'none',
  ));
  $handler->override_option('title', 'Prestadores de Serviços');
  $handler->override_option('items_per_page', 0);
  $handler->override_option('style_plugin', 'gmap');
  $handler->override_option('style_options', array(
    'grouping' => '',
    'macro' => '[gmap | width = 100% | height = 500px ]',
    'datasource' => 'location',
    'latfield' => 'title',
    'lonfield' => 'title',
    'markers' => 'nodetype',
    'markerfield' => 'title',
    'markertype' => 'small orange',
    'center_on_nodearg' => 0,
    'center_on_nodearg_arg' => '',
    'highlight_nodearg' => 0,
    'highlight_nodearg_arg' => '',
    'highlight_nodearg_color' => '#FF0000',
    'tooltipenabled' => 1,
    'tooltipfield' => 'title',
  ));
  $handler = $view->new_display('page', 'Página', 'page_1');
  $handler->override_option('path', 'knowledge/providers');
  $handler->override_option('menu', array(
    'type' => 'tab',
    'title' => 'Service Providers',
    'description' => '',
    'weight' => '0',
    'name' => 'navigation',
  ));
  $handler->override_option('tab_options', array(
    'type' => 'none',
    'title' => '',
    'description' => '',
    'weight' => 0,
    'name' => 'navigation',
  ));
  $handler = $view->new_display('block', 'Bloco', 'block_1');
  $handler->override_option('fields', array(
    'title' => array(
      'label' => '',
      'alter' => array(
        'alter_text' => 0,
        'text' => '',
        'make_link' => 0,
        'path' => '',
        'absolute' => 0,
        'link_class' => '',
        'alt' => '',
        'rel' => '',
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
      'hide_alter_empty' => 1,
      'link_to_node' => 1,
      'spaces' => array(
        'frontpage' => 0,
        'type' => 'spaces_og',
      ),
      'exclude' => 0,
      'id' => 'title',
      'table' => 'node',
      'field' => 'title',
      'override' => array(
        'button' => 'Sobrescrever',
      ),
      'relationship' => 'none',
    ),
    'type' => array(
      'label' => '',
      'alter' => array(
        'alter_text' => 0,
        'text' => '',
        'make_link' => 0,
        'path' => '',
        'absolute' => 0,
        'link_class' => '',
        'alt' => '',
        'rel' => '',
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
      'hide_alter_empty' => 1,
      'link_to_node' => 0,
      'machine_name' => 0,
      'exclude' => 0,
      'id' => 'type',
      'table' => 'node',
      'field' => 'type',
      'relationship' => 'none',
    ),
  ));
  $handler->override_option('filters', array());
  $handler->override_option('block_description', 'Service Provider Map');
  $handler->override_option('block_caching', -1);
  $translatables['service_providers'] = array(
    t('Bloco'),
    t('Padrões'),
    t('Página'),
    t('Service Provider Map'),
    t('Service Providers'),
  );

  $views[$view->name] = $view;

  return $views;
}
