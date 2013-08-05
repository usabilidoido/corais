<?php
// $Id: views_handler_field_mc_amount.inc,v 1.1.2.2 2010/12/22 20:21:51 matslats Exp $

/**
 * @file
 * Field handler to allow linking to a payee's user page
 * @ingroup views_field_handlers
 */
class views_handler_field_mc_amount extends views_handler_field {

  function construct() {
    parent::construct();
    $this->additional_fields['cid'] = 'cid';
    if ($this->definition['group'] != t('Balances')) {
      $this->additional_fields['payer_uid'] = 'payer_uid';
      $this->additional_fields['payee_uid'] = 'payee_uid';
    }
  }

  function option_definition() {
    $options = parent::option_definition();
    $options['output_type'] = array('default' => 'normal');
    return $options;
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    $options = array(
      'incoming' => t('Show only incoming'),
      'outgoing' => t('Show only outgoing'),
      'sign' => t('Change sign to minus'),
      'normal' => t('Just show amount')
    );

    if ($this->definition['group'] == t('Exchanges')) {
      $form['output_type'] = array(
        '#type' => 'radios',
        '#title' => t('Output type'),
        '#description' => t('When a user argument is passed, this option is relative to that user, otherwise ignored'),
        '#options' => $options,
        '#default_value' => $this->options['output_type'],
      );
    }
  }

  function render($values) {
    //if we are using the 'user_exchanges' argument, the options kick in.
    //not the most efficient way to access the arguments
    if (isset($this->view->argument['participant']) && count($this->view->args)) {
      $key = array_search('user_exchanges', array_keys($this->view->argument));
      $uid = $this->view->args[$key];
      switch($this->options['output_type']) {
        case 'outgoing':
          if ($values->{$this->aliases['payee_uid']} == $uid) return;
          break;
        case 'incoming':
          if ($values->{$this->aliases['payer_uid']} == $uid) return;
          break;
        case 'sign':
          if ($values->{$this->aliases['payer_uid']} == $uid) {
            //negate the value
            $values->{$this->field_alias} = -$values->{$this->field_alias};
          }
      }
    }
    elseif ($this->options['output_type'] != 'normal') {
      return t('n/a');
    }
    //in case there isn't a row yet in the cache table
    elseif ($values->{$this->aliases['cid']} == NULL && $values->cleared_balance == NULL) { 
      $values->{$this->aliases['cid']} = db_result(db_query("SELECT nid FROM {node} WHERE type = 'currency'"));
    }
    return theme('money', $values->{$this->field_alias}, $values->{$this->aliases['cid']});
  }
}