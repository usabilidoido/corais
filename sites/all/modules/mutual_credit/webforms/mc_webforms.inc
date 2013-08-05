<?php
// $Id: mc_webforms.inc,v 1.1.2.4 2010/12/22 19:30:29 matslats Exp $

/*
 * 3rd party form. This is for third parties to create exchanges
 * It's pretty close to the api's default node form, so not too much code is required.
 */
function mc_3rdparty_form($form_state, $prefill = NULL) {
  if (!$form_state['storage']['step']) {
    $form_state['storage']['step'] = 1;
  }
  $values = isset($form_state['storage']['values']) ? $form_state['storage']['values'] : '' //this is for stage 1 errors and stage 2
    or $values = $form_state['post'] //for the AHAH in stage 1
    or $values = $prefill //passed from mc_webform_block or menu callback
    or $values = array();
  if (is_numeric($values)) {
    $values = array('cid' => $values); //this is from the menu callback
  }
  if ($form_state['storage']['step'] == 1){
  
    // gets the first currency associated with the current space
  $currencies = views_get_view_result("currency");
  $nid = $currencies[0]->nid;
  $values['cid'] = $nid;
      
    //get the default exchange form, with defaults
    $passed_currencies = $values['cid'] ? array($values['cid'] => node_load($values['cid'])) : NULL;
    $form = mcapi_form_exchange((object)$values, $passed_currencies);

    mc_build_webform($form, $values);
    //there are some circumstances the ajax is not needed, but I'm not going to make all the rules now
    //act of choosing the currency updates the form to show the divisions, if the currency has them
    if (!isset($form['cid']['#value'])) {
      mc_ajaxify($form['cid'], 'mc_3rdparty_form');
    }
    //in case the form is in a block, we need to specify the url for step 2
    $form['#action'] = url('exchange/3rdparty');
  }
  elseif ($form_state['storage']['step'] == 2){
    $form = exchange_step2_form($values);
    //this is a bit of a hack, to hide the tabs on the exchange form, but it works and shouldn't cause any problems
    if ($selector = variable_get('mc_tabs_css', NULL)) {
      $form['hide_tabs'] = array(
        '#value' => '<style>'. $selector. '{display:none;}</style>',
      );
    }
    $form['#redirect'] = 'user';
  }
  return $form;
}


function mc_3rdparty_form_validate($form, &$form_state) {
  //the individual fields are already validated
  if ($form_state['storage']['step'] < 2) {
    //send the pseudo-node off for validation by the mcapi
    mcapi_validate((object)$form_state['values'], $form);
  }
}

//submission handler to control the form rebuilding and value storage
//returns TRUE if the form needs to be rebuilt
function mc_submit_multistep(&$form_state){
  if (arg(1) == 'ajax' || !variable_get('mc_webform_sure', TRUE) || $form_state['storage']['step'] == 2) {
    if (isset($form_state['storage']['values'])) {
      $form_state['values'] += $form_state['storage']['values'];
    }
    //clear the storage so that the form doesn't rebuild
    unset($form_state['storage']);
    return FALSE;
  }
  //multistep processing
  if (!isset($form_state['storage']['values'])) {
    $form_state['storage']['values'] = array();
  }
  //increment the button, save the values, and return
  if ($form_state['clicked_button']['#id'] == 'edit-back') {
    $form_state['storage']['step']--;
    $form_state['storage']['values'] = $form_state['values'] + $form_state['storage']['values'];
    $form_state['values']['quantity']['integer'] = intval($form_state['values']['quantity']);
    return TRUE;
  } elseif ($form_state['storage']['step'] < 2) {
    $form_state['storage']['step']++;
    $form_state['storage']['values'] = $form_state['values'] + $form_state['storage']['values'];
    return TRUE;
  }
  else {
    drupal_set_message('error in mc_webforms: mc_submit_multistep()');
  }
}

//process the exchange form values create the exchange and goto
function mc_3rdparty_form_submit($form, &$form_state) {
  //don't submit if this isn't the final step
  if (mc_submit_multistep($form_state)) return;
  $result = mc_webform_create($form_state['values']);
  drupal_goto(variable_get('mc_webform_destination', 'user'));
}

