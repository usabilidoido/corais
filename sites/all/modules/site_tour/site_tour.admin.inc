<?php
// $Id: site_tour.admin.inc,v 1.1.2.1.2.4 2009/01/29 21:36:46 stefano73 Exp $

/**
 * @file
 * Admin page callback file for the site_tour module.
 */

/**
 * Menu callback for the site tour list page.
 */
function site_tour_admin_list($form_state) {
  if (isset($form_state['values']['operation']) && $form_state['values']['operation'] == 'delete') {
    return site_tour_multiple_delete_confirm($form_state, array_filter($form_state['values']['tours']));
  }

  $form = array('#theme' => 'site_tour_list');
  
  $form['options'] = array(
    '#type' => 'fieldset',
    '#title' => t('Update options'),
    '#prefix' => '<div class="container-inline">',
    '#suffix' => '</div>',
  );
  $options = array();
  $operations = array('show' => t('Show'), 'hide' => t('Hide'), 'delete' => t('Delete'));
  foreach ($operations as $operation => $label) {
    $options[$operation] = $label;
  }
  $form['options']['operation'] = array(
    '#type' => 'select',
    '#options' => $options,
    '#default_value' => 'show',
  );
  $form['options']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Update'),
    '#submit' => array('site_tour_admin_list_submit'),
  );

  // select tour data
  $sql = 'SELECT * FROM {site_tour} ORDER BY title';
  $result = pager_query($sql, 20, 0);
  $tours = array();
  $status = array(t('Hidden'), t('Visible'));
  while ($tour = db_fetch_object($result)) {
    $tours[$tour->id] = '';
    $form['id'][$tour->id] = array('#type' => 'value',  '#value' => $tour->id);
    $form['status'][$tour->id] = array('#value' => $status[$tour->status]);
    $form['title'][$tour->id] = array('#value' => $tour->title);    
    $form['description'][$tour->id] = array('#value' => $tour->description);
    $form['url'][$tour->id] = array('#value' => $tour->url);
    $form['pages'][$tour->id] = array('#value' => $tour->pages);
  }
  
  if (empty($tours)) unset($form['options']);
  
  $form['tours'] = array('#type' => 'checkboxes', '#options' => $tours);
  $form['pager'] = array('#value' => theme('pager', NULL, 20, 0));
  return $form;
}

/**
 * Validate site_tour_admin_list form submissions.
 * 
 * Check if any tours have been selected to perform the chosen
 * 'Update option' on.
 */
function site_tour_admin_list_validate($form, &$form_state) {
  $tours = array_filter($form_state['values']['tours']);
  if (count($tours) == 0) {
    form_set_error('', t('No items selected.'));
  }
}

/**
 * Process site_tour_admin_list form submissions.
 * 
 * Execute the chosen 'Update option' on the selected tours.
 */
function site_tour_admin_list_submit($form, &$form_state) {
  $operation = $form_state['values']['operation'];
  // Filter out unchecked tours
  $tours = array_filter($form_state['values']['tours']);
  switch ($operation) {
    case 'show':      
    case 'hide':
      $status = ($operation == 'hide' ? 0 : 1);
      foreach ($tours as $id => $value) {
        db_query("UPDATE {site_tour} SET status=%d WHERE id=%d", $status, $id);
      }
      drupal_set_message(t('The update has been performed.'));
      return;

    case 'delete':
      // We need to rebuild the form to go to a second step.  For example, to
      // show the confirmation form for the deletion of nodes.
      $form_state['rebuild'] = TRUE;
      return;
  }
}

function site_tour_multiple_delete_confirm(&$form_state, $tours) {
  $form = array('help' => array());
  $form['tours'] = array('#prefix' => '<ul>', '#suffix' => '</ul>', '#tree' => TRUE);
  // array_filter returns only elements with TRUE values
  foreach ($tours as $id => $value) {
    $title = db_result(db_query('SELECT title FROM {site_tour} WHERE id=%d', $id));
    $form['tours'][$id] = array(
      '#type' => 'hidden',
      '#value' => $id,
      '#prefix' => '<li>',
      '#suffix' => check_plain($title) ."</li>\n",
    );
  }
  $form['operation'] = array('#type' => 'hidden', '#value' => 'delete');
  $form['#submit'][] = 'site_tour_multiple_delete_confirm_submit';
  $title = t('Are you sure you want to delete these items?');
  $form['help'] = array('#value' => '<p>'. t('The following site tours will be deleted from your site, and <em>all data will be lost</em>!') .'<br />'.$title.'</p>');
  return confirm_form($form, $title, 'admin/content/site_tour', t('This action cannot be undone.'), t('Delete all'), t('Cancel'));
}

