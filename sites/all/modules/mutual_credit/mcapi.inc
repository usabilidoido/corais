<?php
// $Id: mcapi.inc,v 1.1.2.7 2010/12/22 19:30:29 matslats Exp $

/**
 * @file
 * This file contains all functions to view exchange content
 */
function mcapi_form_exchange_inc($form, $node) {
  //set the currency if there's only one to choose from, or it is given in the node
  if (!empty($node->cid)) {
    if ($form['#currencies'][$node->cid]) {
      $form['cid'] = array(
        '#type' => 'hidden',
        '#value' => $node->cid,
      );
    }
    else {
      drupal_set_message(t('Given currency id is not valid: @id', array('@id' => $node->cid)), 'warning');
    }
  }
  else {
    $form['cid'] = array(
      '#type' => 'select',
      '#options' => mc_node_select_options($form['#currencies'], t('Choose currency...')),
      '#title' => t('Currency'),
      '#description' => t('The node ID of the currency'),
      '#weight' => 0,
      '#element_validate' => array('mc_validate_cid')
    );
  }
  $form['title'] = array(
    '#type' => 'textfield',
    '#title' => t('Reason for exchange'),
    '#default_value' => isset($node->title) ? $node->title : '',
    '#element_validate' => array('mc_validate_title'),
    '#required' => TRUE,
    '#weight' => 3,
    '#size' => 35
  );
  if ($body = db_result(db_query("SELECT body_label from {node_type} where type = 'exchange'"))) {
    $form['body'] = array(
      '#type' => 'textarea',
      '#title' => t('Details of exchange'),
      '#default_value' => isset($node->body) ? $node->body : '',
      '#weight' => 4,
      '#size' => 35,
      '#height' => 4
    );
  }
  $form['payer_uid'] = array(
    '#type' => 'textfield',
    '#title' => t('Account to be debited'),
    '#default_value' => isset($node->payer_uid) ? $node->payer_uid : '',
    '#element_validate' => array('mc_validate_trader'),
    '#weight' => 6
  );
  $form['payee_uid'] = array(
    '#type' => 'textfield',
    '#title' => t('Account to be credited'),
    '#description' => t('A username, email, or user ID'),
    '#default_value' => isset($node->payee_uid) ? $node->payee_uid : '',
    '#element_validate' => array('mc_validate_trader'),
    '#weight' => 9
  );
  $form['quantity'] = array(
    '#type' => 'textfield',
    '#title' => t('Quantity'),
    '#element_validate' => array('mc_validate_quantity'),
    '#default_value' => isset($node->quantity) ? $node->quantity : '',
    '#weight' => 12,
    '#size' => 7,
  );
  $form['state'] = array(
    '#type' => 'radios',
    '#title' => t('State'),
    '#description' => t('Finished exchanges cannot have empty values.'),
    '#default_value' => isset($node->state) ? intval($node->state) : '0', //defaults to 0, or EXCHANGE_STATE_FINISHED
    '#options' => array(
      EXCHANGE_STATE_FINISHED => t('Finished'),
      EXCHANGE_STATE_PENDING => t('Pending'),
      //there is no use case for this, because erased exchanges have no entry in the node table
      //EXCHANGE_STATE_ERASED => t('erased'),
    ),
    '#weight' => 18
  );
  return $form;
}