function template_preprocess_mc_3rdparty_formspecial(&$vars) {
  //render the exchange_type as simple text if there's no choice to be made
  if ($vars['form']['exchange_type']['#type'] == 'hidden' && isset($vars['form']['exchange_type']['#options'])) {
    $vars['form']['exchange_type']['#type'] = 'markup';
    $vars['form']['exchange_type']['#value'] = $vars['form']['exchange_type']['#options'][$vars['form']['exchange_type']['#value']];
  }
  $vars['hidden'] = '';
  foreach (element_children($vars['form']) as $widget) {
    if (isset($vars['form'][$widget]['#type']) && ($vars['form'][$widget]['#type'] == 'hidden' || $vars['form'][$widget]['#type'] == 'token')) {
      //this puts all the hidden bits in one variable
      $vars['hidden'] .= drupal_render($vars['form'][$widget]);
    }
    else {
      $vars[$widget] = drupal_render($vars['form'][$widget]);
    }
  }
}

function exchange_step2_form($values) {
  drupal_set_title(t('Is this the exchange you want to record?'));
  if (strpos($values['exchange_type'], 'signed')) $values['state'] = EXCHANGE_STATE_PENDING;
  $form['#values'] = $values; //this can be used in theming
  $form['type'] = array(
    '#type' => 'hidden',
    '#value' => 'exchange',
  );
  //don't change the key values of these buttons!
  $form['buttons']['back'] = array(
    '#type' => 'submit',
    '#value' => t('Back'),
    '#weight' => -1,
    //using js back button returns to some unfilled fields
    //but it's better than nothing....
    '#attributes' => array('onclick' => 'history.go(-1);return false;')
  );
  $form['buttons']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
    '#weight' => -1
  );
  $form['#theme'] = 'mc_web_create_confirm_form';
  return $form;
}


function template_preprocess_mc_web_create_confirm_form($vars) {
  //we used to use the node teaser for these, using theme('node', $node, TRUE FALSE),
  //but at this point we don't have a node to offer
  $vars['teaser'] = theme('exchange_summary', (object)$vars['form']['#values']);
  $vars['buttons'] = drupal_render($vars['form']['buttons']);
  $vars['hidden'] = drupal_render($vars['form']);
}

//rather than use the node edit form, it's better to control it completely here
//in some cases (but not this module), it will be appropriate to use the same form for creating and editing.
//This module emphasies flexibility on the creation form, and then limits the number of fields which can be edited
function mc_edit_form($form_state, $node) {

  $form = mcapi_form_exchange($node);
  mc_build_webform($form, (array)$node);
  $form['nid'] = array(
    '#type' => 'hidden',
    '#value' => $node->nid
  );

  //this should be in hook_help, if anywhere
  //$form['#prefix'] = t("If you can't edit what you need to, delete this exchange and make a new one");

  //it's' too much trouble right now to sort out the rules which participants can be safely edited,
  //especially considering they may have been emailed asking for their signatures.

  if (user_access('edit all exchanges')) {
    $form['buttons']['erase'] = array(
      '#type' => 'submit',
      '#value' => t('Erase'),
      '#submit' => array('node_form_delete_submit')
    );
  }

  $form['#validate'] = array('mc_3rdparty_form_validate');
  //$form['#redirect'] = 'node/'.$node->nid; //this breaks the confirm_form button
  //change the text on the submit button
  $form['buttons']['submit']['#value'] = t('Save');
  return $form;
}


function mc_edit_form_submit($form, $form_state) {
  //it is only possible to have edited certain fields.
  $node = node_load($form_state['values']['nid']);
  foreach($form_state['values'] as $key => $value) {
    $node->$key = $value;
  }
  //unsigned cannot be affected in this submit handler
  node_save($node);
 drupal_goto('node/'.$node->nid);
}

