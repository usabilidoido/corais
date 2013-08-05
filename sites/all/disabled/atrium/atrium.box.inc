<?php

/**
 * Implementation of hook_default_box().
 */
function atrium_default_box() {
  $export = array();
  $box = new stdClass;
  $box->disabled = FALSE; /* Edit this to true to make a default box disabled initially */
  $box->api_version = 1;
  $box->delta = 'box-831f99a8';
  $box->plugin_key = 'simple';
  $box->title = 'O que é Design Livre?';
  $box->description = 'Abertura';
  $box->options = array(
    'body' => '<iframe src="http://player.vimeo.com/video/16966516" width="600" height="350" frameborder="0"></iframe><p>',
    'format' => '4',
  );

  $export['box-831f99a8'] = $box;
  return $export;
}
