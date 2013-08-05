<?php
//$Id: balance_ometer.tpl.php,v 1.3 2010/12/08 11:43:18 matslats Exp $

/*
 * Balance_limits.tpl.php
 * Themed display the user's balance limits for a given currency
 * Some variables can be set at the start
 * 
 * variables:
 * 
 * $account
 * $min = array($cid => -100...);
 * $max = array($cid => 100...);
 * $balance = array($cid => 43...);
 * $currency = node object
 */

//it's a bit tricky to skew the colors if zero isn't in the center.
$colors = array('FF0000', $currency->data['color'], 'AA', $currency->data['color'], 'FF0000');
//or to skew the needle!

$params = array(
  'cht' => 'gom',
  'chs' => '170x120',
  'chxt' => 'x,y',
  'chds' => $min .','. $max,
  'chxr' => '1,'.$account->balances[$cid]['limit_min'] .','. $account->balances[$cid]['limit_max'] .'25',
  'chd' => 't:'.$balance,
  'chxl' => '1:|'. $min ."|". $max,
  'chco' => implode(',', $colors),
  'chl' => $balance,
  'chtt' => $currency->title,
  'chxs' => '0,000000|1,000000',
  'chf' => 'bg,s,FFFFFFFF'
);


//cleaner than http_build_query
foreach ($params as $key=>$val) {
  $args[] = $key . '=' . $val;
}

$url =  GOOGLE_CHARTS_URI .implode('&', $args);
print '<img src="'.$url.'">';

?>