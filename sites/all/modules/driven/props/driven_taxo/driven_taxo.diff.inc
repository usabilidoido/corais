<?php
// $Id: driven_taxo.diff.inc,v 1.2 2010/04/01 01:08:01 arhak Exp $

/**
 * Implements hook_diff_values().
 */
function driven_taxo_diff_values($element, $form_values, $meta) {
  // $node_type, $realm, $target, $provider,
  // $vid, $hierarchy, $multiple, $tags, [$voc_override]
  extract($meta);

  $voc = driven_taxo_get_vocabulary($vid, $node_type);
  if (!$voc) {
    // can't guess, can't normalize $value
    // can't afford being asked for diff_render
    return FALSE; // nothing to declare (i.e. return no diff_values)
  }
  
  // @per-module-discrimination: support taxonomy-related content_taxonomy widgets
  // (this also supports any custom delegation)
  if (isset($voc_override)) {
    $voc = (object)array_merge((array)$voc, $voc_override);
  }
  
  // beneath taxonomy:vid or taxonomy:tags:vid nothing is handled by core
  // until we meet special cases
  // (e.g. taxiselect uses extra checkbox & hidden, but the should be ignored as well)

  $default = driven_elem_default_value($element);
  // $value will be normized if needed (per case basis)
  // to match #default_value format instead of $form_state['values']
  $value = driven_elem_state_value($element, $form_values);
  
  switch ($element['#type']) {
    
    // support single/multiple select (default taxonomy behavior for non-tags)
    case 'select':
      // non-associative array
      $default = array_values($default);
      // $element['#multiple'] => associative array coming from drupal_map_assoc
      // otherwise plain number (tid)
      if ($element['#multiple']) {
        $value = array_values($value);
        
        // handle option "none"
        // even when $value may have several integer elements
        // option "none" might be selected as well
        // being an empty string which
        // shouldn't be delivered to be compared against int or NULL
        // since there is no tid=0, we can filter empty elements
        $value = array_filter($value); // normalized to #default_value format
        
        if (driven_equal_array_values($default, $value)) {
          return FALSE;
        }
      }
      else {
        $default = array_shift($default);
        
        // handle option "none"
        if ($value === '') {
          // shouldn't deliver string to be compared against int or NULL
          $value = NULL; // normalized to #default_value format
        }
        
        if (_driven_equal_non_array($default, $value)) {
          return FALSE;
        }
      }
      break;
    
    // support tags
    case 'textfield':
      // taxonomy_single_tag delegates to core, therefore no special case for it
      // besides, the imploded version will be recorded, therefore a textfield is always a string
      // which has no conflict with: active_tags, taxiselect, etc
      
      // avoid comparing zero vs string which gives TRUE
      if (_driven_equal_non_array($default, $value)) {
        return FALSE;
      }
      break;
          
/*
    // @TODO: @per-module-discrimination: support betterselect
    // which tweaks taxonomy:vid=>select into taxonomy:vid:tid=>checkbox
    // but they have a bug picking up the #default_value
    // [#705984]: broken default_value into betterselect_process
    // PS: I love the proposal of this module, shame it is so buggy
    case 'checkboxes':
      // strip unchecked checkboxes?
      $default = array_filter($default);
      // strip unchecked checkboxes?
      $value = array_filter($value);
      // compare their keys
      $default = array_keys($default);
      //$value = array_keys($value); // but leave $value normalized (don't change it any more)
      if (driven_equal_array_values($default, array_keys($value))) {
        return FALSE;
      }
      break;
*/
      
    // @per-module-discrimination: support taxiselect
    case 'taxiselect_element':
      // non-associative array
      $default = array_values($default);
      // according to taxiselect_element_expand
      // $element['#multiple']: 0 == Unlimited, 1+ == Max #num (i.e. 1 means single value)
      // either way it is an associative array coming from drupal_map_assoc
      $value = array_values($value);
      if (driven_equal_array_values($default, $value)) {
        return FALSE;
      }
      break;
      
    // @per-module-discrimination: support hierarchical_select
    case 'hierarchical_select':
      // according to hierarchical_select_process:
      // "If #value is not an array, then form_set_value(), which is called by form_builder() will fail"
      // and with this it refers to #default_value as well, since they are handled the same way
      //
      // therefore it will be always a non-associative array in #default_value

      // @TODO: every call to driven_assertion_failed will be removed after alpha/beta stage
      $save_lineage = $element['#config']['save_lineage'];
      $drop_box = $element['#config']['dropbox']['status'];
      if ($voc->multiple != ($save_lineage | $drop_box)) driven_assertion_failed('$voc->multiple != (save_lineage|drop_box) (vid: ' . $voc->vid . ')');
      
      // guaranteed by HS
      // lineage => multiple 
      // dropbox => multiple 
      // multiple <=> lineage|dropbox
      if ($voc->multiple) {
        // handle option "none"
        // $value is set to NULL by HS
        if (is_null($value)) {
          $value = array(); // normalized to #default_value format 
        }
        
        // if HS is gonna save the lineage we can't ignore ascendants
        if (driven_equal_array_values($default, $value)) {
          return FALSE;
        }
      }
      else {
        // handle option "none"
        // $value is set to NULL by HS
        // (core's taxonomy would have it as an array with an empty string)
        // while $default would be an empty array
        // and $default[0] would be NULL
        $value = empty($value) ? array() : array($value);
        if (_driven_equal_non_array($default[0], $value[0])) {
          return FALSE;
        }
      }
      break;
      
    default:
      // @TODO: every call to driven_assertion_failed will be removed after alpha/beta stage 
      driven_assertion_failed('unhandled case for #type: ' . $element['#type']);
      
      // can't guess, can't normalize $value
      // can't afford being asked for diff_render
      return FALSE; // nothing to declare (i.e. return no diff_values)
  }
  return array($default, $value);
}

