<?php

/**
 * @file js.inc
 * Backend for javascript (AJAX) calls in Nodequeue Annotate
 */


/**
 * AJAX callback to add, add and annotate, edit annotation or remove nodes from subqueues.
 */
function nodequeue_annotate_ajax_form_postback($queue, $subqueue, $node) {
  // Check that this is an ajax postback
  if ($_POST['js'] != 1) {
    drupal_access_denied();
    exit();
  }
  
  // Get the annotation from the post
  $annotation = (string)$_POST['annotation'];
  
  $success = TRUE;
  switch ($_POST['action']) {
    case 'add':
      // Add the node to the subqueue
      nodequeue_subqueue_add($queue, $subqueue, $node->nid);
      // no break!
      
    case 'edit':
      // Set the next available action
      $action = 'remove-node';
      
      // Set or unset the annotation
      if (!empty($annotation)) {
        nodequeue_annotate_set_annotation($queue->qid, $subqueue->sqid, $node->nid, $annotation);
      }
      else {
        nodequeue_annotate_unset_annotation($queue->qid, $subqueue->sqid, $node->nid);
      }
      break;
    
    case 'remove-node':
      // Set the next available action
      $action = 'add';
      
      // Set an empty annotation for the next action
      $annotation = '';
      
      // Remove the node
      nodequeue_subqueue_remove_node($subqueue->sqid, $node->nid);
      break;
      
    default:
      // Unknown action - fail!
      $success = FALSE;
      break;
  }
  
  if ($success) {
    // Return a JSON compatible string.
    drupal_json(nodequeue_annotate_link_json($queue, $subqueue, $node, $annotation, $action));
  }
  else {
    drupal_access_denied();
  }
}
