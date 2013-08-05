<?php
// $Id: views_handler_field_mc_running_balance.inc,v 1.1.2.2 2010/12/22 20:21:51 matslats Exp $

/**
 * @file
 * Field handler to allow linking to a payee's user page
 * @ingroup views_field_handlers
 */
class views_handler_field_mc_running_balance extends views_handler_field {

  function construct() {
    parent::construct();
    //this is added by something else, at least usually
    $this->additional_fields['cid'] = 'cid';
    $this->additional_fields['nid'] = 'nid';
    $this->additional_fields['state'] = 'state';
  }
  
  function query() {
    $this->ensure_my_table();
    $this->add_additional_fields();
  }

  function render($values) {
    if ($values->{$this->aliases['state']} != EXCHANGE_STATE_FINISHED) return;
    if (isset($this->view->argument['participant']) && count($this->view->args)) {
      $key = array_search('participant', array_keys($this->view->argument));
      if ($key !== FALSE) {
        $uid = $this->view->args[$key];
        $query = "SELECT sum(quantity)
          FROM {mc_exchanges} e LEFT JOIN {node} n ON n.nid = e.nid
          WHERE payer_uid = $uid 
            AND n.nid <= ".$values->{$this->aliases['nid']}."
            AND cid = ".$values->{$this->aliases['cid']};
        $outgoing = db_result(db_query($query));
        $query = str_replace('payer', 'payee', $query);
        $income = db_result(db_query($query));
        return theme('money', $income-$outgoing, $values->mc_exchanges_cid);
      }
    }
  }
}
