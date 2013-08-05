<?php
// $Id: views_handler_field_other_participant.inc,v 1.1.2.2 2010/12/22 20:21:51 matslats Exp $

/**
 * @file
 * Field handler to allow linking to a payer's user page
 * @ingroup views_field_handlers
 */
class views_handler_field_other_participant extends views_handler_field {
  
  function construct() {
    parent::construct();
    if ($this->definition['group'] == t('Exchanges')) {
      $this->additional_fields['payer_uid'] = 'payer_uid';
      $this->additional_fields['payee_uid'] = 'payee_uid';
    }
  }

  function options(&$options) {
    parent::options($options);
    $options['link_to_other'] = TRUE;
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    if ($this->definition['group'] == t('Exchanges')) {
      $form['link_to_other'] = array(
        '#title' => t("Link this field to the user"),
        '#type' => 'checkbox',
        '#default_value' => $this->options['link_to_other'],
      );
    }
  }
  
  function query() {
    $this->ensure_my_table();
  }

  function render($values) {
    return $this->render_link($values->participant);
  }

  function render_link($data) {
    $name = theme('username', user_load($data));
    if (!$this->options['link_to_other']) {
      $name = strip_tags($name);
    }
    return $name;
  }
}



function mcapi_views_query_alter(&$view, &$query) {
  if (isset($view->argument['participant']) && count($view->args)) {
    $key = array_search('participant', array_keys($view->argument));
    if ($key !== FALSE) {
      $query->fields['other'] = array(
        'field' => 'if (payer_uid = '.$view->args[$key].', payee_uid, payer_uid)',
        'table' => '',
        'alias' => 'participant'
      );
    }
  }
  if (array_intersect(array('unknown ASC', 'unknown DESC'), $query->orderby)) {
    $query->orderby = array('participant');
    unset($query->fields['unknown']);
  }
}