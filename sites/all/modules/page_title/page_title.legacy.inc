<?php

/**
 * @file
 * Small include for the legacy old table handling
 */

function page_title_drop_old_table_form() {
  if (!db_table_exists('page_title_old')) {
    drupal_set_message(t("The old Page Title table (<code>page_title_old</code>) doesn't exists. Please empty the site cache."));
    return array();
  }

  drupal_set_message(t('Are you sure you want to drop the <code>page_title_old</code> table. It contains unused data, but was left in place until an adminitrator could confirm the upgrade completed succesfully.'));
  $form = array();
  return confirm_form($form, t('Remove <code>page_title_old</code> table?'), 'admin/reports/status', t('This action cannot be undone'), t('Drop Table'), t('Cancel'));
}


/**
 * Internal function to remove the {page_title_old} table, if it exists.
 */
function page_title_drop_old_table_form_submit($form, &$form_state) {
  // Drop the table
  $ret = array();
  db_drop_table($ret, 'page_title_old');

  // Rebuild the menu - this should remove the now uneeded callback
  menu_rebuild();

  // redirect back to the status report.
  $form_state['redirect'] = 'admin/reports/status';
}
