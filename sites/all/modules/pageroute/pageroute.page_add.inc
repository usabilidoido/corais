<?php
// $Id: pageroute.page_add.inc,v 1.1.2.13 2009/11/07 13:32:09 sgilits Exp $

/**
 * @file
 * Page add type
 */

include_once(drupal_get_path('module', 'pageroute') .'/pageroute.route.inc');

/**
 * PageroutePageAdd displays a node adding form.
 */
class PageroutePageAdd extends PageroutePage {
  /**
   * Returns the node adding form for the configured node type
   */
  function getForm(&$form, &$form_state, &$args) {
    $args['hide_pageroute_buttons'] = FALSE;
    $args['default_target'] = PAGEROUTE_FORWARD;
    PageroutePageAdd::setNodeAddForm($form, $form_state, $this);
    parent::unsetForm($form);
  }

  public static function setNodeAddForm(&$form, &$form_state, &$page) {
    $type = $page->options['content-type'];

    // If a node type has been specified, validate its existence.
    if (node_access('create', $type)) {
      $account = user_load(array('uid' => pageroute_page_get_uid($page, 'administer nodes')));

      // Initialize settings:
      $node = array('uid' => $account->uid, 'name' => $account->name, 'type' => $type);

      $form += drupal_retrieve_form($type .'_node_form', $form_state, $node);
      drupal_prepare_form($type .'_node_form', $form, $form_state);
      $form['#options']['submit_handler'] = $form['buttons']['submit']['#submit'][0];
    }
    else {
      drupal_access_denied();
      pageroute_exit_now();
    }
  }

  public function getAdminForm($page, &$form) {
    $form['options']['content-type'] = array(
      '#type' => 'select',
      '#title' => t('Content type'),
      '#options' => node_get_types('names'),
      '#required' => TRUE,
      '#default_value' => $page->options['content-type'],
      '#weight' => 2,
    );
    if (!isset($page->options['cancel'])) {
      // We default to show the cancel button on add node forms.
      $form['options']['cancel']['#default_value'] = t('Cancel');
    }
    parent::nodeUI($page, $form, FALSE);
  }

  public static function help() {
    return t('A page of this type will present a common node adding form of a configurable content-type. The id of the new node will be added as new argument to the current path, so that other pages like a node view page can make use of it.');
  }

  public static function info() {
    return array(
      'name' => t('Node adding form'),
    );
  }

  public function getCancelTarget() {
    return PAGEROUTE_FORWARD;
  }

  public function formSubmitted(&$form_state) {
    switch ($form_state['clicked_button']['#value']) {
      case t('Save'):
        $args['default_target'] = PAGEROUTE_CURRENT;
      case t('Forward'):
      case t('Back'):
        unset($form_state['node']);
        break;
      case t('Preview'):
        $form_state['storage']['args']['default_target'] = PAGEROUTE_CURRENT;
        break;
    }
  }

  public function setUp() {
    include_once(drupal_get_path('module', 'node') .'/node.pages.inc');
  }

  public static function getDefaultSubmitHandler($form) {
    return $form['#options']['submit_handler'];
  }
}
