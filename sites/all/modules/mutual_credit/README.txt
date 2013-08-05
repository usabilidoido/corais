This document is 4 documents in one:


** How to set up the Complementary currencies package **
** Testing procedure **

See also the architecture document at
http://matslats.net/mutual-credit-architecture-social-network-2


***********************
**   HOW TO SET UP   **
***********************

ENABLE MODULES
Enable the mcapi FIRST, BY ITSELF
 - This is because it creates the first currency node which its dependents require
Enable any other required modules
Also consider uid_login and autocategorise, and user_tabs


Setup your currency admin/mc/curencies
Visit admin/user/permissions
If you are using 1stparty module, ensure that the exchange types you need are named on admin/mc, and that each currency has the right exchange types enabled
Compose the mail notification templates at admin/mc/notification
Get the css #id of the menu tabs and enter them into admin/mc/forms


Complementary Currencies should now function. However before your site makes sense, you'll need to add some users and data.
Don't forget you can import balances and exchanges with the mcapi_import.module


Architecture
This module attempts to be usable by default, but it's up to you how users will ultimately experience the site. 
The first level of architecture in drupal is in menus, views and blocks.
Then look at the callback functions (each one accessed by its own block) in the mc_display module. These can be incoporated into panels created by the panels module.
For more ideas visit demo.communityforge.net.
The site has been designed with classic LETS in mind. Have fun creating your own terminology on admin/marketplace.
This drupal installation is available on request if you want to use it as a starting point.

Other customisation
Design your own exchange forms starting with the tpl.php files provided. Don't forget to copy tpl.php files into your theme directory first.
Be creative with the exchange certificate exchange.tpl.php
Upload your own a currency icon
Look at the theme methods provided especially in mc_display


Making your own exchange forms

1. The exchange form is very flexible. You can pre-polulate it with an ad hoc exchange object, and make you're own payment blocks.
If you want people to sign up to your theatre trip by making a pre-payment, you might do the following in a block entitled 'Register for MacBeth'
<?php 
  $exchange = array(
    'payer_uid' => $GLOBALS['user']->uid,
    'payee_uid' => 99,
    'exchange_type' => outgoing direct,
    'title' => 'MacBeth pre-registration',
    'quantity'=> 6,
    'cid' => 123 //this is the nodeID of a currency
  );
  print drupal_get_form('mc_3rdparty_form', NULL, $exchange);
?>
The form will be pre-populated with these fields, and the user has only to click ok to record the exchange.
This feature needs to be extended so that each prepopulated field can be optionally disabled
Alternatively, it needs to be easier to generate entirely new forms which are easy to theme, so administrators could quickly make a form that said 'click here to prepay your seat'


***********************
** TESTING PROCEDURE **
***********************

The following procedure is carried out before every point release on Drupal.org
There is no way to test every combination of possibilities.

With a fresh drupal, create 2 users with email addresses you can check
install mcapi
Install all the other modules and follow set up instruction in README.txt
Place the three user_pending blocks where they can be seen.
Edit all features of the default currency.

Check that each form displays correctly with and without special theming.
Log in as user 3.
Make a direct payment to user 4. Check balances and /exchanges and chart.
Make a signed payment from user 4. It should appear in the block as 'Pending'
With default settings user 4 should have received a mail. Check it for sense.
Log in as user 4. The pending exchange should appear in 2 blocks, one of them on every page.
Sign the exchange and check balances, /exchanges and chart.
Check statistics.
Attempt a new exchange with user 3 which will take you outside the balance limits.

Log in as admin and create another currency
Check behaviour of first party form when currency is selected.
Pay a user and check balances /exchanges and chart

Mass pay
Make a direct many2one payment from everyone except user 3
Check balances & stats

Anonymous users
Log out. See if you can see any exchange information, user profiles, or offers and wants
Try going directory to
user/3
exchanges
node/x (of type exchange)