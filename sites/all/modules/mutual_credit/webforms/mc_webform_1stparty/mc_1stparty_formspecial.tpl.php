<?php
// $Id: mc_1stparty_formspecial.tpl.php,v 1.1.2.2 2010/12/06 13:19:46 hutch Exp $

/*
 * mc_webform.tpl.php
 * A web form for exchanges going in either direction in relation to the user.
 *
 * Variables available
 * $user = Logged in user Object
 * $form = the full form definition array
 * ... and all the others
 *
 * //The following variables should ALL be printed unless they are NULL
 * $title = transaction description
 * $completer = user selection widget (or NULL)
 * $exchange_type = selection widget (or NULL)
 * $title = textfield (or NULL)
 * $mc_quantity = textfield (or NULL)
 * $cid, a widget for selecting currency (only if currency isn't selected)
 * $buttons
 * $rating, a dropdown, probably
 */

//The exchange_type is hidden if there's only one option presented to the form.
if (!isset($exchange_type)) {
  $exchange_type = $form['exchange_type']['#options'][$form['exchange_type']['#default_value']];
}


$translations = array(
  '!description' => $title,
  '!trader' => $completer,
  '!exchange_type' => $exchange_type,
  '!quantity' => $mc_quantity,
  '!currency' => isset($cid) ? $cid : '',
  );

$message = t('I exchanged<br />!description<br />with<br />!trader<br />and now I<br />!exchange_type<br />the sum of <br />!quantity !currency', $translations);


print '<p>'. $message .'</p>
<p style="clear:left">';
if (isset($body))  print $body;

print $taxonomy;
print $hidden;
print $buttons;
?></p>
<style>.container-inline{float:inherit;}</style>