function site_tour_multiple_delete_confirm_submit($form, &$form_state) {
  if ($form_state['values']['confirm']) {
    foreach ($form_state['values']['tours'] as $id => $value) {
      db_query("DELETE FROM {site_tour} WHERE id=%d", $id);
      db_query("DELETE FROM {site_tour_pages} WHERE tour=%d", $id);
    }
    drupal_set_message(t('The items have been deleted.'));
  }
  $form_state['redirect'] = 'admin/content/site_tour';
  return;
}

/**
 * Add/edit site tours.
 */
function site_tour_admin_edit($form_values, $id = NULL) {
  if (isset($form_values['values']['tour'])) {
    $tour = $form_values['values']['tour'];
  }
  else {
    $tour = ($id ? db_fetch_object(db_query('SELECT * FROM {site_tour} WHERE id=%d', $id)) : NULL);
  }
  $tour = (object)$tour;
  
  $form = array();
  $form['title'] = array(
    '#type' => 'textfield',
    '#title' => t('Title'),
    '#default_value' => $tour->title,
    '#maxlength' => 128,
    '#size' => 50,
    '#required' => TRUE,
    '#description' => t('Enter the tour title.'),
  );
  $form['description'] = array(
    '#type' => 'textarea',
    '#title' => t('Description'),
    '#default_value' => $tour->description,
    '#rows' => 3,
    '#cols' => 50,
    '#description' => t('Enter the tour description (max 255 chars).'),
  );
  $form['url'] = array(
    '#type' => 'textfield',
    '#title' => t('Url'),
    '#default_value' => $tour->url,
    '#maxlength' => 128,
    '#size' => 30,
    '#required' => TRUE,
    '#description' => t('The path to use to run this site tour. No leading or trailing slashes.'),
    '#field_prefix' => url(NULL, array('absolute' => TRUE)).(variable_get('clean_url', 0) ? '' : '?q=') .'site_tour/',
    '#field_suffix' => ($tour->url ? l(t('Preview'), 'site_tour/'.$tour->url, array('attributes' => array('target' => '_blank'))) : ''),
  );
  $form['pages'] = array(
    '#type' => 'item',
    '#title' => t('Pages'),
    '#value' => (int)$tour->pages,
    '#description' => t('Number of pages for this tour.'),
  );
  $form['status'] = array(
    '#type' => 'radios',
    '#title' => t('Status'),
    '#default_value' => (isset($tour->status) ? $tour->status : 1),
    '#options' => array(t('Hidden'), t('Visible')),
  );
    $url_title = t('This can be an internal path such as %add-node or an external URL such as %example. Enter %front to link to the front page.', array('%front' => '<front>', '%add-node' => 'node/73', '%example' => 'http://www.example.com'));
  $form['close_url'] = array(
    '#type' => 'textfield',
    '#title' => t('Close button url'),
    '#default_value' => $tour->close_url,
    '#maxlength' => 128,
    '#size' => 50,
    '#description' => t("This page is opened when user clicks the Close button.").' '.$url_title,
  );
  $form['exit_url'] = array(
    '#type' => 'textfield',
    '#title' => t('Exit page url'),
    '#default_value' => $tour->exit_url,
    '#maxlength' => 128,
    '#size' => 50,
    '#description' => t("This page is opened when user clicks the Next button on the tour's last page.").' '.$url_title,
  );

  // Get current list of skins
  $options = array();
  $path = drupal_get_path('module', 'site_tour').'/lib/skin';
  $skins = drupal_system_listing('\.tpl\.js', $path, 'filename');
  foreach (array_keys($skins) as $key) {
    $arr = explode('/', dirname($key));
    $skin = $arr[sizeof($arr)-1];
    $options[$skin] = $skin;
  }
  ksort($options);
  $form['skin'] = array(
    '#type' => 'select',
    '#title' => t('Skin'),
    '#default_value' => $tour->skin,
    '#options' => $options,
    '#description' => 'Choose the skin.',
  );  
  $form['text_of'] = array(
    '#type' => 'textfield',
    '#title' => t('Caption of page splitter'),
    '#default_value' => ($tour->id ? $tour->text_of : t('of')),
    '#maxlength' => 10,
    '#size' => 14,
    '#description' => t('e.g. Page 1 of 3, Page 1 / 3'),
  );
  $form['text_close'] = array(
    '#type' => 'textfield',
    '#title' => t('Caption of Close button'),
    '#default_value' => ($tour->id ? $tour->text_close : t('Close')),
    '#maxlength' => 20,
    '#size' => 24,
    '#description' => t('e.g. Close, Close me'),
  );
  $form['btn_prev'] = array(
    '#type' => 'textfield',
    '#title' => t('Caption of Previous button'),
    '#default_value' => ($tour->id ? $tour->btn_prev : t('Previous')),
    '#maxlength' => 30,
    '#size' => 40,
    '#description' => t('e.g. Previous'),
  );
  $form['btn_next'] = array(
    '#type' => 'textfield',
    '#title' => t('Caption of Next button'),
    '#default_value' => ($tour->id ? $tour->btn_next : t('Next')),
    '#maxlength' => 30,
    '#size' => 40,
    '#description' => t('e.g. Next'),
  );
  $options = array(t('Open the Close button url'), t('Close tour control and stay on current page'));
  $form['close_btn_action'] = array(
    '#type' => 'radios',
    '#title' => t('Close button behavior'),
    '#default_value' => ($tour->id ? $tour->close_btn_action : 0),
    '#options' => $options,
  );
  $options = array(t('No'), t('Yes'));
  $form['cover_body'] = array(
    '#type' => 'radios',
    '#title' => t('Cover the toured pages with a transparent layer'),
    '#default_value' => ($tour->id ? $tour->cover_body : 1),
    '#options' => $options,
  );
  $form['cover_click_close'] = array(
    '#type' => 'radios',
    '#title' => t('Close transparent layer on mouse click'),
    '#default_value' => ($tour->id ? $tour->cover_click_close : 1),
    '#options' => $options,
  );

  $form['pages_count'] = array(
    '#type' => 'value',
    '#value' => $tour->pages,
  );
  $form['id'] = array(
    '#type' => 'value',
    '#value' => $tour->id,
  );  
  $form['save_tour'] = array('#type' => 'submit', '#value' => t('Save tour'));

  return $form;
}

