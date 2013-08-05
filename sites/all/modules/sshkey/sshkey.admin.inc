<?php

function sshkey_settings_form() {
  $form['sshkey_help'] = array(
    '#type' => 'textarea',
    '#title' => t('Help text for public key listing pages and the add public key forms'),
    '#default_value' => variable_get('sshkey_help', t('Need help with public keys? View the excellent GitHub.com SSH public key help at <a href="http://github.com/guides/providing-your-ssh-key" target="_blank">http://github.com/guides/providing-your-ssh-key</a>.')),
  );

  return system_settings_form($form);
}
