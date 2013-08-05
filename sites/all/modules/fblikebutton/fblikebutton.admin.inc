<?php
// $Id$

/**
 * @file
 * Admin functions for fblikebutton.
 */

/**
 * Settings for the dynamic FB button in full node viewmode and teaser viewmode.
 */
function fblikebutton_dynamic_settings() {
  $fblikebutton_node_options = node_get_types('names');
  $form['fblikebutton_dynamic_description'] = array(
    '#markup' => '<p>' . t('Configure the dynamic Like button. This Like button will like the URL you\'r visiting. You can set the content types on which the button displays, choose to display it in the content block or it\'s own block and set the appearance.') . '</p>',
  );
  $form['fblikebutton_dynamic_visibility'] = array(
    '#type' => 'fieldset',
    '#title' => t('Visibility settings'),
    '#collapsible' => FALSE,
  );
  $form['fblikebutton_dynamic_visibility']['fblikebutton_node_types'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Display the Like button on these content types:'),
    '#options' => $fblikebutton_node_options,
    '#default_value' => variable_get('fblikebutton_node_types', array('article')),
    '#description' => t('Each of these content types will have the "like" button automatically added to them.'),
  );
  $form['fblikebutton_dynamic_visibility']['fblikebutton_full_node_display'] = array(
    '#type' => 'radios',
    '#title' => t('Where do you want to show the Like button (full node view)?'),
    '#options' => array(t('Content area'), t('Own block'), t('Links area')),
    '#default_value' => variable_get('fblikebutton_full_node_display', 0),
    '#description' => t('If <em>Content area</em> is selected, the button will appear in the same area as the node content. If <em>Own block</em> is selected the Like button gets it\'s own block, which you can position at the ' . l(t('block page'), 'admin/structure/block') . '. When you select <em>Links area</em> the Like button will be visible in the links area, usually at the bottom of the node (When you select this last option you may want to adjust the Appearance settings).'),
  );
  $form['fblikebutton_dynamic_visibility']['fblikebutton_teaser_display'] = array(
    '#type' => 'radios',
    '#title' => t('Where do you want to show the Like button on teasers?'),
    '#options' => array(t('Don\'t show on teasers'), t('Content area'), t('Links area')),
    '#default_value' => variable_get('fblikebutton_teaser_display', 0),
    '#description' => t('If you want to show the like button on teasers you can select the display area.'),
  );
  $form['fblikebutton_dynamic_appearance'] = array(
    '#type' => 'fieldset',
    '#title' => t('Appearance settings'),
    '#collapsible' => FALSE,
  );
  $form['fblikebutton_dynamic_appearance']['fblikebutton_iframe_width'] = array(
    '#type' => 'textfield',
    '#title' => t('Width of the iframe (px)'),
    '#default_value' => variable_get('fblikebutton_iframe_width', '450'),
    '#description' => t('Width of the iframe, in pixels. Default is 450. <em>Note: lower values may crop the output.</em>'),
  );
  $form['fblikebutton_dynamic_appearance']['fblikebutton_iframe_height'] = array(
    '#type' => 'textfield',
    '#title' => t('Height of the iframe (px)'),
    '#default_value' => variable_get('fblikebutton_iframe_height', '80'),
    '#description' => t('Height of the iframe, in pixels. Default is 80. <em>Note: lower values may crop the output.</em>'),
  );
  $form['fblikebutton_dynamic_appearance']['fblikebutton_iframe_css'] = array(
    '#type' => 'textfield',
    '#title' => t('Extra css styling needed'),
    '#default_value' => variable_get('fblikebutton_iframe_css', ''),
    '#description' => t('Extra css attributes needed to make the iframe behave for your specific requirements. Will not necessarily overwrite existing styling. To alter the dimensions of the iframe, use the height and width fields found above.<br/>Example: <em>float: right; padding: 5px;</em>'),
  );
  $form['fblikebutton_dynamic_appearance']['fblikebutton_layout'] = array(
    '#type' => 'select',
    '#title' => t('Layout style'),
    '#options' => array('standard' => t('Standard'), 'box_count' => t('Box Count'), 'button_count' => t('Button Count')),
    '#default_value' => variable_get('fblikebutton_layout', 'standard'),
    '#description' => t('Determines the size and amount of social context next to the button.'),
  );
  // The actial values passed in from the options will be converted to a boolean
  // in the validation function, so it doesn't really matter what we use.
  $form['fblikebutton_dynamic_appearance']['fblikebutton_show_faces'] = array(
    '#type' => 'select',
    '#title' => t('Show faces in the box?'),
    '#options' => array('show' => t('Show faces'), 'hide' => t('Do not show faces')),
    '#default_value' => variable_get('fblikebutton_show_faces', TRUE) ? 'show' : 'hide',
    '#description' => t('Show profile pictures below the button. Only works if <em>Layout style</em> (found above) is set to <em>Standard</em> (otherwise, value is ignored).'),
  );
  $form['fblikebutton_dynamic_appearance']['fblikebutton_action'] = array(
    '#type' => 'select',
    '#title' => t('Verb to display'),
    '#options' => array('like' => t('Like'), 'recommend' => t('Recommend')),
    '#default_value' => variable_get('fblikebutton_action', 'like'),
    '#description' => t('The verbiage to display inside the button itself.'),
  );
/**  $form['fblikebutton_dynamic_appearance']['fblikebutton_displaysend'] = array(
    '#type' => 'select',
    '#title' => t('Display <em>send</em> option?'),
    '#options' => array('true' => t('Display'), 'FALSE' => t('Do not display')),
    '#default_value' => variable_get('fblikebutton_displaysend', 'FALSE'),
    '#description' => t('Optionally display the <em>Send</em> button next to the <em>Like/Recommend</em> box.<br/><strong>Note: this feature is currently unavailable from Facebook.</strong> Although setting the value to <em>Display</em> will not hurt anything, the output will not be affected either way.'),
  ); */
  $form['fblikebutton_dynamic_appearance']['fblikebutton_font'] = array(
    '#type' => 'select',
    '#title' => t('Font'),
    '#options' => array(
      'arial' => 'Arial',
      'lucida+grande' => 'Lucida Grande',
      'segoe+ui' => 'Segoe UI',
      'tahoma' => 'Tahoma',
      'trebuchet+ms' => 'Trebuchet MS',
      'verdana' => 'Verdana',
    ),
    '#default_value' => variable_get('fblikebutton_font', 'arial'),
    '#description' => t('The font with which to display the text of the button.'),
  );
  $form['fblikebutton_dynamic_appearance']['fblikebutton_color_scheme'] = array(
    '#type' => 'select',
    '#title' => t('Color scheme'),
    '#options' => array('light' => t('Light'), 'dark' => t('Dark')),
    '#default_value' => variable_get('fblikebutton_color_scheme', 'light'),
    '#description' => t('The color scheme of the box environtment.'),
  );
  $form['fblikebutton_dynamic_appearance']['fblikebutton_weight'] = array(
    '#type' => 'select',
    '#title' => t('Weight'),
    // Note to self: THIS has GOT to change...
    '#options' => array('-50' => '-50', '-49' => '-49', '-48' => '-48', '-47' => '-47', '-46' => '-46', '-45' => '-45', '-44' => '-44', '-43' => '-43', '-42' => '-42', '-41' => '-41', '-40' => '-40', '-39' => '-39', '-38' => '-38', '-37' => '-37', '-36' => '-36', '-35' => '-35', '-34' => '-34', '-33' => '-33', '-32' => '-32', '-31' => '-31', '-30' => '-30', '-29' => '-29', '-28' => '-28', '-27' => '-27', '-26' => '-26', '-25' => '-25', '-24' => '-24', '-23' => '-23', '-22' => '-22', '-21' => '-21', '-20' => '-20', '-19' => '-19', '-18' => '-18', '-17' => '-17', '-16' => '-16', '-15' => '-15', '-14' => '-14', '-13' => '-13', '-12' => '-12', '-11' => '-11', '-10' => '-10', '-9' => '-9', '-8' => '-8', '-7' => '-7', '-6' => '-6', '-5' => '-5', '-4' => '-4', '-3' => '-3', '-2' => '-2', '-1' => '-1', '0' => '0', '1' => '1', '2' => '2', '3' => '3', '4' => '4', '5' => '5', '6' => '6', '7' => '7', '8' => '8', '9' => '9', '10' => '10', '11' => '11', '12' => '12', '13' => '13', '14' => '14', '15' => '15', '16' => '16', '17' => '17', '18' => '18', '19' => '19', '20' => '20', '21' => '21', '22' => '22', '23' => '23', '24' => '24', '25' => '25', '26' => '26', '27' => '27', '28' => '28', '29' => '29', '30' => '30', '31' => '31', '32' => '32', '33' => '33', '34' => '34', '35' => '35', '36' => '36', '37' => '37', '38' => '38', '39' => '39', '40' => '40', '41' => '41', '42' => '42', '43' => '43', '44' => '44', '45' => '45', '46' => '46', '47' => '47', '48' => '48', '49' => '49', '50' => '50'),
    '#default_value' => variable_get('fblikebutton_weight', '50'),
    '#description' => t('The weight determines where, at the content block, the like button will appear. The larger the weight, the lower it will appear on the node. For example, if you want the button to appear more toward the top of the node, choose <em>-40</em> as opposed to <em>-39, -38, 0, 1,</em> or <em>50,</em> etc. To position the Like button in its own block, go to the ' . l(t('block page'), 'admin/structure/block') . '.'),
    );
  $form['fblikebutton_dynamic_appearance']['fblikebutton_language'] = array(
    '#type' => 'textfield',
    '#title' => t('Language'),
    '#default_value' => variable_get('fblikebutton_language', 'en_US'),
    '#description' => t('Specific language to use. Default is English. Examples:<br />French (France): <em>fr_FR</em><br />French (Canada): <em>fr_CA</em><br />More information can be found at http://developers.facebook.com/docs/internationalization/ and a full XML list can be found at http://www.facebook.com/translations/FacebookLocales.xml'),
  );
  $form['#validate'] = array('fblikebutton_config_form_validate');
  return system_settings_form($form);
}

