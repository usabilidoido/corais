<?php
/**
 * @file
 * Support file for the CCK Content Taxonomy module
 *
 * NOTE: If you have numeric terms, precede them in your import file with a #. If you need to actually start a string
 * with the # sign, then double it up like ##string and it will be converted into #string.
 */


/**
 * Implementation of hook_node_import_fields()
 */
function content_taxonomy_node_import_fields($type) {
  $fields = array();

  foreach (node_import_cck_fields($type, 'content_taxonomy') as $fieldname => $fieldinfo) {
    foreach ($fieldinfo['columns'] as $colname => $colinfo) {
      $cck_fieldname          = node_import_cck_name($fieldname, $colname);
      $fields[$cck_fieldname] = node_import_cck_field_defaults($fieldinfo);

      switch ($colname) {
        case 'value':
          $fields[$cck_fieldname]['title']        = $fieldinfo['widget']['label'];
          $fields[$cck_fieldname]['map_required'] = $fieldinfo['required'];
          break;
        default:
          $fields[$cck_fieldname]['title'] = t('Unsupported: ') . $fieldinfo['widget']['label'] .' - '. $colname;
      }
    }
  }
  return $fields;
}

/**
 * Implementation of hook_node_import_values_alter()
 */
function content_taxonomy_node_import_values_alter(&$values, $type, $defaults, $options, $fields, $preview) {
  // Place the following line in the validation function on the content_taxonomy module in order to determine
  // what the widget is expecting:
  //
  //   drupal_set_message( '<pre>Element: ' . print_r( $element, TRUE ) . '</pre>' );
  //
  // For example, I placed the above line in the content_taxonomy_tree_validate function in order to discover
  // the data structure that the validator expects. I assume the other widgets expect different structures.

  foreach (node_import_cck_fields($type, 'content_taxonomy') as $fieldname => $fieldinfo) {
    $vid = $fieldinfo['vid'];
    //***************************************tree widget***************************************
    if ($fieldinfo['widget']['type'] == 'content_taxonomy_tree') {
      $terms   = array();
      $results = array();
      foreach ($values[$fieldname] as $i => $value) {
        if (is_numeric($value['value'])) {
          $foundterms = taxonomy_get_term(intval($value['value']));
        }
        else {
          $value['value'] = content_taxonomy_node_import_check_numeric($value['value']);
          $foundterms = taxonomy_get_term_by_name($value['value']);
        }
        foreach ($foundterms as $index => $termfound) {
          if ($termfound->vid == $vid) {
            $tid = $termfound->tid;
            $terms[$tid] = (array)$termfound;
            $children = taxonomy_get_children($tid, $vid);
            $terms[$tid]['children'] = $children;
            $parents = taxonomy_get_parents($tid);
            $terms[$tid]['parents'] = $parents;
          }
        }
      }

      foreach ($terms as $term_id => $data) {
        if (empty($data['parents'])) {
          // We only want top-level terms here
          $results['value'][$term_id] = array('checkbox' => $term_id);

          if (!empty($data['children'])) {
            // Handle the children terms
            content_taxonomy_node_import_tree_adjust($terms, $data, $results['value'][$term_id]['children']);
          }
          else {
            unset($terms[$term_id]);
          }
        }
      }
      $values[$fieldname] = $results;
    }
    //***************************************options widget***************************************
    elseif ($fieldinfo['widget']['type'] == 'content_taxonomy_options') {
      // Content taxonomy checkboxes
      $results = array();
      foreach ($values[$fieldname] as $i => $value) {
        if (is_numeric($value['value'])) { // Using TID...
          $sql = "SELECT tid FROM {term_data} WHERE vid = %d AND tid = %d";
          if ($tid = db_result(db_query($sql, $vid, intval($value['value'])))) {
            $results[$tid] = $tid;
          }
        }
        else { // Using term name...
          $value['value'] = content_taxonomy_node_import_check_numeric($value['value']);
          $sql = "SELECT tid FROM {term_data} WHERE vid = %d AND LOWER(name) = '%s'";
          if ($tid = db_result(db_query($sql, $vid, drupal_strtolower($value['value'])))) {
            $results[$tid] = $tid;
          }
        }
      }
      $values[$fieldname]['value'] = $results;
    }
    //***************************************hierarchical select widget***************************************
    elseif ($fieldinfo['widget']['type'] == 'content_taxonomy_hs') {
      $terms   = array();
      $results = array();
      foreach ($values[$fieldname] as $i => $value) {
        $position = stripos($value['value'], '>>');
        if ($position) {//using the delimiter, we are given the explicit parentage of the term. we can therefore have duplicate terms.
          $lineage = explode('>>', $value['value']);
          $terms = content_taxonomy_node_import_get_children($lineage, $vid);
        }
        else {//we have just one term, hopefully there are no duplicates, so let's find the parents
          if (is_numeric($value['value'])) {
            $foundterms = taxonomy_get_term(intval($value['value']));
          }
          else {
            $value['value'] = content_taxonomy_node_import_check_numeric($value['value']);
            $foundterms = taxonomy_get_term_by_name($value['value']);
          }
          foreach ($foundterms as $index => $termfound) {
            if ($termfound->vid == $vid) {
              $tid = $termfound->tid;
              $terms[$tid] = (array)$termfound;
              $children = taxonomy_get_children($tid, $vid);
              $terms[$tid]['children'] = $children;
              $parents = taxonomy_get_parents($tid);
              $terms[$tid]['parents'] = $parents;
            }
          }
        }
      }

      //We now have our list of hierarchical terms in order of parent>>child>>child, etc. Now let's arrange them how
      //hierarchical select expects them to be.
      if (!empty($terms)) {
        foreach ($terms as $index => $data) {
          $results[] = $data['tid'];
        }
        $values[$fieldname]['tids'] = $results;
      }
      else {
        $values[$fieldname]['tids'] = NULL;
      }
    }
    //***************************************other widgets***************************************
    else {
      foreach ($values[$fieldname] as $i => $value) {
        if (is_numeric($value['value'])) { // Using TID...
          $sql = "SELECT tid FROM {term_data} WHERE vid = %d AND tid = %d";
          if ($tid = db_result(db_query($sql, $vid, intval($value['value'])))) {
            $values[$fieldname][$i]['value'] = $tid;
          }
          else {
            $value['value'] = content_taxonomy_node_import_check_numeric($value['value']);
            if (trim($value['value']) != '') {
              $edit = array('vid' => $vid, 'name' => $value['value']);
              taxonomy_save_term($edit);
              $sql = "SELECT tid FROM {term_data} WHERE vid = %d AND tid = %d";
              if ($tid = db_result(db_query($sql, $vid, intval($value['value'])))) {
                $values[$fieldname][$i]['value'] = $tid;
              }
            }
          }
        }
        else { // Using term name...
          $sql = "SELECT tid FROM {term_data} WHERE vid = %d AND LOWER(name) = '%s'";
          if ($tid = db_result(db_query($sql, $vid, drupal_strtolower($value['value'])))) {
            $values[$fieldname][$i]['value'] = $tid;
          }
          else {
            $value['value'] = content_taxonomy_node_import_check_numeric($value['value']);
            if (trim($value['value']) != '') {
              $edit = array('vid' => $vid, 'name' => $value['value']);
              taxonomy_save_term($edit);
              $sql = "SELECT tid FROM {term_data} WHERE vid = %d AND LOWER(name) = '%s'";
              if ($tid = db_result(db_query($sql, $vid, drupal_strtolower($value['value'])))) {
                $values[$fieldname][$i]['value'] = $tid;
              }
            }
          }
        }
      }
    }
  }
}