/**
 * Implements hook_diff_render().
 */
function driven_taxo_diff_render($old, $new, $meta) {
  // $node_type, $realm, $target, $provider,
  // $vid, $hierarchy, $multiple, $tags, [$voc_override]
  extract($meta);
  
  $voc = driven_taxo_get_vocabulary($vid, $node_type);
  if (!$voc) {
    // can't say, but if we got here was because diff_values delivered something (?!)
    driven_assertion_failed('Can\'t figure diff_render of a gone property: ' . $realm . ':' . $target);
    return array('?', '?');
  }
  
  // @per-module-discrimination: support taxonomy-related content_taxonomy widgets
  // (this also supports any custom delegation)
  if (isset($voc_override)) {
    $voc = (object)array_merge((array)$voc, $voc_override);
  }
  else {
    // @per-module-discrimination: support taxonomy_single_tag
    // $voc_override implies non-single_tag
    // since taxonomy_single_tag is not a widget used by other 3rd party modules
    //
    // taxonomy_single_tag delegates to core
    // but does its tweaks escaping/unescaping comma and quotes on form_alter/submit
    if (module_exists('taxonomy_single_tag')) {
      // according to taxonomy_single_tag_form_alter these are the conditions to alter the widget
      $vids_single_tag = variable_get('taxonomy_single_tag:vids', array());
      $single_tag = ($tags && in_array($vid, $vids_single_tag));
    }
  }
  //$hierarchy = $voc->hierarchy;
  $multiple = $voc->multiple;
  $tags = $voc->tags;
  
  // @per-module-discrimination: support taxonomy_single_tag
  if ($single_tag) {
    $outgoing = '';
    $incoming = '';
    
    if (!empty($old)) {
      if ($term = driven_taxo_get_term_by_name($vid, $old, TRUE)) {
        $outgoing = driven_diff_output('driven_diff_term', array('value' => $term));
        
        // @TODO: every call to driven_assertion_failed will be removed after alpha/beta stage 
        if (strtolower($term->name) !== strtolower($old)) driven_assertion_failed('tag actually not found: ' . $old);
      }
      else {
        // tags shouldn't complain about unexistency
        // since taxonomy module renders them for preview
        // lets support the same here
        $outgoing = check_plain($old);
      }
    }
    
    if (!empty($new)) {
      if ($term = driven_taxo_get_term_by_name($vid, $new, TRUE)) {
        $incoming = driven_diff_output('driven_diff_term', array('value' => $term));
                  
        // @TODO: every call to driven_assertion_failed will be removed after alpha/beta stage 
        if (strtolower($term->name) !== strtolower($new)) driven_assertion_failed('tag actually not found: ' . $new);
      }
      else {
        // tags shouldn't complain about unexistency
        // since taxonomy module renders them for preview
        // lets support the same here
        $incoming = check_plain($new);
      }
    }
    
    return array($outgoing, $incoming);
  }
  // no elseif, since we returned at sight
  
  $outgoing = array();
  $incoming = array();
  if ($tags) {
    // usual tags
    $old_terms = drupal_explode_tags($old);
    $new_terms = drupal_explode_tags($new);
    list($old_terms, $new_terms) = driven_diff_symmetric_diff($old_terms, $new_terms);
    
    foreach ($old_terms as $term_name) {
      if ($term = driven_taxo_get_term_by_name($vid, $term_name, TRUE)) {
        $content = driven_diff_output('driven_diff_term', array('value' => $term));
        // @TODO: every call to driven_assertion_failed will be removed after alpha/beta stage 
        if (strtolower($term->name) !== strtolower($term_name)) driven_assertion_failed('tag actually not found: ' . $term_name);
      }
      else {
        // tags shouldn't complain about unexistency
        // since taxonomy module renders them for preview
        // lets support the same here
        $content = check_plain($term_name);
      }
      $outgoing[] = driven_diff_output('driven_diff_outgoing', array(
        'content' => $content,
        'classes' => array('term', 'tag', 'vid-' . $vid),
      ));
    }
    
    foreach ($new_terms as $term_name) {
      if ($term = driven_taxo_get_term_by_name($vid, $term_name, TRUE)) {
        $content = driven_diff_output('driven_diff_term', array('value' => $term));
        // @TODO: every call to driven_assertion_failed will be removed after alpha/beta stage 
        if (strtolower($term->name) !== strtolower($term_name)) driven_assertion_failed('tag actually not found: ' . $term_name);
      }
      else {
        // tags shouldn't complain about unexistency
        // since taxonomy module renders them for preview
        // lets support the same here
        $content = check_plain($term_name);
      }
      $incoming[] = driven_diff_output('driven_diff_incoming', array(
        'content' => $content,
        'classes' => array('term', 'tag', 'vid-' . $vid),
      ));
    }
  }
  else {
    // non-tags
    
    // @per-module-discrimination: support hierarchical_select
    // check whether we are receiving arrays or plain tids
    // multiple <=> lineage|dropbox  (guaranteed by HS on vocabulary submit)
    // lets wrap it to use the same code and unwrap it afterwards
    //
    // a pitfall for HS and non-tags is changing the multiplicity of the vocabulary
    // after some changes were recorded in logs
    // then, if the vocabulary was multiple and suddenly pass to be single
    // diff_render will display them as comma separated list
    // without themed +/- prefixes
    // that's why we don't check wether the vocabulary is multiple
    // instead whether we are receiving arrays or tids
    // (there will be a similar problem with any CCK field changing multiplicity)
    //
    // remember that having live_render disabled will always display
    // the proper changes according to the time they were made
    // but won't reflect renamed terms or vocabularies
    //
    // we won't babysit, choice have to be made regarding live_render or not
    // this is a little babysit for HS (but have nothing similar for tags above)
    // (also if the multiplicity change happens without HS, error will come up)
    $wrapped = !is_array($old); 
    
    if ($wrapped) {
      // we got plain tids, but lets use the same code bellow
      // temporary convert them to arrays
      
      // according to core's taxonomy
      // option "none" will come as NULL for single select
      $old = is_null($old) ? array() : array($old);
      $new = is_null($new) ? array() : array($new);
    }
    else {
      // according to core's taxonomy
      // option "none" is filtered out for multiple select
      list($old, $new) = driven_diff_symmetric_diff($old, $new);
    }
    
    foreach ($old as $tid) {
      if ($term = driven_taxo_get_term($tid, $vid)) {
        $rendered_term = driven_diff_output('driven_diff_term', array('value' => $term));
        if ($multiple) {
          $rendered_term = driven_diff_output('driven_diff_outgoing', array(
            'content' => $rendered_term,
            'classes' => array('term', 'vid-' . $vid)
          ));
        }
        $outgoing[] = $rendered_term;
      }
      else {
        // value gone was already reported by driven_taxo_get_term
        // (avoid rendering empty sides if value is gone)
        $outgoing[] = '?';
      }
    }
    
    foreach ($new as $tid) {
      if ($term = driven_taxo_get_term($tid, $vid)) {
        $rendered_term = driven_diff_output('driven_diff_term', array('value' => $term));
        if ($multiple) {
          $rendered_term = driven_diff_output('driven_diff_incoming', array(
            'content' => $rendered_term,
            'classes' => array('term', 'vid-' . $vid)
          )); 
        }
        $incoming[] = $rendered_term;
      }
      else {
        // value gone was already reported by driven_taxo_get_term
        // (avoid rendering empty sides if value is gone)
        $incoming[] = '?';
      }
    }
    
    if ($wrapped) {
      // unwrap the tids from the temporary arrays
      // avoid rendering as multiple values
      // actually it won't matter an implode
      // of arrays with just one element, but lets be faithful
      $outgoing = array_shift($outgoing);
      $incoming = array_shift($incoming);
    }
  }
  return array($outgoing, $incoming);
}
