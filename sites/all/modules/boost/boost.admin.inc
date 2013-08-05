<?php

/**
 * @file
 * All the code for the Boost module's administrative interface.
 */

//////////////////////////////////////////////////////////////////////////////
// Performance Page Edits

/**
 * Performs alterations to the system settings form before it is rendered.
 *
 * @see hook_form_alter()
 *   in boost.module
 */
function boost_admin_performance_page($form = array()) {
  $form['#title'] = t('Anonymous page caching');
  $form['page_compression']['#title'] = t('Gzip page compression (Boost & Core)');
  $form['#description'] .= ' ' . t('The page must first be visited by an anonymous user in order for the cache to work on subsequent requests for that page. Boost & Core caching do not work for logged in users.');
  $form['boost_core_clear'] = array(
    '#type' => 'submit',
    '#value' => t('Clear core page cached data: !count pages', array('!count' => boost_count_core_db())),
    '#submit' => array('boost_clear_core_page_cache_submit'),
  );

  return $form;
}

/**
 * Process system_themes_form form submissions.
 */
function boost_admin_performance_page_submit($form, &$form_state) {
  if (variable_get('boost_ignore_flush', 0) == 0 && (variable_get('preprocess_css', FALSE)==TRUE || variable_get('preprocess_js', FALSE)==TRUE)) {
    if (boost_cache_clear_all()) {
      drupal_set_message(t('Boost: Static page cache cleared.'), 'warning');
    }
  }
}

/**
 * Flushes core page cache
 */
function boost_clear_core_page_cache_submit($form, &$form_state) {
  cache_clear_all('*', 'cache_page', TRUE);
  drupal_set_message(t('Core page cache cleared.'));
}

/**
 * Submit callback; clear system caches.
 */
function boost_admin_clear_cache_submit($form, &$form_state) {
  drupal_flush_all_caches();
  if (boost_cache_clear_all()) {
    drupal_set_message(t('Static page cache (boost) and all other caches cleared.'));
  }
  else {
    drupal_set_message(
      t('Boost: Static page cache NOT cleared. To clear Boosts cache set "Ignore cache flushing:" to \'Disabled\' in the <a href="@link">boost advanced settings</a> & try again.', array('@link' => url('admin/settings/performance/boost', array('fragment' => 'edit-boost-ignore-flush-0-wrapper')))), 'warning');
    drupal_set_message(t('All other caches cleared.'));
  }
}

//////////////////////////////////////////////////////////////////////////////
// Boost Settings Form

/**
 * Form builder; Displays Boost's configuration page.
 *
 * @see system_settings_form()
 */
