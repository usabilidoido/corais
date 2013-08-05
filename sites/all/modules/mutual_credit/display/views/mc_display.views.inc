<?php
//$Id: mc_display.views.inc,v 1.1.2.2 2010/12/22 20:21:51 matslats Exp $
/**
 * @file
 * Contains default views on behalf of the exchanges  module.
 */


function mc_display_views_handlers() {
  $handlers= array(
    'info' => array(
      'path' => drupal_get_path('module', 'mc_display') .'/views',
    ),
    'handlers' => array(
      'views_handler_field_mc_trader' => array(
        'parent' => 'views_handler_field'
      ),
      'views_handler_field_mc_currency' => array(
        'parent' => 'views_handler_field'
      ),
      'views_handler_field_mc_amount' => array(
        'parent' => 'views_handler_field'
      ),
      'views_handler_field_mc_running_balance' => array(
        'parent' => 'views_handler_field'
      ),
      'views_handler_field_other_participant' => array(
        'parent' => 'views_handler_field'
      ),
      'views_handler_filter_mc_exchange_types' => array(
        'parent' => 'views_handler_filter_many_to_one'
      ),
      'views_handler_filter_mc_currency' => array(
        'parent' => 'views_handler_filter_in_operator'
      ),
      'views_handler_argument_mc_participant' => array(
        'parent' => 'views_handler_argument_numeric'
      )
    ),
  );
  return $handlers;
}


