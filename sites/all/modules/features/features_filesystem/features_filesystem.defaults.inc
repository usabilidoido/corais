<?php

/**
 * Helper to implementation of hook_content_default_fields().
 */
function _features_filesystem_content_default_fields() {
  $fields = array();

  // Exported field: field_file_file
  $fields[] = array(
    'field_name' => 'field_file_file',
    'type_name' => 'file',
    'display_settings' => array(
      'weight' => '31',
      'parent' => '',
      'label' => array(
        'format' => 'above',
      ),
      'teaser' => array(
        'format' => 'default',
        'exclude' => 0,
      ),
      'full' => array(
        'format' => 'default',
        'exclude' => 0,
      ),
      '5' => array(
        'format' => 'default',
        'exclude' => 0,
      ),
      '4' => array(
        'format' => 'default',
        'exclude' => 0,
      ),
      '2' => array(
        'format' => 'default',
        'exclude' => 0,
      ),
      '3' => array(
        'format' => 'default',
        'exclude' => 0,
      ),
      'token' => array(
        'format' => 'default',
        'exclude' => 0,
      ),
    ),
    'widget_active' => '1',
    'type' => 'filefield',
    'required' => '1',
    'multiple' => '0',
    'module' => 'filefield',
    'active' => '1',
    'list_field' => '0',
    'list_default' => 1,
    'description_field' => '1',
    'widget' => array(
      'file_extensions' => '',
      'file_path' => '',
      'progress_indicator' => 'bar',
      'max_filesize_per_file' => '',
      'max_filesize_per_node' => '',
      'label' => 'File',
      'weight' => '31',
      'description' => '',
      'type' => 'filefield_widget',
      'module' => 'filefield',
    ),
  );

  // Exported field: field_parentid
  $fields[] = array(
    'field_name' => 'field_parentid',
    'type_name' => 'file',
    'display_settings' => array(
      'weight' => '39',
      'parent' => '',
      'label' => array(
        'format' => 'hidden',
      ),
      'teaser' => array(
        'format' => 'default',
        'exclude' => 1,
      ),
      'full' => array(
        'format' => 'hidden',
        'exclude' => 1,
      ),
      '5' => array(
        'format' => 'default',
        'exclude' => 0,
      ),
      '4' => array(
        'format' => 'default',
        'exclude' => 0,
      ),
      '2' => array(
        'format' => 'default',
        'exclude' => 0,
      ),
      '3' => array(
        'format' => 'default',
        'exclude' => 0,
      ),
      'token' => array(
        'format' => 'default',
        'exclude' => 0,
      ),
    ),
    'widget_active' => '1',
    'type' => 'number_integer',
    'required' => '0',
    'multiple' => '0',
    'module' => 'number',
    'active' => '1',
    'prefix' => '',
    'suffix' => '',
    'min' => '',
    'max' => '',
    'allowed_values' => '',
    'allowed_values_php' => '',
    'widget' => array(
      'default_value' => array(
        '0' => array(
          'value' => '',
          '_error_element' => 'default_value_widget][field_parentid][0][value',
        ),
      ),
      'default_value_php' => NULL,
      'label' => 'parentid',
      'weight' => '39',
      'description' => '',
      'type' => 'number',
      'module' => 'number',
    ),
  );

  // Translatables
  array(
    t('File'),
    t('parentid'),
  );

  return $fields;
}

/**
 * Helper to implementation of hook_context_default_contexts().
 */
function _features_filesystem_context_default_contexts() {
  $items = array();

  $items[] = array(
    'namespace' => 'spaces',
    'attribute' => 'feature',
    'value' => 'features_filesystem',
    'description' => 'This is the filesystem context',
    'node' => array(
      '0' => 'folder',
    ),
    'views' => array(
      '0' => 'oa_custom_folders',
    ),
    'menu' => 'group_folder',
    'block' => array(),
  );
  return $items;
}

/**
 * Helper to implementation of hook_menu_default_items().
 */
function _features_filesystem_menu_default_items() {
  $items = array();

  $items[] = array(
    'title' => 'File Explorer',
    'path' => 'group_folder',
    'weight' => '0',
  );
  // Translatables
  array(
    t('File Explorer'),
  );


  return $items;
}

/**
 * Helper to implementation of hook_rules_defaults().
 */
