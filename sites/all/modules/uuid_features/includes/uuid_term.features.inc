<?php
/**
 * @file
 * Features hooks for the uuid_term features component.
 */

/**
 * Implementation of hook_features_export_options().
 */
function uuid_term_features_export_options() {
  $options = array();

  $query = 'SELECT t.tid, t.name, v.name AS vname, u.uuid
    FROM {term_data} t 
    INNER JOIN {uuid_term_data} u ON t.tid = u.tid
    INNER JOIN {vocabulary} v ON t.vid = v.vid
    ORDER BY v.name ASC, t.name ASC';
  $result = db_query($query);
  while ($term = db_fetch_object($result)) {
    $options[$term->uuid] = $term->vname .' - '. $term->name;
  }

  return $options;
}

/**
 * Implementation of hook_features_export().
 */
function uuid_term_features_export($data, &$export, $module_name = '') {
  $pipe = array();

  $export['dependencies']['taxonomy'] = 'taxonomy';
  $export['dependencies']['uuid'] = 'uuid';
  $export['dependencies']['uuid_features'] = 'uuid_features';

  foreach ($data as $uuid) {
    $export['features']['uuid_term'][$uuid] = $uuid;

    // Get the UUID of the term's vocabulary.
    $query = 'SELECT uv.uuid FROM {uuid_term_data} utd
      INNER JOIN {term_data} t ON utd.tid = t.tid
      INNER JOIN {uuid_vocabulary} uv ON t.vid = uv.vid
      WHERE utd.uuid = "%s"';
    $voc_uuid = db_result(db_query($query, $uuid));

    // Add the vocab UUID to the pipe, to be processed by uuid_vocabulary.
    $export['features']['uuid_vocabulary'][$voc_uuid] = $voc_uuid;
    
    // Get the UUID of the term's parents.
    $tid = uuid_get_serial_id('term_data', 'tid', $uuid);
    $parents = taxonomy_get_parents($tid);
    foreach ($parents as $parent) {
      $parent_uuid = uuid_get_uuid('term_data', 'tid', $parent->tid);
      $export['features']['uuid_term'][$parent_uuid] = $parent_uuid;
    }
  }

  return $pipe;
}

/**
 * Implementation of hook_features_export_render().
 */
function uuid_term_features_export_render($module = 'foo', $data) {
  $translatables = $code = array();

  $code[] = '  $terms = array();';
  $code[] = '';
  foreach ($data as $uuid) {
    $tid =  db_result(db_query('SELECT tid FROM {uuid_term_data} WHERE uuid = "%s"', $uuid));
    $term = taxonomy_get_term($tid);
    if (!$term) {
      continue;
    }

    $export = array('uuid' => $uuid);

    // Whitelist of term properties.
    $props = array('name', 'description', 'weight');
    foreach ($props as $key) {
      if (isset($term->$key)) {
        $export[$key] = $term->$key;
      }
    }
    
    // Get the UUID of the term's parent terms.
    $parents = taxonomy_get_parents($tid);
    foreach ($parents as $parent) {
    	$export['parent'][] = uuid_get_uuid('term_data', 'tid', $parent->tid);
    }

    // Get the UUID of the term's vocabulary.
    $query = 'SELECT uv.uuid FROM {uuid_vocabulary} uv WHERE uv.vid = "%s"';
    $voc_uuid = db_result(db_query($query, $term->vid));

    $export['uuid_vocabulary'] = $voc_uuid;

    $code[] = '  $terms[] = '. features_var_export($export, '  ') .';';
  }

  if (!empty($translatables)) {
    $code[] = features_translatables_export($translatables, '  ');
  }

  $code[] = '  return $terms;';
  $code = implode("\n", $code);
  return array('uuid_features_default_terms' => $code);
}

/**
 * Implementation of hook_features_revert().
 */
function uuid_term_features_revert($module) {
  uuid_term_features_rebuild($module);
}

/**
 * Implementation of hook_features_rebuild().
 * Rebuilds terms based on UUID from code defaults.
 */
function uuid_term_features_rebuild($module) {
  $terms = module_invoke($module, 'uuid_features_default_terms');
  if (!empty($terms)) {
    foreach ($terms as $data) {
      $existing = uuid_get_serial_id('term_data', 'tid', $data['uuid']);

      if (!empty($existing)) {
        $data['tid'] = $existing;
      }
      
      $vid = db_result(db_query('SELECT vid FROM {uuid_vocabulary} WHERE uuid = "%s"', $data['uuid_vocabulary']));
      if ($vid) {
        $data['vid'] = $vid;
        
        if (isset($data['parent']) && is_array($data['parent'])) {
	        foreach ($data['parent'] as $key => $parent_uuid) {
	          $data['parent'][$key] = uuid_get_serial_id('term_data', 'tid', $parent_uuid);
	        }
	      }

        $ret = taxonomy_save_term($data);

        // TODO: remove when http://drupal.org/node/860442 is resolved.
        uuid_set_uuid('term_data', 'tid', $data['tid'], $data['uuid']);
      }

    }
  }
}
