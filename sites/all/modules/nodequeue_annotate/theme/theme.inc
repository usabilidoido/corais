<?php

/**
 * @file theme.inc
 * Base theme functions for nodequeue_annotate.
 */


/**
 * Returns the standard output of theme_links.
 *
 * Themers can override this function, which will provide them with a way to
 * manipulate these links as an array without the need to override theme_links,
 * which would affect all links.
 */
function theme_nodequeue_annotate_views_links($links, $attributes = array()) {
  
  // Add the nodequeue_annotate classes to the $attributes array
  if (isset($attributes['class'])) {
    $attributes['class'] .= ' links nodequeue-annotate-views-links';
  }
  else {
    $attributes['class'] = 'links nodequeue-annotate-views-links';
  }
  
  return theme('links', $links, $attributes);
}
