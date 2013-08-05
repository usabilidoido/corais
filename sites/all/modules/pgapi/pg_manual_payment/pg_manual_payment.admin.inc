<?php

/********************************************************************
 * Functions callback
 ********************************************************************/

function pg_manual_payment_list() {
  $result = db_query('SELECT * FROM {pg_manual_payment_accounts} ORDER BY name');
  $form['#tree'] = TRUE;
  while ($template = db_fetch_object($result)){
    $form[$template->paid]['purse'] = array('#value' => $template->purse );
    $form[$template->paid]['rate'] = array('#value' => $template->rate );
    $form[$template->paid]['edit'] = array('#value' => l($template->name,'admin/pgdata/pgsettings/manual_payment/edit/'.$template->paid,array('query' => drupal_get_destination())));
    $purses[$template->paid]='';
   }//while
  $form['check']=array(
        '#type' => 'checkboxes',
        '#options' => $purses
      );
  $form['submit'] = array(
    '#type'  => 'submit',
    '#value' => t('Delete checked Accounts'),
  );

  return $form;
}

function pg_manual_payment_list_submit($form, &$form_state){
  foreach($form_state['values']['check'] as $key => $val) {
    if($key === $val){
      $pursed = db_fetch_object(db_query('SELECT * FROM {pg_manual_payment_accounts} WHERE paid = %d ',$paid));
      db_query("DELETE FROM {pg_manual_payment_accounts} WHERE paid = %d", $val);
      drupal_set_message(t('Your account %name has been deleted.',array('%name' => $pursed->name)));
    }
  }
}


function pg_manual_payment_edit(&$form_state, $paid){
  $pursed = db_fetch_object(db_query('SELECT * FROM {pg_manual_payment_accounts} WHERE paid = %d ',$paid));
  $form['paid'] = array(
    '#type'          => 'value',
    '#value'         => $paid
  );

  $form['name'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Name'),
    '#default_value' => $pursed->name,
    '#description'   => t("Please, enter Payment system name."),
    '#required'      => TRUE,
  );
  $form['purse'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Account Number'),
    '#default_value' => $pursed->purse,
    '#description'   => t("Please, enter Payment system account number."),
    '#required'      => TRUE,
  );
  $form['symbol'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Currency Symbol'),
    '#default_value' => $pursed->symbol,
    '#description'   => t("Please, enter Payment system Currency symbol."),
    '#required'      => TRUE,
  );

  $form['rate'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Currency rate'),
    '#default_value' => $pursed->rate,
    '#description'   => t("Please, enter Payment system Currency rate."),
    '#required'      => TRUE,
  );

  $form['submit'] = array(
    '#type'  => 'submit',
    '#value' => t('Save'),
  );

  return $form;
}

function pg_manual_payment_edit_submit($form, &$form_state){
  drupal_write_record('pg_manual_payment_accounts',$form_state['values'],'paid');
  drupal_set_message(t('Your Account %name has been modified.',array('%name' => $form_state['values']['name'])));
}


function pg_manual_payment_add() {
  $form['name'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Name'),
    '#default_value' => '',
    '#description'   => t("Please, enter Payment system name."),
    '#required'      => TRUE,
  );
  $form['purse'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Account Number'),
    '#default_value' => '',
    '#description'   => t("Please, enter Payment system account number."),
    '#required'      => TRUE,
  );
  $form['symbol'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Currency Symbol'),
    '#default_value' => '',
    '#description'   => t("Please, enter Payment system Currency symbol."),
    '#required'      => TRUE,
  );

  $form['rate'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Currency rate'),
    '#default_value' => '',
    '#description'   => t("Please, enter Payment system Currency rate."),
    '#required'      => TRUE,
  );

  $form['submit'] = array(
    '#type'  => 'submit',
    '#value' => t('Add Account'),
  );

  return $form;
}

function pg_manual_payment_add_submit($form, &$form_state){
  drupal_write_record('pg_manual_payment_accounts',$form_state['values']);
  drupal_set_message(t('Your Account %name has been added.',array('%name' => $form_state['values']['name'])));
}



function pg_manual_payment_settings() {

  $form['pg_manual_payment_email']=array(
    '#type'          => 'textfield',
    '#title'         => t("Admin email with notification emails"),
    '#default_value' => variable_get('pg_manual_payment_email',''),
    '#description'   => t("Please enter email address for receive a notify and warning messages."),
    '#required'      => TRUE,
  );


  return system_settings_form($form);
}
