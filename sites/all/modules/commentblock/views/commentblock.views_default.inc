<?php
// $Id: commentblock.views_default.inc,v 1.1.2.1 2009/09/17 12:45:54 ximo Exp $

/**
 * @file
 * Default views data for 'Comment block' module.
 */

/**
 * Implementation of hook_views_default_views().
 */
function commentblock_views_default_views() {
  $view = new view;
  $view->name = 'comments';
  $view->description = 'A view to emulate Drupal core\'s comment listing.';
  $view->tag = 'default';
  $view->view_php = '';
  $view->base_table = 'comments';
  $view->is_cacheable = FALSE;
  $view->api_version = 2;
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */
  $handler = $view->new_display('default', 'Defaults', 'default');
  $handler->override_option('relationships', array(
    'nid' => array(
      'label' => 'Node',
      'required' => 1,
      'id' => 'nid',
      'table' => 'comments',
      'field' => 'nid',
      'relationship' => 'none',
    ),
  ));
  $handler->override_option('sorts', array(
    'timestamp' => array(
      'order' => 'ASC',
      'granularity' => 'second',
      'id' => 'timestamp',
      'table' => 'comments',
      'field' => 'timestamp',
      'override' => array(
        'button' => 'Override',
      ),
      'relationship' => 'none',
    ),
  ));
  $handler->override_option('arguments', array(
    'nid' => array(
      'default_action' => 'default',
      'style_plugin' => 'default_summary',
      'style_options' => array(),
      'wildcard' => 'all',
      'wildcard_substitution' => 'All',
      'title' => '',
      'default_argument_type' => 'node',
      'default_argument' => '',
      'validate_type' => 'none',
      'validate_fail' => 'not found',
      'break_phrase' => 0,
      'not' => 0,
      'id' => 'nid',
      'table' => 'node',
      'field' => 'nid',
      'relationship' => 'nid',
      'default_options_div_prefix' => '',
      'default_argument_user' => 0,
      'default_argument_fixed' => '',
      'default_argument_php' => '',
      'validate_argument_node_type' => array(
        'page' => 0,
        'story' => 0,
      ),
      'validate_argument_node_access' => 0,
      'validate_argument_nid_type' => 'nid',
      'validate_argument_vocabulary' => array(
        '2' => 0,
      ),
      'validate_argument_type' => 'tid',
      'user_argument_type' => '',
      'restrict_user_roles' => 0,
      'user_roles' => array(),
      'validate_argument_php' => '',
    ),
  ));
  $handler->override_option('filters', array(
    'comment' => array(
      'operator' => 'not in',
      'value' => array(
        '0' => '0',
      ),
      'group' => '0',
      'exposed' => FALSE,
      'expose' => array(
        'operator' => FALSE,
        'label' => '',
      ),
      'id' => 'comment',
      'table' => 'node',
      'field' => 'comment',
      'relationship' => 'nid',
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
      'table' => 'comments',
      'field' => 'status',
      'relationship' => 'none',
    ),
  ));
  $handler->override_option('access', array(
    'type' => 'perm',
    'perm' => 'access comments',
  ));
  $handler->override_option('title', 'Comments');
  $handler->override_option('items_per_page', 0);
  $handler->override_option('style_options', NULL);
  $handler->override_option('row_plugin', 'comment');
  $handler->override_option('row_options', array(
    'links' => 1,
  ));
  $handler = $view->new_display('block', 'Block', 'block_1');
  $handler->override_option('block_description', 'Comments');
  $handler->override_option('block_caching', -1);
  $views[$view->name] = $view;

  return $views;
}
