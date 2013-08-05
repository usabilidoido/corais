<?php
// $Id: views_plugin_display_feed.inc,v 1.7 2009/09/15 22:20:01 merlinofchaos Exp $
/**
 * @file
 * Contains the feed display plugin.
 */

/**
 * This plugin is somewhat like a page, because it displays as the main item,
 * and somewhat like an attachment, because it is reached through a link at the bottom of other views
 */
class views_plugin_display_print_table extends views_plugin_display_page {
  function uses_breadcrumb() { 
    return FALSE;
  }

  /**
   * We skip the normal drupal page theming and output the themed view directly to the page then exit.
   * So we have to provide our own page head headers
   */
  function execute() {
    $output = $this->view->render();
    if (empty($output)) {
      return drupal_not_found();
    }
?><!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns = "http://www.w3.org/1999/xhtml" lang = "fr" xml:lang = "fr">   
  <head>
    <meta http-equiv = "Content-Type" content = "text/html; charset=utf-8" />
    <meta http-equiv = "Content-Type" content = "text/html; charset=utf-8" />
    <meta name="robots" content = "noindex, nofollow, noarchive" />
    <title><?php print variable_get('site_name', ''); ?></title>
    <style>
      img{height:5em;float:left;margin-right:1em}
      thead{background-color:#dddddd;}
      th{text-align: left; font-size:1.3em;}
      caption {text-align: left; font-size:2em;}
      table{width:100%;margin-bottom:1em; font-family: arial; font-size: 8pt; page-break-inside:auto;}
      td{border-bottom:thin solid #dddddd;}

    </style>
  </head>
  <body onload = "window.print();">
    <img src = "<?php print theme_get_setting('logo'); ?>" alt = "logo" />
      <h1><?php print $this->view->get_title(); ?></h1>
    <h3><?php print t('Printed on @date', array('@date' => format_date(time(), 'small'))); ?> </h3>
    <?php print $output; ?>
  </body>
</html>
<?php
    exit;
  }

  function preview() {
    if (!empty($this->live_preview)) {
      return '<pre>' . check_plain($this->view->render()) . '</pre>';
    }
    return $this->view->render();
  }

  /**
   * Instead of going through the standard views_view.tpl.php, delegate this
   * to the style handler.
   */
  function render() {
    return $this->view->style_plugin->render($this->view->result);
  }


  function option_definition() {
    $options = parent::option_definition();

    $options['attachment_position'] = array('default' => 'after');
    $options['inherit_arguments'] = array('default' => TRUE);
    $options['displays'] = array('default' => array());
    return $options;
  }

  function options_summary(&$categories, &$options) {
    // It is very important to call the parent function here:
    parent::options_summary($categories, $options);

    // Since we're childing off the 'page' type, we'll still *call* our
    // category 'page' but let's override it so it says feed settings.
    $categories['page'] = array(
      'title' => t('Print-page settings'),
    );
    // I don't think we want to give feeds menus directly.
    unset($options['menu']);

    $displays = array_filter($this->get_option('displays'));
    if (count($displays) > 1) {
      $attach_to = t('Multiple displays');
    }
    else if (count($displays) == 1) {
      $display = array_shift($displays);
      if (!empty($this->view->display[$display])) {
        $attach_to = check_plain($this->view->display[$display]->display_title);
      }
    }

    if (!isset($attach_to)) {
      $attach_to = t('None');
    }

    $options['displays'] = array(
      'category' => 'page',
      'title' => t('Attach to'),
      'value' => $attach_to,
    );
    $options['attachment_position'] = array(
      'category' => 'page',
      'title' => t('Position'),
      'value' => $this->attachment_positions($this->get_option('attachment_position')),
    );
  }

  /**
   * Provide the default form for setting options.
   */
  function options_form(&$form, &$form_state) {
    // It is very important to call the parent function here.
    parent::options_form($form, $form_state);
    switch ($form_state['section']) {
      case 'displays':
        $form['#title'] .= t('Attach to');
        $displays = array();
        foreach ($this->view->display as $display_id => $display) {
          if (!empty($display->handler) && $display->handler->accept_attachments()) {
            $displays[$display_id] = $display->display_title;
          }
        }
        $form['displays'] = array(
          '#type' => 'checkboxes',
          '#description' => t('The feed icon will be available only to the selected displays.'),
          '#options' => $displays,
          '#default_value' => $this->get_option('displays'),
        );
        break;

      case 'attachment_position':
        $form['#title'] .= t('Position');
        $form['attachment_position'] = array(
          '#type' => 'radios',
          '#description' => t('Attach before or after the parent display?'),
          '#options' => $this->attachment_positions(),
          '#default_value' => $this->get_option('attachment_position'),
        );
        break;
      case 'path':
        $form['path']['#description'] = t('This view will be displayed by visiting this path on your site. It is recommended that the path be something like "path/%/%/feed" or "path/%/%/rss.xml", putting one % in the path for each argument you have defined in the view.');
    }
  }

  /**
   * Perform any necessary changes to the form values prior to storage.
   * There is no need for this function to actually store the data.
   */
  function options_submit(&$form, &$form_state) {
    // It is very important to call the parent function here:
    parent::options_submit($form, $form_state);
    switch ($form_state['section']) {
      case 'displays':
      case 'attachment_position':
        $this->set_option($form_state['section'], $form_state['values'][$form_state['section']]);
        break;
    }
  }

  function attachment_positions($position = NULL) {
    $positions = array(
      'before' => t('Before'),
      'after' => t('After'),
      'both' => t('Both'),
    );

    if ($position) {
      return $positions[$position];
    }

    return $positions;
  }

  /**
   * Attach to another view.
   */
  function attach_to($display_id) {
    $displays = $this->get_option('displays');
    if (empty($displays[$display_id])) {
      return;
    }
    if ($this->get_plugin()) {
      $path = array($this->get_path());
      //tricky to get the args out of the view object
      if ($view = array_pop($this->get_handlers('argument'))->view) {
        $path =  array_merge($path, $view->args);
      }
      //this will show the print icon eventually
      $img_icon = theme('image', drupal_get_path('module', 'cforge_custom') . '/print_icon.gif');
      $attachment = l($img_icon, implode('/', $path), array('html' => TRUE));
      $attachment .= l(t('Printer-friendly version'), implode('/', $path));
    }

    switch ($this->get_option('attachment_position')) {
      case 'before':
        $this->view->attachment_before .= $attachment;
        break;
      case 'after':
        $this->view->attachment_after .= $attachment;
        break;
      case 'both':
        $this->view->attachment_before .= $attachment;
        $this->view->attachment_after .= $attachment;
        break;
    }
  }
  
  function uses_link_display() {
    return TRUE;
  }
}
