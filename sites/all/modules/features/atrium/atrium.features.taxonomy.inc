<?php

/**
 * Implementation of hook_taxonomy_default_vocabularies().
 */
function atrium_taxonomy_default_vocabularies() {
  return array(
    'keywords' => array(
      'name' => 'Keywords',
      'description' => '',
      'help' => 'Enter tags related to your post.',
      'relations' => '1',
      'hierarchy' => '0',
      'multiple' => '0',
      'required' => '0',
      'tags' => '1',
      'module' => 'features_keywords',
      'weight' => '0',
      'language' => '',
      'nodes' => array(
        'group' => 'group',
      ),
    ),
  );
}
