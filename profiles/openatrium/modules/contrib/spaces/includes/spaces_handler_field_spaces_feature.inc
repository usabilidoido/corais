<?php

/**
 * Field handler to translate a node type into its spaces feature.
 */
class spaces_handler_field_spaces_feature extends views_handler_field_node_type {
  function render($values) {
    static $map;
    static $features;
    if (!isset($map)) {
      $map = features_get_component_map('node');
      $features = spaces_features();
    }
    $output = '';
    if (!empty($map[$values->{$this->field_alias}])) {
      $feature = reset($map[$values->{$this->field_alias}]);
      $label = $this->options['machine_name'] == TRUE ? $features[$feature]->name : $features[$feature]->info['name'];
      $output = "<span class='spaces-feature feature-{$feature}'>{$label}</span>";
    }
    return $output;
  }
}
