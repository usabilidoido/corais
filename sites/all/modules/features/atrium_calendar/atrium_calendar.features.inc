<?php

/**
 * Implementation of hook_ctools_plugin_api().
 */
function atrium_calendar_ctools_plugin_api() {
  list($module, $api) = func_get_args();
  if ($module == "context" && $api == "context") {
    return array("version" => 3);
  }
  elseif ($module == "feeds" && $api == "feeds_importer_default") {
    return array("version" => 1);
  }
  elseif ($module == "strongarm" && $api == "strongarm") {
    return array("version" => 1);
  }
}

/**
 * Implementation of hook_node_info().
 */
function atrium_calendar_node_info() {
  $items = array(
    'event' => array(
      'name' => t('Reunião'),
      'module' => 'features',
      'description' => t('Un evento del calendario con horas de inicio y fin opcionales.'),
      'has_title' => '1',
      'title_label' => t('Título'),
      'has_body' => '1',
      'body_label' => t('Pauta'),
      'min_word_count' => '0',
      'help' => '',
    ),
    'feed_ical' => array(
      'name' => t('iCal Feed'),
      'module' => 'features',
      'description' => t('A feed used to aggregate events from external iCal calendars.'),
      'has_title' => '1',
      'title_label' => t('Título'),
      'has_body' => '0',
      'body_label' => '',
      'min_word_count' => '0',
      'help' => '',
    ),
    'feed_ical_item' => array(
      'name' => t('iCal Event'),
      'module' => 'features',
      'description' => t('An event aggregated from an external iCal feed.'),
      'has_title' => '1',
      'title_label' => t('Título'),
      'has_body' => '1',
      'body_label' => t('Corpo'),
      'min_word_count' => '0',
      'help' => '',
    ),
  );
  return $items;
}

/**
 * Implementation of hook_views_api().
 */
function atrium_calendar_views_api() {
  return array(
    'api' => '2',
  );
}
