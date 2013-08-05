<?php

/**
 * Implementation of hook_spaces_presets().
 */
function atrium_groups_spaces_presets() {
  $export = array();

  $spaces_presets = new stdClass;
  $spaces_presets->disabled = FALSE; /* Edit this to true to make a default spaces_presets disabled initially */
  $spaces_presets->api_version = 3;
  $spaces_presets->name = 'atrium_groups_private';
  $spaces_presets->title = 'Private group';
  $spaces_presets->description = 'A private group is accessible only to users added to that group by one of the group\'s managers.';
  $spaces_presets->space_type = 'og';
  $spaces_presets->value = array(
    'variable' => array(
      'spaces_features' => array(
        'atrium_answers' => '0',
        'atrium_blog' => '1',
        'atrium_calendar' => '1',
        'atrium_members' => '1',
        'atrium_poll' => '0',
        'atrium_shoutbox' => '1',
        'features_task' => '1',
        'features_webform' => '0',
        'files' => '0',
        'imageboard' => '0',
        'mindmaps' => '0',
        'spaces_dashboard' => '1',
        'spreadsheets' => '1',
        'texto_colaborativo' => '1',
      ),
      'spaces_dashboard_custom' => array(
        'custom-1' => 'Boas vindas',
        'custom-2' => 'Novidades',
        'custom-3' => '',
        'custom-4' => '',
        'custom-5' => '',
        0 => '',
      ),
      'spaces_setting_home' => 'dashboard',
      'spaces_og_selective' => '3',
      'spaces_og_directory' => 0,
      'spaces_og_register' => 0,
      'spaces_og_private' => 1,
      'designkit_color' => array(
        'background' => '#3399aa',
      ),
      'site_frontpage' => 'dashboard',
      'space_menu_items' => array(
        'dashboard' => -20,
        'blog' => -19,
        'notebook' => -18,
        'casetracker' => -17,
        'shoutbox' => -16,
        'calendar' => -15,
        'members' => -14,
      ),
      'site_frontpage_path' => 'dashboard',
      'designkit_image' => array(
        'logo' => 0,
      ),
    ),
    'context' => array(
      'spaces_dashboard-custom-1:reaction:block' => array(
        'blocks' => array(
          'views-calendar_upcoming-block_1' => array(
            'module' => 'views',
            'delta' => 'calendar_upcoming-block_1',
            'region' => 'left',
            'weight' => 0,
          ),
          'og_image-0' => array(
            'module' => 'og_image',
            'delta' => '0',
            'region' => 'content',
            'weight' => 0,
          ),
          'og_frontpage-0' => array(
            'module' => 'og_frontpage',
            'delta' => '0',
            'region' => 'right',
            'weight' => 0,
          ),
          'views-project_completeness-block_2' => array(
            'module' => 'views',
            'delta' => 'project_completeness-block_2',
            'region' => 'right',
            'weight' => 1,
          ),
          'og_cc-0' => array(
            'module' => 'og_cc',
            'delta' => '0',
            'region' => 'right',
            'weight' => 2,
          ),
        ),
      ),
      'spaces_dashboard-custom-2:reaction:block' => array(
        'blocks' => array(),
      ),
      'spaces_dashboard-custom-3:reaction:block' => array(
        'blocks' => array(),
      ),
      'spaces_dashboard-custom-4:reaction:block' => array(
        'blocks' => array(),
      ),
      'spaces_dashboard-custom-5:reaction:block' => array(
        'blocks' => array(),
      ),
    ),
  );

  // Translatables
  // Included for use with string extractors like potx.
  t('A private group is accessible only to users added to that group by one of the group\'s managers.');
  t('Private group');
  $export['atrium_groups_private'] = $spaces_presets;

  $spaces_presets = new stdClass;
  $spaces_presets->disabled = FALSE; /* Edit this to true to make a default spaces_presets disabled initially */
  $spaces_presets->api_version = 3;
  $spaces_presets->name = 'atrium_groups_public';
  $spaces_presets->title = 'Public group';
  $spaces_presets->description = 'A public group is accessible to all users on the site. Users can join and leave public groups freely.';
  $spaces_presets->space_type = 'og';
  $spaces_presets->value = array(
    'variable' => array(
      'spaces_features' => array(
        'atrium_answers' => '0',
        'atrium_blog' => '1',
        'atrium_calendar' => '0',
        'atrium_members' => '1',
        'atrium_poll' => '0',
        'atrium_shoutbox' => '0',
        'features_task' => '1',
        'features_webform' => '0',
        'files' => '0',
        'imageboard' => '0',
        'mindmaps' => '0',
        'spaces_dashboard' => '1',
        'spreadsheets' => '0',
        'texto_colaborativo' => '0',
      ),
      'site_frontpage' => 'dashboard',
      'spaces_dashboard_custom' => array(
        'custom-1' => 'Boas vindas',
        'custom-2' => 'Novidades',
        'custom-3' => '',
        'custom-4' => '',
        'custom-5' => '',
        0 => '',
      ),
      'spaces_og_selective' => '0',
      'spaces_og_directory' => 1,
      'spaces_og_private' => 0,
      'designkit_color' => array(
        'background' => '#97fb74',
      ),
      'space_menu_items' => array(
        'dashboard' => -20,
        'blog' => -19,
        'tasks' => -18,
        'imageboard' => -17,
        'files' => -16,
        'members' => -15,
      ),
      'site_frontpage_path' => 'dashboard',
      'atrium_book' => '0',
      'spaces_og_register' => 0,
      'designkit_image' => array(
        'logo' => '1774',
      ),
    ),
    'context' => array(
      'spaces_dashboard-custom-1:reaction:block' => array(
        'blocks' => array(
          'views-calendar_upcoming-block_1' => array(
            'module' => 'views',
            'delta' => 'calendar_upcoming-block_1',
            'region' => 'left',
            'weight' => 0,
          ),
          'og_frontpage-0' => array(
            'module' => 'og_frontpage',
            'delta' => '0',
            'region' => 'content',
            'weight' => 0,
          ),
          'og_image-0' => array(
            'module' => 'og_image',
            'delta' => '0',
            'region' => 'content',
            'weight' => 1,
          ),
          'block-7' => array(
            'module' => 'block',
            'delta' => '7',
            'region' => 'right',
            'weight' => 0,
          ),
          'views-og_image-block_1' => array(
            'module' => 'views',
            'delta' => 'og_image-block_1',
            'region' => 'right',
            'weight' => 1,
          ),
          'views-og_videos-block_1' => array(
            'module' => 'views',
            'delta' => 'og_videos-block_1',
            'region' => 'right',
            'weight' => 2,
          ),
          'og_cc-0' => array(
            'module' => 'og_cc',
            'delta' => '0',
            'region' => 'right',
            'weight' => 3,
          ),
        ),
        'layout' => 'split',
      ),
      'spaces_dashboard-custom-2:reaction:block' => array(
        'blocks' => array(
          'views-activity_listing-block_1' => array(
            'module' => 'views',
            'delta' => 'activity_listing-block_1',
            'region' => 'content',
            'weight' => 0,
          ),
          'grayside_oa_feeds-oa_feeds_space' => array(
            'module' => 'grayside_oa_feeds',
            'delta' => 'oa_feeds_space',
            'region' => 'right',
            'weight' => 0,
          ),
          'views-blog_comments-block_1' => array(
            'module' => 'views',
            'delta' => 'blog_comments-block_1',
            'region' => 'right',
            'weight' => 1,
          ),
        ),
      ),
      'spaces_dashboard-custom-4:reaction:block' => array(
        'blocks' => array(),
      ),
      'spaces_dashboard-custom-5:reaction:block' => array(
        'blocks' => array(),
      ),
    ),
    'boxes' => array(
      'og-4-ba570e6a' => (object) array(
        'delta' => 'og-4-ba570e6a',
        'title' => 'O que é Design Livre?',
        'description' => 'Introdução',
        'options' => array(
          'body' => '<iframe src="http://player.vimeo.com/video/16966516" width="600" height="350" frameborder="0"></iframe>',
          'format' => '4',
        ),
        'plugin_key' => 'simple',
      ),
    ),
  );

  // Translatables
  // Included for use with string extractors like potx.
  t('A public group is accessible to all users on the site. Users can join and leave public groups freely.');
  t('Public group');
  $export['atrium_groups_public'] = $spaces_presets;

  $spaces_presets = new stdClass;
  $spaces_presets->disabled = FALSE; /* Edit this to true to make a default spaces_presets disabled initially */
  $spaces_presets->api_version = 3;
  $spaces_presets->name = 'og_closed';
  $spaces_presets->title = 'Closed group';
  $spaces_presets->description = 'All users may view public content from this group. Only administrators can add new members.';
  $spaces_presets->space_type = 'og';
  $spaces_presets->value = array(
    'variable' => array(
      'spaces_og_register' => 0,
      'spaces_og_directory' => 1,
      'spaces_og_private' => 0,
      'spaces_features' => array(
        'atrium_answers' => '0',
        'atrium_blog' => '1',
        'atrium_calendar' => '0',
        'atrium_members' => '1',
        'atrium_poll' => '0',
        'atrium_shoutbox' => '0',
        'features_task' => '1',
        'features_webform' => '0',
        'files' => '0',
        'imageboard' => '0',
        'mindmaps' => '0',
        'spaces_dashboard' => '1',
        'spreadsheets' => '0',
        'texto_colaborativo' => '0',
      ),
      'site_frontpage' => 'dashboard',
      'site_frontpage_path' => 'dashboard',
      'designkit_color' => array(
        'background' => '#b47536',
      ),
      'spaces_dashboard_custom' => array(
        'custom-1' => 'Painel',
        'custom-2' => 'Novidades',
        'custom-3' => '',
        'custom-4' => '',
        'custom-5' => '',
        0 => '',
      ),
      'spaces_og_selective' => '2',
      'designkit_image' => array(
        'logo' => '1716',
      ),
      'space_menu_items' => array(
        'dashboard' => -20,
        'blog' => -19,
        'tasks' => -18,
        'members' => -17,
      ),
    ),
    'context' => array(
      'spaces_dashboard-custom-1:reaction:block' => array(
        'blocks' => array(
          'views-calendar_upcoming-block_1' => array(
            'module' => 'views',
            'delta' => 'calendar_upcoming-block_1',
            'region' => 'left',
            'weight' => 0,
          ),
          'og_frontpage-0' => array(
            'module' => 'og_frontpage',
            'delta' => '0',
            'region' => 'content',
            'weight' => 0,
          ),
          'og_image-0' => array(
            'module' => 'og_image',
            'delta' => '0',
            'region' => 'content',
            'weight' => 1,
          ),
          'block-7' => array(
            'module' => 'block',
            'delta' => '7',
            'region' => 'right',
            'weight' => 0,
          ),
          'views-og_image-block_1' => array(
            'module' => 'views',
            'delta' => 'og_image-block_1',
            'region' => 'right',
            'weight' => 1,
          ),
          'views-og_videos-block_1' => array(
            'module' => 'views',
            'delta' => 'og_videos-block_1',
            'region' => 'right',
            'weight' => 2,
          ),
          'og_cc-0' => array(
            'module' => 'og_cc',
            'delta' => '0',
            'region' => 'right',
            'weight' => 3,
          ),
        ),
        'layout' => 'split',
      ),
      'spaces_dashboard-custom-2:reaction:block' => array(
        'blocks' => array(
          'views-activity_listing-block_1' => array(
            'module' => 'views',
            'delta' => 'activity_listing-block_1',
            'region' => 'content',
            'weight' => 0,
          ),
          'grayside_oa_feeds-oa_feeds_space' => array(
            'module' => 'grayside_oa_feeds',
            'delta' => 'oa_feeds_space',
            'region' => 'right',
            'weight' => 0,
          ),
          'views-blog_comments-block_1' => array(
            'module' => 'views',
            'delta' => 'blog_comments-block_1',
            'region' => 'right',
            'weight' => 1,
          ),
          'user-3' => array(
            'module' => 'user',
            'delta' => '3',
            'region' => 'right',
            'weight' => 2,
          ),
        ),
      ),
    ),
  );

  // Translatables
  // Included for use with string extractors like potx.
  t('All users may view public content from this group. Only administrators can add new members.');
  t('Closed group');
  $export['og_closed'] = $spaces_presets;

  return $export;
}
