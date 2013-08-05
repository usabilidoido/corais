<?php
/********************************************************************
 * Functions callback
 ********************************************************************/


function pg_rbkmoney_done_payment (){
/*  $handle = fopen('/tmp/pg_rbkmoney.txt', 'a');
  $C .= print_r($_POST,TRUE);
  fwrite($handle, $C);
  fclose($handle);*/
  
  drupal_set_header('Content-type: text/html; charset=iso-8859-1');

  $txnid              = $GLOBALS['_POST']['orderId'];
  $eshopId            = $GLOBALS['_POST']['eshopId'];
  $paymentId          = $GLOBALS['_POST']['paymentId'];
  $eshopAccount       = $GLOBALS['_POST']['eshopAccount'];
  $serviceName        = $GLOBALS['_POST']['serviceName'];
  $recipientAmount    = $GLOBALS['_POST']['recipientAmount'];
  $recipientCurrency  = $GLOBALS['_POST']['recipientCurrency'];
  $paymentStatus      = $GLOBALS['_POST']['paymentStatus'];
  $userName           = $GLOBALS['_POST']['userName'];
  $userEmail          = $GLOBALS['_POST']['userEmail'];
  $paymentData        = $GLOBALS['_POST']['paymentData'];
  $hash               = $GLOBALS['_POST']['hash'];

  $secretKey=variable_get('pg_rbkmoney_secretkey','');
  
  $tocheck = md5($eshopId .'::'.$txnid.'::'.$serviceName.'::'.$eshopAccount.'::'.$recipientAmount.'::'.$recipientCurrency.'::'.$paymentStatus.'::'.$userName.'::'.$userEmail.'::'.$paymentData.'::'.$secretKey);

  $t=pgapi_transaction_load($txnid);

  $stored_price=round($t->extra['amount'],2);

  $res = 'OK';
  
  if($recipientAmount != $recipientAmount ){
      $res = t('This number does not match the original price');
      $status = 'failed';      
  }
  
  if(!empty($hash) ){
    if($hash != $tocheck) {
      $res = t('Security check failed');
      $status = 'denied'; 
    }
  }
  if($paymentStatus == 5){
	  if($res == 'OK'){
	      $description = t('Payment from %User %Email was accepted. PaymentId: !paymentId', array('%User' => $userName, '%Email' => $userEmail, '!paymentId' => $paymentId));
	      $t->description=$description;
	      $t->status = pgapi_get_status_id('completed');
	      pgapi_transaction_save($t);
	  }else{
	    $t->description=$res;
	    $t->status = pgapi_get_status_id($status);
	    pgapi_transaction_save($t);
	  }
  }
  echo $res;
}

function pg_rbkmoney_settings() {

  $form['pg_rbkmoney_rate'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Rate to site currency'),
    '#default_value' => variable_get('pg_rbkmoney_rate','1.00'),
    '#description'   => t("Please enter rbkmoney rate according to site currency."),
  );

  $form['pg_rbkmoney_action'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Action url'),
    '#default_value' => variable_get('pg_rbkmoney_action_url',crbkmoneyPayActionUrl),
    '#description'   => t("Please enter action url.\nDefault: !url", array('!url' => crbkmoneyPayActionUrl)),
  );

  $form['pg_rbkmoney_eshopId'] = array(
    '#type'          => 'textfield',
    '#title'         => t('eshopId'),
    '#default_value' => variable_get('pg_rbkmoney_eshopId',''),
    '#required'      => TRUE,
    '#description'   => t("Please enter rbkmoney eshopId."),
  );

  $form['pg_rbkmoney_secretkey'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Secret Key'),
    '#default_value' => variable_get('pg_rbkmoney_secretkey',''),
    '#description'   => t("Please enter rbkmoney purse secret key."),
  );
  
  $form['pg_rbkmoney_currency'] = array(
    '#type'          => 'select',
    '#title'         => t('Currency'),
    '#default_value' => variable_get('pg_rbkmoney_currency','RUR'),
    '#options'       => array('USD' => 'USD', 'RUR' => 'RUR', 'EUR' => 'EUR' , 'UAH' => 'UAH'),
    '#description'   => t("Please select rbkmoney currency."),
  );

  return system_settings_form($form);
}
