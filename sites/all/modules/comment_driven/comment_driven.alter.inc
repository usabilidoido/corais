<?php
// $Id: comment_driven.alter.inc,v 1.3 2010/05/01 17:32:04 arhak Exp $

/**
 * Deferred heavier fragment of comment_driven_form_alter().
 * ONCE KNOWN it is a comment_form, it is being created (not edited)
 * and there is something to do about it rather than stepping out of the way
 * i.e. there are driven properties, user has some drive permissions, etc
 * (see comment_driven_form_alter for preconditions)
 */
function _comment_driven_proceed_comment_form_alter(&$form, &$form_state) {
  // reloading these variables has no impact,
  // since everything is statically cached
  $nid = $form['nid']['#value'];
  $node = node_load($nid);
  $node_type = $node->type;
  
  $driver_mod = 'comment_driven';
  $driven_props = driven_props_get_properties($driver_mod, $node_type);
  
  // if not previewing start with a fresh $ned_form_state
  $ned_form_state = array();
  _comment_driven_restore_from_preview($form, $form_state, $ned_form_state);
  
  // node/node.pages.inc: required for node_form
  // being in form_alter => cck/includes/content.node_form.inc
  module_load_include('inc', 'node', 'node.pages');
  // comment/comment.pages.inc: not needed
  
  // 'ned' prefix stands for 'node edit'
  $ned_form_id = $node_type . '_node_form';
  // on preview needs to receive the real $form_state (many tweaks, e.g. taxonomy)
  $ned_form = drupal_retrieve_form($ned_form_id, $ned_form_state, $node);
  // kindly request driven_nep to step out of the way
  $ned_form['#driven_nep:disable'] = TRUE;
  drupal_prepare_form($ned_form_id, $ned_form, $ned_form_state);
  
  // IMPORTANT: apply supported behaviors/policies
  $policies = driven_props_get_policies($driver_mod, $node_type);
  // IMPORTANT: apply Access Control Policies
  if (module_exists('driven_acp')) {
    driven_acp_apply_to_properties($driven_props, $policies);
  }
  else {
    driven_full_access_apply_to_properties($driven_props);
  }
  // if it returns FALSE then cleanup left nothing to work with
  if (!driven_access_control_to_form($ned_form, $driven_props)) {
    return; // step out of the way
  }
  // IMPORTANT: producer/consumer requires to be after driven_full_access
  // (or any other ACP that might be granting access in depth)
  if (module_exists('driven_cck_producer_consumer')) {
    driven_cck_producer_consumer_apply_to_form($ned_form, $driven_props, $policies);
  }
  
  if (comment_driven_allow_empty_comment($node_type)) {
    // allow empty comments (on validate will check if driven properties are changed)
    $form['comment_filter']['comment']['#required'] = FALSE;
  }
  
  // support comment preview
  // form might end up being cached or not
  // if cached, we won't have the chance to alter it again
  // it will pass to the validation stage bypassing form_alter
  // therefore, can't follow the approach of comment module for previewing
  // since it relies in comment_form not being cached 
  // instead we'll follow node_form approach
  $form['submit']['#submit'] = $form['#submit'];
  $form['preview']['#type'] = 'submit'; // no longer a button
  $form['preview']['#submit'] = array('comment_driven_build_preview');
  
  // these properties doesn't mean anything to an element
  $form_level_keys = array(
    'form_id', 
    'form_build_id', 
    'form_token',
  );
  // prepare to merge node_form into comment_form
  // avoid name clashes
  // (e.g. 'comment' is the body in comment_form and the comment_settings in node_form)
  $keys = array_diff(element_children($form), $form_level_keys);
  foreach ($keys as $key) {
    $form['comment_driven'][$key] = $form[$key];
    unset($form[$key]);
  }
  $form['comment_driven'] = _comment_driven_disguise_children($form['comment_driven']);
  // disguise_children supports only element_children
  // (disguise_form_values doesn't address non-children either) 
  // undisguise does support non-children as well
  // manually disguise non-children
  $keys = array('#validate', '#submit');
  foreach ($keys as $key) {
    $form['comment_driven']['#' . COMMENT_DRIVEN__DISGUISE_PREFIX . $key] = $form[$key];
    unset($form[$key]);
  }
  // this was already unset while manually disguising non-children (right above)
  // but lets make clear the intention of letting $ned_form['#submit'] get in on $form += $ned_form
  unset($form['#submit']);
  // preserve other submit handlers from node_form (e.g. menu_node_form_submit, taxonomy_single_tag_node_form_submit)
  // (the main submit won't get in, since it is in the buttons)
  // since $form['#submit'] was unset above,
  // this will happen in $form += $ned_form
  //$form['#submit'] = $ned_form['#submit'];
  
  // preserve #after_build assigned to the element they address
  // lets ensure we have an array to avoid headaches bellow
  $form['comment_driven']['#after_build'] = empty($form['#after_build']) ? array() : $form['#after_build'];
  unset($form['#after_build']); 
  // ensure comment module don't show its preview
  // which might happen if the resulting node_form wasn't cached
  // i.e. $ned_form['#cache'] ended up being FALSE (form_builder)
  // then comment module will have the chance
  // to add comment_form_add_preview in #after_build
  $after_build = &$form['comment_driven']['#after_build'];
  $after_build = array_diff($after_build, array('comment_form_add_preview'));
  $after_build[] = 'comment_driven_after_build';
  
  // if has no parent comment use actual node preview (how changes would look like)
  if (!empty($form_state['comment_driven:preview']['#suffix'])) {
    $form['#suffix'] .= $ned_form['#prefix'];
  }
  
  // support more theme functions
  // (note that if $form['#theme'] is not set over here it will receive $ned_form['#theme'] when added)
  // usually comment_form comes without #theme (and the disguised won't be themed neither)
  // alternate our suggestions with traditional node_form suggestions
  $form['#theme'] = array($node_type . '_comment_driven_form', $node_type . '_node_form', 'comment_driven_form', 'node_form');
  
  // final modifications to $ned_form before copying it to the comment's form
  // avoid name clash between 'Preview' buttons at node/comment forms
  unset($ned_form['buttons']);
  // drop preview
  unset($ned_form['#prefix']);
  
  // merge comment_form+node_form
  $form += $ned_form;
  
  // node_form is cached many times (e.g. menu_form_alter),
  // but comment_form won't support previewing
  // then comment preview should be handled in another fashion
  // (node_form fashion instead of comment_form fashion)
  // can't enforce disabling the cache, #process functions might bring it back
  //   filefield_elements adds #input=TRUE to filefield_widget
  //   with #process'=>array('filefield_widget_process')
  //   which inserts $element['filefield_upload']['#ahah']
  // moreover shouldn't disable the cache or AHAH elements will brake
  // (e.g. CCK's add_more, filefield upload/remove buttons, etc)
  //$form['#cache'] = FALSE;
//$form['#cache'] = FALSE; // @TODO: temporal testing uncached (when no AHAH)
  
  // @TODO: every call to comment_driven_assertion_failed will be removed after alpha/beta stage
  if (count(array_diff($ned_form['#submit'], $form['#submit']))) comment_driven_assertion_failed('form\'s #submit handlers weren\'t properly copied.');
  
  array_unshift($form['#validate'], 'comment_driven_validate');
  // submit handler will be set on validate to avoid infinite recursion
  // comment_driven_submit -> node_form_submit -> node_form_submit_build_node -> form_execute_handlers -> comment_driven_submit
  //array_unshift($form['#submit'], 'comment_driven_submit');
  
  // enhance presentation
  // avoid actually moving form elements around or node_form might brake
  // (it will brake for AHAH and who knows what other stuff contrib modules are doing)
  // having JavaScript disabled will render the node form right bellow the comment form
  // since it might be too big and misleading if prepended (remind collapsible fields will be expanded then)
  // having JavaScript enabled will render the node form wrapped
  // into a collapsible fieldset right above the comment form
  $form['comment_driven'] += array(
    // when JS is disabled comment_form should be above everything else
    '#weight' => -1000,
    // DOM id to grab everything within (the comment_form)
    '#prefix' => '<div id="comment_driven">',
    '#suffix' => '</div>',
  );
  // empty fieldset to be altered via jQuery
  // it will hold the node_form
  $type_name = node_get_types('name', $node_type);
  $default = t('Driven @type', array('@type' => $type_name));
  $title = driven_props_get_type_setting($driver_mod, $node_type, 'fieldset:title', $default);
  $collapsed = driven_props_get_type_setting($driver_mod, $node_type, 'fieldset:collapsed', 1);
  $form['driven_node'] = array(
    '#type' => 'fieldset',
    '#title' => $title,
    '#collapsible' => TRUE,
    '#collapsed' => $collapsed,
    // DOM id to grab it, non-visible when JS is disabled
    '#attributes' => array('id' => 'driven_node_container',  'style' => 'display:none'),
    '#weight' => 1000, // sink it (don't bother browsers without CSS support)
  );
  // this could be set regarless vertical_tabs being enabled or not
  // but I would prefer to avoid another 3rd party module
  // interpreting the #group key as something else
  if (module_exists('vertical_tabs')) {
    // support vertical_tabs [#765972]
    $form['driven_node']['#group'] = FALSE;
  }
}

