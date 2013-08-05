<?php

/**
 * @file
 * This class extends the context_condition
 */
class context_spaces_features_context_condition extends context_condition {
  /**
   * Omit condition values. We will provide a custom input form for our conditions.
   */
  function condition_values() {
    $features = spaces_features();
    foreach ($features as $key => $feature) {
      $options[$key] = $feature->info['name'];
    }
    return $options;
  }

  /**
   * Condition form.
   */
  function condition_form($context) {
    $form = parent::condition_form($context);
    $form['#type'] = 'select';
    $form['#multiple'] = TRUE;
    $form['#description'] = 'Select the features that you want to check if they verify are active.';
    $form['#default_value'] = $this->fetch_from_context($context, 'values');
    return $form;
  }

  /**
   * Condition form submit handler.
   */
  function condition_form_submit($values) {
    return $values;
  }

  /**
   * Execute.
   */
  function execute() {
    foreach (context_enabled_contexts() as $context) {
      if ($vars = $this->fetch_from_context($context, 'values')) {
        $all_conditions_met = TRUE;
        // loop through all selected options
        foreach ($vars as $key => $name) {
          if (!spaces_access_feature('view', $key)) {
            $all_conditions_met = FALSE;
          }
        }
        // if all the conditions are met, pass ahead that they are met
        if ($all_conditions_met) {
          $this->condition_met($context);
        }
      }
    }
  }
}
