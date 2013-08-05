<?php

/**
 * @file
 * Administration settings for iTweak Upload module
 */

/*
 * Collect list of available presets for image options.
 */
function _itweak_upload_setting_image_options($default_text = NULL) {
  $ret = array();
  if ($default_text) $ret['_default'] = $default_text;
  $ret['_none'] = t('No preview');
  $ret['_original'] = t('Full image');
  if (module_exists('imagecache')) {
    foreach (imagecache_presets() as $preset_id => $preset) {
      $ret[$preset_id] = $preset['presetname'];
    }
  }
  else {
    drupal_set_message(t('Imagecache module is not installed / enabled. Previously configured attachement preview images will be disabled. Use of "Full image" will impact website performance.'));
  }
  return $ret;
}

/*
 * Collect list of available handlers for link options.
 */
function _itweak_upload_setting_link_options($default_text = NULL) {
  $ret = array();
  if ($default_text) $ret['_default'] = $default_text;
  $ret['none'] = t('Open image');
  if (module_exists('lightbox2')) {
    $ret['lightbox2'] = t('Lightbox');
    $ret['lightbox2grouped'] = t('Lightbox Grouped');
    $ret['lightbox2slideshow'] = t('Lightbox Slideshow');
  }
  if (module_exists('colorbox')) {
    $ret['colorbox'] = t('Colorbox');
  }
  if (module_exists('thickbox')) {
    $ret['thickbox'] = t('Thickbox');
  }
  if (module_exists('fancybox')) {
    $ret['fancybox'] = t('Fancybox');
  }
  if (module_exists('shadowbox')) {
    $ret['shadowbox'] = t('Shadowbox');
    $ret['shadowboxgrouped'] = t('Shadowbox Grouped');
  }
  if (module_exists('highslide')) {
    $ret['highslide'] = t('Highslide');
    $ret['highslidegrouped'] = t('Highslide Grouped');
  }
  return $ret;
}

/*
 * Collect list of available handlers for gallery type options.
 */
function _itweak_upload_gallery_type_options($default_text = NULL) {
  $ret = array();
  if ($default_text) $ret['_default'] = $default_text;
  $ret['none'] = t('iTweak Upload');
  if (module_exists('jcarousel')) {
    $ret['jcarousel'] = t('jCarousel');
  }
  if (module_exists('jcarousellite')) {
    $ret['jcarousellite'] = t('jCarousel Lite');
  }
  return $ret;
}

/* jCarousel Lite additional options
          '#description' => t("Additional options. See !link for more detail.<br /><code>
btnGo: ['.externalControl .1', '.externalControl .2', '.externalControl .3'],<br />
auto: null,<br />
speed: 200,<br />
vertical: false,<br />
circular: true,<br />
visible: 3,<br />
start: 0,<br />
scroll: 1,
</code>", array('!link' => l('documentation', 'http://www.gmarwaha.com/jquery/jcarousellite/index.php', array('target' => '_blank', 'fragment' => 'doc')))),
*/

/**
 * Pre-render callback to adjust weights of fields.
 */
function _itweak_upload_node_type_alter_weights($elements) {
  if (isset($elements['workflow']) && isset($elements['workflow']['#weight'])
    && isset($elements['itweak_upload_display'])) {
    $elements['itweak_upload_display']['#weight'] = $elements['workflow']['#weight'] + 0.001;
    unset($elements['#sorted']);
  }
  if (isset($elements['workflow']) && isset($elements['workflow']['upload']) && isset($elements['workflow']['upload']['#weight'])
    && isset($elements['workflow']['itweak_upload_collapse'])) {
    $elements['workflow']['itweak_upload_collapse']['#weight'] = $elements['workflow']['upload']['#weight'] + 0.001;
    unset($elements['#sorted']);
    unset($elements['workflow']['#sorted']);
  }
  // Hide "Image attachments on comments" setting - it is ignored anyway.
  unset($elements['comment']['comment_upload_images']);
  return $elements;
}

/**
 * Pre-render callback to adjust weights of fields.
 */
