<?php
// $Id: views_handler_field_mc_trader.inc,v 1.1.2.2 2010/12/22 20:21:51 matslats Exp $

/**
 * @file
 * Field handler to allow linking to a payer's user page
 * this avoids having to make a relationship with the user table
 * @ingroup views_field_handlers
 */
class views_handler_field_mc_trader extends views_handler_field {

  function options(&$options) {
    parent::options($options);
    $options['link_to_trader'] = TRUE;
    $options['uid_only'] = FALSE;
  }

  function option_definition() {
    $options = parent::option_definition();
    $options['link_to_trader'] = array('default' => TRUE);
    $options['uid_only'] = array('default' => FALSE);
    return $options;
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    $form['link_to_trader'] = array(
      '#title' => t("Link this field to the user"),
      '#type' => 'checkbox',
      '#default_value' => $this->options['link_to_trader'],
    );
    $form['uid_only'] = array(
      '#title' => t("Show the user ID, not the username"),
      '#type' => 'checkbox',
      '#default_value' => $this->options['uid_only'],
    );
  }

  function render_link($name) {
    if (!$this->options['link_to_trader']) {
      $name = strip_tags($name);
    }
    return $name;
  }

  function render($values) {
    $user = &$values->{$this->field_alias};//this is the uid
    if (!$this->options['uid_only']) {
      $user = theme('username', user_load($user));
    }
    return $this->render_link($user);
  }
}

