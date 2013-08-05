<?php
// $Id: pg_fundraising_donation_menu.tpl.php,v 1.1.2.3 2011/02/18 02:15:35 dokumori Exp $
/**
 * @file
 * Template file for the donation menu
 */

/**
 * Available variables:
 *
 * $donation_menu
 *   Formatted donation menu.
 *
 * $images
 *   An array of images, already processed by imagecache
 *
 * $amount
 *   An array of amount, preformatted
 *
 * $descriptions
 *   An array of description, preformatted
 */
?>
<div id="fundraising-donation-menu">
<?php print theme('pg_fundraising_donation_menu_box', $images, $amount, $descriptions); ?>
</div>