function boost_admin_boost_performance_page() {
  Global $base_path, $_boost;

  $period = drupal_map_assoc(array(0, 60, 180, 300, 600, 900, 1800, 2700, 3600, 10800, 21600, 32400, 43200, 64800, 86400, 2*86400, 3*86400, 4*86400, 5*86400, 6*86400, 604800, 2*604800, 3*604800, 4*604800, 8*604800, 16*604800, 52*604800), 'format_interval');
  //$period[0] = '<' . t('none') . '>';

  $form['boost'] = array(
    '#type'          => 'fieldset',
    '#title'         => t('Boost File Cache'),
  );
  $form['boost']['boost_enabled'] = array(
    '#type'          => 'radios',
    '#title'         => t('Boost - Static page cache'),
    '#default_value' => variable_get('boost_enabled', CACHE_NORMAL),
    '#options'       => array(
      CACHE_DISABLED => t('Disabled'),
      CACHE_NORMAL => t('Enabled'),
      CACHE_AGGRESSIVE => t('(Not Recommended) Set Boost & Core (if enabled) cache for each page'),
    ),
    '#description'   => t('Static page caching is a mechanism that stores dynamically generated web pages as HTML files in a special cache directory located under the Drupal installation directory. By caching a web page in this manner, the web server can serve it out in the fastest possible manner, without invoking PHP or Drupal at all. While this does provide a significant performance and scalability boost, you should note that it could have negative usability side-effects unless your site is targeted at an audience consisting mostly of "anonymous" visitors.'),
  );
  $form['boost']['page_compression'] = array(
    '#type' => 'radios',
    '#title' => t('Gzip page compression (Boost & Core)'),
    '#default_value' => variable_get('page_compression', TRUE),
    '#options' => array(t('Disabled'), t('Enabled')),
    '#description' => t("By default, Drupal compresses the pages it caches in order to save bandwidth and improve download times. This option should be disabled when using a webserver that performs compression."),
    '#disabled' => !function_exists('gzencode'),
  );
  $form['boost']['boost_cache_lifetime'] = array(
    '#type' => 'select',
    '#title' => t('Boost - HTML - Default maximum cache lifetime'),
    '#default_value' => BOOST_CACHE_LIFETIME,
    '#options' => $period,
    '#description' => !BOOST_CACHE_HTML ? t('Enable the caching of this content type to enable this selection box.') : t('The maximum cache lifetime is the maximum amount of time that will elapse before the cache is emptied. Cache lifetime gets checked on cron runs. Flushing the content before it is expired can happen like for example when a node is edited.'),
    '#disabled' => !BOOST_CACHE_HTML,
  );
  $form['boost']['boost_cache_xml_lifetime'] = array(
    '#type' => 'select',
    '#title' => t('Boost - XML - Default maximum cache lifetime'),
    '#default_value' => BOOST_CACHE_XML_LIFETIME,
    '#options' => $period,
    '#description' => !BOOST_CACHE_XML ? t('Enable the caching of this content type to enable this selection box.') : t('The maximum cache lifetime is the maximum amount of time that will elapse before the cache is emptied. Cache lifetime gets checked on cron runs. Flushing the content before it is expired can happen like for example when a node is edited.'),
    '#disabled' => !BOOST_CACHE_XML,
  );
  $form['boost']['boost_cache_json_lifetime'] = array(
    '#type' => 'select',
    '#title' => t('Boost - JSON - Default maximum cache lifetime'),
    '#default_value' => BOOST_CACHE_JSON_LIFETIME,
    '#options' => $period,
    '#description' => !BOOST_CACHE_JSON ? t('Enable the caching of this content type to enable this selection box.') : t('The maximum cache lifetime is the maximum amount of time that will elapse before the cache is emptied. Cache lifetime gets checked on cron runs. Flushing the content before it is expired can happen like for example when a node is edited.'),
    '#disabled' => !BOOST_CACHE_JSON,
  );
  $form['boost']['boost_clear'] = array(
    '#type' => 'submit',
    '#value' => t('_0 Clear ALL Boost cached data: !count pages', array('!count' => boost_count_db(0))),
    '#submit' => array('boost_clear_cache_submit'),
  );
  $form['boost']['boost_clear_expired'] = array(
    '#type' => 'submit',
    '#value' => t('_1 Clear Boost expired data: !count pages', array('!count' => boost_count_db(1))),
    '#submit' => array('boost_clear_expired_cache_submit'),
  );

  // Cacheability settings
  $options = array(t('Cache every page except the listed pages.'), t('Cache only the listed pages.'));
  $description = t("Enter one page per line as Drupal paths. The '*' character is a wild-card. Example paths are '%blog' for the blog page and %blog-wild-card for every personal blog. %front is the front page.", array('%blog' => 'blog', '%blog-wild-card' => 'blog/*', '%front' => '<front>'));
  if (user_access('use PHP for block visibility')) {
    $options[] = t('Cache pages for which the following PHP code returns <code>TRUE</code> (PHP-mode, experts only).');
    $description .= t('If the PHP-mode is chosen, enter PHP code between %php. Note that executing incorrect PHP-code can severely break your Drupal site.', array('%php' => '<?php ?>'));
  }
  $form['cacheability'] = array(
    '#type'          => 'fieldset',
    '#title'         => t('Boost cacheability settings'),
  );
  $form['cacheability']['boost_cache_query'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Cache pages that contain URL Variables'),
    '#default_value' => BOOST_CACHE_QUERY,
    '#description'   => t('Boost will cache pages that end with "?page=1" among others (anything with a "?" in the url).'),
  );
  $form['cacheability']['boost_cache_html'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Cache html documents/pages'),
    '#default_value' => BOOST_CACHE_HTML,
    '#description'   => t('Boost will cache most Drupal pages.'),
  );
  $form['cacheability']['boost_cache_xml'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Cache .xml & /feed'),
    '#default_value' => BOOST_CACHE_XML,
    '#description'   => t('Boost will cache .xml and /feed urls as xml data.'),
  );
  $form['cacheability']['boost_cache_json'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Cache ajax/json'),
    '#default_value' => BOOST_CACHE_JSON,
    '#description'   => t('Boost will cache ajax/json responses.'),
  );
  $form['cacheability']['boost_cache_css'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Cache .css'),
    '#default_value' => BOOST_CACHE_CSS,
    '#description'   => t('Boost will cache CSS files.'),
  );
  $form['cacheability']['boost_cache_js'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Cache .js'),
    '#default_value' => BOOST_CACHE_JS,
    '#description'   => t('Boost will cache javascript files.'),
  );
  $form['cacheability']['boost_only_ascii_path'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Only allow ASCII characters in path'),
    '#default_value' => BOOST_ONLY_ASCII_PATH,
    '#description'   => t('Only allowing ACSII characters is a safe way to cache pages. It severely limits i18n support so this can be turned off. Fair warning, disabling this may cause "page not found" errors depending on your url structure (spaces are bad, ect...). If you follow RFC 3986 you should be ok.'),
  );
  $form['cacheability']['boost_set_file_encoding'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Convert the filename to this encoding'),
    '#default_value' => BOOST_SET_FILE_ENCODING,
    '#description'   => t('Use iconv to change the filename before its saved to disk. Odds are you will not need to set this. Example: CP1255 !link', array('!link' => l('http://php.net/iconv', 'http://php.net/iconv'))),
  );

  // Assumes fatal errors are fatal, thus the script will never get past the error.
  // http://php.net/errorfunc.constants
  $boost_error_levels = array(
    E_NOTICE => t('E_NOTICE: Run-time notices. Indicate that the script encountered something that could indicate an error, but could also happen in the normal course of running a script.'),
    E_USER_NOTICE => t('E_USER_NOTICE User-generated notice message. This is like an E_NOTICE, except it is generated in PHP code by using the PHP function trigger_error().'),

    E_STRICT => t('E_STRICT: Enable to have PHP suggest changes to your code which will ensure the best interoperability and forward compatibility of your code.'),
    E_DEPRECATED => t('E_DEPRECATED: Run-time notices. Enable this to receive warnings about code that will not work in future versions.'),
    E_USER_DEPRECATED => t('E_USER_DEPRECATED: User-generated warning message. This is like an E_DEPRECATED, except it is generated in PHP code by using the PHP function trigger_error().'),

    E_WARNING => t('E_WARNING: Run-time warnings (non-fatal errors). Execution of the script is not halted.'),
    E_CORE_WARNING => t('E_CORE_WARNING: Warnings (non-fatal errors) that occur during PHPs initial startup. This is like an E_WARNING, except it is generated by the core of PHP.'),
    E_COMPILE_WARNING => t('E_COMPILE_WARNING: Compile-time warnings (non-fatal errors). This is like an E_WARNING, except it is generated by the Zend Scripting Engine.'),
    E_USER_WARNING => t('E_USER_WARNING: User-generated warning message. This is like an E_WARNING, except it is generated in PHP code by using the PHP function trigger_error().'),

    E_RECOVERABLE_ERROR => t('E_RECOVERABLE_ERROR: Catchable fatal error. It indicates that a probably dangerous error occured, but did not leave the Engine in an unstable state. If the error is not caught by a user defined handle (see also set_error_handler()), the application aborts as it was an E_ERROR.'),
    E_USER_ERROR => t('E_USER_ERROR: User-generated error message. This is like an E_ERROR, except it is generated in PHP code by using the PHP function trigger_error().'),
  );
  $boost_error_levels_defaults = $boost_error_levels;
  $boost_error_levels_defaults[E_NOTICE] = E_NOTICE;
  $boost_error_levels_defaults[E_USER_NOTICE] = E_USER_NOTICE;
  $form['cacheability']['boost_halt_on_errors'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable PHP error cacheability checking'),
    '#description' => t('Checked - Do not cache the page if there are PHP errors. Unchecked - Cache pages even if it might contain errors.'),
    '#default_value' => variable_get('boost_halt_on_errors', FALSE),
  );
  $form['cacheability']['boost_errors_fields'] = array(
    '#type' => 'fieldset',
    '#title' => t('PHP Error Levels and Cacheability'),
    '#collapsible' => TRUE,
    '#collapsed' => !variable_get('boost_halt_on_errors', FALSE),
    '#description' => t('Select the error levels you wish to ingore. Checked: Cache this page even if this error was thrown. Unchecked: Do no cache this page if this error was thrown'),
  );
  $form['cacheability']['boost_errors_fields']['boost_halt_on_errors_list'] = array(
    '#type'          => 'checkboxes',
    '#title'         => t('Error types to ignore & still cache page'),
    '#default_value' => variable_get('boost_halt_on_errors_list', $boost_error_levels_defaults),
    '#disabled' => !variable_get('boost_halt_on_errors', FALSE),
    '#options' =>  $boost_error_levels,
  );

  $form['cacheability']['boost_halt_on_messages'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Do not cache if a message is on the page'),
    '#default_value' => BOOST_HALT_ON_MESSAGES,
    '#description'   => t('Selected - Do not cache the page if there are Drupal messages. Not Selected - Cache pages even if it might contain a Drupal message.'),
  );
  $form['cacheability']['boost_cache_url_alias_src'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Do not cache if there is an alias for this URL.'),
    '#default_value' => variable_get('boost_cache_url_alias_src', FALSE),
    '#description'   => t('Enabling this will prevent Boost from caching node/8 if node/8 is also mapped to a path like /about-us. If using Global Redirect enabling this is a good idea.'),
  );
  $form['cacheability']['boost_cacheability_option'] = array(
    '#type'          => 'radios',
    '#title'         => t('Statically cache specific pages'),
    '#options'       => $options,
    '#default_value' => BOOST_CACHEABILITY_OPTION,
  );
  $form['cacheability']['boost_cacheability_pages'] = array(
    '#type'          => 'textarea',
    '#title'         => t('Pages'),
    '#default_value' => BOOST_CACHEABILITY_PAGES,
    '#description'   => $description,
    '#wysiwyg'       => FALSE,
  );

  // Views intergration
  if (module_exists('views')) {
    $form['views'] = array(
      '#type'          => 'fieldset',
      '#title'         => t('Boost settings for views'),
    );
    $form['views']['boost_flush_views'] = array(
      '#type'          => 'checkbox',
      '#title'         => t('Clear all cached views pages associated with a node on update/delete'),
      '#default_value' => BOOST_FLUSH_VIEWS,
      // '#description'   => '', // TODO
    );
    $form['views']['boost_flush_views_insert'] = array(
      '#type'          => 'checkbox',
      '#title'         => t('Clear all cached views pages associated with a node on insert'),
      '#default_value' => BOOST_FLUSH_VIEWS_INSERT,
      '#description'   => t('WARNING: This can be very slow, all views get run to find out where this node lives. Odds are it will not be slow, but if a node save takes a very long time with this enabled, you know why.'),
    );
    $form['views']['boost_flush_views_update'] = array(
      '#type'          => 'checkbox',
      '#title'         => t('Clear all cached views pages associated with a node on update'),
      '#default_value' => BOOST_FLUSH_VIEWS_UPDATE,
      '#description'   => t('WARNING: This can be very slow, all views get run to find out where this node lives. Odds are it will not be slow, but if a node save takes a very long time with this enabled, you know why.'),
    );
    $form['views']['boost_views_list_behavior'] = array(
      '#type'          => 'radios',
      '#title'         => t('New views behavior'),
      '#default_value' => BOOST_VIEWS_LIST_BEHAVIOR,
      '#options'       => array(
        0 => t('Run code on cron to check if boost should use the view'),
        1 => t('View is not tracked by boost until enabled'),
      ),
      '#description'   => t('This setting mainly effects new views.'),
    );
    $views = boost_views_get_valid_list(TRUE);
    $form['views']['boost_views_list_custom'] = array(
      '#type' => 'checkboxes',
      '#title' => t('Views to run when looking for content relationships'),
      '#default_value' => $views,
      '#options' => array_combine(array_keys($views), array_keys($views)),
      '#description' => t('Leave these at the defaults unless you have a good reason to change it. Non-default settings should be at the top of this list.'),
    );
  }

  // Cache expiration settings
  $form['expiration'] = array(
    '#type'          => 'fieldset',
    '#title'         => t('Boost cache expiration/flush settings'),
  );
  $form['expiration']['boost_expire_cron'] = array(
    '#type'          => 'radios',
    '#title'         => t('Clear expired pages on cron runs'),
    '#default_value' => (int)variable_get('boost_expire_cron', TRUE),
    '#options'       => array(
      0 => t('Disabled'),
      1 => t('Enabled'),
      ),
    '#description'   => t('Caution - If disabled you will need some other method of clearing cached pages. If Disabled, files have to be manually deleted from server or cleared using the administrative interface (<a href="@blocks">Enable</a> the <a href="@status">Boost: Pages cache status</a> block).', array('@blocks' => url('admin/build/block'), '@status' => url('admin/build/block/configure/boost/status'))),
  );
  $form['expiration']['boost_check_before_cron_expire'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Check database timestamps for any site changes. Only if there has been a change will boost flush the expired content on cron.'),
    '#default_value' => BOOST_CHECK_BEFORE_CRON_EXPIRE,
  );
  $form['expiration']['boost_flush_dir'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Clear all empty folders from cache.'),
    '#default_value' => BOOST_FLUSH_DIR,
    '#description'   => t('Disable this if you have to set settings for each dir/subdir, due to the way your server operates (permissions, etc...).'),
  );
  $form['expiration']['boost_flush_front'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Clear the front page cache when promoted'),
    '#default_value' => BOOST_FLUSH_FRONT,
    '#description'   => t('Clear the front page cache when a node that is promoted to the from page is also changed.'),
  );
  $form['expiration']['boost_flush_cck_references'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Clear all cached pages referenced via CCK with a node on insert/update/delete'),
    '#default_value' => BOOST_FLUSH_CCK_REFERENCES,
    '#description'   => t('The <a href="@url">Node Referrer</a> module is recommended.', array('@url' => 'http://drupal.org/project/nodereferrer')),
  );
  $form['expiration']['boost_flush_node_terms'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Clear all cached terms pages associated with a node on insert/update/delete'),
    '#default_value' => BOOST_FLUSH_NODE_TERMS,
    '#description'   => t('Works with view\'s taxonomy/term/% path as well as core.'),
  );
  $form['expiration']['boost_flush_menu_items'] = array(
    '#type'          => 'radios',
    '#title'         => t('Clear all cached pages in a menu on an insert/update/delete operation'),
    '#default_value' => BOOST_FLUSH_MENU_ITEMS,
    '#description'   => t('This can flush a lot of pages depending on your menu structure.'),
    '#options'       => array(
      0 => t('Disabled'),
      1 => t('Only Flush Menu Parents, Siblings & Children'),
      2 => t('Flushes Entire Menu Tree'),
    ),
  );
  $form['expiration']['boost_clear_cache_offline'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Clear Boosts cache when site goes offline'),
    '#default_value' => BOOST_CLEAR_CACHE_OFFLINE,
    '#description'   => t('Under site maintenance when the status is set to offline, boost clears its cache. If you do not want this to happen, clear this checkbox. Pages that are not cached will still send out a Site off-line message, so be smart if turning this off.'),
  );
  $form['expiration']['boost_flush_all_multisite'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Flush all sites caches in this database (singe db, multisite).'),
    '#default_value' => BOOST_FLUSH_ALL_MULTISITE,
    '#description'   => t('This will flush/expire all cached files stored in this database, instead of only being specific to this site. Useful for i18n sites. You need to setup a separate cron call for each database (in your multisite install) either way though. This covers shared database usage; or place the boost tables into the a shared database, to have this setting work in a multiple database environment.'),
  );
  $form['expiration']['boost_expire_no_flush'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Expire content in DB, do not flush file.'),
    '#default_value' => BOOST_EXPIRE_NO_FLUSH,
    '#description'   => t('Instead of flushing the file ASAP this will set the database entry to expired. File will be flushed on next cron run. This is more on the extreme side of tweaks.'),
  );
  $form['expiration']['boost_ignore_flush'] = array(
    '#type'          => 'radios',
    '#title'         => t('Ignore cache flushing'),
    '#default_value' => variable_get('boost_ignore_flush', 1),
    '#options'       => array(
      0 => t('Disabled'),
      1 => t('Only Ignore Clear Entire Cache Commands (Recommended if caching css/js files)'),
      2 => t('Ignore Clear Entire Cache Commands & Cron Expiration'),
      3 => t('Ignore All Delete Commands (Not Recommended)'),
    ),
    '#description'   => t('Make a selection to put your site into a static cached state. Recommend turning on CSS & JS caching if enabled.'),
  );

  // Directories
  $form['directories'] = array(
    '#type'          => 'fieldset',
    '#title'         => t('Boost directories and file extensions'),
  );
  $form['directories']['boost_root_cache_dir'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Cache Dir'),
    '#default_value' => BOOST_ROOT_CACHE_DIR,
    '#size'          => 15,
    '#maxlength'     => 255,
    '#required'      => TRUE,
    // '#description'   => '', // TODO
  );
  $form['directories']['boost_multisite_single_db'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Do not store the cache file path in the database'),
    '#default_value' => BOOST_MULTISITE_SINGLE_DB,
    '#description'   => t('Enabling will allow for correct multi-site caching, in cases where different content is served from the same Drupal installation, based on domain. Examples: Multi-site with a single/shared database, site translation detection based on domain, and the contributed "Domain Access" module.'),
  );
  if (!BOOST_MULTISITE_SINGLE_DB) {
//     $form['directories']['boost_host'] = array(
//       '#type'          => 'textfield',
//       '#title'         => t('Hostname'),
//       '#default_value' => BOOST_HOST,
//       '#size'          => 60,
//       '#maxlength'     => 255,
//       '#description'   => '',
//     );
    $form['directories']['boost_file_path'] = array(
      '#type'          => 'textfield',
      '#title'         => t('Cache file path'),
      '#default_value' => BOOST_FILE_PATH,
      '#size'          => 60,
      '#maxlength'     => 255,
      '#required'      => TRUE,
      '#description'   => t('A file system path where the static cache files will be stored. This directory has to exist and be writable by Drupal. The default setting is to store the files in a directory named %default-path under the Drupal installation directory. If you change this, you must also change the URL rewrite rules in your web server configuration (.htaccess for Apache, lighttpd.conf for Lighttpd), or caching will not work.', array('%default-path' => boost_cache_directory(BOOST_HOST, FALSE))),
    );
  }
  $form['directories']['generated'] = array(
    '#type'          => 'fieldset',
    '#collapsible'   => TRUE,
    '#collapsed'     => TRUE,
    '#title'         => t('Generated output storage (HTML, XML, AJAX)'),
  );
  $form['directories']['generated']['boost_normal_dir'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Directory for non compressed files'),
    '#default_value' => BOOST_NORMAL_DIR,
    '#size'          => 15,
    '#maxlength'     => 255,
    '#description'   => t('Leave blank to not use a subdirectory'),
  );
  $form['directories']['generated']['boost_gzip_dir'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Directory for compressed files'),
    '#default_value' => BOOST_GZIP_DIR,
    '#size'          => 15,
    '#maxlength'     => 255,
    '#description'   => t('Leave blank to not use a subdirectory'),
  );
  $form['directories']['generated']['boost_char'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Character used to replace "?"'),
    '#default_value' => BOOST_CHAR,
    '#size'          => 15,
    '#maxlength'     => 16,
    '#required'      => TRUE,
    '#description'   => t('Best to leave at _'),
  );
  if (BOOST_CACHE_HTML) {
    $form['directories']['generated']['boost_file_extension'] = array(
      '#type'          => 'textfield',
      '#title'         => t('Cache html file extension'),
      '#default_value' => BOOST_FILE_EXTENSION,
      '#size'          => 10,
      '#maxlength'     => 32,
      '#required'      => TRUE,
      '#description'   => t('The file extension to append to the file name of the generated cache files. Note that this setting is of no relevance to any public URLs, and it is strongly recommended to leave this as the default \'.html\' unless you know what you are doing. If you change this, you must also change the URL rewrite rules in your web server configuration (.htaccess for Apache, lighttpd.conf for Lighttpd), or caching will not work.'),
    );
  }
  if (BOOST_CACHE_XML) {
    $form['directories']['generated']['boost_xml_extension'] = array(
      '#type'          => 'textfield',
      '#title'         => t('Cache xml file extension'),
      '#default_value' => BOOST_XML_EXTENSION,
      '#size'          => 10,
      '#maxlength'     => 32,
      '#required'      => TRUE,
      '#description'   => t('The file extension to append to the file name of the generated cache files. Note that this setting is of no relevance to any public URLs, and it is strongly recommended to leave this as the default \'.xml\' unless you know what you are doing. If you change this, you must also change the URL rewrite rules in your web server configuration (.htaccess for Apache, lighttpd.conf for Lighttpd), or caching will not work.'),
    );
  }
  if (BOOST_CACHE_JSON) {
    $form['directories']['generated']['boost_json_extension'] = array(
      '#type'          => 'textfield',
      '#title'         => t('Cache ajax/json file extension'),
      '#default_value' => BOOST_JSON_EXTENSION,
      '#size'          => 10,
      '#maxlength'     => 32,
      '#required'      => TRUE,
      '#description'   => t('The file extension to append to the file name of the generated cache files. Note that this setting is of no relevance to any public URLs, and it is strongly recommended to leave this as the default \'.js\' unless you know what you are doing. If you change this, you must also change the URL rewrite rules in your web server configuration (.htaccess for Apache, lighttpd.conf for Lighttpd), or caching will not work.'),
    );
  }
  $form['directories']['static'] = array(
    '#type'          => 'fieldset',
    '#collapsible'   => TRUE,
    '#collapsed'     => TRUE,
    '#title'         => t('Static storage (CSS, JS)'),
  );
  $form['directories']['static']['boost_perm_normal_dir'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Directory for more permanent files (css/js)'),
    '#default_value' => BOOST_PERM_NORMAL_DIR,
    '#size'          => 15,
    '#maxlength'     => 255,
    '#description'   => t('Leave blank to not use a subdirectory'),
  );
  $form['directories']['static']['boost_perm_gz_dir'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Directory for compressed more permanent files (css/js)'),
    '#default_value' => BOOST_PERM_GZ_DIR,
    '#size'          => 15,
    '#maxlength'     => 255,
    '#description'   => t('Leave blank to not use a subdirectory'),
  );
  $form['directories']['static']['boost_perm_char'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Character used to replace "?" in the permanent directory'),
    '#default_value' => BOOST_PERM_CHAR,
    '#size'          => 15,
    '#maxlength'     => 16,
    '#required'      => TRUE,
    '#description'   => t('Best to leave at _'),
  );
  if (BOOST_CACHE_CSS) {
    $form['directories']['static']['boost_css_extension'] = array(
      '#type'          => 'textfield',
      '#title'         => t('Cache css file extension'),
      '#default_value' => BOOST_CSS_EXTENSION,
      '#size'          => 10,
      '#maxlength'     => 32,
      '#required'      => TRUE,
      '#description'   => t('The file extension to append to the file name of the generated cache files. Note that this setting is of no relevance to any public URLs, and it is strongly recommended to leave this as the default \'.css\' unless you know what you are doing. If you change this, you must also change the URL rewrite rules in your web server configuration (.htaccess for Apache, lighttpd.conf for Lighttpd), or caching will not work.'),
    );
  }
  if (BOOST_CACHE_JS) {
    $form['directories']['static']['boost_js_extension'] = array(
      '#type'          => 'textfield',
      '#title'         => t('Cache javascript file extension'),
      '#default_value' => BOOST_JS_EXTENSION,
      '#size'          => 10,
      '#maxlength'     => 32,
      '#required'      => TRUE,
      '#description'   => t('The file extension to append to the file name of the generated cache files. Note that this setting is of no relevance to any public URLs, and it is strongly recommended to leave this as the default \'.js\' unless you know what you are doing. If you change this, you must also change the URL rewrite rules in your web server configuration (.htaccess for Apache, lighttpd.conf for Lighttpd), or caching will not work.'),
    );
  }

  // Advanced settings
  $form['advanced'] = array(
    '#type'          => 'fieldset',
    '#title'         => t('Boost advanced settings'),
  );
  $form['advanced']['boost_pre_process_function'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Pre-process function'),
    '#default_value' => BOOST_PRE_PROCESS_FUNCTION,
    '#maxlength'     => 255,
    '#description'   => t('The name of a PHP function used to pre-process the contents of each page before writing them out to static files. The function is called with the contents of the page passed as a string argument, and its return value is used as the data written out to the disk.'),
  );
  $form['advanced']['boost_exit_in_hook_exit'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Exit() inside of boost_exit'),
    '#default_value' => BOOST_EXIT_IN_HOOK_EXIT,
    '#description'   => t('If using ubercart disable this setting.'),
  );
  $form['advanced']['boost_aggressive_cookie'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Aggressive setting of the boost cookie'),
    '#default_value' => BOOST_AGGRESSIVE_COOKIE,
    '#description'   => t('Set/Remove the boost cookie in the boost_init function.'),
  );
  $form['advanced']['boost_asynchronous_output'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Asynchronous Operation: output HTML, close connection, then store static file.'),
    '#default_value' => BOOST_ASYNCHRONOUS_OUTPUT,
    '#description'   => t('Run php in the background. When a cached page is generated, this will allow for faster page generation; downside is the headers are not the standard ones outputted by Drupal; sends "Connection: close" instead of "Connection: Keep-Alive".'),
  );
  $form['advanced']['boost_overwrite_file'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Overwrite the cached file if it already exits'),
    '#default_value' => BOOST_OVERWRITE_FILE,
    '#description'   => t('This is useful if crawling a site before it goes live.'),
  );
  $form['advanced']['boost_pager_clean'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Save ?page=1 as /page/1'),
    '#default_value' => BOOST_PAGER_CLEAN,
    '#description'   => t('See http://drupal.org/node/897970'),
  );
  $form['advanced']['boost_disable_clean_url'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Turn off clean url\'s for logged in users'),
    '#default_value' => BOOST_DISABLE_CLEAN_URL,
    '#description'   => t('Drupal will output non clean url\'s for non anonymous users. This allows for the browser to cache the page and still have logging in work. This is more on the extreme side of tweaks.'),
  );
  if (BOOST_GZIP) {
    $form['advanced']['boost_aggressive_gzip'] = array(
      '#type'          => 'checkbox',
      '#title'         => t('Aggressive Gzip: Deliver gzipped content independent of the request header.'),
      '#default_value' => BOOST_AGGRESSIVE_GZIP,
      '#description'   => t('In order to deliver gzipped content independent of the header, this will test for gzip compression in a small iframe by sending it compressed content. This compressed content is javascript which creates a cookie with a note of gzip support. On the server side it checks for the cookie and then sends out gzipped content accordingly. See <a href="@url">Website Performance - Activate Gzip</a>. In short some firewalls/proxies mangle the gzip header; this gets around that. iframe is on non compressed version of the frontpage only.', array('@url' => 'http://actionable-stats.com/website-performance-activate-gzip')),
    );
  }
  $form['advanced']['boost_insert_node_on_creation'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Insert node into boost_cache table on node creation'),
    '#default_value' => variable_get('boost_insert_node_on_creation', FALSE),
    '#description'   => t('Useful for the crawler, insert a "not cached" entry into the db on node creation; fully supports the domain access module.'),
  );
  $form['advanced']['boost_store_url_percent_enc'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Store the URL in the database as a percent encoded string.'),
    '#default_value' => variable_get('boost_store_url_percent_enc', FALSE),
    '#description'   => t('Convert any characters in the URI that can be percent encoded. See <a href="@link">RFC 3986 Section 2</a> for more info. Runs this code on the URL: urldecode(rawurlencode($url))', array('@link' => 'http://tools.ietf.org/html/rfc3986#section-2')),
  );
  $form['advanced']['boost_permissions_file'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Files: Enter in a 4 digit number (octal) that will be used by !link. Example 0664', array('!link' => l(t('chmod()'), 'http://php.net/chmod'))),
    '#default_value' => BOOST_PERMISSIONS_FILE,
    '#description'   => t('Sometimes because of funky servers you need it use a different file mode then the default.'),
  );
  $form['advanced']['boost_permissions_dir'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Directories: Enter in a 4 digit number (octal) that will be used by !link. Example 0775', array('!link' => l(t('chmod()'), 'http://php.net/chmod'))),
    '#default_value' => BOOST_PERMISSIONS_DIR,
    '#description'   => t('Sometimes because of funky servers you need it use a different file mode then the default.'),
  );
  $form['advanced']['boost_verbose'] = array(
    '#type'          => 'select',
    '#title'         => t('Watchdog Verbose Setting'),
    '#default_value' => BOOST_VERBOSE,
    '#options'       => array(
      1 => t('1 Record almost no errors'),
      3 => t('3'),
      5 => t('5 Record all errors to the db log (watchdog)'),
      7 => t('7 Debug Mode: Output a lot of extra info.'),
      9 => t('9 Debug Overkill: Output what is getting expired.'),
    ),
  );
  $form['advanced']['verbose'] = array(
    '#type'          => 'fieldset',
    '#collapsible'   => TRUE,
    '#collapsed'     => TRUE,
    '#title'         => t('Watchdog Verbose Settings'),
  );
  $options = array(
    'boost_init_404' => t('L7 - 404 received from server via redirect. boost_init()'),
    'boost_exit_headers' => t('L7 - Headers already sent. boost_exit()'),
    'boost_cron_expire' => t('L5 - Expired stale files from static page cache. boost_cron()'),
    'boost_view_insert' => t('L7 - Views Searched; X contain new node. _boost_view_insert()'),
    'boost_expire_node' => t('L7 - Node was flushed; resulting in X pages being expired. boost_expire_node()'),
    'boost_user_op' => t('L9 - X operation sent to hook_user. boost_user()'),
    'boost_user_delete' => t('L7 - UID was deleted resulting in X pages being expired. boost_user()'),
    'boost_block_flush_submit' => t('L7 - Path was deleted/expired resulting in X pages being expired. boost_block_form_flush_submit()'),
    'boost_block_settings_submit' => t('L7 - X pages where expired from cache so new settings take effect. boost_block_db_settings_form_submit()'),
    'boost_block_rm_settings_submit' => t('L7 - X pages where expired from cache so new settings take effect. boost_block_db_rm_settings_form_submit()'),
    'boost_ob_handler_info' => t('L7 - Info about page in boosts output buffer. _boost_ob_handler()'),
    'boost_ob_handler_redirect' => t('L7 - Browser redirect detected; remove cache entry for this path. _boost_ob_handler()'),
    'boost_cache_clear_all' => t('L5 - Flushed ALL files from static page cache. boost_cache_clear_all()'),
    'boost_cache_flush_filename' => t('L9 - The Following files where flushed. boost_cache_flush_by_filename()'),
    'boost_cache_expire_router' => t('L9 - The Following files where flushed. boost_cache_expire_router()'),
    'boost_cache_expire_all_db_list' => t('L9  - Following files where flushed. boost_cache_expire_all_db()'),
    'boost_cache_expire_all_db_count' => t('L7  - X files where flushed. boost_cache_expire_all_db()'),
    'boost_cache_set_node_relationships' => t('L7 - X entries to the boost_cache_relationships table added or updated. boost_cache_set_node_relationships()'),
    'boost_crawler_run_stop' => t('L5 - Thread stopped. boost_crawler_run()'),
    'boost_crawler_run_rogue' => t('L5 - Rogue thread killed. boost_crawler_run()'),
    'boost_crawler_run_sleep' => t('L5 - Crawler Sleep for 15 seconds. boost_crawler_run()'),
    'boost_crawler_run_shutdown' => t('L5 - Crawler sleeping for X seconds, do to forced shutdown. boost_crawler_run()'),
    'boost_crawler_run_kill' => t('L5 - Thread X of X Killed. boost_crawler_run()'),
    'boost_crawler_run_startup' => t('L5 - Thread X of X started. boost_crawler_run()'),
    'boost_crawler_run_restart' => t('L5 - Restarting with 1 thread. boost_crawler_run()'),
    'boost_crawler_run_done' => t('L5 - Thread X of X Done. boost_crawler_run()'),
    'boost_crawler_run_start' => t('L5 - Crawler Start X. boost_crawler_run()'),
  );
  $form['advanced']['verbose']['boost_verbose_refined'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Watchdog Verbose Refinement'),
    '#default_value' => variable_get('boost_verbose_refined', $_boost['verbose_option_defaults']),
    '#options' => $options,
    '#description' => t('Enable/Disable certian watchdog messages.'),
  );
  if (ini_get('safe_mode')) {
    $form['advanced']['boost_ignore_safe_warning'] = array(
      '#type'          => 'checkbox',
      '#title'         => t('Disable warning about php safe mode'),
      '#default_value' => BOOST_IGNORE_SAFE_WARNING,
      '#description'   => t('Disable the warning on the status page about running boost in safe mode.'),
    );
  }
  $form['advanced']['boost_ignore_subdir_limit'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Disable warning about reaching the ext3 file system subdir limit.'),
    '#default_value' => BOOST_IGNORE_SUBDIR_LIMIT,
    '#description'   => t('Disable the warning on the status page about coming close to the file system directory limit; warning thrown when at 31,000 subdirectories. Boost will stop creating new subdirectories when 31,998 subdirectories have been created. Can be a slow operation & might even timeout; if your status page will not load, Disable this warning.'),
  );
  $form['advanced']['no_db'] = array(
    '#type'          => 'fieldset',
    '#collapsible'   => TRUE,
    '#collapsed'     => TRUE,
    '#title'         => t('Boost Retro Mode (no database)'),
  );
  $form['advanced']['no_db']['boost_no_database'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('NOT RECOMMENDED. Do not use the database at all.'),
    '#default_value' => BOOST_NO_DATABASE,
    '#description'   => t('Go old school and don\'t use the database. Very extreme tweak & support for features in this mode is pretty much non existent.'),
  );

  // Domain Access. Issue #1056578
  $form['domain'] = array(
    '#type'          => 'fieldset',
    '#title'         => t('Domain-specific settings'),
  );
  $form['domain']['boost_domain_use_lists'] = array(
    '#type'          => 'select',
    '#title'         => t('Use domain whitelisting or blacklisting?'),
    '#options'       => array(
        BOOST_DOMAIN_NO_LISTS       => t('No'),
        BOOST_DOMAIN_WHITELIST_ONLY => t('Whitelist only'),
        BOOST_DOMAIN_BLACKLIST_ONLY => t('Blacklist only'),
        BOOST_DOMAIN_BOTH_LISTS     => t('Whitelist and Blacklist'),
      ),
    '#default_value' => variable_get('boost_domain_use_lists', BOOST_DOMAIN_NO_LISTS),
    '#description'   => t("Allows you to specify which domains will and won't be cached.  If you're not running a multi-domain site, don't worry about this."),
  );
  $whitelist = variable_get('boost_domain_whitelist', array());
  if (variable_get('boost_domain_whitelist_use_domain', FALSE) && function_exists('domain_domains')) {
    $extra_domains = domain_domains();
    foreach ($extra_domains as $ed_key => $ed_value) {
      unset($whitelist[ $ed_value['subdomain'] ]);
    }
  }
  $form['domain']['boost_domain_whitelist'] = array(
    '#type'          => 'textarea',
    '#title'         => t('Whitelist'),
    '#default_value' => implode("\n", $whitelist),
    '#description'   => t("Domains in this list will be considered for caching, if they don't appear on the blacklist.  Note that if a domain does not appear in this list and is not matched by an entry on the wildcard whitelist, Boost will not cache pages on that domain.<br>List fully-qualified domain names here, one per line.  Example: foo.example.com"),
    '#prefix'        => '<div id="boost-domain-whitelist">',
  );
  if (module_exists('domain')) {
    $form['domain']['boost_domain_whitelist_use_domain'] = array(
      '#type'          => 'checkbox',
      '#default_value' => variable_get('boost_domain_whitelist_use_domain', FALSE),
      '#title'         => t('Include all managed domains in the whitelist.'),
      '#description'   => t('If this box is checked, all domains and domain aliases managed by the <a href="admin/build/domain/view">Domain Access</a> module will be whitelisted automatically.'),
    );
  }
  $form['domain']['boost_domain_whitelist_wild'] = array(
    '#type'          => 'textarea',
    '#title'         => t('Whitelist (wildcards)'),
    '#default_value' => implode("\n", variable_get('boost_domain_whitelist_wild', array())),
    '#description'   => t('Domains that match entries in this list will be considered for caching unless they appear on the blacklist.<br>List fully-qualified domain names here, one per line, using "*" to represent wildcard values.  Example: *.quux.*.example.com'),
    '#suffix'        => '</div>',
  );
  $form['domain']['boost_domain_blacklist'] = array(
    '#type'          => 'textarea',
    '#title'         => t('Blacklist'),
    '#default_value' => implode("\n", variable_get('boost_domain_blacklist', array())),
    '#description'   => t('Domains in this list will never be considered for caching, even if they appear on or are matched by an entry of the whitelist.<br>List fully-qualified domain names here, one per line.  Example: mail.example.com'),
    '#prefix'        => '<div id="boost-domain-blacklist">',
    '#suffix'        => '</div>',
  );
  $whitelist_js = "
