<?php

/**
 * Implementation of hook_taxonomy_default_vocabularies().
 */
function atrium_book_taxonomy_default_vocabularies() {
  return array(
    'inputs' => array(
      'name' => 'Inputs/Outputs',
      'description' => 'Requerimentos para a realização de um conhecimento.',
      'help' => 'Requerimentos para a realização de um conhecimento.',
      'relations' => '1',
      'hierarchy' => '0',
      'multiple' => '0',
      'required' => '0',
      'tags' => '1',
      'module' => 'features_inputs',
      'weight' => '0',
      'language' => '',
      'nodes' => array(
        'book' => 'book',
      ),
    ),
    'phase' => array(
      'name' => 'Fase de projeto',
      'description' => '',
      'help' => '',
      'relations' => '1',
      'hierarchy' => '0',
      'multiple' => '1',
      'required' => '1',
      'tags' => '0',
      'module' => 'features_phase',
      'weight' => '0',
      'language' => '',
      'nodes' => array(
        'book' => 'book',
      ),
    ),
  );
}
