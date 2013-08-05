<?php
/**
 * Field handler to show case counts for projects.
 *
 * @ingroup views_field_handlers
 */
class casetracker_views_handler_field_case_count extends views_handler_field {
  // Empty query method -- we query in pre_render().
  function query() {
  }

  // Add options for case types.
  function option_definition() {
    $options = parent::option_definition();
    $options['type'] = array('default' => 0);
    return $options;
  }

  // Options form. Use fieldgroups to simplify selection.
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $options = array(
      0 => t('All cases'),
      t('Priority') => array(),
      t('Status') => array(),
      t('Type') => array(),
    );
    $realms = array(
      'priority' => t('Priority'),
      'status' => t('Status'),
      'type' => t('Type'),
    );
    foreach ($realms as $realm => $realm_name) {
      foreach (casetracker_realm_load($realm) as $csid => $name) {
        // @TODO: this is a user-configured string -- should not be wrapped in t().
        $options[$realm_name]["{$realm}-{$csid}"] = t($name);
      }
    }
    $form['type'] = array(
      '#type' => 'select',
      '#title' => t('Count for type'),
      '#options' => $options,
      '#default_value' => $this->options['type'],
      '#description' => t('Text to put after the number, such as currency symbol.'),
    );
  }

  // Query for counts once all rows have been collected.
  function pre_render($values) {
    $nids = array();
    foreach ($values as $row) {
      $nids[] = $row->nid;
    }
    if (!empty($nids)) {
      $this->counts = array();
      $placeholders = db_placeholders($nids, 'int');

      // Add a filter by realm/type if option is set.
      $where = '';
      if (!empty($this->options['type'])) {
        $filter = explode('-', $this->options['type']);
        $where = "AND case_{$filter[0]}_id = %d";
        $nids[] = $filter[1];
      }

      $result = db_query("SELECT count(c.nid) AS count, c.pid FROM {casetracker_case} c JOIN {node} n ON c.nid = n.nid AND c.vid = n.vid WHERE c.pid IN ($placeholders) $where GROUP BY c.pid", $nids);
      while ($row = db_fetch_object($result)) {
        $this->counts[$row->pid] = $row->count;
      }
    }
  }

  // Render method.
  function render($values) {
    $count = !empty($this->counts[$values->nid]) ? $this->counts[$values->nid] : 0;
    return "<span class='case-count'>{$count}</span>";
  }
}