/**
 * Settings for the static FB Like button block.
 */
function fblikebutton_static_settings() {
  global $base_url;
  $form['fblikebutton_static_block_description'] = array(
    '#markup' => '<p>' . t('Set the static URL to like with the button. This Like button will like the given URL, no matter on which page it is displayed. To position this block go the ' . l(t('block page'), 'admin/structure/block') . '.</p>'),
  );
  $form['fblikebutton_static_block_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Button settings'),
    '#collapsible' => FALSE,
  );
  $form['fblikebutton_static_block_settings']['fblikebutton_block_url'] = array(
    '#type' => 'textfield',
    '#default_value' => variable_get('fblikebutton_block_url', $base_url),
    '#description' => t('URL of the page to like (could be your homepage or a facebook page e.g.)')
  );
  $form['fblikebutton_static_block_appearance'] = array(
    '#type' => 'fieldset',
    '#title' => t('Button appearance'),
    '#collapsible' => FALSE,
  );
  $form['fblikebutton_static_block_appearance']['fblikebutton_bl_layout'] = array(
    '#type' => 'select',
    '#title' => t('Layout style'),
    '#options' => array('standard' => t('Standard'), 'box_count' => t('Box Count'), 'button_count' => t('Button Count')),
    '#default_value' => variable_get('fblikebutton_bl_layout', 'standard'),
    '#description' => t('Determines the size and amount of social context next to the button'),
  );
  // The actial values passed in from the options will be converted to a boolean
  // in the validation function, so it doesn't really matter what we use.
  $form['fblikebutton_static_block_appearance']['fblikebutton_bl_show_faces'] = array(
    '#type' => 'select',
    '#title' => t('Display faces in the box'),
    '#options' => array('show' => t('Show faces'), 'hide' => t('Do not show faces')),
    '#default_value' => variable_get('fblikebutton_bl_show_faces', TRUE) ? 'show': 'hide',
    '#description' => t('Show profile pictures below the button. Only works with Standard layout'),
  );
  $form['fblikebutton_static_block_appearance']['fblikebutton_bl_action'] = array(
    '#type' => 'select',
    '#title' => t('Verb to display'),
    '#options' => array('like' => t('Like'), 'recommend' => t('Recommend')),
    '#default_value' => variable_get('fblikebutton_bl_action', 'like'),
    '#description' => t('The verb to display in the button.'),
  );
  $form['fblikebutton_static_block_appearance']['fblikebutton_bl_font'] = array(
    '#type' => 'select',
    '#title' => t('Font'),
    '#options' => array(
      'arial' => 'Arial',
      'lucida+grande' => 'Lucida Grande',
      'segoe+ui' => 'Segoe UI',
      'tahoma' => 'Tahoma',
      'trebuchet+ms' => 'Trebuchet MS',
      'verdana' => 'Verdana',
    ),
    '#default_value' => variable_get('fblikebutton_bl_font', 'arial'),
    '#description' => t('The font to display in the button'),
  );
  $form['fblikebutton_static_block_appearance']['fblikebutton_bl_color_scheme'] = array(
    '#type' => 'select',
    '#title' => t('Color scheme'),
    '#options' => array('light' => t('Light'), 'dark' => t('Dark')),
    '#default_value' => variable_get('fblikebutton_bl_color_scheme', 'light'),
    '#description' => t('The color scheme of box environtment'),
  );
  $form['fblikebutton_static_block_appearance']['fblikebutton_bl_iframe_width'] = array(
    '#type' => 'textfield',
    '#title' => t('Width of the iframe (px)'),
    '#default_value' => variable_get('fblikebutton_bl_iframe_width', '450'),
    '#description' => t('Width of the iframe, in pixels. Default is 450. <em>Note: lower values may crop the output.</em>'),
  );
  $form['fblikebutton_static_block_appearance']['fblikebutton_bl_iframe_height'] = array(
    '#type' => 'textfield',
    '#title' => t('Height of the iframe (px)'),
    '#default_value' => variable_get('fblikebutton_bl_iframe_height', '80'),
    '#description' => t('Height of the iframe, in pixels. Default is 80. <em>Note: lower values may crop the output.</em>'),
  );
  $form['fblikebutton_static_block_appearance']['fblikebutton_bl_iframe_css'] = array(
    '#type' => 'textfield',
    '#title' => t('Extra css styling needed'),
    '#default_value' => variable_get('fblikebutton_bl_iframe_css', ''),
    '#description' => t('Extra css attributes needed to make the iframe behave for your specific requirements. Will not necessarily overwrite existing styling. To alter the dimensions of the iframe, use the height and width fields found above.<br/>Example: <em>float: right; padding: 5px;</em>'),
  );
  $form['fblikebutton_static_block_appearance']['fblikebutton_bl_language'] = array(
    '#type' => 'textfield',
    '#title' => t('Language'),
    '#default_value' => variable_get('fblikebutton_bl_language', 'en_US'),
    '#description' => t('Specific language to use. Default is English. Examples:<br />French (France): <em>fr_FR</em><br />French (Canada): <em>fr_CA</em><br />More information can be found at http://developers.facebook.com/docs/internationalization/ and a full XML list can be found at http://www.facebook.com/translations/FacebookLocales.xml'),
  );
  $form['#validate'] = array('fblikebutton_config_form_validate');
  return system_settings_form($form);
 }

/**
 * Validation handler for fblikebutton_dynamic_settings form.
 */
function fblikebutton_config_form_validate($form, &$form_state) {
  // Convert the show faces options into booleans.
  if (isset($form_state['values']['fblikebutton_show_faces'])) {
    $form_state['values']['fblikebutton_show_faces'] = $form_state['values']['fblikebutton_show_faces'] == 'show' ? TRUE : FALSE;
  }
  if (isset($form_state['values']['fblikebutton_bl_show_faces'])) {
    $form_state['values']['fblikebutton_bl_show_faces'] = $form_state['values']['fblikebutton_bl_show_faces'] == 'show' ? TRUE : FALSE;
  }
}
