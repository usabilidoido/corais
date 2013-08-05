<?php
// $Id: mcapi.api.inc,v 1.1.2.3 2010/12/08 10:29:42 matslats Exp $

/**
 * API functions for mutual_credit
 */

/**
 * Create an exchange
 * It enables other modules to create exchanges by passing in the properties
 * these other modules should do all form-level data checking though.
 * this api allows a lot of empty data through
 * Any unknown $options will be packed in a serialized array for storage,
 * and appear as normal node properties outside the db
 *
 * There are two functions provided here, one for adding single and one for bundles of exchanges. Each one does:
 * build node object
 * validate node array
 * node_save()
 * hook to update balance & stats & sendmail
 */

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function mcapi_add_node($payer_uid, $payee_uid, $quantity, $options = array()) {
  static $result;
  module_load_include('inc', 'mcapi');

  $exchange = build_exchange_node($payer_uid, $payee_uid, $quantity, $options);
  //implementations of this hook should provide pre-built exchanges
  //NOT TESTED YET
  $result = new stdClass();
  $result->success = FALSE;
  if (count(drupal_get_messages('error', FALSE))) {
    $result->nodes = $exchanges;
    $result->messages['error'][] = t('Exchange failed validation');
    return $result;
  }
  else {
    node_save($exchange);
    //there shouldn't be failures at this point
    if (!isset($exchange->nid)) {
      //node was not saved, so stop trying and get to error reporting
      $errors = drupal_get_messages('error');
      $errors['error'][] = t('Failed to create exchange. Details have been forwarded to the site administrator');
      $result->errors = $errors['error'];
      drupal_mail_send(array(
        'to' => db_result(db_query("SELECT mail FROM {users} WHERE uid = :uid", array(':uid' => 1))),
        'subject' => t('Exchange failed on @site', array('@site' => variable_get('site_name', 'no_site'))),
        'body' => print_r($result, 1),
        'headers' => array('From' => variable_get('site_mail', '')),
      ));
      return $result;
    }
  }
  $result->node = $exchange;
  $result->success = TRUE;

  //gather up all the notifications
  $result->messages = $_SESSION['messages'];
  unset($_SESSION['messages']);

  return $result;
}


/*
 * limit checker
 * checks an account to see if the proposed difference will violate the balance limits
 * this is also called during exchange validation
 * static variable is in case 2 exchanges are being processed in one go.
 */
function mc_api_check_user_limits($account, $difference, $cid, $nid = NULL) {
  module_load_include('inc', 'mcapi');
  static $projected_totals;
  $uid = &$account->uid;

  if (!isset($projected_totals[$uid])) {
    $projected_totals[$uid] = 0;
  }
  $limits = user_limits($account, $cid);
  $balances = db_fetch_array(db_query("SELECT cleared_balance, pending_dif FROM {mc_cache} WHERE uid = %d and cid = %d", array(':uid' => $uid, ':cid' => $cid)));
  $balance = $balances['cleared_balance'] + $balances['pending_dif'];

  //bit of an afterthought this
  //if the node is already saved, we adjust the retrived the balance without counting the saved node
  if ($nid) {
    $exchange = node_load($nid);
    if ($exchange->state == EXCHANGE_STATE_FINISHED) {
      $sign = $account->uid == $exchange->payer_uid ? 1 : -1;
      $balance += $sign*$exchange->quantity;
    }
  }
  $projected_totals[$uid] += $balance + $difference;

  $messages = array();
  if (is_numeric($limits['max']) && (float)$difference > 0) {
    $surplus = $projected_totals[$uid] - $limits['max'];
    if ($surplus > 0) {
      $message = t('Exchange takes @username !quant above the maximum balance of !max',
        array(
          '@username' => strip_tags(theme('username', $account)),
          '!quant' => theme('money', $surplus, $cid),
          '!max' => theme('money', $limits['max'], $cid)
        )
      );
    }
  }
  elseif (is_numeric($limits['min']) && (float)$difference < 0) {
    $deficit = $limits['min'] - $projected_totals[$uid];
    if ($deficit > 0) {
      $message = t('Exchange takes @username !quant below the minimum balance of !min',
        array(
          '@username' => strip_tags(theme('username', $account)),
          '!quant' => theme('money', $deficit, $cid),
          '!min' => theme('money', $limits['min'], $cid)
        )
      );
    }
  }
  if (isset($message)) {
    return $message;
  }
}



