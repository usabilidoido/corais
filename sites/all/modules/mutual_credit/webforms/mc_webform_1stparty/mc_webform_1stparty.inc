<?php
// $Id: mc_webform_1stparty.inc,v 1.1.2.3 2010/12/15 22:04:04 matslats Exp $

module_load_include('inc', 'mc_webforms');

//although this isn't a node form, as far as Drupal is concerned, we are going to use the node validation from mcapi
// as well as validating a few things specific to this form
function mc_1stparty_form_validate($form, &$form_state) {
  //the individual fields are already validated
  if ($form_state['storage']['step'] == 1) {
    //send the pseudo-node off for validation by the mcapi
    mcapi_validate((object)$form_state['values'], $form);
  }
}

//process the exchange form values create the exchange and goto
function mc_1stparty_form_submit($form, &$form_state) {
  if (mc_submit_multistep($form_state)) return;
  //set the exchange 'state' depending on exchange sub-type
  if (strpos($form_state['values']['exchange_type'], 'direct')) {
    $form_state['values']['state'] = EXCHANGE_STATE_FINISHED;
  }
  else{
    $form_state['values']['state'] = EXCHANGE_STATE_PENDING;
  }
  mc_webform_create($form_state['values']);
  drupal_goto(variable_get('mc_webform_destination', 'user'));
}



//this was abstracted so it can be accessed by the startform stage 1 and the startform in edit mode
function mc_build_1stparty_form(&$form, $values = NULL) {
  global $user;
  mc_build_webform($form, $values);

  //this form uses the currencies' own exchange types, added by this module to the mcapi currencies
  //in any case the form HAS to have this field inserted or it will fail validation
  //so first we discover if the currency has been set
  if (isset($values['cid']) && $values['cid'] > 0) {
    $currencies = array(node_load($values['cid']));
  }
  else $currencies = $form['#currencies'];
  $available_exchange_subtypes = get_common_exchange_subtypes($currencies, FALSE);
  //we have to render this field regardless or the AJAX might not get attached to it in stage 1
  //instead of hiding it, we make access = FALSE by default, then change it below
  $form['exchange_type'] = array(
    '#type' => 'select',
    '#title' => t('Type of exchange'),
    '#options' => array('' => t('Please choose...')),
    '#default_value' => isset($values['exchange_type']) ? $values['exchange_type'] : '',
    '#required' => TRUE,
    '#ahah' => array(),
  );
  if (count($available_exchange_subtypes) == 1) {
    //fill in the one possible value and let the field remain hidden
    list($key, $val) = each($available_exchange_subtypes);
    $form['exchange_type']['#default_value'] = $key;
    $form['exchange_type']['#options'][$key] = $val; //this single option can be used in the theme layer
    $form['exchange_type']['#type'] = 'hidden';
  }
  //show the field with all its options
  elseif (count($available_exchange_subtypes) > 1) {
    $form['exchange_type']['#options'] += $available_exchange_subtypes;
  }

  //now we know the exchange_type we know whether the completer is payer or payee
  switch (substr($form['exchange_type']['#default_value'], 0, 8)) {
  	case 'incoming':
  	  $dest_field = 'payer_uid';
  	  break;
  	case 'outgoing':
      $dest_field = 'payee_uid';
      break;
    default:
      $dest_field = '';
  }

  $form['completer'] = array(
    '#title' => t('Exchange partner'),
    '#dest_field' => $dest_field,
    '#default_value' => isset($values['completer']) ? $values['completer'] : '',
    '#element_validate' => array('determine_completer')
  ) + $form['payee'];
  //remove the present user from #options
  $form['completer']['#after_build'] = array('mc_unlist_user');

  unset($form['payee'], $form['payer']);

  //then remove unused fields
  $form['state']['#type'] = 'hidden'; //will be populated in form_validate stage
  //change the name of the button from 'submit'
  $form['buttons']['submit']['#value'] = t('Next');
}