function mc_display_views_data() {
  $data = array();
  $data['mc_exchanges']['table']['group']  = t('Exchanges');
  $data['mc_exchanges']['table']['base'] = array(
    'field' => 'nid',
    'title' => t('Exchange'),
    'help' => t('Money flows between users.'),
  );
  $data['mc_exchanges']['table']['join'] = array(
    'node' => array(
      'left_field' => 'nid',
      'field' => 'nid',
    ),
  );

  $data['mc_exchanges']['payer_uid'] = array(
    'title' => t('Payer'), // The item it appears as on the UI,
    'field' => array(
      'help' => t('The user id who spent currency in the exchange.'),
      'handler' => 'views_handler_field_mc_trader',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'help' => t('Show only exchanges where the specified user paid'),
      'handler' => 'views_handler_filter_user_name'
    ),
    'argument' => array(
      'help' => t('The user who gave currency in the exchange.'),
      'handler' => 'views_handler_argument'
    ),
  );
  $data['mc_exchanges']['payee_uid'] = array(
    'title' => t('Payee'),
    'field' => array(
      'help' => t('The user id who received currency in the exchange.'),
      'handler' => 'views_handler_field_mc_trader',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'help' => t('Show only exchanges where the specified user was paid.'),
      'handler' => 'views_handler_filter_user_name'
    ),
    'argument' => array(
      'help' => t('The user who received currency in the exchange'),
      'handler' => 'views_handler_argument'
    ),
  );
  $data['mc_exchanges']['quantity'] = array(
    'title' => t('Amount'), // The item it appears as on the UI,
    'help' => t('Number of units of currency transferred'),
    'field' => array(
      'handler' => 'views_handler_field_mc_amount',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'help' => t('Exchanges above or below a certain value'),
      'handler' => 'views_handler_filter_numeric'
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    )
  );
  $data['mc_exchanges']['state'] = array(
    'title' => t('Exchange state'),
    'help' => t('Whether the exchange is completed (FALSE) or pending (TRUE)'),
    //strictly speaking this isn't a boolean, but an enumerated value
    //it should really have its own handler to allow theming of the output
    'field' => array(
      'handler' => 'views_handler_field_boolean',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_boolean_operator',
    ),
  );
  $data['mc_exchanges']['cid'] = array(
    'title' => t('Currency ID'),
    'field' => array(
      'help' => t("Use this field (check 'exclude') for the theming to work on all quantities in the row."),
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'help' => t('The currency the exchange was denominated in'),
      'handler' => 'views_handler_filter_mc_currency',
    ),
    'argument' => array(
      'help' => t('The currency the exchange was denominated in'),
      'handler' => 'views_handler_argument_numeric',
    ),
  );

  $data['mc_exchanges']['exchange_type'] = array(
    'title' => t('Exchange type'),
    'help' => t('Form or module which created the exchange'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_mc_exchange_types',
    ),
  );

  $data['mc_exchanges']['participant'] = array(
    'argument' => array(
      'title' => t('Involved user'),
      'help' => t('Show only those exchanges in which a user was involved'),
      'handler' => 'views_handler_argument_mc_participant',
    ),
    'field' => array(
      'title' => t('Other user'),
      'help' => t("Show the other user who wasn't passed in the views argument"),
      'handler' => 'views_handler_field_other_participant',
      'click sortable' => TRUE 
    )
  );
  $data['mc_exchanges']['running_balance'] = array(
    'title' => t('Running balance'),
    'help' => t('Balance after the exchange, assuming that all previous exchanges were completed.') .' '.  t('(Expensive on the CPU)'),
    'field' => array(
      'handler' => 'views_handler_field_mc_running_balance',
    )
  );


  $data['mc_cache']['table']['group'] = t('Balances');
  $data['mc_cache']['table']['base'] = array(
    'field' => 'uid',
    'title' => t('User Balance'),
    'help' => t('Balances of users.'),
  );
  $data['mc_cache']['table']['join'] = array(
    'users' => array(
      'left_field' => 'uid',
      'field' => 'uid',
    ),
  );
  $data['mc_cache']['uid'] = array(
    'title' => t('User'),
    'argument' => array(
      'help' => t("The user id"),
      'handler' => 'views_handler_argument',
    )
  );
  $data['mc_cache']['cid'] = array(
    'title' => t('Currency ID'),
    'field' => array(
      'help' => t("Use this field (check 'exclude') for the theming to work on all quantities in the row."),
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'filter' => array(
       'help' => t('Filter the balances by currency ID'),
       'handler' => 'views_handler_filter_mc_currency'
    ),
  );
  $data['mc_cache']['cleared_balance'] = array(
    'title' => t('Balance'),
    'field' => array(
      'help' => t("The user's total cleared income minus total cleared expenditure.") .' '.  t('Will return one row for each currency.'),
      'handler' => 'views_handler_field_mc_amount',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'help' => t("The user's total cleared income minus total cleared expenditure."),
      'handler' => 'views_handler_sort',
    )
  );
  $data['mc_cache']['pending_dif'] = array(
    'title' => t('Pending Difference'),
    'field' => array(
      'help' => t("The sum of all pending exchanges, (@type)", array('@type'=>'')) .' '.  t('Will return one row for each currency.'),
      'handler' => 'views_handler_field_mc_amount',
      'click sortable' => TRUE,
    )
  );
  $data['mc_cache']['gross_in'] = array(
    'title' => t('Gross Income'),
    'help' => t("The user's total completed income."),
    'field' => array(
      'help' => t("The user's total completed income.") .' '.  t('Will return one row for each currency.'),
      'handler' => 'views_handler_field_mc_amount',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    )
  );
  $data['mc_cache']['gross_out'] = array(
    'title' => t('Gross Expenditure'),
    'help' => t("The user's total completed expenditure."),
    'field' => array(
      'help' => t("The user's total completed expenditure.") .' '.  t('Will return one row for each currency.'),
      'handler' => 'views_handler_field_mc_amount',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    )
  );
  $data['mc_cache']['count'] = array(
    'title' => t('Number of exchanges.'),
    'help' => ' ('. t('One result for per currency per user.') .')',
    'field' => array(
      'handler' => 'views_handler_field_mc_amount',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    )
  );
  return $data;
}


function mc_display_views_default_views() {
  $view = mc_exchanges_all_view();
  $views[$view->name] = $view;
  $view = mc_exchanges_user_view();
  $views[$view->name] = $view;
  //with only one view to get, this may as well be brought into one function
  return $views;
}

function mc_exchanges_all_view() {
  $view = new view;
  $view->name = 'mc_exchanges_all';
  $view->description = t('list of all exchanges');
  $view->tag = 'exchanges';
  $view->view_php = '';
  $view->base_table = 'node';
  $view->is_cacheable = FALSE;
  $view->api_version = 2;
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */
  $handler = $view->new_display('default', 'Defaults', 'default');
  $handler->override_option('fields', array(
    'changed' => array(
      'label' => t('Date'),
      'date_format' => 'custom',
      'custom_date_format' => 'd-m-y',
      'exclude' => 0,
      'id' => 'changed',
      'table' => 'node',
      'field' => 'changed',
      'relationship' => 'none',
    ),
    'title' => array(
      'label' => t('Description'),
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
      'label' => t('Payer'),
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
      'label' => t('Payee'),
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
      'label' => t('Amount'),
      'exclude' => 0,
      'id' => 'quantity',
      'table' => 'mc_exchanges',
      'field' => 'quantity',
      'relationship' => 'none',
    ),
  ));
  $handler->override_option('arguments', array(
    'cid' => array(
      'default_action' => 'ignore',
      'style_plugin' => 'default_summary',
      'style_options' => array(),
      'wildcard' => 'all',
      'wildcard_substitution' => 'All',
      'title' => '',
      'breadcrumb' => '',
      'default_argument_type' => 'fixed',
      'default_argument' => '',
      'validate_type' => 'node',
      'validate_fail' => 'not found',
      'break_phrase' => 0,
      'not' => 0,
      'id' => 'cid',
      'table' => 'mc_exchanges',
      'field' => 'cid',
      'relationship' => 'none',
      'default_options_div_prefix' => '',
      'default_argument_user' => 0,
      'default_argument_fixed' => '',
      'default_argument_php' => '',
      'validate_argument_node_type' => array(
        'currency' => 'currency',
        'exchange' => 0,
        'page' => 0,
        'story' => 0,
      ),
      'validate_argument_node_access' => 0,
      'validate_argument_nid_type' => 'nids',
      'validate_argument_vocabulary' => array(),
      'validate_argument_type' => 'tid',
      'user_argument_type' => 'uid',
      'restrict_user_roles' => 0,
      'user_roles' => array(),
      'validate_argument_php' => '',
      'validate_user_argument_type' => 'uid',
      'validate_user_roles' => array(
        '2' => 0,
      ),
      'validate_argument_transform' => 0,
      'validate_user_restrict_roles' => 0,
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
  ));
  $handler->override_option('access', array(
    'type' => 'perm',
    'perm' => 'view all exchanges',
  ));
  $handler->override_option('cache', array(
    'type' => 'none',
  ));
  $handler->override_option('title', t('Exchange list'));
  $handler->override_option('footer_format', '1');
  $handler->override_option('empty', t('There are no exchanges in the system yet'));
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
  $handler = $view->new_display('page', t('Member exchanges'), 'page_1');
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
  ));
  $handler->override_option('header', t('all the exchanges on the system, except the mass-exchanges, and those involving admin'));
  $handler->override_option('header_format', '1');
  $handler->override_option('header_empty', 0);
  $handler->override_option('path', 'exchanges/list');
  $handler->override_option('menu', array(
    'type' => 'default tab',
    'title' => 'All exchanges',
    'description' => t('All member-to-member exchanges'),
    'weight' => '20',
    'name' => 'navigation',
  ));
  $handler->override_option('tab_options', array(
    'type' => 'normal',
    'title' => t('Exchanges'),
    'description' => '',
    'weight' => '0',
    'name' => 'navigation',
  ));
  $handler = $view->new_display('page', t('Filter exchanges'), 'page_2');
  $types = module_invoke_all('exchange_type_info');
  foreach ($types as $type => $info) {
    $options[$type] = $type;
  }
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
      'value' => $options,
      'group' => '0',
      'exposed' => TRUE,
      'expose' => array(
        'use_operator' => 0,
        'operator' => 'exchange_type_op',
        'identifier' => 'exchange_type',
        'label' => t('Exchange type'),
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
        'label' => t('Payee'),
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
        'label' => t('Payer'),
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
  $handler->override_option('header', t('Use the widgets to filter through all the exchanges ever listed, and then sort the columns by clicking the column headings'));
  $handler->override_option('header_format', '1');
  $handler->override_option('header_empty', 0);
  $handler->override_option('empty', t('No exchanges meet these criteria. Try using fewer filters'));
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
  $handler = $view->new_display('block', t('who gave to whom'), 'block_1');
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
        'text' => t('[payer_uid] gave [quantity] to [payee_uid]'),
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
  $handler = $view->new_display('block', t('exchanging what?'), 'block_2');
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
  $handler->override_option('access', array(
    'type' => 'none',
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

  return $view;
}

function mc_exchanges_user_view() {
  $view = new view;
  $view->name = 'mc_exchanges_user';
  $view->description = t('exchanges of one user');
  $view->tag = 'exchanges';
  $view->view_php = '';
  $view->base_table = 'node';
  $view->is_cacheable = FALSE;
  $view->api_version = 2;
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */
  $handler = $view->new_display('default', 'Defaults', 'default');
  $handler->override_option('fields', array(
    'created' => array(
      'label' => t('Date'),
      'date_format' => 'custom',
      'custom_date_format' => 'd-m-y',
      'exclude' => 0,
      'id' => 'created',
      'table' => 'node',
      'field' => 'created',
      'relationship' => 'none',
    ),
    'title' => array(
      'label' => t('Description'),
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
      'label' => t('Payer'),
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
      'label' => t('Payee'),
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
      'label' => t('Amount'),
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
      'label' => t('incoming'),
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
      'label' => t('outgoing'),
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
      'label' => t('Change'),
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
      'label' => t('Balance'),
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
      'label' => t('Other user'),
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
        '2' => 0,
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
        '1' => 0,
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
  $handler->override_option('title', t('Exchange list'));
  $handler->override_option('footer', l(t('Record exchange'), 'exchange'));
  $handler->override_option('footer_format', '1');
  $handler->override_option('footer_empty', 1);
  $handler->override_option('empty', t('There are no exchanges in the system yet'));
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
  $handler->override_option('title', t('My exchanges'));
  $handler->override_option('header', t('Click the column headings to re-sort'));
  $handler->override_option('header_format', '1');
  $handler->override_option('header_empty', 0);
  $handler->override_option('path', 'user/%/exchanges');
  $handler->override_option('menu', array(
    'type' => 'tab',
    'title' => t('Exchanges'),
    'description' => t('Exchanges in which this user participated'),
    'weight' => '3',
    'name' => 'navigation',
  ));
  $handler->override_option('tab_options', array(
    'type' => 'normal',
    'title' => t('Exchanges'),
    'description' => '',
    'weight' => '0',
    'name' => 'navigation',
  ));

  return $view;
}
//shorten the user select text boxes on the exposed views form
//this could also be done by css but I'm not creating a file just for that
function mc_display_form_views_exposed_form_alter(&$form, $form_state) {
  $form['payee_uid']['#size'] = 30;
  $form['payer_uid']['#size'] = 30;
}
