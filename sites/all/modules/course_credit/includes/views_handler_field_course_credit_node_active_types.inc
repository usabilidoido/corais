<?php

/**
 * Field handler to provide a list of credit titles.
 */
class views_handler_field_course_credit_node_active_types extends views_handler_field_prerender_list {

  /**
   * Working with node table, adding active credit types to it.
   */
  function construct() {
    parent::construct();
    $this->additional_fields['nid'] = array('table' => 'node', 'field' => 'nid');
  }

  /**
   * Add fields, alias.
   */
  function query() {
    $this->add_additional_fields();
    $this->field_alias = $this->aliases['nid'];
  }

  /**
   * Get the active credit types on each of the nodes in this view.
   */
  function pre_render($values) {
    foreach ($values as $value) {
      $nids[] = $value->nid;
    }

    if ($nids) {
      $placeholders = db_placeholders($nids);
      $sql = "SELECT cct.type, cct.title, cct.description, cc.* FROM {course_credit} cc
        LEFT JOIN {course_credit_type} cct USING (type)
        WHERE cc.active AND nid IN ($placeholders)";

      if ($this->options['course_credit_types']) {
        $selected = array_filter($this->options['course_credit_types']);
        if ($selected) {
          if ($this->options['course_credit_types_negate']) {
            $in = " AND cct.type NOT IN ";
          }
          else {
            $in = " AND cct.type IN ";
          }

          $in .= "('" . implode("','", $selected) . "')";
          $sql .= $in;
        }
      }
      $result = db_query($sql, $nids);
      while ($row = db_fetch_array($result)) {
        foreach ($row as $key => $value) {
          $key = "[{$key}]";
          $this->items[$row['nid']][$row['type']][$key] = $value;
        }
        $this->items[$row['nid']][$row['type']]["[max-s]"] = format_plural($row['max'], '', 's');
        $this->items[$row['nid']][$row['type']]["[min-s]"] = format_plural($row['min'], '', 's');
      }
    }
  }

  /**
   * Render each credit title.
   *
   * This will use tokens on the field if provided.
   */
  function render_item($count, $item) {
    $out = $this->options['course_credit_types_format'];
    if (!$out) {
      $out = '[title]';
    }

    $out = strtr($out, $item);

    return $out;
  }

  /**
   * Declare options.
   */
  function option_definition() {
    $options = parent::option_definition();

    $options['course_credit_types'] = array('default' => array());
    $options['course_credit_negate'] = array('default' => FALSE);
    $options['course_credit_types_format'] = array('default' => '');

    return $options;
  }

  /**
   * Select credit types to show.
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    $types = course_credit_get_credit_types();

    if ($types) {
      $options = array();
      foreach ($types as $ctid => $type) {
        $options[$type->type] = $type->title;
      }
    }

    $form['course_credit_types'] = array(
      '#title' => 'Credit types',
      '#type' => 'checkboxes',
      '#options' => $options,
      '#description' => 'Select credit types to show instead of showing all types.',
      '#default_value' => $this->options['course_credit_types'] ? $this->options['course_credit_types'] : array(),
    );

    $form['course_credit_types_negate'] = array(
      '#title' => 'Negate',
      '#type' => 'checkbox',
      '#description' => 'Do not show selected types.',
      '#default_value' => $this->options['course_credit_types_negate'],
    );

    $form['course_credit_types_format'] = array(
      '#title' => 'Format',
      '#type' => 'textfield',
      '#description' => 'Specify a format to use other than the name of the active credit type. Possible tokens: [title] [type] [min] [max]. You can also use [min-s] or [max-s] for the "S" after "credit" based on the number of credits.',
      '#default_value' => $this->options['course_credit_types_format'],
    );
  }

}