function mcapi_form_currency(&$node) {
  $form['title'] = array(
    '#type' => 'textfield',
    '#title' => t('Currency name'),
    '#default_value' => $node->title,
    '#description' => t('Use the plural'),
    '#required' => TRUE,
    '#size' => 10,
  );
  $form['body'] = array(
    '#type' => 'textarea',
    '#title' => t('Rationale'),
    '#default_value' => $node->body,
    '#description' => t('What are the credentials of this currency? Richard Douthewaite poses these questions:') . theme('item_list', array(
      t('Who issues the money?'),
      t('Why do they issue it?'),
      t('Where is the money created?'),
      t('What gives the money its value?'),
      t('How is the money created?'),
      t('When is the money created?'),
    ), NULL, 'ol'),
    '#required' => TRUE,
    '#size' => 10,
  );
  $form['range'] = array(
    '#type' => 'fieldset',
    '#title' => t('Default balance limits'),
    '#description' => t("Up to !num digits. The system will prevent exchanges if the either participant's balance would go outside these numbers.", array('!num' => 6)) . ' '.
      t("Users with 'edit all exchanges' permission can overridde these limits on the users' account pages, and bypass these limits in any exchanges they edit."),
    '#prefix' => '<style>#edit-min-wrapper, #edit-max-wrapper{width:49%;float:left;}</style>', //helps with garland theme at least
    '#element_validate' => array('mc_validate_limits')
  );
  $def_min = isset($node->min) ? $node->min : 0;
  $def_max = isset($node->max) ? $node->max : 100;
  $form['range']['min'] = array(
    '#type' => 'textfield',
    '#title' => t('Optional min balance'),
    '#default_value' => $def_min,
    '#size' => 7,
    '#maxlength' => 7,
    '#description' => t('Must not be larger than 0'),
    '#attributes' => array('style' => 'width:inherit;') //garland says all form textfields should be 100%
  );
  $form['range']['max'] = array(
    '#type' => 'textfield',
    '#title' => t('Optional max balance'),
    '#default_value' => $def_max,
    '#size' => 6,
    '#maxlength' => 6,
    '#weight' => 1,
    '#description' => t('Must be a positive integer.'),
    '#attributes' => array('style' => 'width:inherit;'),
  );

  $form['visual'] = array(
    '#type' => 'fieldset',
    '#title' => t('Appearance'),
    '#description' => t("Displays of money can be further tweaked by overriding theme_money and/or with css class 'currency-0'"),
  );
  $form['#attributes'] = array('enctype' => "multipart/form-data");
  $form['visual']['symbol'] = array(
    '#type' => 'textfield',
    '#title' => t('Notation'),
    '#default_value' => $node->symbol,
    '#description' => t('Write an expression to control the display of the currency using the following tokens:') .' @icon, @quant, @subdivision ' .
       t('For example to display -$45:23 AUS, enter @minus@icon@integer:@subdivision AUS'),
    '#required' => TRUE,
    '#size' => 10,
  );
  $form['visual']['icon'] = array(
    '#type' => 'file',
    '#title' => t('Upload icon'),// array('!icon' => theme('1', $currency))),
    '#prefix' => t('Current icon') .': ' . theme('image', $node->icon),
    '#description' => t("Suggested size: 12px, preferably with a transparent background."),
    '#element_validate' => array('mc_validate_icon_upload'),
    '#size' => 25,//the default, 60, ruins the 2 col theming
  );
  $form['visual']['color'] = array(
    '#type' => 'textfield',
    '#title' => t('A hex colour value'),
    '#default_value' => $node->data['color'] ? $node->data['color'] : '000000',
    '#description' => t('6 hex digits'),
    '#maxlength' => 6,
    '#size' => 6,
    '#field_prefix' => '#'
  );

  $form['sub_divisions'] = array(
    '#type' => 'fieldset',
    '#title' => t('Fractions'),
  );
  $form['sub_divisions']['sub'] = array(
    '#type' => 'radios',
    '#default_value' => intval($node->sub),
    '#options' => array(
      '0' => t('1 unit is the smallest possible exchange'),
      '1' => t('Enable fractions').'...'
    )
  );
  $form['sub_divisions']['divisions'] = array(
    '#type' => 'textarea',
    '#title' => t('Fractions visible in form'),
    '#description' => '<p>'. t("Only applies when 'database storage' is set to 'fractions'.") .' '.
      t('Leave blank to use hundredths. To use a dropdown, such as for quarters of an hour, write a "hundredths value| visible text" on each line. E.g. ') .'</p>' .
      '<pre>0| exactly<br />50| half an hour',
    '#default_value' => $node->data['divisions'],
    '#element_validate' => array('mc_validate_currency_divisions'),
    '#rows' => 4,
  );
  $form['value'] = array(
    '#type' => 'textfield',
    '#title' => t('Comparative value'),
    '#default_value' => $node->value,
    '#size' => 4,
    '#maxlength' => 6,
    '#description' => t("The value of the currency in relation to other currencies in the same trading network (not yet operational)")
  );
  return $form;
}

