<?php
// $Id: views_rss_views_fields.theme.inc,v 1.1.2.12 2010/08/26 20:38:04 davidg Exp $

/**
 * Template preprocessor for views-view-views-rss-fields.tpl.php.
 */
function template_preprocess_views_view_views_rss_fields(&$vars) {
  $view = $vars['view'];

  // Set basic info - title, description - about the feed
  if ($view->display_handler->get_option('sitename_title')) {
    $title = variable_get('site_name', 'Drupal');
    if ($slogan = variable_get('site_slogan', '')) {
      $title .= ' - ' . $slogan;
    }
  }
  else {
    $title = $view->get_title();
  }
  $vars['viewtitle'] = check_plain($title);

  if ($view->style_options['description']['feed_description']) {
    $description = $view->style_options['description']['feed_description'];
  }
  else {
    $description = variable_get('site_mission', '');
  }
  $vars['description'] = theme('views_rss_feed_description', $description, $view);

  // Base URL for link tag
  global $base_url;
  $vars['link'] = $base_url;

  // Grab the rows, push to field mapping function, gather namespaces.
  $elements = $namespaces = array();
  $rows = '';
  $items = $view->style_plugin->map_rows($vars['rows']);
  foreach($items as $item) {
    // Special handling for GeoRSS.
    if (is_numeric($item['lat']) && is_numeric($item['lon'])) {
      $item['georss:point'] = $item['lat'] .' '.  $item['lon'];
    }
    if(isset($item['featureName'])) {
      $item['georss:featureName'] = $item['featureName'];
    }
    unset($item['lat']);
    unset($item['lon']);
    unset($item['featureName']);
    $rows .= theme('views_rss_fields_item', $item);
    foreach ($item as $k => $v) {
      $elements[$k] = $k;
    }
  }
  $vars['rows'] = $rows;
  foreach ($elements as $e) {
    if ($namespace = $view->style_plugin->xml_namespace($e)) {
      $namespaces[] = "xmlns:{$namespace['local']}=\"{$namespace['namespace']}\"";
    }
  }
  $vars['namespaces'] = implode(' ', array_unique($namespaces));

  // Set Headers
  drupal_set_header('Content-Type: application/rss+xml; charset=utf-8');
}

/**
 * Template preprocessor for views-rss-fields-item.tpl.php.
 */
function template_preprocess_views_rss_fields_item(&$vars) {
  $item = $vars['item'];

  // Loop through key=>value pairs
  $row = '';
  foreach ($item as $key => $value) {
    if ($value) {
      $row .= "<$key>". check_plain(htmlspecialchars_decode($value)) ."</$key>\n";
    }
  }

  $vars['row'] = $row;
}

/**
 * Theme function for feed icon.
 */
function theme_views_rss_feed_icon($url, $title, $icon) {
  if ($image = theme('image', $icon, t('Download RSS Feed'), $title)) {
    return '<a href="'. check_url($url) .'" class="feed-icon">'. $image .'</a>';
  }
}

/**
 * Theme function for feed description.
 */
function theme_views_rss_feed_description($description, $view) {
  return $description;
}
