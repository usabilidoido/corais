<?php
//$Id: statement.tpl.php,v 1.3 2010/12/08 11:43:18 matslats Exp $
  
/* statement view
 * 
 * VARIABLES:
 * $per_page
 * $account
 * $cid (optional)
 * $exchanges array, following variables are available
   * //raw
   * $title            //the exchange title,
   * $payer_uid        //uid of payer
   * $payee_uid        //uid of payee
   * $cid              //nid of currency (0 unless currencies module installed)
   * $currency         //currency node object if only one currency is shown
   * $quantity         //quant of currency in exchange
   * $exchange_type    //readable string
   * $state            //1 = pending, 0 = completed, -1 = erased
   *
   * //pre-processed
   * $title_link      //title of exchange, linked to node
   * $submitted       //formatted date
   * $amount          //formatted quantity
   * $payer           //name linked to payer profile
   * $payee           //name linked to payee profile
   * $other           //name linked to profile of other trader
   * $income OR $expenditure //formatted quantity
   * $balance         //formatted running balance
   * $classes         //array of css classes
   *
  [pager] => themed pager
)
 */
if (isset($currency)) { ?>
<h4><?php print t('@currency statement (reverse chronological)', array('@currency' => $currency->title)); ?></h4>
<?php }

if (!count($exchanges)) {
  print t('No exchanges.');
  return;
}

global $pager_total, $pager_page_array;

//first of all sort out the order, and the paging
$exchanges = array_reverse($exchanges);
if (isset($_GET['page'])) {
  $pager_page_array = explode(',', $_GET['page']);
}
else {
  $pager_page_array = array(0,0);
}
$page_no = $pager_page_array[EXCHANGES_PAGER_ELEMENT];
$pager_total[EXCHANGES_PAGER_ELEMENT] = ceil(count($exchanges)/$per_page);

$exchanges = array_slice($exchanges, $page_no*$per_page, $per_page);

//need to delare the column headings
//array keys must correspond to the keys in the exchange objects
$columns = array(
  'submitted' => t('Date'),
  'title_link' => t('Item or service'),
  'other' => t('With'),
  'amount' => t('Amount'),
  'income' => t('Income'),
  'expenditure' => t('Expenditure'),
  'balance' => t('Running Total'),
);

//put the given array into the columns declared to make a table
foreach($exchanges as $key => $exchange) {
  foreach ($columns as $field => $title){
    $rows[$key]['data'][$field] = $exchange[$field];
    $rows[$key]['class'] = implode(' ', $exchange['classes']);
  }
}

print theme('table', array_values($columns), $rows) .
  theme('pager', NULL, $per_page, EXCHANGES_PAGER_ELEMENT);