$(update_list_visibility);
$(function(){ $('#edit-boost-domain-use-lists').change(function (){update_list_visibility();})});
function update_list_visibility() {
  var use_lists = $('#edit-boost-domain-use-lists').val();
  var white_only = ".BOOST_DOMAIN_WHITELIST_ONLY.";
  var black_only = ".BOOST_DOMAIN_BLACKLIST_ONLY.";
  var both_lists = ".BOOST_DOMAIN_BOTH_LISTS.";
  //show blacklist
  use_lists == both_lists || use_lists == black_only
    ? $('#boost-domain-blacklist').show()
    : $('#boost-domain-blacklist').hide();

  //show whitelist
  use_lists == both_lists || use_lists == white_only
    ? $('#boost-domain-whitelist').show()
    : $('#boost-domain-whitelist').hide();
}";
  drupal_add_js($whitelist_js, 'inline');

  // Crawler
  $form['crawler'] = array(
    '#type'          => 'fieldset',
    '#title'         => t('Boost crawler'),
  );
  $form['crawler']['boost_crawl_on_cron'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Enable the cron crawler'),
    '#default_value' => BOOST_CRAWL_ON_CRON,
    '#description'   => t('Pre-cache boosted URL\'s so they get cached before anyone accesses them. Enable the crawler first and save settings to use Preemptive Cache settings.'),
  );
  if (BOOST_CRAWL_ON_CRON) {
    $description = t('Bypass cron cache expiration and load to be expired content in the crawler. Allow this servers IP to bypass the boost cache. Check for fast but potentially stale; uncheck for slow but always fresh.');
  }
  else {
    $description = t('Enable the crawler first to enable this checkbox.');
  }
  $form['crawler']['boost_loopback_bypass'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Do not flush expired content on cron run, instead recrawl and overwrite it.'),
    '#default_value' => BOOST_LOOPBACK_BYPASS,
    '#description'   => $description,
    '#disabled' => !BOOST_CRAWL_ON_CRON,
  );
  if (BOOST_CRAWL_ON_CRON && BOOST_CACHE_HTML) {
    $description = t('Crawl Site after cron runs, so the cache is primed.');
  }
  elseif (!BOOST_CRAWL_ON_CRON) {
    $description = t('Enable the crawler first to enable this checkbox.');
  }
  elseif (!BOOST_CACHE_HTML) {
    $description = t('Enable the caching of this content type to enable this checkbox');
  }
  $form['crawler']['boost_push_html'] = array(
    '#type' => 'checkbox',
    '#title' => t('Preemptive Cache HTML'),
    '#default_value' => BOOST_PUSH_HTML,
    '#description' => $description,
    '#disabled' => !BOOST_CRAWL_ON_CRON || !BOOST_CACHE_HTML,
  );
  if (BOOST_CRAWL_ON_CRON && BOOST_CACHE_XML) {
    $description = t('Crawl Site after cron runs, so the cache is primed.');
  }
  elseif (!BOOST_CRAWL_ON_CRON) {
    $description = t('Enable the crawler first to enable this checkbox.');
  }
  elseif (!BOOST_CACHE_XML) {
    $description = t('Enable the caching of this content type to enable this checkbox');
  }
  $form['crawler']['boost_push_xml'] = array(
    '#type' => 'checkbox',
    '#title' => t('Preemptive Cache XML'),
    '#default_value' => BOOST_PUSH_XML,
    '#description' => $description,
    '#disabled' => !BOOST_CRAWL_ON_CRON || !BOOST_CACHE_XML,
  );
  if (BOOST_CRAWL_ON_CRON && BOOST_CACHE_JSON) {
    $description = t('Crawl Site after cron runs, so the cache is primed.');
  }
  elseif (!BOOST_CRAWL_ON_CRON) {
    $description = t('Enable the crawler first to enable this checkbox.');
  }
  elseif (!BOOST_CACHE_JSON) {
    $description = t('Enable the caching of this content type to enable this checkbox');
  }
  $form['crawler']['boost_push_json'] = array(
    '#type' => 'checkbox',
    '#title' => t('Preemptive Cache AJAX/JSON'),
    '#default_value' => BOOST_PUSH_JSON,
    '#description' => $description,
    '#disabled' => !BOOST_CRAWL_ON_CRON || !BOOST_CACHE_JSON,
  );
  $form['crawler']['boost_crawl_url_alias'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Crawl All URL\'s in the url_alias table'),
    '#default_value' => BOOST_CRAWL_URL_ALIAS,
    '#description'   => !BOOST_CRAWL_ON_CRON ? t('Enable the crawler first to enable this checkbox.') : t('Preemptively cache all urls found in the Drupal url_alias table. This will crawl that page even if it is not expired. Enable & run cron to get the boost cache loaded.'),
    '#disabled' => !BOOST_CRAWL_ON_CRON
  );
  $form['crawler']['boost_crawl_db_import_size'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Number of URLs to grab at a time when loading up the crawler table'),
    '#default_value' => BOOST_CRAWL_DB_IMPORT_SIZE,
    '#description'   => t('You might need to adjust this (shrink) if you get mysql errors when starting the crawler. Increase if your trying load up the crawler with a lot of URLs, as this can make the operation be faster.'),
  );
  $form['crawler']['boost_crawler_throttle'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Crawler Throttle'),
    '#default_value' => BOOST_CRAWLER_THROTTLE,
    '#description'   => t('Wait X micro seconds in between hitting each url. 1000000 is 1 second.'),
  );
  $form['crawler']['boost_crawler_batch_size'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Crawler Batch Size'),
    '#default_value' => BOOST_CRAWLER_BATCH_SIZE,
    '#description'   => t('Number of URL\'s each thread grabs per database operation.'),
  );
  // TODO use BOOST_MAX_THREADS, or textfield...
  $form['crawler']['boost_crawler_threads'] = array(
    '#type'          => 'select',
    '#title'         => t('Number Of Threads'),
    '#default_value' => BOOST_CRAWLER_THREADS,
    '#options' => array(
      1 => 1,
      2 => 2,
      3 => 3,
      4 => 4,
      5 => 5,
      6 => 6,
      7 => 7,
      8 => 8,
    ),
    '#description'   => t('Be careful when choosing more then 2 threads.'),
  );
  $total = boost_crawler_total_count();
  $number_done = min($total, unserialize(db_result(db_query("SELECT value FROM {variable} WHERE name = 'boost_crawler_position'")))) + 1;
  $crawl_rate = round((BOOST_TIME - variable_get('boost_crawler_start_time', BOOST_TIME))/$number_done, 2) + .01;
  $crawler_live = boost_crawler_threads_alive() && $number_done > 0 && !variable_get('boost_crawler_stopped', FALSE) && round(($total - $number_done)/$crawl_rate) > 0;
  $form['crawler']['live'] = array(
    '#type'          => 'fieldset',
    '#collapsible'   => TRUE,
    '#collapsed'     => !$crawler_live,
    '#title'         => t('Boost crawler - Live info'),
  );
  $form['crawler']['live']['boost_crawler_rate'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Crawl Rate - Seconds Per Node (highly inaccurate)'),
    '#default_value' => $crawl_rate,
    '#disabled'      => TRUE,
  );
  $form['crawler']['live']['boost_crawler_eta'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Crawl ETA - highly inaccurate'),
    '#default_value' => format_interval(($total - $number_done)/$crawl_rate),
    '#disabled'      => TRUE,
  );
  $form['crawler']['live']['boost_stop_crawler'] = array(
    '#type' => 'submit',
    '#value' => t('_2 Stop Crawler. !count URL\'s left.', array('!count' => $total - $number_done)),
    '#submit' => array('boost_stop_crawler_submit'),
    '#disabled'      => !$crawler_live,
  );
  $form['crawler']['boost_reset_crawler'] = array(
    '#type' => 'submit',
    '#value' => t('_3 Reset Crawler & Cron Semaphore'),
    '#submit' => array('boost_reset_crawler_submit'),
    '#disabled'      => $crawler_live,
  );
  if (BOOST_CRAWL_ON_CRON) {
    $form['crawler']['boost_crawler_eta'] = array(
      '#type'          => 'textfield',
      '#title'         => t('Estimated Time to crawl site - highly inaccurate'),
      '#default_value' => format_interval(boost_count_db(2) * boost_average_time()),
      '#disabled'      => TRUE,
    );
  }

  // Apache .htaccess settings generation
  $htaccess = boost_admin_generate_htaccess();
  $form['htaccess'] = array(
    '#type'          => 'fieldset',
    '#title'         => t('Boost Apache .htaccess settings generation'),
    '#description'   => t('<a href="!link">Explanation of .htaccess variables</a> <br /><br /> <strong>Be sure to save the configuration and then go to the <a href="!rules">htaccess rules generation page</a> and copy the rules.</strong>', array('!link' => url('http://www.askapache.com/htaccess/mod_rewrite-variables-cheatsheet.html'), '!rules' => url('admin/settings/performance/boost-rules'))),
  );
  $form['htaccess']['boost_server_name_http_host'] = array(
    '#type'          => 'radios',
    '#title'         => t('Servers URL or Name'),
    '#default_value' => variable_get('boost_server_name_http_host', '%{HTTP_HOST}'),
    '#options'       => array(
      '%{SERVER_NAME}' => '%{SERVER_NAME}',
      '%{HTTP_HOST}' => '%{HTTP_HOST}',
      $_SERVER['SERVER_NAME'] => $_SERVER['SERVER_NAME'],
      $_SERVER['HTTP_HOST'] => $_SERVER['HTTP_HOST'],
    ),
    '#description'   => t('Best to leave these as %{}, only try the last option(s) if boost is still not working.'),
  );
  // Set DOCUMENT_ROOT
  $drupal_subdir = rtrim($base_path, '/');
  $document_root = str_replace("\\", '/', getcwd()); // fix windows dir slashes
  $document_root = str_replace($drupal_subdir, '', $document_root); // remove subdir
  $options = array('%{DOCUMENT_ROOT}' => '%{DOCUMENT_ROOT}', $document_root => $document_root); // initial options
  $rejects = array('SCRIPT_FILENAME', 'DOCUMENT_ROOT'); // values to ignore
  $output = boost_array_find($document_root, $_SERVER, $rejects); //search for values that match getcwd
  $description_extra = '';
  if (!empty($output)) {
    foreach ($output as $key => $value) {
      $temp = '%{ENV:' . $key . '}';
      $options[$temp] = $temp; // adding values to options
      if ($value == $document_root) {
        $best = $temp; // set best since it's a match
      }
    }
  }
  if ($_SERVER['DOCUMENT_ROOT'] == $document_root) {
    $best = '%{DOCUMENT_ROOT}';
  }
  elseif (!isset($best)) {
    $best = $document_root;
    $description_extra = t('Please <a href="!link">open an boost issue on Drupal.org</a>, since apache and php might not be configured correctly.', array('!link' => url('http://drupal.org/node/add/project-issue/boost')));
  }
  $description = t('Value of %best is recommended for this server.', array('%best' => $best)) . ' ' . $description_extra;
  $form['htaccess']['boost_document_root'] = array(
    '#type'          => 'radios',
    '#title'         => t('Document Root'),
    '#default_value' => variable_get('boost_document_root', '%{DOCUMENT_ROOT}'),
    '#options'       => $options,
    '#description'   => $description,
  );
  $form['htaccess']['boost_apache_etag'] = array(
    '#type'          => 'radios',
    '#title'         => t('ETag Settings'),
    '#default_value' => variable_get('boost_apache_etag', 0),
    '#options'       => array(
      0 => 'Do Nothing',
      1 => "Set FileETag 'None' - Do not send an etag",
      2 => "Set FileETag 'All' - Default if enabled",
      3 => "Set FileETag 'MTime Size' - Useful in server clusters (Recommended)",
    ),
    '#description'   => t('Uses <a href="!link">FileETag Directive</a> to set <a href="!about">ETags</a> for the files cached by Boost. <a href="!stack">More info on this subject</a>', array('!link' => url('http://httpd.apache.org/docs/trunk/mod/core.html#fileetag'), '!about' => url('http://en.wikipedia.org/wiki/HTTP_ETag'), '!stack' => url('http://stackoverflow.com/questions/tagged?tagnames=etag&sort=votes&pagesize=50'))),
  );
  $form['htaccess']['boost_apache_xheader'] = array(
    '#type'          => 'radios',
    '#title'         => t('Boost Tags'),
    '#default_value' => variable_get('boost_apache_xheader', 0),
    '#options'       => array(
      0 => 'Inject boost tags at the bottom of the file',
      1 => 'Set Header and tags',
      2 => 'Only Set the header',
    ),
    '#description'   => t('In order to identify that the page is being served from the cache Boost can: Set a comment at the bottom of the page, indicating that this page is cached by boost; It can also send out a header that will identify any files served from the boost cache; Or it can do both.'),
  );
  $form['htaccess']['boost_apache_rfc2616'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Follow RFC2616 14.9.4'),
    '#default_value' => variable_get('boost_apache_rfc2616', FALSE),
    '#description'   => t('Bypass the boost cache when the headers contain a "no-cache" cache-control directive (ctrl-F5 in browser). Disable this to make boost always serve pages from the cache. Good time to disable this is right before your site goes into maintenance mode. IE & this RFC do not work well together, recommend disabling it.'),
  );
  $form['htaccess']['boost_ssl_bypass'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Bypass the boost cache for ssl requests.'),
    '#default_value' => variable_get('boost_ssl_bypass', TRUE),
    '#description'   => t('Ticking this is recommended if you use the securepages module.'),
  );
  $form['htaccess']['boost_force_utf8'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Add "AddDefaultCharset utf-8" to the htaccess rules'),
    '#default_value' => variable_get('boost_force_utf8', TRUE),
    '#description'   => t('Depending on your i18n settings you might want this disabled or enabled.'),
  );
  $form['htaccess']['boost_ignore_htaccess_warning'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Ignore .htaccess warning'),
    '#default_value' => variable_get('boost_ignore_htaccess_warning', FALSE),
    '#description'   => t('Do not warn about missing or modified boost rules in the .htaccess file on the <a href="!link">status report page</a>. Enable this if you have a good reason to modify the boost rules in .htaccess.', array('!link' => url('admin/reports/status'))),
  );

  // Clear database button
  $form['clear'] = array(
    '#type' => 'fieldset',
    '#title' => t("Clear Boost's Database & File Cache"),
    '#description' => t('Warning: This gives you a clean slate for the boost database & file system, use with caution. If you change the directories or file extensions (in "Boost directories and file extensions"), then pressing this button would be a good idea, but not required. If you changed the CSS or JavaScript and wish to push it out, you need to press this button.'),
  );
  $form['clear']['boost_reset'] = array(
    '#type' => 'submit',
    '#value' => t('_4 Reset Button - Database Records: !records, Files: !files', array('!records' => boost_count_db(2), '!files' => boost_count_all_files(BOOST_ROOT_CACHE_DIR))),
    '#submit' => array('boost_reset_database_file_submit'),
  );

  // Form validation
  $form['#validate'][] = 'boost_admin_boost_performance_page_validate';
  $form['#submit'][] = 'boost_admin_boost_performance_page_submit';
  return system_settings_form($form);
}

