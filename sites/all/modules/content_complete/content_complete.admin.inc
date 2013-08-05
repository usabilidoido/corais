<?php
// $Id: content_complete.admin.inc,v 1.1.2.8 2009/09/11 15:24:36 pvhee Exp $ 

/** 
 * @file 
 * Administration page callbacks for the content_complete module. 
 */

/**
 * Menu Callback Function.
 */
function content_complete_admin_settings() {
  return drupal_get_form('content_complete_admin_settings_form');
}

/**
 * Admin settings form.
 */
function content_complete_admin_settings_form() {
  $form['info'] = array(
    '#prefix' => '<div>', 
    '#value' => t('Rebuild the complete cache for all nodes of a Content-Complete-enabled type, using the Batch API.'),
    '#suffix' => '</div>',
  );
  
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Rebuild Cache'),
  );
  
  return $form;
}

/** 
 * Handle form submission by beginning batch operation. 
 */
function content_complete_admin_settings_form_submit($form_id, &$form_state) {
  $batch = array(
    'title' => t('Rebuilding Cache'),
    'operations' => array( 
      array('content_complete_rebuild_cache', array()), 
    ), 
    'finished' => 'content_complete_rebuild_cache_finished', 
    'init_message' => t('The Content Complete cache rebuilding process is beginning.'), 
    'error_message' => t('The Content Complete cache rebuilding process encountered an error.'), 
    'file' => drupal_get_path('module', 'content_complete') .'/content_complete.admin.inc',
  ); 
  batch_set($batch); 
}



/** 
 * Batch callback operation: Import users. 
 * 
 * @param $context 
 *  Batch context containing state information. 
 */ 
function content_complete_rebuild_cache(&$context) {
  // How many node cache rebuilds to process in each batch.
  $limit = 50;
  
  // Initialize sandbox the first time through.
  if (!isset($context['sandbox']['progress'])) {
    // Clear the cache, we will rebuild it entirely.
    content_complete_clear_cache();
    
    // Types tagged for completeness (can be empty)
    $types = array_keys(content_complete_get_enabled_types()); 
    $list = "('". implode("','", $types) ."')";

    // Count the number of cache rebuilds we will have to do.
    $max = db_result(db_query('SELECT count(nid) FROM {node} WHERE type IN '. $list));    
    
    $context['sandbox']['progress'] = 0;
    $context['sandbox']['current_nid'] = 0;
    $context['sandbox']['list'] = $list;
    $context['sandbox']['max'] = $max;
    
  }
   
  // Retrieve range of nids to rebuild cache for
  $result = db_query_range(
    'SELECT nid FROM {node} WHERE nid > %d AND type in '. $context['sandbox']['list'] .' ORDER BY nid ASC',
    $context['sandbox']['current_nid'], 0, $limit
  );
    
  // Loop over all the nodes to process.
  while ($node = db_fetch_object($result)) {
    
    // Rebuild the cache, the actual operation.
    $node = node_load($node->nid);
    content_complete_get_data($node);
    
    // Store some result for post-processing in the finished callback.
    $context['results'][] = $node->nid;
        
    // Update progress information. 
    $context['sandbox']['progress']++; 
    $context['sandbox']['current_nid'] = $node->nid; 
    $context['message'] = t('Rebuilding complete cache for node %nid', array('%nid' => $node->nid)); 
  }
  
  // Let the batch engine know how close we are to completion. 
  if ($context['sandbox']['progress'] != $context['sandbox']['max']) { 
    $context['finished'] = $context['sandbox']['progress'] / $context['sandbox']['max']; 
  }
}

function content_complete_rebuild_cache_finished($success, $results, $operations) {  
  if ($success) {
    $message = format_plural(count($results), 'One node cache rebuilt.', '@count node caches rebuilt.');
    drupal_set_message($message); 
  } 
  else { 
    // A fatal error occurred during batch processing. 
    $error_operation = reset($operations); 
    $operation = array_shift($error_operation); 
    $arguments = array_shift($error_operation); 
    $arguments_as_string = implode(', ', $arguments);
    
    watchdog('content_complete', "Rebuild cache: error when calling operation '%s'('%s')", array($operation, $arguments_as_string)); 
    drupal_set_message(t('An error occurred and has been recorded in the system log.'), 'error'); 
  }
}

