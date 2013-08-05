<?php
// $Id: pageroute.page_useredit.inc,v 1.1.2.7 2009/09/15 10:01:00 fago Exp $

/**
 * @file
 * Page user edit type
 */

/**
 * PageroutePageUserEdit displays a page, where user can edit user accounts
 */
class PageroutePageUserEdit extends PageroutePage {

  public function getForm(&$form, &$form_state, &$args) {

    if ($args && isset($args['uid'])) {
      $account = user_load(array('uid' => $args['uid']));
    }
    else {
      $account = user_load(array('uid' => pageroute_page_get_uid($this, 'administer users')));
    }

    if (user_edit_access($account)) {
      if (!$account->uid) {
        drupal_not_found();
        pageroute_exit_now();
      }

      $args['hide_pageroute_buttons'] = FALSE;
      $args['default_target'] = PAGEROUTE_FORWARD;

      $this->options += array(
        'category' => 'account'
      );

      $form += drupal_retrieve_form('user_profile_form', $form_state, $account, $this->options['category']);
      drupal_prepare_form('user_profile_form', $form, $form_state);
      $this->unsetForm($form);
    }
    else {
      drupal_access_denied();
      pageroute_exit_now();
    }
  }
  
  public function unsetForm(&$form) {
    $options = $this->options;

    if (isset($options['submit']) && !$options['submit']) {
      unset($form['submit']);
    }
    unset($form['delete']);
  }

  public function getAdminForm($page, &$form) {
    $categories = array();
    foreach (_user_categories(false) as $category) {
      $categories[$category['name']] = $category['title'];
    }
    $form['options']['category'] = array(
      '#type' => 'select',
      '#title' => t('Category'),
      '#description' => t('The form of the chosen category will be used for this page.'),
      '#required' => TRUE,
      '#default_value' => isset($page->options['category']) ? $page->options['category'] : '',
      '#weight' => 2,
      '#options' => $categories,
    );
    $form['options']['submit'] = array(
      '#type' => 'checkbox',
      '#title' => t('Display submit button'),
      '#default_value' => isset($page->options['submit']) ? $page->options['submit'] : 1,
      '#weight' => 5,
    );
  }

  public static function help() {
    return t('A page of this type will present users\' account editing page inside a pageroute. By giving a user id as second pageroute argument it\'s also possible for administrators to edit another users\' account. By choosing another category it could also be used to integrate a drupal user profile into a pageroute.<br />!warn This page type is still experimental. If you consider using it, make sure you \'ve read !link before!',
      array('!warn' => '<strong>'. t('Warning!') .'</strong>', '!link' => l(t('this issue'), 'http://drupal.org/node/128045')));
  }

  public static function info() {
    return array('name' => t('User editing form'), 'useForm' => TRUE);
  }

  public function formSubmitted(&$form_state) {
    $todo = NULL;
    $args = &$form_state['storage']['route']->args;

    if ($form_state['clicked_button']['#value'] == 'Save') {
      $args['default_target'] = PAGEROUTE_CURRENT;
    }

    return $todo;
  }

  public function getCancelTarget() {
    return PAGEROUTE_FORWARD;
  }

  public function setUp() {
    include_once(drupal_get_path('module', 'user') .'/user.pages.inc');
  }
  
  public static function getDefaultSubmitHandler($form) {
    return 'user_profile_form_submit';
  }
}
