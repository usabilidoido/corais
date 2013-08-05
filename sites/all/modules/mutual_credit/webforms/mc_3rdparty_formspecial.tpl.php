<?php
// $Id: mc_3rdparty_formspecial.tpl.php,v 1.1.2.2 2010/12/06 13:19:46 hutch Exp $

/*
 * mc_3rdparty_formspecial.tpl.php
 * A powerful default web form for entering and editing exchanges
 * N.B. This form is only called when special theming is on, see admin/mc/webforms
 *
 * Variables available
 * $user = Logged in user Object
 * $form = the full form definition array
 * ... and all the others
 *
 * //The following variables should ALL be printed unless they are NULL
 * $description = transaction description
 * $payer = user selection widget (or NULL)
 * $payee = user selection widget (or NULL)
 * $exchange_type = selection widget (or NULL)
 * $title = textfield (or NULL)
 * $mc_quantity = special field containing widgets for entering quantity
 * $state = checkbox only visible in fulledit mode
 * $summary = teaser from transaction.tpl.php
 * $cid, a widget for selecting currency
 * $buttons
 * OPTIONAL (without special theming);
 * $body (if the exchange node-type body is named in admin/content/node-type/exchange)
 * $taxonomy
 * +the names of any fields added by CCK
 *
 * * NOTE any cck, taxonomy or other fields need to be included explicitly in a copy of this file in the theme directory
 */
?>
<table>
<?php
if (isset($payer)) {
  print '<tr><td>'. t('Payer') .':</td><td>'. $payer .'</td></tr>';
}
if (isset($payee)) {
  print '<tr><td>'. t('Payee') .':</td><td>'. $payee .'</td></tr>';
}
if (isset($exchange_type)) {
  print '<tr><td>'. t('Exchange type') .':</td><td>'. $exchange_type.'</td></tr>';
}
elseif(isset($state)) {
  print '<tr><td></td><td>'. $state .'</td></tr>';
}
if (isset($title)) {
  print '<tr><td>'. t('Description') .':</td><td>'. $title.'</td></tr>';
}
if (isset($mc_quantity)) {
  print '<tr><td>'. t('Quantity') .':</td><td>'. $mc_quantity;
  if (isset($cid)) print $cid;
  print '</td></tr>';
}
?>
</table>
<?php
if (isset($body)) print $body;
print $taxonomy;
print $hidden;
print $buttons;
?>