/**
 * submit boost_admin_boost_performance_page form submissions.
 */
function boost_admin_boost_performance_page_submit($form, &$form_state) {
  boost_htaccess_cache_dir_put();

  // Skip if views not enabled
  if (module_exists('views')) {
    $key = 'boost_views_list_custom';
    $op = isset($form_state['values']['op']) ? $form_state['values']['op'] : '';

    if ($op == t('Reset to defaults')) {
      variable_del($key);
    }
    else {
      $defaults = boost_views_generate_default_list();
      $new = $form_state['values'][$key];
      $disable = array_diff($defaults, $new);
      $enable = array_diff($new, $defaults);
      $values = $enable;
      foreach ($disable as $hash => $value) {
        $values[$hash] = $new[$hash];
      }
      unset($values['line-break']);

      if ($values) {
        variable_set($key, $values);
      }
      else {
        variable_del($key);
      }
    }
    unset($form_state['values'][$key]);
  }

  //split white/blacklists into arrays
  $whitelist_values = explode("\n", $form_state['values']['boost_domain_whitelist']);
  $whitelist_values = array_map('trim', $whitelist_values);
  $whitelist = array_combine($whitelist_values, $whitelist_values);
  if ($form_state['values']['boost_domain_whitelist_use_domain'] && function_exists('domain_domains')) {
    $extra_domains = domain_domains();
    foreach ($extra_domains as $key => $extra_domain) {
      $whitelist[ $extra_domain['subdomain'] ] = $extra_domain['subdomain'];
    }
  }
  asort($whitelist);
  variable_set('boost_domain_whitelist', $whitelist);
  unset($form_state['values']['boost_domain_whitelist']);

  $whitelist_wild_values = explode("\n", $form_state['values']['boost_domain_whitelist_wild']);
  $whitelist_wild_values = array_map('trim', $whitelist_wild_values);
  $whitelist_wild = array_combine($whitelist_wild_values, $whitelist_wild_values);
  asort($whitelist_wild);
  variable_set('boost_domain_whitelist_wild', $whitelist_wild);
  unset($form_state['values']['boost_domain_whitelist_wild']);

  $blacklist_values = explode("\n", $form_state['values']['boost_domain_blacklist']);
  $blacklist_values = array_map('trim', $blacklist_values);
  $blacklist = array_combine($blacklist_values, $blacklist_values);
  asort($blacklist);
  variable_set('boost_domain_blacklist', $blacklist);
  unset($form_state['values']['boost_domain_blacklist']);
}