function determine_completer(&$element, &$form_state){
  global $user;
  if (substr($form_state['values']['exchange_type'], 0,8) == 'incoming') {
    $element['#dest_field'] = 'payer_uid';
    $form_state['values']['payee_uid'] = $user->uid;
  }
  else {
    $element['#dest_field'] = 'payee_uid';
    $form_state['values']['payer_uid'] = $user->uid;
  }
}

/*
 * Functions for manipulating, editing and displaying the subtype names
 */

// make an array suitable for use as #options in the exchange_type field
// this form actually provides 4 exchange types, called subtypes
// this is the point at which we convert the subtype constant values used as bitmasks
// into the subtype name which is stored in the transaction itself
// few other exchange forms will be this complicated!
function get_common_exchange_subtypes($currencies, $exclusive = TRUE) {
  $subtypes = module_invoke_all('exchange_type_info');
  $new_names = array();
  if ($exclusive) {
    //if the checkbox isn't ticked on any of the currencies given, remove the type from a copy of the full array
    foreach ($subtypes as $key => $details) {
      $new_names[$key] = $details['title'];
      foreach ($currencies as $currency) {
        if (!($currency->data['subtype_bitmask'] & $details['bitmask'])) {
          unset($new_names[$key]);
          continue 2;
        }
      }
    }
  }
  else{
    //if the checkbox is set, then add the subtype to the new array
    foreach ($subtypes as $key => $details) {
      foreach ($currencies as $currency) {
        if ($currency->data['subtype_bitmask'] & $details['bitmask']) {
          $new_names[$key] =  $details['title'];
          continue 2;
        }
      }
    }
  }
  return $new_names;
}

function mc_webform_1stparty_block_list($delta) {
  $blocks['start_exchange']= array(
    'info' => 'Mc '. t('1st Party exchange'),
    'visibility' => 0,
    'pages' => "exchange*\nadmin*\nnode*edit",
  );
  return $blocks;
}

function mc_webform_1stparty_block_view() {
  module_load_include('inc', 'mc_webforms');
  $values = array();
  global $user;
  $title_options = array();
  if (user_access('exchange')) {
    if (arg(0) == 'user') {
      if (arg(1) && $user->uid != arg(1)) {
        $account = user_load(arg(1));
        if (user_access('exchange', $account)) {
          $values['completer'] = $account->uid;
        }
        else return; //block will not show on a user's page who can't trade
        $title_options['user'] = $account;
      }
      else return; //block will not work on a user's own page
    }
    return array(
      'subject' => theme('mc_webform_title', $title_options),
      'content' => drupal_get_form('mc_1stparty_form', $values),
    );
  }
}
//put a helpful comment on the block edit page for the block produced by this module
function mc_webform_1stparty_form_block_admin_configure_alter(&$form, $form_state) {
  if ($form['#parameters'][2] == 'mc_webforms' && $form['#parameters'][3] == 'mc_1stparty_form') {
    $form['block_settings']['title']['#description'] = t("OVERRIDE theme_mc_webform_title() for more control here");
  }
}


//determine the exchange type names, which are the same accross the system
//can be moved to inc, but would need to mess with hook_menu_alter
function exchange_subtype_names_form(){
  $subtype_names = variable_get('mc_subtypes', array());
  $form['subtype_names'][INCOMING_DIRECT] = array(
    '#type' => 'textfield',
    '#title' => t('Incoming') . ' '. t('Direct'),
    '#default_value' => $subtype_names[INCOMING_DIRECT],
    '#element_validate' => array('mc_validate_subtype_names'),
    '#weight' => 1,
    '#size' => 20,
  );
  $form['subtype_names'][OUTGOING_DIRECT] = array(
    '#type' => 'textfield',
    '#title' => t('Outgoing') . ' '. t('Direct'),
    '#default_value' => $subtype_names[OUTGOING_DIRECT],
    '#element_validate' => array('mc_validate_subtype_names'),
    '#weight' => 2,
    '#size' => 20,
  );
  if (module_exists('mc_signatures')) {
    $form['subtype_names'][INCOMING_SIGNED] = array(
      '#type' => 'textfield',
      '#title' => t('Incoming') . ' '. t('Signed'),
      '#default_value' => $subtype_names[INCOMING_SIGNED],
      '#element_validate' => array('mc_validate_subtype_names'),
      '#weight' => 3,
      '#size' => 20,
    );
    $form['subtype_names'][OUTGOING_SIGNED] = array(
      '#type' => 'textfield',
      '#title' => t('Outgoing') . ' '. t('Signed'),
      '#default_value' => $subtype_names[OUTGOING_SIGNED],
      '#element_validate' => array('mc_validate_subtype_names'),
      '#weight' => 4,
      '#size' => 20,
    );
  }
  $form['subtype_names']['#theme'] = 'subtype_names';
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit')
  );
  return $form;
}

