<?php

/**
 * @file form_builder.api.inc
 * Universally used API functions within the Form builder module.
 */

/**
 * Get a list of all properties that are supported within a form type.
 */
function form_builder_get_properties($form_type, $reset = FALSE) {
  static $properties = array();

  if ($reset) {
    $properties = array();
  }

  if (!isset($properties[$form_type])) {
    // Get the list of all properties for all elements.
    foreach (module_implements('form_builder_properties') as $module) {
      $properties = array_merge($properties, module_invoke($module, 'form_builder_properties', $form_type));
    }
    drupal_alter('form_builder_properties', $properties, $form_type);
  }

  return $properties;
}

/**
 * Get a list of all properties that are supported by a particular form type.
 */
function form_builder_get_form_type($form_type = NULL, $reset = FALSE) {
  static $types;

  if (!isset($types) || $reset) {
    $types = array();
    // Get the list of all properties for all elements.
    $types = module_invoke_all('form_builder_types');

    // Add default values for undefined properties.
    foreach ($types as $type_key => &$type) {
      $groups = module_invoke_all('form_builder_palette_groups', $type_key);

      foreach ($type as $field_key => &$field) {
        $field['unique'] = isset($field['unique']) && $field['unique'];
        $field['configurable'] = isset($field['configurable']) ? $field['configurable'] : TRUE;
        $field['removable'] = isset($field['removable']) ? $field['removable'] : TRUE;
        $field['addable'] = isset($field['addable']) ? $field['addable'] : $field['removable'] && isset($field['default']);
        $field['palette_group'] = isset($field['palette_group']) && isset($groups[$field['palette_group']]) ? $field['palette_group'] : 'default';
        $field['properties'] = isset($field['properties']) ? $field['properties'] : array();

        // All fields must support weight.
        if (!in_array('weight', $field['properties'])) {
          $field['properties'][] = 'weight';
        }

        // Update the default elements with some defaults.
        // Note that if a field is not removable, it doesn't have a default.
        if ($field['addable']) {
          if ($field['unique']) {
            $field['default']['#form_builder']['element_id'] = $field_key;
            $field['default']['#form_builder']['element_type'] = $field_key;
          }
          elseif (!isset($field['default']['#form_builder']['element_type'])) {
            $field['default']['#form_builder']['element_type'] = $field_key;
          }
        }
      }

      // Sort fields by weight and title.
      uasort($type, '_form_builder_sort');
    }

    drupal_alter('form_builder_types', $types);
  }

  return isset($form_type) ? $types[$form_type] : $types;
}

/**
 * Given an element type, return properties that are supported by Form builder.
 *
 * @param $form_type
 *   The type of form being edited (profile, node, webform, etc.)
 * @param $element_type
 *   A the #type property of a FAPI element.
 * @param
 *   An associative array of properties supported for editing, keyed by
 *   the property name and containing an array of the form, submit, and
 *   validate functions (if any).
 */
function form_builder_get_element_properties($form_type, $element_type) {
  // Get the list of all properties for this type.
  $properties = form_builder_get_properties($form_type);

  // Get the list of supported properties per field in this form type.
  $form_type = form_builder_get_form_type($form_type);

  $element_properties = array();
  if (isset($form_type[$element_type]['properties'])) {
    foreach ($form_type[$element_type]['properties'] as $property) {
      if (isset($properties[$property])) {
        $element_properties[$property] = $properties[$property];
      }
    }
  }

  return $element_properties;
}

/**
 * Get a list of properties that are supported in any way by an element.
 *
 * This returns a list of all supported properties within an element, even
 * if some of those properties do not have an interface for editing or are
 * only used internally by the module providing the form type this element
 * is being saved in.
 *
 * @param $form_type
 *   The type of form being edited (profile, node, webform, etc.)
 * @param $element
 *   A standard FAPI element whose properties are being checked.
 * @return
 *   A non-indexed list of properties that may be saved for this element.
 * */
function form_builder_get_saveable_properties($form_type, $element) {
  // Get the list of supported properties on each element.
  $form_type = form_builder_get_form_type($form_type);

  $saveable = array();
  if (isset($form_type[$element['#form_builder']['element_type']]['properties'])) {
    $saveable = $form_type[$element['#form_builder']['element_type']]['properties'];
  }

  return $saveable;
}

