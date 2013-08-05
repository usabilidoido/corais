<?php

/**
 * @file
 * Administration functions for the uuid module.
 */

/**
 * Menu callback: options for UUID.
 */
function uuid_admin() {
  $form = array();

  $form['content'] = array(
    '#type' => 'fieldset',
    '#title' => t('Content settings'),
  );
  $types = node_get_types('names');
  $form['content']['uuid_automatic_for_nodes'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Content types with automatic UUID generation'),
    '#default_value' => variable_get('uuid_automatic_for_nodes', array()),
    '#options' => $types,
    '#description' => t('Content of these types will have UUIDs automatically generated.'),
    '#required' => FALSE,
  );

  $form['user'] = array(
    '#type' => 'fieldset',
    '#title' => t('User settings'),
  );
  $form['user']['uuid_automatic_for_users'] = array(
    '#type' => 'radios',
    '#title' => t('Automatic UUID generation for users'),
    '#default_value' => variable_get('uuid_automatic_for_users', FALSE),
    '#options' => array(
      TRUE => t('Enabled'),
      FALSE => t('Disabled'),
    ),
    '#description' => t('Should UUIDs be created automatically for users?'),
  );

  if (module_exists('taxonomy')) {
    $form['taxonomy'] = array(
      '#type' => 'fieldset',
      '#title' => t('Taxonomy settings'),
    );

    if ($options = array_map(create_function('$voc', 'return $voc->name;'), taxonomy_get_vocabularies())) {
      $form['taxonomy']['uuid_automatic_for_taxonomy'] = array(
        '#type' => 'checkboxes',
        '#title' => t('Automatic UUID generation for taxonomy'),
        '#default_value' => variable_get('uuid_automatic_for_taxonomy', array()),
        '#options' => $options,
        '#description' => t("UUIDs will be created for the vocabulary and its terms."),
        '#required' => FALSE,
      );
    }
    else {
      $form['taxonomy']['uuid_automatic_for_taxonomy'] = array(
        '#type' => 'item',
        '#value' => t('There is currently no vocabulary defined.'),
      );
    }
  }

  $form['comment'] = array(
    '#type' => 'fieldset',
    '#title' => t('Comment settings'),
  );
  $form['comment']['uuid_automatic_for_comments'] = array(
    '#type' => 'radios',
    '#title' => t('Automatic UUID generation for comments'),
    '#default_value' => variable_get('uuid_automatic_for_comments', FALSE),
    '#options' => array(
      TRUE => t('Enabled'),
      FALSE => t('Disabled'),
    ),
    '#description' => t('Should UUIDs be created automatically for comments?'),
  );

  $form['settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Synchronization'),
  );
  $form['settings']['sync'] = array(
    '#type' => 'submit',
    '#value' => t('Create missing UUIDs'),
    '#submit' => array('uuid_sync_submit'),
    '#weight' => 10,
  );

  return system_settings_form($form);
}

/**
 * Submit handler for sync
 */
function uuid_sync_submit($form, &$form_state) {
  return uuid_sync();
}

/**
 * Ensure all content and users have UUIDs, if they are supposed to.
 */
function uuid_sync($types = NULL) {
  // Users.
  if (variable_get('uuid_automatic_for_users', FALSE)) {
    $result = db_query("SELECT uid FROM {users} WHERE uid NOT IN (SELECT uid FROM {uuid_users})");
    while($item = db_fetch_object($result)) {
      db_query("INSERT INTO {uuid_users} (uid, uuid) VALUES(%d, '%s')", $item->uid, uuid_uuid());
    }
  }

  // Comments.
  if (variable_get('uuid_automatic_for_comments', FALSE)) {
    $result = db_query("SELECT cid FROM {comments} WHERE cid NOT IN (SELECT cid FROM {uuid_comments})");
    while($item = db_fetch_object($result)) {
      db_query("INSERT INTO {uuid_comments} (cid, uuid) VALUES(%d, '%s')", $item->cid, uuid_uuid());
    }
  }

  if (!$types) {
    $types = variable_get('uuid_automatic_for_nodes', array());
  }

  // Remove disabled node types.
  $types = array_filter($types);
  if (!empty($types)) {
    $placeholders = db_placeholders($types, 'varchar');

    // Nodes.
    $result = db_query("SELECT nid FROM {node} WHERE type IN (" . $placeholders . ") AND nid NOT IN (SELECT nid FROM {uuid_node})", $types);
    while ($item = db_fetch_object($result)) {
      db_query("INSERT INTO {uuid_node} (nid, uuid) VALUES(%d, '%s')", $item->nid, uuid_uuid());
    }

    // Revisions.
    $result = db_query("SELECT nr.vid AS vid, nr.nid AS nid FROM {node_revisions} AS nr INNER JOIN {node} n ON nr.nid = n.nid WHERE n.type IN (" . $placeholders . ") AND nr.vid NOT IN (SELECT vid FROM {uuid_node_revisions})", $types);
    while ($item = db_fetch_object($result)) {
      db_query("INSERT INTO {uuid_node_revisions} (vid, uuid, nid) VALUES(%d, '%s', %d)", $item->vid, uuid_uuid(), $item->nid);
    }
  }

  $vids = variable_get('uuid_automatic_for_taxonomy', array());

  // Remove disabled vocabularies.
  $vids = array_filter($vids);
  if ($vids) {
    $placeholders = db_placeholders($vids, 'int');

    // Vocabularies.
    $result = db_query("SELECT v.vid FROM {vocabulary} AS v WHERE v.vid IN (" . $placeholders . ") AND NOT EXISTS (SELECT vid FROM {uuid_vocabulary} WHERE vid = v.vid)", $vids);
    while ($item = db_fetch_object($result)) {
      db_query("INSERT INTO {uuid_vocabulary} (vid, uuid) VALUES(%d, '%s')", $item->vid, uuid_uuid());
    }

    // Terms.
    $result = db_query("SELECT td.tid FROM {term_data} AS td WHERE td.vid IN (" . $placeholders . ") AND NOT EXISTS (SELECT tid FROM {uuid_term_data} WHERE tid = td.tid)", $vids);
    while ($item = db_fetch_object($result)) {
      db_query("INSERT INTO {uuid_term_data} (tid, uuid) VALUES(%d, '%s')", $item->tid, uuid_uuid());
    }
  }

  drupal_set_message(t("UUID tables have been updated."));
}