function _comment_driven_restore_from_preview(&$form, &$form_state, &$ned_form_state) {
  if (!isset($form_state['comment_driven:preview'])) {
    // didn't came from preview
    return; // nothing to do
  }
  
  // the reason for the orig_form back & forth is that
  // first time we need it way past form_alter (process & after_build included)
  // therefore, that spot is at validation time
  //
  // change detection can't use the same form after previewing
  // because form gets updated to reflect last previewed version
  // therefore wouldn't be possible to know the actual state of the node
  // preserve a copy of the original node_form
  // (the one reflecting the state of the saved node, not the previewed)
  //
  // comes from validate via $form_state
  // and goes back to the flow via non-child in the $form
  $form['#comment_driven:orig_form'] = $form_state['#comment_driven:orig_form'];
  
  $preview = $form_state['comment_driven:preview'];
  // previews the comment
  $form['comment_preview'] = $preview['comment_preview'];
  // previews the node if has no parent comment (will use actual node preview bellow)
  //$form['#suffix'] .= $preview['#suffix'];
  // previews the parent comment (if any)
  $form['comment_preview_below'] = $preview['comment_preview_below'];
  
  // display diff summary table
  // (couldn't be inside the body, since it was filtered in comment_form_add_preview)
  // (wasn't set if there were no changes to avoid rendering empty div/table/etc)
  if (!empty($form_state['comment_driven:preview']['diff_summary_table'])) {
    $form['comment_preview_above'] = array(
      '#value' => $form_state['comment_driven:preview']['diff_summary_table'],
      '#weight' => -200,
      '#prefix' => '<div class="comment_driven diff preview">',
      '#suffix' => '</div>',
    );
  }
  
  // don't unset $form_state['node'] for node_form processing (don't know if it will be needed)
  $ned_form_state = $form_state; // needed to preserve values (e.g. taxonomy)
  
  // restore comment's previewed values
  $previewed = $form_state['comment_driven:preview']['comment'];
  $keys = array('author', 'name', 'mail', 'homepage', 'subject');
  foreach($keys as $key) {
    // not using empty neither isset to match comment module
    if ($previewed[$key]) {
      $form[$key]['#default_value'] = $previewed[$key];
    }
  }
  // don't take care of the admin sub-elements
  // we are not handling edition
  if ($previewed['comment']) {
    $form['comment_filter']['comment']['#default_value'] = $previewed['comment'];
  }
  if ($previewed['format']) {
    $value = $previewed['format'];
    $form['comment_filter']['format'][$value]['#value'] = $value;
  }
}