/**
 * Function to retrieve a single element within a form structure.
 *
 * If needing to retreive multiple elements at once, use
 * form_builder_get_elements().
 *
 * @param $form
 *   A complete hierarchical FAPI structure.
 * @param $element_id
 *   The unique identifier for an element that is to be retrieved.
 * @return
 *   A single Form API element array.
 *
 * @see form_builder_get_elements().
 */
function form_builder_get_element(&$form, $element_id) {
  $elements = form_builder_get_elements($form, array($element_id));
  return isset($elements[$element_id]) ? $elements[$element_id] : FALSE;
}

/**
 * Recursive function to retrieve multiple elements within a form structure.
 *
 * @param $form
 *   A complete hierarchical FAPI structure.
 * @param $element_ids
 *   An array of unique identifiers for elements that are to be retreived. These
 *   identifiers match against the special property
 *   "#form_builder['element_id']", which is not available in normal FAPI
 *   structures. It must be added by the respective module that is providing
 *   support for a certain field type.
 *
 *   For example, CCK provides a unique identifier for each field such as
 *   "field_my_name". This field name must be added to the form array as
 *   #form_builder['element_id'] = 'field_my_name' in CCK's implementation of
 *   hook_form_builder_load().
 * @return
 *   A single Form API element array.
 */
function form_builder_get_elements(&$form, $element_ids) {
  $elements = array();
  foreach (element_children($form) as $key) {
    if (isset($form[$key]['#form_builder']['element_id']) && in_array($form[$key]['#form_builder']['element_id'], $element_ids)) {
      $elements[$form[$key]['#form_builder']['element_id']] = $form[$key];
    }
    $additional_elements = form_builder_get_elements($form[$key], $element_ids);
    $elements = array_merge($elements, $additional_elements);
  }
  return $elements;
}

/**
 * Recursive function to set an element within a form structure.
 *
 * @return
 *   TRUE if an element was updated, FALSE if it was not found.
 */
function form_builder_set_element(&$form, $element, &$entire_form = NULL, $parent_id = FORM_BUILDER_ROOT) {
  $return = FALSE;

  if (!isset($entire_form)) {
    $entire_form = &$form;
  }

  // Add new elements into the current parent.
  if (isset($element['#form_builder']['is_new']) && strcmp($element['#form_builder']['parent_id'], $parent_id) == 0) {
    unset($element['#form_builder']['is_new']);
    unset($element['#form_builder']['parent_id']);
    $new_key = $element['#key'];
    $form[$new_key] = $element;
    return TRUE;
  }

  foreach (element_children($form) as $key) {
    // Update an existing element if it lives in the current parent.
    if (isset($form[$key]['#form_builder']['element_id']) && $form[$key]['#form_builder']['element_id'] == $element['#form_builder']['element_id']) {

      // If the parent has changed, re-parent the element to a new fieldset.
      if (isset($element['#form_builder']['parent_id']) && strcmp($element['#form_builder']['parent_id'], $parent_id) != 0) {
        // Remove the current element from the form.
        unset($form[$key]);
        // Recurse again through the entire form to insert into the new position.
        $element['#form_builder']['is_new'] = TRUE;
        $return = form_builder_set_element($entire_form, $element);
      }
      // Handle key changes and replace the existing element in place.
      elseif (isset($element['#key']) && $key != $element['#key']) {
        $new_key = $element['#key'];
        $index = array_search($key, array_keys($form));
        $before = array_slice($form, 0, $index, TRUE);
        $after = array_slice($form, $index + 1, NULL, TRUE);
        $form = $before + array($new_key => $element) + $after;
        unset($form[$key]);
        $return = TRUE;
      }
      // Or, most common case scenario, just update the element, no key changes.
      else {
        $form[$key] = $element;
        $return = TRUE;
      }
    }

    // Recurse into this element to look for the target element.
    if (!$return && isset($form[$key]['#form_builder'])) {
      $return = form_builder_set_element($form[$key], $element, $entire_form, $form[$key]['#form_builder']['element_id']);
    }

    if ($return) {
      return $return;
    }
  }

  return $return;
}

/**
 * Recursive function to unset an element within a form structure.
 */
function form_builder_unset_element(&$form, $element_id) {
  foreach (element_children($form) as $key) {
    if (isset($form[$key]['#form_builder']['element_id']) && $form[$key]['#form_builder']['element_id'] == $element_id) {
      unset($form[$key]);
      break;
    }
    form_builder_unset_element($form[$key], $element_id);
  }
}

/**
 * Recursive function to check if an element exists at all within a form.
 */
