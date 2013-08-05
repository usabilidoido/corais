<?php

class sheetnode_handler_field_named_range extends views_handler_field {
  function option_definition() {
    $options = parent::option_definition();
    $options['named_range'] = array('default' => '');
    $options['labels'] = array('default' => SHEETNODE_RANGE_LABELS_ROW_COL);
    return $options;
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    $form['named_range'] = array(
      '#type' => 'textfield',
      '#title' => t('Named range'),
      '#default_value' => $this->options['named_range'],
      '#description' => t('A named range which might refer to a literal value, a cell or a range.'),
    );
    $form['labels'] = array(
      '#type' => 'radios',
      '#title' => t('Labels'),
      '#options' => array(
        0 => t('None'),
        SHEETNODE_RANGE_LABELS_ROW => t('Rows only'),
        SHEETNODE_RANGE_LABELS_COL => t('Columns only'),
        SHEETNODE_RANGE_LABELS_ROW_COL => t('Rows and columns'),
      ),
      '#default_value' => $this->options['labels'],
      '#description' => t('Select whether row or column labels should appear in case of a range.'),
    );
  }

  function render($values) {
    require_once(drupal_get_path('module', 'sheetnode') .'/socialcalc.inc');
    $sheet = $values->{$this->field_alias};
    if (empty($sheet)) return NULL;
    $sc = socialcalc_parse_sheet($sheet);

    if (!empty($sc['names'])) foreach ($sc['names'] as $name => $info) {
      if (strcasecmp($name, $this->options['named_range']) == 0) {
        $reference = $info['definition'];

        // Is it a literal value?
        if (substr($reference, 0, 1) == '=') {
          return substr($reference, 1);
        }
        // Is it a cell reference?
        else if (strpos($reference, ':') === FALSE) {
          return isset($sc['cells'][$reference]) ? $sc['cells'][$reference]['datavalue'] : NULL;
        }
        // It's a range reference.
        else {
          $range = preg_split('/:/', $reference, 2);
          $start = socialcalc_coord_to_cr($range[0]);
          $end   = socialcalc_coord_to_cr(isset($range[1]) ? $range[1] : $range[0]);
          if (empty($end[0])) $end[0] = $sc['attribs']['lastcol'];
          if (empty($end[1])) $end[1] = $sc['attribs']['lastrow'];

          // Build the range of values.
          $range = array();
          for ($r=$start[1]; $r<=$end[1]; $r++) {
            for ($c=$start[0]; $c<=$end[0]; $c++) {
              $coord = socialcalc_cr_to_coord($c, $r);
              $range[$coord] = !empty($sc['cells'][$coord]['datavalue']) ? $sc['cells'][$coord]['datavalue'] : NULL;
            }
          }
          return theme('sheetnode_range', $range, $this->options['labels']);
        }
      }
    }

    return '';
  }
}