/**
 * validate boost_admin_boost_performance_page form submissions.
 */
function boost_admin_boost_performance_page_validate($form, &$form_state) {
  $boost_previously = variable_get('boost_enabled', '');
  extract($form_state['values'], EXTR_SKIP | EXTR_REFS);
  if (BOOST_MULTISITE_SINGLE_DB) {
    $boost_file_path = boost_cache_directory(NULL, FALSE, $boost_root_cache_dir, $boost_normal_dir);
  }

  // Boost enabled/disabled logic
  if ($boost_enabled != CACHE_DISABLED) {
    // The cache is enabled
    // Ensure the cache directory exists or can be created:
    _boost_mkdir_p($boost_file_path);
    file_check_directory($boost_file_path, FILE_CREATE_DIRECTORY, 'boost_file_path');
  }
  elseif ($boost_enabled == CACHE_DISABLED && $boost_previously != CACHE_DISABLED) {
    // The cache was previously enabled
    variable_set('boost_ignore_flush', 0);
    if (boost_cache_clear_all()) {
      drupal_set_message(t('Boost: Static page cache cleared.'));
    }
  }

  // Validate file extensions
  if (BOOST_CACHE_HTML && isset($boost_file_extension) && strpos($boost_file_extension, '.') !== 0) {
    form_set_error('boost_file_extension', t('Cache file extension %extension must begin with a period.', array('%extension' => $boost_file_extension)));
  }
  if (BOOST_CACHE_XML && isset($boost_xml_extension) && strpos($boost_xml_extension, '.') !== 0) {
    form_set_error('boost_xml_extension', t('Cache file extension %extension must begin with a period.', array('%extension' => $boost_xml_extension)));
  }
  if (BOOST_CACHE_JSON && isset($boost_json_extension) && strpos($boost_json_extension, '.') !== 0) {
    form_set_error('boost_json_extension', t('Cache file extension %extension must begin with a period.', array('%extension' => $boost_json_extension)));
  }
  if (BOOST_CACHE_CSS && isset($boost_css_extension) && strpos($boost_css_extension, '.') !== 0) {
    form_set_error('boost_css_extension', t('Cache file extension %extension must begin with a period.', array('%extension' => $boost_css_extension)));
  }
  if (BOOST_CACHE_JS && isset($boost_js_extension) && strpos($boost_js_extension, '.') !== 0) {
    form_set_error('boost_js_extension', t('Cache file extension %extension must begin with a period.', array('%extension' => $boost_js_extension)));
  }

  // Check that the preprocess function exists.
  if (!empty($boost_pre_process_function) && !is_callable($boost_pre_process_function)) {
    form_set_error('boost_pre_process_function', t('Pre-process function %function() does not exist.', array('%function' => $boost_pre_process_function)));
  }

  // Validate whitelist and blacklist elements.
  $use_lists = $form_state['values']['boost_domain_use_lists'];
  if (  $use_lists != BOOST_DOMAIN_NO_LISTS
     && $use_lists != BOOST_DOMAIN_WHITELIST_ONLY
     && $use_lists != BOOST_DOMAIN_BLACKLIST_ONLY
     && $use_lists != BOOST_DOMAIN_BOTH_LISTS) {
    form_set_error('boost_domain_use_lists', t('Invalid selection'));
  }

  if ($use_lists == BOOST_DOMAIN_WHITELIST_ONLY || $use_lists == BOOST_DOMAIN_BOTH_LISTS) {
    $whitelist_values = explode("\n", $form_state['values']['boost_domain_whitelist']);
    $whitelist_values = array_map('trim', $whitelist_values);
    $whitelist = array_combine($whitelist_values, $whitelist_values);
    if (function_exists('domain_valid_domain')) {
      foreach ($whitelist as $key => $value) {
        $error_msg = domain_valid_domain($value);
        if (!empty($error_msg)) {
          $error_msg = str_replace(':', t(' in domain whitelist:'), $error_msg);
          form_set_error('boost_domain_whitelist', $error_msg);
        }
      }
    }

    $whitelist_wild_values = explode("\n", $form_state['values']['boost_domain_whitelist_wild']);
    $whitelist_wild_values = array_map('trim', $whitelist_wild_values);
    $whitelist_wild = array_combine($whitelist_wild_values, $whitelist_wild_values);
    if (function_exists('domain_valid_domain')) {
      foreach ($whitelist_wild as $key => $value) {
        $value = str_replace('*', 'qwerqwrkasdwiopekasdn', $value);
        $error_msg = domain_valid_domain($value);
        if (!empty($error_msg)) {
          $error_msg = str_replace(':', t(' in domain wildcard whitelist:'), $error_msg);
          $error_msg = str_replace('qwerqwrkasdwiopekasdn', '*', $error_msg);
          form_set_error('boost_domain_whitelist_wild', $error_msg);
        }
      }
    }
  }

  if ($use_lists == BOOST_DOMAIN_BLACKLIST_ONLY || $use_lists == BOOST_DOMAIN_BOTH_LISTS) {
    $blacklist_values = explode("\n", $form_state['values']['boost_domain_blacklist']);
    $blacklist_values = array_map('trim', $blacklist_values);
    $blacklist = array_combine($blacklist_values, $blacklist_values);
    if (function_exists('domain_valid_domain')) {
      foreach ($blacklist as $key => $value) {
        $error_msg = domain_valid_domain($value);
        if (!empty($error_msg)) {
          $error_msg = str_replace(':', t(' in domain blacklist:'), $error_msg);
          form_set_error('boost_domain_blacklist', $error_msg);
        }
      }
    }
  }
}

