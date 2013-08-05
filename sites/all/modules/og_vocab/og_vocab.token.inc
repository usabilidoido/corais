<?php

/**
 * @file
 * Integration of token module with og_vocab module.
 */

/**
 * Implementation of hook_token_values().
 */
function og_vocab_token_values($type, $object = NULL, $options = array()) {
  if ($type == 'taxonomy' && !empty($object) && $nid = og_vocab_get_group($object->vid)) {
    $node = node_load($nid['nid']);
    $tokens = array(
      'ogname' => check_plain($node->title),
      'ogname-raw' => $node->title,
      'og-id' => $nid['nid'],
    );
    return $tokens;
  }
}

/**
 * Implementation of hook_token_list().
 */
function og_vocab_token_list($type = 'all') {
  if ($type == 'taxonomy' || $type == 'all') {
    $tokens['taxonomy'] = array(
      'ogname' => t('The title of the organic group the category belongs to.'),
      'ogname-raw' => t('The unfiltered title of the organic group the category belongs to. WARNING - raw user input.'),
      'og-id' => t('ID of the organic group the category belongs to.'),
    );
    return $tokens;
  }
}