/**
 * Used to contruct the lineage of terms for use by hierarchical select widget.
 * 
 * @param <type> $lineage
 * @param <type> $vid
 * @param <type> $parent_tid
 * @return <type>
 */
function content_taxonomy_node_import_get_children($lineage, $vid, $parent_tid = NULL) {
  $top = array_shift($lineage);
  $remainder = $lineage;
  if (is_numeric($top)) {
    $foundterms = taxonomy_get_term(intval($top));
  }
  else {
    $top = content_taxonomy_node_import_check_numeric($top);
    $foundterms = taxonomy_get_term_by_name($top);
  }
  //first, let's clear out any terms in other vocabularies
  foreach ($foundterms as $index => $termfound) {
    if ($termfound->vid != $vid) {
      unset($foundterms[$index]);
    }
  }
  if (empty($foundterms)) {
    node_import_input_error(t("The term %value was not found at the expected level in your taxonomy hierarchy. Please check your import file and try again.", array('%value' => $top)));
    return NULL;
  }
  //let's take what we have left and make sure we found the right child - there should just be one term left after the parent sifting that follows
  $current_tid = 0;
  foreach ($foundterms as $index => $termfound) {
    $tid = $termfound->tid;
    $parents = taxonomy_get_parents($tid);
    if (($parent_tid != NULL && !isset($parents[$parent_tid])) || ($parent_tid == NULL && !empty($parents))) {
      unset($foundterms[$index]);//we found a term whose text matched, but isn't in the right spot of the hierarchy
      continue;
    }
    else {
      $current_tid = $tid;
    }
    //drupal_set_message('<pre>Parents: ' . print_r($parents, TRUE) . '</pre>');
    $terms[$tid] = (array)$termfound;
    $children = taxonomy_get_children($tid, $vid);
    $terms[$tid]['children'] = $children;    
    $terms[$tid]['parents'] = $parents;
  }
  if (empty($terms)) {
    node_import_input_error(t("The term %value was not found at the expected level in your taxonomy hierarchy, but a match was found at a different level. Please check your import file and try again.", array('%value' => $top)));
    return NULL;
  }
  if (count($terms) > 1) {
    node_import_input_error(t("The term %value was found MORE THAN ONCE as children of the same term. Please check your import file and try again.", array('%value' => $top)));
    return NULL;
  }
  if (is_array($remainder) && !empty($remainder)) {
    //drupal_set_message('<pre>Terms inside: ' . print_r($terms, TRUE) . '</pre>');
    $child_terms = content_taxonomy_node_import_get_children($remainder, $vid, $current_tid);
    if ($child_terms != NULL) {
      $terms = array_merge($terms, $child_terms);
    }
  }
  return $terms;
}

/**
 * This is for using numeric values as terms. Normally, numeric values are treated as TIDs. Using a # in front of your
 * number will make sure it is treated as a value.
 * 
 * @param <type> $value
 * @return <type>
 */
function content_taxonomy_node_import_check_numeric($value) {
  $pos = strpos($value, "#");
  if ($pos === 0) {
    $value = substr($value, 1);
  }
  return $value;
}

/**
 * This is for fixing the output of the tree widget type.
 *
 * @param <type> $terms
 * @param <type> $data
 * @param <type> $results
 */
function content_taxonomy_node_import_tree_adjust(&$terms, &$data, &$results) {
  foreach ($data['children'] as $id => $contents) {
    if (key_exists($id, $terms)) {
      $results[$id] = array('checkbox' => $id);
      if (!empty($terms[$id]['children'])) {
        content_taxonomy_node_import_tree_adjust($terms, $terms[$id], $results[$id]['children']);
      }
      else {
        // Unset the item from the terms list; this should be a leaf term
        unset($terms[$id]);
      }
    }
  }
}