function _features_filesystem_rules_defaults() {
  return array(
    'rule_sets' => array(),
    'rules' => array(
      'features_filesystem_3' => array(
        '#type' => 'rule',
        '#set' => 'event_node_delete',
        '#label' => 'Del OG Vocab for File System',
        '#active' => 1,
        '#weight' => '0',
        '#categories' => array(
          '0' => 'features_filesystem',
          'features_filesystem' => 'features_filesystem',
        ),
        '#status' => 'default',
        '#conditions' => array(
          '0' => array(
            '#weight' => 0,
            '#info' => array(
              'label' => 'Created content is Group',
              'arguments' => array(
                'node' => array(
                  'type' => 'node',
                  'label' => 'Content',
                ),
              ),
              'module' => 'Node',
            ),
            '#name' => 'rules_condition_content_is_type',
            '#settings' => array(
              'type' => array(
                'group' => 'group',
              ),
              '#argument map' => array(
                'node' => 'node',
              ),
            ),
            '#type' => 'condition',
          ),
        ),
        '#actions' => array(
          '0' => array(
            '#weight' => 0,
            '#info' => array(
              'label' => 'Remove all vocabulary from group',
              'arguments' => array(
                'group' => array(
                  'type' => 'node',
                  'label' => 'Group',
                ),
              ),
              'module' => 'Organic groups vocabulary',
            ),
            '#name' => 'og_vocab_rules_action_remove_all_group',
            '#settings' => array(
              '#argument map' => array(
                'group' => 'node',
              ),
            ),
            '#type' => 'action',
          ),
        ),
        '#version' => 6003,
      ),
      'features_filesystem_1' => array(
        '#type' => 'rule',
        '#set' => 'event_node_insert',
        '#label' => 'Add OG Vocab for File System',
        '#active' => 1,
        '#weight' => '0',
        '#categories' => array(
          '0' => 'features_filesystem',
          'features_filesystem' => 'features_filesystem',
        ),
        '#status' => 'default',
        '#conditions' => array(
          '0' => array(
            '#type' => 'condition',
            '#settings' => array(
              'type' => array(
                'group' => 'group',
              ),
              '#argument map' => array(
                'node' => 'node',
              ),
            ),
            '#name' => 'rules_condition_content_is_type',
            '#info' => array(
              'label' => 'Created content is Group',
              'arguments' => array(
                'node' => array(
                  'type' => 'node',
                  'label' => 'Content',
                ),
              ),
              'module' => 'Node',
            ),
            '#weight' => 0,
          ),
        ),
        '#actions' => array(
          '0' => array(
            '#weight' => 0,
            '#info' => array(
              'label' => 'Add a new vocabulary',
              'new variables' => array(
                'taxonomy_vocab' => array(
                  'type' => 'taxonomy_vocab',
                  'label' => 'Taxonomy vocabulary',
                ),
              ),
              'eval input' => array(
                '0' => 'name',
                '1' => 'description',
                '2' => 'help',
              ),
              'module' => 'Taxonomy',
            ),
            '#name' => 'rules_action_taxonomy_add_vocab',
            '#settings' => array(
              'name' => 'file category ',
              'description' => '',
              'help' => '',
              'nodes' => array(
                'file' => 'file',
              ),
              'tags' => 1,
              'multiple' => 0,
              'required' => 0,
              'weight' => '0',
              'hierarchy' => '0',
              'relations' => '1',
            ),
            '#type' => 'action',
          ),
          '1' => array(
            '#type' => 'action',
            '#settings' => array(
              '#argument map' => array(
                'group' => 'node',
                'taxonomy_vocab' => 'taxonomy_vocab',
              ),
            ),
            '#name' => 'og_vocab_rules_action_assign_vocab',
            '#info' => array(
              'label' => 'Assign vocabulary to group',
              'arguments' => array(
                'group' => array(
                  'type' => 'node',
                  'label' => 'Group',
                ),
                'taxonomy_vocab' => array(
                  'type' => 'taxonomy_vocab',
                  'label' => 'Taxonomy vocabulary that will be assigned to group',
                ),
              ),
              'module' => 'Organic groups vocabulary',
            ),
            '#weight' => 0,
          ),
        ),
        '#version' => 6003,
      ),
      'features_filesystem_2' => array(
        '#type' => 'rule',
        '#set' => 'event_node_insert',
        '#label' => 'Redirect after adding New Folder / File',
        '#active' => 1,
        '#weight' => '0',
        '#categories' => array(
          '0' => 'features_filesystem',
          'features_filesystem' => 'features_filesystem',
        ),
        '#status' => 'default',
        '#conditions' => array(
          '0' => array(
            '#weight' => 0,
            '#info' => array(
              'label' => 'Created content is file or folder',
              'arguments' => array(
                'node' => array(
                  'type' => 'node',
                  'label' => 'Content',
                ),
              ),
              'module' => 'Node',
            ),
            '#name' => 'rules_condition_content_is_type',
            '#settings' => array(
              'type' => array(
                'file' => 'file',
                'folder' => 'folder',
              ),
              '#argument map' => array(
                'node' => 'node',
              ),
            ),
            '#type' => 'condition',
          ),
        ),
        '#actions' => array(
          '0' => array(
            '#type' => 'action',
            '#settings' => array(
              'path' => 'group_folder',
              'query' => '',
              'fragment' => '',
              'force' => 0,
              'immediate' => 0,
            ),
            '#name' => 'rules_action_drupal_goto',
            '#info' => array(
              'label' => 'Page redirect',
              'module' => 'System',
              'eval input' => array(
                '0' => 'path',
                '1' => 'query',
                '2' => 'fragment',
              ),
            ),
            '#weight' => 0,
          ),
        ),
        '#version' => 6003,
      ),
      'features_filesystem_8' => array(
        '#type' => 'rule',
        '#set' => 'event_node_update',
        '#label' => 'Redirect after Updating Folder / File',
        '#active' => 1,
        '#weight' => '0',
        '#categories' => array(
          '0' => 'features_filesystem',
          'features_filesystem' => 'features_filesystem',
        ),
        '#status' => 'default',
        '#conditions' => array(
          '0' => array(
            '#weight' => 0,
            '#info' => array(
              'label' => 'Created content is file or folder',
              'arguments' => array(
                'node' => array(
                  'type' => 'node',
                  'label' => 'Content',
                ),
              ),
              'module' => 'Node',
            ),
            '#name' => 'rules_condition_content_is_type',
            '#settings' => array(
              'type' => array(
                'file' => 'file',
                'folder' => 'folder',
              ),
              '#argument map' => array(
                'node' => 'node',
              ),
            ),
            '#type' => 'condition',
          ),
        ),
        '#actions' => array(
          '0' => array(
            '#type' => 'action',
            '#settings' => array(
              'path' => 'group_folder',
              'query' => '',
              'fragment' => '',
              'force' => 0,
              'immediate' => 0,
            ),
            '#name' => 'rules_action_drupal_goto',
            '#info' => array(
              'label' => 'Page redirect',
              'module' => 'System',
              'eval input' => array(
                '0' => 'path',
                '1' => 'query',
                '2' => 'fragment',
              ),
            ),
            '#weight' => 0,
          ),
        ),
        '#version' => 6003,
      ),
    ),
  );
}