/**
 * Submit handler for boost_admin_htaccess_page
 */
function boost_admin_htaccess_page_submit($form, &$form_state) {
  // Save Settings
  $boost_autoupdate_robots_txt = $form_state['values']['boost_autoupdate_robots_txt'];
  $boost_autoupdate_htaccess = $form_state['values']['boost_autoupdate_htaccess'];
  variable_set('boost_autoupdate_robots_txt', $boost_autoupdate_robots_txt);
  variable_set('boost_autoupdate_htaccess', $boost_autoupdate_htaccess);

  // Act on settings
  if (!empty($boost_autoupdate_robots_txt)) {
    boost_update_robots_txt(variable_get('boost_enabled', CACHE_NORMAL));
  }
  if (!empty($boost_autoupdate_htaccess)) {
    boost_update_htaccess(variable_get('boost_enabled', CACHE_NORMAL));
  }
}

function boost_admin_site_offline_submit($form, &$form_state) {
  if (!empty($form_state['values']['site_offline']) && BOOST_CLEAR_CACHE_OFFLINE) {
    if (boost_cache_clear_all()) {
      drupal_set_message(t('Boost: Static page cache cleared.'), 'warning');
    }
  }
}

function boost_admin_modules_submit($form, &$form_state) {
  if ((variable_get('preprocess_css', FALSE)==TRUE || variable_get('preprocess_js', FALSE)==TRUE)) {
    if (boost_cache_clear_all()) {
      drupal_set_message(t('Boost: Static page cache cleared.'), 'warning');
    }
  }
}

function boost_admin_themes_submit($form, &$form_state) {
  if ((variable_get('preprocess_css', FALSE)==TRUE || variable_get('preprocess_js', FALSE)==TRUE)) {
    if (boost_cache_clear_all()) {
      drupal_set_message(t('Boost: Static page cache cleared.'), 'warning');
    }
  }
}

/**
 * Form builder; Displays Boost's htaccess generation page.
 *
 * @see system_settings_form()
 */
