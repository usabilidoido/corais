<?php

/**
 * Implementation of hook_ctools_plugin_api().
 */
function atrium_book_ctools_plugin_api() {
  list($module, $api) = func_get_args();
  if ($module == "context" && $api == "context") {
    return array("version" => 3);
  }
  elseif ($module == "strongarm" && $api == "strongarm") {
    return array("version" => 1);
  }
}

/**
 * Implementation of hook_flag_default_flags().
 */
function atrium_book_flag_default_flags() {
  $flags = array();
  // Exported flag: "Expert".
  $flags['expert'] = array(
    'content_type' => 'node',
    'title' => 'Expert',
    'global' => '0',
    'types' => array(
      '0' => 'book',
    ),
    'flag_short' => 'I\'m an expert',
    'flag_long' => '',
    'flag_message' => 'You have added this to your profile.',
    'unflag_short' => 'I\'m an expert',
    'unflag_long' => '',
    'unflag_message' => 'You have removed this from your profile.',
    'unflag_denied_text' => '',
    'link_type' => 'toggle',
    'roles' => array(
      'flag' => array(
        '0' => 1,
        '1' => 2,
      ),
      'unflag' => array(
        '0' => 1,
        '1' => 2,
      ),
    ),
    'weight' => 0,
    'anon_display' => '',
    'anon_message' => '',
    'show_on_page' => 0,
    'show_on_teaser' => 0,
    'show_on_form' => 0,
    'access_author' => '',
    'i18n' => '0',
    'module' => 'atrium_book',
    'locked' => array(
      '0' => 'name',
    ),
    'api_version' => 2,
  );
  // Exported flag: "Estudando".
  $flags['studying'] = array(
    'content_type' => 'node',
    'title' => 'Studying',
    'global' => '0',
    'types' => array(
      '0' => 'book',
    ),
    'flag_short' => 'I\'m studying',
    'flag_long' => '',
    'flag_message' => 'You have added this to your profile.',
    'unflag_short' => 'I\'m studying',
    'unflag_long' => '',
    'unflag_message' => 'You have removed this from your profile.',
    'unflag_denied_text' => '',
    'link_type' => 'toggle',
    'roles' => array(
      'flag' => array(
        '0' => 1,
        '1' => 2,
      ),
      'unflag' => array(
        '0' => 1,
        '1' => 2,
      ),
    ),
    'weight' => 0,
    'anon_display' => '',
    'anon_message' => '',
    'show_on_page' => 0,
    'show_on_teaser' => 0,
    'show_on_form' => 0,
    'access_author' => '',
    'i18n' => '0',
    'module' => 'atrium_book',
    'locked' => array(
      '0' => 'name',
    ),
    'api_version' => 2,
  );
  // Exported flag: "Já usei".
  $flags['used'] = array(
    'content_type' => 'node',
    'title' => 'I used this',
    'global' => '0',
    'types' => array(
      '0' => 'book',
    ),
    'flag_short' => 'I used this',
    'flag_long' => '',
    'flag_message' => 'You have added this to your profile.',
    'unflag_short' => 'I used this',
    'unflag_long' => '',
    'unflag_message' => 'You have removed this from your profile.',
    'unflag_denied_text' => '',
    'link_type' => 'toggle',
    'roles' => array(
      'flag' => array(
        '0' => 1,
        '1' => 2,
      ),
      'unflag' => array(
        '0' => 1,
        '1' => 2,
      ),
    ),
    'weight' => 0,
    'anon_display' => 0,
    'anon_message' => '',
    'show_on_page' => 0,
    'show_on_teaser' => 0,
    'show_on_form' => 0,
    'access_author' => '',
    'i18n' => '0',
    'module' => 'atrium_book',
    'locked' => array(
      '0' => 'name',
    ),
    'api_version' => 2,
  );
  return $flags;

}

/**
 * Implementation of hook_node_info().
 */
function atrium_book_node_info() {
  $items = array(
    'book' => array(
      'name' => t('Conhecimento'),
      'module' => 'features',
      'description' => t('Uma página que descreve um conhecimento específico sobre Design. '),
      'has_title' => '1',
      'title_label' => t('Título'),
      'has_body' => '1',
      'body_label' => t('Corpo'),
      'min_word_count' => '0',
      'help' => '',
    ),
    'service_provider' => array(
      'name' => t('Prestador de Serviços'),
      'module' => 'features',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Name'),
      'has_body' => '1',
      'body_label' => t('Description'),
      'min_word_count' => '0',
      'help' => '',
    ),
    'toolkit' => array(
      'name' => t('Toolkit'),
      'module' => 'features',
      'description' => '',
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
function atrium_book_views_api() {
  return array(
    'api' => '2',
  );
}
