<?php
// $Id: purl_domain.inc,v 1.1.2.7 2010/07/02 14:23:28 jmiccolis Exp $

/**
 *  Full domain handling.
 */
class purl_domain implements purl_processor {

  public function admin_form(&$form, $id) {
    global $base_url;
    $form['purl_location'] = array(
      '#type' => 'fieldset',
    );
    $form['purl_location']['purl_base_domain'] = array(
      '#type' => 'textfield',
      '#title' => t('Default domain'),
      '#description' => t('Enter the default domain if you are using domain modifiers.'),
      '#required' => FALSE,
      '#default_value' => variable_get('purl_base_domain', $base_url),
      '#element_validate' => array('purl_validate_fqdn'),
    );
  }

  function detect($q) {
    return str_replace('http://','',$_SERVER['HTTP_HOST']);
  }

  public function method() {
    return 'domain';
  }

  public function description() {
    return t('Enter a domain registered for this context, such as "www.example.com".  Do not include http://');
  }

  /**
   * Simply match our 'q' (aka domain) against an allowed value.
   */
  public function parse($valid_values, $q) {
    $parsed = array();
    if (isset($valid_values[$q])) {
      $parsed[$q] = $valid_values[$q];
    }
    return purl_path_elements($this, $parsed);
  }

  public function adjust(&$value, $item, &$q) { return; }

  public function rewrite(&$path, &$options, $element) {
    $options['absolute'] = TRUE;
    if ($base_url = $this->base_url()) {
      if (!_purl_skip($element, $options)) {
        $base = parse_url($base_url);
        $options['base_url'] = "{$base['scheme']}://{$element->value}{$base['path']}";
      }
      else {
        $options['base_url'] = $base_url;
      }
    }
  }

  protected function base_url() {
    global $base_url;
    $base = variable_get('purl_base_domain', $base_url);
    return !empty($base) ? $base : $base_url;
  }
}

