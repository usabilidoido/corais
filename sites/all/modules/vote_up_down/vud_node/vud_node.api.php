<?php

/**
 * Alter the widget type depending on  the $node.
 *
 * @param string $widget
 *   The widget type.
 * @param $node
 *   The node object or NID being viewed. In the case of Views handlers the
 *   full node object may not be available.
 */
function hook_vud_node_widget_alter(&$widget, $node) {
  if ($node->type == 'answer') {
    $widget = 'upanddown';
  }
}
