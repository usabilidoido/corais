<?php

class views_handler_filter_node_nid_php extends views_handler_filter {

  function admin_summary() {return '';}

  function can_expose() {return FALSE;}

  function operator_options() {
    return array(
      'AND' => t('filter to these IDs'),
      'NAND' => t('exclude these IDs'),
    );
  }
  
  function option_definition() { 
    $options = parent::option_definition();
    $options['operator']['default'] = 'AND';
    $options['value']['default'] = '';
    $options['handler']['default'] = 'php';
    return ($options);
  }

  function value_form(&$form, &$form_state) {
    $form['value'] = array(
      '#title' => t('PHP (or IDs)'),
      '#type' => 'textarea',
      '#default_value' => $this->value,
    );
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    $form['handler'] = array(
      '#type' => 'radios',
      '#title' => t('Handler'),
      '#options' => array('php' => t('PHP code'), 'nid' => t('Node ID list')),
      '#default_value' => $this->options['handler'],
    );
  }

  function _parse_input($rawcode) {
    $rawcode = trim($rawcode);
    if ($rawcode == "") {
      watchdog('viewsphpfilter', "PHP filter undefined; filter ignored", array(), WATCHDOG_NOTICE);
      return false;
    }
    if ($this->options['handler'] == 'php') {
      $result = eval ($rawcode);
      if ($result === false) {
        watchdog('viewsphpfilter', "Parse error in PHP: $this->value; filter ignored", array(), WATCHDOG_ERROR);
        return false;
      }
    } else $result = $rawcode;
    if ($result === null) {
      watchdog('viewsphpfilter', "PHP filter returned null; filter ignored", array(), WATCHDOG_NOTICE);
      return false;
    }
    if (!is_array($result)) {
      $result = explode(',',$result);
    }
    $trimresult = array();
    foreach ($result as $retval) {
      $trimmed = trim($retval);
      if ($trimmed != "") {
        if ((((string)((int)$retval)) != $retval) || ($retval < 1)) {
          watchdog('viewsphpfilter', "Invalid return value in PHP: " . implode(",",$result) . "; filter ignored", array(), WATCHDOG_ERROR);
          return false;
        } else {
          $trimresult[] = $trimmed;
        }
      }
    }
    //watchdog('viewsphpfilter', "PHP filter return value: array('" . implode("','",$trimresult) . "')", array(), WATCHDOG_DEBUG);
    return $trimresult;
  }
  
  function query() {
    if (($result = $this->_parse_input($this->value)) === false) return;
    $this->ensure_my_table();
    if ($this->operator == 'NAND') {
      if (!(empty($result) || (count($result) == 1 && $result[0]==NULL))) {
        $this->query->add_where($this->options['group'], "{$this->table_alias}.{$this->real_field} NOT IN (%s)", implode(",", $result));
      }
    }
    else { // $this->operator == 'AND'
      if (empty($result) || (count($result) == 1 && $result[0]==NULL)) {
        $this->query->add_where($this->options['group'], "{$this->table_alias}.{$this->real_field} IS NULL");
      } else {
        $this->query->add_where($this->options['group'], "{$this->table_alias}.{$this->real_field} IN (%s)", implode(",", $result));
      }
    }
  }
}
