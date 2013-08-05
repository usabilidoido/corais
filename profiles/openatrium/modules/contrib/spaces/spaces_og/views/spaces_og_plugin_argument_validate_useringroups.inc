<?php

/**
 * Validate whether an argument is a valid user that is a member of one
 * of the current user's groups. This check works through the implementation
 * of db_rewrite_sql() in spaces_og.
 */
class spaces_og_plugin_argument_validate_useringroups extends views_plugin_argument_validate {
  function validate_form(&$form, &$form_state) {
    // We are unable to rely on options having already been set, so let's make
    // sure defaults are here:
    if (!isset($this->argument->options['validate_user_argument_type'])) {
      $this->argument->options['validate_user_argument_type'] = 'uid';
    }

    $form['validate_user_argument_type'] = array(
      '#type' => 'radios',
      '#title' => t('Type of user argument to allow'),
      '#options' => array(
        'uid' => t('Only allow numeric UIDs'),
        'name' => t('Only allow string usernames'),
        'either' => t('Allow both numeric UIDs and string usernames'),
      ),
      '#default_value' => $this->argument->options['validate_user_argument_type'],
      '#process' => array('expand_radios', 'views_process_dependency'),
      '#dependency' => array('edit-options-validate-type' => array($this->id)),
      '#prefix' => '<div id="edit-options-validate-user-argument-type-wrapper">',
      '#suffix' => '</div>',
    );
  }

  function validate_argument($argument) {
    $type = $this->argument->options['validate_user_argument_type'];
    // is_numeric() can return false positives, so we ensure it's an integer.
    // However, is_integer() will always fail, since $argument is a string.
    if (is_numeric($argument) && $argument == (int)$argument) {
      if ($type == 'uid' || $type == 'either') {
        $where = 'u.uid = %d';
      }
    }
    else {
      if ($type == 'name' || $type == 'either') {
        $where = "u.name = '%s'";
      }
    }
    if (!empty($where)) {
      $query = db_rewrite_sql("SELECT u.uid FROM {users} u WHERE {$where}", 'u', 'uid', array($argument));
      $validated = db_result(db_query($query, $argument));
      return $validated;
    }
    return FALSE;
  }
}
