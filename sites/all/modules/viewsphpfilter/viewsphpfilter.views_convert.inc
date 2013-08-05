<?php

function viewsphpfilter_views_convert($display, $type, &$view, $field) {
  //looks like it won't even go so far as to load... alias issues?
  error_log("field: " . print_r($field, TRUE));
  switch ($type) {
    case 'filter':
      switch ($field['field']) {
        case 'php':
          $view->display['default']->display_options['filters']['handler']['value'] = $field['value'];
          return;
      }
      return;
  }
  return;
}
