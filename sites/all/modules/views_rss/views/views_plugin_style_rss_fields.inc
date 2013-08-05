<?php
// $Id: views_plugin_style_rss_fields.inc,v 1.1.2.9 2010/08/25 20:04:30 davidg Exp $

/**
 * Extend the view_plugin_style class to provide an RSS view style.
 */
class views_plugin_style_rss_fields extends views_plugin_style {
  /**
   * Attach this view to another display as a feed.
   *
   * Provide basic functionality for all export style views like attaching a
   * feed image link.
   */
  function attach_to($display_id, $path, $title) {
    $display = $this->view->display[$display_id]->handler;
    $url_options = array();
    $input = $this->view->get_exposed_input();
    if ($input) {
      $url_options['query'] = $input;
    }

    $url = url($this->view->get_url(NULL, $path), $url_options);
    if ($display->has_path()) {
      if (empty($this->preview)) {
        drupal_add_feed($url, $title);
      }
    }
    else {
      if (empty($this->view->feed_icon)) {
        $this->view->feed_icon = '';
      }

      $this->view->feed_icon .= theme('feed_icon', $url, $title);
      drupal_add_link(array(
        'rel' => 'alternate',
        'type' => 'application/rss+xml',
        'title' => $title,
        'href' => $url
      ));
    }
  }
  
  
  function option_definition() {
    $options = parent::option_definition();

    $options['description'] = array(
      'contains' => array(
        'feed_description' => array('default' => ''),
      ),
    );
    
    $field_options = array();
    foreach ($this->xml_fields() as $k => $option) {
      $field_options[$k] = array('default' => '');
    }
    
    $options['fields'] = array(
      'contains' => $field_options,
    );
    
    $options['georss'] = array(
      'contains' => array(
        'lat' => array('default' => ''),
        'lon' => array('default' => ''),
        'featureName' => array('default' => ''),
      ),
    );
    
    return $options;
  }

  /**
   * Provide a form for setting options.
   *
   * @param array $form
   * @param array $form_state
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    $options = parent::option_definition();

    $handlers = $this->display->handler->get_handlers('field');
    if (empty($handlers)) {
      $form['error_markup'] = array(
        '#value' => t('You need at least one field before you can configure your field settings'),
        '#prefix' => '<div class="error form-item description">',
        '#suffix' => '</div>',
      );
    }
    else {
      // Feed Description
      $form['description'] = array(
        '#type' => 'fieldset',
        '#title' => t('Feed Description'),
        '#collapsible' => TRUE,
        '#collapsed' => TRUE,
        '#weight' => -5,
      );
      $form['description']['feed_description'] = array(
        '#type' => 'textarea',
        '#default_value' => $this->options['description']['feed_description'],
        '#description' => t('Description for this feed.  If left blank, the default site mission will be used'),
      );
      // Field Chooser
      $field_names = array('' => '--');
      foreach ($handlers as $field => $handler) {
        if ($label = $handler->label()) {
          $field_names[$field] = $label;
        }
        else {
          $field_names[$field] = $handler->ui_name();
        }
      }
      $form['fields'] = array(
        '#type' => 'fieldset',
        '#title' => 'Field usage',
        '#description' => t('Select the fields that relevant data for each element of the feed'),
        '#collapsible' => TRUE,
        '#collapsed' => TRUE,
        '#weight' => 0,
      );
      foreach ($this->xml_fields() as $k => $option) {
        $form['fields'][$k] = array(
          '#type' => 'select',
          '#title' => $option['title'],
          '#description' => isset($option['description']) ? $option['description'] : '',
          '#options' => $field_names,
          '#default_value' => $this->options['fields'][$k],
        );
      }
      // GeoRSS
      $form['georss'] = array(
        '#type' => 'fieldset',
        '#title' => t('GeoRSS'),
        '#description' => t('Select fields that provide the latitude and longitude values'),
        '#collapsible' => TRUE,
        '#collapsed' => TRUE,
        '#weight' => 5,
      );
      $form['georss']['lat'] = array(
        '#type' => 'select',
        '#title' => t('Latitude'),
        '#options' => $field_names,
        '#default_value' => $this->options['georss']['lat'],
      );
      $form['georss']['lon'] = array(
        '#type' => 'select',
        '#title' => t('Longitude'),
        '#options' => $field_names,
        '#default_value' => $this->options['georss']['lon'],
      );
      $form['georss']['featureName'] = array(
        '#type' => 'select',
        '#title' => t('Feature Name'),
        '#options' => $field_names,
        '#default_value' => $this->options['georss']['featureName'],
      );
    }
  }

  /**
   * Map views row result to an RSS item.
   */
  function map_rows($rows) {
    // Fields must be rendered in order as of Views 2.3, so we will pre-render everything.
    $renders = array();
    $keys = array_keys($this->view->field);

    foreach ($rows as $count => $row) {
      foreach ($keys as $id) {
        $renders[$count][$id] = $this->view->field[$id]->theme($row);
      }
    }

    $items = array();
    foreach ($renders as $id => $row) {
      $item = array();
      foreach (array_merge($this->options['fields'], $this->options['georss']) as $key => $val) {
        if (!isset($this->view->field[$val])) continue;
        $item[$key] = $row[$val];
      }
      $items[] = $item;
    }
    return $items;
  }

  /**
   * An array of available XML fields to map to.
   */
  function xml_fields() {
    return array(
      'title' => array(
        'title' => t('Title'),
        'description' => t('RSS 2.0 title element'),
      ),
      'description' => array(
        'title' => t('Description'),
        'description' => t('RSS 2.0 description element'),
      ),
      'link' => array(
        'title' => t('Link'),
        'description' => t('RSS 2.0 link element'),
      ),
      'author' => array(
        'title' => t('Author e-mail'),
        'description' => t('RSS 2.0 author element'),
      ),
      'dc:creator' => array(
        'title' => t('Author name'),
        'description' => t('Dublin core creator element'),
      ),
      'category' => array(
        'title' => t('category'),
        'description' => t('RSS 2.0 category element'),
      ),
      'guid' => array(
        'title' => t('guid'),
        'description' => t('RSS 2.0 guid element'),
      ),
      'pubDate' => array(
        'title' => t('pubDate'),
        'description' => t('RSS 2.0 pubDate element'),
      ),
    );
  }

  /**
   * Return an array of all known XML namespaces.
   */
  function xml_namespaces() {
    return array(
      'dc' => 'http://purl.org/dc/elements/1.1/',
      'georss' => 'http://www.georss.org/georss',
    );
  }

  /**
   * Convenience method: return an XML Namespace for a given element.
   *
   * @param $elem
   *   An XML element like 'title' or 'dc:creator'
   *
   * @return
   *   An array where with a local and a global namespace identifier. Returns
   *   FALSE if no corresponding namespace could be found.
   */
  function xml_namespace($elem) {
    $e = explode(':', $elem);
    if (is_array($e)) {
      $local = array_shift($e);
      $namespaces = $this->xml_namespaces($local);
      if (isset($namespaces[$local])) {
        return array(
          'local' => $local,
          'namespace' => $namespaces[$local],
        );
      }
    }
    return FALSE;
  }
}