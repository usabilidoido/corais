<?php

/**
 * @file
 * Administrative pages for Certificate module.
 */

/**
 * Returns the form for the per-node certificate settings.
 *
 * This is shared by the settings page and the node edit page.
 *
 * @param $node
 *   The fully loaded node object if we've got it.
 *
 * @return
 *   The form array for the per-node certificate settings.
 *
 */
function certificate_type_mapping_form($form_state, $node = NULL) {
  // Get existing node-specific template settings.
  $node_template_settings = $node->certificate['node_settings'];

  // Get existing templates.
  $templates = certificate_certificate_load_all();
  $template_options[0] = 'Global';
  foreach ($templates as $key => $template) {
    $template_options[$key] = $template['title'];
  }

  if (variable_get('certificate_field_grouping', 0)) {
    // Group stuff
    $fieldgroups = variable_get('certificate_field_groups', array());

    // Generate form elements based on selected field's allowed values.
    if (count($fieldgroups)) {
      foreach ($fieldgroups as $key => $fieldgroup) {
        // We don't care about $fieldgroup - just the key.
        // Get node-based setting, if not set, use global.
        if ($node) {
          $template = $node_template_settings[$key];
          $template = $template ? $template : 0;
        }
        else {
          $template = variable_get("certificate_type_{$key}_template", 0);
        }

        $form[$key] = array(
          '#type' => 'select',
          '#title' => check_plain($key),
          '#options' => $template_options,
          '#default_value' => $template,
        );
      }
    }
    else {
      $form['nogroups'] = array(
        '#prefix' => '<p>',
        '#suffix' => '</p>',
        '#value' => t("Please go to !groups to configure certificate field mapping groups.", array('!groups' => l('groups', 'admin/build/certificates/mapping/groups'))),
      );
    }
  }
  else {
    // Get field's options from types selection.
    $field_options_array = certificate_get_selected_type_options();
    if (arg(0) == 'node' && !count($field_options_array)) {
      $form['#type'] = 'markup';
      $form['#value'] = "<div>" . t("Please !link before using certificates.", array('!link' => l('set up field mappings', 'admin/build/certificates/mapping'))) . "</div>";
    }

    // Generate form elements based on selected field's allowed values.
    if ($field_options_array) {
      foreach ($field_options_array as $key => $option_name) {
        // Get node-based setting, if not set, use global.
        if ($node) {
          $template = $node_template_settings[$key];
          $template = $template ? $template : 0;
        }
        else {
          $template = variable_get("certificate_type_{$key}_template", 0);
        }

        $form[$key] = array(
          '#type' => 'select',
          '#title' => check_plain($option_name),
          '#options' => $template_options,
          '#default_value' => $template,
        );
      }
    }
  }

  return $form;
}

/**
 * Alters the node form to inject the appropriate per-node template settings.
 */
function certificate_alter_node_form(&$form, &$form_state) {
  global $user;

  // Add template-mapping form.
  // Note, make sure to set #tree to TRUE.
  if (user_access('administer certificates')) {
    $form['certificate'] = array(
      '#type' => 'fieldset',
      '#title' => t('Certificate template settings'),
      '#weight' => 100,
      '#tree' => TRUE,
      '#group' => TRUE,
    );

    certificate_mapping_form($form, $form_state);
  }
}

/**
 * Add mapping elements to a form.
 */
