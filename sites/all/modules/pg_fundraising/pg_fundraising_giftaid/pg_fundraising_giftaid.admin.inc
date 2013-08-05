<?php 
// $id$

/**
 * @file
 *   admin-related forms
 */

/**
 * GiftAid admin form
 */
function pg_fundraising_giftaid_admin() {

  $form['pg_fundraising_giftaid_desc'] = array(
    '#type'          => 'textarea',
    '#title'         => t('Description on GiftAid'),
    '#default_value' => variable_get('pg_fundraising_giftaid_desc', ''),
    '#size'          => 50,
    '#maxlength'     => 400,
    '#description'   => t('Explanation on GiftAid for users'),
  );

  return system_settings_form($form);
}
