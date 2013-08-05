<?php

function pg_payflowpro_settings() {

  $form['pg_payflowpro_rate'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Rate to site currency'),
    '#default_value' => variable_get('pg_payflowpro_rate','1.00'),
    '#description'   => t("Please enter payflowpro rate according to site currency."),
  );

  $form['pg_payflowpro_currency'] = array(
    '#type'          => 'select',
    '#title'         => t('Currency'),
    '#default_value' => variable_get('pg_payflowpro_currency','RUR'),
    '#options'       => array('USD' => 'USD', 'GBP' => 'GBP', 'EUR' => 'EUR' , 'CAD' => 'CAD', 'JPY' => 'JPY', 'AUD' => 'AUD'),
    '#description'   => t("Please select payflowpro currency."),
  );

  $form['pg_payflowpro_mode'] = array(
    '#type' => 'radios',
    '#title' => t('Transaction Mode'),
    '#description' =>t('Transactions are run through the test server by default. Adjust to live transactions when you are ready to start processing real payments.'),
    '#default_value' => variable_get('pg_payflowpro_mode', 'test'),
    '#options' => array(
      'test' => t('Test transactions'),
      'live' => t('Live transactions'),
    ),
  );
  
  $form['pg_payflowpro_payment_action'] = array(
    '#type' => 'select',
    '#title' => t('Payment action'),
    '#description' => t('Complete Sale will authorize and capture the funds at the time the payment is processed.<br>Authorization will only reserve funds on the card to be captured later through your PayFlow account.'),
    '#options' => array(
      'S' => t('Complete Sale'),
      'A' => t('Authorization'),
    ),
    '#default_value' => variable_get('pg_payflowpro_payment_action', 'S'),
  );

  $form['pg_payflowpro_notification'] = array(
    '#type' => 'checkbox',
    '#title' => t('Send notification about payments'),
    '#description' => t('Site administrator will recieve email about every payflow transaction.'),
    '#default_value' => variable_get('pg_payflowpro_notification', 0),
  );

  $form['api'] = array(
    '#type' => 'fieldset',
    '#title' => t('API Credentials'),
    '#description' => t('PayFlow Pro account information (test this at manager.paypal.com)'),
  );

  $form['api']['pg_payflowpro_partner'] = array(
    '#type' => 'textfield',
    '#title' => t('Partner'),
    '#default_value' => variable_get('pg_payflowpro_partner', 'PayPal'),
    '#description' => t('The ID provided to you by the authorized PayPal Reseller who registered you for the Payflow Pro service.  If you purchased your account directly from PayPal, use PayPal.  Otherwise, it is typically Verisign.  This is case sensitive.'),
  );

  // PayFlow XMLPay Guide refers to this as vendor instead of merchant
  $form['api']['pg_payflowpro_vendor'] = array(
    '#type' => 'textfield',
    '#title' => t('Vendor'),
    '#default_value' => variable_get('pg_payflowpro_vendor', ''),
    '#description' => t('The vendor name for the PayFlow service.'),
  );

  $form['api']['pg_payflowpro_user'] = array(
    '#type' => 'textfield',
    '#title' => t('User'),
    '#default_value' => variable_get('pg_payflowpro_user', ''),
    '#description' => t('The user name for the PayFlow service (this is often the same as the Vendor name).'),
  );

  $form['api']['pg_payflowpro_password'] = array(
    '#type' => 'password',
    '#title' => t('Password'),
    '#default_value' => variable_get('pg_payflowpro_password', ''),
    '#description' => t('PayFlow password.'),
  );

  return system_settings_form($form);
}

function pg_payflowpro_settings_validate($element, &$form_state) {

  if (empty($form_state['values']['pg_payflowpro_partner']) || empty($form_state['values']['pg_payflowpro_vendor'])) {
    form_set_error('pg_payflowpro_partner', t('Payflow Pro Partner or Vendor are required fields.'));
  }
  if (empty($form_state['values']['pg_payflowpro_user'])) {
    // The web service requires this field, but manager.paypal.com does not.  Make the difference fairly transparent to the end user.
    $form_state['values']['pg_payflowpro_user'] = $form_state['values']['pg_payflowpro_vendor'];
  }
  $password = variable_get('pg_payflowpro_password', '');
  if (empty($form_state['values']['pg_payflowpro_password']) && !$password) {
    form_set_error('pg_payflowpro_password', t('Payflow Pro password is required.'));
  }
  elseif (empty($form_state['values']['pg_payflowpro_password']) && $password) {
    $form_state['values']['pg_payflowpro_password'] = $password;
  }
}
