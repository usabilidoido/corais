<?php

/**
 * @file certificate.api.php
 * Document certificate hooks.
 */

/**
 * Implementation of hook_access_certificate().
 *
 * Any module wishing to award a certificate based on arbitrary criteria should
 * implement this hook. The $node is the node in question, the $user is the user
 * viewing the node.
 *
 * @return mixed
 *   FALSE if user should not be shown the menu tab or link.
 *   TRUE if user should be able to download a certificate.
 *   A string if the user should be displayed a friendly message instead.
 */
function hook_access_certificate($node, $user) {
  if ($user->score > $node->pass_rate) {
    // Let the user get a certificate if they passed something.
    return TRUE;
  }
}

/**
 * Implementation of certificate_template_id_alter().
 *
 * Single the template ID that will be loaded when the user
 * downloads a certificate.
 */
function hook_certificate_template_id_alter(&$template_id, $node, $user) {
  if ($node->nid % 2 == 0) {
    // Set certificate to use node 476 when the node NID is even.
    $template_id = 476;
  }
}

/**
 * Implementation of hook_certificate_map_options().
 *
 * Provide a list of options to the user that can be mapped to certificate
 * templates.
 *
 * @return Array of mapping sets.
 */
function hook_certificate_map_options() {
  $options = array(
    'sad' => 'Sad',
    'happy' => 'Happy',
  );

  return array(
    'mood' => array(
      'title' => "User's mood",
      'options' => $options,
      'description' => 'Using this mapping will award a certificate based on what mood the user is currently in.',
    ),
  );
}

/**
 * Implementation of hook_certificate_map().
 *
 * Return the key of the mapping to use.
 *
 * @param stdClass $node
 * @param stdClass $user
 * @param string $map_type
 * @param array $options
 *   An array of keys that the user wants to check.
 *
 * @return String
 *   Key of matched mapping.
 */
function hook_certificate_map($node, $user, $map_type, $options) {
  if ($map_type == 'mood') {
    foreach ($options as $key) {
      if ($user->mood == $key) {
        // User's mood matched, so return the key. Certificate module will then
        // match the key to the template ID.
        return $key;
      }
    }
  }
}