function certificate_mapping_form(&$form, &$form_state) {
  if (isset($form['nid']['#value'])) {
    $nid = $form['nid']['#value'];
  }
  else {
    $nid = 0;
  }

  $stored = array();
  $sql = "select * from {certificate_node} cn where nid = %d";
  $result = db_query($sql, $nid);
  while ($row = db_fetch_object($result)) {
    $stored[$row->mapper][$row->type] = $row->template;
  }

  // Custom (module-provided) mappings.
  $mappings = module_invoke_all('certificate_map_options');

  $options = array('' => '- no certificate -');
  foreach (certificate_certificate_load_all() as $cert) {
    $options[$cert['nid']] = $cert['title'];
  }

  foreach ($mappings as $map_type => $map) {
    $form['certificate']['map'][$map_type] = array(
      '#title' => check_plain($map['title']),
      '#type' => 'fieldset',
      '#group' => TRUE,
      '#description' => check_plain($map['description']),
      '#collapsed' => TRUE,
      '#collapsible' => TRUE,
    );

    if (count($map['options'])) {
      foreach ($map['options'] as $key => $title) {
        $form['certificate']['map'][$map_type][$key] = array(
          '#type' => 'select',
          '#title' => check_plain($title),
          '#options' => $options,
          '#default_value' => isset($stored[$map_type][$key]) ? $stored[$map_type][$key] : NULL,
        );
      }
    }
    else {
      $form['certificate']['map'][$map_type]['empty'] = array(
        '#value' => '<p>' . t('There are no mappings available for %title.', array('%title' => $map['title'])) . '</p>',
      );
    }
  }
}

/**
 * Form for global mappings.
 */
function certificate_settings_form(&$form_state) {
  $form['certificate']['#tree'] = TRUE;

  module_load_include('inc', 'certificate', 'certificate.admin');
  certificate_mapping_form($form, $form_state);

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Update'),
  );

  return $form;
}

/**
 * Save the certificate mappings. For global mappings, we use node ID zero to
 * store it in the database.
 */
function certificate_settings_form_submit($form, &$form_state) {
  certificate_update_node_mappings(0, $form_state['values']['certificate']['map']);

  drupal_set_message(t('The certificate mapping options have been saved.'));
}

/**
 * Theme overview form.
 *
 * Arranges certificates in a table.
 *
 * @ingroup themeable
 * @ingroup forms
 * @see certificate_overview_form()
 */
function theme_certificate_overview_form($form) {
  $rows = array();
  foreach (element_children($form['certificates']) as $key) {
    $row = array();

    // Render the hidden 'certificate id' field and the title of the certificate into the
    // same column of the row.
    $row[] = drupal_render($form['certificates'][$key]['cid']) . drupal_render($form['certificates'][$key]['title']);

    // Render the edit and delete links into their own column.
    $row[] = drupal_render($form['certificates'][$key]['operations']);

    // Add the new row to our collection of rows.
    $rows[] = array(
      'data' => $row,
    );
  }

  // If there were no certificates found, let users know.
  if (count($rows) == 0) {
    $rows[] = array(array('data' => t('No certificate templates have been added.'), 'colspan' => 3));
  }

  // Render a list of header titles, and our array of rows, into a table. Even
  // we've already rendered all of our certificates, we always call drupal_render()
  // on the form itself after we're done, so hidden security fields and other
  // elements (like buttons) will appear properly at the bottom of the form.
  $header = array(t('Title'), t('Operations'));
  // Add an optional caption that appears above the table.
  $caption = t('');
  $output = theme('table', $header, $rows, array('id' => 'certificates-overview'), $caption);
  $output .= drupal_render($form);

  return $output;
}

/**
 * Theme the form as a table.
 */
function theme_certificate_history_form($form) {
  $rows = array();
  $header = array(t('User'), t('Course'), t('Operations'));
  $rows = array(
    // Simple row
    array('Fred Flintstone', 'Course 101', 'View | Edit | Email'),
    array('Fred Flintstone', 'Course 102', 'View | Edit | Email'),
    array('Joan Smith', 'Course 102', 'View | Edit | Email'),
  );

  $caption = t('');
  $output = theme('table', $header, $rows, array('id' => 'certificates-history'), $caption);
  $output .= drupal_render($form);

  return $output;
}

/**
 * Form to select which content profile field values are part of which group.
 */