function mc_webform_settings() {
  $form['mc_webform_sure'] = array(
    '#type' => 'checkbox',
    '#title' => t('Confirmation page'),
    '#description' => t("Display an 'Are you sure?' page when recording an exchange"),
    '#default_value' => variable_get('mc_webform_sure', TRUE),
    '#weight' => -5,
  );

  $form['mc_webform_destination'] = array(
    '#type' => 'textfield',
    '#title' => t('Destination'),
    '#description' => t('The exchange form should end up on which drupal path?'),
    '#default_value' => variable_get('mc_webform_destination', 'user'),
    '#weight' => -4,
  );
  $form['trader_selection'] = array(
    '#type' => 'fieldset',
    '#title' => t('Trader Selection'),
    '#description' => t("Users often need to select other users in forms, but sometimes a dropdownlist of usernames ordered by uid isn't the most usable solution"),
    '#weight' => -3
  );
  $form['trader_selection']['mc_trader_autocomplete'] = array(
    '#type' => 'checkbox',
    '#title' => t('Autocomplete trader names in transaction form'),
    '#default_value' => variable_get('mc_trader_autocomplete', TRUE),
    '#description' => t('Reccommended above about 150 users. If not enabled, will use selection widget below.'),
    '#weight' => 1,
  );
  $form['trader_selection']['mc_trader_selector'] = array(
    '#type' => 'textfield',
    '#title' => t('Widget for selecting users'),
    '#default_value' => variable_get('mc_trader_selector', '%uid | %theme_username'),
    '#weight' => 2,
    '#description' => t('Use the following tokens: %uid, %username, %email, %theme_username') .' '.
      t('Dropdown list will be ordered alphabetically.') .' '.
      t('In Drupal 7 any user field will be allowed')
  );
  $form['theming'] = array(
    '#type' => 'fieldset',
    '#title' => t('Theme adjustments'),
    '#weight' => -3
  );
  $form['theming']['mc_webform_special_theme'] = array(
    '#type' => 'radios',
    '#title' => t('Special theming mode'),
    '#default_value' => variable_get('mc_webform_special_theme', TRUE),
    '#required' => TRUE,
    '#options' => array(
      FALSE => t("Start with the Drupal defaults, and theme forms as normal"),
      TRUE => t('Use the special theming features.')
    ),
    '#description' => t('Transaction forms need to be exactly right. Special theming mode offers a ready-made transaction form and some form-widget theme overrides which avoid !theme_form_element ',
        array('!theme_form_element' => l('theme_form_element()', 'http://api.drupal.org/api/function/theme_form_element/6'))) .' '.
      t('Then, using the provided tpl.php, you can arrange things more exactly as you want.') .' ',
  );
  $form['theming']['mc_tabs_css'] = array(
    '#type' => 'textfield',
    '#title' => t('CSS selector for the menu tabs'),
    '#default_value' => variable_get('mc_tabs_css', ''),
    '#description' => t("On step 2 of the multistep forms, it is desirable to hide the menu tabs.") .' '.
      t("Enter the css selector here which will be set to display:none")
  );
  return system_settings_form($form);
}
function mc_webforms_form_mc_webform_settings_alter(&$form, $form_state) {
  $form['#submit'][] = 'refresh_trader_names';
}


//common building tasks for all forms in this module
function mc_build_webform(&$form, $values = NULL) {

  $form['exchange_type'] = array(
    '#type' => 'hidden',
    '#value' => isset($values['exchange_type']) ? $values['exchange_type'] : '3rdparty',
  );
  //this can't be in mcapi module because the node-form tries to add and screws up.
  $form['type'] = array(
    '#type' => 'hidden',
    '#value' => 'exchange',
  );

  //if unaltered, this will go through as zero, or EXCHANGE_STATE_FINISHED
  $form['state']['#type'] = 'hidden';

  //see settings page. set up for special theming with normal or block callback.
  if (variable_get('mc_webform_special_theme', TRUE)) {
    $form['#after_build'][] = 'mc_specialtheme';
  }
  //this is a proxy field widget. The validation actually puts a float, quantity, into $form_state
  $form['mc_quantity'] = array(
    //this fieldtype is defined by hook_form_elements.
    '#type' => 'mc_quantity',
    //putting validate function here means it can be added to - in form elements it would be overwritten
    '#element_validate' => array('validate_mc_quantity_field'),
  ) + $form['quantity'];
  unset($form['quantity']);

  
  //more proxy fields
  foreach (array('payer', 'payee') as $trader) {
    $form[$trader] = array(
      '#type' => 'mc_trader',
      '#dest_field' => $trader.'_uid',
      //'#default_value' => isset($values[$trader]) ? $values[$trader] : '',
    );
    if (isset($values[$trader])) {
      $form[$trader]['#default_value'] = $values[$trader];
    }
    $form[$trader] += $form[$trader.'_uid'];
    unset($form[$trader.'_uid']); //this is replaced in the mc_trader's validate function
  }

  $form['buttons'] = array(
    'submit' => array(
      '#type' => 'submit',
      '#value' => t('Submit'),
    ),
    '#weight' => 30
  );
  $form['buttons']['submit']['#value'] = t('Next');
}

