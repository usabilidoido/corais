<?php

/**
 * @file
 *   Provides page callbacks for menu
 */

/**
 * Page callback for payment/% page
 *
 * Return form with payment methods
 *
 * @param  $form_state
 *   State of form
 * @param  $transaction
 *   Current transaction
 * @return form
 */
function pgapi_payment(&$form_state, $transaction) {
  global $user;

  // If someone tries to view not own page before transaction status is pending or workflow is recieved - show access denied page
  if(($user->uid != 1 && $user->uid != $transaction->uid) || ($transaction->status != PG_PENDING) || ($transaction->workflow != PG_WORKFLOW_RECEIVED)){
    drupal_access_denied();
  }

  // Get array with enabled payment systems
  $pgapi_gw = pgapi_get_enabled_payment_systems();
  $default  = '';
  $options  = array();

  foreach ($pgapi_gw as $module => $checked) {
    if ($checked) {
      // Get data about payment gateway from hook_pgapi_gw()
      $payment_data = module_invoke($module, 'pgapi_gw', 'payment gateway info', $transaction->amount, $transaction);
      if ($payment_data) {
        if($transaction->method == $module){
          $default = $transaction->method;
        }
        else {
          $default = $module;
        }
        $form[$module]['modulename'] = array(
          '#value' => $payment_data['name'],
        );
        $options[$module] = '';

        $form[$module]['price'] = array(
          '#value' => theme('pgapi_gateway_price', $payment_data['price'], $transaction->type),
        );
      }
    }

    $form['method'] = array(
      '#type'    => 'radios',
      '#options' => $options,
      '#default_value' => $default,
      '#ahah'    => array(
        'path'     => 'payment/form_load_js/' . $transaction->txnid,
        'wrapper'  => 'pgapi-wrapper-form',
        'method'   => 'replace',
        'effect'   => 'slide',
        'progress' => array(
          'type'     => 'throbber',
          'message'  => t('Please wait...')
        ),
      ),
    );
  }

  $form['pgapi_wrapper'] = array(
    '#prefix' => '<div id = "pgapi-wrapper-form">',
    '#suffix' => '</div>',
    '#tree' => TRUE,
  );

  // If checked default payment system - load special form for it
  if ($default) {
    $extra_form = module_invoke($default, 'pgapi_gw','get form', $transaction, $form_state);
    if (is_array($extra_form)) {
      $form['pgapi_wrapper'] += $extra_form;
    }
  }

  $form['#tree'] = TRUE;

  $form['txnid'] = array(
    '#type'  => 'value',
    '#value' => $transaction->txnid,
  );

  $form['submit'] = array(
    '#type'  => 'submit',
    '#value' => t('Process purchase'),
  );

  $form['cancel'] = array(
    '#type'  => 'submit',
    '#value' => t('Cancel purchase'),
  );

  return $form;
}

/**
 * Validate function for payment form.
 */
function pgapi_payment_validate($form, &$form_state) {
  $values = $form_state['values'];
  if ($values['op'] == $values['submit']) {
    $transaction = pgapi_transaction_load($values['txnid']);
    module_invoke($values['method'], 'pgapi_gw','validate form', $transaction, $values['pgapi_wrapper']);
  }
}
 

/**
 * Submit function for payment form
 * Process transaction and redirect to payment system page
 */
function pgapi_payment_submit($form, &$form_state) {
  $values = $form_state['values'];
  if ($values['txnid']) {
    // Load transaction by ID
    $transaction = pgapi_transaction_load($values['txnid']);
    if ($values['op'] == $values['submit']){
      // If user clicked "Submit" button - process transcaction
      $transaction = module_invoke($values['method'], 'pgapi_gw','process form', $transaction , $values['pgapi_wrapper']);
      $form_state['redirect'] = pgapi_get_redirect_url($transaction);
    }
    elseif($values['op'] == $values['cancel']){
      // If user clicked "Cancel" button - cancel transaction
      $transaction->status = PG_CANCELED ;
      $transaction->workflow = PG_WORKFLOW_CANCELED;
      $transaction->description = t('User has canceled payment');
      pgapi_transaction_save($transaction);
      $form_state['redirect'] = pgapi_get_redirect_url($transaction);
    }
  }
}

/**
 * Page callback for AJAX
 * Loads when user click on radio button on payment/% page
 */
function pgapi_extra_form_load_js($transaction){
  global $user;

  $cached_form_state = array();

  // Load the form from the cache.
  $cached_form = form_get_cache($_POST['form_build_id'], $cached_form_state);

  if(!empty($cached_form) && (isset($cached_form['pgapi_fieldset']['pgapi_wrapper']) || isset($cached_form['pgapi_wrapper']))) {

    $form_state = array('values' => $_POST);
    $form = module_invoke($_POST['method'], 'pgapi_gw','get form', $transaction, $form_state);

    if(isset($cached_form['pgapi_wrapper'])) {
      $cached_form['pgapi_wrapper'] = array_merge($cached_form['pgapi_wrapper'], $form);
      form_set_cache($_POST['form_build_id'], $cached_form, $cached_form_state);
    }
    else{
      $cached_form['pgapi_fieldset']['pgapi_wrapper'] = array_merge($cached_form['pgapi_fieldset']['pgapi_wrapper'], $form);
      form_set_cache($_POST['form_build_id'], $cached_form, $cached_form_state);
    }

  }
  else{
    form_set_error('form_token', t('Validation error, please try again. If this error persists, please contact the site administrator.'));
    print drupal_to_js(array('status' => TRUE, 'data' => theme('status_messages')));
    exit();
  }

  $form += array(
    '#post' => $_POST,
    '#programmed' => FALSE,
    '#tree' => TRUE,
    '#parents' => array('pgapi_wrapper'),
  );

  $form_state = array('submitted' => FALSE);
  $form = form_builder('pgapi_extra_form', $form, $form_state);

  $setting = '';
  $javascript = drupal_add_js(NULL, NULL, 'header');
  if(is_array($javascript['setting'])){
    $embed_prefix = "\n<!--//--><![CDATA[//><!--\n";
    $embed_suffix = "\n//--><!]]>\n";
    $setting =
      '<script type="text/javascript">' . $embed_prefix . 'jQuery.extend(Drupal.settings, ' .
      drupal_to_js(call_user_func_array('array_merge_recursive', $javascript['setting'])) .");" . $embed_suffix . "</script>\n";
  }
  $output = $setting . theme('status_messages') . drupal_render($form);
  print drupal_to_js(array('status' => TRUE, 'data' => $output));
  exit();
}
