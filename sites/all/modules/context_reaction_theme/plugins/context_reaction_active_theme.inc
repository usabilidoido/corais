<?php
// $Id: context_reaction_active_theme.inc,v 1.1.2.2 2010/10/25 18:47:40 brynbellomy Exp $

/**
 * Expose themes as context reactions.
 */
class context_reaction_active_theme extends context_reaction {

  /**
   * Allow admins to choose the theme to be set.
   */
  function options_form($context) {
    $options = array();
    foreach(list_themes() as $name => $theme) {
      if($theme->status) {
        $options[$name] = $name;
      }
    }
    
    $values = $this->fetch_from_context($context);
    $form = array(
      '#tree' => TRUE,
      '#title' => t('Theme'),
      'theme' => array(
        '#title' => t('Active theme'),
        '#description' => t('Choose a theme to activate when this context is active.'),
        '#type' => 'select',
        '#options' => $options,
        '#default_value' => isset($values['theme']) ? $values['theme'] : '',
      ),
    );
    return $form;
  }

  /**
   * Set the active theme.
   */
  function execute() {
    foreach (context_active_contexts() as $context) {
      if (!empty($context->reactions[$this->plugin]['theme'])) {
        global $custom_theme;
        $custom_theme = $context->reactions[$this->plugin]['theme'];
      }
    }
  }
}