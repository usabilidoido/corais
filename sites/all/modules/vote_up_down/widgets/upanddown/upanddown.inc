<?php

/**
 * @file
 * Up Down Widget.
 */

/**
 * Implementation of hook_vud_widgets(). (Specialized)
 */
function vud_upanddown_vud_widgets() {
  return array(
    'title' => t('Up and down'),
    'widget template' => 'widget',
    'alter template variables' => 'vud_widget_upanddown_alter_template_vars',
  );
}

function vud_widget_upanddown_alter_template_vars($template_type, &$variables) {
  $criteria = array(
    'content_type' => $variables['type'],
    'content_id' => $variables['content_id'],
    'value_type' => 'points',
    'tag' => $variables['tag'],
    'function' => 'positives'
  );
  $positives = (int)votingapi_select_single_result_value($criteria);
  $variables['up_points'] = $positives;

  $criteria = array(
    'content_type' => $variables['type'],
    'content_id' => $variables['content_id'],
    'value_type' => 'points',
    'tag' => $variables['tag'],
    'function' => 'negatives'
  );
  $negatives = (int)votingapi_select_single_result_value($criteria);
  $variables['down_points'] = $negatives;
}
