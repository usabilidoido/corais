<?php

/**
 * Execute AMFPHP request
 */
function amfphp_server_exec() {
  $path = drupal_get_path('module', 'amfphp');
  
  define("PRODUCTION_SERVER", !variable_get('services_debug', FALSE));
	
	require_once $path . "/amfphp/globals.php";
	require_once $path . "/overrides/AmfphpGateway.php";

	$gateway = new AmfphpGateway();
	$gateway->setClassPath($servicesPath);
	$gateway->setClassMappingsPath($voPath); 
	$gateway->setCharsetHandler(variable_get('charset_method','utf8_decode'), variable_get('charset_php','ISO-8859-1'), variable_get('charset_sql','ISO-8859-1'));
	$gateway->setErrorHandling(variable_get('amfphp_exception_handler', E_ALL ^ E_NOTICE));
	
	if (PRODUCTION_SERVER) {
		$gateway->disableDebug();
		$gateway->disableStandalonePlayer();
	}

	$gateway->enableGzipCompression(25*1024);
	$gateway->service();
}

/*
 * ugly! ugly! ugly!
 * we need to use a method call wrapper here to convert all 'uid' values in the result
 * to 'userid'.  this is because flex uses the property 'uid' in objects and will overwrite
 * anything we send with its own value.
 */
function amfphp_method_call($method_name, $args) {
  
  // convert all userid to uid
  $args = amfphp_fix_uid($args, 0);
  
  $result = services_method_call($method_name, $args);
  
  // convert all uid to userid
  $result = amfphp_fix_uid($result);
  
  return $result;
}

/*
 * ugly! ugly! ugly!
 */
function amfphp_fix_uid($data, $direction = 1) {
  $uid    = 's:3:"uid";';
  $userid = 's:6:"userid";';
  
  $from = ($direction) ? $uid : $userid;
  $to   = (!$direction) ? $uid : $userid;
  
  $data = _amfphp_serialize_lite($data);
  $data = str_replace($from, $to, $data);
  $data = unserialize($data);
  
  return $data;
}

/*
 * Implementation of hook_server_error()
 */
function amfphp_server_error($message) {
  // log error to watchdog
  watchdog('amfphp server', $message, NULL, WATCHDOG_ERROR);
  
  // we must throw an error here as it is caught by AMFPHP to send faults back
  // to the client
  trigger_error($message, E_USER_ERROR);
}

/*
 * A less memory intesive serializer
 * see: http://bugs.php.net/bug.php?id=39736
 */
function _amfphp_serialize_lite(&$data, $buf = '') {
  if (is_array($data)) {
    $buf.= "a:" . count($data) . ":{";
    foreach($data as $key => &$value) {
      $buf .= serialize($key) . _amfphp_serialize_lite($value);
    }
    $buf.= "}";
    return $buf;
  }
  else {
    return $buf . serialize($data);
  }
}