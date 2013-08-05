<?php

/**
 * @file
 * Field handler to provide additional control for the email field.
 */

class views_rss_handler_field_user_mail extends views_handler_field_user_mail {

  function option_definition() {
    $options = parent::option_definition();
    $options['rss_format'] = array('default' => 'default');
    return $options;
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    $form['rss_format'] = array(
      '#title' => t('Display type'),
      '#type' => 'select',
      '#options' => array(
        'default' => t('Generic email'),
        'with_name' => t('RSS <author> element (with user name)'),
      ),
      '#default_value' => isset($this->options['rss_format']) ? $this->options['rss_format'] : 'default',
      '#weight' => 1,
    );
    // Move "Link this field" radios below just added "Display type",
    // to be hidden when "RSS <author> element" is selected.
    $form['link_to_user']['#weight'] = 2;
    $form['link_to_user']['#process'] = array('expand_radios', 'views_process_dependency');
    $form['link_to_user']['#dependency'] = array('edit-options-rss-format' => array('default'));
    $form['link_to_user']['#prefix'] = '<div id="edit-options-link-to-user-wrapper" style="padding-left:0">';
    $form['link_to_user']['#suffix'] = '</div>';
  }

  function render($values) {
    if ($this->options['rss_format'] == 'with_name') {
      // Try to load full user object based on email address.
      $value = $values->{$this->field_alias};
      if ($account = user_load(array('mail' => $value))) {
        $value = $account->mail . ' (' . $account->name . ')';
      }
      // XML element array in format_xml_elements() format.
      $rss_element = array(
        'key' => 'author',
        'value' => $value,
      );
      // Make sure that element value is XML-safe.
      views_rss_htmlspecialchars($rss_element);
      // Prepare and store raw-rendered values in view results,
      // the same way as values from standard field formatters are added.
      if (isset($this->view->row_index)) {
        $rendered = array(
          '#item' => $values->{$this->field_alias},
          '#markup' => format_xml_elements(array($rss_element)),
          '#rss_element' => $rss_element,
          '#settings' => $this->options,
        );
        $row_index = $this->view->row_index;
        $field_name = 'field_' . $this->field;
        $this->view->result[$row_index]->{$field_name}[0] = array(
          'rendered' => $rendered,
          'raw' => $values->{$this->field_alias},
        );
      }
      // Return formatted XML element.
      return format_xml_elements(array($rss_element));
    }
    // Standard Views render().
    return parent::render($values);
  }

}
