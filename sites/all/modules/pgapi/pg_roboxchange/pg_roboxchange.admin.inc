<?php
// $Id: pg_roboxchange.admin.inc,v 1.4 2010/10/31 05:12:08 gor Exp $

/********************************************************************
 * Functions callback
 ********************************************************************/
function pg_roboxchange_settings() {

  if(FALSE === @include('XML/Unserializer.php')){
    drupal_set_message(t('You have to install !url, otherwise RoboXchange Gateway will not work.', array('!url' => l('XML/Serializer','http://pear.php.net/package/XML_Serializer'))),'error');
  }

  $form['pg_roboxchange_rate_url'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Rate url'),
    '#default_value' => variable_get('pg_roboxchange_rate_url',cRoboXchangeRateUrl),
    '#description'   => t("Please enter rate url.\nDefault: !url", array('!url' => cRoboXchangeRateUrl)),
  );
  $form['#tree'] = TRUE;
  
  $mrh=variable_get('pg_roboxchange_mrh','');
  if(empty($mrh)){
    $form['rate'] = array(
      '#type'  => 'fieldset',
      '#title' => t('Rate information'),
      '#description' => t('Please fill and save "Merchant Login" first'),
    );
  }else{
    pg_roboxchange_cron();
    $form['rate'] = array(
      '#type'  => 'fieldset',
      '#title' => t('Rate information'),
      '#theme' => 'pg_roboxchange_settings_table',
      '#description' => t('Please check all currency that you want to accept'),
    );
    $form['rate']['currency'] = array(
      '#type'  => 'markup',
      '#value' => variable_get('pg_roboxchange_out_curr',''),
    );
    $form['rate']['last_update'] = array(
      '#type'  => 'markup',
      '#value' => variable_get('pg_roboxchange_last_update',''),
    );
    $result = db_query('SELECT * FROM {pg_roboxchange_rates} ORDER BY type');
    $checked= variable_get('pg_roboxchange_checked','');
    while ($rr = db_fetch_object($result)){
      $form['rate']['type'][$rr->type] = array(
        '#type'  => 'checkbox',
        '#default_value' => $checked[$rr->type]
      );
      $form['rate']['name'][$rr->type] = array(
        '#type'  => 'markup',
        '#value' => $rr->name,
      );
  
      $form['rate']['rate'][$rr->type] = array(
        '#type'  => 'markup',
        '#value' => $rr->rate,
      );
    }

  }

  $form['pg_roboxchange_action_url'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Action url'),
    '#default_value' => variable_get('pg_roboxchange_action_url',cRoboXchangeActionUrl),
    '#description'   => t("Please enter action url.\nDefault: !url", array('!url' => cRoboXchangeActionUrl)),
  );
  $form['pg_roboxchange_mrh'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Merchant Login'),
    '#default_value' => variable_get('pg_roboxchange_mrh',''),
    '#description'   => t("Please enter Merchant Login."),
    '#required'      => TRUE,
  );
  $form['pg_roboxchange_mps1'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Merchant Pass 1'),
    '#default_value' => variable_get('pg_roboxchange_mps1',''),
    '#description'   => t("Please enter Merchant Login."),
    '#required'      => TRUE,
  );
  $form['pg_roboxchange_mps2'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Merchant Pass 2'),
    '#default_value' => variable_get('pg_roboxchange_mps2',''),
    '#description'   => t("Please enter Merchant Login."),
    '#required'      => TRUE,
  );

  $form['pg_roboxchange_rate'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Rate to site currency'),
    '#default_value' => variable_get('pg_roboxchange_rate','1.00'),
    '#description'   => t("Please enter RoboXchange rate according to site currency."),
  );


  $form['url'] = array(
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#title' => t('Urls(result,success,fail) for merchant interfase')
  );

  $form['url']['result'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Result url'),
    '#value' => url('roboxchange/done',array('absolute' => TRUE)),
    '#description'   => t("Just for copy."),
  );

  $form['url']['success'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Success url'),
    '#value' => url('roboxchange/success',array('absolute' => TRUE)),
    '#description'   => t("Just for copy."),
  );
  $form['url']['fail'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Fail url'),
    '#value' => url('roboxchange/fail',array('absolute' => TRUE)),
    '#description'   => t("Just for copy."),
  );

  $form['submit'] = array(
    '#type'  => 'submit',
    '#value' => t('Submit'),
  );

  return $form;
}

function pg_roboxchange_settings_validate($form, &$form_state) {
  if($form_state['values']['pg_roboxchange_rate']<=0){
    form_set_error('pg_roboxchange_rate', t('%rate must be more 0.', array('%rate' => $form_state['values']['pg_roboxchange_rate'])));
  }
}

function pg_roboxchange_settings_submit($form, &$form_state){

  if(isset($form_state['values']['pg_roboxchange_action_url']))
    variable_set('pg_roboxchange_action_url',$form_state['values']['pg_roboxchange_action_url']);

  if(isset($form_state['values']['pg_roboxchange_rate_url']))
    variable_set('pg_roboxchange_rate_url',$form_state['values']['pg_roboxchange_rate_url']);
    
  if(isset($form_state['values']['pg_roboxchange_mrh']))
    variable_set('pg_roboxchange_mrh',$form_state['values']['pg_roboxchange_mrh']);

  if(isset($form_state['values']['pg_roboxchange_mps1']))
    variable_set('pg_roboxchange_mps1',$form_state['values']['pg_roboxchange_mps1']);

  if(isset($form_state['values']['pg_roboxchange_mps2']))
    variable_set('pg_roboxchange_mps2',$form_state['values']['pg_roboxchange_mps2']);

  if(isset($form_state['values']['pg_roboxchange_rate']))
    variable_set('pg_roboxchange_rate',$form_state['values']['pg_roboxchange_rate']);

  if(isset($form_state['values']['rate']['type']))
    variable_set('pg_roboxchange_checked',$form_state['values']['rate']['type']);
}


function pg_roboxchange_done_payment (){

  $txnid  = $_POST['InvId'];
  $amount = $_POST['OutSum'];
  $hash   = strtoupper($_POST['SignatureValue']);

  $mps2= variable_get('pg_roboxchange_mps2','');
  $crc=strtoupper(md5($amount.':'.$txnid.':'.$mps2));

  $t=pgapi_transaction_load($txnid);
  $status="";
  if($t->extra['amount'] != $amount  ){
      $res = t('This number does not match the original price');
      $status = 'denied';
    }
  if($crc!=$hash ){
      $res = t('Security check failed');
      $status = 'denied';
  }
  if($status==""){
      $description = t('Payment was accepted.');
      $t->description=$description;
      $t->status = pgapi_get_status_id('completed');
      pgapi_transaction_save($t);
      $answer = "OK".$txnid;
  }else {
      $t->description=$res;
      $t->status = pgapi_get_status_id($status);
      pgapi_transaction_save($t);
      $answer = "ERR";
  }

  fwrite($fp, "answer:".$answer." code ".$crc);
  fwrite($fp, print_r($_GET,true));
  fclose($fp);
  echo $answer;
  exit();
}


function pg_roboxchange_payment_end ($type){
  global $user;
  $txnid = $_POST['inv_id'];
  $t=pgapi_transaction_load($txnid);
  $goto = pgapi_get_redirect_url($t);
  drupal_goto($goto);
}
