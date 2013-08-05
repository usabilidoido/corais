<?php

/********************************************************************
 * Functions callback
 ********************************************************************/

function pg_showtransactions_edit(&$form_state,$t) {

  $form['email'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Email'),
    '#default_value' => $t->email,
  );

  $form['workflow']=array(
    '#title'         => t('Payment workflow'),
    '#type'          => 'select',
    '#options'       => pgapi_build_workflow(),
    '#default_value' => $t->workflow,
  );

  $form['status']=array(
    '#title'         => t('Payment status'),
    '#type'          => 'select',
    '#options'       => pgapi_build_status(),
    '#default_value' => $t->status,
  );

  $form['method']=array(
    '#title'         => t('Payment method'),
    '#type'          => 'select',
    '#options'       => pgapi_build_gateway(),
    '#default_value' => $t->method,
  );

  $form['txnid']=array(
    '#type'  => 'value',
    '#value' => $t->txnid,
  );
  $form['submit'] = array(
    '#type'  => 'submit',
    '#value' => t('Send'),
  );

  return $form;
}

function pg_showtransactions_edit_submit($form, &$form_state) {
  $txnid = $form_state['values']['txnid'];
  $t = pgapi_transaction_load($txnid);
  $t->method   = $form_state['values']['method'];
  $t->status   = $form_state['values']['status'];
  $t->workflow = $form_state['values']['workflow'];
  $t->email    = $form_state['values']['email'];
  pgapi_transaction_save($t);
}


function pg_showtransactions_history() {
  $filters = pg_showtransactions_build_filter_query('history_filter');
  $header = array(
    array('data' => t('ID'), 'field' => 't.txnid', 'sort' => 'desc'),
    array('data' => t('User'), 'field' => 'u.name'),
    array('data' => t('Email'), 'field' => 't.email'),
    array('data' => t('Title'), 'field' => 't.title'),
    array('data' => t('amount'), 'field' => 't.amount'),
    array('data' => t('Method'), 'field' => 't.method'),
    array('data' => t('payment status'), 'field' => 't.status'),
    array('data' => t('created'), 'field' => 't.created'),
    array('data' => t('changed'), 'field' => 't.changed'),
    t('Operations')
  );

  $sql='SELECT t.*, u.name FROM {pgapi_transaction} t LEFT JOIN {users} u ON u.uid=t.uid '.$filters['join'].' '.$filters['where'].tablesort_sql($header);

  $result = pager_query($sql, variable_get('pg_showtransactions_default_num',10), 0, NULL, $filters['args']);

  while ($txn = db_fetch_object($result)) {
    $action=l(t('Edit'), "admin/reports/showtransactions/edit/".$txn->txnid, array('query' => drupal_get_destination()));
    $rows[] = array(
      '<input type="checkbox" name="items['.$txn->txnid.']">',
      $txn->txnid,
      $txn->name ? l($txn->name, "user/".$txn->uid) : variable_get('anonymous',t('anonymous')),
      $txn->email,
      $txn->title,
      $txn->amount,
      pgapi_get_gateway_name($txn->method),
      pgapi_get_status($txn->status),
      format_date($txn->created,'small'),
      format_date($txn->changed,'small'),
      $action
    );
  }//while
  $filter_form = drupal_get_form('pg_showtransactions_filter_form','history_filter');

  $output = theme('pg_showtransactions_history',$rows,$filter_form);
  return $output;

}


function pg_showtransactions_operator_history() {
  $filters = pg_showtransactions_build_filter_query('history_filter');
  $header = array(
    array('data' => t('ID'), 'field' => 't.txnid', 'sort' => 'desc'),
    array('data' => t('User'), 'field' => 'u.name'),
    array('data' => t('Email'), 'field' => 't.email'),
    array('data' => t('Title'), 'field' => 't.title'),
    array('data' => t('amount'), 'field' => 't.amount'),
    array('data' => t('Method'), 'field' => 't.method'),
    array('data' => t('payment status'), 'field' => 't.status'),
    array('data' => t('created'), 'field' => 't.created'),
    array('data' => t('changed'), 'field' => 't.changed'),
    t('Operations')
  );

  $sql='SELECT t.*, u.name FROM {pgapi_transaction} t LEFT JOIN {users} u ON u.uid=t.uid '.$filters['join'].' '.$filters['where'].tablesort_sql($header);

  $result = pager_query($sql, variable_get('pg_showtransactions_default_num',10), 0, NULL, $filters['args']);

  while ($txn = db_fetch_object($result)) {
    $action=l(t('Edit'), "operator/showtransactions/edit/".$txn->txnid);
    $rows[] = array(
      '<input type="checkbox" name="items['.$txn->txnid.']">',
      $txn->txnid,
      $txn->name ? l($txn->name, "user/".$txn->uid) : variable_get('anonymous',t('anonymous')),
      $txn->email.'&nbsp;',
      $txn->title,
      $txn->amount,
      pgapi_get_gateway_name($txn->method),
      pgapi_get_status($txn->status),
      format_date($txn->created,'small'),
      format_date($txn->changed,'small'),
      $action
    );
  }//while
  $filter_form = drupal_get_form('pg_showtransactions_filter_form','history_filter');
  $output = theme('pg_showtransactions_operator_history',$rows,$filter_form);
  return $output;

}

