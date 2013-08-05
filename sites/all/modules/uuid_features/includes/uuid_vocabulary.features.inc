<?php
/**
 * @file
 * Features hooks for the uuid_vocabulary features component.
 */

/**
 * Implementation of hook_features_export_options().
 */
function uuid_vocabulary_features_export_options() {
  $options = array();

  $query = 'SELECT v.vid, v.name, u.uuid
    FROM {vocabulary} v INNER JOIN {uuid_vocabulary} u 
    ON v.vid = u.vid ORDER BY v.name ASC';
  $result = db_query($query);
  while ($vocab = db_fetch_object($result)) {
    $options[$vocab->uuid] = $vocab->name;
  }

  return $options;
}

/**
 * Implementation of hook_features_export().
 */
function uuid_vocabulary_features_export($data, &$export, $module_name = '') {
  $pipe = array();

  $export['dependencies']['taxonomy'] = 'taxonomy';
  $export['dependencies']['uuid'] = 'uuid';
  $export['dependencies']['uuid_features'] = 'uuid_features';

  foreach ($data as $uuid) {
    $export['features']['uuid_vocabulary'][$uuid] = $uuid;
  }

  return $pipe;
}

/**
 * Implementation of hook_features_export_render().
 */
function uuid_vocabulary_features_export_render($module = 'foo', $data) {
  $translatables = $code = array();

  $code[] = '  $vocabularies = array();';
  $code[] = '';
  foreach ($data as $uuid) {
    $vid = uuid_get_serial_id('vocabulary', 'vid', $uuid);
    $vocab = taxonomy_vocabulary_load($vid);

    if (!$vocab) {
      continue;
    }

    $export = array('uuid' => $uuid);

    // Whitelist of vocabulary properties.
    $props = array('name', 'description', 'help', 'relations', 'heirarchy', 
      'multiple', 'required', 'tags', 'module', 'weight', 'nodes');
    foreach ($props as $key) {
      if (isset($vocab->$key)) {
        $export[$key] = $vocab->$key;
      }
    }

    $code[] = '  $vocabularies[] = '. features_var_export($export, '  ') .';';
  }

  if (!empty($translatables)) {
    $code[] = features_translatables_export($translatables, '  ');
  }

  $code[] = '  return $vocabularies;';
  $code = implode("\n", $code);
  return array('uuid_features_default_vocabularies' => $code);
}

/**
 * Implementation of hook_features_revert().
 */
function uuid_vocabulary_features_revert($module) {
  uuid_vocabulary_features_rebuild($module);
}

/**
 * Implementation of hook_features_rebuild().
 * Rebuilds nodes based on UUID from code defaults.
 */
function uuid_vocabulary_features_rebuild($module) {
  $vocabs = module_invoke($module, 'uuid_features_default_vocabularies');
  if (!empty($vocabs)) {
    foreach ($vocabs as $data) {
      $existing = db_result(db_query('SELECT vid FROM {uuid_vocabulary} WHERE uuid = "%s"', $data['uuid']));

      if (!empty($existing)) {
        $data['vid'] = $existing;
      }

      taxonomy_save_vocabulary($data);

      // TODO: remove when http://drupal.org/node/860442 is resolved.
      uuid_set_uuid('vocabulary', 'vid', $data['vid'], $data['uuid']);
    }
  }
}
