<?php

/**
 * Implementation of hook_spaces_presets().
 */
function atrium_moderated_groups_spaces_presets() {
  $export = array();

  $spaces_presets = new stdClass;
  $spaces_presets->disabled = FALSE; /* Edit this to true to make a default spaces_presets disabled initially */
  $spaces_presets->api_version = 3;
  $spaces_presets->name = 'og_moderated';
  $spaces_presets->title = 'Moderated group';
  $spaces_presets->description = ' A moderated group is accessible to all users on the site. Users must request to join this group.';
  $spaces_presets->space_type = 'og';
  $spaces_presets->value = array(
    'variable' => array(
      'spaces_og_selective' => '1',
      'spaces_og_register' => 0,
      'spaces_og_directory' => 1,
      'spaces_og_private' => 0,
    ),
  );

  // Translatables
  // Included for use with string extractors like potx.
  t(' A moderated group is accessible to all users on the site. Users must request to join this group.');
  t('Moderated group');
  $export['og_moderated'] = $spaces_presets;

  return $export;
}