function theme_currency_node_form($form) {
  $output = "\n<div class=\"node-form currency\">\n";
  $output .= "\n<div style = \"width:49%;float:left;\">";
  $output .= drupal_render($form['title']);
  $output .= drupal_render($form['body']);
  $output .= drupal_render($form['value']);
  $output .= drupal_render($form['range']);
  if ($form['subtype_bitmask']) {// this saves overriding the whole theme in the 1stparty form
    $output .= drupal_render($form['subtype_bitmask']);
  }
  $output .='</div><div style = "width:49%; float:right;">';
  $output .= drupal_render($form['visual']);
  $output .= drupal_render($form['sub_divisions']);
  $output .= "</div>";
  $buttons = drupal_render($form['buttons']);
  // Everything else gets rendered here, and is displayed before the admin form
  // field and the submit buttons.
  $output .= "  <div class=\"standard\">\n";
  $output .= drupal_render($form);
  $output .= "  </div>\n";

  $output.= $buttons;
  $output .= "</div>\n";

  return $output;
}
function theme_currency_range($currency) {
  $header = array(t('Min'), t('Max'));
  $rows = array(
    array(
      theme('money', $currency->min, $currency->nid),
      theme('money', $currency->max, $currency->nid)
    )
  );
  return theme('table', $header, $rows);
}

function mcapi_node_presave_currency(&$node) {//this $node is just the form values
  $node->data['color'] = $node->color;
  if (isset($node->upload_icon)) {
    $file = $node->upload_icon;
    //it was parked in the temp directory in the validation phase
    $dest = file_directory_path() .'/currencies/'. $file->filename;
    if (file_move($file->filepath, $dest, FILE_EXISTS_REPLACE)) {
      $node->icon = $dest;
    }
    else {
      drupal_set_message(t('Failed to upload new currency icon'), 'error');
    }
  }
  elseif ($node->icon == '') {
    unset($node->icon); //otherwise it will overwrite the icon with blank
  }
  $node->data['divisions'] = $node->divisions;
}

function mc_preprocess_exchange($node) {
  //created is only blank if the exchange isn't actually created
  if(!isset($node->created)) {
    $node->created = time();
    $props['title_link'] = $node->title;
  }
  else {
    $props['title_link'] = l($node->title, 'node/'. $node->nid);
  }
  $props['submitted'] = format_date($node->created, 'small');
  $props['payer'] = theme('username', user_load($node->payer_uid));
  $props['payee'] = theme('username', user_load($node->payee_uid));
  $props['amount'] = theme('money', $node->quantity, $node->cid);

  $props['classes'] = array();
  return $props;
}


//this could almost have been done with a view, but as this is the API module, we're saving a dependency
function currency_list() {
  $currencies = currencies_load();
  $headers = array(t('Symbol'), t('Name'), '-99 '. t('Format'), t('usage'));
  $output = '';
  foreach ($currencies as $currency) {
    $row['icon'] = theme('image', $currency->icon);
    $row['name'] = l($currency->title, 'node/'.$currency->nid);
    $row['format'] = theme('money', -99, $currency->nid);
    $row['usage'] = db_result(db_query("SELECT count(nid) FROM {mc_exchanges} WHERE cid = %d", array(':cid' => $currency->nid)));
    if (user_access('configure currencies')) {
      $row['edit'] = l(t('edit'), 'node/'. $currency->nid . '/edit');
    }
    $rows[] = $row;
  }
  $output .= theme('table', $headers, $rows);

  if (user_access('declare currency')) {
    $output .= l(t('Declare new currency'), 'node/add/currency');
  }
  return $output;
}


//return the max and min limits, by overriding currency limits with personal ones
//if $cid is not supplied an array of currencies is returned.
function user_limits($account, $cid){
  $currency = node_load($cid);
  $limits = array(
    'max' => $currency->max,
    'min' => $currency->min
  );
  if (!isset($account->mc_limits)) {
    return $limits;
  }
  foreach (array('max', 'min') as $extent) {
    if (isset($account->mc_limits[$currency->nid][$extent]) && $account->mc_limits[$currency->nid][$extent]) {
      $limits[$extent] = $account->mc_limits[$currency->nid][$extent];
    }
  }
  return $limits;
}

function mc_admin_menu_block_page() {
  module_load_include('admin.inc', 'system');
  return system_admin_menu_block_page();
}

