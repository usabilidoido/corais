<?php
/**
 * @file
 *  Admin form for OG Node Access module.
 */

/**
 * Form callback for node settings.
 */
function og_node_access_node_settings_form(&$form_state) {
  $form = array();

  $gid = FALSE;
  if (module_exists('spaces_og')) {
    $space = spaces_get_space();
    if ($space->type == 'og') {
      $gid = $space->group->nid;
    }
  }
  $needs_rebuild = og_node_access_needs_rebuild();
  $no_task = empty($needs_rebuild) && !$gid;
  
  $form['og_node_access'] = array(
    '#title' => t('Group post access'),
    '#description' => t('Rebuild node access for groups marked as having a privacy state change.'),
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
  );

  if ($no_task) {
    $output = '<p>' . t('There are no groups marked for access rebuild.') . '</p>';
    if (user_access('administer nodes')) {
      $output .= '<p>' . t('Please visit !link to rebuild access to all nodes.', array('!link' => l(t('Post settings'), 'admin/content/node-settings'))) . '</p>';
    }

    $form['og_node_access']['no_task'] = array(
      '#value' => $output,   
    );
  }
  else {
    $form['og_node_access']['rebuild'] = array(
      '#type' => 'submit', 
      '#value' => t('Rebuild permissions'), 
      '#submit' => array('_og_node_access_rebuild_submit'),
    );
  }
  return $form;
}

/**
 * Submit handler for og node access rebuild.
 */
function _og_node_access_rebuild_submit($form, &$form_state) {
  $form_state['redirect'] = arg(0) . '/og/node-settings/rebuild';
}

/**
 * Confirm form for node access rebuild.
 */
function og_node_access_rebuild_confirm() {
  $prefix = arg(0);
  return confirm_form(array(), t('Are you sure you want to rebuild the permissions on group content?'), 
    $prefix . '/og/node-settings', t('This action rebuilds all permissions on group content, and may be a lengthy process. This action cannot be undone.'), t('Rebuild permissions'), t('Cancel'));
}

/**
 * Submit handler for confirm form.
 */
function og_node_access_rebuild_confirm_submit($form, &$form_state) {
  $groups = og_node_access_needs_rebuild();

  // If there are no groups marked for rebuild, see if the current space is a group and rebuild that.
  if (empty($groups) && module_exists('spaces_og')) {
    $space = spaces_get_space();
    if ($space->type == 'og') {
      $groups = array($space->id => $space->id);
    }
  }
  if (!empty($groups)) {
    og_node_access_rebuild($groups, TRUE);
  }

  $form_state['redirect'] = arg(0) . '/og/node-settings';
}
