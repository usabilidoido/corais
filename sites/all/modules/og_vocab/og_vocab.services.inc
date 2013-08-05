<?php

/**
 * @file
 * Integration of services module with og_vocab module.
 */

/**
 * Implementation of hook_service().
 */
function og_vocab_service() {
  return array(
    // Get vocabs from group.
    array(
      '#method'           => 'og_vocab.getVocabs',
      '#callback'         => 'og_vocab_service_get_vocabularies',
      '#access callback'  => 'og_vocab_service_get_access',
      '#key'              => FALSE,
      '#args'             => array(
        array(
          '#name'         => 'nid',
          '#type'         => 'int',
          '#description'  => t('A node ID of a group node.')
        ),
      ),
      '#return'           => 'struct',
      '#help'             => t('Returns the vocabularies associated with a group.'),
    ),
    // Get group from vocab.
    array(
      '#method'           => 'og_vocab.getGroup',
      '#callback'         => 'og_vocab_service_get_group',
      '#access callback'  => 'og_vocab_service_get_access',
      '#key'              => FALSE,
      '#args'             => array(
        array(
          '#name'         => 'vid',
          '#type'         => 'int',
          '#description'  => t('Vocabulary ID.')
        ),
      ),
      '#return'           => 'struct',
      '#help'             => t('Returns the group ID associated with a vocabulary ID.'),
    ),
  );
}

/**
 * Service callback; Returns the vocabularies associated with a group.
 */
function og_vocab_service_get_vocabularies($nid){
  return og_vocab_get_vocabularies($nid);
}

/**
 * Service callback; Returns the group ID associated with a vocabulary ID.
 */
function og_vocab_service_get_group($vid){
  return og_vocab_get_group($vid);
}

/**
 * Access callback; Determine if user can access service.
 */
function og_vocab_service_get_access() {
  return user_access('access og_vocab service');
}