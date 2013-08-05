<?php
// $Id: pageroute.page_edit.inc,v 1.1.2.13 2009/10/17 11:49:20 sgilits Exp $

/**
 * @file
 * Page edit type
 */

include_once(drupal_get_path('module', 'pageroute') .'/pageroute.page_add.inc');

/**
 * PageroutePageEdit displays a node editing form.
 */
class PageroutePageEdit extends PageroutePage {

  function getForm(&$form, &$form_state, &$args) {

      $args['hide_pageroute_buttons'] = FALSE;
      $args['default_target'] = PAGEROUTE_CURRENT;

      $this->setNodeEditForm($form, $form_state, $this, $args);
      parent::unsetForm($form);
  }

  /**
   * Returns the node edit form for the configured node type or the give node id
   *
   * @param $argument_index Tells the function, which route argument shall be used for getting the node id.
   *   This allows reusing this function from other pages, e.g. the node management page. If nothing is
   *   given the function will use $nid page argument
   */
  public static function setNodeEditForm(&$form, &$form_state, &$page, &$args, $node = NULL) {

    if (!$node) {
      if ($args && isset($args['nid'])) {
        $nid = $args['nid'];
      }

      if (empty($nid) && $page->options['content-type']) {
        PageroutePageAdd::setNodeAddForm($form, $form_state, $page);
        return;
      }
      elseif (!is_numeric($nid) || !($node = node_load($nid)) ) {
        drupal_not_found();
        pageroute_exit_now();
      }
    }

    $type = isset($page->options['content-type']) ? $page->options['content-type'] : $node->type;

    if (isset($args['todo']['action']) &&  $args['todo']['action'] == t('Delete')) {
      $form += pageroute_node_delete_confirm($node, $form, $page, $args);
      unset($args['todo']);
      return;
    }

    if (node_access('update', $node)) {
      $form += drupal_retrieve_form($type .'_node_form', $form_state, $node);
      drupal_prepare_form($type .'_node_form', $form, $form_state);
    }
    else {
      drupal_access_denied();
      pageroute_exit_now();
    }
    $form['#options']['submit_handler'] = $form['buttons']['submit']['#submit'][0];
  }

  public function getAdminForm($page, &$form) {

    $form['options']['content-type'] = array(
      '#type' => 'select',
      '#title' => t('Content type for new nodes'),
      '#options' => array('' => '') + node_get_types('names'),
      '#default_value' => $page->options['content-type'],
      '#weight' => 2,
      '#description' => t('If there is no node id in the URL, a node add form
			for this content-type will be displayed. Leave it empty to show the Page Not Found error instead.'),
    );

    parent::nodeUI($page, $form, TRUE);
  }

  public static function help() {
    return t('A page of this type will present a common node editing form of a configurable content-type. It will edit the node with the id taken from the first argument of the pageroute. Furthermore this type can be configured to show a node adding form of a specific content-type if the node id argument is missing. So you can build a pageroute that manages the creation and editing of nodes of the same type.');
  }

  public static function info() {
    return array('name' => t('Node editing form'));
  }

  public function formSubmitted(&$form_state) {
    $todo = NULL;
    $args = &$form_state['storage']['route']->args;

    switch ($form_state['clicked_button']['#value']) {
      case t('Delete'):
        if (is_numeric($form_state['clicked_button']['#name'])) {
          $target = $form_state['clicked_button']['#name'];
        }
        else {
          $target = $args['todo']['target'];
        }
        $args['hide_pageroute_buttons'] = TRUE;
        $todo = array('action' => $form_state['clicked_button']['#value'], 'target' => $target);
        break;
      case t('Save'):
        $args['default_target'] = PAGEROUTE_CURRENT;
      case t('Forward'):
      case t('Back'):
        unset($form_state['node']);
        break;
    }

    $args['todo'] = $todo;
  }

  public function getCancelTarget() {
    return PAGEROUTE_FORWARD;
  }

  public function setUp() {
    include_once(drupal_get_path('module', 'node') .'/node.pages.inc');
  }

  public static function getDefaultSubmitHandler($form) {
    return $form['#options']['submit_handler'];
  }
}

/*
 * Provide an extra delete page to keep control about the destination parameter.
 */
function pageroute_node_delete_confirm($node, $form, &$page, &$args) {
  if (node_access('delete', $node)) {

    $form = array();
    $form['nid'] = array('#type' => 'value', '#value' => $node->nid);

    $form = confirm_form($form,
      t('Are you sure you want to delete %title?', array('%title' => $node->title)),
      pagerouteGetPagePath($page, $args),
      t('This action cannot be undone.'), t('Delete'), t('Cancel')
    );

    $form['actions']['submit']['#submit'][] = 'pageroute_node_delete_confirm_submit';

    return $form;
  }
  drupal_access_denied();
  pageroute_exit_now();
}

function pageroute_node_delete_confirm_submit($form, &$form_state) {
  $form_state['target'] = PAGEROUTE_CURRENT;

  if ($form_state['values']['confirm']) {
    node_delete($form_state['values']['nid']);
  }
  unset($form_state['clicked_button']);
  $form_state['rebuild'] = TRUE;
}