function form_builder_get_element_ids($form) {
  $element_ids = array();
  foreach (element_children($form) as $key) {
    if (isset($form[$key]['#form_builder']['element_id'])) {
      $element_ids[] = $form[$key]['#form_builder']['element_id'];
    }
    $additional_ids = form_builder_get_element_ids($form[$key]);
    $element_ids = array_merge($element_ids, $additional_ids);
  }

  return $element_ids;
}

/**
 * Recursive function to get the types of all element within a form.
 */
function form_builder_get_element_types($form) {
  $element_types = array();
  foreach (element_children($form) as $key) {
    if (isset($form[$key]['#form_builder']['element_type'])) {
      $element_types[] = $form[$key]['#form_builder']['element_type'];
    }
    $additional_types = form_builder_get_element_types($form[$key]);
    $element_types = array_merge($element_types, $additional_types);
  }

  return $element_types;
}

/**
 * Loader function to retrieve a form builder configuration array.
 *
 * @param $form_type
 *   The type of form being edited. Usually the name of the providing module.
 * @param $form_id
 *   The unique identifier for the form being edited with the type.
 */
function form_builder_load_form($form_type, $form_id) {
  $form = module_invoke_all('form_builder_load', $form_type, $form_id);
  drupal_alter('form_builder_load', $form, $form_type, $form_id);

  // Convert the form array keys to #key properties for editing.
  return form_builder_add_default_properties($form, $form_type);
}

/**
 * Execute the save methods for a form array.
 */
function form_builder_save_form(&$form, $form_type, $form_id) {
  module_invoke_all('form_builder_save', $form, $form_type, $form_id);
  form_builder_cache_delete($form_type, $form_id);
}

/**
 * Helper function to add default #form_builder properties to a form.
 */
function form_builder_add_default_properties($form, $form_type, $key = NULL, $parent_id = FORM_BUILDER_ROOT) {
  $form_type_fields = form_builder_get_form_type($form_type);

  // Add properties to this element.
  if (isset($form['#form_builder']['element_id'])) {
    $element_id = $form['#form_builder']['element_id'];

    // Add a #key property.
    $form['#key'] = isset($form['#key']) ? $form['#key'] : $key;

    // Add a #form_builder['parent_id'] property.
    $form['#form_builder']['parent_id'] = $parent_id;
    $parent_id = $element_id;

    // Set defaults based on the form type.
    if (isset($form_type_fields[$element_id]) && $form_type_fields[$element_id]['unique']) {
      $form['#form_builder']['unique'] = TRUE;
      $form['#form_builder']['element_type'] = isset($form['#form_builder']['element_type']) ? $form['#form_builder']['element_type'] : $element_id;
      $settings = $form_type_fields[$element_id];
    }
    else {
      $form['#form_builder']['element_type'] = isset($form['#form_builder']['element_type']) ? $form['#form_builder']['element_type'] : $form['#type'];
      if (isset($form_type_fields[$form['#form_builder']['element_type']])) {
        $settings = $form_type_fields[$form['#form_builder']['element_type']];
      }
      else {
        // If the type cannot be found, prevent editing of this field.
        unset($form['#form_builder']);
        return $form;
      }
    }

    // Set defaults for configurable and removable.
    if (!isset($form['#form_builder']['configurable'])) {
      $form['#form_builder']['configurable'] = isset($settings['configurable']) ? $settings['configurable'] : TRUE;
    }
    if (!isset($form['#form_builder']['removable'])) {
      $form['#form_builder']['removable'] = isset($settings['removable']) ? $settings['removable'] : TRUE;
    }
  }

  // Recurse into sub-elements.
  foreach (element_children($form) as $key) {
    if (isset($form[$key]['#form_builder']['element_id'])) {
      $form[$key] = form_builder_add_default_properties($form[$key], $form_type, $key, $parent_id);
    }
  }

  return $form;
}

/**
 * Helper function to sort elements by 'weight' and 'title'.
 */
function _form_builder_sort($a, $b) {
  $a_weight = (is_array($a) && isset($a['weight'])) ? $a['weight'] : 0;
  $b_weight = (is_array($b) && isset($b['weight'])) ? $b['weight'] : 0;
  if ($a_weight == $b_weight) {
    if (!isset($b['title'])) {
      return -1;
    }
    if (!isset($a['title'])) {
      return 1;
    }
    return strcasecmp($a['title'], $b['title']);
  }
  return ($a_weight < $b_weight) ? -1 : 1;
}