function pg_showtransactions_personal_history() {
  global $user;

  drupal_add_js(drupal_get_path('module', 'pg_showtransactions') .'/pg_showtransactions.js');

  $filters = pg_showtransactions_build_filter_query('personal_history_filter');

  $header = array(
    array('data' => t('ID'), 'field' => 't.txnid', 'sort' => 'desc'),
    array('data' => t('Title'), 'field' => 't.title'),
    array('data' => t('amount'), 'field' => 't.amount'),
    array('data' => t('Method'), 'field' => 't.method'),
    array('data' => t('payment status'), 'field' => 't.status'),
    array('data' => t('created'), 'field' => 't.created'),
    array('data' => t('changed'), 'field' => 't.changed'),
    t('Operations')
  );
  if(empty($filters['where'])){
    $filters['where'] = "WHERE t.uid='".$user->uid."'";
  } else {
    $filters['where'] .= " AND t.uid='".$user->uid."'";
  }
  $sql='SELECT t.* FROM {pgapi_transaction} t '.$filters['join'].' '.$filters['where'].tablesort_sql($header);
  $result = pager_query($sql, variable_get('pg_showtransactions_'.$user->uid.'_num',10), 0, NULL, $filters['args']);

  while ($txn = db_fetch_object($result)) {
    if($txn->status == PG_PENDING ){
      $action=l(t('Pay'), "payment/".$txn->txnid);
    }else{
      $action='';
    }

    $rows[] = array(
      //'<input type="checkbox" name="items['.$txn->txnid.']">',
      $txn->txnid,
      $txn->title,
      $txn->amount,
      pgapi_get_gateway_name($txn->method),
      pgapi_get_status($txn->status),
      format_date($txn->created,'small'),
      $action
    );
  }//while
  $filter_form = drupal_get_form('pg_showtransactions_filter_form', 'personal_history_filter');
  $output.= theme('pg_showtransactions_personal_history', $rows, $filter_form);
  return $output;

}

function pg_showtransactions_filter_form($form_state,$sess){
  $sess='pg_showtransactions_'.$sess;
  $session = &$_SESSION[$sess];
  $session = is_array($session) ? $session : array();
  $filtersfunction=$sess.'s';
  $filters = $filtersfunction();

  $i = 0;
  $form['session_name']= array ('#type' => 'hidden', '#value' => $sess);
  $form['filters'] = array('#type' => 'fieldset',
                       '#title' => t('Show payment where'),
                       '#theme' => 'pg_showtransactions_filters',
  					   '#collapsible'=>true,
  					   '#collapsed'=>!count($session)
                     );

  foreach ($session as $filter) {
    list($type, $value) = $filter;
    // Merge an array of arrays into one if necessary.
    if($filters[$type]['type']=='textfield'){
      $params = array('%property' => $filters[$type]['title'] , '%value' => $value);
    }else{
      $options = $type == 'permission' ? call_user_func_array('array_merge', $filters[$type]['options']) : $filters[$type]['options'];
      $params = array('%property' => $filters[$type]['title'] , '%value' => $options[$value]);
    }
    if ($i++ > 0) {
      $form['filters']['current'][] = array('#value' => t('<em>and</em> where <strong>%property</strong> - <strong>%value</strong>', $params));
    }
    else {
      $form['filters']['current'][] = array('#value' => t('<strong>%property</strong> - <strong>%value</strong>', $params));
    }
    unset($filters[$type]);
  }

  foreach ($filters as $key => $filter) {
    $names[$key] = $filter['title'];
    if($filter['type'] == "textfield"){
     $form['filters']['status'][$key] = array('#type' => 'textfield', '#size' => 23);
    } else {
      $form['filters']['status'][$key] = array(
      	'#type' => 'select',
        '#options' => $filter['options'],
//        '#attributes' => array()
      );
    }
  }

  $form['filters']['filter'] = array('#type' => 'radios',
                                 '#options' => $names,
                               );
  $form['filters']['buttons']['submit'] = array('#type' => 'submit',
                                            '#value' => (count($session) ? t('Refine') : t('Filter'))
                                          );
  if (count($session)) {
    $form['filters']['buttons']['undo'] = array('#type' => 'submit',
                                            '#value' => t('Undo')
                                          );
    $form['filters']['buttons']['reset'] = array('#type' => 'submit',
                                             '#value' => t('Reset')
                                           );
  }

  return $form;
}

