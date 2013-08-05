<?php
// $Id: views_plugin_argument_default_context.inc,v 1.1.2.1 2010/10/25 20:27:58 brynbellomy Exp $
/**
 * @file
 * Contains the active context argument default plugin.
 */

/**
 * Default argument plugin to extract a value from the active context
 */
class views_plugin_argument_default_context extends views_plugin_argument_default {
  var $option_name = 'views_arg_context';

  function argument_form(&$form, &$form_state) {
    $form['namespace'] = array(
      '#type' => 'textfield',
      '#title' => t('Namespace'),
      '#default_value' => $this->argument->options['namespace'],
      '#process' => array('views_process_dependency'),
      '#dependency' => array(
        'radio:options[default_action]' => array('default'),
        'radio:options[default_argument_type]' => array($this->id)
      ),
      '#dependency_count' => 2,
    );
    $form['attribute'] = array(
      '#type' => 'textfield',
      '#title' => t('Attribute'),
      '#default_value' => $this->argument->options['attribute'],
      '#process' => array('views_process_dependency'),
      '#dependency' => array(
        'radio:options[default_action]' => array('default'),
        'radio:options[default_argument_type]' => array($this->id)
      ),
      '#dependency_count' => 2,
    );
    $this->check_access($form);
  }

  function check_access(&$form) {
    if (!$this->access()) {
      foreach(array('namespace','attribute') as $elem) {
        $form[$elem]['#disabled'] = TRUE;
        $form[$elem]['#value'] = $form[$elem]['#default_value'];
        $form[$elem]['#description'] .= ' <strong>' . t('Note: you do not have permission to modify this. If you change the default argument type, this setting will be lost and you will NOT be able to get it back.') . '</strong>';
      }
    }
  }

  function get_argument() {
    return context_get($this->argument->options['namespace'], $this->argument->options['attribute']);
  }
}