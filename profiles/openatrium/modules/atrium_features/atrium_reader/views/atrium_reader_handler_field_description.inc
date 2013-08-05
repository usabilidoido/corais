<?php
// $Id$

class atrium_reader_handler_field_description extends views_handler_field {
  var $field_alias = 'unknown';
  var $aliases = array();

  function option_definition() {
    $options = parent::option_definition();
    $options['truncate'] = array('default' => 0);
    $options['filter_html'] = array('default' => 0);
    $options['allowed_tags'] = array('default' => '');
    return $options;
  }

  /**
   * Provide options for linking the feed item title.
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    $form['truncate'] = array(
      '#title' => t('Truncate text'),
      '#type' => 'textfield',
      '#description' => t('Enter the number of characters to truncate to below (includes HTML). Leave empty or 0 to show full text.'),
      '#default_value' => !empty($this->options['truncate']) ? $this->options['truncate'] : 0,
    );
    $form['filter_html'] = array(
      '#title' => t('Filter content'),
      '#type' => 'select',
      '#options' => array(
        0 => t('Don\'t filter content'),
        'filter_html' => t('Allow some HTML tags'),
        'strip_html' => t('Strip all HTML tags'),
        'images' => t('Display images only'),
        'twitter' => t('Twitter'),
      ),
      '#description' => t('Check this box if you would like to selectively filter HTML tags (dangerous tags will be stripped regardless of this setting).'),
      '#default_value' => !empty($this->options['filter_html']) ? $this->options['filter_html'] : 0,
    );
    $form['allowed_tags'] = array(
      '#type' => 'textfield',
      '#desciprion' => t('Enter the tags you would like to allow. Leave blank to strip all tags. Example: <strong>a b p div ul</strong>.'),
      '#default_value' => !empty($this->options['allowed_tags']) ? $this->options['allowed_tags'] : '',
      '#process' => array('views_process_dependency'),
      '#dependency' => array(
        'edit-options-filter-html' => array('filter_html'),
      ),
    );
  }

  /**
   * Render the feed item field.
   */
  function render($values) {
    // Note: FeedAPI has already filtered this on its way into the DB. See the feedapi_allowed_html_tags variable.
    // @TODO: make the filter format here more flexible -- some displays may want to strip out images, for example.
    $output = filter_xss_admin($values->{$this->field_alias});

    if (!empty($this->options['truncate'])) {
      $output = truncate_utf8($output, $this->options['truncate'], TRUE, TRUE);
    }

    switch ($this->options['filter_html']) {
      case 'strip_html':
        return strip_tags($output);
      case 'filter_html':
        $tags = explode(' ', $this->options['allowed_tags']);
        $output = filter_xss($output, $tags);
        return _filter_htmlcorrector($output);
      case 'images':
        // Properly entify angles.
        $output = preg_replace('!<([^a-zA-Z/])!', '&lt;\1', $output);

        // Grab the image tags
        $matches = array();
        $output = preg_match('/<img[^>]+?>/', $output, $matches);
        if (!empty($matches)) {
          return implode("", $matches);
        }
        return '';
      case 'twitter':
        // Format twitter #, @ tags
        $output = strip_tags($output);
        $output = $this->twitter_link_filter($output);
        $output = $this->twitter_link_filter($output, '#', 'http://search.twitter.com/search?q=%23');

        // Link urls
        $output = module_invoke('filter', 'filter', 'process', 2, -1, $output);

        $destination = 'http://www.twitter.com/';
        $matches = array('/([a-zA-Z0-9_]{0,15})(: )/');
        $replacements = array('<a href="' . $destination . '${1}">${1}</a>${2}');
        return preg_replace($matches, $replacements, $output, 1);
      default:
        return _filter_htmlcorrector($output);
    }
  }

  /**
  * Taken from the twitter module by walkah.
  * This helper function converts Twitter-style @usernames and #hashtags into
  * actual links.
  */
  function twitter_link_filter($text, $prefix = '@', $destination = 'http://twitter.com/') {
    $matches = array(
      '/\>' . $prefix . '([a-zA-Z0-9_]{0,15})/i',
      '/^' . $prefix . '([a-zA-Z0-9_]{0,15})/i',
      '/(\s+)' . $prefix . '([a-zA-Z0-9_]{0,15})/i',
    );
    $replacements = array(
      '><a href="' . $destination . '${1}">' . $prefix . '${1}</a>',
      '<a href="' . $destination . '${1}">' . $prefix . '${1}</a>',
      '${1}<a href="' . $destination . '${2}">' . $prefix . '${2}</a>',
    );
    return preg_replace($matches, $replacements, $text);
  }
}
