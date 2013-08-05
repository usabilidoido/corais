<?php

/**
 * @file
 * The finder admin screens.
 */

/**
 * Admin finder list page redirect.
 */
function finder_admin_list_redirect() {
  drupal_goto('admin/build/finder');
}

/**
 * Admin finder list page.
 */
function finder_admin_list() {
  $output = '';
  $finders = finder_load_multiple();

  if (!$finders) {
    $output .= t('There are currently no finders configured.');
  }
  else {
    foreach ((array)$finders as $finder) {

      // allow modules to change the finder here
      finder_invoke_finderapi($finder, 'finder_admin_list');

      if (!$finder->settings['programmatic']) {
        $rows = array();
        $rows[] = array(
          array(
            'data' => '<strong>'. check_plain($finder->title) .'</strong>',
            'class' => 'finder-title',
          ),
          array(
            'data' => l(t('Edit'), 'admin/build/finder/'. $finder->finder_id .'/edit')
              . ' | ' . l(t('Delete'), 'admin/build/finder/'. $finder->finder_id .'/delete'),
            'class' => 'finder-ops',
            'align' => 'right',
          ),
        );
        $rows[] = array(
          array(
            'data' => '
              <div class="type">'. t('Type') .'<span class="finder-colon">:</span> <span>'. t($finder->base_handler['#title']) .'</span></div>
              <div class="path">'. t('Path') .'<span class="finder-colon">:</span> <span>'. l($finder->path, $finder->path) .'</span></div>
              ',
            'class' => 'finder-summary',
          ),
          array(
            'data' =>  '<div class="description">'. check_markup($finder->description) .'</div>',
            'class' => 'finder-desc description',
          ),
        );
        $output .= theme('table', array(), $rows, array('class' => 'finder-table finder-'. $finder->finder_id));
      }
    }
  }

  $output .= drupal_get_form('finder_admin_add_form');

  return $output;

}

/**
 * Admin finder add form.
 */
function finder_admin_add_form($form_state) {
  $form = array();

  $form['base'] = array(
    '#type' => 'select',
    '#title' => t('Add a new finder'),
    '#default_value' => '',
    '#options' => finder_admin_get_base_options(),
    '#description' => t('Choose what kind of finder to create.'),
    '#required' => TRUE,
    '#weight' => 20,
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Add finder'),
    '#weight' => 22,
  );

  return $form;

}

/**
 * Submit function for admin finder add form.
 */
function finder_admin_add_form_submit($form, &$form_state) {
  $values = &$form_state['values'];
  $finder = (object)array('base' => $values['base']);
  finder_save($finder);
  $finder->title = t('Finder !1', array('!1' => $finder->finder_id));
  $finder->path = 'finder/'. $finder->finder_id;
  finder_save($finder);
  drupal_set_message(t('A finder was added.  Configure the finder below.'));
  menu_rebuild();
  drupal_goto('admin/build/finder/'. $finder->finder_id .'/edit');
}

/**
 * Admin finder edit page.
 */
