<?php

class crayon_views_handler_field_og extends og_views_handler_field_og_group_nids {
  /**
  * Override of option_definition().
  */
  function option_definition() {
    $options = parent::option_definition();
    $options['crayon'] = array('default' => array('enabled' => FALSE, 'acronym' => FALSE));
    return $options;
  }

  /**
  * Override of options_form().
  */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    $form['crayon'] = array(
      '#type' => 'checkboxes',
      '#title' => t('Crayon'),
      '#options' => array(
        'enabled' => t('Display as color swatch'),
        'acronym' => t('Display acronym for swatch text'),
      ),
    );
  }

  /**
  * Override of render().
  */
  function render($values) {
    if (!empty($this->options['crayon']['enabled'])) {
      $field = $values->{$this->field_alias};
      if (!empty($this->items[$field])) {
        $key = key($this->items[$field]);
        $label = !empty($this->options['crayon']['acronym']) ? $this->acronym(strip_tags($this->items[$field][$key])) : $this->items[$field][$key];
        $color = $this->color($key);
        $content = implode(check_plain($this->options['separator']), $this->items[$field]);
        return theme('crayon_swatch', $label, $color, $content);
      }
    }
    else {
      return parent::render($values);
    }
  }

  /**
  * Generate an acronym for a given string value.
  * Override this in a subclass to provide custom acronym generation logic.
  */
  protected function acronym($value) {
    return crayon_generate_acronym($value);
  }

  /**
  * Override this in a subclass to provide custom color generation logic.
  */
  protected function color($value) {
    return $value;
  }
}
