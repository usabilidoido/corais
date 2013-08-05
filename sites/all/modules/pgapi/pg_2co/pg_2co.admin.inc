<?php

/********************************************************************
 * Functions callback
 ********************************************************************/
function pg_2co_settings() {
  $form['pg_2co_action_url'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Action url'),
    '#default_value' => variable_get('pg_2co_action_url',c2coActionUrlSingle),
    '#description'   => t("Please enter action url.\nDefault: !url", array('!url' => c2coActionUrlSingle)),
  );
  $form['pg_2co_sid'] = array(
    '#type' => 'textfield',
    '#title' => t('Vendor account number'),
    '#description' => t('Your 2Checkout vendor account number.'),
    '#size' => 16,
    '#default_value' => variable_get('pg_2co_sid',''),
  );

  $form['pg_2co_secret_word'] = array(
    '#type' => 'textfield',
    '#title' => t('Secret word for order verification'),
    '#description' => t('The secret word entered in your 2Checkout account Look and Feel settings.'),
    '#size' => 16,
    '#default_value' => variable_get('pg_2co_secret_word','tango'),
  );


  $form['pg_2co_demo'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable demo mode, allowing you to process fake orders for testing purposes.'),
    '#default_value' => variable_get('pg_2co_demo', TRUE),
  );

  $form['pg_2co_language'] = array(
    '#type' => 'select',
    '#title' => t('Language preference'),
    '#description' => t('Adjust language on 2Checkout pages.'),
    '#options' => array(
      'en' => t('English'),
      'sp' => t('Spanish'),
    ),
    '#default_value' => variable_get('pg_2co_language', 'en'),
  );
  $form['pg_2co_rate'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Rate to site currency'),
    '#default_value' => variable_get('pg_2co_rate','1.00'),
    '#description'   => t("Please enter 2co rate according to site currency."),
  );


  $form['submit'] = array(
    '#type'  => 'submit',
    '#value' => t('Submit'),
  );

  return $form;
}

function pg_2co_settings_validate($form, &$form_state) {
  if($form_state['values']['pg_2co_rate']<=0){
    form_set_error('pg_2co_rate', t('%rate must be more 0.', array('%rate' => $form_state['values']['pg_2co_rate'])));
  }
}

function pg_2co_settings_submit($form, &$form_state){

  if(isset($form_state['values']['pg_2co_action_url']))
    variable_set('pg_2co_action_url',$form_state['values']['pg_2co_action_url']);
  if(isset($form_state['values']['pg_2co_sid']))
    variable_set('pg_2co_sid',$form_state['values']['pg_2co_sid']);
  if(isset($form_state['values']['pg_2co_secret_word']))
    variable_set('pg_2co_secret_word',$form_state['values']['pg_2co_secret_word']);
  if(isset($form_state['values']['pg_2co_demo']))
    variable_set('pg_2co_demo',$form_state['values']['pg_2co_demo']);
  if(isset($form_state['values']['pg_2co_language']))
    variable_set('pg_2co_language',$form_state['values']['pg_2co_language']);
  if(isset($form_state['values']['pg_2co_check']))
    variable_set('pg_2co_check',$form_state['values']['pg_2co_check']);
  if(isset($form_state['values']['pg_2co_rate']))
    variable_set('pg_2co_rate',$form_state['values']['pg_2co_rate']);

}


function pg_2co_done_payment (){
  $handle = fopen('/tmp/paypal.txt', 'a');
  $C = "First";
  $C .= print_r($_POST,TRUE);
  fwrite($handle, $C);
  fclose($handle);

  $txnid            = $_POST['item_number'];
  $payment_status   = check_plain($_POST['payment_status']);
  $payment_amount   = check_plain($_POST['mc_gross']);
  $payment_currency = check_plain($_POST['mc_currency']);
  $receiver_email   = check_plain($_POST['receiver_email']);
  $payer_email      = check_plain($_POST['payer_email']);
  $receiver_email   = check_plain($_POST['receiver_email']);

  $headers = array(
    'Content-Type' => 'application/x-www-form-urlencoded',
  );
  $ipn = $_POST;
  $post = array(
    'cmd' => '_notify-validate',
  );
  $post+= $ipn;

  $request = drupal_http_request(variable_get('pg_2co_validation_url', cPaypalValidationUrl), $headers, 'POST', _query_string_encode($post, array('q')));

  $description = t('Payment has been received.');
  $status="completed";

  if ($request->data != 'VERIFIED') {
      $handle = fopen('/tmp/paypal.txt', 'a');
      $C = "Answer";
      $C .= print_r($request,TRUE);
      fwrite($handle, $answer);
      fclose($handle);
      $res = t('Verification has been failed');
      $status = 'denied';
  }

  $t=pgapi_transaction_load($txnid);

  if($t->extra['amount'] != $payment_amount  ){
      $description = t('This amount does not match with the original price');
      $status = 'denied';
    }

  $currency=variable_get('pg_2co_currency_code','USD');

  if($currency != $payment_currency  ){
      $description = t('This currency does not match with the original currency');
      $status = 'denied';
    }

  $email=variable_get('pg_2co_email','');

  if($email != $receiver_email  ){
      $description = t('This receiver email does not match with the original email');
      $status = 'denied';
    }

  $t->description=$description;
  $t->status = pgapi_get_status_id($status);
  pgapi_transaction_save($t);

}

/********************************************************************
 * Other Functions
 ********************************************************************/
function _query_string_encode($query, $exclude = array(), $parent = '') {
  $params = array();

  foreach ($query as $key => $value) {
    $key = urlencode($key);
    if ($parent) {
      $key = $parent .'['. $key .']';
    }

    if (in_array($key, $exclude)) {
      continue;
    }

    if (is_array($value)) {
      $params[] = _query_string_encode($value, $exclude, $key);
    }
    else {
      $params[] = $key .'='. urlencode($value);
    }
  }

  return implode('&', $params);
}
