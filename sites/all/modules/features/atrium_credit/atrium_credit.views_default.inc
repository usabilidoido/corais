<?php

/**
 * Implementation of hook_views_default_views().
 */
function atrium_credit_views_default_views() {
  $views = array();

  // Exported view: mc_exchanges_all
	$view = new view;
	$view->name = 'mc_exchanges_all';
	$view->description = 'list of all exchanges';
	$view->tag = 'exchanges';
	$view->base_table = 'node';
	$view->core = 0;
	$view->api_version = '2';
	$view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */
	$handler = $view->new_display('default', 'Defaults', 'default');
	$handler->override_option('fields', array(
	  'changed' => array(
		'label' => 'Data',
		'date_format' => 'custom',
		'custom_date_format' => 'd-m-y',
		'exclude' => 0,
		'id' => 'changed',
		'table' => 'node',
		'field' => 'changed',
		'relationship' => 'none',
	  ),
	  'title' => array(
		'label' => 'Descrição',
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
	  'payer_uid' => array(
		'label' => 'Sacado',
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
		'link_to_payer' => TRUE,
		'exclude' => 0,
		'link_to_user' => 0,
		'overwrite_anonymous' => 0,
		'anonymous_text' => '',
		'id' => 'payer_uid',
		'table' => 'mc_exchanges',
		'field' => 'payer_uid',
		'relationship' => 'none',
		'link_to_trader' => 1,
	  ),
	  'payee_uid' => array(
		'label' => 'Payee',
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
		'link_to_payer' => TRUE,
		'exclude' => 0,
		'link_to_user' => 0,
		'overwrite_anonymous' => 0,
		'anonymous_text' => '',
		'id' => 'payee_uid',
		'table' => 'mc_exchanges',
		'field' => 'payee_uid',
		'relationship' => 'none',
		'link_to_trader' => 1,
	  ),
	  'quantity' => array(
		'label' => 'Valor',
		'exclude' => 0,
		'id' => 'quantity',
		'table' => 'mc_exchanges',
		'field' => 'quantity',
		'relationship' => 'none',
	  ),
	));
	$handler->override_option('filters', array(
	  'type' => array(
		'operator' => 'in',
		'value' => array(
		  'exchange' => 'exchange',
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
	  'exchange_type' => array(
		'operator' => 'or',
		'value' => array(
		  '3rdparty' => '3rdparty',
		  'incoming direct' => 'incoming direct',
		  'incoming confirm' => 'incoming confirm',
		  'outgoing direct' => 'outgoing direct',
		  'outgoing confirm' => 'outgoing confirm',
		),
		'group' => '0',
		'exposed' => FALSE,
		'expose' => array(
		  'operator' => FALSE,
		  'label' => '',
		),
		'id' => 'exchange_type',
		'table' => 'mc_exchanges',
		'field' => 'exchange_type',
		'relationship' => 'none',
		'reduce_duplicates' => 0,
		'override' => array(
		  'button' => 'Override',
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
		'relationship' => 'none',
	  ),
	));
	$handler->override_option('access', array(
	  'type' => 'spaces_feature',
	  'spaces_feature' => 'atrium_credit',
	  'perm' => 'access content',
	));
	$handler->override_option('cache', array(
	  'type' => 'none',
	));
	$handler->override_option('title', 'Exchange list');
	$handler->override_option('footer_format', '1');
	$handler->override_option('empty', 'There are no exchanges in the system yet');
	$handler->override_option('empty_format', '1');
	$handler->override_option('items_per_page', 25);
	$handler->override_option('use_pager', 'mini');
	$handler->override_option('distinct', 0);
	$handler->override_option('style_plugin', 'table');
	$handler->override_option('style_options', array(
	  'grouping' => '',
	  'override' => 1,
	  'sticky' => 0,
	  'order' => 'desc',
	  'columns' => array(
		'changed' => 'changed',
		'title' => 'title',
		'payer_uid' => 'payer_uid',
		'payee_uid' => 'payee_uid',
		'quantity' => 'quantity',
		'rating' => 'rating',
	  ),
	  'info' => array(
		'created' => array(
		  'sortable' => 1,
		  'separator' => '',
		),
		'title' => array(
		  'sortable' => 0,
		  'separator' => '',
		),
		'payer_uid' => array(
		  'separator' => '',
		),
		'payee_uid' => array(
		  'separator' => '',
		),
		'quantity' => array(
		  'sortable' => 1,
		  'separator' => '',
		),
		'rating' => array(
		  'sortable' => 0,
		  'separator' => '',
		),
	  ),
	  'default' => 'changed',
	));
	$handler = $view->new_display('page', 'Member exchanges', 'page_1');
	$handler->override_option('filters', array(
	  'type' => array(
		'operator' => 'in',
		'value' => array(
		  'exchange' => 'exchange',
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
	  'exchange_type' => array(
		'operator' => 'or',
		'value' => array(
		  '3rdparty' => '3rdparty',
		  'outgoing direct' => 'outgoing direct',
		  'incoming direct' => 'incoming direct',
		  'outgoing signed' => 'outgoing signed',
		  'incoming signed' => 'incoming signed',
		  'manytoone' => 'manytoone',
		  'onetomany' => 'onetomany',
		  'alltoone' => 'alltoone',
		  'onetoall' => 'onetoall',
		),
		'group' => '0',
		'exposed' => FALSE,
		'expose' => array(
		  'operator' => FALSE,
		  'label' => '',
		),
		'id' => 'exchange_type',
		'table' => 'mc_exchanges',
		'field' => 'exchange_type',
		'relationship' => 'none',
		'reduce_duplicates' => 0,
		'override' => array(
		  'button' => 'Use default',
		),
	  ),
	  'payee_uid_1' => array(
		'operator' => 'in',
		'value' => '',
		'group' => '0',
		'exposed' => TRUE,
		'expose' => array(
		  'use_operator' => 0,
		  'operator' => 'payee_uid_1_op',
		  'identifier' => 'payee_uid_1',
		  'label' => 'Payee',
		  'optional' => 1,
		  'remember' => 0,
		  'reduce' => 1,
		),
		'id' => 'payee_uid_1',
		'table' => 'mc_exchanges',
		'field' => 'payee_uid',
		'override' => array(
		  'button' => 'Use default',
		),
		'relationship' => 'none',
	  ),
	  'payer_uid' => array(
		'operator' => 'in',
		'value' => '',
		'group' => '0',
		'exposed' => TRUE,
		'expose' => array(
		  'use_operator' => 0,
		  'operator' => 'payer_uid_op',
		  'identifier' => 'payer_uid',
		  'label' => 'Payer',
		  'optional' => 1,
		  'remember' => 0,
		  'reduce' => 1,
		),
		'id' => 'payer_uid',
		'table' => 'mc_exchanges',
		'field' => 'payer_uid',
		'override' => array(
		  'button' => 'Use default',
		),
		'relationship' => 'none',
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
		  'button' => 'Usar o padrão',
		),
		'relationship' => 'none',
	  ),
	));
	$handler->override_option('header', 'all the exchanges on the system, except the mass-exchanges, and those involving admin');
	$handler->override_option('header_format', '1');
	$handler->override_option('header_empty', 0);
	$handler->override_option('path', 'exchanges');
	$handler->override_option('menu', array(
	  'type' => 'normal',
	  'title' => 'Mutual Credit',
	  'description' => 'Member-to-member exchanges',
	  'weight' => '20',
	  'name' => 'features',
	));
	$handler->override_option('tab_options', array(
	  'type' => 'normal',
	  'title' => 'Exchanges',
	  'description' => '',
	  'weight' => '0',
	  'name' => 'navigation',
	));
	$handler = $view->new_display('page', 'Filter exchanges', 'page_2');
	$handler->override_option('filters', array(
	  'type' => array(
		'operator' => 'in',
		'value' => array(
		  'exchange' => 'exchange',
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
	  'exchange_type' => array(
		'operator' => 'or',
		'value' => array(
		  'alltoone' => 'alltoone',
		  'onetoall' => 'onetoall',
		  'manytoone' => 'manytoone',
		  'onetomany' => 'onetomany',
		  '3rdparty' => '3rdparty',
		  'incoming direct' => 'incoming direct',
		  'outgoing direct' => 'outgoing direct',
		  'incoming signed' => 'incoming signed',
		  'outgoing signed' => 'outgoing signed',
		),
		'group' => '0',
		'exposed' => TRUE,
		'expose' => array(
		  'use_operator' => 0,
		  'operator' => 'exchange_type_op',
		  'identifier' => 'exchange_type',
		  'label' => 'Exchange type',
		  'optional' => 1,
		  'single' => 0,
		  'remember' => 0,
		  'reduce' => 0,
		),
		'id' => 'exchange_type',
		'table' => 'mc_exchanges',
		'field' => 'exchange_type',
		'override' => array(
		  'button' => 'Use default',
		),
		'relationship' => 'none',
		'reduce_duplicates' => 0,
	  ),
	  'payee_uid_1' => array(
		'operator' => 'in',
		'value' => '',
		'group' => '0',
		'exposed' => TRUE,
		'expose' => array(
		  'use_operator' => 0,
		  'operator' => 'payee_uid_1_op',
		  'identifier' => 'payee_uid_1',
		  'label' => 'Payee',
		  'optional' => 1,
		  'remember' => 0,
		  'reduce' => 0,
		),
		'id' => 'payee_uid_1',
		'table' => 'mc_exchanges',
		'field' => 'payee_uid',
		'override' => array(
		  'button' => 'Use default',
		),
		'relationship' => 'none',
	  ),
	  'payer_uid' => array(
		'operator' => 'in',
		'value' => '',
		'group' => '0',
		'exposed' => TRUE,
		'expose' => array(
		  'use_operator' => 0,
		  'operator' => 'payer_uid_op',
		  'identifier' => 'payer_uid',
		  'label' => 'Sacado',
		  'optional' => 1,
		  'remember' => 0,
		  'reduce' => 0,
		),
		'id' => 'payer_uid',
		'table' => 'mc_exchanges',
		'field' => 'payer_uid',
		'override' => array(
		  'button' => 'Use default',
		),
		'relationship' => 'none',
	  ),
	  'state' => array(
		'operator' => '=',
		'value' => 'All',
		'group' => '0',
		'exposed' => TRUE,
		'expose' => array(
		  'operator' => '',
		  'identifier' => 'state',
		  'label' => 'Pending',
		  'optional' => 1,
		  'remember' => 0,
		),
		'id' => 'state',
		'table' => 'mc_exchanges',
		'field' => 'state',
		'override' => array(
		  'button' => 'Use default',
		),
		'relationship' => 'none',
	  ),
	  'keys' => array(
		'operator' => 'optional',
		'value' => '',
		'group' => '0',
		'exposed' => TRUE,
		'expose' => array(
		  'use_operator' => 0,
		  'operator' => 'keys_op',
		  'identifier' => 'keys',
		  'label' => 'keywords',
		  'optional' => 1,
		  'remember' => 0,
		),
		'id' => 'keys',
		'table' => 'search_index',
		'field' => 'keys',
		'override' => array(
		  'button' => 'Use default',
		),
		'relationship' => 'none',
	  ),
	));
	$handler->override_option('header', 'Use the widgets to filter through all the exchanges ever listed, and then sort the columns by clicking the column headings');
	$handler->override_option('header_format', '1');
	$handler->override_option('header_empty', 0);
	$handler->override_option('empty', 'No exchanges meet these criteria. Try using fewer filters');
	$handler->override_option('empty_format', 0);
	$handler->override_option('style_options', array(
	  'grouping' => '',
	  'override' => 1,
	  'sticky' => 0,
	  'order' => 'asc',
	  'columns' => array(
		'changed' => 'changed',
		'title' => 'title',
		'payer_uid' => 'payer_uid',
		'payee_uid' => 'payee_uid',
		'quantity' => 'quantity',
		'rating' => 'rating',
	  ),
	  'info' => array(
		'changed' => array(
		  'sortable' => 1,
		  'separator' => '',
		),
		'title' => array(
		  'sortable' => 0,
		  'separator' => '',
		),
		'payer_uid' => array(
		  'sortable' => 1,
		  'separator' => '',
		),
		'payee_uid' => array(
		  'sortable' => 1,
		  'separator' => '',
		),
		'quantity' => array(
		  'sortable' => 1,
		  'separator' => '',
		),
		'rating' => array(
		  'sortable' => 1,
		  'separator' => '',
		),
	  ),
	  'default' => '-1',
	));
	$handler->override_option('path', 'exchanges/filter');
	$handler->override_option('menu', array(
	  'type' => 'tab',
	  'title' => 'Filtered exchanges',
	  'description' => '',
	  'weight' => '25',
	  'name' => 'navigation',
	));
	$handler->override_option('tab_options', array(
	  'type' => 'none',
	  'title' => '',
	  'description' => '',
	  'weight' => 0,
	  'name' => 'navigation',
	));
	$handler = $view->new_display('block', 'who gave to whom', 'block_1');
	$handler->override_option('fields', array(
	  'payer_uid' => array(
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
		  'word_boundary' => 0,
		  'ellipsis' => 0,
		  'html' => 0,
		  'strip_tags' => 0,
		),
		'empty' => '',
		'hide_empty' => 0,
		'empty_zero' => 0,
		'link_to_payer' => TRUE,
		'exclude' => 1,
		'id' => 'payer_uid',
		'table' => 'mc_exchanges',
		'field' => 'payer_uid',
		'override' => array(
		  'button' => 'Use default',
		),
		'relationship' => 'none',
		'link_to_trader' => 1,
	  ),
	  'payee_uid' => array(
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
		'link_to_payer' => TRUE,
		'exclude' => 1,
		'id' => 'payee_uid',
		'table' => 'mc_exchanges',
		'field' => 'payee_uid',
		'override' => array(
		  'button' => 'Use default',
		),
		'relationship' => 'none',
		'link_to_trader' => 1,
	  ),
	  'quantity' => array(
		'label' => '',
		'alter' => array(
		  'alter_text' => 1,
		  'text' => '[payer_uid] gave [quantity] to [payee_uid]',
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
		'id' => 'quantity',
		'table' => 'mc_exchanges',
		'field' => 'quantity',
		'relationship' => 'none',
		'override' => array(
		  'button' => 'Use default',
		),
	  ),
	));
	$handler->override_option('items_per_page', 5);
	$handler->override_option('use_pager', '0');
	$handler->override_option('style_plugin', 'list');
	$handler->override_option('style_options', array(
	  'grouping' => '',
	  'type' => 'ul',
	));
	$handler->override_option('row_options', array(
	  'inline' => array(
		'payer_uid' => 'payer_uid',
		'quantity' => 'quantity',
		'payee_uid' => 'payee_uid',
	  ),
	  'separator' => '',
	  'hide_empty' => 0,
	));
	$handler->override_option('block_description', '');
	$handler->override_option('block_caching', -1);
	$handler = $view->new_display('block', 'exchanging what?', 'block_2');
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
		'exclude' => 0,
		'id' => 'title',
		'table' => 'node',
		'field' => 'title',
		'relationship' => 'none',
		'override' => array(
		  'button' => 'Use default',
		),
	  ),
	  'quantity' => array(
		'label' => '',
		'alter' => array(
		  'alter_text' => 0,
		  'text' => ':[quantity]',
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
		'id' => 'quantity',
		'table' => 'mc_exchanges',
		'field' => 'quantity',
		'override' => array(
		  'button' => 'Use default',
		),
		'relationship' => 'none',
	  ),
	));
	$handler->override_option('items_per_page', 5);
	$handler->override_option('use_pager', '0');
	$handler->override_option('style_plugin', 'default');
	$handler->override_option('style_options', array());
	$handler->override_option('row_options', array(
	  'inline' => array(
		'title' => 'title',
		'quantity' => 'quantity',
	  ),
	  'separator' => ':',
	  'hide_empty' => 0,
	));
	$handler->override_option('block_description', '');
	$handler->override_option('block_caching', -1);
	$translatables['mc_exchanges_all'] = array(
	  t('Defaults'),
	  t('Exchange list'),
	  t('Filter exchanges'),
	  t('Member exchanges'),
	  t('No exchanges meet these criteria. Try using fewer filters'),
	  t('There are no exchanges in the system yet'),
	  t('Use the widgets to filter through all the exchanges ever listed, and then sort the columns by clicking the column headings'),
	  t('all the exchanges on the system, except the mass-exchanges, and those involving admin'),
	  t('exchanging what?'),
	  t('who gave to whom'),
	);


  // Exported view: mc_exchanges_pending

	$view = new view;
$view->name = 'mc_exchanges_pending';
$view->description = 'pending exchanges, and their signatories';
$view->tag = 'exchanges';
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
  'cid' => array(
    'label' => 'Currency ID',
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
    'exclude' => 1,
    'id' => 'cid',
    'table' => 'mc_exchanges',
    'field' => 'cid',
    'relationship' => 'none',
  ),
  'quantity' => array(
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
    'id' => 'quantity',
    'table' => 'mc_exchanges',
    'field' => 'quantity',
    'relationship' => 'none',
    'override' => array(
      'button' => 'Override',
    ),
  ),
  'signatories' => array(
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
    'id' => 'signatories',
    'table' => 'mc_unsigned',
    'field' => 'signatories',
    'relationship' => 'none',
  ),
));
$handler->override_option('sorts', array(
  'quantity' => array(
    'order' => 'DESC',
    'id' => 'quantity',
    'table' => 'mc_exchanges',
    'field' => 'quantity',
    'relationship' => 'none',
  ),
));
$handler->override_option('arguments', array(
  'participant' => array(
    'default_action' => 'default',
    'style_plugin' => 'default_summary',
    'style_options' => array(),
    'wildcard' => 'all',
    'wildcard_substitution' => 'All',
    'title' => '',
    'breadcrumb' => '',
    'default_argument_type' => 'user',
    'default_argument' => '',
    'validate_type' => 'user',
    'validate_fail' => 'not found',
    'break_phrase' => 0,
    'not' => 0,
    'id' => 'participant',
    'table' => 'mc_exchanges',
    'field' => 'participant',
    'validate_user_argument_type' => 'either',
    'relationship' => 'none',
    'default_options_div_prefix' => '',
    'default_argument_fixed' => '',
    'default_argument_user' => 0,
    'default_argument_php' => '',
    'validate_argument_node_access' => 0,
    'validate_argument_nid_type' => 'nid',
    'validate_argument_type' => 'tid',
    'validate_argument_transform' => 0,
    'validate_user_restrict_roles' => 0,
    'validate_argument_php' => '',
    'default_argument_image_size' => '_original',
    'override' => array(
      'button' => 'Supplanter',
    ),
  ),
));
$handler->override_option('filters', array(
  'uid' => array(
    'value' => 'participant',
    'id' => 'uid',
    'table' => 'mc_unsigned',
    'field' => 'uid',
    'relationship' => 'none',
  ),
));
$handler->override_option('access', array(
  'type' => 'spaces_feature',
  'spaces_feature' => '0',
  'perm' => 'access content',
));
$handler->override_option('cache', array(
  'type' => 'none',
));
$handler->override_option('items_per_page', 0);
$handler->override_option('row_options', array(
  'inline' => array(
    'title' => 'title',
    'quantity' => 'quantity',
    'cid' => 'cid',
    'signatories' => 'signatories',
  ),
  'separator' => '',
  'hide_empty' => 0,
));
$handler = $view->new_display('block', 'I must sign', 'block_1');
$handler->override_option('arguments', array(
  'participant' => array(
    'default_action' => 'default',
    'style_plugin' => 'default_summary',
    'style_options' => array(),
    'wildcard' => 'all',
    'wildcard_substitution' => 'All',
    'title' => '',
    'breadcrumb' => '',
    'default_argument_type' => 'current_user',
    'default_argument' => '',
    'validate_type' => 'user',
    'validate_fail' => 'not found',
    'break_phrase' => 0,
    'not' => 0,
    'id' => 'participant',
    'table' => 'mc_exchanges',
    'field' => 'participant',
    'validate_user_argument_type' => 'either',
    'validate_user_roles' => array(
      2 => 0,
    ),
    'relationship' => 'none',
    'default_options_div_prefix' => '',
    'default_argument_fixed' => '',
    'default_argument_user' => 0,
    'default_argument_php' => '',
    'validate_argument_node_access' => 0,
    'validate_argument_nid_type' => 'nid',
    'validate_argument_vocabulary' => array(
      1 => 0,
    ),
    'validate_argument_type' => 'tid',
    'validate_argument_transform' => 0,
    'validate_user_restrict_roles' => 0,
    'validate_argument_php' => '',
    'override' => array(
      'button' => 'Use default',
    ),
  ),
));
$handler->override_option('block_description', 'I must sign');
$handler->override_option('block_caching', -1);
$handler = $view->new_display('block', 'User must sign', 'block_2');
$handler->override_option('block_description', 'User must sign');
$handler->override_option('block_caching', -1);
$handler = $view->new_display('block', 'User waiting on', 'block_3');
$handler->override_option('filters', array(
  'uid' => array(
    'value' => 'another',
    'id' => 'uid',
    'table' => 'mc_unsigned',
    'field' => 'uid',
    'relationship' => 'none',
    'override' => array(
      'button' => 'Use default',
    ),
  ),
));
$handler->override_option('block_description', 'user waiting on');
$handler->override_option('block_caching', -1);
$handler = $view->new_display('page', 'All pending', 'page_1');
$handler->override_option('fields', array(
  'created' => array(
    'id' => 'created',
    'table' => 'node',
    'field' => 'created',
  ),
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
    'link_to_node' => 1,
    'exclude' => 0,
    'id' => 'title',
    'table' => 'node',
    'field' => 'title',
    'relationship' => 'none',
  ),
  'quantity' => array(
    'label' => 'Valor',
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
    'id' => 'quantity',
    'table' => 'mc_exchanges',
    'field' => 'quantity',
    'override' => array(
      'button' => 'Use default',
    ),
    'relationship' => 'none',
  ),
  'signatories' => array(
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
    'id' => 'signatories',
    'table' => 'mc_unsigned',
    'field' => 'signatories',
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
    'override' => array(
      'button' => 'Use default',
    ),
    'relationship' => 'none',
  ),
));
$handler->override_option('arguments', array());
$handler->override_option('filters', array(
  'state' => array(
    'operator' => '=',
    'value' => '1',
    'group' => '0',
    'exposed' => FALSE,
    'expose' => array(
      'operator' => FALSE,
      'label' => '',
    ),
    'id' => 'state',
    'table' => 'mc_exchanges',
    'field' => 'state',
    'override' => array(
      'button' => 'Override',
    ),
    'relationship' => 'none',
  ),
));
$handler->override_option('access', array(
  'type' => 'perm',
  'perm' => 'view all exchanges',
));
$handler->override_option('style_plugin', 'table');
$handler->override_option('style_options', array(
  'grouping' => '',
  'override' => 1,
  'sticky' => 0,
  'order' => 'asc',
  'columns' => array(
    'created' => 'created',
    'title' => 'title',
    'quantity' => 'quantity',
    'signatories' => 'signatories',
  ),
  'info' => array(
    'created' => array(
      'sortable' => 1,
      'separator' => '',
    ),
    'title' => array(
      'sortable' => 0,
      'separator' => '',
    ),
    'quantity' => array(
      'sortable' => 1,
      'separator' => '',
    ),
    'signatories' => array(
      'separator' => '',
    ),
  ),
  'default' => '-1',
));
$handler->override_option('row_options', array(
  'inline' => array(),
  'separator' => '',
  'hide_empty' => 0,
));
$handler->override_option('path', 'admin/mc/unsigned');
$handler->override_option('menu', array(
  'type' => 'normal',
  'title' => 'Unsigned exchanges',
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
$translatables['mc_exchanges_pending'] = array(
  t('All pending'),
  t('Defaults'),
  t('I must sign'),
  t('User must sign'),
  t('User waiting on'),
  t('user waiting on'),
);


  // Exported view: mc_exchanges_user
	
  $view = new view;
$view->name = 'mc_exchanges_user';
$view->description = 'exchanges of one user';
$view->tag = 'exchanges';
$view->base_table = 'node';
$view->core = 0;
$view->api_version = '2';
$view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */
$handler = $view->new_display('default', 'Defaults', 'default');
$handler->override_option('fields', array(
  'created' => array(
    'label' => 'Data',
    'date_format' => 'custom',
    'custom_date_format' => 'd-m-y',
    'exclude' => 0,
    'id' => 'created',
    'table' => 'node',
    'field' => 'created',
    'relationship' => 'none',
  ),
  'title' => array(
    'label' => 'Descrição',
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
  'payer_uid' => array(
    'label' => 'Sacado',
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
    'link_to_payer' => TRUE,
    'exclude' => 0,
    'link_to_user' => 0,
    'overwrite_anonymous' => 0,
    'anonymous_text' => '',
    'id' => 'payer_uid',
    'table' => 'mc_exchanges',
    'field' => 'payer_uid',
    'relationship' => 'none',
    'link_to_trader' => 1,
  ),
  'payee_uid' => array(
    'label' => 'Payee',
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
    'link_to_payer' => TRUE,
    'exclude' => 0,
    'link_to_user' => 0,
    'overwrite_anonymous' => 0,
    'anonymous_text' => '',
    'id' => 'payee_uid',
    'table' => 'mc_exchanges',
    'field' => 'payee_uid',
    'relationship' => 'none',
    'link_to_trader' => 1,
  ),
  'quantity' => array(
    'label' => 'Valor',
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
    'output_type' => 'normal',
    'exclude' => 0,
    'format' => 'outgoing',
    'type' => 'normal',
    'id' => 'quantity',
    'table' => 'mc_exchanges',
    'field' => 'quantity',
    'relationship' => 'none',
    'override' => array(
      'button' => 'Override',
    ),
  ),
  'quantity_1' => array(
    'label' => 'incoming',
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
    'output_type' => 'incoming',
    'exclude' => 0,
    'id' => 'quantity_1',
    'table' => 'mc_exchanges',
    'field' => 'quantity',
    'override' => array(
      'button' => 'Override',
    ),
    'relationship' => 'none',
  ),
  'quantity_2' => array(
    'label' => 'outgoing',
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
    'output_type' => 'outgoing',
    'exclude' => 0,
    'id' => 'quantity_2',
    'table' => 'mc_exchanges',
    'field' => 'quantity',
    'override' => array(
      'button' => 'Override',
    ),
    'relationship' => 'none',
  ),
  'quantity_3' => array(
    'label' => 'Mudar',
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
    'output_type' => 'sign',
    'exclude' => 0,
    'id' => 'quantity_3',
    'table' => 'mc_exchanges',
    'field' => 'quantity',
    'override' => array(
      'button' => 'Override',
    ),
    'relationship' => 'none',
  ),
  'running_balance' => array(
    'label' => 'Balanço',
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
    'id' => 'running_balance',
    'table' => 'mc_exchanges',
    'field' => 'running_balance',
    'override' => array(
      'button' => 'Remplacer',
    ),
    'relationship' => 'none',
  ),
  'participant' => array(
    'label' => 'Other user',
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
    'link_to_other' => 1,
    'exclude' => 0,
    'id' => 'participant',
    'table' => 'mc_exchanges',
    'field' => 'participant',
    'override' => array(
      'button' => 'Override',
    ),
    'relationship' => 'none',
  ),
));
$handler->override_option('arguments', array(
  'participant' => array(
    'default_action' => 'default',
    'style_plugin' => 'default_summary',
    'style_options' => array(),
    'wildcard' => 'all',
    'wildcard_substitution' => 'All',
    'title' => '',
    'breadcrumb' => '',
    'default_argument_type' => 'current_user',
    'default_argument' => '',
    'validate_type' => 'user',
    'validate_fail' => 'not found',
    'break_phrase' => 0,
    'not' => 0,
    'id' => 'participant',
    'table' => 'mc_exchanges',
    'field' => 'participant',
    'validate_user_argument_type' => 'uid',
    'validate_user_roles' => array(
      2 => 0,
    ),
    'override' => array(
      'button' => 'Override',
    ),
    'relationship' => 'none',
    'default_options_div_prefix' => '',
    'default_argument_fixed' => '',
    'default_argument_user' => 0,
    'default_argument_php' => '',
    'validate_argument_node_type' => array(
      'currency' => 0,
      'exchange' => 0,
      'offer' => 0,
      'want' => 0,
      'page' => 0,
      'profile' => 0,
      'story' => 0,
    ),
    'validate_argument_node_access' => 0,
    'validate_argument_nid_type' => 'nid',
    'validate_argument_vocabulary' => array(
      1 => 0,
    ),
    'validate_argument_type' => 'tid',
    'validate_argument_transform' => 0,
    'validate_user_restrict_roles' => 0,
    'validate_argument_php' => '',
  ),
));
$handler->override_option('filters', array(
  'type' => array(
    'operator' => 'in',
    'value' => array(
      'exchange' => 'exchange',
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
  'type' => 'perm',
  'perm' => 'view all exchanges',
));
$handler->override_option('cache', array(
  'type' => 'none',
));
$handler->override_option('title', 'Exchange list');
$handler->override_option('footer', '<a href="/exchange">Record exchange</a>');
$handler->override_option('footer_format', '1');
$handler->override_option('footer_empty', 1);
$handler->override_option('empty', 'There are no exchanges in the system yet');
$handler->override_option('empty_format', '1');
$handler->override_option('items_per_page', 25);
$handler->override_option('use_pager', 'mini');
$handler->override_option('distinct', 0);
$handler->override_option('style_plugin', 'table');
$handler->override_option('style_options', array(
  'grouping' => '',
  'override' => 1,
  'sticky' => 0,
  'order' => 'desc',
  'columns' => array(
    'created' => 'created',
    'title' => 'title',
    'payer_uid' => 'payer_uid',
    'payee_uid' => 'payee_uid',
    'quantity' => 'quantity',
    'quantity_1' => 'quantity_1',
    'quantity_2' => 'quantity_2',
    'quantity_3' => 'quantity_3',
    'participant' => 'participant',
  ),
  'info' => array(
    'created' => array(
      'sortable' => 1,
      'separator' => '',
    ),
    'title' => array(
      'sortable' => 0,
      'separator' => '',
    ),
    'payer_uid' => array(
      'sortable' => 1,
      'separator' => '',
    ),
    'payee_uid' => array(
      'sortable' => 1,
      'separator' => '',
    ),
    'quantity' => array(
      'sortable' => 1,
      'separator' => '',
    ),
    'quantity_1' => array(
      'sortable' => 1,
      'separator' => '',
    ),
    'quantity_2' => array(
      'sortable' => 1,
      'separator' => '',
    ),
    'quantity_3' => array(
      'sortable' => 0,
      'separator' => '',
    ),
    'participant' => array(
      'sortable' => 1,
      'separator' => '',
    ),
  ),
  'default' => 'changed',
));
$handler = $view->new_display('page', 'My exchanges', 'my_exchanges');
$handler->override_option('title', 'My exchanges');
$handler->override_option('header', 'Click the column headings to re-sort');
$handler->override_option('header_format', '1');
$handler->override_option('header_empty', 0);
$handler->override_option('path', 'user/%/exchanges');
$handler->override_option('menu', array(
  'type' => 'tab',
  'title' => 'Exchanges',
  'description' => 'Exchanges in which this user participated',
  'weight' => '3',
  'name' => 'navigation',
));
$handler->override_option('tab_options', array(
  'type' => 'normal',
  'title' => 'Exchanges',
  'description' => '',
  'weight' => '0',
  'name' => 'navigation',
));
$translatables['mc_exchanges_user'] = array(
  t('<a href="/exchange">Record exchange</a>'),
  t('Click the column headings to re-sort'),
  t('Defaults'),
  t('Exchange list'),
  t('My exchanges'),
  t('There are no exchanges in the system yet'),
);

  
  
  
  $views[$view->name] = $view;

  return $views;
}
