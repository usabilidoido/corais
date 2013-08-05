<?php
//$Id: 
/**
 * @file
 * Contains default views to supplement the offers/wants modules
 */

function offers_wants_views_handlers() {
  $handlers= array(
    'info' => array(
      'path' => drupal_get_path('module', 'offers_wants') .'/views',
    ),
    'handlers' => array(
      'views_handler_field_ow_want' => array(
        'parent' => 'views_handler_field_boolean'
      )
    )
  );
  return $handlers;
}

/**
 * Implementation of hook views_data().
 */
function offers_wants_views_data() {
  $data = array();
  $data['offers_wants']['table']['group']  = t('Propositions');
  $data['offers_wants']['table']['join'] = array(
    'node' => array(
      'left_field' => 'nid',
      'field' => 'nid',
    ),
  );
  $data['offers_wants']['expires'] = array(
    'title' => t('Expires'), // The item it appears as on the UI,
    'help' => t('Date that the proposition is schedules to be unpublished'),
    'field' => array(
      'handler' => 'views_handler_field_date',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_date'
    ),
    'sort' => array(
      'handler' => 'views_handler_sort_date'
    ),
  );
  $data['offers_wants']['want'] = array(
    'title' => t('Is a want'),
    'help' => t('Show a 1'),
    'field' => array(
      'handler' => 'views_handler_field_ow_want',
    ),
    'filter' => array(
      'help' => t('Filter by proposition direction'),
      'handler' => 'views_handler_filter_boolean_operator',
    )
  );
  $data['offers_wants']['end'] = array(
    'title' => t('Is time limited'),
    'help' => t('Show only temporary or permanent propositions'),
    'filter' => array(
      'handler' => 'views_handler_filter_boolean_operator',
    )
  );
  return $data;
}

/**
 * Implementation of hook views_default_views().
 */
function offers_wants_views_default_views() {
  
  $view = _ow_user_view();
  $views[$view->name] = $view;

  $view = _ow_offers_wants_view(0, 'offers', t('Offer'), t('Offers'));
  $views[$view->name] = $view;
  if (variable_get('offers_wants_proposition_wants', TRUE)) {
    $view = _ow_offers_wants_view(1, 'wants', t('Want'), t('Wants'));
    $views[$view->name] = $view;
  }
  if (variable_get('offers_wants_expiry', TRUE)) {
    $view = _ow_expiry_view();
    $views[$view->name] = $view;
  }
  return $views;
}

/*
 * implements hook_views_default_views_alter
 * selectively remove some bits of views according to settings.
 * Assumes complete symmetry between offers and wants
 */
function offers_wants_views_default_views_alter(&$views) {
  //expiry date field (no filter yet)
  if (!variable_get('offers_wants_expiry', TRUE)) {
    foreach (array('offers', 'wants') as $propname) {
      foreach ($views[$propname]->display as $key => $display) {
        //fields
        if (isset($views[$propname]->display[$key]->display_options['fields']['expires'])) {
          unset($views[$propname]->display[$key]->display_options['fields']['expires']);
        }
        //sorts
        if (isset($views[$propname]->display[$key]->display_options['sorts']['expires'])) {
          unset($views[$propname]->display[$key]->display_options['sorts']['expires']);
        }
      }
    }
    //do the same on the user view.
    unset($views['offers_wants_user']->display['default']->display_options['fields']['expires']);
    unset($views['offers_wants_user']->display['default']->display_options['sorts']['expires']);
  }

  if (!variable_get('offers_wants_proposition_wants', TRUE)) {
    //we'll ignore that the table is grouped by this field. Views can deal with it.
    unset($views['offers_wants_user']->display['default']->display_options['fields']['want']);
  }

}