//refreshes a cache of all the exchanges, calculates the running balances and other totals
//this is an unusual kind of cache which should never be empty, because some functions access the cache table directly
//for this reason this function REPLACES rows.
//in v3, this is handled completely differently!
function _refresh_mc_cache($uid = 0, $cid = 0) {
  $cleared_balance = $pending_dif = $gross_out = $gross_in = $count = 0;
  $result = _get_transactions($uid, $cid);
  //collate them
  $users = array();
  while ($exchange = db_fetch_object($result)) {
    $q = &$exchange->quantity;
    $payer = &$users[$exchange->payer_uid][$exchange->cid];
    $payee = &$users[$exchange->payee_uid][$exchange->cid];
    if ($exchange->state == EXCHANGE_STATE_FINISHED) {
      $payer['cleared_balance'] -= $q;
      $payee['cleared_balance'] += $q;
      $payer['gross_out'] += $q;
      $payee['gross_in'] += $q;
      $payer['count']++;
      $payee['count']++;
    }
    else {
      $payer['pending_dif'] -= $q;
      $payee['pending_dif'] += $q;
    }
  }
  //if we are doing this for one user, which is mostly the case, exclude all the other users
  if ($uid) {
    if ($users[$uid]) {
      $users = array($uid => $users[$uid]);
    }
    elseif ($cid) {
      //create a null row if there are no transactions for the given user
      $users = array($uid => array($cid => array('cleared_balance' => 0, 'pending_dif' => 0, 'gross_in' => 0, 'gross_out' => 0, 'count' => 0)));
    }
  }
  while (list($user_id, $row) = each($users)) {
    foreach ($row as $curr_id => $values) {
      db_query("REPLACE INTO {mc_cache} (uid, cid, cleared_balance, pending_dif, gross_in, gross_out, count)
        VALUES (%d, %d, %f, %f, %f, %f, %d)",
        array($user_id, $curr_id, $values['cleared_balance'], $values['pending_dif'], $values['gross_in'], $values['gross_out'], $values['count'])
      );
    }
  }
}

function _get_transactions($uid = 0, $cid = 0, $states = array(EXCHANGE_STATE_FINISHED, EXCHANGE_STATE_PENDING)) {
  foreach ($states as $state) {
    $state_filters[] = "e.state = " . $state;
  }
  $filters[] = '('. implode(' OR ', $state_filters) .' )';
  if ($cid) {
    $filters[] = " e.cid = $cid ";
  }
  if ($uid) {
    $filters[] = "(payer_uid = $uid OR payee_uid = $uid)";
  }
  return db_query("SELECT n.uid, n.title, n.nid, e.payer_uid, e.payee_uid, e.quantity, e.state, n.created, e.exchange_type, e.cid
    FROM {node} n
    LEFT JOIN {mc_exchanges} e ON e.nid = n.nid
    WHERE n.type = 'exchange'
    AND ".implode(' AND ', $filters)."
    ORDER BY n.created ASC"
  );
}

function _mcapi_default_mails($key) {
  $template = variable_get($key, '');
  if ($template) return $template;
  switch ($key) {
    case 'mc_notification_subject':
      return t('An exchange with !username was recorded and your balance updated');

    case 'mc_notification_body':
      return t("Dear !username,") ."\n\n". 
        t('The following exchange was recorded on !site by !creator') ."\n\n!summary\n\n".
        t('Your balance has been updated.') ."\n\n". t("The team at !site");

    case 'mc_notification_update_subject':
      return t('An exchange was edited on !site');

    case 'mc_notification_update_body':
      return t("Dear !username,") ."\n\n". t("For your information, this exchange recorded previously, has been edited:") ."\n\n!summary\n\n".
        t("The team at !site");
  }
}


