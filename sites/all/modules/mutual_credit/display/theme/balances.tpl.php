<?php
//$Id: balances.tpl.php,v 1.3 2010/12/08 11:43:18 matslats Exp $

/* themes the balances for a given user.
 * Available variables
 * $balances & $themed_balances is an array of the form:
 * Array (
    [$cid] => Array (
      [cleared_balance] => numeric
      [pending_dif] => numeric
      [gross_in] => numeric
      [gross_out] => numeric
      [max_in] => numeric
      [max_out] => numeric
      [max] => numeric
      [min] => numeric
    )
  )
 * also $currencies = array() //generated by preprocess
 * plus the usual
 */
if (!count($balances)) {
  print t("Yet to trade.");
  return;
}
$rows = array(
  0 => array(0 => t('Cleared balance')),
  1 => array(0 => t('Total income')),
  2 => array(0 => t('Spending limit'))
);

$headings = array(0 => '');
foreach ($themed_balances as $cid => $set) {
  $headings[] = $currencies[$cid]->title;
  foreach($set as $value) {
    $rows[0][$cid] = $set['cleared_balance'];
    $rows[1][$cid] = $set['gross_in'];
    $rows[2][$cid] = $set['max_out'];
  }
}

print theme('table', $headings, $rows);
?>