function site_tour_admin_edit_submit($form, &$form_state) {
  $tour = (object)$form_state['values'];
  $update = array();
  if ($tour->id) {
    $update = array('id');
  }
  $tour->description = truncate_utf8($tour->description, 255, TRUE, TRUE);
  $result = drupal_write_record('site_tour', $tour, $update);
  if (!$result) {
    drupal_set_message(t('There was an error saving the tour.'), 'error');
    return;
  }
  drupal_set_message(t('The tour has been successfully saved.'));
  if (!$tour->pages_count) {
    drupal_set_message(t('The tour has no pages. Click the <a href="@edit-pages">Edit pages</a> tab to add pages.', array('@edit-pages' => url('admin/content/site_tour/edit/'.$tour->id.'/pages'))));
  }
  $form_state['redirect'] = 'admin/content/site_tour/edit/'.$tour->id;
}

function site_tour_admin_edit_pages($form_values, $id = NULL) {
  $tour = db_fetch_object(db_query('SELECT * FROM {site_tour} WHERE id=%d', $id));
  drupal_set_title($tour->title);
  $form = array();

  $form['tour_title'] = array(
    '#type' => 'item',
    '#title' => t('Tour'),
    '#value' => $tour->title,
    '#description' => t('<a href="!url" target="_blank">View</a> the tour.', array('!url' => url('site_tour/'.$tour->url))),
    '#weight' => -10,
  );

  $sql = db_query("SELECT * FROM {site_tour_pages} WHERE tour=%d ORDER BY weight, title", $tour->id);
  $delta = db_affected_rows() + 1;
  
  $form['new'] = array(
    '#type' => 'fieldset',
    '#title' => t('Add a new page'),
    '#collapsible' => TRUE,
    '#collapsed' => $tour->pages,
    '#theme' => 'site_tour_page_add',
  ); 
  $form['new'] += _site_tour_page_form(array('delta' => $delta, 'tour' => $tour));

  $form['pages'] = array(
    '#tree' => TRUE,
    '#theme' => 'site_tour_pages',
  );
  while ($page = db_fetch_object($sql)) {
    $form['pages']['delete'][$page->id] = array('#type' => 'checkbox');
    $form['pages']['status'][$page->id] = array('#type' => 'checkbox', '#default_value' => $page->status);     
    $form['pages']['title'][$page->id] = array('#value' => $page->title);    
    $form['pages']['description'][$page->id] = array('#value' => strip_tags(truncate_utf8($page->description, 150, TRUE, TRUE)));
    $form['pages']['url'][$page->id] = array('#value' => $page->url);
    $form['pages']['weight'][$page->id] = array('#type' => 'weight', '#delta' => $delta, '#default_value' => $page->weight);
  }
  if ($tour->pages) {
    $form['pages']['submit'] = array('#type' => 'submit', '#value' => t('Save changes'));
  }

  $form['tour'] = $form['pages']['tour'] = array(
    '#type' => 'value',
    '#value' => $tour->id,
  );
  
  return $form;  
}

