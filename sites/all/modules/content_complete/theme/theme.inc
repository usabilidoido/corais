<?php
// $Id: theme.inc,v 1.1.2.8 2010/10/08 14:02:37 pvhee Exp $

/**
 * @file
 * Theme and template preprocessing code
 * 
 * TODO refactor inside preprocess functions
 */
 
/**
 * Block Theme function that displays the default output of a content
 * complete percent. Use this theme function to override
 * the output / display of this block.
 *
 * @param $complete_data
 *   Assoc array of
 *   ['type'] - str - The machine-readable name of this content type.
 *   ['name'] - str - The human-readable name of this content type.
 *   ['nid'] - int - The node ID of the node from this content type.
 *   ['percent'] - int - A number that represents the total percent complete (ie 50).
 *   ['leq_percent'] - int - A number that the current percent is lower or equal to (ie 25, 50, 75 or 100).
 *   ['completed'] - int - How many fields total that have been completed (filled out).
 *   ['incomplete'] - int - How many fields still need to be filled out.
 *   ['total'] - int - The count of all tagged profile fields.
 *   ['nextfield'] - str - The next field to fill out that is currently empty.
 *   ['nextpercent'] - int - What the total percent complete value will be when ['nextfield'] is complete.
 *   ['nextname'] - str - The field name of the next field for field focus.
 */
function theme_content_complete_profile_percent_complete($complete_data) {  
  // add CSS for theming
  drupal_add_css(drupal_get_path('module', 'content_complete') .'/content_complete.css');
    
  $output .= '<div id="content-complete-wrapper" class="content-complete-complete-wrapper">';
  
  if (isset($complete_data['nextpercent'])) {
    $output .= t('!complete% complete', array(
      '!complete' => $complete_data['percent'] 
    ));
  }
  
  // Divide percentages in 4 regions of 25 each
  $complete_data['leq_percent']=0;
  while ($complete_data['leq_percent']<=100) {
    if ($complete_data['percent'] <= $complete_data['leq_percent']) break;
    $complete_data['leq_percent']+=25;
  }
  
  if (!isset($complete_data['percent'])) {
    $complete_data['percent'] = 0; // protect against unset values
  }

  $output .= '<div id="content-complete-percent-bar-wrapper" class="content-complete-percent-bar-wrapper">';
  $output .= '<div id="content-complete-percent-bar" class="content-complete-percent-bar content-complete-percent-bar-leq-'. $complete_data['leq_percent'] .' content-complete-percent-bar-'. $complete_data['percent'] .'" style="width: '. $complete_data['percent'] .'%"></div>';
  $output .= '</div>';
  $output .= '</div>';

  if (isset($complete_data['edit']) && $complete_data['edit'] && isset($complete_data['nextfield']) && isset($complete_data['nextpercent'])) {
    $output .= t('Filling out <i>!nextfield</i> will bring !typename to !complete%.', array(
      '!nextfield' => l(t($complete_data['nextfield']), 'node/'. $complete_data['nid'] .'/edit', array(
        'query' => 'content_complete_fieldname='. $complete_data['nextname'] 
      )),
      '!typename' => $complete_data['name'],
      '!complete' => $complete_data['nextpercent']
    ));
  }
  
  return $output;
}