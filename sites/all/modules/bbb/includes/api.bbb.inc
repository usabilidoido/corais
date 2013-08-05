<?php

/**
 * @file
 * Big Blue Button - Enables universities and colleges to deliver a high-quality
 * learning experience.
 *
 * @author
 * Stefan Auditor <stefan.auditor@erdfisch.de>
 */

/**
 * Parse xml response
 *
 * @param XML
 * @return OBJECT
 */
function bbb_api_parse_response($xml) {
  //TODO: Refactor
  $response = new StdClass;
  if ($xml) {
    foreach ($xml as $element => $node) {
      $response->$element = (string) $node;
    }
  }
  return $response;
}

/**
 * Custom debugging
 */
function bbb_api_debug($var) {
  if (BBB_API_DEBUG) {
    if (module_exists('devel')) {
      dsm($var);
    }
    watchdog('big blue button debug', '%message', array('%message' => '<pre>' . print_r($var, 1) . '</pre>'), WATCHDOG_DEBUG);
  }
}
