<?php
// $Id: mc_web_create_confirm_form.tpl.php,v 1.1.2.2 2010/12/06 13:19:46 hutch Exp $

/*
 * mc_webform.tpl.php
 * A powerful default web form for entering and editing exchanges
 *
 * Variables available
 * $user = Logged in user Object
 * $form = the full form definition array
 * ... and all the others
 *
 * //The following form fields should ALL be rendered unless they are NULL
 * $description = transaction description
 * $payer_uid = user selection widget (or NULL)
 * $payee_uid = user selection widget (or NULL)
 * $completer_uid = user selection widget (or NULL)
 * $exchange_type = selection widget (or NULL)
 * $title = textfield (or NULL)
 * $quantity = textfield (or NULL)
 * $state = checkbox only visible in fulledit mode
 * $summary = teaser from transaction.tpl.php
 * $cid, a widget for selecting currency
 * $back, $next, $submit buttons
 *
 * + $form['#values']
 */

print $teaser;
print $buttons;
//need to make sure the ccka and taxonomy of the fields are hidden
print '<div style = "display:none;">'.$hidden . '</div>';
?>