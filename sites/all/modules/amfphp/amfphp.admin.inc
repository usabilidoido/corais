<?php
/**
 * Admin settings callback.
 */
function amfphp_settings() {
  // Charsets
  $form['charset'] = array(
    '#type' => 'fieldset',
    '#title' => t('Charset Encodings'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
  );
  $form['charset']['charset_method'] = array(
    '#type' => 'select',
    '#title' => t('Charset method'),
    '#options' => array('utf8_decode','iconv','mbstring','recode','none'),
    '#default_value' => variable_get('charset_method','iconv'),
  );
  $form['charset']['charset_php'] = array(
    '#type' => 'select',
    '#title' => t('Charset PHP'),
    '#options' => array('ISO-8859-1','UTF-8','big5','CP950','Shift_JIS','CP932','CP949'),
    '#default_value' => variable_get('charset_php','UTF-8'),
  );
  $form['charset']['charset_sql'] = array(
    '#type' => 'select',
    '#title' => t('Charset SQL'),
    '#options' => array('ISO-8859-1','UTF-8','big5','CP950','Shift_JIS','CP932','CP949'),
    '#default_value' => variable_get('charset_sql','ISO-8859-1'),
  );
  
  // Exceptions  
  $form['exceptions'] = array(
    '#type' => 'fieldset',
    '#title' => t('Exception Settings'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
  );
  $form['exceptions']['amfphp_exception_handler'] = array(
    '#type' => 'select',
    '#title' => t('Select PHP error threshold'),
    '#default_value' => variable_get('amfphp_exception_handler', E_ALL ^ E_NOTICE),
    '#options' => array(
      E_ALL ^ E_NOTICE => t('E_ALL ^ E_NOTICE'),
      E_ERROR => t('E_ERROR'),
    ),
    '#description' => t('Configure the PHP error types for which AMFPHP will throw exceptions'),
  );
  
  return system_settings_form($form);
}