function site_tour_admin_edit_pages_validate($form, &$form_state) {
  $values = $form_state['values'];
  if (!$values['tour']) {
    drupal_set_message(t('Unable to retrieve the tour.'), 'error');
    $form_state['redirect'] = 'admin/content/site_tour';
    return;
  }
  switch ($values['op']) {
    case t('Add page'):
      foreach (array('title' => t('Title'), 'description' => t('Description'), 'url' => t('Url')) as $field => $label) {
        if (!$values[$field]) {
          form_set_error($field, t('!name field is required.', array('!name' => $label)));
        }
      }
      return;
  }
}

function site_tour_admin_edit_pages_submit($form, &$form_state) {
  $values = $form_state['values'];
  if (!$values['tour']) {
    drupal_set_message(t('Unable to retrieve the tour.'), 'error');
    $form_state['redirect'] = 'admin/content/site_tour';
    return;
  }
  
  switch ($values['op']) {
    case t('Add page'):
    case t('Save page'):
      $page = (object)$values;
      $update = array();
      if ($page->id) $update[] = 'id';
      $result = drupal_write_record('site_tour_pages', $page, $update);
      if (!$result) {
        drupal_set_message(t('Unable to save the page.'), 'error');
      }
      else {
        drupal_set_message(t('The page was successfully saved.'));
      }
      if (empty($update)) $form_state['redirect'] = 'admin/content/site_tour/edit/'.$page->tour.'/pages';
      break;

    case t('Save changes'):
      $update = array('id');
      foreach ($values['pages']['delete'] as $key => $val) {
        if ($val) {
          db_query("DELETE FROM {site_tour_pages} WHERE id=%d AND tour=%d", $key, $values['tour']);
        }
        else {
          $page = new stdClass();
          $page->id = $key;
          $page->tour = $values['tour'];
          $page->status = $values['pages']['status'][$key];
          $page->weight = $values['pages']['weight'][$key];
          drupal_write_record('site_tour_pages', $page, $update);
        }
      }
      drupal_set_message(t('The changes have been saved.'));
      break;      
  }

  // refresh tour pages counter
  $count = db_result(db_query("SELECT COUNT(*) FROM {site_tour_pages} WHERE tour=%d", $values['tour']));
  db_query("UPDATE {site_tour} SET pages=%d WHERE id=%d", $count, $values['tour']);
}

function site_tour_admin_edit_page($form_values, $tour = NULL, $id = NULL) {
  $page = db_fetch_array(db_query("SELECT * FROM {site_tour_pages} WHERE id=%d AND tour=%d", $id, $tour));
  if (!$page) {
    drupal_set_message(t('The requested page was not found.'), 'error');
    drupal_goto('admin/content/site_tour/edit/'.$tour.'/pages');
    exit();
  }

  $form = _site_tour_page_form($page);
  $form['id'] = array(
    '#type' => 'value',
    '#value' => $id,
  );
  $form['tour'] = array(
    '#type' => 'value',
    '#value' => $tour,
  );
  $form['#submit'] = array('site_tour_admin_edit_pages_submit');
  return $form;
}

