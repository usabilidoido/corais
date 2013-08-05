<?php
// $Id: pageroute.page_view.inc,v 1.1.2.9 2009/10/29 12:07:50 sgilits Exp $

/**
 * @file
 * Page view type
 */


include_once(drupal_get_path('module', 'pageroute') .'/pageroute.route.inc');
/**
 * PageroutePageView display a simple node.
 */
class PageroutePageView extends PageroutePage {
  /**
   * Returns the page display for the configured node
   */
  public function getForm(&$form, &$form_state, &$args) {

    if ($this->options['nid'] == 0) {
      $this->options['nid'] = $args['nid'];
    }

    if ($this->options['nid']) {
      $node = node_load($this->options['nid']);

      if ($node->nid && node_access('view', $node)) {
        if (empty($this->title)) {
          drupal_set_title(check_plain($node->title));
        }
        node_tag_new($node->nid);
        $form += array('pageroute-view' => array('#value' => node_view($node, FALSE, TRUE, FALSE)));

        return;
      }
      elseif (db_result(db_query('SELECT nid FROM {node} WHERE nid = %d', $this->options['nid']))) {
        drupal_access_denied();
        pageroute_exit_now();
      }
    }
    drupal_not_found();
    pageroute_exit_now();
  }

  public function getAdminForm($page, &$form) {
    $form['options']['nid'] = array(
      '#type' => 'textfield',
      '#title' => t('Node ID'),
      '#description' => t('Enter the node ID of the node that should be displayed at this page. Enter 0 to use the first argument as node ID like other node page type does.'),
      '#required' => TRUE,
      '#default_value' => isset($page->options['nid']) ? $page->options['nid'] : '',
      '#weight' => 2,
    );
  }

  public static function help() {
    return t('This page type just displays a configurable node. It can also be configured to display the node with the id taken from the first argument. Combined with a node adding or editing form, this enables you to build a page that shows the added or updated node.');
  }

  public static function info() {
    return array('name' => t('Node display'));
  }

  public function getCancelTarget() {
    return PAGEROUTE_FORWARD;
  }

  public function setUp() {
    include_once(drupal_get_path('module', 'node') .'/node.pages.inc');
  }
}
