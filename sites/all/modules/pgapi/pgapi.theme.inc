<?php

/**
 * @file
 *   Provides theme functions
 */

/**
 * Theme function for PGAPI common settings form
 * Process form and output it as table
 */
function theme_pgapi_common($form) {
  // Set table headers
  $header = array(
    t('Active'),
    t('Payment Gateway'),
  );

  // Create table from rendered form elements
  $rows = array();
  foreach (element_children($form) as $key) {
    if (is_array($form[$key]['checkbox'])) {
      $row    = array();
      $row[]  = drupal_render($form[$key]['checkbox']);
      $row[]  = drupal_render($form[$key]['modulename']);
      $rows[] = $row;
    }
  }
  // If no payment gateways - show message
  if(!$rows){
    $rows[] = array(array('data' => t('No payment gateway available.'), 'colspan' => '2'));
  }

  // Return form table and render remain part of form
  $output = theme('table', $header, $rows);
  $output .= drupal_render($form);
  return $output;
}

/**
 * Theme function for PGAPI payment form
 * Process form and output it as table
 */
function theme_pgapi_payment($form) {
  // Set table headers
  $header = array(
    '',
    t('Payment Gateway'),
    t('Price')
  );

  // Create table from rendered form elements
  $rows = array();
  foreach ($form as $name => $element) {
    if (is_array($form[$name]['modulename'])) {
      $row    = array();
      $row[]  = drupal_render($form['method'][$name]);
      $row[]  = drupal_render($form[$name]['modulename']);
      $row[]  = drupal_render($form[$name]['price']);
      $rows[] = $row;
    }
  }
  // If no payment gateways - show message
  if (!$rows) {
    $rows[] = array(array('data' => t('No payment gateway available.'), 'colspan' => '3'));
  }

  // Return form table and render remain part of form
  $output = theme('table', $header, $rows);
  $output .= drupal_render($form);
  return $output;
}


/**
 * Theme function for PGAPI gateway select form
 * Process form and output it as table
 */
function theme_pgapi_gateway_select_form($form) {
  // Set table headers
  $header = array(
    '',
    t('Payment Gateway'),
    t('Price'),
  );

  // Create table from rendered form elements
  $rows = array();
  foreach ($form as $name => $element) {
    if (is_array($form[$name]['modulename'])) {
      $row    = array();
      $row[]  = drupal_render($form['method'][$name]);
      $row[]  = drupal_render($form[$name]['modulename']);
      $row[]  = drupal_render($form[$name]['price']);
      $rows[] = $row;
    }
  }
  // If no payment gateways - show message
  if(!$rows){
    $rows[] = array(array('data' => t('No payment gateway available.'), 'colspan' => '3'));
  }

  // Return form table and render remain part of form
  $output  = drupal_render($form['message']);
  $output .= theme('table', $header, $rows);
  $output .= drupal_render($form);
  return $output;

}

/**
 * Theme function for payment system price
 *
 * @param  $prices
 *   Array with prices to format
 * @param  $type
 *   Payment system name
 * @return formatted|string
 */
function theme_pgapi_gateway_price($prices, $type) {
  $output = '';
  if (is_array($prices)) {
    foreach ($prices as $cur => $gross) {
      if (is_array($gross)) {
        $output .= "<div style='float:left;'>" . $cur . "&nbsp</div>" . pgapi_format_price($type, $gross['price'], $gross['currency']);
      }
      else {
        $output .= pgapi_format_price($type, $gross, $cur);
      }
    }
  }
  return $output;
}

/**
 * Theme function for formating price with html
 *
 * @param  $price
 *   Price to format
 * @param  $symbol
 *   Symbol of currency
 * @param bool $position
 *   Position of price and it symbol
 * @return string
 *   Formated price
 */
function theme_pgapi_format_price($price, $symbol, $position = FALSE) {
  if ($position) {
    $output = '<div class="price"><span class="symbol">' . $symbol . '</span> ' . $price . '</div>';
  }
  else {
    $output = '<div class="price">' . $price . '<span class="symbol">' . $symbol . '</span></div>';
  }
  return $output;
}

/**
 * Theme function for formating price without html
 *
 * @param  $price
 *   Price to format
 * @param  $symbol
 *   Symbol of currency
 * @param bool $position
 *   Position of price and it symbol
 * @return string
 *   Formated price
 */
function theme_pgapi_format_price_plain($price, $symbol, $position = FALSE) {
  if ($position) {
    $output = $symbol . ' ' . $price;
  }
  else {
    $output = $price . ' ' . $symbol;
  }
  return $output;
}