//because each exchangetype has it's own edit form this implements a new HOOK
function mc_edit_forms($node) {
  foreach (module_implements('exchange_type_info') as $module) {
    $function = $module .'_exchange_type_info';
    $types = $function ($node->exchange_type);
    $edit_callback = &$types[$node->exchange_type]['edit'];
    if (function_exists($edit_callback)) {
      return drupal_get_form($edit_callback, $node);
    }
  }
  module_load_include('pages.inc', 'node');
  return drupal_get_form($node->type .'_node_form', $node);
}

function theme_select_stripped($element) {
  $select = '';
  $size = isset($element['#size']) ? ' size="'. $element['#size'] .'"' : '';
  _form_set_class($element, array('form-select'));
  $multiple = isset($element['#multiple']) ? $element['#multiple'] : 0;
  return '<select name="'. $element['#name'] .''. ($multiple ? '[]' : '') .'"'. ($multiple ? ' multiple="multiple" ' : '') . drupal_attributes($element['#attributes']) .' id="'. $element['#id'] .'" '. $size .'>'. form_select_options($element) .'</select>';
  //return theme('form_element', $element, '<select name="'. $element['#name'] .''. ($multiple ? '[]' : '') .'"'. ($multiple ? ' multiple="multiple" ' : '') . drupal_attributes($element['#attributes']) .' id="'. $element['#id'] .'" '. $size .'>'. form_select_options($element) .'</select>'
}
function theme_textfield_stripped($element) {
  $size = empty($element['#size']) ? '' : ' size="'. $element['#size'] .'"';
  $maxlength = empty($element['#maxlength']) ? '' : ' maxlength="'. $element['#maxlength'] .'"';
  $class = array('form-text stripped');
  $extra = '';
  $output = '';

  if ($element['#autocomplete_path'] && menu_valid_path(array('link_path' => $element['#autocomplete_path']))) {
    drupal_add_js('misc/autocomplete.js');
    $class[] = 'form-autocomplete';
    $extra =  '<input class="autocomplete" type="hidden" id="'. $element['#id'] .'-autocomplete" value="'. check_url(url($element['#autocomplete_path'], array('absolute' => TRUE))) .'" disabled="disabled" />';
  }
  _form_set_class($element, $class);

  if (isset($element['#field_prefix'])) {
    $output .= '<span class="field-prefix">'. $element['#field_prefix'] .'</span> ';
  }

  $output .= '<input type="text"'. $maxlength .' name="'. $element['#name'] .'" id="'. $element['#id'] .'"'. $size .' value="'. check_plain($element['#value']) .'"'. drupal_attributes($element['#attributes']) .' />';

  if (isset($element['#field_suffix'])) {
    $output .= ' <span class="field-suffix">'. $element['#field_suffix'] .'</span>';
  }
  return $output . $extra;
}

function theme_checkbox_stripped($element) {
  _form_set_class($element, array('form-checkbox'));
  $checkbox = '<input ';
  $checkbox .= 'type="checkbox" ';
  $checkbox .= 'name="'. $element['#name'] .'" ';
  $checkbox .= 'id="'. $element['#id'] .'" ' ;
  $checkbox .= 'value="'. $element['#return_value'] .'" ';
  $checkbox .= $element['#value'] ? ' checked="checked" ' : ' ';
  $checkbox .= drupal_attributes($element['#attributes']) .' />';

  if (!is_null($element['#title'])) {
    $checkbox = '<label class="option" for="'. $element['#id'] .'">'. $checkbox .' '. $element['#title'] .'</label>';
  };
  if (!empty($element['#description'])) {
    $checkbox .= ' <div class="description">'. $element['#description'] ."</div>\n";
  }

  return $checkbox;
}
//we need this theme function because otherwise we don't know which widget to use
function theme_mc_trader($element) {
  if (!isset($elements['#printed'])) {
    return theme($element['#element_type'], $element);
  }
}
function theme_mc_trader_stripped($element) {
  if (!isset($elements['#printed'])) {
    return theme($element['#element_type'] . '_stripped', $element);
  }
}
function theme_mc_quantity($element) {
  $currname = isset($element['#single_currency_name']) ? $element['#single_currency_name'] : '';
  $naked = '<div class="container-inline">'. $element['#children'] . $currname .'</div>';
  //assumes the children have been themed with theme_form_item().
  //Under no circumstances to we want to view the labels for the child fields
  foreach (element_children($element) as $child) {
    $classes[] = '#'. $element[$child]['#id'] .'-wrapper label';
  }
  $css = "<style>".implode(',', $classes)."{display:none;}</style>\n";
  //it's not supposed to be this way, but overrides of this theme wouldn't work, looks to me like drupal render is badly thought out
  //so this theme function checks a variable from mc_webforms. the alternative is to override this in mc_webforms with phptemplate_mc_quality

  if (variable_get('mc_webform_special_theme', TRUE)) {
    return $css.$naked;
  }
  else return theme('form_element', $element, $css.$naked);
}