/**
 * Helper to implementation of hook_user_default_permissions().
 */
function _features_filesystem_user_default_permissions() {
  $permissions = array();

  // Exported permission: Allow Reordering
  $permissions[] = array(
    'name' => 'Allow Reordering',
    'roles' => array(
      '0' => 'admin',
      '1' => 'authenticated user',
      '2' => 'manager',
    ),
  );

  // Exported permission: create file content
  $permissions[] = array(
    'name' => 'create file content',
    'roles' => array(
      '0' => 'admin',
      '1' => 'authenticated user',
      '2' => 'manager',
    ),
  );

  // Exported permission: create folder content
  $permissions[] = array(
    'name' => 'create folder content',
    'roles' => array(
      '0' => 'admin',
      '1' => 'authenticated user',
    ),
  );

  // Exported permission: delete own file content
  $permissions[] = array(
    'name' => 'delete own file content',
    'roles' => array(
      '0' => 'admin',
      '1' => 'authenticated user',
      '2' => 'manager',
    ),
  );

  // Exported permission: delete own folder content
  $permissions[] = array(
    'name' => 'delete own folder content',
    'roles' => array(
      '0' => 'admin',
      '1' => 'authenticated user',
      '2' => 'manager',
    ),
  );

  // Exported permission: edit own file content
  $permissions[] = array(
    'name' => 'edit own file content',
    'roles' => array(
      '0' => 'admin',
      '1' => 'authenticated user',
      '2' => 'manager',
    ),
  );

  // Exported permission: edit own folder content
  $permissions[] = array(
    'name' => 'edit own folder content',
    'roles' => array(
      '0' => 'admin',
      '1' => 'authenticated user',
      '2' => 'manager',
    ),
  );

  // Exported permission: view revisions
  $permissions[] = array(
    'name' => 'view revisions',
    'roles' => array(
      '0' => 'admin',
      '1' => 'authenticated user',
    ),
  );

  return $permissions;
}