function _itweak_upload_admin_settings_alter_weights($elements) {
  if (isset($elements['settings_general']) && isset($elements['settings_general']['#weight'])
    && isset($elements['itweak_upload'])) {
    $elements['itweak_upload']['#weight'] = $elements['settings_general']['#weight'] + 0.001;
    unset($elements['#sorted']);
  }
  return $elements;
}

/**
 * Administration settings form.
 *
 * @return
 *   The completed form definition.
 */
function itweak_upload_admin_settings() {
  $form = array();
  _itweak_upload_admin_settings($form);
  return system_settings_form($form);
}

/**
 * Administration settings form worker code.
 */
function _itweak_upload_admin_settings(&$form) {
  $link_options = _itweak_upload_setting_link_options();
  $image_options = _itweak_upload_setting_image_options();
  $image_option_default = _itweak_upload_encode_derivative();
  $gallery_type_options = _itweak_upload_gallery_type_options();

  $form['itweak_upload'] = array(
    '#type' => 'fieldset',
    '#title' => t('Attachments display'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#description' => t('These global settings apply to the display of attachments in all site content by default. Each content type can override these settings.'),
  );
  // Register our own form after_build function to order our fieldset
//  if (!isset($form['#pre_render'])) $form['#pre_render'] = array();
  $form['#pre_render'][] = '_itweak_upload_admin_settings_alter_weights';

  $form['itweak_upload']['itweak_upload_progress_indicator'] = array(
    '#type' => 'radios',
    '#title' => t('Progress indicator'),
    '#options' => array(
      'bar' => t('Bar with progress meter'),
      'throbber' => t('Throbber'),
    ),
    '#default_value' => variable_get('itweak_upload_progress_indicator', 'bar'),
    '#description' => t('Your server supports upload progress capabilities. The "throbber" display does not indicate progress but takes up less room on the form, you may want to use it if you\'ll only be uploading small files or if experiencing problems with the progress bar.'),
    '#weight' => 5,
    '#access' => itweak_upload_progress_implementation(),
  );

  $form['itweak_upload']['itweak_upload_thumbnail_preset_default'] = array(
    '#type' => 'select',
    '#title' => t('Default preset for image thumbnail'),
    '#default_value' => variable_get('itweak_upload_thumbnail_preset_default', $image_option_default),
    '#options' => $image_options,
    '#description' => t('This preset will be used by default for image thumbnails. This setting can be overridden on each content type configuration page.')
      . (user_access('administer imagecache')
        ? ' ' . t('Presets can be edited on <a href="!link">ImageCache</a> page.',
            array('!link' => '/admin/build/imagecache'))
        : ''),
  );
  $form['itweak_upload']['itweak_upload_thumbnail_link_default'] = array(
    '#type' => 'select',
    '#title' => t('Default thumbnail link open mode'),
    '#default_value' => variable_get('itweak_upload_thumbnail_link_default', _itweak_upload_setting_link_default()),
    '#options' => $link_options,
    '#description' => t('This mode will be used by default for opening image thumbnails. This setting can be overridden on each content type configuration page.'),
  );
  $form['itweak_upload']['itweak_upload_thumbnail_title_default'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show file name on the opened thumbnail link'),
    '#default_value' => variable_get('itweak_upload_thumbnail_title_default', 1),
    '#description' => t('Check to show by default the file name on the image opened from thumbnails (only if selected link open mode supports it).')
  );

  $form['itweak_upload']['itweak_upload_gallery_type_default'] = array(
    '#type' => 'select',
    '#title' => t('Default image gallery type'),
    '#default_value' => variable_get('itweak_upload_gallery_type_default', _itweak_upload_gallery_type_default()),
    '#options' => $gallery_type_options,
    '#description' => t('This gallery type will be used by default for image thumbnails. This setting can be overridden on each content type configuration page.'),
  );
  _itweak_upload_insert_form($form['itweak_upload'], 'default');
}

/**
 * Theme the output of the styles list on the settings form -
 * replacement for insert_field_widget_settings_styles with previews.
 */
function theme_itweak_upload_field_widget_settings_styles($element) {
  drupal_add_js('misc/tableselect.js');

  $header = array(
    array('class' => 'select-all', 'data' => ' ' . t('Select all')),
    array('data' => t('Preview')),
  );

  $rows = array();
  foreach ($element['#options'] as $key => $label) {
    $row = array();
    $row[] = drupal_render($element[$key]);
    $row[] = $element['#previews'][$key];
    $rows[] = $row;
  }
  return theme('table', $header, $rows);
}

/*
 *  Insert options form builder
 */
function _itweak_upload_insert_form(&$form, $node_type) {
  if (ITU_USE_INSERT_MODULE && module_exists('insert')) {
    $widget = _itweak_upload_get_insert_widget_settings($node_type);
    $insert = insert_widget_form($widget);
    $suffix = ($node_type == 'default') ? '_default' : '';  // Node type form adds type automatically
    $first = TRUE;
    foreach ($insert['insert'] as $name => $field) {
      if (substr($name, 0,1) == '#')
        $form['insert'][$name] = $field;  // Copy the fieldset
      else {
        if ($node_type != 'default' && $first) {
          // Inject custom field
          $first = FALSE;
          $form['#pre_render'][] = '_itweak_upload_insert_js';
          $form['insert']['itweak_upload_insert_override_default'] = array(
            '#type' => 'checkbox',
            '#title' => t('Use custom insert settings for %node_type content', array('%node_type' => $node_type)),
            '#default_value' => variable_get('itweak_upload_insert_override_default_' . $node_type, 0),
            '#description' => t('Check to override site-wide settings (enables the below configuration).')
          );
        }
        if ($name == 'insert') {
          $name = 'insert_enable';  // This avoids naming hole in _itweak_upload_get_setting()
        }

        if ($name == 'insert_styles') {

if (1) { // Experimental: Preview formats
          // Insert module limits file links to the default/files/ directory. Instead of
          // copying our example image file there, we prepare the link as if it is in
          // default/files/ directory. Then we replace the string
          $fname = 'example.jpg';
          $seek = insert_create_url($fname);
          $repl = url(drupal_get_path('module', 'itweak_upload') . '/images/' . $fname);
          $item = array(
            'filepath' => $fname,
            'filename' => $fname,
          ); // Build a pretend item
          $widget = _itweak_upload_get_insert_widget_settings('default');
          $field['#previews'] = array();
          foreach ($field['#options'] as $style_name => $description) {
            $style = insert_style_load($style_name);
            $value = insert_content($item, $style, $widget);
            // Replace directory with module's path
            $field['#previews'][$style_name] = str_replace ( $seek, $repl, $value);;
          }
          $field['#theme'] = 'itweak_upload_field_widget_settings_styles';
//watchdog('debug','in itu_upload_insert_form, widget='.iprintr($widget).' insert_styles='.iprintr($field));
}  // END Experimental

          // Wrap the list of styles into a fieldset
          $field = array (
            '#type' => 'fieldset',
            '#title' => t('Insert styles'),
            '#collapsible' => TRUE,
            '#collapsed' => TRUE,
//            '#description' => t(''),
            'itweak_upload_' . $name . $suffix => $field,
          );
          $name .= '_fieldset';
          // Move the list of styles to the end
          $field['#weight'] = 10;
        }

        $form['insert']['itweak_upload_' . $name . $suffix] = $field;
      }
    }  // foreach
    unset($form['insert']['#weight']);

    if (isset($form['insert']['itweak_upload_insert_class' . $suffix])) {
      $form['insert']['itweak_upload_insert_class' . $suffix]['#description'] .=
        ' ' . t('Pre-defined class %cl is provided with button-like CSS styling.', array('%cl' => 'itu-insert-file'));
    }
  }
}

/**
 * Prerender callback - just to load javascript on demand (works on cached forms).
 */
function _itweak_upload_insert_js($form) {
  drupal_add_js(drupal_get_path('module', 'itweak_upload') . '/itweak_upload.insert.js');
  return $form;
}

/**
 * Node type settings form.
 */
function _itweak_upload_node_type_form(&$form) {
  $node_type = (isset($form['old_type']) && isset($form['old_type']['#value'])) ? $form['old_type']['#value'] : NULL;

  $display_options = array(
//    '_default' => t('(Site default)'),
    0 => t('Disabled'),
    1 => t('Show files list (images included as files)'),
    2 => t('Show files list with image thumbnails'),
    3 => t('Show files list and image gallery'),
    4 => t('Show image gallery only')
  );

  $image_options = _itweak_upload_setting_image_options( t('(Site default)') );
  $image_option_default = '_default';

  $link_options = _itweak_upload_setting_link_options(t('(Site default)'));
  $gallery_type_options = _itweak_upload_gallery_type_options(t('(Site default)'));

  if (isset($form['workflow'])) {
    $form['workflow']['itweak_upload_collapse'] = array(
      '#type' => 'radios',
      '#title' => t('Collapse state of the Attachments box in <em>!type</em>', array('!type' => $node_type)),
      '#default_value' => variable_get('itweak_upload_collapse_' . $node_type, 0),
      '#options' => array(
        t('Not collapsible'),
        t('Collapsible'),
        t('Collapsed'),
      ),
      '#description' => t('Select collapsible mode for "Attach files" box of this type.')
    );
  }

  $form['itweak_upload_display'] = array(
    '#type' => 'fieldset',
    '#title' => t('Attachments display'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#description' => t('These settings apply only to the display of all attachments in <em>!type</em> content. They do not change the settings that enable or allow attachments and comments.', array('!type' => $node_type)),
  );
  // Register our own form after_build function to order our fieldset
//  if (!isset($form['#pre_render'])) $form['#pre_render'] = array();
  $form['#pre_render'][] = '_itweak_upload_node_type_alter_weights';

  $form['itweak_upload_display']['itweak_upload_thumbnail_preset_default'] = array(
    '#type' => 'select',
    '#title' => t('Default preset for image thumbnail'),
    '#default_value' => variable_get('itweak_upload_thumbnail_preset_default_' . $node_type, $image_option_default),
    '#options' => $image_options,
    '#description' => t('This preset will be used by default for image thumbnails. Individual settings can be done below.')
      . (user_access('administer imagecache')
        ? ' ' . t('Presets can be edited on <a href="!link">ImageCache</a> page.',
            array('!link' => '/admin/build/imagecache'))
        : ''),
  );
  $form['itweak_upload_display']['itweak_upload_thumbnail_link_default'] = array(
    '#type' => 'select',
    '#title' => t('Default thumbnail link open mode'),
    '#default_value' => variable_get('itweak_upload_thumbnail_link_default_' . $node_type, '_default'),
    '#options' => $link_options,
    '#description' => t('This mode will be used by default for opening image thumbnails. Individual settings can be done below.'),
  );
  $form['itweak_upload_display']['itweak_upload_gallery_type_default'] = array(
    '#type' => 'select',
    '#title' => t('Default image gallery type'),
    '#default_value' => variable_get('itweak_upload_gallery_type_default_' . $node_type, '_default'),
    '#options' => $gallery_type_options,
    '#description' => t('This gallery type will be used by default for image thumbnails. Individual settings can be done below.'),
  );

  // Change default wordings for the rest of the form
  $image_options['_default'] = t('(Default)');
  $link_options['_default'] = t('(Default)');
  $gallery_type_options['_default'] = t('(Default)');

  $form['itweak_upload_display']['upload'] = array(
    '#type' => 'fieldset',
    '#title' => t('Upload form'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#description' => t('These settings apply to the upload attachments form in <em>!type</em> content. They do not change the settings that enable or allow attachments and comments.', array('!type' => $node_type)),
  );
  $form['itweak_upload_display']['upload']['itweak_upload_upload_preview'] = array(
    '#type' => 'checkbox',
    '#title' => t('Preview uploaded attachments while editing  <em>!type</em>', array('!type' => $node_type)),
    '#default_value' => variable_get('itweak_upload_upload_preview_' . $node_type, 1),
    '#description' => t('Check to enable attachments preview while editing nodes of this type.')
  );
  $form['itweak_upload_display']['upload']['itweak_upload_thumbnail_preset_upload'] = array(
    '#type' => 'select',
    '#title' => t('Preset for image thumbnail'),
    '#default_value' => variable_get('itweak_upload_thumbnail_preset_upload_' . $node_type, $image_option_default),
    '#options' => $image_options,
    '#description' => t('This preset will be used for image thumbnails.')
      . (user_access('administer imagecache')
        ? ' ' . t('Presets can be edited on <a href="!link">ImageCache</a> page.',
            array('!link' => '/admin/build/imagecache'))
        : ''),
  );
  $form['itweak_upload_display']['upload']['itweak_upload_thumbnail_link_upload'] = array(
    '#type' => 'select',
    '#title' => t('Thumbnail link open mode'),
    '#default_value' => variable_get('itweak_upload_thumbnail_link_upload_' . $node_type, '_default'),
    '#options' => $link_options,
    '#description' => t('This mode will be used for opening image thumbnails.'),
  );
  _itweak_upload_insert_form($form['itweak_upload_display'], $node_type);

  $form['itweak_upload_display']['node'] = array(
    '#type' => 'fieldset',
    '#title' => t('Node view'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#description' => t('These settings apply to the attachments display in <em>!type</em> content.', array('!type' => $node_type)),
  );
  $form['itweak_upload_display']['node']['itweak_upload_node_display'] = array(
    '#type' => 'radios',
    '#title' => t('Attachments display in <em>!type</em>', array('!type' => $node_type)),
    '#default_value' => variable_get('itweak_upload_node_display_' . $node_type, 2),
    '#options' => $display_options,
    '#description' => t('Select display mode for attached files and images for nodes of this type.')
  );
  $form['itweak_upload_display']['node']['itweak_upload_thumbnail_preset_node'] = array(
    '#type' => 'select',
    '#title' => t('Preset for image thumbnail'),
    '#default_value' => variable_get('itweak_upload_thumbnail_preset_node_' . $node_type, $image_option_default),
    '#options' => $image_options,
    '#description' => t('This preset will be used for image thumbnails.')
      . (user_access('administer imagecache')
        ? ' ' . t('Presets can be edited on <a href="!link">ImageCache</a> page.',
            array('!link' => '/admin/build/imagecache'))
        : ''),
  );
  $form['itweak_upload_display']['node']['itweak_upload_thumbnail_link_node'] = array(
    '#type' => 'select',
    '#title' => t('Thumbnail link open mode'),
    '#default_value' => variable_get('itweak_upload_thumbnail_link_node_' . $node_type, '_default'),
    '#options' => $link_options,
    '#description' => t('This mode will be used for opening image thumbnails.'),
  );
  $form['itweak_upload_display']['node']['itweak_upload_gallery_type_node'] = array(
    '#type' => 'select',
    '#title' => t('Image gallery type'),
    '#default_value' => variable_get('itweak_upload_gallery_type_node_' . $node_type, '_default'),
    '#options' => $gallery_type_options,
    '#description' => t('This gallery type will be used for image thumbnails.'),
  );

  $form['itweak_upload_display']['teaser'] = array(
    '#type' => 'fieldset',
    '#title' => t('Teaser view'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#description' => t('These settings apply to the attachments display in <em>!type</em> teaser.', array('!type' => $node_type)),
  );
  $form['itweak_upload_display']['teaser']['itweak_upload_teaser_display'] = array(
    '#type' => 'radios',
    '#title' => t('Attachments display in <em>!type</em> teaser', array('!type' => $node_type)),
    '#default_value' => variable_get('itweak_upload_teaser_display_' . $node_type, 0),
    '#options' => $display_options,
    '#description' => t('Select display mode for attached files and images in teaser for nodes of this type.')
  );
  $form['itweak_upload_display']['teaser']['itweak_upload_teaser_images'] = array(
    '#type' => 'textfield',
    '#title' => t('Number of images shown in <em>!type</em> teaser gallery', array('!type' => $node_type)),
    '#default_value' => variable_get('itweak_upload_teaser_images_' . $node_type, 0),
    '#size' => 5,
    '#maxlength' => 2,
    '#description' => t('The maximum number of images to show in the node teaser. Only applies when <em>Gallery</em> image display is selected. 0 will not show images. Leave blank to show all images.'),
  );
  $form['itweak_upload_display']['teaser']['itweak_upload_thumbnail_preset_teaser'] = array(
    '#type' => 'select',
    '#title' => t('Preset for image thumbnail'),
    '#default_value' => variable_get('itweak_upload_thumbnail_preset_teaser_' . $node_type, $image_option_default),
    '#options' => $image_options,
    '#description' => t('This preset will be used for image thumbnails.')
      . (user_access('administer imagecache')
        ? ' ' . t('Presets can be edited on <a href="!link">ImageCache</a> page.',
            array('!link' => '/admin/build/imagecache'))
        : ''),
  );
  $form['itweak_upload_display']['teaser']['itweak_upload_thumbnail_link_teaser'] = array(
    '#type' => 'select',
    '#title' => t('Thumbnail link open mode'),
    '#default_value' => variable_get('itweak_upload_thumbnail_link_teaser_' . $node_type, '_default'),
    '#options' => $link_options,
    '#description' => t('This mode will be used for opening image thumbnails.'),
  );
  $form['itweak_upload_display']['teaser']['itweak_upload_gallery_type_teaser'] = array(
    '#type' => 'select',
    '#title' => t('Image gallery type'),
    '#default_value' => variable_get('itweak_upload_gallery_type_teaser_' . $node_type, '_default'),
    '#options' => $gallery_type_options,
    '#description' => t('This gallery type will be used for image thumbnails.'),
  );

  if (module_exists('comment_upload')) {
    $form['itweak_upload_display']['comment'] = array(
      '#type' => 'fieldset',
      '#title' => t('Comments'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
      '#description' => t('These settings apply to the attachments display in <em>!type</em> comments.', array('!type' => $node_type)),
    );
    if (module_exists('nodecomment')) {
      $form['itweak_upload_display']['comment']['#description'] .= '<br>'
       . t('(Applicable only to normal Drupal comments - provided by core comment module. If Nodecomment module is selected for comments, attachments should be configured in the corresponding content type.)');
    }
    $form['itweak_upload_display']['comment']['itweak_upload_comment_display'] = array(
      '#type' => 'radios',
      '#title' => t('Attachments display in <em>!type</em> comments', array('!type' => $node_type)),
      '#default_value' => variable_get('itweak_upload_comment_display_' . $node_type, 2),
      '#options' => $display_options,
      '#description' => t('Select display mode for attached files and images in comments for nodes of this type.')
    );
    $form['itweak_upload_display']['comment']['itweak_upload_thumbnail_preset_comment'] = array(
      '#type' => 'select',
      '#title' => t('Preset for image thumbnail'),
      '#default_value' => variable_get('itweak_upload_thumbnail_preset_comment_' . $node_type, $image_option_default),
      '#options' => $image_options,
      '#description' => t('This preset will be used for image thumbnails.')
        . (user_access('administer imagecache')
          ? ' ' . t('Presets can be edited on <a href="!link">ImageCache</a> page.',
              array('!link' => '/admin/build/imagecache'))
          : ''),
    );
    $form['itweak_upload_display']['comment']['itweak_upload_thumbnail_link_comment'] = array(
      '#type' => 'select',
      '#title' => t('Thumbnail link open mode'),
      '#default_value' => variable_get('itweak_upload_thumbnail_link_comment_' . $node_type, '_default'),
      '#options' => $link_options,
      '#description' => t('This mode will be used for opening image thumbnails.'),
    );
    $form['itweak_upload_display']['comment']['itweak_upload_gallery_type_comment'] = array(
      '#type' => 'select',
      '#title' => t('Image gallery type'),
      '#default_value' => variable_get('itweak_upload_gallery_type_comment_' . $node_type, '_default'),
      '#options' => $gallery_type_options,
      '#description' => t('This gallery type will be used for image thumbnails.'),
    );
  }
}