//apply the 'special' theme functions and templates to any form (this is a form #after_build function)
function mc_specialtheme($form, $form_state) {
  //set the form #theme to it's 'special' template; the edit form mc_edit_form borrows the 3rdparty form
  $baseform_id = $form['form_id']['#value'] == 'mc_edit_form' ? 'mc_3rdparty_form' : $form['form_id']['#value'];
  $form['#theme'] = $baseform_id.'special';
  //make the form elements 'special' - only works for immediate children tho
  foreach (element_children($form) as $fieldname) {
    if (isset($form[$fieldname]['#type']) && in_array($form[$fieldname]['#type'], array('textfield', 'select', 'mc_trader', 'checkbox'))) {
      $form[$fieldname]['#theme'] = $form[$fieldname]['#type'].'_stripped';
    }
  }
  return $form;
}
//other args are the fieldnames
function mc_ajaxify(&$element, $form_id) {
  if ($element['#type'] == 'hidden') return;
  $element['#ahah'] = array(
    'path' => 'exchange/ajax',
    'wrapper' => str_replace(array('][', '_', ' '), '-', $form_id), //see form_clean_id()
    'progress' => array('type' => 'throbber')
  );
}

//the $element has been validated, now we set the payer and payee in $form_state
function mc_trader_validate($element, &$form_state) {
  if (!$element['#value']) {
    form_error($element, t('There must be two participants in any exchange.'));
  }
  $uid = _mc_check_trader($element['#value'], $element); //this delievers it's own error messages

  $form_state['values'][$element['#dest_field']] = $uid;
}


//ajax menu callback
function mc_ajax_webform($block = FALSE) {
  //this is forgetting the contents of the autocomplete fields
  $form = ajax_form_callback();
  //to prevent rendering the form tag itself, since we are replacing only the contents of the form tag
  $form['#printed'] = TRUE;
  //this is probably redundant
  if ($block) {
    $form['#theme'] = 'mc_web_create_form_block';
  }
  //clear the form errors and messages before re-rendering the form
  form_set_error(NULL, NULL, TRUE);
  drupal_get_messages(NULL, TRUE);
  $rendered = drupal_render($form);
  drupal_json($rendered);
}

function mc_prepopulate_3rdparty($type, $node) {
  $prefill = array(
    'title' => $node->title,
  );
  switch ($type) {
    case 'outgoing':
      $prefill['payee_uid'] = $node->uid;
      if ($node->uid != $GLOBALS['user']->uid) {
        $prefill['payer_uid'] = $GLOBALS['user']->uid;
      }
    break;
    case 'incoming':
      $prefill['payer_uid'] = $node->uid;
      if ($node->uid != $GLOBALS['user']->uid) {
        $prefill['payee_uid'] = $GLOBALS['user']->uid;
      }
    break;
    default:
      drupal_set_message(t('Unknown content type in link.'));
  }
  $prefill['exchange_type'] = '3rdparty';

  return drupal_get_form('mc_3rdparty_form', $prefill);
}

/*
 * Implementation of hook_form_FORM_ID_alter
 * put the taxonomy widgets and cck widgets on the 3rdparty and edit forms
 * this is mostly copied from those modules, with a few alterations.
 */
