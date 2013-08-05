<?php

/**
 * @file
 * Theme functions for Views RSS module.
 */

/**
 * Template preprocessor for views-view-views-rss.tpl.php.
 */
function template_preprocess_views_view_views_rss(&$variables) {
  $view = $variables['view'];

  // Prepare <channel> elements.
  $variables['channel'] = '';
  foreach (views_rss_get('channel_elements') as $module => $module_channel_elements) {
    foreach ($module_channel_elements as $element => $definition) {
      list($namespace, $element_name) = views_rss_extract_element_names($element, 'core');
      if (isset($view->style_plugin->options['channel'][$namespace][$module][$element_name])) {
        $value = $view->style_plugin->options['channel'][$namespace][$module][$element_name];
      }
      elseif (isset($definition['default_value'])) {
        $value = $definition['default_value'];
      }
      else {
        $value = NULL;
      }
      // Preprocess element value.
      if (isset($definition['preprocess functions']) && is_array($definition['preprocess functions'])) {
        foreach ($definition['preprocess functions'] as $preprocess_function) {
          if (function_exists($preprocess_function)) {
            $item_variables = array(
              'element' => $element,
              'value' => $value,
              'item' => $view->style_plugin->options['channel'],
              'view' => $view,
            );
            $preprocess_function($item_variables);
            $value = $item_variables['value'];
          }
        }
      }
      // If there is no value and no arguments (in case of self-closing elements)
      // already set for the element at this stage, it is not going to be set
      // at any point further, so the element should not be added to the feed.
      if (empty($value)) continue;
      // Make sure that $value is an array (in cases when Views' rendering,
      // CCK formatter or preprocessor returns simple value).
      if (!is_array($value)) {
        $value = array('value' => $value);
      }
      // There are cases when an element is really a set of elements
      // (for example core <category> element). So let's just convert
      // each element into array of elements, which in most cases
      // would containt just a single element.
      if (empty($value['elements'])) {
        $value = array('elements' => array($value));
      }
      // Theme channel element value.
      // If theme definition was provided by hook_views_rss_channel_elements_alter()
      // implementation, but theme value was set to FALSE, it means that such
      // element should not be themed here.
      $theme_function = !empty($definition['theme']) ? $definition['theme'] : 'views_view_views_rss_field';
      foreach ($value['elements'] as $single_element) {
        $item_variables = array_merge(array(
          'element' => $element,
          'item' => $view->style_plugin->options['channel'],
          'view' => $view,
        ), $single_element);
        $variables['channel'] .= views_rss_theme7($theme_function, $item_variables);
      }
    }
  }

  // Prepare <item> elements.
  $elements = array();
  $rows = '';
  $item_elements = views_rss_get('item_elements');
  $items = $view->style_plugin->map_rows($variables['rows']);
  foreach ($items as $item_key => $item) {
    $row = '';
    // Preprocess whole item array before processing and theming separate elements.
    foreach (module_implements('views_rss_preprocess_item') as $module) {
      $preprocess_function = $module . '_views_rss_preprocess_item';
      $item_variables = array(
        'item' => $item,
        'view' => $view,
      );
      // Add raw row if generated based on raw item values provided by field formatter.
      if (!empty($view->views_rss['raw_items'][$item_key])) {
        $item_variables['raw'] = $view->views_rss['raw_items'][$item_key];
      }
      $preprocess_function($item_variables);
      $item = $item_variables['item'];
    }
    // Process each element separately.
    foreach ($item as $module => $module_item_elements) {
      foreach ($module_item_elements as $element => $value) {
        // Preprocess element initial value if required.
        if (isset($item_elements[$module][$element]['preprocess functions']) && is_array($item_elements[$module][$element]['preprocess functions'])) {
          foreach ($item_elements[$module][$element]['preprocess functions'] as $preprocess_function) {
            if (function_exists($preprocess_function)) {
              $item_variables = array(
                'element' => $element,
                'value' => $value,
                'item' => $item,
                'view' => $view,
              );
              // Add raw item if provided by field formatter.
              if (!empty($view->views_rss['raw_items'][$item_key][$module][$element])) {
                $item_variables['raw'] = $view->views_rss['raw_items'][$item_key][$module][$element];
              }
              $preprocess_function($item_variables);
              // $element and $item values could have been updated in
              // the preprocess function, so let's re-assing them back.
              $element = $item_variables['element'];
              $value = $item_variables['value'];
            }
          }
        }
        // If no preprocess function was defined, and we have received
        // #markup value (fully formatted feed element) from the formatter,
        // it should be passed to field template to be displayed without
        // any further modifications.
        elseif (!empty($view->views_rss['raw_items'][$item_key][$module][$element]) && is_array($view->views_rss['raw_items'][$item_key][$module][$element])) {
          $markups = array();
          foreach ($view->views_rss['raw_items'][$item_key][$module][$element] as $raw_item) {
            if (!empty($raw_item['rendered']['#markup'])) {
              $markups[] = $raw_item['rendered']['#markup'];
            }
          }
          if (!empty($markups)) {
            $value = array(
              'markup' => implode('', $markups),
            );
          }
        }
        // If there is no value and no arguments (in case of self-closing elements)
        // already set for the element at this stage, it is not going to be set
        // at any point further, so the element should not be added to the feed.
        if (empty($value)) continue;
        // Make sure that $value is an array (in cases when Views' rendering,
        // CCK formatter or preprocessor returns simple value).
        if (!is_array($value)) {
          $value = array('value' => $value);
        }
        // There are cases when an element is really a set of elements
        // (for example core <category> element). So let's just convert
        // each element into array of elements, which in most cases
        // would containt just a single element.
        if (empty($value['elements'])) {
          $value = array('elements' => array($value));
        }
        // Theme item element value(s).
        $theme_function = !empty($item_elements[$module][$element]['theme']) ? $item_elements[$module][$element]['theme'] : 'views_view_views_rss_field';
        foreach ($value['elements'] as $single_element) {
          $item_variables = array_merge(array(
            'element' => $element,
            'item'    => $item,
            'view'    => $view,
          ), $single_element);
          $row .= views_rss_theme7($theme_function, $item_variables);
        }
      }
    }
    $rows .= views_rss_theme7('views_view_views_rss_fields', array('row' => $row, 'view' => $view));
  }
  $variables['rows'] = $rows;

  // Prepare namespaces.
  $namespaces = array('base' => 'xml:base="' . url('<front>', array('absolute' => TRUE)) . '"');
  foreach (views_rss_get('namespaces') as $module => $module_namespaces) {
    foreach ($module_namespaces as $namespace => $definition) {
      // Check if definition provided through modules hooks
      // should be overwritten by module configuration.
      if (
        isset($view->style_plugin->options['namespaces'][$module][$namespace])
        && !empty($view->style_plugin->options['namespaces'][$module][$namespace])
      ) {
        $definition['uri'] = $view->style_plugin->options['namespaces'][$module][$namespace];
      }
      if (isset($definition['uri']) && !empty($definition['uri'])) {
        // Namespaces with prefix, for example: xml:base="" or xmlns:dc=""
        if (isset($definition['prefix']) && !empty($definition['prefix'])) {
          $namespaces[$namespace] = "{$definition['prefix']}:$namespace=\"{$definition['uri']}\"";
        }
        // Namespaces without prefix, for example: content="" or foaf=""
        else {
          $namespaces[$namespace] = "$namespace=\"{$definition['uri']}\"";
        }
      }
    }
  }
  $variables['namespaces'] = implode(' ', array_unique($namespaces));

  // Set XML header.
  drupal_set_header('Content-Type: application/rss+xml; charset=utf-8');
}