/**
 * Provides a list of exchanges in a way that views cannot, and more efficiently.
 * This is not yet possible with views, because you can't do WHERE payer_uid = x OR payee_uid = x
 *
 * Options are (showing defaults) array(
 *   'to' => unixtime
 *   'from' => unixtime
 *   'direction' => enum(both, in, out)
 *   'states' = array()
 *   )
 * Main purpose of this function is to filter the list which is cached already.
 */

//need to find a way to cache these, but the list will be different for every user
//because each exchange has it's own visibility.
function mc_api_user_exchanges($uid, $cid, $options = array()) {
  module_load_include('inc', 'mcapi');
  $options += array(
    'to' => REQUEST_TIME,
    'from' => 0,
    'direction' => 'both',
    'states' => array(EXCHANGE_STATE_FINISHED),
  );
  $filtered = array();

  //this recalls the whole exchange history in one currency, in descending order of time
  //TODO IMPORTANT CACHE THIS
  $results = _get_transactions($uid, $cid, $options['states']);
  $balance = 0;
  while ($exchange = db_fetch_object($results)) {
    //add the running balance
    if ($exchange->payer_uid == $uid) {
      $balance -= $exchange->quantity;
      if ($options['direction'] == 'incoming') {
        continue;
      }
    }
    else {
      $balance += $exchange->quantity;
      if ($options['direction'] == 'outgoing') {
        continue;
      }
    }
    //then filters according to several criteria
    if (!mcapi_access('view', $exchange, $GLOBALS['user'])) {
      continue;
    }
    if (!in_array($exchange->state, $options['states'])) {
      continue;
    }
    if ($exchange->created > $options['to'] || ($exchange->created < $options['from'])) {
      continue;
    }

    $exchange->balance = $balance;
    $filtered[] = $exchange;
  }

  return $filtered;
}


//returns all the currency limits and cached balances for one user, in an array using currency ids as keys
function mc_balances($account, $cid = NULL) {
  if (!$cid) $cid = array_pop(array_keys(currencies_load()));
  module_load_include('inc', 'mcapi');
  //get all the currency rows for the given member
  $result = db_query(
    "SELECT cleared_balance, pending_dif, gross_in, gross_out, count
      FROM {mc_cache}
      WHERE uid = %d
      AND cid = %d",
    array(':uid' => $account->uid, ':cid' => $cid)
  );

  $balances = array();
  $data = db_fetch_array($result) or
  $data = array(
    'cleared_balance' => 0,
    'pending_dif' => 0,
    'gross_in' => 0,
    'gross_out' => 0,
    'count' => 0
  );

  $data += user_limits($account, $cid);
  $data['max_in'] = $data['max'] - $data['cleared_balance'];
  $data['max_out'] = $data['min'] - $data['cleared_balance'];
  return $data;
}



//returns all currencies
//this could happily be cached
function currencies_load() {
  static $currencies;
  if (!$currencies) {
    $results = db_query("SELECT nid FROM {node} WHERE type = 'currency'");
    while ($nid = db_result($results)) {
      $currencies[$nid] = node_load($nid);
    }
  }
  return $currencies;
}

function mc_currency_picker($default_cid = 0){
  $result = db_query("SELECT nid, title from {node} WHERE type = 'currency'");
  while ($c = db_fetch_object($result)) {
    $currencies[$c->nid] = $c->title;
  }
  if (!$default_cid) {
    $default_cid = key($currencies);
  }
  if (!isset($currencies[$default_cid])){
    drupal_set_message('Bad default supplied to currency_picker', 'error');
  }
  return array(
    '#title' => t('Currency'),
    '#description' => t('Choose from all the available currencies'),
    '#type' => 'select',
    '#options' => $currencies,
    '#default_value' => $default_cid,
    '#required' => TRUE,
  );
}