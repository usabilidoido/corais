<?php
// $Id: finder.theme.inc,v 1.1.2.57 2011/02/12 06:55:18 danielb Exp $

/**
 * @file
 * Theme functions for the finder module.
 */

/**
 * Theme the admin table of draggable elements.
 *
 * @param $variables['form']
 *   The form element to theme.
 */
function theme_finder_admin_edit_elements_table($variables) {
  $form = $variables['form'];

  $children = element_children($form);
  if (!empty($children)) {
    $css_id = 'finder-admin-edit-elements-table';
    $css_class = 'finder-admin-edit-elements-table-order';
    drupal_add_tabledrag($css_id, 'order', 'sibling', $css_class);
    $rows = array();
    $headers = array(
      t('Element'),
      //t('Weight'),
      t('Operations'),
    );
    foreach ((array)$children as $key) {
      $value = &$form[$key];
      $value['weight']['#attributes']['class'][] = $css_class;
      $rows[] = array(
        'data' => array(
          drupal_render($value['value']),
          //drupal_render($value['weight']),
          drupal_render($value['ops']),
        ),
        'class' => array('draggable'),
      );
    }
    $output = theme('table', array('header' => $headers, 'rows' => $rows, 'attributes' => array('id' => $css_id)));
  }
  else {
    $output = t('There are no items to display');
  }

  return $output;
}

/**
 * Theme the finder admin links.
 *
 * @param $variables['finder']
 *   The finder object.
 */
function theme_finder_admin_links($variables) {
  $finder = $variables['finder'];
  $output = '';

  $links = array();
  if (is_array($finder->admin_links)) {
    foreach ($finder->admin_links as $path => $title) {
      // don't show this link if the current path starts with $path
      if (strpos($_GET['q'], $path) !== 0) {
        $links[] = l($title, $path);
      }
    }
  }
  if (!empty($links)) {
    $output .= '<div id="finder-admin-links-' . $finder->finder_id . '"
              class="finder-admin-links">';
    $output .= theme('item_list', array('items' => $links, 'attributes' => array('class' => 'links')));
    $output .= '</div>';
  }

  return $output;
}

/**
 * Theme the finder links.
 *
 * @param $variables['finder']
 *   The finder object.
 */
function theme_finder_links($variables) {
  $finder = $variables['finder'];
  $output = '';

  $links = array();
  foreach ($finder->links as $path => $title) {
    // don't show this link if the current path starts with $path
    if (strpos($_GET['q'], $path) !== 0) {
      $links[] = l($title, $path);
    }
  }
  if (!empty($links)) {
    $output .= '<div id="finder-links-' . $finder->finder_id . '"
              class="finder-links">';
    $output .= theme('item_list', array('items' => $links, 'attributes' => array('class' => 'links')));
    $output .= '</div>';
  }

  return $output;
}

/**
 * Theme the finder page wrapper.
 *
 * @param $variables['finder']
 *   The finder object.
 */
function theme_finder_page($variables) {
  $finder = $variables['finder'];
  $output = '<div id="finder-page-' . $finder->finder_id . '" class="finder-page">';
  $output .= finder_view($finder, 'page');
  $output .= '</div>';
  return $output;
}

/**
 * Theme the finder block wrapper.
 *
 * @param $variables['finder']
 *   The finder object.
 */
function theme_finder_block($variables) {
  $finder = $variables['finder'];
  $output = '<div id="finder-block-' . $finder->finder_id . '" class="finder-block">';
  $output .= finder_view($finder, 'block');
  $output .= '</div>';
  return $output;
}

/**
 * Theme the finder.
 *
 * $output_array contains themed output of various items to put on the page,
 * such as the Finder form, and the results output. The implode is a quick
 * way to put all of these together, but you may choose to be more specific
 * about how to do this.
 *
 * @param $variables['finder']
 *   The finder object.
 * @param $variables['display']
 *   The type of display ('page', 'block', or 'ajax').
 * @param $variables['output_array']
 *   A associative array of all the themed 'pieces' to put in the output.
 */
function theme_finder_view($variables) {
  $finder = $variables['finder'];
  $display = $variables['display'];
  $output_array = $variables['output_array'];
  drupal_add_css(drupal_get_path('module', 'finder') . '/finder.css');
  $output = '<div class="finder-view-' . $finder->finder_id . ' finder-view">';
  if ($display == 'ajax') {
    $output .= theme('status_messages');
  }
  $output .= implode('', $output_array);
  $output .= '</div>';
  return $output;
}

/**
 * Theme the finder results wrapper.
 *
 * @param $variables['results']
 *   Themed results list as returned from base handler module.
 * @param $variables['finder']
 *   The finder object.
 * @param $variables['keywords']
 *   An array keyed by finder_element_id, where the values are any
 *   str/num/bool/null or an array of such values to be OR'd together.
 *   This is provided so themers can reformat the keywords and output them back
 *   to the user.
 * @param $variables['pager']
 *   Used to limit results per page.
 * @param $variables['params']
 *   Attributes to pass through to theme_pager().
 * @param $variables['form_state']
 *   The Forms API form state array.  There may be information in here useful
 *   in making decisions about output.
 */
function theme_finder_results($variables) {
  $results = $variables['results'];
  $finder = $variables['finder'];
  $keywords = $variables['keywords'];
  $pager = $variables['pager'];
  $params = $variables['params'];
  $form_state = $variables['form_state'];
  $no_results = $variables['no_results'];

  $output = '';
  //$output .= '<h3 class="finder-results">' . t('Results') . '</h3>';
  $output .= '<div class="finder-results">';
  if ($results) {
    $output .= $results;
    if ($pager) {
      //$output .= theme('pager', NULL, $pager, 0, $params);
    }
  }
  else {
    $output .= $no_results;
  }
  $output .= '</div>';
  return $output;
}