function certificate_field_grouping_form() {
  $form = array();

  // Map against a CCK content_profile field.
  if (module_exists('content_profile')) {

    $groupsinfo = variable_get('certificate_field_groups', array());
    $fieldgroups = variable_get('certificate_field_grouping', array());
    $form['certificate_add_new_group'] = array(
      '#prefix' => '<div>',
      '#suffix' => '</div>',
      '#type' => 'markup',
      '#value' => l('Add a new group?', 'admin/build/certificates/mapping/groups/add'),
    );


    $types = content_types();
    foreach ($types as $key => $type) {
      if (variable_get('content_profile_use_' . $key, 0)) {
        $profile_types[] = $key;
      }
    }
    $info = _content_type_info();

    $form['map']['#tree'] = TRUE;
    // Create multiselect box for each field key.
    foreach ($groupsinfo as $key => $title) {
      $form['map'][$key] = array(
        '#type' => 'fieldset',
        '#title' => check_plain($title),
        '#tree' => TRUE,
        '#collapsible' => TRUE,
        '#collapsed' => TRUE,
      );
      foreach ($info['content types'] as $ckey => $type) {
        if (in_array($ckey, $profile_types)) {
          foreach ($type['fields'] as $field_key => $field) {
            $options = optionwidgets_options($field, FALSE);
            if (!empty($options)) {
              $form['map'][$key][$field_key] = array(
                '#title' => check_plain($field['widget']['label']),
                '#type' => 'select',
                '#multiple' => TRUE,
                '#options' => $options,
                // This is a trick to set the default value to ' ', which doesn't
                // match anything. This was accidentally causing 'None' to
                // actually be selected as a mapping.
                '#default_value' => isset($fieldgroups[$key][$field_key]) ? $fieldgroups[$key][$field_key] : array(),
              );
            }
          }
        }
      }
    }
  }
  else {
    $form['disabled']['#value'] = '<p>' . t('Please enable content_profile to work with profile field grouping.') . '</p>';
  }

  $form['submit'] = array(
    '#value' => 'Submit',
    '#type' => 'submit',
  );

  return $form;
}

/**
 * Save content profile field grouping.
 */
function certificate_field_grouping_form_submit(&$form, &$form_state) {
  variable_set('certificate_field_grouping', $form_state['values']['map']);

  drupal_set_message('Grouping settings updated.');
}

function certificate_field_grouping_add_form() {
  $form = array();

  $form['fieldgroup_name'] = array(
    '#title' => 'Machine name of field group',
    '#type' => 'textfield',
    '#required' => TRUE,
  );

  $form['fieldgroup_title'] = array(
    '#title' => 'Title of field group',
    '#type' => 'textfield',
    '#required' => TRUE,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Create',
  );

  return $form;
}

/**
 * Validate the fieldgroup machine name.
 */
function certificate_field_grouping_add_form_validate(&$form, &$form_state) {
  $fieldgroup_name = $form_state['values']['fieldgroup_name'];
  $fieldgroups = variable_get('certificate_field_groups', array());

  if (isset($fieldgroups[$fieldgroup_name])) {
    form_set_error('fieldgroup_name', 'Duplicate, please choose another name.');
  }
}

/**
 * Form to add another field group.
 */
function certificate_field_grouping_add_form_submit(&$form, &$form_state) {
  $fieldgroup_name = $form_state['values']['fieldgroup_name'];
  $fieldgroup_title = $form_state['values']['fieldgroup_title'];

  $fieldgroups = variable_get('certificate_field_groups', array());
  $fieldgroups[$fieldgroup_name] = $fieldgroup_title;
  variable_set('certificate_field_groups', $fieldgroups);
  drupal_set_message(t('Fieldgroup %f created.', array('%f' => $fieldgroup_name)));
  drupal_goto('admin/build/certificates/mapping/groups');
}

/**
 * Fieldgroup deletion.
 */
function certificate_field_grouping_delete_form_submit(&$form, &$form_state) {
  $fieldgroup_name = $form_state['values']['fieldgroup_name'];
  $fieldgroup_title = $form_state['values']['fieldgroup_title'];
  $fieldgroups = variable_get('certificate_field_groups', array());
  unset($fieldgroups[$fieldgroup_name]);
  variable_set('certificate_field_groups', $fieldgroups);

  drupal_set_message(t('Fieldgroup %f deleted.', array('%f' => $fieldgroup_name)));
  drupal_goto('admin/build/certificates/mapping/groups');
}