function boost_admin_htaccess_page() {
  if (is_writable('robots.txt') || is_writable('.htaccess')) {
    // Filesystem autoupdates
    $form['autoupdate'] = array(
      '#type' => 'fieldset',
      '#title' => t('Auto patch .htaccess & robots.txt')
    );
    // robots.txt
    if (is_writable('robots.txt')) {
      $form['autoupdate']['boost_autoupdate_robots_txt'] = array(
        '#type' => 'checkbox',
        '#title' => t('Insert rule into robots.txt on Save'),
        '#description' => t('If this box is checked, insert &quot;Disallow: /boost_stats.php&quot; into robots.txt if boost is enabled.'),
        '#default_value' => variable_get('boost_autoupdate_robots_txt', FALSE),
      );
    }
    // .htaccess
    if (is_writable('.htaccess')) {
      $form['autoupdate']['boost_autoupdate_htaccess'] = array(
        '#type' => 'checkbox',
        '#title' => t('Update .htaccess on Save'),
        '#description' => t('If this box is checked, update .htaccess with the generated Boost htaccess rules if Boost is enabled. Shown below is what will be inserted; editing the text below will have no impact on what gets written.'),
        '#default_value' => variable_get('boost_autoupdate_htaccess', FALSE),
      );
    }
    $form['autoupdate']['submit'] = array('#type' => 'submit', '#value' => t('Save configuration to file system'));
  }

  // Generated .htaccess output
  $htaccess = boost_admin_generate_htaccess();
  $form['boost_generated'] = array(
    '#type'          => 'textarea',
    '#title'         => t('Generated Rules'),
    '#default_value' => $htaccess,
    '#rows'          => count(explode("\n", $htaccess))+1,
    '#wysiwyg'       => FALSE,
    '#description'   => t("Copy this into your .htaccess file below <pre><tt>  # RewriteBase / </tt></pre> and above <pre><tt>  # Rewrite URLs of the form 'x' to the form 'index.php?q=x'</tt></pre><br />Note that the generated rules' settings can be configure at !link under 'Boost Apache .htaccess settings generation' section.", array('!link' => l('admin/settings/performance/boost', 'admin/settings/performance/boost'))),
  );
  $form['#submit'][] = 'boost_admin_htaccess_page_submit';

  return $form;
}

/**
 * Generate htaccess code.
 *
 * http://www.askapache.com/htaccess/mod_rewrite-variables-cheatsheet.html
 *
 * @return string
 *   htaccess code
 */
function boost_admin_generate_htaccess() {
  Global $base_path;
  $server_name = variable_get('boost_server_name_http_host', '%{HTTP_HOST}');
  $document_root = variable_get('boost_document_root', '%{DOCUMENT_ROOT}');
  $drupal_subdir = rtrim($base_path, '/');

  // Various dir's
  $cache_dir = BOOST_ROOT_CACHE_DIR;
  $gzip_dir = !(BOOST_GZIP_DIR == '') ? '/' . BOOST_GZIP_DIR . '/' : '/';
  $normal_dir = !(BOOST_NORMAL_DIR == '') ? '/' . BOOST_NORMAL_DIR . '/': '/';
  $permanent_dir_gzip = !(BOOST_PERM_GZ_DIR == '') ? '/' . BOOST_PERM_GZ_DIR  . '/': '/';
  $permanent_dir = !(BOOST_PERM_NORMAL_DIR == '') ? '/' . BOOST_PERM_NORMAL_DIR  . '/': '/';

  // with a \ slash
  $html = str_replace('.', '\\.', BOOST_FILE_EXTENSION);
  $xml  = str_replace('.', '\\.', BOOST_XML_EXTENSION);
  $css  = str_replace('.', '\\.', BOOST_CSS_EXTENSION);
  $js   = str_replace('.', '\\.', BOOST_JS_EXTENSION);
  $json = str_replace('.', '\\.', BOOST_JSON_EXTENSION);
  $gz   = str_replace('.', '\\.', BOOST_GZIP_EXTENSION);

  $char = BOOST_CHAR;
  $permanent_char = BOOST_PERM_CHAR;
  $server_ip = str_replace('.', '\\.', $_SERVER['SERVER_ADDR']);


  // Generate the rules
  $string  = "  ### BOOST START ###\n";
  if (BOOST_AGGRESSIVE_GZIP) {
    $string .= "\n";
    $string .= "  # Gzip Cookie Test\n";
    $string .= "  RewriteRule ^(.*)boost-gzip-cookie-test\.html $cache_dir$permanent_dir_gzip" . "boost-gzip-cookie-test\.html\.gz [L,T=text/html,E=no-gzip:1]\n";
  }
  if (BOOST_CACHE_CSS || BOOST_CACHE_JS) {
    if (BOOST_GZIP) {
      $skip = BOOST_CACHE_CSS && BOOST_CACHE_JS ? 2 : 1;
      $string .= "\n";
      $string .= "  # GZIP - Cached css & js files\n";
      $string .= BOOST_AGGRESSIVE_GZIP ? "  RewriteCond %{HTTP_COOKIE} !(boost-gzip)\n" : '';
      $string .= "  RewriteCond %{HTTP:Accept-encoding} !gzip\n";
      $string .= "  RewriteRule .* - [S=$skip]\n";
      if (BOOST_CACHE_CSS) {
        $string .= "  RewriteCond $document_root$base_path$cache_dir$permanent_dir_gzip$server_name%{REQUEST_URI}$permanent_char$css$gz -s\n";
        $string .= "  RewriteRule .* $cache_dir$permanent_dir_gzip$server_name%{REQUEST_URI}$permanent_char$css$gz [L,QSA,T=text/css,E=no-gzip:1]\n";
      }
      if (BOOST_CACHE_JS) {
        $string .= "  RewriteCond $document_root$base_path$cache_dir$permanent_dir_gzip$server_name%{REQUEST_URI}$permanent_char$js$gz -s\n";
        $string .= "  RewriteRule .* $cache_dir$permanent_dir_gzip$server_name%{REQUEST_URI}$permanent_char$js$gz [L,QSA,T=text/javascript,E=no-gzip:1]\n";
      }
    }
    $string .= "\n";
    $string .= "  # NORMAL - Cached css & js files\n";
    if (BOOST_CACHE_CSS) {
      $string .= "  RewriteCond $document_root$base_path$cache_dir$permanent_dir$server_name%{REQUEST_URI}$permanent_char$css -s\n";
      $string .= "  RewriteRule .* $cache_dir$permanent_dir$server_name%{REQUEST_URI}$permanent_char$css [L,QSA,T=text/css]\n";
    }
    if (BOOST_CACHE_JS) {
      $string .= "  RewriteCond $document_root$base_path$cache_dir$permanent_dir$server_name%{REQUEST_URI}$permanent_char$js -s\n";
      $string .= "  RewriteRule .* $cache_dir$permanent_dir$server_name%{REQUEST_URI}$permanent_char$js [L,QSA,T=text/javascript]\n";
    }
  }
  if (BOOST_CACHE_HTML || BOOST_CACHE_XML || BOOST_CACHE_JSON) {
    $skip = (int)BOOST_CACHE_HTML + (int)BOOST_CACHE_XML + (int)BOOST_CACHE_JSON;
    $skip = BOOST_GZIP ? $skip*2+1 : $skip;
    $string .= "\n";
    $string .= "  # Caching for anonymous users\n";
    $string .= "  # Skip boost IF not get request OR uri has wrong dir OR cookie is set OR request came from this server" . (variable_get('boost_ssl_bypass', TRUE) ? " OR https request" : "") . "\n";
    $string .= "  RewriteCond %{REQUEST_METHOD} !^(GET|HEAD)$ [OR]\n";
    $string .= "  RewriteCond %{REQUEST_URI} (^$base_path(admin|$cache_dir|misc|modules|sites|system|openid|themes|node/add|comment/reply))|(/(edit|user|user/(login|password|register))$) [OR]\n";
    $string .= BOOST_LOOPBACK_BYPASS && BOOST_OVERWRITE_FILE ? "  RewriteCond %{REMOTE_ADDR} ^$server_ip$ [OR]\n" : '';
    if (variable_get('boost_apache_rfc2616', FALSE)) {
      $string .= "  RewriteCond %{HTTP:Pragma} no-cache [OR]\n";
      $string .= "  RewriteCond %{HTTP:Cache-Control} no-cache [OR]\n";
    }
    if (variable_get('boost_ssl_bypass', TRUE)) {
      $string .= "  RewriteCond %{HTTPS} on [OR]\n";
    }
    $string .= "  RewriteCond %{HTTP_COOKIE} DRUPAL_UID\n";
    $string .= "  RewriteRule .* - [S=$skip]\n";
    if (BOOST_GZIP) {
      $skip = (int)BOOST_CACHE_HTML + (int)BOOST_CACHE_XML + (int)BOOST_CACHE_JSON;
      $string .= "\n";
      $string .= "  # GZIP\n";
      $string .= BOOST_AGGRESSIVE_GZIP ? "  RewriteCond %{HTTP_COOKIE} !(boost-gzip)\n" : '';
      $string .= "  RewriteCond %{HTTP:Accept-encoding} !gzip\n";
      $string .= "  RewriteRule .* - [S=$skip]\n";
      if (BOOST_CACHE_HTML) {
        $string .= "  RewriteCond $document_root$base_path$cache_dir$gzip_dir$server_name%{REQUEST_URI}$char%{QUERY_STRING}$html$gz -s\n";
        $string .= "  RewriteRule .* $cache_dir$gzip_dir$server_name%{REQUEST_URI}$char%{QUERY_STRING}$html$gz [L,T=text/html,E=no-gzip:1]\n";
      }
      if (BOOST_CACHE_XML) {
        $string .= "  RewriteCond $document_root$base_path$cache_dir$gzip_dir$server_name%{REQUEST_URI}$char%{QUERY_STRING}$xml$gz -s\n";
        $string .= "  RewriteRule .* $cache_dir$gzip_dir$server_name%{REQUEST_URI}$char%{QUERY_STRING}$xml$gz [L,T=text/xml,E=no-gzip:1]\n";
      }
      if (BOOST_CACHE_JSON) {
        $string .= "  RewriteCond $document_root$base_path$cache_dir$gzip_dir$server_name%{REQUEST_URI}$char%{QUERY_STRING}$json$gz -s\n";
        $string .= "  RewriteRule .* $cache_dir$gzip_dir$server_name%{REQUEST_URI}$char%{QUERY_STRING}$json$gz [L,T=text/javascript,E=no-gzip:1]\n";
      }
    }
    $string .= "\n";
    $string .= "  # NORMAL\n";
    if (BOOST_CACHE_HTML) {
      $string .= "  RewriteCond $document_root$base_path$cache_dir$normal_dir$server_name%{REQUEST_URI}$char%{QUERY_STRING}$html -s\n";
      $string .= "  RewriteRule .* $cache_dir$normal_dir$server_name%{REQUEST_URI}$char%{QUERY_STRING}$html [L,T=text/html]\n";
    }
    if (BOOST_CACHE_XML) {
      $string .= "  RewriteCond $document_root$base_path$cache_dir$normal_dir$server_name%{REQUEST_URI}$char%{QUERY_STRING}$xml -s\n";
      $string .= "  RewriteRule .* $cache_dir$normal_dir$server_name%{REQUEST_URI}$char%{QUERY_STRING}$xml [L,T=text/xml]\n";
    }
    if (BOOST_CACHE_JSON) {
      $string .= "  RewriteCond $document_root$base_path$cache_dir$normal_dir$server_name%{REQUEST_URI}$char%{QUERY_STRING}$json -s\n";
      $string .= "  RewriteRule .* $cache_dir$normal_dir$server_name%{REQUEST_URI}$char%{QUERY_STRING}$json [L,T=text/javascript]\n";
    }
  }
  $string .= "\n";
  $string .= "  ### BOOST END ###\n";

  return $string;
}

/**
 * Helper function to update robots.txt
 * Inserts or removes the line 'Disallow: /boost_stats.php'
 *
 * @TODO
 *   Make this work with subdirectory installs
 * @param $enable
 *   Whether to patch robots.txt to work with boost.
 * @return
 *   TRUE or nonzero on success, FALSE if error
 */
