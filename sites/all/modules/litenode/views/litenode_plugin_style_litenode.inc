<?php
// $Id$
/**
 * @file
 * Contains the litenode style plugin.
 */

/**
 * Style plugin to render each item in an ordered or unordered list.
 *
 * @ingroup views_style_plugins
 */
class litenode_plugin_style_litenode extends views_plugin_style {
  
  function uses_fields() {
    return TRUE;
  }

  /**
   * Set default options
   */
  function option_definition() {
    $options = parent::option_definition();
    $options['modules'] = array('default' => array('node' => 1, 'user' => 1));
    $options['teaser'] = array('default' => TRUE);
    return $options;
  }

  /**
   * Render the given style.
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $form['modules'] = array(
      '#type' => 'checkboxes',
      '#title' => t('Modules'),
      '#description' => t('Enable support for the following modules'),
    );

    foreach (module_invoke_all('litenode') as $module => $info) {
      if (empty($info['required'])) {
        $form['modules'][$module] = array(
          '#type' => 'checkbox',
          '#title' => $info['module'],
          '#default_value' => !empty($this->options['modules'][$module]) ? TRUE : FALSE,
        );
      }
      else {
        $form['modules'][$module] = array(
          '#type' => 'checkbox',
          '#title' => $info['module'],
          '#default_value' => TRUE,
          '#value' => TRUE,
          '#disabled' => TRUE,
        );
      }
    }

    $form['teaser'] = array(
      '#type' => 'radios',
      '#title' => t('View type'),
      '#options' => array(TRUE => t('Teaser'), FALSE => t('Full view')),
      '#default_value' => $this->options['teaser'],
    );
  }

  /**
   * Template preprocessor.
   */
  function preprocess(&$vars) {
    static $horizon; // User history horizon
    if (!isset($horizon)) {
      global $user;
      $horizon = $user->access - (14 * 24 * 3600);
    }

    $view = $vars['view'];
    $teaser = !empty($vars['options']['teaser']) ? TRUE: FALSE;
    $nodes = array();

    if (module_exists('content')) {
      $cck = array();
      // Grab an array of any CCK fields that should be rendered
      foreach ($view->display_handler->options['fields'] as $field => $info) {
        if (strpos($field, 'field_') === 0) {
          $cck[] = $field;
        }
      }
    }

    foreach ($view->result as $row) {
      $node = new stdClass();

      $map = litenode_views_fieldmap();

      // Assign view row values to node properties
      foreach ($map as $nprop => $vprop) {
        if (isset($row->{$vprop})) {
          $node->{$nprop} = $row->{$vprop};
        }
      }

      // Render CCK
      if (!empty($cck)) {
        foreach ($cck as $id) {
          $handler = $view->field[$id];

          $field = !empty($handler->content_field) ? $handler->content_field : NULL;
          if ($field) {
            $options = $handler->options;
            $db_info = content_database_info($field);

            $item = array();
            foreach ($db_info['columns'] as $column => $attributes) {
              $item[$column] = $row->{$handler->aliases[$attributes['column']]};
            }
            // @TODO: the line underneath forces this to be a single-display
            // CCK field. Research how to make multiple values work!
            $node->{$field['field_name']} = array($item);
          }
        }
      }

      // OG
      if (!empty($vars['options']['modules']['og'])) {
        // @TODO group type nodes
        // @TODO og_private
        // @TODO og_selective
        if (!og_is_omitted_type($node->type)) {
          // @TODO og_groups_both -- we could do this similar to the
          // other litenode loaders as a batch query.
          $node->og_groups = array();
          $field_alias = $view->field['group_nid']->aliases['group_nid'];
          if (!empty($row->{$field_alias})) {
            $node->og_groups[$row->{$field_alias}] = $row->{$field_alias};
          }
        }
      }

      // Allow implementing modules to execute extra logic if a simple
      // field mapping is insufficient.
      drupal_alter('litenode', $node, $row);

      $nodes[$node->nid] = $node;
    }

    // Taxonomy module integration
    if (!empty($vars['options']['modules']['taxonomy'])) {
      litenode_taxonomy_node_get_terms($nodes);
    }

    // Upload module integration
    if (!empty($vars['options']['modules']['upload'])) {
      litenode_upload_load($nodes);
    }

    $vars['nodes'] = array();
    foreach ($nodes as $nid => $node) {
      $vars['nodes'][$nid] = node_view($node, $teaser);
    }
  }
}

/**
 * LITENODE LOADERS ===================================================
 */

/**
 * Litenode version of upload_load(). Pulls all uploaded file
 * information for a given view in 1 query.
 */
function litenode_upload_load(&$nodes) {
  $files = array();

  $vids = array();
  foreach ($nodes as $nid => $node) {
    $vids[] = $node->vid;
  }

  $result = db_query('SELECT *, r.nid FROM {files} f INNER JOIN {upload} r ON f.fid = r.fid WHERE r.vid IN(%s) ORDER BY r.weight, f.fid', implode(',', $vids));
  while ($file = db_fetch_object($result)) {
    $nid = $file->nid;
    unset($file->nid);
    if (!isset($nodes[$nid]->files)) {
      $nodes[$nid]->files = array();
    }
    $nodes[$nid]->files[$file->fid] = $file;
  }
}

/**
 * Find all terms associated with the given node, ordered by vocabulary and term weight.
 */
function litenode_taxonomy_node_get_terms(&$nodes) {
  $vids = array();
  foreach ($nodes as $nid => $node) {
    $vids[] = $node->vid;
  }

  $result = db_query(db_rewrite_sql('SELECT t.*, r.nid FROM {term_node} r INNER JOIN {term_data} t ON r.tid = t.tid INNER JOIN {vocabulary} v ON t.vid = v.vid WHERE r.vid IN (%s) ORDER BY v.weight, t.weight, t.name', 't', 'tid'), implode(',', $vids));
  while ($term = db_fetch_object($result)) {
    $nid = $term->nid;
    unset($term->nid);
    if (!isset($nodes[$nid]->taxonomy)) {
      $nodes[$nid]->taxonomy = array();
    }
    $nodes[$nid]->taxonomy[$term->tid] = $term;
  }
}