/**
 * Deferred function comment_driven_build_preview().
 */
function _comment_driven_build_preview($form, &$form_state) {
  unset($form_state['submit_handlers']);
  $comment_form = _comment_driven_undisguise($form);
  $comment_form_state = $form_state; // copied
  $comment_form_state['values'] = _comment_driven_extract_state_values($form_state['values']);
  
  $comment_form = comment_form_add_preview($comment_form, $comment_form_state);
  // previews the comment
  $preview['comment_preview'] = $comment_form['comment_preview'];
  // previews the node if has no parent comment
  $preview['#suffix'] = $comment_form['#suffix'];
  // previews the parent comment (if any)
  $preview['comment_preview_below'] = $comment_form['comment_preview_below'];
  $form_state['comment_driven:preview'] = $preview;
  
  // comments bellow explain the nasty tweaks performed here
  node_form_build_preview($form, $form_state);
  // pick up $form_state['node']
  //$node = node_form_submit_build_node($form, $form_state);
  // build the preview, since taxonomy will rely on it
  // to perform $node->taxonomy = taxonomy_preview_terms($node);
  //$form_state['node_preview'] = node_preview($node);
  //$form_state['rebuild'] = TRUE; // already happens in node_form_submit_build_node
  
  // recover comment's values
  _comment_form_submit($comment_form_state['values']);
  $form_state['comment_driven:preview']['comment'] = $comment_form_state['values'];
  
  // display diff summary table
  // (couldn't be inside the body, since it was filtered in comment_form_add_preview)
  // ($changes were stored on validation)
  $changes = $form_state['comment_driven:changes'];
  if (!empty($changes)) {
    // if there are no changes avoid rendering empty div/table/etc
    $node_type = $form['#node']->type;
    $diff_render = driven_diff_render($node_type, $changes);
    $form_state['comment_driven:preview']['diff_summary_table'] = driven_diff_summary_table($diff_render);
  }
  
  return $form;
}