function _ow_offers_wants_view($want, $propnameplural, $proptitle, $proptitleplural) {

  $tree = taxonomy_get_tree(variable_get('offers_wants_type_vid', 0));
  $types = array();
  if (count($tree) > 1) {
    foreach ($tree as $term) {
      $types[$term->tid] = $term->name;
    }
  }

  $view = new view;
  $view->name = $propnameplural;
  $view->description = t('@prop directory', array('@prop' => $proptitle));
  $view->tag = 'offers/wants';
  $view->view_php = '';
  $view->base_table = 'node';
  $view->is_cacheable = FALSE;
  $view->api_version = 2;
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */
  $handler = $view->new_display('default', 'Defaults', 'default');
  $handler->override_option('fields', array(
    'title' => array(
      'label' => t('Title, click for more'),
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
    'name' => array(
      'label' => t('Member'),
      'link_to_user' => 1,
      'exclude' => 0,
      'id' => 'name',
      'table' => 'users',
      'field' => 'name',
      'relationship' => 'none',
    ),
    'value' => array(
      'label' => t('Locality'),
      'empty' => '',
      'hide_empty' => 0,
      'empty_zero' => 0,
      'link_to_user' => 0,
      'exclude' => 0,
      'id' => 'value',
      'table' => 'profile_values_profile_locality',
      'field' => 'value',
      'relationship' => 'none',
    ),
    'tid' => array(
      'label' => t('Type'),
      'empty' => '',
      'hide_empty' => 0,
      'empty_zero' => 0,
      'type' => 'separator',
      'separator' => ', ',
      'link_to_taxonomy' => 1,
      'limit' => 1,
      'vids' => array(
        variable_get('offers_wants_type_vid', 0) => variable_get('offers_wants_type_vid', 0),
      ),
      'exclude' => 0,
      'id' => 'tid',
      'table' => 'term_node',
      'field' => 'tid',
      'relationship' => 'none',
    ),
    'expires' => array(
      'label' => t('Expires'),
      'empty' => '',
      'hide_empty' => 1,
      'empty_zero' => 0,
      'date_format' => 'custom',
      'custom_date_format' => 'd/m/y',
      'exclude' => 0,
      'id' => 'expires',
      'table' => 'offers_wants',
      'field' => 'expires',
      'override' => array(
        'button' => 'Override',
      ),
      'relationship' => 'none',
    ),
  ));
  $handler->override_option('sorts', array(
    'changed' => array(
      'order' => 'DESC',
      'granularity' => 'hour',
      'id' => 'changed',
      'table' => 'node',
      'field' => 'changed',
      'relationship' => 'none',
    ),
  ));
  $handler->override_option('arguments', array(
    'tid' => array(
      'default_action' => 'ignore',
      'style_plugin' => 'default_summary',
      'style_options' => array(),
      'wildcard' => 'all',
      'wildcard_substitution' => 'Tous / Toutes',
      'title' => 'Offer: %1',
      'breadcrumb' => '',
      'default_argument_type' => 'fixed',
      'default_argument' => '',
      'validate_type' => 'taxonomy_term',
      'validate_fail' => 'not found',
      'break_phrase' => 0,
      'add_table' => 0,
      'require_value' => 0,
      'reduce_duplicates' => 0,
      'set_breadcrumb' => 0,
      'id' => 'tid',
      'table' => 'term_node',
      'field' => 'tid',
      'validate_user_argument_type' => 'uid',
      'relationship' => 'none',
      'default_options_div_prefix' => '',
      'default_argument_fixed' => '',
      'default_argument_user' => 0,
      'default_argument_image_size' => '_original',
      'default_argument_php' => '',
      'validate_argument_node_access' => 0,
      'validate_argument_nid_type' => 'nid',
      'validate_argument_vocabulary' => array(
        variable_get('offers_wants_type_vid', 0) => variable_get('offers_wants_type_vid', 0),
        variable_get('offers_wants_cat_vid', 0) => variable_get('offers_wants_cat_vid', 0),
      ),
      'validate_argument_type' => 'tid',
      'validate_argument_transform' => 0,
      'validate_user_restrict_roles' => 0,
      'validate_argument_php' => '',
      'override' => array(
        'button' => 'Override',
      ),
      'image_size' => array(
        '_original' => '_original',
        'thumbnail' => 'thumbnail',
        'preview' => 'preview',
      ),
    ),
  ));
  $handler->override_option('filters', array(
    'type' => array(
      'operator' => 'in',
      'value' => array(
        'proposition' => 'proposition',
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
    'want' => array(
      'operator' => '=',
      'value' => $want,
      'group' => '0',
      'exposed' => FALSE,
      'expose' => array(
        'operator' => FALSE,
        'label' => '',
      ),
      'id' => 'want',
      'table' => 'offers_wants',
      'field' => 'want',
      'relationship' => 'none',
    ),
    'status_1' => array(
      'operator' => '=',
      'value' => '1',
      'group' => '0',
      'exposed' => FALSE,
      'expose' => array(
        'operator' => FALSE,
        'label' => '',
      ),
      'id' => 'status_1',
      'table' => 'node',
      'field' => 'status',
      'relationship' => 'none',
    ),
  ));
  $handler->override_option('access', array(
    'type' => 'perm',
    'perm' => 'view propositions',
  ));
  $handler->override_option('cache', array(
    'type' => 'time',
    'results_lifespan' => '120',
    'output_lifespan' => '120',
  ));
  $handler->override_option('css_class', $propnameplural);
  $handler->override_option('title', t('@prop directory', array('@prop' => $proptitle)));
  $handler->override_option('header_empty', 1);
  $handler->override_option('footer_format', '1');
  $handler->override_option('footer_empty', 1);
  $handler->override_option('empty', t('There are no propositions in this category'));
  $handler->override_option('empty_format', '1');
  $handler->override_option('items_per_page', 25);
  $handler->override_option('use_pager', 'full');
  $handler->override_option('use_more', 1);
  $handler->override_option('distinct', 1);
  $handler->override_option('style_plugin', 'table');
  $handler->override_option('style_options', array(
    'grouping' => '',
    'override' => 1,
    'sticky' => 0,
    'order' => 'asc',
    'columns' => array(
      'title' => 'title',
      'name' => 'name',
      'value' => 'value',
    ),
    'info' => array(
      'title' => array(
        'sortable' => 0,
        'separator' => '',
      ),
      'name' => array(
        'sortable' => 1,
        'separator' => '',
      ),
      'value' => array(
        'sortable' => 1,
        'separator' => '',
      ),
    ),
    'default' => '-1',
  ));
  $handler = $view->new_display('page', 'All', 'all');
  $handler->override_option('title', $proptitleplural);
  $handler->override_option('path', $propnameplural.'/all');
  $handler->override_option('menu', array(
    'type' => 'default tab',
    'title' => t('All'),
    'description' => '',
    'weight' => '0',
    'name' => 'navigation',
  ));
  $handler->override_option('tab_options', array(
    'type' => 'normal',
    'title' => $proptitleplural,
    'description' => '',
    'weight' => '-6',
    'name' => 'primary-links',
  ));


  $handler = $view->new_display('block', 'latest', 'latest');
  $handler->override_option('fields', array(
    'name' => array(
      'label' => '',
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
      'relationship' => 'none',
    ),
    'title' => array(
      'label' => '',
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
  ));
  $handler->override_option('title', t('Latest @types', array('@types' => $proptitleplural)));
  $handler->override_option('items_per_page', 5);
  $handler->override_option('style_plugin', 'list');
  $handler->override_option('style_options', array(
    'grouping' => '',
    'type' => 'ul',
  ));
  $handler->override_option('row_options', array(
    'inline' => array(
      'name' => 'name',
      'title' => 'title',
    ),
    'separator' => ': ',
    'hide_empty' => 0,
  ));
  $handler->override_option('block_description', t('Latest @proptitle', array('@proptitle' => $proptitleplural)));
  $handler->override_option('block_caching', -1);


  $handler = $view->new_display('feed', 'Flux', 'feed_1');
  $handler->override_option('style_plugin', 'rss');
  $handler->override_option('style_options', array(
    'mission_description' => FALSE,
    'description' => '',
  ));
  $handler->override_option('row_plugin', 'node_rss');
  $handler->override_option('path', $propnameplural.'.xml');
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
  $handler->override_option('displays', array(
    'all' => 'all',
  ));
  $handler->override_option('sitename_title', FALSE);


  $handler = $view->new_display('feed', 'CSV', 'feed_2');
  $handler->override_option('fields', array(
    'uid' => array(
      'label' => 'uid',
      'empty' => '',
      'hide_empty' => 0,
      'empty_zero' => 0,
      'link_to_user' => 0,
      'exclude' => 0,
      'id' => 'uid',
      'table' => 'users',
      'field' => 'uid',
      'relationship' => 'none',
    ),
    'title' => array(
      'label' => '',
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
    'name' => array(
      'label' => 'username',
      'link_to_user' => 0,
      'exclude' => 0,
      'id' => 'name',
      'table' => 'users',
      'field' => 'name',
      'relationship' => 'none',
    ),
    'field_datetime_value' => array(
      'label' => 'expiry',
      'empty' => '',
      'hide_empty' => 0,
      'empty_zero' => 0,
      'link_to_node' => 0,
      'label_type' => 'widget',
      'format' => 'default',
      'multiple' => array(
        'multiple_number' => '',
        'multiple_from' => '',
        'multiple_to' => '',
        'group' => '',
      ),
      'repeat' => array(
        'show_repeat_rule' => '',
      ),
      'fromto' => array(
        'fromto' => 'value',
      ),
      'exclude' => 0,
      'id' => 'field_datetime_value',
      'table' => 'node_data_field_datetime',
      'field' => 'field_datetime_value',
      'relationship' => 'none',
    ),
    'nid' => array(
      'label' => 'nid',
      'empty' => '',
      'hide_empty' => 0,
      'empty_zero' => 0,
      'link_to_node' => 0,
      'exclude' => 0,
      'id' => 'nid',
      'table' => 'node',
      'field' => 'nid',
      'relationship' => 'none',
    ),
    'cat_tid' => array(
      'label' => t('Category'),
      'empty' => '',
      'hide_empty' => 0,
      'empty_zero' => 0,
      'type' => 'separator',
      'separator' => ', ',
      'link_to_taxonomy' => 1,
      'limit' => 1,
      'vids' => array(
        variable_get('offers_wants_cat_vid', 0) => variable_get('offers_wants_cat_vid', 0),
      ),
      'exclude' => 0,
      'id' => 'tid',
      'table' => 'term_node',
      'field' => 'tid',
      'relationship' => 'none',
    ),
    'type_tid_' => array(
      'label' => t('Type'),
      'empty' => '',
      'hide_empty' => 0,
      'empty_zero' => 0,
      'type' => 'separator',
      'separator' => ', ',
      'link_to_taxonomy' => 1,
      'limit' => 1,
      'vids' => array(
        variable_get('offers_wants_type_vid', 0) => variable_get('offers_wants_type_vid', 0),
      ),
      'exclude' => 0,
      'id' => 'tid',
      'table' => 'term_node',
      'field' => 'tid',
      'relationship' => 'none',
    ),
    'expires' => array(
      'label' => t('Expires'),
      'empty' => '',
      'hide_empty' => 1,
      'empty_zero' => 0,
      'date_format' => 'custom',
      'custom_date_format' => 'd/m/y',
      'exclude' => 0,
      'id' => 'expires',
      'table' => 'offers_wants',
      'field' => 'expires',
      'override' => array(
        'button' => 'Override',
      ),
      'relationship' => 'none',
    ),
  ));
  $handler->override_option('sorts', array(
    'nid' => array(
      'order' => 'ASC',
      'id' => 'nid',
      'table' => 'node',
      'field' => 'nid',
      'relationship' => 'none',
    ),
  ));
  $handler->override_option('access', array(
    'type' => 'perm',
    'perm' => 'access user profiles',
  ));
  $handler->override_option('items_per_page', 0);
  $handler->override_option('style_plugin', 'views_csv');
  $handler->override_option('style_options', array(
    'mission_description' => FALSE,
    'description' => '',
    'attach_text' => $proptitleplural,
    'provide_file' => 0,
    'filename' => $propnameplural.'_'.variable_get('offers_wants_type_vid', 0).'.csv',
    'parent_sort' => 0,
    'seperator' => ',',
    'quote' => 1,
    'trim' => 0,
    'header' => 1,
  ));
  $handler->override_option('row_plugin', '');
  $handler->override_option('path', $propnameplural.'_'.variable_get('offers_wants_type_vid', 0).'.csv');
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
  $handler->override_option('displays', array(
    'all' => 'all',
  ));
  $handler->override_option('sitename_title', FALSE);

  $handler = $view->new_display('print-table', 'Print-table', 'print-table');
  $handler->override_option('fields', array(
    'body' => array(
      'label' => '',
      'alter' => array(
        'strip_tags' => 1,
      ),
      'empty' => '',
      'hide_empty' => 0,
      'empty_zero' => 0,
      'exclude' => 1,
      'id' => 'body',
      'table' => 'node_revisions',
      'field' => 'body',
      'relationship' => 'none',
    ),
    'title' => array(
      'label' => t('Description'),
      'alter' => array(
        'alter_text' => 1,
        'text' => '[title]  <em>[body]</em>',
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
    ),
    'uid' => array(
      'label' => '',
      'empty' => '',
      'hide_empty' => 0,
      'empty_zero' => 0,
      'link_to_user' => 0,
      'exclude' => 1,
      'id' => 'uid',
      'table' => 'users',
      'field' => 'uid',
      'relationship' => 'none',
    ),
    'name_1' => array(
      'label' => 'Member',
      'empty' => '',
      'hide_empty' => 0,
      'empty_zero' => 0,
      'hide_alter_empty' => 0,
      'link_to_user' => 0,
      'overwrite_anonymous' => 1,
      'anonymous_text' => 'Anon',
      'exclude' => 0,
      'id' => 'name_1',
      'table' => 'users',
      'field' => 'name',
      'relationship' => 'none',
    ),
    'value_3' => array(
      'label' => t('Telephone'),
      'empty' => '',
      'hide_empty' => 0,
      'empty_zero' => 0,
      'link_to_user' => 0,
      'exclude' => 0,
      'id' => 'value_3',
      'table' => 'profile_values_profile_phone1',
      'field' => 'value',
      'relationship' => 'none',
    ),
    'value' => array(
      'label' => t('Locality'),
      'empty' => '',
      'hide_empty' => 0,
      'empty_zero' => 0,
      'link_to_user' => 0,
      'exclude' => 0,
      'id' => 'value',
      'table' => 'profile_values_profile_locality',
      'field' => 'value',
      'relationship' => 'none',
    ),
    'expires' => array(
      'label' => t('Expires'),
      'empty' => '',
      'hide_empty' => 1,
      'empty_zero' => 0,
      'date_format' => 'custom',
      'custom_date_format' => 'd/m/y',
      'exclude' => 0,
      'id' => 'expires',
      'table' => 'offers_wants',
      'field' => 'expires',
      'relationship' => 'none',
    ),
    'name' => array(
      'label' => '',
      'empty' => '',
      'hide_empty' => 0,
      'empty_zero' => 0,
      'hide_alter_empty' => 1,
      'link_to_taxonomy' => 0,
      'exclude' => 1,
      'id' => 'name',
      'table' => 'term_data',
      'field' => 'name',
      'relationship' => 'none',
    ),
  ));
  $handler->override_option('sorts', array(
    'weight' => array(
      'order' => 'ASC',
      'id' => 'weight',
      'table' => 'term_data',
      'field' => 'weight',
      'relationship' => 'none',
    ),
    'changed' => array(
      'order' => 'DESC',
      'granularity' => 'hour',
      'id' => 'changed',
      'table' => 'node',
      'field' => 'changed',
      'relationship' => 'none',
    ),
  ));
  $handler->override_option('filters', array(
    'type' => array(
      'operator' => 'in',
      'value' => array(
        'proposition' => 'proposition',
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
    'want' => array(
      'operator' => '=',
      'value' => 0,
      'group' => '0',
      'exposed' => FALSE,
      'expose' => array(
        'operator' => FALSE,
        'label' => '',
      ),
      'id' => 'want',
      'table' => 'offers_wants',
      'field' => 'want',
      'relationship' => 'none',
    ),
    'status_1' => array(
      'operator' => '=',
      'value' => '1',
      'group' => '0',
      'exposed' => FALSE,
      'expose' => array(
        'operator' => FALSE,
        'label' => '',
      ),
      'id' => 'status_1',
      'table' => 'node',
      'field' => 'status',
      'relationship' => 'none',
    ),
    'vid' => array(
      'operator' => 'in',
      'value' => array(
        1 => '1',
      ),
      'group' => '0',
      'exposed' => FALSE,
      'expose' => array(
        'operator' => FALSE,
        'label' => '',
      ),
      'id' => 'vid',
      'table' => 'term_data',
      'field' => 'vid',
      'relationship' => 'none',
    ),
  ));
  $handler->override_option('access', array(
    'type' => 'perm',
    'perm' => 'access user profiles',
  ));
  $handler->override_option('items_per_page', 0);
  $handler->override_option('distinct', 0);
  $handler->override_option('style_options', array(
    'grouping' => 'name',
    'override' => 1,
    'sticky' => 0,
    'order' => 'asc',
    'summary' => '',
    'columns' => array(
      'body' => 'body',
      'title' => 'title',
      'uid' => 'uid',
      'name_1' => 'name_1',
      'value_3' => 'value_3',
      'value' => 'value',
      'expires' => 'expires',
      'name' => 'name',
    ),
    'info' => array(
      'body' => array(
        'separator' => '',
      ),
      'title' => array(
        'sortable' => 0,
        'separator' => '',
      ),
      'uid' => array(
        'sortable' => 0,
        'separator' => '',
      ),
      'name_1' => array(
        'sortable' => 0,
        'separator' => '',
      ),
      'value_3' => array(
        'sortable' => 0,
        'separator' => '',
      ),
      'value' => array(
        'sortable' => 0,
        'separator' => '',
      ),
      'expires' => array(
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
  $handler->override_option('path', $propnameplural.'/print');
  $handler->override_option('attachment_position', 'after');
  $handler->override_option('inherit_arguments', TRUE);
  $handler->override_option('displays', array(
    'all' => 'all',
  ));

  $handler = $view->new_display('print-table', 'Print category', 'print-table-cat');
  $handler->override_option('fields', array(
    'body' => array(
      'label' => '',
      'alter' => array(
        'strip_tags' => 1,
      ),
      'empty' => '',
      'hide_empty' => 0,
      'empty_zero' => 0,
      'exclude' => 1,
      'id' => 'body',
      'table' => 'node_revisions',
      'field' => 'body',
      'relationship' => 'none',
    ),
    'title' => array(
      'label' => t('Description'),
      'alter' => array(
        'alter_text' => 1,
        'text' => '[title]  <em>[body]</em>',
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
    ),
    'uid' => array(
      'label' => '',
      'empty' => '',
      'hide_empty' => 0,
      'empty_zero' => 0,
      'link_to_user' => 0,
      'exclude' => 1,
      'id' => 'uid',
      'table' => 'users',
      'field' => 'uid',
      'relationship' => 'none',
    ),
    'value_1' => array(
      'label' => '',
      'empty' => '',
      'hide_empty' => 0,
      'empty_zero' => 0,
      'link_to_user' => 0,
      'exclude' => 1,
      'id' => 'value_1',
      'table' => 'profile_values_profile_firstname',
      'field' => 'value',
      'relationship' => 'none',
    ),
    'value_2' => array(
      'label' => t('Member'),
      'alter' => array(
        'alter_text' => 1,
        'text' => '[value_1] [value_2] ([uid])',
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
      'link_to_user' => 0,
      'exclude' => 0,
      'id' => 'value_2',
      'table' => 'profile_values_profile_familyname',
      'field' => 'value',
      'relationship' => 'none',
    ),
    'value_3' => array(
      'label' => t('Telephone'),
      'empty' => '',
      'hide_empty' => 0,
      'empty_zero' => 0,
      'link_to_user' => 0,
      'exclude' => 0,
      'id' => 'value_3',
      'table' => 'profile_values_profile_phone1',
      'field' => 'value',
      'override' => array(
        'button' => 'Utiliser la valeur par défaut',
      ),
      'relationship' => 'none',
    ),
    'value' => array(
      'label' => t('Locality'),
      'empty' => '',
      'hide_empty' => 0,
      'empty_zero' => 0,
      'link_to_user' => 0,
      'exclude' => 0,
      'id' => 'value',
      'table' => 'profile_values_profile_locality',
      'field' => 'value',
      'relationship' => 'none',
    ),
    'expires' => array(
      'label' => t('Expires'),
      'empty' => '',
      'hide_empty' => 1,
      'empty_zero' => 0,
      'date_format' => 'custom',
      'custom_date_format' => 'd/m/y',
      'exclude' => 0,
      'id' => 'expires',
      'table' => 'offers_wants',
      'field' => 'expires',
      'override' => array(
        'button' => 'Override',
      ),
      'relationship' => 'none',
    ),
  ));
  $handler->override_option('access', array(
    'type' => 'perm',
    'perm' => 'access user profiles',
  ));
  $handler->override_option('items_per_page', 0);
  $handler->override_option('distinct', 0);
  $handler->override_option('style_options', array(
    'grouping' => '',
    'override' => 0,
    'sticky' => 0,
    'order' => 'asc',
    'columns' => array(
      'body' => 'body',
      'title' => 'title',
      'uid' => 'uid',
      'value_1' => 'value_1',
      'value_2' => 'value_2',
      'value_3' => 'value_3',
      'value' => 'value',
    ),
    'info' => array(
      'body' => array(
        'separator' => '',
      ),
      'title' => array(
        'sortable' => 0,
        'separator' => '',
      ),
      'uid' => array(
        'sortable' => 0,
        'separator' => '',
      ),
      'value_1' => array(
        'sortable' => 0,
        'separator' => '',
      ),
      'value_2' => array(
        'sortable' => 0,
        'separator' => '',
      ),
      'value_3' => array(
        'sortable' => 0,
        'separator' => '',
      ),
      'value' => array(
        'sortable' => 0,
        'separator' => '',
      ),
    ),
    'default' => '-1',
  ));
  $handler->override_option('path', $propnameplural.'/print/'.variable_get('offers_wants_type_vid', 0));
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
  $handler->override_option('attachment_position', 'after');
  $handler->override_option('inherit_arguments', TRUE);
  $handler->override_option('displays', drupal_map_assoc($types));
  

  //Finally one tab for each term in the propositionType vocab, but only if there is more than one term
  foreach ($types as $tid => $name) {
    $handler = $view->new_display('page', $name, $name);
    $handler->override_option('fields', array(
      'title' => array(
        'label' => t('Title, click for details'),
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
      'name' => array(
        'label' => t('Member'),
        'link_to_user' => 1,
        'exclude' => 0,
        'id' => 'name',
        'table' => 'users',
        'field' => 'name',
        'relationship' => 'none',
      ),
      'value' => array(
        'label' => t('Locality'),
        'empty' => '',
        'hide_empty' => 0,
        'empty_zero' => 0,
        'link_to_user' => 0,
        'exclude' => 0,
        'id' => 'value',
        'table' => 'profile_values_profile_locality',
        'field' => 'value',
        'relationship' => 'none',
      ),
      'expires' => array(
        'label' => t('Expires'),
        'empty' => '',
        'hide_empty' => 0,
        'empty_zero' => 1,
        'date_format' => 'custom',
        'custom_date_format' => 'd/m/y',
        'exclude' => 0,
        'id' => 'expires',
        'table' => 'offers_wants',
        'field' => 'expires',
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
          'proposition' => 'proposition',
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
      'want' => array(
        'operator' => '=',
        'value' => $want,
        'group' => '0',
        'exposed' => FALSE,
        'expose' => array(
          'operator' => FALSE,
          'label' => '',
        ),
        'id' => 'want',
        'table' => 'offers_wants',
        'field' => 'want',
        'relationship' => 'none',
      ),
      'status_1' => array(
        'operator' => '=',
        'value' => '1',
        'group' => '0',
        'exposed' => FALSE,
        'expose' => array(
          'operator' => FALSE,
          'label' => '',
        ),
        'id' => 'status_1',
        'table' => 'node',
        'field' => 'status',
        'relationship' => 'none',
      ),
      'tid' => array(
        'operator' => 'or',
        'value' => array(
          $tid => $tid,
        ),
        'group' => '0',
        'exposed' => FALSE,
        'expose' => array(
          'operator' => FALSE,
          'label' => '',
        ),
        'type' => 'select',
        'limit' => TRUE,
        'vid' => '45',
        'id' => 'tid',
        'table' => 'term_node',
        'field' => 'tid',
        'hierarchy' => 0,
        'override' => array(
          'button' => 'Use default',
        ),
        'relationship' => 'none',
        'reduce_duplicates' => 0,
      ),
    ));
    $handler->override_option('access', array(
      'type' => 'perm',
      'perm' => 'view propositions',
    ));
    $handler->override_option('title', $name);
    $handler->override_option('path', $propnameplural.'/'.$tid);
    $handler->override_option('menu', array(
      'type' => 'tab',
      'title' => $name,
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
  }
  return $view;
}


function _ow_user_view(){
  $view = new view;
  $view->name = 'offers_wants_user';
  $view->description = 'offers/wants from a given user';
  $view->tag = 'offers/wants';
  $view->view_php = '';
  $view->base_table = 'node';
  $view->is_cacheable = FALSE;
  $view->api_version = 2;
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */
  $handler = $view->new_display('default', 'default', 'default');
  $handler->override_option('fields', array(
    'title' => array(
      'label' => t('Description'),
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
    'cat_tid' => array(
      'label' => t('Category'),
      'empty' => '',
      'hide_empty' => 0,
      'empty_zero' => 0,
      'type' => 'ul',
      'separator' => ', ',
      'link_to_taxonomy' => 1,
      'limit' => 1,
      'vids' => array(variable_get('offers_wants_cat_vid', 0) => variable_get('offers_wants_cat_vid', 0)),
      'exclude' => 0,
      'id' => 'tid',
      'table' => 'term_node',
      'field' => 'tid',
      'relationship' => 'none',
    ),
    'type_tid_' => array(
      'label' => t('Type'),
      'empty' => '',
      'hide_empty' => 0,
      'empty_zero' => 0,
      'type' => 'separator',
      'separator' => ', ',
      'link_to_taxonomy' => 1,
      'limit' => 1,
      'vids' => array(variable_get('offers_wants_type_vid', 0) => variable_get('offers_wants_type_vid', 0)),
      'exclude' => 0,
      'id' => 'tid',
      'table' => 'term_node',
      'field' => 'tid',
      'relationship' => 'none',
    ),
    'expires' => array(
      'label' => t('Expires'),
      'empty' => '',
      'hide_empty' => 0,
      'empty_zero' => 0,
      'date_format' => 'custom',
      'custom_date_format' => 'd/m/y',
      'exclude' => 0,
      'id' => 'expires',
      'table' => 'offers_wants',
      'field' => 'expires',
      'relationship' => 'none',
    ),
    'want' => array(
      'label' => '',
      'empty' => '',
      'hide_empty' => 0,
      'empty_zero' => 0,
      'type' => 'only',
      'exclude' => 1,
      'not' => 0,
      'id' => 'want',
      'table' => 'offers_wants',
      'field' => 'want',
      'relationship' => 'none',
    ),
    'edit_node' => array(
      'label' => '',
      'text' => t('Edit'),
      'exclude' => 0,
      'id' => 'edit_node',
      'table' => 'node',
      'field' => 'edit_node',
      'relationship' => 'none',
    ),
    'delete_node' => array(
      'label' => '',
      'empty' => '',
      'hide_empty' => 0,
      'empty_zero' => 0,
      'text' => t('delete'),
      'exclude' => 0,
      'id' => 'delete_node',
      'table' => 'node',
      'field' => 'delete_node',
      'relationship' => 'none',
    ),
  ));
  $handler->override_option('sorts', array(
    'expires' => array(
      'order' => 'DESC',
      'granularity' => 'hour',
      'id' => 'expires',
      'table' => 'offers_wants',
      'field' => 'expires',
      'relationship' => 'none',
    ),
  ));
  $handler->override_option('arguments', array(
    'uid' => array(
      'default_action' => 'default',
      'style_plugin' => 'default_summary',
      'style_options' => array(),
      'wildcard' => 'me',
      'wildcard_substitution' => 'My',
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
      'relationship' => 'none',
      'default_options_div_prefix' => '',
      'default_argument_user' => 0,
      'default_argument_fixed' => '',
      'default_argument_php' => '',
      'validate_argument_node_type' => array(),
      'validate_argument_node_access' => 0,
      'validate_argument_nid_type' => 'nid',
      'validate_argument_vocabulary' => array(),
      'validate_argument_type' => 'tid',
      'validate_argument_php' => '',
      'validate_user_argument_type' => 'uid',
      'validate_user_roles' => array(),
      'default_argument_image_size' => '_original',
      'validate_argument_transform' => 0,
      'validate_user_restrict_roles' => 0,
      'image_size' => array(
        '_original' => '_original',
        'thumbnail' => 'thumbnail',
        'preview' => 'preview',
      ),
    ),
  ));
  $handler->override_option('filters', array(
    'type' => array(
      'operator' => 'in',
      'value' => array(
        'proposition' => 'proposition',
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
    'perm' => 'view propositions',
  ));
  $handler->override_option('cache', array(
    'type' => 'none',
  ));
  $handler->override_option('items_per_page', 0);
  $handler->override_option('header_empty', 0);
  $handler->override_option('footer_empty', 1);
  $handler->override_option('style_plugin', 'table');
  $handler->override_option('style_options', array(
    'grouping' => 'want',
    'override' => 1,
    'sticky' => 1,
    'order' => 'asc',
    'columns' => array(
      'title' => 'title',
      'edit_node' => 'edit_node',
      'prop_types' => 'prop_types',
      'expires' => 'expires',
      'want' => 'want',
    ),
    'info' => array(
      'title' => array(
        'sortable' => 1,
        'separator' => '',
      ),
      'edit_node' => array(
        'separator' => '',
      ),
      'prop_types' => array(
        'separator' => '',
      ),
      'expires' => array(
        'sortable' => 0,
        'separator' => '',
      ),
      'want' => array(
        'separator' => '',
      ),
    ),
    'default' => '-1',
  ));
  $handler = $view->new_display('page', 'my_propositions', 'page_1');
  $handler->override_option('empty', t('No offers have been entered.'));
  $handler->override_option('empty_format', '1');
  $handler->override_option('path', 'user/%/propositions');
  $handler->override_option('menu', array(
    'type' => 'tab',
    'title' => t('Propositions'),
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
  $handler = $view->new_display('attachment', 'expired', 'attachment_2');
  $handler->override_option('fields', array(
    'title' => array(
      'label' => t('Expired'),
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
    'edit_node' => array(
      'label' => '',
      'text' => 'Edit',
      'exclude' => 0,
      'id' => 'edit_node',
      'table' => 'node',
      'field' => 'edit_node',
      'relationship' => 'none',
    ),
    'delete_node' => array(
      'label' => '',
      'empty' => '',
      'hide_empty' => 0,
      'empty_zero' => 0,
      'text' => '',
      'exclude' => 0,
      'id' => 'delete_node',
      'table' => 'node',
      'field' => 'delete_node',
      'relationship' => 'none',
    ),
  ));
  $handler->override_option('filters', array(
    'type' => array(
      'operator' => 'in',
      'value' => array(
        'proposition' => 'proposition',
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
    ),
  ));
  $handler->override_option('title', t('Expired propositions'));
  $handler->override_option('header', t('The following propositions have expired and are not visible to other users. To re-enable them, simply edit and save them.'));
  $handler->override_option('attachment_position', 'after');
  $handler->override_option('inherit_arguments', TRUE);
  $handler->override_option('inherit_exposed_filters', FALSE);
  $handler->override_option('inherit_pager', FALSE);
  $handler->override_option('render_pager', TRUE);
  $handler->override_option('displays', array(
    'page_1' => 'page_1',
    'default' => 0,
  ));
  return $view;
}

function _ow_expiry_view() {
  $view = new view;
  $view->name = 'propositions_expiring';
  $view->description = t('Propositions about to expire');
  $view->tag = 'offers/wants';
  $view->view_php = '';
  $view->base_table = 'node';
  $view->is_cacheable = FALSE;
  $view->api_version = 2;
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */
  $handler = $view->new_display('default', 'Defaults', 'default');
  $handler->override_option('fields', array(
    'title' => array(
      'label' => '',
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
    'name' => array(
      'label' => '',
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
      'relationship' => 'none',
    ),
    'expires' => array(
      'label' => 'Expires',
      'empty' => '',
      'hide_empty' => 0,
      'empty_zero' => 0,
      'date_format' => 'custom',
      'custom_date_format' => 'd/m/y',
      'exclude' => 0,
      'id' => 'expires',
      'table' => 'offers_wants',
      'field' => 'expires',
      'relationship' => 'none',
    ),
  ));
  $handler->override_option('sorts', array(
    'expires' => array(
      'order' => 'ASC',
      'granularity' => 'hour',
      'id' => 'expires',
      'table' => 'offers_wants',
      'field' => 'expires',
      'relationship' => 'none',
    ),
  ));
  $handler->override_option('filters', array(
    'type' => array(
      'operator' => 'in',
      'value' => array(
        'proposition' => 'proposition',
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
    'end' => array(
      'operator' => '=',
      'value' => '0',
      'group' => '0',
      'exposed' => FALSE,
      'expose' => array(
        'operator' => FALSE,
        'label' => '',
      ),
      'id' => 'end',
      'table' => 'offers_wants',
      'field' => 'end',
      'relationship' => 'none',
      'override' => array(
        'button' => 'Override',
      ),
    ),
  ));
  $handler->override_option('access', array(
    'type' => 'perm',
    'perm' => 'view propositions',
  ));
  $handler->override_option('cache', array(
    'type' => 'none',
  ));
  $handler->override_option('items_per_page', 25);
  $handler->override_option('style_plugin', 'table');
  $handler->override_option('style_options', array(
    'grouping' => '',
    'override' => 1,
    'sticky' => 0,
    'order' => 'asc',
    'columns' => array(
      'title' => 'title',
      'name' => 'name',
      'expires' => 'expires',
      'want' => 'want',
    ),
    'info' => array(
      'title' => array(
        'sortable' => 0,
        'separator' => '',
      ),
      'name' => array(
        'sortable' => 0,
        'separator' => '',
      ),
      'expires' => array(
        'sortable' => 0,
        'separator' => '',
      ),
      'want' => array(
        'separator' => '',
      ),
    ),
    'default' => '-1',
  ));
  $handler = $view->new_display('block', 'Block', 'block_1');
  $handler->override_option('fields', array(
    'want' => array(
      'label' => '',
      'empty' => '',
      'hide_empty' => FALSE,
      'empty_zero' => FALSE,
      'type' => 'only',
      'exclude' => 0,
      'id' => 'want',
      'table' => 'offers_wants',
      'field' => 'want',
      'override' => array(
        'button' => 'Use default',
      ),
    ),
    'title' => array(
      'label' => '',
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
  ));
  $handler->override_option('items_per_page', 5);
  $handler->override_option('use_more', 1);
  $handler->override_option('use_more_always', 0);
  $handler->override_option('style_plugin', 'list');
  $handler->override_option('style_options', array(
    'grouping' => '',
    'type' => 'ul',
  ));
  $handler->override_option('row_options', array(
    'inline' => array(
      'want' => 'want',
      'title' => 'title',
    ),
    'separator' => ': ',
    'hide_empty' => 0,
  ));
  $handler->override_option('block_description', '');
  $handler->override_option('block_caching', -1);


  $handler = $view->new_display('page', 'Page', 'page_1');
  $handler->override_option('path', 'propositions/expiring');
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

  return $view;
}
