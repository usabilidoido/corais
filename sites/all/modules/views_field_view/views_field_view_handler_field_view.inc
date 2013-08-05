<?php
// $Id: views_field_view_handler_field_view.inc,v 1.1.2.3 2010/08/03 18:09:41 dereine Exp $

class views_field_view_handler_field_view extends views_handler_field {
  function option_definition() {
    $options = parent::option_definition();
    $options['view'] = array('default' => '');
    $options['display'] = array('default' => 'default');
    $options['arguments'] = array('default' => '');

    return $options;
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $options = drupal_map_assoc(array_keys(views_get_all_views()));
    unset($options[$this->view->name]);

    $form['view'] = array(
      '#type' => 'select',
      '#title' => t('View'),
      '#default_value' => $this->options['view'],
      '#options' => $options,
    );
    if ($this->options['view']) {
      $view = views_get_view($this->options['view']);

      $options = array();
      foreach ($view->display as $name => $display) {
        $options[$name] = $display->display_title;
      }

      $form['display'] = array(
        '#title' => t('Display'),
        '#type' => 'select',
        '#default_value' => $this->options['display'],
        '#options' => $options,
      );

      $form['arguments'] = array(
        '#title' => t('Arguments'),
        '#description' => t('Use a comma-seperated list of each argument which should be forwared to the view') . $form['alter']['help']['#prefix'],
        '#type' => 'textfield',
        '#default_value' => $this->options['arguments'],
      );
    }
    $form['alter']['#access'] = FALSE;
  }
  function query() {
    $this->ensure_my_table();
    $this->add_additional_fields();
  }

  function render($values) {
    static $running = array();
    if ($this->options['view']) {
      // Protect against the evil / recursion.
      // Set the variable for yourself, this is not for the normal "user".
      if (empty($running[$this->options['view']][$this->options['display']]) || variable_get('views_field_view_evil', FALSE)) {
        $running[$this->options['view']][$this->options['display']] = TRUE;
        $args = array();
        $args[] = $this->options['view'];
        $args[] = $this->options['display'];

        foreach (explode(',', $this->options['arguments']) as $argument) {
          $alter = array(
            'text' => $argument,
          );
          $tokens = $this->get_render_tokens($alter);
          $value = $this->render_altered($alter, $tokens);
          $args[] = $value;
        }

        $output = call_user_func_array('views_embed_view', $args);
        $running[$this->options['view']][$this->options['display']] = FALSE;
      }
      else {
        $output = t('Recursion, stop!');
      }
      return $output;
    }
  }
}