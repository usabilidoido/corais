<?php
// $Id: comment_driven_inspect.admin.inc,v 1.2 2010/04/01 01:09:27 arhak Exp $

function comment_driven_inspect_form($form_state) {
  $form_values = $form_state['values'];
  $ids = $form_values['ids'];
  if (empty($ids) && isset($_GET['ids'])) {
    // if no ids were provided attempt to load a default provided via URL
    // the only awkward effect would be having a query string param ids=X
    // while provided input makes ids=Y and it prevails,
    // but copy/pasting the URL will bring ids=X instead of ids=Y
    // the cost worth it, since this URL feature will be used
    // to provide inspect links inplace
    // whenever a diff_summary is rendered (and the user has permission)
    $ids = $_GET['ids'];
  }
  
  // IMPORTANT: special support for comment_driven & comment_cck
  // ensure comment_driven in the first position
  $mapping['comment_driven'] = array(
    // shema mapping for cid,old_vid,new_vid,changes,diff_render
    'query' => 'SELECT * FROM {comment_driven_log}',
    'diff_render' => TRUE, // i.e. supported phase: frozen (aka snapshot) 
    'changes' => TRUE, // i.e. supported phase: live render
  );
  $mapping['comment_cck'] = array(
    // shema mapping for cid,old_vid,new_vid
    'query' => 'SELECT cid, previous_vid AS old_vid, vid AS new_vid FROM {comment_cck_revisions}',
    // no diff_render, i.e. unsupported phase: frozen (aka snapshot)
    // no changes, i.e. unsupported phase: live render
  );
  // @TODO: temporal dirty flag (until we have advanced_help)
  $at_least_one_mapping_installed = FALSE;
  foreach (array_keys($mapping) as $module) {
    $mapping[$module]['installed'] = driven_inspect_module_installed($module);
    if ($mapping[$module]['installed']) {
      $at_least_one_mapping_installed = TRUE;
    }
  }
  
  $schema = empty($form_values['schema']) ? 'comment_driven' : $form_values['schema'];
  // if schema != 'comment_driven' phases will be ignored
  // and live_comparison will be used
  $phases = isset($form_values['phases']) ? $form_values['phases'] : array(
    'live_comparison' => TRUE,
    'live_render' => TRUE,
    'frozen' => TRUE,
    'diff_values' => TRUE,
  );
  // if schema != 'comment_driven' tokens will be ignored
  $tokens = isset($form_values['tokens']) ? $form_values['tokens'] : array(
    'ulist' => TRUE, // lets enable one by default
  );
  
  // in this case this approach was preferred over hook_help
  // nevertheless, advanced_help would be better
  $form['intro'] = array(
    '#type' => 'fieldset',
    '#title' => t('Help'),
    '#collapsible' => TRUE,
    '#collapsed' => !empty($ids),
    '#description' => t('When <b>inspecting arbitrary couples of node revisions</b> (vid pairs) the only constraint is that both revisions in a couple must belong to nodes of the same content type.')
      . '<br/>' . t('Therefore, it is considered valid to compare a revision to itself, to a previous or subsequent revision, or even to a revision of a different node.')
      . '<br/>' . t('When <b>inspecting comments</b> (by their cids) the two associated revisions (before/after) will be compared.')
      . '<br/>' . t('Such comments should be found in the selected "revision changes history" table.') 
      . '<br/>' . t('Any schema/mapping can be supported, as long as the three required columns be provided (with arbitrary names) relating the comment id to a couple of node revision ids.'),
  );
  
  $form['prop_providers'] = array(
    '#type' => 'fieldset',
    '#title' => t('Property providers'),
    '#description' => t('Operations will be performed within the scope of enabled property provider modules'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['prop_providers']['list'] = array(
    '#type' => 'item',
    '#prefix' => '<ul>',
    '#suffix' => '</ul>',
  );
  $at_least_one_property_provided = FALSE;
  $property_providers = array(
    'driven_node', 
    'driven_taxo', 
    'driven_cck', 
    'driven_path', 
    'driven_workflow',
  );
  foreach ($property_providers as $module) {
    $found = module_exists($module);
    $form['prop_providers']['list'][$module] = array(
      '#type' => 'markup',
      '#value' => $module,
      '#prefix' => '<li>',
      '#suffix' => '</li>',
    );
    if ($found) {
      $at_least_one_property_provided = TRUE;
    }
    else {
      $form['prop_providers']['list'][$module]['#value'] .= ' (' . t('disabled') . ')';
    }
  }
  if (!$at_least_one_property_provided) {
    $form['prop_providers']['#collapsed'] = FALSE;
    $form['prop_providers']['useless'] = array(
      '#type' => 'item',
      '#value' => t('There are no driven properties exposed. You have to enable some property provider modules.'),
      '#prefix' => '<div class="warning">',
      '#suffix' => '</div>',
    );
  }
  
  $form['ids'] = array(
    '#type' => 'textfield',
    '#title' => t('IDs'),
    '#description' => t('Comma (or space) separated vid pairs (e.g. 123-124, 124-125) or cids (e.g. 123, 456).'),
    '#default_value' => $ids,
  );
  $form['inspect'] = array(
    '#type' => 'fieldset',
    '#title' => t('What to inspect'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,//!(empty($ids) && $at_least_one_mapping_installed),
    '#description' => t('Even when following modules might not be installed at all, these options are (temporary) provided as documentation.'),
  );
  foreach (array_keys($mapping) as $module) {
    $options[$module] = $module;
    if (!$mapping[$module]['installed']) {
      $options[$module] .= ' (' . t('not installed') . ')';
    }
  }
  $form['inspect']['schema'] = array(
    '#type' => 'radios',
    '#title' => t('Revision changes history'),
    '#description' => t('Only applicable to cids, since vid pairs come from {node_revision} table'),
    '#default_value' => $schema,
    '#options' => $options,
  );
  $form['inspect']['phases'] = array(
    '#type' => 'fieldset',
    '#title' => t('Phases'),
    '#description' => t('Phases are only appicable for comment driven changes history, otherwise live comparison will be used regardless of selected phases bellow.'),
    '#collapsible' => TRUE,
    '#collapsed' => ($schema != 'comment_driven' || !$mapping['comment_driven']['installed']),
    '#tree' => TRUE,
  );
  
  // @TODO: claiming for advanced_help
  $pro_cons = array(
    '#type' => 'fieldset',
    '#title' => t('Pro/Cons'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['inspect']['phases']['diff_values'] = array(
    '#type' => 'checkbox',
    '#title' => t('diff_values (dump)'),
    '#description' => t('Fetch diff_values from DB and display them using var_export.'),
    '#default_value' => $phases['diff_values'],
  );
  $form['inspect']['phases']['frozen'] = array(
    '#type' => 'checkbox',
    '#title' => t('Frozen (aka snapshot)'),
    '#description' => t('Fetch diff_render from DB (bypass diff_values and live render).'),
    '#default_value' => $phases['frozen'],
  );
  $form['inspect']['phases']['frozen_pro_cons'] = $pro_cons + array(
    '#description' => t('<b>Advantage</b>: faithful to the changes introduced at comment creation time.')
      . '<br/>' . t('<b>Disadvantage</b>: theme registry changes will be ignored, since a saved render (snapshot) will be used.'),
  );
  $form['inspect']['phases']['live_render'] = array(
    '#type' => 'checkbox',
    '#title' => t('Live render'),
    '#description' => t('Fetch diff_values from DB and display them using live render.')
      . '<br/>' . t('If an inaccurate (or empty) diff summary is rendered, then is should be due to gone properties/values.'),
    '#default_value' => $phases['live_render'],
  );
  $form['inspect']['phases']['live_render_pro_cons'] = $pro_cons + array(
    '#description' => t('<b>Advantage</b>: theme registry changes will be properly reflected.')
      . '<br/>' . t('<b>Disadvantage</b>: it might render innacurate diff summaries due to changes made to property definitions (e.g. CCK fields, taxonomy vocabularies) or deleted values (e.g. taxonomy terms, nodes referenced by CCK fields, etc).')
      . '<br/>' . t('<b>Attenuation</b>: comments might be flagged to be frozen, i.e. individually refrain from using live render.'),
  );
  $form['inspect']['phases']['live_comparison'] = array(
    '#type' => 'checkbox',
    '#title' => t('Live comparison'),
    '#description' => t('Build diff_values from node revisions (aka import/refresh), which implies using live render.')
      . '<br/>' . t('Note that this phase is not required for comment driven to properly work and therefore might have different bugs (i.e. out of synch).'),
    '#default_value' => $phases['live_comparison'],
  );
  $form['inspect']['phases']['live_comparison_pro_cons'] = $pro_cons + array(
    '#description' => t('<b>Advantage</b>: it can perform comparisons between two arbitrary revisions of the same content type (even if they don\'t belong to the same node).')
      . '<br/>' . t('<b>Disadvantage</b>: it can NOT detect changes on properties not handled by revisions (e.g. published, promoted).')
      . '<br/>' . t('<b>Attenuation</b>: title, body, taxonomy and CCK fields do support revisions, therefore, there isn\'t much out of its scope.')
      . '<br/>' . t('<b>Disadvantage</b>: requires revisions enabled, otherwise comparing a revision to itself returns no changes.')
      . '<br/>' . t('<b>Attenuation</b>: working with revisions (driven enforced at least) is highly encouraged, since it facilitates further migration paths (even migrating to other modules, e.g. back to comment_cck).'),
  );
  $form['inspect']['tokens'] = array(
    '#type' => 'fieldset',
    '#title' => t('Tokens'),
    '#description' => t('Tokens are only appicable for comment driven changes history (they fetch diff_values or diff_render from DB according to: per log frozen flag or per content type live render setting).'),
    '#collapsible' => TRUE,
    '#collapsed' => $form['inspect']['phases']['#collapsed'],
    '#tree' => TRUE,
  );
  $form['inspect']['tokens']['plain_text'] = array(
    '#type' => 'checkbox',
    '#title' => t('Plain text'),
    '#default_value' => $tokens['plain_text'],
  );
  $form['inspect']['tokens']['ulist'] = array(
    '#type' => 'checkbox',
    '#title' => t('HTML unordered list'),
    '#default_value' => $tokens['ulist'],
  );
  $form['inspect']['tokens']['table'] = array(
    '#type' => 'checkbox',
    '#title' => t('HTML table'),
    '#default_value' => $tokens['table'],
  );
  
  // the menu for this form is manually provided by this module
  // due to the specific behavior of this form
  // therefore, lets include the driven API signature
  $form += driven_form_signature();
  
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Inspect'),
  );
  
  if (empty($ids)) {
    return $form; // nothing to do yet
  }
  
  $cids = array();
  $vid_pairs = array();
  $old_vids = array();
  $new_vids = array();
  if (preg_match('/^(?:[,\s]*(\d++)[,\s]*)+$/', $ids)) {
    preg_match_all('/[,\s]*(\d++)/', $ids, $matches);
    $cids = $matches[1];
    $processed = 'cids: ' . implode(', ', $cids);
  }
  elseif (preg_match('/^(?:[,\s]*(\d++)-(\d++)[,\s]*)+$/', $ids)) {
    preg_match_all('/[,\s]*(\d++)-(\d++)/', $ids, $matches);
    $old_vids = $matches[1];
    $new_vids = $matches[2];
    $processed = 'vid pairs: ';
    foreach ($old_vids as $index => $value) {
      $vid_pairs[] = $old_vids[$index] . '-' . $new_vids[$index];
    }
    $processed .= implode(', ', $vid_pairs);
  }
  else {
    $processed = t('Could not parse cids nor vid pairs.');
  }
  $form['processed'] = array(
    '#type' => 'item',
    '#value' => $processed,
  );
  
  if (empty($cids) && empty($vid_pairs)) {
    return $form; // nothing else to do
  }
  
  // drop unselected phases (values are boolean)
  $phases = array_filter($phases);
  // drop unselected tokens and pick up remaining keys (values are boolean)
  $tokens = array_keys(array_filter($tokens));
  
  $processed = &$form['processed']['#value'];
  if ($cids) {
    // $schema is the module name
    $module = $schema;
    if (!driven_inspect_module_installed($module)) {
      $processed .= '<br/>' . t('Module %module is not installed, therefore its schema is not available.', array('%module' => $module));
      return $form; // nothing else to do
    }
    $query = $mapping[$schema]['query']
      . ' WHERE cid IN (' . db_placeholders($cids, 'int') . ')';
    $result = db_query($query, $cids);
    // vid pairs are empty, since we picked up cids
    while($row = db_fetch_array($result)) {
      $cid = $row['cid'];
      $old_vids[$cid] = $row['old_vid'];
      $new_vids[$cid] = $row['new_vid'];
      // remove every fetched triplet
      $cids = array_diff($cids, array($cid));
    }
    
    if (!empty($cids)) {
      // there were some cids we couldn't find
      $processed .= '<br/>' . t('Could not fetch following cids:') . ' ' . implode(', ', $cids);
    }
    // restore just the found cids
    $cids = array_keys($old_vids);
    foreach ($cids as $cid) {
      $vid_pairs[$cid] = '{cid: ' . $cid . ', vid pair: ' . $old_vids[$cid] . '-' . $new_vids[$cid] . '}';
    }
    $processed .= '<br/>' . t('Fetched triplets:') . ' ' . implode(', ', $vid_pairs);
    
    foreach ($cids as $cid) {
      // we only need to know the type of the associated node
      // to find out available driven properties
      $comment = _comment_load($cid);
      // it must exists, since we already filtered fetched triplets
      $log = comment_driven_load_log($cid);
      $node_type = node_get_types('type', node_load($comment->nid))->type;
      
      $output[$cid] = array(
        '#type' => 'fieldset',
        '#title' => $vid_pairs[$cid],
        '#collapsible' => TRUE,
        '#description' => t('revisions:') . l($old_vids[$cid], "node/$comment->nid/revisions/$old_vids[$cid]/view") . '-' . l($new_vids[$cid], "node/$comment->nid/revisions/$new_vids[$cid]/view"),
      );

      if ($schema == 'comment_driven') {
        $output[$cid]['#description'] .= '<br/>' . t('Frozen flag is: <b>%state</b>', array(
            '%state' => $log->frozen ? t('on') : t('off'), 
          ))
          . '<br/>' . t('Content type %type has live render: <b>%state</b>', array(
            '%type' => $node_type, 
            '%state' => comment_driven_is_live_render($node_type) ? t('enabled') : t('disabled'), 
          ));
        
        if (!empty($tokens)) {
          $output[$cid]['tokens'] = array(
            '#type' => 'fieldset',
            '#title' => t('Tokens'),
            '#description' => t('Note that token\'s diff summary should match "frozen" or "live render" deppending on per log frozen flag or per content type live render setting.'),
            '#collapsible' => TRUE,
          );
          // don't need token module to test our own tokens
          $token_values = comment_driven_token_values('comment', $comment);
          foreach ($tokens as $token) {
            $output[$cid]['tokens'][$token] = array(
              '#type' => 'item',
              '#value' => ($token == 'plain_text')
                ? nl2br($token_values['comment-driven-diff-lf'])
                : $token_values['comment-driven-diff-' . $token],
            );
            if ($token == 'table') {
              $table = &$output[$cid]['tokens'][$token]['#value'];
              // need a minimal style to avoid width:100%
              // it could be class="comment_driven",
              // but lets make clear that it didn't passed through an advanced theme function
              // and ensure theming overrides won't force it to a larger width
              $table = str_replace('<table>', '<table style="width:auto">', $table);
            }
          }
        }
        
        $phase = 'diff_values';
        if ($phases[$phase]) {
          $var_export = '';
          foreach ($log->changes as $property_id => $change) {
            extract($change); // $label, $old, $new
            // using check_plain to make HTML visible
            $var_export .= $property_id . ' (-) ' . check_plain(var_export($old, TRUE)) . '<br/>';
            $var_export .= $property_id . ' (+) ' . check_plain(var_export($new, TRUE)) . '<br/>';
          }
          // let tokens be near frozen/live_render for visual comparison
          // therefore, not nested within $output[$cid]['phases']
          $output[$cid][$phase] = array(
            '#type' => 'fieldset',
            '#title' => $form['inspect']['phases'][$phase]['#title'],
            '#collapsible' => TRUE,
            '#value' => '<code>' . $var_export . '</code>',
            '#weight' => -10,
          );
        }
        
        foreach (array('frozen', 'live_render') as $phase) {
          if (!$phases[$phase]) continue;
          
          switch ($phase) {
            case 'live_render':
              $diff_render = driven_diff_render($node_type, $log->changes);
              break;
            case 'frozen':
              $diff_render = $log->diff_render;
              break;
          }
          $output[$cid]['phases'][$phase] = array(
            '#type' => 'fieldset',
            '#title' => $form['inspect']['phases'][$phase]['#title'],
            '#collapsible' => TRUE,
            '#value' => driven_diff_summary_table($diff_render),
          );
        }
      }
    }
  }
  
  $arbitrary_vid_pairs = empty($cids);
  $phase = 'live_comparison';
  if ($phases[$phase] || $schema != 'comment_driven') {
    // if !$arbitrary_vid_pairs then each $index is a $cid 
    // which will match above $output[$cid]['phases'][$phase]
    foreach ($old_vids as $index => $old_vid) {
      $new_vid = $new_vids[$index];

      $diff_summary = t('Could not perform comparison.');
      $errors = array();
      // if input was vid-pairs then we haven't figured out $node_type yet
      // we waited for diff_values_refresh to perform comparison
      // to know if both vids in the pair matched the same content type
      // if so, $node_type (passed by ref) will be properly set in passing by
      // if input was cids then we already have $node_type, but it should come back being the same
      $changes = driven_inspect_diff_vids($old_vid, $new_vid, $errors, $node_type);
      // FALSE means comparison couldn't be performed
      if ($changes !== FALSE) {
        // $errors might be non empty,
        // but if $changes isn't FALSE then errors aren't fatal
        if (empty($changes)) {
          $diff_summary = t('No changes were detected.');
        }
        else {
          $diff_render = driven_diff_render($node_type, $changes);
          $diff_summary = driven_diff_summary_table($diff_render);
        }
      }
      
      $errors[] = t('Be aware that ONLY properties which support revisions will be shown on live comparison.');
      $errors[] = t('Also note that CCK restructuring might cause property revisions lost (mostly when changing fields\' multiplicity).');
      $output[$index]['phases'][$phase] = array(
        '#type' => 'fieldset',
        '#title' => $form['inspect']['phases'][$phase]['#title'],
        '#collapsible' => TRUE,
        '#value' => $diff_summary,
        '#description' => implode('<br/>', $errors),
      );
      
      if ($arbitrary_vid_pairs) {
        // the only existing fielset is the live comparison
        // and it is not wrapped in anything
        $output[$index]['phases'][$phase]['#title'] .= ' {vid-pair: ' . $old_vid . '-' . $new_vid . '}';
        
        $query = 'SELECT nid, vid FROM {node_revisions} WHERE vid IN (%d, %d)'; 
        $result = db_query($query, $old_vid, $new_vid);
        while($row = db_fetch_array($result)) {
          $nid = $row['nid'];
          $vid = $row['vid'];
          if ($vid == $old_vid) {
            $old_nid = $nid;
          }
          else {
            $new_nid = $nid;
          }
        }
        if (!empty($errors)) {
          $output[$index]['phases'][$phase]['#description'] .= '<br/>';
        }
        $output[$index]['phases'][$phase]['#description'] .= t('revisions:') . l($old_vid, "node/$old_nid/revisions/$old_vid/view") . '-' . l($new_vid, "node/$new_nid/revisions/$new_vid/view");
      } 
    }
  }
  
  $form['output'] = $output;
  return $form;
}

function comment_driven_inspect_form_submit($form, &$form_state) {
  // we don't want to do nothing else than falling back to the same form
  // kind of self post
  $form_state['rebuild'] = TRUE;
}
