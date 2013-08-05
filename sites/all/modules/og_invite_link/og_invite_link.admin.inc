<?php

/**
 * @file
 *  Contains administration pages.
 */

/**
 * Settings form for this module.
 *
 */
function og_invite_link_admin() {
  $form = array();
  //Provide an option for site administrators to choose
  //how long the invitations are valid. Expired ones are
  //just purged at cron.
  $options = array(1 => 1, 2 => 2, 5 => 5, 10 => 10, 15 => 15, 30 => 30, 45 => 45, 60 => 60, 90 => 90, 0 => t('Never'));
  $form['og_invite_link_expiration'] = array(
    '#type' => 'select',
    '#title' => t('Valid for number of days'),
    '#description' => t('Select the number of days after which the invitations will become expired.'),
    '#default_value' => variable_get('og_invite_link_expiration', 30),
    '#options' => $options,
  );
  
  $form['registration'] = array(
    '#type' => 'fieldset',
    '#title' => t('Site registration'),
  );
  $form['registration']['og_invite_link_invite_unregistered'] = array(
    '#type' => 'checkbox',
    '#title' => t('Invite people without accounts'),
    '#description' => t('If enabled, invitations are sent for user registration along with a token so that users can accept invitations when registering.'),
    '#default_value' => variable_get('og_invite_link_invite_unregistered', 0),
  );
  // This setting depends on our implementation of hook_menu_alter(),
  // so we clear the menu cache after submitting the setting sform.
  $form['registration']['og_invite_link_allow_registration'] = array(
    '#type' => 'checkbox',
    '#title' => t('Allow user registration with a valid invite even when site-wide user registration is disabled.'),
    '#description' => t('Anyone with a valid invitation link and email address combination will be able to register for the site regardless of the settings at admin/user/register'),
    '#default_value' => variable_get('og_invite_link_allow_registration', 0),
  );
  $form['#submit'][] = 'menu_cache_clear_all';
  return system_settings_form($form);
}

