<?php

function sshkey_list_page($entity_type, $entity_id) {
  $destination = drupal_get_destination();

  // Add the local actions.
  if (sshkey_access('create', $entity_type, $entity_id)) {
    $local_actions['add'] = array(
      'title' => t('Add a public key'),
      'href' => "ssh-keys/{$entity_type}/{$entity_id}/add",
      'query' => $destination,
      'modal' => TRUE,
    );
    $build['actions'] = theme('links', $local_actions, array('class' => 'item-list action-links'));
  }

  $header = array(
    'name' => array('data' => t('Title'), 'field' => 'title', 'sort' => 'asc'),
    'fingerprint' => array('data' => t('Fingerprint'), 'field' => 'fingerprint'),
    'operations' => array('data' => t('Operations')),
  );

  $limit = 25;
  $query = pager_query("SELECT key_id FROM {sshkey} WHERE entity_type = '%s' AND entity_id = %d" . tablesort_sql($header), $limit, 0, NULL, $entity_type, $entity_id);
  $key_ids = sshkey_db_fetch_col($query);
  $keys = sshkey_load_multiple($key_ids);

  $rows = array();
  foreach ($key_ids as $key_id) {
    $key = $keys[$key_id];
    $key_base_uri = "ssh-keys/{$entity_type}/{$entity_id}/{$key_id}";

    $rows[$key_id]['title'] = check_plain($key->title);
    $rows[$key_id]['fingerprint'] = theme('sshkey_fingerprint', $key);

    $operations = array();
    if (sshkey_access('view', $entity_type, $entity_id, $key)) {
      $operations['view'] = array(
        'title' => t('View'),
        'href' => $key_base_uri . '/view',
      );
    }
    if (sshkey_access('edit', $entity_type, $entity_id, $key)) {
      $operations['edit'] = array(
        'title' => t('Edit'),
        'href' => $key_base_uri . '/edit',
        'query' => $destination,
        'modal' => TRUE,
      );
    }
    if (sshkey_access('delete', $entity_type, $entity_id, $key)) {
      $operations['delete'] = array(
        'title' => t('Delete'),
        'href' => $key_base_uri . '/delete',
        'query' => $destination,
        'modal' => TRUE,
      );
    }
    $rows[$key_id]['operations'] = array(
      'data' => theme('links', $operations, array('class' => 'links inline')),
    );
  }

  if (empty($rows)) {
    $empty_text[] = t('No SSH public keys available.');
    if (sshkey_access('create', $entity_type, $entity_id)) {
      $empty_text[] = l(t('Add a public key'), "ssh-keys/{$entity_type}/{$entity_id}/add", array('query' => $destination, 'modal' => TRUE));
    }
    $rows['add'] = array(array(
      'data' => implode(' ', $empty_text),
      'colspan' => count($header),
      'class' => 'empty add',
    ));
  }

  $build['keys_table'] = theme('table', $header, $rows);
  $build['keys_table_pager'] = theme('pager', NULL, $limit, 0);

  //$build['pre_form_title'] = '<div><hr /><h3>Add a public key</h3>';
  //$build['add_form'] = drupal_get_form('sshkey_edit_form', $entity_type, $entity_id);
  //$build['post_add_form'] = '</div>';

  return implode("\n", $build);
}

/**
 * Page callback for viewing an SSH key.
 */
function sshkey_view_key($entity_type, $entity_id, $key) {
  // We are returning plain text, so tell the browser.
  drupal_set_header('Content-Type: text/plainl; charset=utf-8');
  print $key->value;
  drupal_page_footer();
}

function sshkey_edit_form($form_state, $entity_type, $entity_id, $key = NULL) {
  $entity = sshkey_entity_object_load($entity_type, $entity_id);

  if (!isset($key)) {
    $key = new stdClass();
    $key->is_new = TRUE;
    $key->key_id = NULL;
    $key->fingerprint = NULL;
    $key->title = '';
    $key->value = '';
    $key->entity_type = $entity_type;
    $key->entity_id = $entity_id;
  }

  $form['#sshkey'] = $key;
  $form['#entity'] = $entity;

  $form['key_id'] = array(
    '#type' => 'value',
    '#value' => isset($key->key_id) ? $key->key_id : NULL,
  );
  $form['entity_type'] = array(
    '#type' => 'value',
    '#value' => $entity_type,
  );
  $form['entity_id'] = array(
    '#type' => 'value',
    '#value' => $entity_id,
  );
  $form['title'] = array(
    '#type' => 'textfield',
    '#title' => t('Title'),
    '#description' => t("If this field is left blank, the key's title will be automatically generated."),
    '#default_value' => $key->title,
    '#maxlength' => 128,
  );

  //$form['key_upload'] = array(
  //  '#type' => 'file',
  //  '#title' => t('Upload your private key file'),
  //);

  $form['value'] = array(
    '#type' => 'textarea',
    '#title' => t('Key'),
    '#default_value' => $key->value,
    '#rows' => 10,
    '#wysiwyg' => FALSE,
    '#required' => TRUE,
  );

  $form['actions'] = array(
    '#weight' => 100,
  );
  $form['actions']['save'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );
  $form['actions']['cancel'] = array(
    '#type' => 'markup',
    '#value' => l(t('Cancel'), isset($_GET['destination']) ? $_GET['destination'] : '<front>'),
  );

  return $form;
}

function sshkey_edit_form_validate($form, &$form_state) {
  // Import the uploaded key file.
  //if ($file = file_save_upload('key_upload', array(), FALSE, FILE_EXISTS_REPLACE)) {
  //  $contents = @file_get_contents($file->filepath);
  //  file_delete($file->filepath);
  //  if ($contents === FALSE) {
  //    drupal_set_message(t('The public key import failed, because the file %filename could not be read.', array('%filename' => $file->filename)), 'error');
  //  }
  //  else {
  //    $form_state['values']['value'] = $contents;
  //  }
  //}

  sshkey_validate($form_state['values'], $form, $form_state);
}

function sshkey_edit_form_submit($form, &$form_state) {
  $key = (object) $form_state['values'];
  sshkey_save($key);
  drupal_set_message(t('The SSH public key %title has been saved.', array('%title' => $key->title)));
}

function sshkey_delete_form($form_state, $key) {
  $form['#sshkey'] = $key;

  $form['key_id'] = array(
    '#type' => 'value',
    '#value' => $key->key_id,
  );

  return confirm_form(
    $form,
    t('Are you sure you want to delete the public key %title?', array('%title' => $key->title)),
    isset($_GET['destination']) ? $_GET['destination'] : '<front>',
    '',
    t('Delete'),
    t('Cancel')
  );
}

function sshkey_delete_form_submit($form, $form_state) {
  sshkey_delete($form_state['values']['key_id']);
  drupal_set_message(t('The SSH public key %title has been deleted.', array('%title' => $form['#sshkey']->title)));
}
