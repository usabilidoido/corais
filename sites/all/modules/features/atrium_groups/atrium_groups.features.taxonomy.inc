<?php

/**
 * Implementation of hook_taxonomy_default_vocabularies().
 */
function atrium_groups_taxonomy_default_vocabularies() {
  return array(
    'groups' => array(
      'name' => 'Project Group',
      'description' => 'Organize projects into groups',
      'help' => 'Group that this project belongs to (optional). That\'s useful if you plan to develop related projects.',
      'relations' => '1',
      'hierarchy' => '1',
      'multiple' => '0',
      'required' => '0',
      'tags' => '0',
      'module' => 'features_groups',
      'weight' => '0',
      'language' => '',
      'type' => NULL,
      'nodes' => array(),
    ),
  );
}
