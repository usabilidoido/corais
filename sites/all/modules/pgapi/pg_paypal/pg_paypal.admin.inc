<?php
// $Id: pg_paypal.admin.inc,v 1.1 2009/11/24 22:06:36 gor Exp $
/********************************************************************
 * Functions callback
 ********************************************************************/
function pg_paypal_settings() {
  $form['pg_paypal_action_url'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Action url'),
    '#default_value' => variable_get('pg_paypal_action_url',cPaypalActionUrl),
    '#description'   => t("Please enter action url.\nDefault: !url", array('!url' => cPaypalActionUrl)),
  );
  $form['pg_paypal_validation_url'] = array(
    '#type'          => 'textfield',
    '#title'         => t('PayPal IPN "request back" validation URL'),
    '#default_value' => variable_get('pg_paypal_validation_url',cPaypalValidationUrl),
    '#description'   => t("URL where IPN 'request back' is posted to validate the transaction.\nDefault: !url", array('!url' => cPaypalValidationUrl)),
  );
  $form['pg_paypal_email'] = array(
    '#type'          => 'textfield',
    '#title'         => t('PayPal Receiver Email'),
    '#default_value' => variable_get('pg_paypal_email',''),
    '#description'   => t("Primary email address of the payment recipent. This is also your main paypal email address."),
  );
  $currency_codes = array(
    'USD' => 'USD - U.S. Dollars',
    'EUR' => 'EUR - Euros',
    'AUD' => 'AUD - Australian Dollars',
    'CAD' => 'CAD - Canadian Dollars',
    'GBP' => 'GBP - Pounds Sterling',
    'JPY' => 'JPY - Yen',
    'NZD' => 'NZD - New Zealand Dollar',
    'CHF' => 'CHF - Swiss Franc',
    'HKD' => 'HKD - Hong Kong Dollar',
    'SGD' => 'SGD - Singapore Dollar',
    'SEK' => 'SEK - Swedish Krona',
    'DKK' => 'DDK - Danish Krone',
    'PLN' => 'PLN - Polish Zloty',
    'NOK' => 'NOK - Norwegian Krone',
    'HUF' => 'HUF - Hungarian Forint',
    'CZK' => 'CZK - Czech Koruna'
  );
  $form['pg_paypal_currency_code'] = array(
    '#type'          => 'select',
    '#title'         => t('Currency code'),
    '#options'       => $currency_codes,
    '#default_value' => variable_get('pg_paypal_currency_code','USD'),
    '#description'   => t("The currecy code that PayPal should process the payment in."),
  );

  $form['pg_paypal_rate'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Rate to site currency'),
    '#default_value' => variable_get('pg_paypal_rate','1.00'),
    '#description'   => t("Please enter Paypal rate according to site currency."),
  );

  $form['submit'] = array(
    '#type'  => 'submit',
    '#value' => t('Submit'),
  );

  return $form;
}

function pg_paypal_settings_validate($form, &$form_state) {
  if($form_state['values']['pg_paypal_rate']<=0){
    form_set_error('pg_paypal_rate', t('%rate must be more 0.', array('%rate' => $form_state['values']['pg_paypal_rate'])));
  }
}

function pg_paypal_settings_submit($form, &$form_state){

  if(isset($form_state['values']['pg_paypal_action_url']))
    variable_set('pg_paypal_action_url',$form_state['values']['pg_paypal_action_url']);
  if(isset($form_state['values']['pg_paypal_validation_url']))
    variable_set('pg_paypal_validation_url',$form_state['values']['pg_paypal_validation_url']);
  if(isset($form_state['values']['pg_paypal_email']))
    variable_set('pg_paypal_email',$form_state['values']['pg_paypal_email']);
  if(isset($form_state['values']['pg_paypal_currency_code']))
    variable_set('pg_paypal_currency_code',$form_state['values']['pg_paypal_currency_code']);
  if(isset($form_state['values']['pg_paypal_rate']))
    variable_set('pg_paypal_rate',$form_state['values']['pg_paypal_rate']);

}


function pg_paypal_done_payment (){

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

  $request = drupal_http_request(variable_get('pg_paypal_validation_url', cPaypalValidationUrl), $headers, 'POST', _query_string_encode($post, array('q')));
  
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
    
  $currency=variable_get('pg_paypal_currency_code','USD');
   
  if($currency != $payment_currency  ){
      $description = t('This currency does not match with the original currency');
      $status = 'denied';
    }

  $email=variable_get('pg_paypal_email','');

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