function mc_validate_subtype_names($element, &$form_state) {
  $val = $element['#value'];
  if (!$val) return;
  if (check_plain($val) != $val) {
    form_error($element, t("plain text values only: @string", array('@string' => $val)));
  }
  if (array_search($val, $form_state['values']) != $element['#name']) {
    form_error($element, t("Each name must be unique"));
  }
}

/*
 * Ensure that at least one exchange type is named
 */
function exchange_subtype_names_form_validate($form, $form_state) {
  foreach ($form['subtype_names'] as $key => $array) {
    if (strlen($form_state['values'][$key])) return;
  }
  form_set_error('1', t("The '1stparty' module requires that at least one exchange type be named"));
}

function exchange_subtype_names_form_submit($form, $form_state) {
  foreach ($form['subtype_names'] as $key => $array) {
    $new_subtype_names[$key] = $form_state['values'][$key];
  }
  variable_set('mc_subtypes', $new_subtype_names);
}


/*
 * theme callback for exchange form, page and block titles
 *
 * $options can be array(
 *   'user' => a user object (of the page being visited
 *   'exchange_type' => the name of an exchange_type
 *   'cid' => a currency node ID
 * )
 * this default function doesn't attempt to cover all the possible permutations
 * only username. Much more could be done, particularly with transaction type,
 * but its too many permutations for a general purpose module
 *
 */
function theme_mc_webform_title($options = array()){
  if (array_key_exists('user', $options)) {
    return t('Trade with @user', array('@user' => strip_tags(theme('username', $options['user']))));
  }
  if (isset($options['cid'])) {
    $currencies = array(node_load($options['cid']));
  }
  else $currencies = currencies_load();
  if (array_key_exists('exchange_type', $options)) {
    $exchange_types = array($options['exchange_type']);
  }
  elseif (count($currencies) == 1){
    $exchange_types = get_common_exchange_subtypes($currencies);
  }

  if(isset($exchange_types) && count($exchange_types) == 1) {
    return array_pop($exchange_types);
  }
  else return t('1st Party');
}

function theme_subtype_names($element){
  foreach (element_children($element) as $key) {
    unset ($element[$key]['#title']);
  }
  //put these in a table.
  $rows = array(
    array('<strong>'.t('Direct').'</strong>', drupal_render($element[1]), drupal_render($element[2]))
  );
  if (module_exists('mc_signatures')) {
    $rows[] = array('<strong>'.t('Signed').'</strong>', drupal_render($element[4]), drupal_render($element[8]));
  }
  return '<h4>'.t('Exchange types').'</h4>' .
    theme('table', array('', t('Incoming'), t('Outgoing')), $rows);
}


//assumes that types to be converted are 'offer' and 'want'
function mc_prepopulate_1stparty($node, $type) {
  $prefill = array(
    'title' => $node->title,
    'completer' => $node->uid,
  );
  $type_defs = module_invoke_all('exchange_type_info');
  $dir = $type == 'offer' ? 'outgoing' : 'incoming';
  //gets the first available exchange type in the right direction
  foreach (array_keys($type_defs) as $exchange_type) {
    if (substr($exchange_type, 0,8) == $dir) break;
  }
  $prefill['exchange_type'] = $exchange_type;

  return drupal_get_form('mc_1stparty_form', $prefill);
}
