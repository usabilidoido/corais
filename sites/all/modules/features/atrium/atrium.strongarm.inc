<?php

/**
 * Implementation of hook_strongarm().
 */
function atrium_strongarm() {
  $export = array();

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'admin_toolbar';
  $strongarm->value = array(
    'layout' => 'vertical',
    'position' => 'nw',
    'blocks' => array(
      'admin-create' => -1,
      'admin-menu' => 1,
    ),
  );
  $export['admin_toolbar'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'allowed_html_1';
  $strongarm->value = '<div> <span> <br> <a> <em> <strong> <cite> <code> <strike> <ul> <ol> <li> <dl> <dt> <dd> <img> <blockquote> <p> <pre> <h1> <h2> <h3> <h4> <h5> <h6> <table> <caption> <thead> <th> <td> <tr> <sup> <sub>';
  $export['allowed_html_1'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'allowed_html_2';
  $strongarm->value = '<div> <span> <br> <a> <em> <strong> <cite> <code> <strike> <ul> <ol> <li> <dl> <dt> <dd> <img> <blockquote> <p> <pre> <h1> <h2> <h3> <h4> <h5> <h6> <table> <caption> <thead> <th> <td> <tr> <sup> <sub>';
  $export['allowed_html_2'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'allowed_html_3';
  $strongarm->value = '<div> <span> <br> <a> <em> <strong> <cite> <code> <strike> <ul> <ol> <li> <dl> <dt> <dd> <img> <blockquote> <p> <pre> <h1> <h2> <h3> <h4> <h5> <h6> <table> <caption> <thead> <th> <td> <tr> <sup> <sub>';
  $export['allowed_html_3'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'allowed_html_4';
  $strongarm->value = '<cite><blockquote><code><iframe><object><embed><div> <span> <br> <a> <em> <strong> <cite> <code> <strike> <ul> <ol> <li> <dl> <dt> <dd> <img> <blockquote> <p> <pre> <h1> <h2> <h3> <h4> <h5> <h6> <table> <caption> <thead> <th> <td> <tr> <sup> <sub>';
  $export['allowed_html_4'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'allowed_html_5';
  $strongarm->value = '<div> <span> <br> <a> <em> <strong> <cite> <code> <strike> <ul> <ol> <li> <dl> <dt> <dd> <img> <blockquote> <p> <pre> <h1> <h2> <h3> <h4> <h5> <h6> <table> <caption> <thead> <th> <td> <tr> <sup> <sub>';
  $export['allowed_html_5'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'context_reaction_block_all_regions';
  $strongarm->value = 1;
  $export['context_reaction_block_all_regions'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'date_format_field_date_long';
  $strongarm->value = 'g:ia D M j, Y';
  $export['date_format_field_date_long'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'date_format_field_date_medium';
  $strongarm->value = 'g:ia M j, Y';
  $export['date_format_field_date_medium'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'date_format_field_date_short';
  $strongarm->value = 'M j, Y';
  $export['date_format_field_date_short'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'date_format_long';
  $strongarm->value = 'l, j F, Y - H:i';
  $export['date_format_long'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'date_format_medium';
  $strongarm->value = 'D, d/m/Y - H:i';
  $export['date_format_medium'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'date_format_short';
  $strongarm->value = 'd/m/Y - H:i';
  $export['date_format_short'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'filter_default_format';
  $strongarm->value = '4';
  $export['filter_default_format'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'imageapi_jpeg_quality';
  $strongarm->value = 90;
  $export['imageapi_jpeg_quality'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'menu_primary_links_source';
  $strongarm->value = 'features';
  $export['menu_primary_links_source'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'menu_secondary_links_source';
  $strongarm->value = '';
  $export['menu_secondary_links_source'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'messaging_default_method';
  $strongarm->value = 'mail';
  $export['messaging_default_method'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'messaging_method_mail';
  $strongarm->value = array(
    'filter' => 3,
  );
  $export['messaging_method_mail'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'notifications_content_per_type';
  $strongarm->value = 1;
  $export['notifications_content_per_type'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'notifications_team_max_options';
  $strongarm->value = 200;
  $export['notifications_team_max_options'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'notifications_ui_account_options';
  $strongarm->value = array(
    'block' => 'block',
    'links' => 0,
  );
  $export['notifications_ui_account_options'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'notifications_ui_node';
  $strongarm->value = array(
    'block' => 'block',
  );
  $export['notifications_ui_node'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'notifications_ui_node_options';
  $strongarm->value = array(
    'block' => 'block',
    'form' => 0,
    'comment' => 0,
    'links' => 0,
    'teaserlinks' => 0,
    'subform' => 0,
  );
  $export['notifications_ui_node_options'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'notifications_ui_per_type';
  $strongarm->value = '0';
  $export['notifications_ui_per_type'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'notifications_ui_types';
  $strongarm->value = array(
    'grouptype' => 'grouptype',
    'thread' => 'thread',
    'nodetype' => 'nodetype',
    'author' => 'author',
  );
  $export['notifications_ui_types'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'notifications_ui_user';
  $strongarm->value = array(
    'page' => 'page',
  );
  $export['notifications_ui_user'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'og_home_page_view';
  $strongarm->value = '0';
  $export['og_home_page_view'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'og_notifications_content_types';
  $strongarm->value = array(
    'blog' => 'blog',
    0 => 'book',
    1 => 'event',
    2 => 'feed_ical',
    3 => 'feed_ical_item',
    4 => 'casetracker_basic_case',
    5 => 'casetracker_basic_project',
  );
  $export['og_notifications_content_types'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'purl_method_spaces_og';
  $strongarm->value = 'path';
  $export['purl_method_spaces_og'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'purl_types';
  $strongarm->value = array(
    'path' => 'path',
  );
  $export['purl_types'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'seed_book';
  $strongarm->value = array(
    'book_nav' => '0',
  );
  $export['seed_book'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'seed_imagecache_attachment';
  $strongarm->value = 'fileview';
  $export['seed_imagecache_attachment'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'site_403';
  $strongarm->value = 'access-denied';
  $export['site_403'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'site_404';
  $strongarm->value = 'not-found';
  $export['site_404'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'site_frontpage';
  $strongarm->value = 'dashboard';
  $export['site_frontpage'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'spaces_dashboard_blocks';
  $strongarm->value = array(
    'atrium_calendar-mini' => 1,
    'block-6' => 1,
    'boxes-box-d3d6afda' => 1,
    'comment-0' => 1,
    'og_cc-0' => 1,
    'og_frontpage-0' => 1,
    'og_image-0' => 1,
    'og_vocab-0' => 1,
    'user-3' => 1,
    'userpoints_top_contributors-0' => 1,
    'views-activity_listing-block_1' => 1,
    'views-activity_listing-block_2' => 1,
    'views-apoiadores-block_1' => 1,
    'views-atrium_poll-block_1' => 1,
    'views-blog_comments-block_1' => 1,
    'views-blog_listing-block_2' => 1,
    'views-calendar_upcoming-block_1' => 1,
    'views-groups_listing-block_2' => 1,
    'views-imageboard-block_1' => 1,
    'views-mc_exchanges_all-block_1' => 1,
    'views-members_listing-block_2' => 1,
    'views-oa_custom_task2-block_1' => 1,
    'views-oa_custom_webform-block_1' => 1,
    'views-og_videos-block_1' => 1,
    'views-text-block_1' => 1,
  );
  $export['spaces_dashboard_blocks'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'spaces_dashboard_layouts';
  $strongarm->value = array(
    'default' => 1,
    'columns' => 1,
    'split' => 1,
    'wide' => 0,
    'login' => 0,
  );
  $export['spaces_dashboard_layouts'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'spaces_features';
  $strongarm->value = array(
    'atrium_book' => '1',
    'atrium_calendar' => '0',
    'atrium_credit' => '0',
    'atrium_groups' => '1',
    'atrium_members' => '1',
    'atrium_poll' => '0',
    'features_task' => '0',
    'files' => '0',
    'imageboard' => '0',
    'spaces_dashboard' => '1',
  );
  $export['spaces_features'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'spaces_preset_status';
  $strongarm->value = array(
    'og_private' => TRUE,
    'og_public' => TRUE,
  );
  $export['spaces_preset_status'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'space_menu_items';
  $strongarm->value = array(
    'dashboard' => -20,
    'og' => -19,
    'knowledge' => -18,
    'members' => -17,
  );
  $export['space_menu_items'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'theme_settings';
  $strongarm->value = array(
    'toggle_logo' => 1,
    'toggle_name' => 1,
    'toggle_slogan' => 0,
    'toggle_mission' => 1,
    'toggle_node_user_picture' => 1,
    'toggle_comment_user_picture' => 1,
    'toggle_search' => 1,
    'toggle_favicon' => 1,
    'toggle_primary_links' => 1,
    'toggle_secondary_links' => 1,
    'toggle_node_info_book' => 0,
    'toggle_node_info_milestone' => 1,
    'toggle_node_info_images' => 1,
    'toggle_node_info_graphmind' => 0,
    'toggle_node_info_mass_contact' => 1,
    'toggle_node_info_board' => 1,
    'toggle_node_info_patch' => 1,
    'toggle_node_info_profile' => 1,
    'toggle_node_info_sheetnode' => 1,
    'toggle_node_info_blog' => 1,
    'toggle_node_info_service_provider' => 1,
    'toggle_node_info_group' => 1,
    'toggle_node_info_page' => 1,
    'toggle_node_info_webform' => 1,
    'toggle_node_info_event' => 1,
    'toggle_node_info_question' => 1,
    'toggle_node_info_task' => 1,
    'toggle_node_info_text' => 0,
    'toggle_node_info_toolkit' => 1,
    'toggle_node_info_poll' => 1,
    'toggle_node_info_currency' => 1,
    'toggle_node_info_exchange' => 1,
    'toggle_node_info_feed_ical_item' => 1,
    'toggle_node_info_feed_ical' => 1,
    'default_logo' => 1,
    'logo_path' => '',
    'logo_upload' => '',
    'default_favicon' => 0,
    'favicon_path' => 'sites/default/files/favicon_2.gif',
    'favicon_upload' => '',
    'designkit_image' => array(
      'logo' => 0,
    ),
    'designkit_color' => array(
      'background' => '#1a9adb',
    ),
  );
  $export['theme_settings'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'upload_extensions_default';
  $strongarm->value = 'fla au avi bzip2 csv doc docx flv gif graffle gz htm html iso jpeg jpg kml kmz mov mp2 mp3 mp4 odp ods odt pages patch pdf png pps ppt pptx psd rar svg swf template tif tgz txt vsd wav wmv xls xlsx zip';
  $export['upload_extensions_default'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'upload_list_default';
  $strongarm->value = '1';
  $export['upload_list_default'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'upload_uploadsize_default';
  $strongarm->value = '100';
  $export['upload_uploadsize_default'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'upload_usersize_default';
  $strongarm->value = '200';
  $export['upload_usersize_default'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'user_pictures';
  $strongarm->value = '1';
  $export['user_pictures'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'user_picture_default';
  $strongarm->value = 'profiles/openatrium/themes/ginkgo/images/user.png';
  $export['user_picture_default'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'user_picture_dimensions';
  $strongarm->value = '500x500';
  $export['user_picture_dimensions'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'user_picture_file_size';
  $strongarm->value = '500';
  $export['user_picture_file_size'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'user_picture_imagecache_comments';
  $strongarm->value = 'user-s';
  $export['user_picture_imagecache_comments'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'user_picture_imagecache_profiles';
  $strongarm->value = 'user-s';
  $export['user_picture_imagecache_profiles'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'user_picture_imagecache_profiles_default';
  $strongarm->value = 'user-s';
  $export['user_picture_imagecache_profiles_default'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'user_picture_imagecache_profiles_min_height';
  $strongarm->value = '60';
  $export['user_picture_imagecache_profiles_min_height'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'user_picture_imagecache_profiles_min_width';
  $strongarm->value = '60';
  $export['user_picture_imagecache_profiles_min_width'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'user_picture_path';
  $strongarm->value = 'pictures';
  $export['user_picture_path'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'user_register';
  $strongarm->value = '1';
  $export['user_register'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'user_signatures';
  $strongarm->value = '0';
  $export['user_signatures'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'views_no_hover_links';
  $strongarm->value = 1;
  $export['views_no_hover_links'] = $strongarm;

  return $export;
}
