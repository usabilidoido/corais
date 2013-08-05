<?php
// $Id: node-exchange.tpl.php,v 1.1.2.3 2010/12/06 13:19:46 hutch Exp $

/*
 * we'll do the preprocessing here, rather than try to interrupt the normal node preprocessing hierarchy
 * which would be inefficient
 * We have access to all the normal node fields, plus
 * $title            //the node title
 * $payer_uid        //uid of payer
 * $payee_uid        //uid of payee
 * $cid              //nid of currency (0 unless currencies module installed)
 * $quantity         //quant of currency in exchange
 * $exchange_type //either incoming_confirm, outgoing_confirm, incoming_direct or outgoing_direct, or others
 * $quality          //exchange rating - reflects on the payee
 * $state            //1 = pending, 0 = completed, -1 = erased
*/
//the preprocess function is added here because then it will only run for exchanges, not all nodes.
module_load_include('inc', 'mcapi');
extract(mc_preprocess_exchange($node));

/*
 * //makes the following available
 * $title
 * $submitted       //formatted date
 * $payer           //name linked to payer profile
 * $payee           //name linked to payee profile
 * $amount          //formatted quantity
 * $balance         //formatted running balance
 * $title_link    //title of exchange, linked to node
 * $classes         //array of css classes
 *
 */
//lumping all these together just makes the following translation strings easier

$currency = node_load($cid);

if (arg(0) == 'node') {
  $page_title = t('Exchange Certificate #@nid', array('@nid' => $nid));
  if ($state == EXCHANGE_STATE_PENDING) {
    $page_title .= '-'. strtoupper(t('pending'));
  }
  drupal_set_title($page_title);
}

$date = t('On @date', array('@date' => $submitted));
$movement = $state == EXCHANGE_STATE_PENDING ? 
  t('!payer will pay !payee', array('!payer' => $payer, '!payee' => $payee)) :
  t('!payer paid !payee', array('!payer' => $payer, '!payee' => $payee));
$sum = t('the sum of !amount', array('!amount' => '</p><p style="font-size:250%">'.$quantity .' '. $currency->title));
$reason = t('for !reason', array('!reason' => '<strong>'.$title.'</strong>'));
?>

<div class="exchange <?php print implode(' ', $classes); ?>">
  <p><?php print $date; ?></p>
   <p><?php print $movement; ?></p>
   <p><?php print $sum; ?> </p>
  <p><?php print $reason; ?>
  
<?php if (isset($body))  print $body; ?>
  <?php
    //links are used by the webforms module for edit/complete/delete actions
    print $links;
  ?>
</div>
