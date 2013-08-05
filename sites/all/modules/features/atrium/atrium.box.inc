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
  $box->title = 'Apresentação';
  $box->description = 'Abertura';
  $box->options = FALSE;
  $export['box-831f99a8'] = $box;

  $box = new stdClass;
  $box->disabled = FALSE; /* Edit this to true to make a default box disabled initially */
  $box->api_version = 1;
  $box->delta = 'box-d3d6afda';
  $box->plugin_key = 'simple';
  $box->title = 'Porque se chama Corais?';
  $box->description = 'Nome Corais';
  $box->options = FALSE;
  $export['box-d3d6afda'] = $box;

  return $export;
}