function pg_showtransactions_filter_form_submit($form, &$form_state) {
  $op = $form_state['values']['op'];
  $sess=$form_state['values']['session_name'];
  $filterfunction = $sess.'s';
  $filters = $filterfunction();

  switch ($op) {
    case t('Filter'):
    case t('Refine'):
      if (isset($form_state['values']['filter'])) {
        $filter = $form_state['values']['filter'];
        // Merge an array of arrays into one if necessary.
        if($filters[$filter]['type'] == 'textfield'){
          $_SESSION[$sess][] = array($filter, $form_state['values'][$filter]);
        }else{
          $options = $filter == 'permission' ? call_user_func_array('array_merge', $filters[$filter]['options']) : $filters[$filter]['options'];
          if (isset($options[$form_state['values'][$filter]])) {
            $_SESSION[$sess][] = array($filter, $form_state['values'][$filter]);
          }
        }
      }
      break;
    case t('Undo'):
      array_pop($_SESSION[$sess]);
      break;
    case t('Reset'):
      $_SESSION[$sess] = array();
      break;
    case t('Update'):
      return;
  }
}



/********************************************************************
 * Other Functions
 ********************************************************************/

function pg_showtransactions_build_filter_query($sess) {
  $sess='pg_showtransactions_'.$sess;
  $filterfunction = $sess.'s';
  $filters = $filterfunction();

  // Build query
  $where = $args = $join = array();
  if(isset($_SESSION[$sess])){
    foreach ($_SESSION[$sess] as $filter) {
      list($key, $value) = $filter;
      $where[] = $filters[$key]['where'];
      $args[] = $value;
      $join[] = $filters[$key]['join'];
    }
  }
  $where = count($where) ? 'WHERE '. implode(' AND ', $where) : '';
  $join = count($join) ? ' '. implode(' ', array_unique($join)) : '';

  return array('where' => $where,
           'join' => $join,
           'args' => $args,
         );
}

function pg_showtransactions_history_filters(){
  $filters = array();
  $filters['ID']= array('type'  => 'textfield',
                          'title' => t('ID'),
                          'where' => "t.txnid='%s'",
                       );
  $filters['name']= array('type'  => 'textfield',
                          'title' => t('User'),
                          'where' => "u.name='%s'",
                       );

  $filters['email']= array('type'  => 'textfield',
                          'title' => t('Email'),
                          'where' => "t.email='%s'",
                       );

  $filters['price_from']= array('type'  => 'textfield',
                          'title' => t('Pay amount more than'),
                          'where' => "t.amount>='%f'",
                       );

  $filters['price_to']= array('type'  => 'textfield',
                          'title' => t('Pay amount less than'),
                          'where' => "t.amount<='%f'",
                       );

  $filters['product']= array('type'  => 'textfield',
                          'title' => t('Title'),
                          'where' => "t.title LIKE '%%%s%'",
                       );
  $filters['method']= array('title' => t('Method'),
                          'where' => "t.method = '%s'",
                          'options' => pgapi_build_gateway(),
                       );

  $filters['status'] = array('title' => t('payment status'),
                         'where' => 't.status = %d',
                         'options' => pgapi_build_status(),
                       );

  return $filters;
}

function pg_showtransactions_personal_history_filters(){
  $filters = array();
  $filters['ID']= array('type'  => 'textfield',
                          'title' => t('ID'),
                          'where' => "t.txnid='%s'",
                       );

  $filters['price_from']= array('type'  => 'textfield',
                          'title' => t('Pay amount more than'),
                          'where' => "t.amount>='%f'",
                       );

  $filters['price_to']= array('type'  => 'textfield',
                          'title' => t('Pay amount less than'),
                          'where' => "t.amount<='%f'",
                       );


  $filters['product']= array('type'  => 'textfield',
                          'title' => t('Title'),
                          'where' => "t.title LIKE '%%%s%'",
                       );
  $filters['method']= array('title' => t('Method'),
                         'where' => "t.method = '%s'",
                         'options' => pgapi_build_gateway(),
                       );

  $filters['status'] = array('title' => t('payment status'),
                         'where' => 't.status = %d',
                         'options' => pgapi_build_status(),
                       );



  return $filters;
}