function  mc_webforms_form_mc_3rdparty_form_alter(&$form, $form_state) {
  mc_webforms_extra_fields($form, $form_state);
}
function  mc_webforms_form_mc_edit_form_alter(&$form, $form_state) {
  mc_webforms_extra_fields($form, $form_state);
}


function mc_webforms_extra_fields(&$form, $form_state) {
  //spoof the node object and put it in the $form
  if (isset($form['nid'])) {
    $node = node_load($form['nid']['#value']);
    $form['#node'] = $node;
  }
  else {
    $form['#node'] = (object)array('type' => 'exchange');
  }
  if (module_exists('taxonomy')) {
    if (isset($form_state['values']['taxonomy'])) {
      foreach ($form_state['values']['taxonomy'] as $id) {
        if (is_array($id)) {
          foreach ($id as $tid) {
            $terms[$tid] = taxonomy_get_term($tid);
          }
        }
        else {
          $terms[$id] = taxonomy_get_term($id);
        }
      }
    }
    elseif (!isset($node->taxonomy)) {
      $terms = empty($node->nid) ? array() : taxonomy_node_get_terms($node);
    }
    else {
      // After a preview or form reload, the terms must be converted to objects.
      reset($node->taxonomy);
      if (!is_object(current($node->taxonomy))) {
        $node->taxonomy = taxonomy_preview_terms($node);
      }
      $terms = $node->taxonomy;
    }
    $c = db_query(db_rewrite_sql("SELECT v.* FROM {vocabulary} v INNER JOIN {vocabulary_node_types} n ON v.vid = n.vid WHERE n.type = '%s' ORDER BY v.weight, v.name", 'v', 'vid'), 'exchange');

    while ($vocabulary = db_fetch_object($c)) {
      if ($vocabulary->tags) {
        if (isset($form_state['node_preview'])) {
          // Typed string can be changed by the user before preview,
          // so we just insert the tags directly as provided in the form.
          $typed_string = $node->taxonomy['tags'][$vocabulary->vid];
        }
        else {
          $typed_string = taxonomy_implode_tags($terms, $vocabulary->vid) . (array_key_exists('tags', $terms) ? $terms['tags'][$vocabulary->vid] : NULL);
        }
        $form['taxonomy']['tags'][$vocabulary->vid] = array(
          '#type' => 'textfield',
          '#title' => $vocabulary->name,
          '#required' => $vocabulary->required,
          '#default_value' => $typed_string,
          '#autocomplete_path' => 'taxonomy/autocomplete/' . $vocabulary->vid,
          '#weight' => $vocabulary->weight,
          '#maxlength' => 1024,
        );
      }
      else {
        // Extract terms belonging to the vocabulary in question.
        $default_terms = array();
        foreach ($terms as $term) {
          // Free tagging has no default terms and also no vid after preview.
          if (isset($term->vid) && $term->vid == $vocabulary->vid) {
            $default_terms[$term->tid] = $term;
          }
        }
        $form['taxonomy'][$vocabulary->vid] = taxonomy_form($vocabulary->vid, array_keys($default_terms), filter_xss_admin($vocabulary->help));
        $form['taxonomy'][$vocabulary->vid]['#weight'] = $vocabulary->weight;
        $form['taxonomy'][$vocabulary->vid]['#required'] = $vocabulary->required;
      }
    }
    if (!empty($form['taxonomy']) && is_array($form['taxonomy'])) {
      if (count($form['taxonomy']) > 1) {
        // Add fieldset only if form has more than 1 element.
        $form['taxonomy'] += array(
          '#type' => 'fieldset',
          '#title' => t('Vocabularies'),
          '#collapsible' => TRUE,
          '#collapsed' => FALSE,
        );
      }
      $form['taxonomy']['#weight'] = -3;
      $form['taxonomy']['#tree'] = TRUE;
    }
  }
  //and the cck widgets
  if (module_exists('content') && isset($form['type'])) {
    $type = content_types($form['#node']->type);
    if (!empty($type['fields'])) {
      module_load_include('inc', 'content', 'includes/content.node_form');
      // Merge field widgets.
      $type = content_types($form['type']['#value']);
      //not sure about this... the second time this runs the $form has no fields
      if (count($type['fields']))
      $form = array_merge($form, content_form($form, $form_state));
    }
    $form['#pre_render'][] = 'content_alter_extra_weights';
    $form['#content_extra_fields'] = $type['extra'];
  }
}