function mc_mail_template() {
  $form['mc_notification_throttle_default'] = notification_throttle();
  $form['mc_notification_throttle_default']['#weight'] = -6;
  $form['mc_notification_throttle_default']['#default_value'] = variable_get('mc_notification_throttle_default', 2);
  $form['mc_notification_option'] = array(
    '#title' => t('Allow user to choose notification frequency'),
    '#description' => t('Users will see the throttle below on user/%/edit page'),
    '#type' => 'checkbox',
    '#default_value' => variable_get('mc_notification_option', TRUE),
    '#weight' => -5
  );

  $form['mc_notification_wrapper'] = array(
    '#title' => t('New completed exchange notification template'),
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#weight' => -4
  );
  $form['mc_notification_wrapper']['mc_notification_subject'] = array(
    '#title' => t('Mail subject'),
    '#description' => t('Tokens may be used.'),
    '#type' => 'textfield',
    '#default_value' => _mcapi_default_mails('mc_notification_subject'),
    '#weight' => 1,
    '#required' => TRUE
  );
  $form['mc_notification_wrapper']['mc_notification_body'] = array(
    '#title' => t('Mail body'),
    '#description' => t('Tokens may be used.'),
    '#type' => 'textarea',
    '#default_value' => _mcapi_default_mails('mc_notification_body'),
    '#weight' => 2,
    '#required' => TRUE
  );

  $form['mc_notification_update_wrapper'] = array(
    '#title' => t('Updated exchanges notification template'),
    '#description' => t('not required for many use cases'),
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#weight' => -3
  );
  $form['mc_notification_update_wrapper']['mc_notification_update_subject'] = array(
    '#title' => t('Mail subject'),
    '#description' => t('Tokens may be used.'),
    '#type' => 'textfield',
    '#default_value' => _mcapi_default_mails('mc_notification_update_subject'),
    '#weight' => 1,
    '#required' => TRUE
  );
  $form['mc_notification_update_wrapper']['mc_notification_update_body'] = array(
    '#title' => t('Mail body'),
    '#description' => t('Tokens may be used.'),
    '#type' => 'textarea',
    '#default_value' => _mcapi_default_mails('mc_notification_update_body'),
    '#weight' => 2,
    '#required' => TRUE
  );
  return system_settings_form($form);
}


/*
 * updates the balances and sends the mail
 */
function mcapi_notify($op, $exchange) {
  foreach (array(user_load($exchange->payer_uid), user_load($exchange->payee_uid)) as $account) {
    //decide whether to send mail, according to system and user settings.
    $throttle = $account->mc_notification_throttle;
    if ($account->uid == $GLOBALS['user']->uid) {// for the current user
      if ($throttle < 3) continue; //to the next transactee
    }
    elseif ($throttle < 1) {//for the other user
      continue; 
    }
    //because this node might not be properly loaded.
    //We can't reload node at this stage because this mail operation is already called from a nodeapi 'insert/update'
    $exchange->taxonomy = taxonomy_node_get_terms($exchange);
    $message = drupal_mail(
      'mcapimail',
      $op,
      $account->mail,
      user_preferred_language($account),
      array(//params
        'recipient' => $account,
        'exchange' => $exchange,
        'send' => NULL //will be set elsewhere
      ),
      NULL, //sender defaults to site_mail
      TRUE //send by default
    );
  }
  if ($message['params']['send']) {
    $message['result'] = drupal_mail_send($message);
    // Log errors
    if (!$message['result']) {
      watchdog('mail', 'Error sending e-mail (from %from to %to).', array('%from' => $message['from'], '%to' => $message['to']), WATCHDOG_ERROR);
      drupal_set_message(t('Unable to send e-mail. Please contact the site administrator if the problem persists.'), 'error');
    }
  }
}

function mcapimail_mail($key, &$message, $params) {
  //Setting of 2 means that users don't want to be notified of their own activity.
  if ($key == 'delete') return;
  $subject = _mcapi_default_mails('mc_notification_subject');
  $body_template = $key == 'update' ? 'mc_notification_update_body' : 'mc_notification_body';
  $body = _mcapi_default_mails($body_template);
  $variables = mc_mail_tokens($params['recipient'], $message['language'], $params['exchange']);
  $message['subject'] = strtr($subject, $variables);
  $message['body'] = strtr($body, $variables);
}

function mc_mail_tokens($account, $language, $exchange) {//$account is the recipient of the mail
  $tokens = user_mail_tokens($account, $language);
  $creator = user_load($exchange->uid);
  $tokens['!exchange_uri'] = url('node/'. $exchange->nid, array('absolute' => TRUE));
  return $tokens+ array(
    '!creator' => $creator->name,
    '!summary' => strip_tags(theme('node', $exchange, TRUE, FALSE)),
  );
  //ADDENDUM
  //it seems that system_mail converts all mails to text, and mimemail doesn't compensate,
  //so any html is rendered literally
  if (module_exists('mimemail')) {
    return $tokens + array(
      '!creator' => theme('username', $creator),
      '!summary' => theme('node', $exchange, TRUE, FALSE),
    );
  }
}


