<?php

/**
 * @file
 * Rules integration for the og_vocab module.
 */

/**
 * Implementation of hook_rules_action_info().
 */
function og_vocab_rules_action_info() {
  $items['og_vocab_rules_action_assign_vocab'] = array(
    'label' => t('Assign vocabulary to group'),
    'arguments' => array(
      'group' => array(
        'type' => 'node',
        'label' => t('Group'),
      ),
      'taxonomy_vocab' => array(
        'type' => 'taxonomy_vocab',
        'label' => t('Taxonomy vocabulary that will be assigned to group'),
      ),
    ),
    'module' => 'Organic groups vocabulary',
  );
  $items['og_vocab_rules_action_remove_vocab'] = array(
    'label' => t('Remove vocabulary from group'),
    'arguments' => array(
      'taxonomy_vocab' => array(
        'type' => 'taxonomy_vocab',
        'label' => t('Taxonomy vocabulary that will removed from group'),
      ),
    ),
    'module' => 'Organic groups vocabulary',
  );
  $items['og_vocab_rules_action_remove_all_group'] = array(
    'label' => t('Remove all vocabulary from group'),
    'arguments' => array(
      'group' => array(
        'type' => 'node',
        'label' => t('Group'),
      ),
    ),
    'module' => 'Organic groups vocabulary',
  );
  return $items;
}

/**
 * Action: Assign vocabulary to group.
 */
function og_vocab_rules_action_assign_vocab($node, $vocab, $settings) {
  og_vocab_write_record($node->nid, $vocab->vid);
}

/**
 * Action: Remove vocabulary from group.
 */
function og_vocab_rules_action_remove_vocab($vocab, $settings) {
  og_vocab_remove_record($vocab->vid);
}

/**
 * Action: Remove vocabulary from group.
 */
function og_vocab_rules_action_remove_all_group($node, $settings) {
  og_vocab_remove_all_records($node->nid);
}