/**
 * Template preprocessor for views-view-views-rss-fields.tpl.php.
 */
function template_preprocess_views_view_views_rss_fields(&$variables) {
  // Theme hook suggestions. Examples assume that:
  // - display plugin = "feed"
  // - current display = "feed_1"
  // - view name = "myfeed"
  $base = 'views-view-views-rss-fields--';
  // views-view-views-rss-fields--feed.tpl.php
  $variables['template_files'][] = $base . $variables['view']->display[$variables['view']->current_display]->display_plugin;
  // views-view-views-rss-fields--feed-1.tpl.php
  $variables['template_files'][] = $base . $variables['view']->current_display;
  // views-view-views-rss-fields--myfeed.tpl.php
  $variables['template_files'][] = $base . $variables['view']->name;
  // views-view-views-rss-fields--myfeed--feed.tpl.php
  $variables['template_files'][] = $base . $variables['view']->name . '--' . $variables['view']->display[$variables['view']->current_display]->display_plugin;
  // views-view-views-rss-fields--myfeed--feed-1.tpl.php
  $variables['template_files'][] = $base . $variables['view']->name . '--' . $variables['view']->current_display;
}

/**
 * Template preprocessor for views-view-views-rss-field.tpl.php.
 */
function template_preprocess_views_view_views_rss_field(&$variables) {
  $element = $variables['element'];
  // Our CCK formatters could return value as a simple string - then we just
  // use it, or as serialized array (in cases when not only value needs to be
  // returned, by also additional arguments, subelements, or whatever else).
  // In such cases we need to unserialize obtained array and merge it back
  // into $variables array here.
  if (strpos($variables['value'], 'serialized:') !== FALSE) {
    $formatter_variables = unserialize(str_replace('serialized:', '', $variables['value']));
    $variables['value'] = NULL;
    $variables = array_merge($variables, $formatter_variables);
  }
  // Theme all subelements first (if any) and pass them to the template ready to use.
  if (!empty($variables['subelements'])) {
    foreach ($variables['subelements'] as $key => $subelement) {
      $subelement['view'] = $variables['view'];
      $subelement['item'] = $variables['item'];
      $variables['subelements'][$key] = views_rss_theme7('views_view_views_rss_field', $subelement);
    }
  }
  // Theme hook suggestions. Examples assume that:
  // - element = "title"
  // - display plugin = "feed"
  // - current display = "feed_1"
  // - view name = "myfeed"
  $base = 'views-view-views-rss-field--';
  // views-view-views-rss-field--views-view-field--title.tpl.php
  $variables['template_files'][] = $base . $element;
  // views-view-views-rss-field--views-view-field--myfeed.tpl.php
  $variables['template_files'][] = $base . $variables['view']->name;
  // views-view-views-rss-field--views-view-field--myfeed--title.tpl.php
  $variables['template_files'][] = $base . $variables['view']->name . '--' . $element;
  // views-view-views-rss-field--views-view-field--feed.tpl.php
  $variables['template_files'][] = $base . $variables['view']->display[$variables['view']->current_display]->display_plugin;
  // views-view-views-rss-field--views-view-field--feed--title.tpl.php
  $variables['template_files'][] = $base . $variables['view']->display[$variables['view']->current_display]->display_plugin . '--' . $element;
  // views-view-views-rss-field--views-view-field--myfeed--feed.tpl.php
  $variables['template_files'][] = $base . $variables['view']->name;
  // views-view-views-rss-field--views-view-field--myfeed--feed.tpl.php
  $variables['template_files'][] = $base . $variables['view']->name . '--' . $variables['view']->display[$variables['view']->current_display]->display_plugin;
  // views-view-views-rss-field--views-view-field--feed-1.tpl.php
  $variables['template_files'][] = $base . $variables['view']->current_display;
  // views-view-views-rss-field--views-view-field--feed-1--title.tpl.php
  $variables['template_files'][] = $base . $variables['view']->current_display . '--' . $element;
  // views-view-views-rss-field--views-view-field--myfeed--feed-1.tpl.php
  $variables['template_files'][] = $base . $variables['view']->name . '--' . $variables['view']->current_display;
  // views-view-views-rss-field--views-view-field--myfeed--feed-1--title.tpl.php
  $variables['template_files'][] = $base . $variables['view']->name . '--' . $variables['view']->current_display . '--' . $element;
}