/**
 * Fieldgroup deletion form.
 */
function certificate_field_grouping_delete_form($form, $key) {
  $form = array();

  $form['fieldgroup_name'] = array(
    '#type' => 'hidden',
    '#value' => $key,
    '#access' => FALSE,
  );

  return confirm_form($form, "Delete field group $key?", 'admin/build/certificates/mapping/groups', 'This action cannot be undone. Access to matched certificates will be lost.', 'Delete', 'Cancel');
}

/**
 * List of certificates with links to add and edit.
 */
function certificate_templates_list() {
  $certificates = array();
  $l = l('Create new certificate', 'node/add/certificate', array('query' => drupal_get_destination()));
  $sql = "select * from {node} where type='certificate'";
  $result = db_query($sql);
  while ($row = db_fetch_array($result)) {
    $destination = drupal_get_destination();
    $certificates[] = array(
      'title' => $row['title'],
      'edit' => l(t('edit'), "node/{$row['nid']}/edit?{$destination}", array('attributes' => array('class' => 'edit-link'))),
      'delete' => l(t('delete'), "node/{$row['nid']}/delete?{$destination}", array('attributes' => array('class' => 'delete-link'))),
      'preview' => l(t('PDF'), "admin/build/certificates/templates/preview/{$row['nid']}", array('attributes' => array('class' => 'preview-link'))),
    );
  }

  return $l . theme_table(array(t('Title'), t('Edit'), t('Delete'), t('Preview')), $certificates);
}

/**
 * General settings form.
 *
 * Currently only has the option to enable snapshots.
 */
function certificate_admin_settings_form() {
  $form = array();

  $form['certificate_snapshots'] = array(
    '#title' => 'Use certificate snapshots?',
    '#description' => 'Certificates will only be generated once per node/user.',
    '#type' => 'checkbox',
    '#default_value' => variable_get('certificate_snapshots', 0),
  );

  return system_settings_form($form);
}

/**
 * Form to clear certificate snapshots.
 */
function certificate_admin_clear_form() {
  $form = array();

  $header = array(
    array(),
    array('data' => 'Title', 'field' => 'n.title'),
    array('data' => 'Count', 'field' => 'count'),
  );

  $sql = "select *, count(cs.uid) as count from {certificate_snapshots} cs
  left join {node} n on (cs.nid = n.nid)
  group by cs.nid";

  $sql .= tablesort_sql($header);

  $result = db_query($sql);
  $nids = array();
  while ($row = db_fetch_object($result)) {
    $nids[$row->nid] = '';
    $form['cs']['title'][$row->nid]['#value'] = $row->title;
    $form['cs']['count'][$row->nid]['#value'] = $row->count;
  }

  $form['cs']['nids'] = array(
    '#type' => 'checkboxes',
    '#options' => $nids,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Clear',
  );

  return $form;
}

/**
 * Theme certificate_admin_clear_form.
 */
function theme_certificate_admin_clear_form($form) {
  $header = array(
    theme('table_select_header_cell'),
    array('data' => 'Title', 'field' => 'n.title'),
    array('data' => 'Count', 'field' => 'cs_count')
  );

  if (!empty($form['cs']['title'])) {
    foreach (element_children($form['cs']['title']) as $key) {
      $rows[] = array(
        drupal_render($form['cs']['nids'][$key]),
        drupal_render($form['cs']['title'][$key]),
        drupal_render($form['cs']['count'][$key]),
      );
    }
  }
  else {
    return "No snapshots to clear.";
  }

  return theme('table', $header, $rows) . drupal_render($form);
}

/**
 * Delete selected certificate snapshots.
 */
function certificate_admin_clear_form_submit(&$form, &$form_state) {
  $nids = array();

  foreach ($form_state['values']['nids'] as $nid => $delete) {
    if ($delete) {
      $nids[] = $nid;
    }
  }

  if (count($nids)) {
    $placeholders = db_placeholders($nids);
    db_query("delete from {certificate_snapshots} where nid in ($placeholders)", $nids);

    drupal_set_message('Cleared certificate snapshots.');
  }
}