function boost_update_robots_txt($enable, $verbose = TRUE) {
  $payload = 'Disallow: /boost_stats.php';
  $filename = 'robots.txt';
  $result = TRUE;

  $text = file_get_contents($filename);
  if ($text) {
    if ($enable && !stristr($text, $payload)) {
      // Boost enabled: Insert payload into file
      $result = file_put_contents($filename, $text . "\n" . $payload . "\n");
      if ($verbose) {
        drupal_set_message($result ?
          t('robots.txt patched for usage with boost') :
          t('Unable to write to robots.txt')
        );
      }
    }
    elseif (!$enable && stristr($text, $payload)) {
      // Boost disabled: Remove payload from file
      $text = preg_replace('|^[^D]*Disallow:[ ]*/boost_stats.php[^\n]*[\n]|im', "", $text);
      $result = file_put_contents($filename, $text);
      if ($verbose) {
        drupal_set_message($result ?
                           t('robots.txt unpatched because boost is disabled') :
                           t('Unable to write to robots.txt'));
      }
    }
  }
  return $result;
}

/**
 * Helper function to update htaccess
 * Inserts or removes the autogenerated boost rules
 *
 * @TODO
 *   Make this work with subdirectory installs
 * @param $enable
 *   Whether to patch .htaccess to work with boost.
 * @return
 *   TRUE or nonzero on success, FALSE if error
 */
function boost_update_htaccess($enable = TRUE, $verbose = TRUE) {
  $filename = '.htaccess';
  $text = file_get_contents($filename);

  // Strip out the lines from BOOST START to BOOST END inclusive, if it exists
  $text = preg_replace('/^[^#]*### BOOST START ###.*### BOOST END ###[^\n]*[\n]*/ims', "\n", $text);

  // Inject the rules before '# Rewrite URLs of the form'
  if ($enable) {
    $rules = boost_admin_generate_htaccess();
    $text = preg_replace('/^[^#]*# Rewrite URLs of the form/im', "\n" . $rules . "$0", $text);
  }

  boost_htaccess_cache_dir_put();
  $result = file_put_contents($filename, $text);
  if ($verbose) {
    if ($result) {
      drupal_set_message($enable ? t('Boost rules written to .htaccess') : t('Boost rules removed from .htaccess'));
    }
    else {
      drupal_set_message(t('Unable to write to .htacess'), 'error');
    }
  }
}

/**
 * Counts the number of pages in the static cache.
 */
function boost_count_db($all = 0) {
  if ($all == 0) {
    return db_result(db_query("SELECT COUNT(*) FROM {boost_cache} WHERE expire <> 0 and base_dir = '%s'", BOOST_FILE_PATH));
  }
  elseif ($all == 1) {
    return db_result(db_query("SELECT COUNT(*) FROM {boost_cache} WHERE expire BETWEEN 1 AND %d AND base_dir ='%s'", BOOST_TIME, BOOST_FILE_PATH));
  }
  elseif ($all == 2) {
    return db_result(db_query("SELECT COUNT(*) FROM {boost_cache}"));
  }
}

/**
 * Count the number of files in a folder
 *
 * @param string $path
 *  Directory name, usually BOOST_ROOT_CACHE_DIR or /cache
 * @param float $timer
 *  Internal timer so function doesn't run forever
 * @param bool $first
 *  Indicator for top call in recursive function
 */
function boost_count_all_files($path, &$timer = NULL, $first = TRUE) {
  $files = 0;

  // Prevent timeouts
  if (is_null($timer)) {
    $timer = _boost_microtime_float();
  }
  if (_boost_microtime_float() - $timer > 7) {
    return $files;
  }

  // Get File Count
  $paths = glob($path . '*', GLOB_MARK|GLOB_ONLYDIR|GLOB_NOSORT);
  $files = count(glob($path . '*.*', GLOB_NOSORT));
  if ($paths) {
    foreach ($paths as $path) {
      $files += boost_count_all_files($path, $timer, FALSE);
    }
  }

  // If we ran out of time round and add a ++ to the file count
  if ($first && _boost_microtime_float() - $timer > 10) {
    return round($files, -2) . '++';
  }
  else {
    return $files;
  }
}

/**
 * Counts the number of pages in the core cache.
 */
function boost_count_core_db($all = FALSE) {
  return db_result(db_query("SELECT COUNT(*) FROM {cache_page}"));
}

/**
 * Flushes boost page cache
 */
function boost_clear_cache_submit($form, &$form_state, $first = TRUE) {
  // Temp fix for bug in form api & buttons
  // Check to make sure this button was actually called
  if (!boost_admin_button_router(0, $first, $form, $form_state)) {
    return FALSE;
  }

  $ignore = variable_get('boost_ignore_flush', 0);
  $GLOBALS['conf']['boost_ignore_flush'] = 0;
  if (boost_cache_clear_all()) {
    boost_clear_cache_parallel(BOOST_PERM_FILE_PATH);
    boost_clear_cache_parallel(BOOST_PERM_GZIP_FILE_PATH);
    drupal_set_message(t('Boost: Static page cache cleared.'));
  }
  else {
    drupal_set_message(t('Boost: Set "Ignore cache flushing:" to \'Disabled\' in the <a href="!link">boost advanced settings</a> & try again.', array('!link' => url('admin/settings/performance/boost', array('fragment' => 'edit-boost-ignore-flush-0-wrapper')))), 'warning');
  }
  $GLOBALS['conf']['boost_ignore_flush'] = $ignore;
}

function boost_clear_cache_parallel($dir) {
  $dirs = _boost_copy_file_get_domains($dir);
  foreach ($dirs as $directory) {
    _boost_rmdir_rf($directory, TRUE, TRUE);
  }
}

/**
 * Flushes all expired pages from database
 */
function boost_clear_expired_cache_submit($form, &$form_state, $first = TRUE) {
  // Temp fix for bug in form api & buttons
  // Check to make sure this button was actually called
  if (!boost_admin_button_router(1, $first, $form, $form_state)) {
    return FALSE;
  }

  $ignore = variable_get('boost_ignore_flush', 0);
  $GLOBALS['conf']['boost_ignore_flush'] = 0;
  if (boost_cache_expire_all()) {
    drupal_set_message(t('Boost: Expired stale files from static page cache.'));
  }
  else {
    drupal_set_message(t('Boost: Set "Ignore cache flushing:" to \'Disabled\' OR \'Only Ignore Complete Flushes\' in the <a href="@link">boost advanced settings</a> & try again.', array('@link' => url('admin/settings/performance/boost', array('fragment' => 'edit-boost-ignore-flush-0-wrapper')))), 'warning');
  }
  $GLOBALS['conf']['boost_ignore_flush'] = $ignore;
}

/**
 * Resets boost database & cache directory
 */
function boost_reset_database_file_submit($form, &$form_state, $first = TRUE) {
  // Temp fix for bug in form api & buttons
  // Check to make sure this button was actually called
  if (!boost_admin_button_router(4, $first, $form, $form_state)) {
    return FALSE;
  }

  $ignore = variable_get('boost_ignore_flush', 0);
  $GLOBALS['conf']['boost_ignore_flush'] = 0;
  if (boost_cache_clear_all()) {
    db_query("TRUNCATE {boost_cache}");
    db_query("TRUNCATE {boost_cache_relationships}");
    db_query("TRUNCATE {boost_crawler}");
    _boost_rmdir_rf(BOOST_ROOT_CACHE_DIR, TRUE, TRUE, TRUE);
    drupal_set_message(t('Boost: Static page cache & 3 database tables cleared.'));
  }
  else {
    drupal_set_message(t('Boost: Set "Ignore cache flushing:" to \'Disabled\' in the <a href="!link">boost advanced settings</a> & try again.', array('!link' => url('admin/settings/performance/boost', array('fragment' => 'edit-boost-ignore-flush-0-wrapper')))), 'warning');
  }
  $GLOBALS['conf']['boost_ignore_flush'] = $ignore;
}

/**
 * Stop Crawler
 */
function boost_stop_crawler_submit($form, &$form_state, $first = TRUE) {
  // Temp fix for bug in form api & buttons
  // Check to make sure this button was actually called
  if (!boost_admin_button_router(2, $first, $form, $form_state)) {
    return FALSE;
  }

  variable_set('boost_crawler_stopped', TRUE);
  variable_set('boost_crawler_number_of_threads', 0);
  drupal_set_message(t('Boost: Stop crawler signal given; all crawler threads should be stopped in less then %time', array('%time' => format_interval(BOOST_MAX_THREAD_TIME))));
}

/**
 * Reset Crawler
 */
function boost_reset_crawler_submit($form, &$form_state, $first = TRUE) {
  // Temp fix for bug in form api & buttons
  // Check to make sure this button was actually called
  if (!boost_admin_button_router(3, $first, $form, $form_state)) {
    return FALSE;
  }

  db_query("DELETE FROM {variable} WHERE name LIKE '%s%%'", 'boost_crawl');
  db_query("DELETE FROM {variable} WHERE name = 'cron_semaphore'");
  cache_clear_all('variables', 'cache');
  variable_set('boost_crawler_key', md5(mt_rand()));
  drupal_set_message(t('Boost: Crawler variables reset & cron semaphore cleared (cron reset)'));
}

/**
 * Returns all key/values in array that are equal.
 *
 * @param $needle
 *  What your searching for
 * @param $haystack
 *  Array of values
 * @param $a_not
 *  Optional array of key names to exclude
 */
function boost_array_find($needle, $haystack, $a_not = array()) {
  $out = array();
  foreach ($haystack as $key => $value) {
    if ($value == $needle) {
      $good = TRUE;
      foreach ($a_not as $not) {
        if (strpos($key, $not) !== FALSE) {
          $good = FALSE;
        }
      }
      if ($good) {
        $out[$key] = $value;
      }
    }
  }
  return $out;
}

/**
 * Returns the value of the key in a multidimensional array.
 *
 * http://php.net/array-key-exists#85184
 */
function &boost_array_find_element_by_key($key, &$form) {
  if (array_key_exists($key, $form)) {
    $ret =& $form[$key];
    return $ret;
  }
  foreach ($form as $k => $v) {
    if (is_array($v)) {
      $ret =& boost_array_find_element_by_key($key, $form[$k]);
      if ($ret) {
        return $ret;
      }
    }
  }
  return FALSE;
}

function boost_admin_button_router($number, $first, $form, &$form_state) {
  $value = boost_array_find_element_by_key('op', $form);
  if (strpos($value, (string)$number) != 1) {
    $correct = substr($value, 1, 1);
    if ($first) {
      watchdog('boost', t('FormAPI Bug Encountered; trying to work around the bug. Given: %given Wanted: %wanted. Are you running an old version of Drupal core? For more information, see: http://drupal.org/node/630000. Please describe which button was clicked and the complete error message.', array('%given' => $number, '%wanted' => $correct)));
      switch ($correct) {
        case 0:
          boost_clear_cache_submit($form, $form_state, FALSE);
          break;
        case 1:
          boost_clear_expired_cache_submit($form, $form_state, FALSE);
          break;
        case 2:
          boost_stop_crawler_submit($form, $form_state, FALSE);
          break;
        case 3:
          boost_reset_crawler_submit($form, $form_state, FALSE);
          break;
        case 4:
          boost_reset_database_file_submit($form, $form_state, FALSE);
          break;
      }
    }
    else {
      drupal_set_message(t('FormAPI Bug Encountered; try again. Given: %given Wanted: %wanted. Are you running an old version of Drupal core? For more information, see: http://drupal.org/node/630000. Please describe which button was clicked and the complete error message.', array('%given' => $number, '%wanted' => $correct)), 'error');
    }
    return FALSE;
  }
  return TRUE;
}