/**
 * Deferred function comment_driven_validate().
 */
function _comment_driven_validate($form, &$form_state) {
  $node_type = $form['#node']->type;

  // @TODO: every call to comment_driven_assertion_failed will be removed after alpha/beta stage
  // after_build is too early to check if $form['#cache'] has been changed
  // which is the very last step in form_builder (form.inc) 
  // @TODO: change to DRIVEN__REPORT__LOG for beta stage
  $assertion_level = variable_get(DRIVEN__VAR_ASSERTION_LEVEL, DRIVEN__REPORT__BOTH);
  if (!empty($form['#cache']) && ($assertion_level & DRIVEN__REPORT__DISPLAY)) {
    // @TODO: no translatable since it is for development purposes only
    drupal_set_message('DEV Note: the node form is being cached (is menu module enabled or an AHAH element being used?)', 'warning', FALSE);
  }
  
  // explanation for this is in preview handling
  // i.e. _comment_driven_restore_from_preview (called from form_alter)
  //
  // goes to form_alter via $form_state
  // and comes back to the flow via #value in the $form
  //
  // first time we make a copy here
  // next times it will come from form_alter already set
  //
  // must to be set on every preview,
  // since $form_state is emptied on (requested) form rebuild
  $orig_form = isset($form['#comment_driven:orig_form'])
    ? $form['#comment_driven:orig_form'] // the previously saved
    : $form; // copied
  // following the approach of content_add_more_js
  // let other modules the oportunity to tweak the original form
  // to add elements with empty values
  // for those created with AHAH while being on preview
  // (e.g. driven_cck_comment_driven_orig_form_alter)
  // note that before the first validation attempt the orig_form hasn't been copied
  // therefore, some AHAH elements might get in
  // without causing any problem, since preview hasn't been performed yet
  // and the form should still reflect the state of the saved node
  // therefore, to test this, re-previewing would be the addressed use case
  // also note that if this is the first time, altering the orig_form
  // shouldn't be needed, but not knowing what 3rd party modules might come up with
  // lets give them the chance anyway
  drupal_alter('comment_driven_orig_form', $orig_form, $form);
  $form_state['#comment_driven:orig_form'] = $orig_form;
  
  // if we are comming from a preview the $form is refleting the previewed node
  // instead of the actual state of the saved node
  // compare the original form with the current $form_state
  $changes = comment_driven_diff_form_state($orig_form, $form_state);
  $form_state['comment_driven:changes'] = $changes; // save it for submit
  
  // @TODO: every call to comment_driven_assertion_failed will be removed after alpha/beta stage 
  if (!is_array($changes)) comment_driven_assertion_failed('!is_array($changes)');
  
  // report comment body as required if no changes
  if (empty($changes) && comment_driven_allow_empty_comment($node_type)) {
    $element = $form['comment_driven'][COMMENT_DRIVEN__DISGUISE_PREFIX . 'comment_filter'][COMMENT_DRIVEN__DISGUISE_PREFIX . 'comment'];
    if (strlen(trim($element['#value'])) == 0) {
      form_error($element, t('Comment field is required (or a driven property change otherwise).'));
    }
  }
  
  // @TODO: not sure about when to include these two
  // they will be both loaded in this request anyway,
  // and required at different times (form build/validate/submit)
  // (the form being cached might cause a direct jump into validate/submit)
  // but not having loaded the required ones on proper time
  // may lead to silent failures
  module_load_include('inc', 'node', 'node.pages');
  module_load_include('inc', 'comment', 'comment.pages');
  
  // set our submit handler on validate to avoid infinite recursion
  // comment_driven_submit -> node_form_submit -> node_form_submit_build_node -> form_execute_handlers -> comment_driven_submit
  if ($form_state['values']['op'] == t('Save')) {
    // drop every submit handler,
    // we'll pick them up and manually handle them on our submit handler
    // honoring their order and also picking up resulting cid with accuracy
    // (nevertheless, the main reason for this lazzy submit handler is to avoid the recursion explained above)
    // (to test with extra submit handlers, a 3rd party module would be needed, e.g. flatcomments-6.x-2.0 [#737982])
    $form_state['submit_handlers'] = array('comment_driven_submit');
    // OTOH comment_driven_build_preview will drop every submit handler
    // as comment module used to do with its preview, but in a different fashion
    // since we convertered Preview from button to actual #type=submit
    // and moved comment_form's #submit to the Save operation
  }
  $comment_form = _comment_driven_undisguise($form['comment_driven']);
  $comment_form_state = $form_state; // copied
  $comment_form_state['values'] = _comment_driven_extract_state_values($form_state['values']);
  
  $validate_handlers = $comment_form['#validate'];
  if (!empty($validate_handlers)) {
    foreach ($validate_handlers as $validate_handler) {
      $validate_handler($comment_form, $comment_form_state);
    }
  }
}

