<?php
// $Id: comment_driven.admin.inc,v 1.6 2010/03/27 23:02:25 arhak Exp $

/**
 * Deferred function comment_driven_driven_props_type_settings_form(). 
 */
function _comment_driven_driven_props_type_settings_form($node_type) {
  $form = array(
    '#type' => 'fieldset',
    '#title' => t('Other settings'),
    '#description' => t('There are <a href="!url">global settings</a> which affect the behavior of every content type being comment driven enabled.', array('!url' => url('admin/settings/driven/comment-driven'))),
    '#collapsible' => TRUE,
  );
  $form['empty_comment'] = array(
    '#type' => 'checkbox',
    '#title' => t('Allow empty comments'),
    '#description' => t('Only applies if the comment introduces changes.'),
    '#default_value' => 1,
  );
  $form['fieldset:collapsed'] = array(
    '#type' => 'checkbox',
    '#title' => t('Collapsed driven properties'),
    '#description' => t('Whether to present the fieldset containing the driven properties collapsed or expanded.'),
    '#default_value' => 1,
  );
  
  $type_name = node_get_types('name', $node_type); // @d6
  $form['fieldset:title'] = array(
    '#type' => 'textfield',
    '#title' => t('Fieldset title'),
    '#description' => t('The title displayed in the fieldset containing the driven properties.'),
    '#default_value' => t('Driven @type', array('@type' => $type_name)),
  );
  
  // hide advanced stuff from plain sight
  // but don't wan't to deal with an extra nesting level due to the fieldset
  // (since driven_props will handle this form as #tree)
  // therefore the fielset should be parent of individual radios pointing to it
  $parents = array('driver_mod', 'live_render');
  $form['live_render'] = array(
    '#type' => 'fieldset',
    '#title' => t('Render'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['live_render']['disabled'] = array(
    '#type' => 'radio',
    '#title' => t('Frozen (aka snapshot)'),
    '#return_value' => 0,
    '#default_value' => 0,
    '#parents' => $parents,
    '#description' => t('Renders snapshot made on comment creation time.'),
  );
  $form['live_render']['enabled'] = array(
    '#type' => 'radio',
    '#title' => t('Live render'),
    '#return_value' => 1,
    '#default_value' => 0,
    '#parents' => $parents,
    '#description' => t('Renders driven properties on display time.')
      // since this option lives in an exclusive fieldset, using <div> instead of <span> might be acceptable 
      . '<br/><span class="warning">' . t('Might become an issue if a module doesn\'t honor guarding itself from gone properties and/or values.') . '</span>'
      . '<br/><span class="ok">' . t('This option is reversible at any time.') . '</span>'
      . '<br/>' . t('If you get weird errors when displaying old comments try disabling live render until you find support to figure it out.'),
  );
  
  return $form;
}

/**
 * Deferred function comment_driven_driven_global_settings_form().
 */
function _comment_driven_driven_global_settings_form() {
  $form['#menu_title'] = 'Comment driven';
  
  $form['inspect'] = array(
    '#type' => 'fieldset',
    '#title' => t('Inspect'),
    '#access' => module_exists('comment_driven_inspect'),
    '#collapsible' => TRUE,
  );
  
  $varname = 'comment_driven:inspect_links';
  $form['inspect'][$varname] = array(
    '#type' => 'checkbox',
    '#title' => t('Provide inspect links above comment\'s diff summary table.'),
    '#description' => t('Only applicable to users with proper permission.'),
    // @d6
    '#default_value' => variable_get($varname, 1),
  );
  
  $form['theme_support'] = array(
    '#type' => 'fieldset',
    '#title' => t('Theme support'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  
  $varname = 'comment_driven:node_form_bellow';
  $form['theme_support']['node_form'][$varname] = array(
    '#type' => 'checkbox',
    '#title' => t('Display driven properties bellow comment form.'),
    '#description' => t('Allows to control whether the driven properties appear above or bellow the comment form.'),
    // @d6
    '#default_value' => variable_get($varname, 0),
  );
  
  $varname = 'comment_driven:theme_support';
  $form['theme_support'][$varname] = array(
    '#type' => 'radios',
    '#title' => t('Driven fieldset'),
    '#description' => t('Disabling the fieldset doesn\'t support to position the driven properties above the comment form.'),
    '#options' => array(
      'default' => t('Default'),
      'atrium' => t('Open Atrium themes: Tao and its sub-themes: Rubik, Ginkgo, etc'),
      // I don't like this idea, but it was requested [#722792]
      'disabled' => t('Disabled'),
    ),
    // @d6
    '#default_value' => variable_get($varname, 'default'),
  );
  
  $form['workflow'] = array(
    '#type' => 'fieldset',
    '#title' => t('Workflow'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  
  $varname = 'comment_driven:enforce_new_revision';
  $form['workflow'][$varname] = array(
    '#type' => 'checkbox',
    '#title' => t('Enforce new revision.'),
    '#description' => t('Even when the content type may have revisions disabled this option will enforce a new revision whenever a driven property change is detected.')
      . ' ' . t('Therefore it only affects those content types with comment driven enabled AND revisions disabled.')
      . '<br/>' . t('Nevertheless, note that this option preserves the "before" state but it can\'t ensure the "after" state to be preserved as well. Which depends on how the resulting revision might be altered by node editions performed without creating a new revision and/or 3rd party modules (e.g. comment_alter_taxonomy).'),
    // @d6
    '#default_value' => variable_get($varname, 1),
  );
  
  return $form;
}
