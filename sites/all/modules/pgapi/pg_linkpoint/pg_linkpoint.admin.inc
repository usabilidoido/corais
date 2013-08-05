<?php

/********************************************************************
 * Functions callback
 ********************************************************************/
function pg_linkpoint_settings() {
  $form['pg_linkpoint_action_url'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Action url'),
    '#default_value' => variable_get('pg_linkpoint_action_url',cLinkPointGAPIURL),
    '#description'   => t("Please enter action url.\nDefault: !url", array('!url' => cLinkPointGAPIURL)),
  );

  $form['pg_linkpoint_login_id'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Store Number'),
    '#default_value' => variable_get('pg_linkpoint_login_id', ''),
    '#description'   => t("Enter your Store Number assigned to you by Linkpoint."),
    '#required'      => TRUE,
  );
  $form['pg_linkpoint_transaction_key_file'] = array(
    '#type'        => 'file',
    '#title'       => t('PEM File'),
    '#description' => t('Upload your PEM file. You must have this for Linkpoint API to work. Get this by logging in to <a target="_blank" href="http://www.linkpointcentral.com">Linkpoint Central</a> > Support > Download Center'),
  );
  $fid = variable_get('pg_linkpoint_transaction_key_fid', '');
  if(!empty($fid)){
    $file=db_fetch_object(db_query("SELECT * FROM {files} WHERE fid = '%d'", $fid));
    $form['pg_linkpoint_transaction_key'] = array(
      '#type' => 'markup',
      '#value' => t('<b>Current file is:</b> !file',array('!file' => $file->filepath))
    );
  };

  $form['pg_linkpoint_transaction_mode'] = array(
    '#type'    => 'select',
    '#title'   => t('Transaction mode'),
    '#description' => t('Transaction mode used for processing orders.'),
    '#options' => array(
      'LIVE' => t('Production'),
      'GOOD' => t('Test'),
    ),
    '#default_value' => variable_get('pg_linkpoint_transaction_mode', 'GOOD'),
  );
  $form['pg_linkpoint_transaction_method'] = array(
    '#type' => 'radios',
    '#title' => t('Transaction Type'),
    '#default_value' => variable_get('pg_linkpoint_transaction_method', 'PREAUTH'),
    '#options' => array(
      'PREAUTH' => t('PREAUTH'),
      'SALE' => t('SALE'),
    ),
    '#description' => t('Linkpoint states for tangible items or anything that will be shipped to always use PREAUTH. If you are selling a download item etc, use SALE. PREAUTH authorizes a credit card but does not charge it. Go to <a target="_blank" href="http://www.linkpointcentral.com">Linkpoint Central</a> > Reports > Transctions to complete the transaction. SALE automatically charges the card.'),
  );

  $form['pg_linkpoint_rate'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Rate to site currency'),
    '#default_value' => variable_get('pg_linkpoint_rate','1.00'),
    '#description'   => t("Please enter linkpoint rate according to site currency."),
  );

  $form['submit'] = array(
    '#type'  => 'submit',
    '#value' => t('Submit'),
  );
  $form['#attributes']['enctype'] = 'multipart/form-data';

  return $form;
}


function pg_linkpoint_settings_submit($form, &$form_state){
  if(isset($form_state['values']['pg_linkpoint_action_url']))
    variable_set('pg_linkpoint_action_url',$form_state['values']['pg_linkpoint_action_url']);
  if(isset($form_state['values']['pg_linkpoint_login_id']))
    variable_set('pg_linkpoint_login_id',$form_state['values']['pg_linkpoint_login_id']);
  if(isset($form_state['values']['pg_linkpoint_transaction_mode']))
    variable_set('pg_linkpoint_transaction_mode',$form_state['values']['pg_linkpoint_transaction_mode']);
  if(isset($form_state['values']['pg_linkpoint_transaction_method']))
    variable_set('pg_linkpoint_transaction_method',$form_state['values']['pg_linkpoint_transaction_method']);

  if(isset($form_state['values']['pg_linkpoint_rate']))
    variable_set('pg_linkpoint_rate',$form_state['values']['pg_linkpoint_rate']);

  $tmp_path=file_directory_path() .'/'. variable_get('pg_linkpoint_path', 'pg_linkpoint');
  file_check_directory($tmp_path, FILE_CREATE_DIRECTORY);

  if($file = file_save_upload('pg_linkpoint_transaction_key_file', '', $tmp_path)){
    $fid = variable_get('pg_linkpoint_transaction_key_fid', '');
    if(!empty($fid)){
      $file_old=db_fetch_object(db_query("SELECT * FROM {files} WHERE fid = '%d'", $fid));
      db_fetch_object(db_query("DELETE FROM {files} WHERE fid = '%d'", $fid));
      file_delete($file->filepath);
    }
    file_set_status($file, FILE_STATUS_PERMANENT);
    variable_set('pg_linkpoint_transaction_key_fid', $file->fid);
  }
}