function finder_admin_edit($form_state, $finder) {
  // allow modules to change the finder here
  finder_invoke_finderapi($finder, 'finder_admin_edit', $form, $form_state);

  $form_state['storage']['finder'] = $finder;

  // build the form
  $form = array();
  $form['#tree'] = TRUE;
  $form['finder_id'] = array(
    '#type' => 'value',
    '#value' => $finder->finder_id ? $finder->finder_id : 'add',
    '#weight' => 0,
  );

  $form['links'] = array(
    '#type' => 'markup',
    '#value' => theme('finder_admin_links', $finder),
    '#weight' => 0,
  );

  $form['base'] = array(
    '#type' => 'select',
    '#title' => t('Finder type'),
    '#default_value' => $finder->base,
    '#options' => finder_admin_get_base_options(),
    '#description' => t('You cannot change this.'),
    '#required' => TRUE,
    '#disabled' => TRUE,
    '#weight' => 5,
  );
  $form['title'] = array(
    '#type' => 'textfield',
    '#title' => t('Title'),
    '#default_value' => $finder->title,
    '#size' => 40,
    '#maxlength' => 255,
    '#required' => TRUE,
    '#weight' => 10,
  );
  $form['description'] = array(
    '#type' => 'textfield',
    '#title' => t('Description'),
    '#default_value' => $finder->description,
    '#size' => 80,
    '#description' => t('This will appear on the finder administration page.'),
    '#weight' => 20,
  );
  $form['path'] = array(
    '#type' => 'textfield',
    '#title' => t('Path'),
    '#default_value' => $finder->path,
    '#description' => t('Finder needs a URL path for some functionality.  No leading or trailing slash.'),
    '#size' => 30,
    '#maxlength' => 128,
    '#required' => TRUE,
    '#weight' => 40,
  );
  $form['block'] = array(
    '#type' => 'checkbox',
    '#title' => t('Provide block'),
    '#default_value' => $finder->block,
    '#weight' => 50,
    '#description' => t('Create a block for the finder form.'),
  );

  $form['settings'] = array(
    '#weight' => 60,
  );

  $form['settings']['form_on_page'] = array(
    '#type' => 'checkbox',
    '#title' => t('Form on page'),
    '#default_value' => $finder->settings['form_on_page'],
    '#weight' => 10,
    '#description' => t('Put a finder form on the path given above.'),
  );

  $form['settings']['form'] = array(
    '#type' => 'fieldset',
    '#title' => t('Form'),
    '#weight' => 100,
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['settings']['form']['prefix'] = array(
    '#type' => 'textarea',
    '#title' => t('Prefix'),
    '#default_value' => isset($finder->settings['form']['prefix']) ? $finder->settings['form']['prefix'] : '',
    '#weight' => 10,
    '#description' => t('Displayed before the form.'),
  );
  $form['settings']['form']['prefix_format'] = filter_form($finder->settings['form']['prefix_format'], 11, array('settings', 'form', 'prefix_format'));

  $form['settings']['form']['suffix'] = array(
    '#type' => 'textarea',
    '#title' => t('Suffix'),
    '#default_value' => isset($finder->settings['form']['suffix']) ? $finder->settings['form']['suffix'] : '',
    '#weight' => 20,
    '#description' => t('Displayed after the form.'),
  );
  $form['settings']['form']['suffix_format'] = filter_form($finder->settings['form']['suffix_format'], 21, array('settings', 'form', 'suffix_format'));

  $form['settings']['form']['button_text'] = array(
    '#type' => 'textfield',
    '#title' => t('Button text'),
    '#default_value' => isset($finder->settings['form']['button_text']) ? $finder->settings['form']['button_text'] : t('Find'),
    '#weight' => 100,
    '#description' => t('Text to display on the submit button.'),
  );
  $form['settings']['form']['go_text'] = array(
    '#type' => 'textfield',
    '#title' => t('Go text'),
    '#default_value' => isset($finder->settings['form']['go_text']) ? $finder->settings['form']['go_text'] : t('Go'),
    '#weight' => 120,
    '#description' => t('Text to display on the go button. Only used with <em>redirect to result</em> "go" button option under <em>Advanced</em>.'),
  );
  $form['settings']['advanced'] = array(
    '#type' => 'fieldset',
    '#title' => t('Advanced'),
    '#weight' => 200,
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['settings']['advanced']['no_results'] = array(
    '#type' => 'fieldset',
    '#title' => t('No results output'),
    '#weight' => 5,
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $variables = array(
    'keywords' => t('An array keyed by the Finder element ID, containing structured data about submitted keywords.'),
    'form_state' => t('The Forms API form state array.'),
    'finder' => t('Object containing data about this finder.'),
  );
  $form['settings']['advanced']['no_results']['no_results'] = finder_php_setting(
    array(
      '#type' => 'textarea',
      '#title' => t('No results output'),
      '#default_value' => isset($finder->settings['advanced']['no_results']['no_results']) ?
                          $finder->settings['advanced']['no_results']['no_results'] :
                          t('There are no results to display'),
      '#weight' => 5,
      '#rows' => 1,
      '#description' => t('
        You can use a mixture of text and PHP code to print or return the
        output when there are no results.  PHP must be enclosed in PHP tags.
        This can also be used to make context-aware redirects.'),
    ),
    array(
      'keywords' => t('An array keyed by the Finder element ID, containing structured data about submitted keywords.'),
      'form_state' => t('The Forms API form state array.'),
      'finder' => t('Object containing data about this finder.'),
    )
  );
  $form['settings']['advanced']['filter'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show results on results page even before the form has been completed'),
    '#default_value' => isset($finder->settings['advanced']['filter']) ? $finder->settings['advanced']['filter'] : 0,
    '#weight' => 10,
    '#description' => t('Allows this finder to behave more as a filter.'),
  );
  $form['settings']['advanced']['validate_empty'] = array(
    '#type' => 'checkbox',
    '#title' => t('Validate empty submission'),
    '#default_value' => isset($finder->settings['advanced']['validate_empty']) ? $finder->settings['advanced']['validate_empty'] : 0,
    '#weight' => 15,
    '#description' => t('Prevent submission where all elements have empty values.'),
  );

  $form['settings']['advanced']['submission'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use submit button'),
    '#default_value' => isset($finder->settings['advanced']['submission']) ? $finder->settings['advanced']['submission'] : 1,
    '#weight' => 20,
    '#description' => t('Gives the finder a submit button.'),
  );

  $form['settings']['advanced']['ahah'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use AHAH on submit button'),
    '#default_value' => isset($finder->settings['advanced']['ahah']) ? $finder->settings['advanced']['ahah'] : 0,
    '#weight' => 30,
    '#description' => t('Dynamically update the form and results with <a href="http://en.wikipedia.org/wiki/AHAH">AHAH javascript replacement</a> instead of loading a whole new page.'),
  );

  $form['settings']['advanced']['ahah_effect'] = array(
    '#type' => 'radios',
    '#title' => t('AHAH effect'),
    '#default_value' => isset($finder->settings['advanced']['ahah_effect']) ? $finder->settings['advanced']['ahah_effect'] : 'none',
    '#weight' => 32,
    '#description' => t('If using AHAH, this will be the transition effect when the update occurs.'),
    '#options' => array(
      'none' => t('None'),
      'fade' => t('Fade'),
      'slide' => t('Slide'),
    ),
  );

  $form['settings']['advanced']['ahah_remote'] = array(
    '#type' => 'checkbox',
    '#title' => t('AHAH block is remote control'),
    '#default_value' => isset($finder->settings['advanced']['ahah_remote']) ? $finder->settings['advanced']['ahah_remote'] : 0,
    '#weight' => 34,
    '#description' => t('If AHAH is being used in a finder block that happens to be on the finder page, make the finder block act as a remote control for the finder page.'),
  );

  $form['settings']['advanced']['element_combination'] = array(
    '#type' => 'radios',
    '#title' => t('Multiple element result combination'),
    '#default_value' => isset($finder->settings['advanced']['element_combination']) ? $finder->settings['advanced']['element_combination'] : 0,
    '#weight' => 40,
    '#description' => t('With multiple elements, how should the results of each element be combined on the results page?'),
    '#options' => array(
      0 => t('<em>Conjunction</em> - Match <em>all</em> elements using the AND operator.'),
      1 => t('<em>Disjunction</em> - Match <em>any</em> element using the OR operator.'),
    ),
  );

  $form['settings']['advanced']['pager'] = array(
    '#type' => 'textfield',
    '#title' => t('Pager'),
    '#default_value' => isset($finder->settings['advanced']['pager']) ? $finder->settings['advanced']['pager'] : '10',
    '#description' => t('Number of items to show per results page.  Set to 0 to disable pager.'),
    '#size' => 5,
    '#maxlength' => 8,
    '#weight' => 50,
  );

  $form['settings']['advanced']['goto'] = array(
    '#type' => 'radios',
    '#title' => t('Redirect to result'),
    '#default_value' => $finder->settings['advanced']['goto'] ? $finder->settings['advanced']['goto'] : '',
    '#weight' => 80,
    '#options' => array(
      '' => t('Never redirect to result, always use results page.'),
      'best' => t('Redirect to the only result, or show results page if there are multiple results.'),
      'always' => t('Always redirect to the first result.'),
      'go' => t('Use a seperate "go" button for the first result, otherwise use results page.'),  // 'separate' is intentionally written as 'seperate' here.
    ),
  );

  $form['settings']['advanced']['hide_args'] = array(
    '#type' => 'checkbox',
    '#title' => t('Hide URL arguments on results page'),
    '#default_value' => isset($finder->settings['advanced']['hide_args']) ? $finder->settings['advanced']['hide_args'] : 0,
    '#weight' => 160,
    '#description' => t("Use the user's session to store submitted values across multiple pages.  Also creates a token for multipage results to identify the stored submission"),
  );

  $form['settings']['advanced']['arg_sep'] = array(
    '#type' => 'textfield',
    '#title' => t('Multiple value URL arguments separator'),
    '#default_value' => isset($finder->settings['advanced']['arg_sep']) ? $finder->settings['advanced']['arg_sep'] : ',',
    '#weight' => 170,
    '#description' => t('Must contain at least one character other than \' \', \'/\', \'_\', and \'-\'.  If this string exists within a keyword, the keyword will be automatically enclosed in double quotes to prevent parsing issues.'),
    '#size' => 5,
    '#required' => TRUE,
  );

  $form['settings']['advanced']['empty_symbol'] = array(
    '#type' => 'textfield',
    '#title' => t('Empty value URL arguments symbol'),
    '#default_value' => isset($finder->settings['advanced']['empty_symbol']) ? $finder->settings['advanced']['empty_symbol'] : '',
    '#weight' => 172,
    '#description' => t('Leave blank for default symbol (a space).  You may need to set this to some other symbol or text if you are getting "forbidden" errors on results pages.  Must contain at least one character other than \' \', \'/\', \'_\', and \'-\'.'),
    '#size' => 5,
    '#required' => FALSE,
  );

  $form['settings']['advanced']['show_links'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show finder links'),
    '#default_value' => isset($finder->settings['advanced']['show_links']) ? $finder->settings['advanced']['show_links'] : 1,
    '#weight' => 180,
    '#description' => t('Show the <em>finder links</em>.'),
  );

  $form['settings']['advanced']['show_admin_links'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show finder admin links'),
    '#default_value' => isset($finder->settings['advanced']['show_admin_links']) ? $finder->settings['advanced']['show_admin_links'] : 1,
    '#weight' => 182,
    '#description' => t('Show the <em>finder admin links</em>.'),
  );

  $form['settings']['advanced']['cache_finder_find'] = array(
    '#type' => 'textfield',
    '#title' => t('Cache duration of options and results'),
    '#default_value' => isset($finder->settings['advanced']['cache_finder_find']) ? $finder->settings['advanced']['cache_finder_find'] : '0',
    '#weight' => 190,
    '#description' => t('Set to 0 to disable.'),
    '#size' => 5,
    '#required' => TRUE,
    '#field_suffix' => t('minutes'),
  );

  if (!empty($finder->elements)) {
    $form['elements'] = array(
      '#title' => t('Elements'),
      '#weight' => 2000,
      '#theme' => 'finder_admin_edit_elements_table',
    );
    foreach ($finder->elements as $key => $value) {
      if (empty($value->settings['choices']['field'])) {
         drupal_set_message(t("Finder element %title is not configured.", array('%title' => $value->title)), 'error');
      }
      $element_output = '<div class="finder-element">';
      $element_output .= '<div class="finder-element-title"><strong>'. check_plain($value->title) .'</strong></div>';
      $element_output .= '<div class="finder-element-element">'. t('Element') .'<span class="finder-colon">:</span> <span>'. $value->element_handler['#title'] .'</span></div>';
      $element_output .= '</div>';
      $form['elements'][$value->finder_element_id]['value']['#value'] = $element_output;
      $form['elements'][$value->finder_element_id]['ops']['#value'] =
        l(t('Edit'), 'admin/build/finder/'. $finder->finder_id .'/edit/'. $value->finder_element_id .'/edit')
          . ' | ' . l(t('Delete'), 'admin/build/finder/'. $finder->finder_id .'/edit/'. $value->finder_element_id .'/delete');
      $form['elements'][$value->finder_element_id]['weight'] = array(
        '#type' => 'weight',
        '#delta' => count($finder->elements),
        '#default_value' => $key,
      );
    }
  }
  else {
    $form['no_elements'] = array(
      '#value' => '
        <div id="edit-no-elements-wrapper" class="form-item">
        <label for="edit-path">'. t('Elements') .'<span class="finder-colon">:</span></label>'
        . t('There are currently no elements configured for this finder.')
        .'</div>
      ',
      '#weight' => 100,
    );
  }

  $form['add_element'] = array(
    '#type' => 'select',
    '#title' => t('Add element'),
    '#default_value' => '',
    '#options' => finder_admin_get_element_options(),
    '#description' => t('Choose the form element to use and press %save_button.', array('%save_button' => t('Save finder'))),
    '#weight' => 10000,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save finder'),
    '#weight' => 10010,
  );
  $form['delete'] = array(
    '#type' => 'submit',
    '#value' => t('Delete finder'),
    '#weight' => 10020,
  );
  return $form;
}

/**
 * Validate function for admin finder add/edit page.
 */
function finder_admin_edit_validate($form, &$form_state) {
  $values = &$form_state['values'];
  $arg_sep = preg_replace("/[\/\-\_\s]/", "", $values['settings']['advanced']['arg_sep']);
  if (!$arg_sep) {
    form_set_error('arg_sep', t('Multiple value URL arguments separator must contain at least one character that is not a space ( ), forward-slash (/), hyphen (-), or underscore (_).'));
  }
  if (!empty($values['settings']['advanced']['empty_symbol'])) {
    $empty_symbol = preg_replace("/[\/\-\_\s]/", "", $values['settings']['advanced']['empty_symbol']);
    if (!$empty_symbol) {
      form_set_error('arg_sep', t('Empty value URL arguments symbol must contain at least one character that is not a space ( ), forward-slash (/), hyphen (-), or underscore (_).'));
    }
    if ($arg_sep == $empty_symbol) {
      form_set_error('empty_symbol', t('The "empty value URL arguments symbol" should not be the same as the "multiple value URL arguments separator".'));
    }
  }
}

/**
 * Submit function for admin finder add/edit page.
 */
function finder_admin_edit_submit($form, &$form_state) {
  $values = &$form_state['values'];
  if ($form_state['clicked_button']['#id'] == 'edit-delete') {
    drupal_goto('admin/build/finder/'. $values['finder_id'] .'/delete');
  }
  else {
    $finder = (object)array(
      'finder_id' => $values['finder_id'],
      'base' => $values['base'],
      'title' => $values['title'],
      'description' => $values['description'],
      'path' => $values['path'],
      'block' => $values['block'],
      'settings' => $values['settings'],
    );
    finder_save($finder);
    if (is_array($values['elements'])) {
      foreach ($values['elements'] as $finder_element_id => $value) {
        $finder_element = (object)array(
          'finder_element_id' => $finder_element_id,
          'weight' => $value['weight'],
        );
        finder_element_save($finder_element);
      }
    }
    drupal_set_message(t('The %ft finder was saved.', array('%ft' => $finder->title)));
    menu_rebuild();

    // Clear the finder_find caches for this finder.
    if ($finder->settings['advanced']['cache_finder_find']) {
      $finder = finder_load($finder->finder_id);
      if (!empty($finder->elements)) {
        cache_clear_all('f'. $finder->finder_id .'|', 'cache_finder_find', TRUE);
        foreach ($finder->elements as $element) {
          cache_clear_all('e'. $element->finder_element_id .'|', 'cache_finder_find', TRUE);
        }
        drupal_set_message(t('Cache for the %ft finder was reset.', array('%ft' => $finder->title)));
      }
    }

    if ($values['add_element']) {
      $finder_element = (object)array(
        'finder_id' => $values['finder_id'],
        'element' => $values['add_element'],
      );
      finder_element_save($finder_element);
      $finder_element->title = t('Finder element !1', array('!1' => $finder_element->finder_element_id));
      finder_element_save($finder_element);
      drupal_set_message(t('A finder element was added.  Configure the element below.'));
      drupal_goto('admin/build/finder/'. $values['finder_id'] .'/edit/'. $finder_element->finder_element_id .'/edit/');
    }
    else {
      drupal_goto('admin/build/finder/'. $values['finder_id'] .'/edit/');
    }
  }
}

/**
 * Admin finder delete page.
 */
function finder_admin_delete($form_state, $finder) {
  // allow modules to change the finder here
  finder_invoke_finderapi($finder, 'finder_admin_delete');

  $form_state['storage']['finder'] = $finder;
  // build the form
  $form = array();
  $form['#tree'] = TRUE;
  $form['finder_id'] = array(
    '#type' => 'value',
    '#value' => $finder->finder_id,
  );
  return confirm_form($form,
    t('Are you sure you want to delete the %ft finder?', array('%ft' => $finder->title)),
    $_GET['destination'] ? $_GET['destination'] : 'admin/build/finder',
    t('This action cannot be undone.'),
    t('Delete'), t('Cancel'));
}

/**
 * Submit function for admin finder delete page.
 */
function finder_admin_delete_submit($form, &$form_state) {
  $finder_id = &$form_state['values']['finder_id'];
  finder_delete($finder_id);
  menu_rebuild();
  drupal_goto('admin/build/finder');
}

/**
 * Admin finder element add/edit page.
 * Must have a Finder object at this point.
 */
function finder_admin_element_edit($form_state, $finder, $finder_element_id) {

  $element = &finder_element($finder, $finder_element_id);

  // allow modules to change the element here
  finder_invoke_finderapi($element, 'finder_admin_element_edit');

  $module = &$finder->base_handler['#module'];
  $fields = module_invoke($module, 'finder_fields', $finder, $finder_element_id);

  $form_state['storage']['finder'] = $finder;
  $form_state['storage']['finder_element_id'] = $finder_element_id;
  $form_state['storage']['finder_element_defaults'] = $element;


  $form = array();

  $form['#tree'] = TRUE;

  $form['finder_id'] = array(
    '#type' => 'value',
    '#value' => $finder->finder_id,
    '#weight' => 0,
  );

  $form['finder_element_id'] = array(
    '#type' => 'value',
    '#value' => $finder_element_id,
    '#weight' => 2,
  );

  $form['element'] = array(
    '#type' => 'select',
    '#title' => t('Element'),
    '#default_value' => $element->element ? $element->element : '',
    '#options' => finder_admin_get_element_options(),
    '#description' => t('The form element to use.'),
    '#required' => TRUE,
    '#weight' => 10,
  );

  $form['title'] = array(
    '#type' => 'textfield',
    '#title' => t('Title'),
    '#default_value' => $element->title ? $element->title : '',
    '#size' => 40,
    '#maxlength' => 255,
    '#required' => TRUE,
    '#weight' => 20,
    '#description' => t('The label on the form element.'),
  );

  $form['settings'] = array(
    '#weight' => 30,
  );

  $form['settings']['form'] = array(
    '#type' => 'fieldset',
    '#title' => t('Form element'),
    '#weight' => 0,
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $form['settings']['form']['description'] = array(
    '#type' => 'textfield',
    '#title' => t('Description'),
    '#default_value' => isset($element->settings['form']['description']) ? $element->settings['form']['description'] : '',
    '#weight' => 10,
    '#description' => t('The description or help message to display with the form element.'),
  );

  $form['settings']['form']['prefix'] = array(
    '#type' => 'textarea',
    '#title' => t('Prefix'),
    '#default_value' => isset($element->settings['form']['prefix']) ? $element->settings['form']['prefix'] : '',
    '#weight' => 20,
    '#description' => t('Displayed before the form element.'),
  );
  $form['settings']['form']['prefix_format'] = filter_form($element->settings['form']['prefix_format'], 21, array('settings', 'form', 'prefix_format'));

  $form['settings']['form']['suffix'] = array(
    '#type' => 'textarea',
    '#title' => t('Suffix'),
    '#default_value' => isset($element->settings['form']['suffix']) ? $element->settings['form']['suffix'] : '',
    '#weight' => 30,
    '#description' => t('Displayed after the form element.'),
  );
  $form['settings']['form']['suffix_format'] = filter_form($element->settings['form']['suffix_format'], 31, array('settings', 'form', 'suffix_format'));

  $form['settings']['form']['default_value'] = array(
    '#type' => 'textfield',
    '#title' => t('Default value'),
    '#default_value' => isset($element->settings['form']['default_value']) ? $element->settings['form']['default_value'] : '',
    '#weight' => 40,
    '#description' => t('Default value of the form element.'),
  );

  $form['settings']['form']['required'] = array(
    '#type' => 'checkbox',
    '#title' => t('Required'),
    '#default_value' => isset($element->settings['form']['required']) ? $element->settings['form']['required'] : 0,
    '#weight' => 50,
    '#description' => t('Enforce user input in this field.'),
  );

  $form['settings']['choices'] = array(
    '#type' => 'fieldset',
    '#title' => t('Possible choices'),
    '#weight' => 60,
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  if ($fields) {
    $form['settings']['choices']['field'] = array(
      '#type' => 'select',
      '#title' => t('Finder field'),
      '#default_value' => $element->settings['choices']['field'],
      '#options' => $fields,
      '#description' => t('This is the field that will be searched on (filtered).  If you pick multiple fields; performance will be affected.  This field will also be displayed in choices lists (if applicable), however often the raw values of this field are unacceptable for human readability, you can overcome this with PHP by using a computed field, using rewrite options below (if applicable), or through theming.'),
      '#weight' => 20,
      '#required' => TRUE,
      '#multiple' => TRUE,
      '#size' => min(6, count($fields)+1),
    );
  }
  else {
    $form['settings']['choices']['no_field'] = array(
      '#value' => t("<em>There are currently no fields to choose from.</em>"),
      '#weight' => 20,
    );
  }

  $form['settings']['choices']['sort'] = array(
    '#type' => 'checkbox',
    '#title' => t('Sort options'),
    '#default_value' => $element->settings['choices']['sort'],
    '#weight' => 100,
    '#description' => t('Will put the list into order for easier scanning.'),
  );

  $form['settings']['choices']['sanitization'] = array(
    '#type' => 'fieldset',
    '#title' => t('Sanitization'),
    '#description' => t('No piece of user-submitted content should ever be placed as-is into HTML.'),
    '#weight' => 120,
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $format_options = array(
    'filter_xss' => t('Filter XSS') . theme('filter_tips', array(array(
      array('tip' => t('Filters HTML to prevent cross-site-scripting (XSS) vulnerabilities.')),
      array('tip' => t('Allowed HTML tags') . check_plain(': <a> <em> <strong> <cite> <code> <ul> <ol> <li> <dl> <dt> <dd>')),
      array('tip' => '<strong>' . t('Recommended for finder fields that contain only plain values or the above HTML tags.') . '</strong>')
    )), FALSE),
    'filter_xss_admin' => t('Filter XSS permissive') . theme('filter_tips', array(array(
      array('tip' => t('Allows all tags that can be used inside an HTML body, save for scripts and styles.')),
      array('tip' => '<strong>' . t('Recommended for finder fields that contain HTML.') . '</strong>')
    )), FALSE),
    'check_plain' => t('Check plain') . theme('filter_tips', array(array(
      array('tip' => t('Encodes special characters in a plain-text string for display as HTML.')),
      array('tip' => '<strong>' . t('Approved for finder fields that contain only plain values.') . '</strong>')
    )), FALSE),
    'check_url' => t('Check URL') . theme('filter_tips', array(array(
      array('tip' => t('Strips dangerous protocols from a URI and encodes it for output to HTML.')),
      array('tip' => '<strong>' . t('Approved for finder fields where the value is a URL.') . '</strong>')
    )), FALSE),
  );
  $filter_formats = filter_formats();
  foreach ($filter_formats as $filter_format) {
    $tips = _filter_tips($filter_format->format, FALSE);
    $tips[$filter_format->name]['finder'] = array('tip' => t('Not pre-approved for finder fields, use with caution.'));
    $format_options[$filter_format->format] = $filter_format->name . theme('filter_tips', $tips);
  }

  $form['settings']['choices']['sanitization']['format'] = array(
    '#type' => 'radios',
    '#title' => t('Sanitization filter'),
    '#default_value' => isset($element->settings['choices']['sanitization']['format']) ? $element->settings['choices']['sanitization']['format'] : 'filter_xss',
    '#options' => $format_options,
  );

  $form['settings']['advanced'] = array(
    '#type' => 'fieldset',
    '#title' => t('Submitted values'),
    '#weight' => 500,
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $form['settings']['advanced']['delimit'] = array(
    '#type' => 'textfield',
    '#title' => t('Treat delimited values as separate keywords upon submission'),
    '#default_value' => isset($element->settings['advanced']['delimit']) ? $element->settings['advanced']['delimit'] : '',
    '#weight' => 10,
    '#description' => t('For example, if you type a space here, the submitted value will be expanded into a value for each word.  Leave empty to disable this feature.'),
  );

  $form['settings']['advanced']['field_combination'] = array(
    '#type' => 'radios',
    '#title' => t('Multiple field result combination'),
    '#default_value' => isset($element->settings['advanced']['field_combination']) ? $element->settings['advanced']['field_combination'] : 0,
    '#weight' => 20,
    '#description' => t('With multiple fields being select above, how should the fields be combined when calculating results?'),
    '#options' => array(
      // I know these choices are in a different order than on the finder admin
      // page, but that's because the first option is the more appropriate
      // default in both cases.
      0 => t('<em>Disjunction</em> - Match <em>any</em> field using the OR operator.'),
      1 => t('<em>Conjunction</em> - Match <em>all</em> fields using the AND operator.'),
    ),
  );

  $form['settings']['advanced']['value_combination'] = array(
    '#type' => 'radios',
    '#title' => t('Multiple value result combination'),
    '#default_value' => isset($element->settings['advanced']['value_combination']) ? $element->settings['advanced']['value_combination'] : 0,
    '#weight' => 25,
    '#description' => t('With multiple submitted values for this element, how should the values be combined when calculating results?'),
    '#options' => array(
      // I know these choices are in a different order than on the finder admin
      // page, but that's because the first option is the more appropriate
      // default in both cases.
      0 => t('<em>Disjunction</em> - Match <em>any</em> value using the OR operator.'),
      1 => t('<em>Conjunction</em> - Match <em>all</em> values using the AND operator.'),
    ),
  );

  $form['settings']['advanced']['nesting_order'] = array(
    '#type' => 'radios',
    '#title' => t('Multiple field/value nesting order'),
    '#default_value' => isset($element->settings['advanced']['nesting_order']) ? $element->settings['advanced']['nesting_order'] : 0,
    '#weight' => 30,
    '#description' => t('With multiple values or fields, how should fields and values be matched together?  This is difficult to explain.  Suppose you have an element that selects two fields and a user submits two values (X and Y), the first option here will do matching like this: <em>(field_1 matches X; field_2 matches X), (field_1 matches Y; field_2 matches Y)</em> whereas the second will do it like this: <em>(field_1 matches X, field_1 matches Y); (field_2 matches X, field_2 matches Y)</em>.  The semicolons represent <em>multiple field result combination</em>, the commas represent <em>multiple value result combination</em>, and the word <em>matches</em> refers to the <em>result matching</em> option below.  So simple.'),
    '#options' => array(
      0 => t('Match multiple fields for each value first, then combine the results of multiple values. (recommended)'),
      1 => t('Match multiple values for each field first, then combine the results of multiple fields.'),
    ),
  );

  $operators = finder_match_operator();
  foreach ($operators as $k => $v) {
    $operators[$k] = $v['description'];
  }
  $form['settings']['advanced']['match'] = array(
    '#type' => 'radios',
    '#title' => t('Result matching'),
    '#default_value' => isset($element->settings['advanced']['match']) ? $element->settings['advanced']['match'] : 'e',
    '#options' => $operators,
    '#weight' => 60,
    '#description' => t('"Equals" is the most common result matching method.'),
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save finder element'),
    '#weight' => 10000,
  );

  $form['delete'] = array(
    '#type' => 'submit',
    '#value' => t('Delete finder element'),
    '#weight' => 10020,
  );

  return $form;
}

/**
 * Submit function for admin finder add/edit page.
 */
function finder_admin_element_edit_submit($form, &$form_state) {
  $values = &$form_state['values'];
  if ($form_state['clicked_button']['#id'] == 'edit-delete') {
    drupal_goto('admin/build/finder/'. $values['finder_id'] .'/edit/'. $values['finder_element_id'] .'/delete');
  }
  else {
    $finder_element = (object)array(
      'finder_element_id' => $values['finder_element_id'],
      'finder_id' => $values['finder_id'],
      'element' => $values['element'],
      'title' => $values['title'],
      'weight' => $values['weight'],
      'settings' => $values['settings'],
    );
    finder_element_save($finder_element);

    drupal_set_message(t('The finder element was saved.'));
    drupal_goto('admin/build/finder/'. $values['finder_id'] .'/edit');
  }
}

/**
 * Admin finder delete page
 */
function finder_admin_element_delete($form_state, $finder, $finder_element_id) {
  $element = &finder_element($finder, $finder_element_id);

  // allow modules to change the element here
  finder_invoke_finderapi($element, 'finder_admin_element_delete');

  $form = array();
  $form['#tree'] = TRUE;
  $form['finder_element_id'] = array(
    '#type' => 'value',
    '#value' => $finder_element_id,
  );
  $form['finder_id'] = array(
    '#type' => 'value',
    '#value' => $finder->finder_id,
  );
  return confirm_form($form,
    t('Are you sure you want to delete the finder element %fet ?', array('%fet' => $element->title)),
    $_GET['destination'] ? $_GET['destination'] : 'admin/build/finder/'. $finder->finder_id .'/edit',
    t('This action cannot be undone.'),
    t('Delete'), t('Cancel'));
}

/**
 * Submit function for admin finder delete page.
 */
function finder_admin_element_delete_submit($form, &$form_state) {
  $finder_element_id = &$form_state['values']['finder_element_id'];
  $finder_id = &$form_state['values']['finder_id'];
  finder_element_delete($finder_element_id);
  drupal_goto('admin/build/finder/'. $finder_id .'/edit');
}

/**
 * Get base handlers and convert to options array.
 */
function finder_admin_get_base_options() {
  $bases = finder_base_handlers();
  $base_options = array('' => t('- Select a finder type -'));
  if (!empty($bases)) {
    foreach ($bases as $base_key => $base_data) {
      $base_options[$base_key] = $base_data['#title'];
    }
  }
  else {
    drupal_set_message(t('Error: You must activate a finder base handler module such as Finder Views or Finder Node to begin.'), 'error');
    return array('' => t('- No finder types to choose from -'));
  }
  return $base_options;
}

/**
 * Get element handlers and convert to options array.
 */
function finder_admin_get_element_options() {
  $elements = finder_element_handlers();
  $element_options = array('' => t('- Select an element type -'));
  if (!empty($elements)) {
    foreach ($elements as $element_key => $element_data) {
      $element_options[$element_key] = t($element_data['#title']);
    }
  }
  else {
    drupal_set_message(t('Error: You must activate a finder element handler module such as Finder Autocomplete or Finder Optionwidgets to begin.'), 'error');
    return array('' => t('- No element types to choose from -'));
  }
  return $element_options;
}

/**
 * Admin finder import page.
 */
function finder_admin_import() {
  global $user;
  $output;
  if ($_POST['finder_code']) {
    $finder_code = trim($_POST['finder_code']);
    $import = finder_import($finder_code);
    if (is_array($import)) {
      foreach ($import as $finder) {
        $new_finder = finder_clone($finder);
        drupal_set_message(t("Imported !finder", array('!finder' => l($new_finder->title, 'admin/build/finder/'. $new_finder->finder_id .'/edit'))));
      }
      drupal_goto('admin/build/finder');
    }
    else {
      $new_finder = finder_clone($import);
      drupal_set_message(t("Imported !finder", array('!finder' => l($new_finder->title, 'admin/build/finder/'. $new_finder->finder_id .'/edit'))));
      drupal_goto('admin/build/finder/'. $new_finder->finder_id .'/edit');
    }
  }
  return drupal_get_form('finder_admin_import_form', $form);
}

/**
 * Admin finder import form.
 */
function finder_admin_import_form($form_state) {
  $form['#prefix'] = t('<p>You may import a finder by copy-and-pasting the results of an exported finder. </p>');
  $form['finder_code'] = array(
    '#type' => 'textarea',
    '#title' => t('Finder code'),
    '#default_value' => '',
    '#required' => TRUE,
    '#rows' => 30,
    '#description' => t('Paste the finder code here.'),
  );
  $form['#redirect'] = FALSE;
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Import'),
  );
  return $form;
}

/**
 * Admin finder export page.
 *
 * Set $return_code to TRUE to not return form but the code instead.
 */
function finder_admin_export($original_finder, $return_code = FALSE) {
  $finder = drupal_clone($original_finder);
  drupal_set_title(t('Export of !title', array('!title' => check_plain($finder->title))));
  finder_invoke_finderapi($finder, 'finder_export');
  $finder_code = finder_export($finder);
  return ($return_code ? $finder_code : drupal_get_form('finder_admin_export_form', $finder_code));
}

/**
 * Admin finder export form.
 */
function finder_admin_export_form($form_state, $code) {
  $form = array();
  $form['export'] = array(
    '#type' => 'textarea',
    '#title' => t('Finder code'),
    '#default_value' => $code,
    '#rows' => 30,
    '#description' => t('Copy this code and then on the site you want to import to, go to the "Finder import" link on the finder admin page, and paste it in there.'),
    '#attributes' => array(
      'style' => 'width: 97%;',
    ),
  );
  return $form;
}