/**
 * Deferred function comment_driven_submit().
 */
function _comment_driven_submit($form, &$form_state) {
  // @TODO: not sure about when to include these two
  // they will be both loaded in this request anyway,
  // and required at different times (form build/validate/submit)
  // (the form being cached might cause a direct jump into validate/submit)
  // but not having loaded the required ones on proper time
  // may lead to silent failures
  module_load_include('inc', 'node', 'node.pages');
  module_load_include('inc', 'comment', 'comment.pages');
  
  $comment_form = _comment_driven_undisguise($form['comment_driven']);
  $comment_form_state = $form_state; // copied
  $comment_form_state['values'] = _comment_driven_extract_state_values($form_state['values']);
  
  $submit_handlers = $comment_form['#submit'];
  // @TODO: every call to comment_driven_assertion_failed will be removed after alpha/beta stage 
  if (!in_array('comment_form_submit', $submit_handlers)) comment_driven_assertion_failed('!in_array(\'comment_form_submit\', $submit_handlers)');
  foreach ($submit_handlers as $submit_handler) {
    if (function_exists($submit_handler)) {
      $submit_handler($comment_form, $comment_form_state);
      // honor submit handlers order
      if ($submit_handler == 'comment_form_submit') {
        // saved comment redirects to its respective fragment
        $redirect = $comment_form_state['redirect'];
        // @TODO: is there a better way to pick up the cid from comment_save?
        // can't be db_last_insert_id('comments', 'cid')
        // since comment_invoke_comment($edit, 'insert') already happened
        // the approach taken is certain, since comment_form_submit
        // picks up the $cid on success, and its last action is
        // $form_state['redirect'] = array('node/'. $node->nid, $page, "comment-$cid");
        $cid = substr($redirect[2], strlen('comment-'));
        // comment_save receives $edit by value
        // which is the $form_state['values'] comming from comment_form_submit
        //$cid = $form_state['values']['cid'];        
      }
    }
  }
  
  // @TODO: every call to comment_driven_assertion_failed will be removed after alpha/beta stage 
  if (!is_numeric($cid) && $cid > 0) comment_driven_assertion_failed('picked up an invalid $cid');

  $changes = $form_state['comment_driven:changes'];
  if (!empty($changes)) {
    //$nid = $form['nid']['#value'];
    //$node = node_load($nid);
    // this is the one we want,
    // since it got boolean 'revision' populated
    $node = $form['#node']; 
    $node_type = $node->type;
    
    // this is a global setting 
    if (variable_get('comment_driven:enforce_new_revision', 1)) {
      $node->revision = $form_state['values']['revision'] = TRUE;
    }
    
    // properties might no longer exist suddenly
    // a backup render has to be made instantly and kept
    $diff_render = driven_diff_render($node_type, $changes);
    $old_vid = $node->vid;
    $new_vid = ($node->revision ? 0 : $old_vid);
    
    $query = "INSERT INTO {comment_driven_log}(cid, old_vid, new_vid, changes, diff_render, timestamp) VALUES(%d, %d, %d, '%s', '%s', %d)";
    db_query($query, $cid, $old_vid, $new_vid, serialize($changes), serialize($diff_render), time());
    $did = db_last_insert_id('comment_driven_log', 'did');
    
    // only save if some value differs from its #default_value
    // otherwise it is just a comment
    node_form_submit($form, $form_state);
    
    if ($node->revision) {
      $nid = $form_state['nid'];
      $new_vid = db_result(db_query('SELECT vid FROM {node} WHERE nid = %d', $nid));
      db_query('UPDATE {comment_driven_log} SET new_vid = %d WHERE did = %d', $new_vid, $did);
    }
  }

  // restore proper redirect (which changed if node was saved)
  $form_state['redirect'] = $redirect;
}
