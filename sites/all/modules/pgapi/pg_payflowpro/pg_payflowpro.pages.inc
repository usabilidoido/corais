<?php

function pg_payflowpro_done_payment() {

  watchdog('test', print_r($_POST, 1), array(), WATCHDOG_INFO);

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

  $secretKey=variable_get('pg_payflowpro_secretkey','');

  $tocheck = md5($eshopId .'::'.$txnid.'::'.$serviceName.'::'.$eshopAccount.'::'.$recipientAmount.'::'.$recipientCurrency.'::'.$paymentStatus.'::'.$userName.'::'.$userEmail.'::'.$paymentData.'::'.$secretKey);

  $t = pgapi_transaction_load($txnid);

  $stored_price=round($t->extra['amount'], 2);

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

  if ($paymentStatus == 5) {
	  if ($res == 'OK') {
	      $description = t('Payment from %User %Email was accepted. PaymentId: !paymentId', array('%User' => $userName, '%Email' => $userEmail, '!paymentId' => $paymentId));
	      $t->description=$description;
	      $t->status = pgapi_get_status_id('completed');
	      pgapi_transaction_save($t);
	  }
    else {
	    $t->description=$res;
	    $t->status = pgapi_get_status_id($status);
	    pgapi_transaction_save($t);
	  }
  }
  echo $res;
}
