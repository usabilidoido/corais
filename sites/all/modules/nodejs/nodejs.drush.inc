<?php

/**
 * Implements hook_drush_help().
 */
function nodejs_drush_help($section) {
  switch ($section) {
    case 'drush:broadcast-message':
     return dt('Broadcast a message to all connected cliets.');
    case 'drush:user-message':
     return dt('Send a message to a given user.');
    case 'drush:kick-user':
     return dt('Kick the given user off the node.js server.');
  }
}

/**
 * Implements hook_drush_command().
 */
function nodejs_drush_command() {
  $items['broadcast-message'] = array(
    'callback' => 'nodejs_broadcast_message',
    'description' => 'Broadcast a message to all connected cliets.',
    'arguments' => array(
      'subject' => 'The subject of the message to broadcast.',
      'body' => 'The body of message to broadcast.',
    ),
  );
  $items['user-message'] = array(
    'callback' => 'nodejs_send_user_message',
    'description' => 'Send a message to a given user.',
    'arguments' => array(
      'uid' => 'The user id of the message recipient.',
      'subject' => 'The subject of the message.',
      'body' => 'The body of message.',
    ),
  );
  $items['kick-user'] = array(
    'callback' => 'nodejs_kick_user',
    'description' => 'Kick the given user off the node.js server.',
    'arguments' => array(
      'uid' => 'The user id of the person to be kicked off.',
    ),
  );
  return $items;
}