function notification_throttle() {
  return array(
    '#title' => t('When to notify about exchanges via email'),
    '#description' => t('Set the default frequency of notifications'),
    '#type' => 'radios',
    '#options' => array(
      0 => t("Never"),
      1 => t("Only when action is needed"),
      2 => t('When affected by another user'),
      3 => t('For all events'),
    ),
  );
}

//this applies to subdivisions
function theme_currency_division_array($settings_array) {
  if (!is_array($settings_array)) return $settings_array;
  if (count($settings_array)) {
    foreach ($settings_array as $key => $row) {
      $rows[] = array($row);
    }
    return theme('table', array(), $rows);
  }
}

function theme_exchange_summary($node) {
  $submitted = isset($node->created) ? $node->created : time();
  $replacements = array(
    '!amount' => theme('money', $node->quantity, $node->cid),
    '@title' => isset($node->nid) ? l($node->title, 'node/'.$node->nid.'/title') : $node->title,
    '!payer' => theme('username', user_load($node->payer_uid)),
    '!payee' => theme('username', user_load($node->payee_uid)),
    '@submitted' => format_date($submitted, 'small'),
  );
  switch($node->state) {
    case EXCHANGE_STATE_PENDING:
      return t("On @submitted, !payer will pay !payee !amount for '@title'", $replacements);
    case EXCHANGE_STATE_FINISHED:
      return t("On @submitted, !payer gave !payee !amount for '@title'", $replacements);
    case EXCHANGE_STATE_DELETED:
      return t("On @submitted, !payer did not give !payee !amount for '@title'. (DELETED)'", $replacements);
  }
}
function get_trader_uids() {
  //can't use user_roles() here because it matches with mysql LIKE which gives inaccurate results
  $result = db_query("SELECT distinct(rid) FROM {permission} WHERE perm LIKE '%%exchange,%%' OR perm = 'exchange'");
  while ($rid = db_result($result)) {
    $rids[] = $rid;
  }
  $where = array('status = 1',  'u.uid > 0');

  if (count($rids)) {
    if (!in_array(DRUPAL_AUTHENTICATED_RID, $rids)) {
      $where[] = 'rid in ('. implode(', ', $rids) .')';
    }
    $results = db_query("SELECT DISTINCT u.uid
      FROM {users} u LEFT JOIN {users_roles} ur ON u.uid = ur.uid
      WHERE ". implode(' AND ', $where)
    );
    while ($uid = db_result($results)) {
      $uids[] = $uid;
    }
    return $uids;
  }
  else {
    drupal_set_message(
      t(
        "No roles have permission to '@trade'. Go to !link",
        array(
          '@trade' => t('exchange'),
          '!link' => l('admin/user/permissions', 'admin/user/permissions')
        ),
      'warning'
      )
    );
  }
}

function build_exchange_node($payer_uid, $payee_uid, $quantity, $options) {
  global $user;
  //although mcapi.module allows it, this version of the API will not allow zero value exchanges
  if (!$quantity) {
    drupal_set_message(t('Cannot create a zero value exchange'), 'error');
    return;
  }
  if (!isset($options['title'])) {
    $title = format_date(time(), 'short') . t('No details given');
  }

  $node = $options + array(
    'payee_uid' => $payee_uid,
    'payer_uid' => $payer_uid,
    'quantity' => $quantity,
   //default $options
    'cid' => array_pop(array_keys(currencies_load())),
    'title' => t('no description given'),  //shouldn't really be omitted
    'state' => EXCHANGE_STATE_FINISHED, //shouldn't really be omitted
    'exchange_type' => 'api unspecified', //shouldn't really be omitted
    'uid' => $user->uid,
    'name' => $user->name,
    'created' => time(),
    'data' => array(),
    'status' => FALSE,
   //essential node property
    'type' => 'exchange',
  );
  $node = (object)$node;
  mcapi_validate($node);
  return $node;
}