function _site_tour_page_form($data = array()) {
  $data = (array)$data;
  if (!$data['delta']) $data['delta'] = db_result(db_query("SELECT pages FROM {site_tour} WHERE id=%d", $data['tour'])) + 1;

  $form = array();
  $form['title'] = array(
    '#type' => 'textfield',
    '#title' => t('Title'),
    '#maxlength' => 128,
    '#default_value' => $data['title'],
    '#size' => 50,
  );
  $form['description'] = array(
    '#type' => 'textarea',
    '#title' => t('Description'),
    '#default_value' => $data['description'],    
    '#rows' => 6,
    '#cols' => 50,
  );
  $form['url'] = array(
    '#type' => 'textfield',
    '#title' => t('Url'),
    '#default_value' => $data['url'],
    '#maxlength' => 128,
    '#size' => 50,
  );
  $form['weight'] = array(
    '#type' => 'weight',
    '#title' => t('Weight'),
    '#default_value' => isset($data['weight']) ? $data['weight'] : 1,
    '#delta' => ($data['delta'] ? $data['delta'] : 10),
  );
  $form['status'] = array(
    '#type' => 'select',
    '#title' => t('Status'),
    '#options' => array(1 => t('Visible'), 0 => t('Hidden')),
    '#default_value' => (isset($data['status']) ? $data['status'] : 1),
  );
  
  $form['submit'] = array('#type' => 'submit', '#value' => isset($data['id']) ? t('Save page') : t('Add page'));
  return $form;
}

/************************************************************
 * Theme functions
 ************************************************************/

function theme_site_tour_list($form) {
  $rows = array();
  foreach (element_children($form['title']) as $key) {
    $row = array();
    $url = drupal_render($form['url'][$key]);
    $row[] = drupal_render($form['tours'][$key]);
    $row[] = l(drupal_render($form['title'][$key]), 'site_tour/'.$url, array('attributes' => array('target' => '_blank')));
    $row[] = array('data' => drupal_render($form['description'][$key]), 'width' => '100%');
    $row[] = l($url, 'site_tour/'.$url, array('attributes' => array('target' => '_blank')));
    $row[] = drupal_render($form['status'][$key]);
    $row[] = drupal_render($form['pages'][$key]);
    $row[] = l(t('edit tour'), 'admin/content/site_tour/edit/'.$key).'<br />'.
      l(t('edit pages'), 'admin/content/site_tour/edit/'.$key.'/pages');
    $rows[] = $row;
  }

  $output = drupal_render($form['options']);
  $select_header = empty($rows)? '' : theme('table_select_header_cell');
  $header = array($select_header, t('Title'), t('Description'), t('Url'), t('Status'), t('Pages'), t('Operations'));
  if (!empty($rows)) $output .= theme('table', $header, $rows, array('id' => 'site_tour_list'));
  $output .= drupal_render($form);
  return $output;
}

function theme_site_tour_pages($form) {
  $tour = $form['tour']['#value'];
  $rows = array();
  drupal_add_tabledrag('site_tour_pages', 'order', 'sibling', 'site_tour_pages-weight');
  foreach (element_children($form['title']) as $key) {
    // Add class to group weight fields for drag and drop.
    $form['weight'][$key]['#attributes']['class'] = 'site_tour_pages-weight';
       
    $row = array();
    $row[] = '';
    $row[] = drupal_render($form['delete'][$key]);
    $row[] = drupal_render($form['status'][$key]);
    $row[] = drupal_render($form['weight'][$key]);
    $row[] = drupal_render($form['title'][$key]);
    $row[] = array('data' => drupal_render($form['description'][$key]), 'width' => '100%');
    $row[] = drupal_render($form['url'][$key]);
    $row[] = l(t('edit'), 'admin/content/site_tour/edit/'.$tour.'/pages/'.$key);
    $rows[] = array('data' => $row, 'class' => 'draggable');
  }

  $output = '';
  $header = array('', t('Delete'), t('List'), t('Weight'), t('Title'), t('Description'), t('Url'), t('Operations'));
  if (!empty($rows)) $output .= theme('table', $header, $rows, array('id' => 'site_tour_pages'));
  $output .= drupal_render($form);
  return $output;
}

function theme_site_tour_page_add(&$form) {
  $output = '';
  foreach (array('title', 'description') as $key) {
    $output .= drupal_render($form[$key]);
  }
  foreach (array('url', 'status', 'weight') as $key) { 
    $output .= '<div style="float:left; margin-right:20px;">';
    $output .= drupal_render($form[$key]);
    $output .= '</div>';
  }
  $output .= '<div style="clear:both;"></div>';
  $output .= drupal_render($form);
  return $output;
}