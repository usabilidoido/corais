<?php

/**
 * @file
 * Support file for the core book module.
 */

/**
 * Implementation of hook_node_import_fields().
 */
function book_node_import_fields($type) {
  $fields = array();

  if (($node_type = node_import_type_is_node($type)) !== FALSE && book_type_is_allowed($node_type) &&
      (user_access('administer book outlines') || user_access('add content to books'))) {

    $fields['book:book'] = array(
      'title' => t('Book'),
      'group' => t('Book outline'),
      'module' => 'book',
      'input_format' => 'book_reference',
    );

    $fields['book:parent'] = array(
      'title' => t('Parent item'),
      'group' => t('Book outline'),
      'module' => 'book',
      'input_format' => 'book_page_reference',
    );

    $fields['book:weight'] = array(
      'title' => t('Weight'),
      'group' => t('Book outline'),
      'module' => 'book',
      'input_format' => 'weight',
      'delta' => 15,
    );
  }

  return $fields;
}

/**
 * Implementation of hook_node_import_fields_alter().
 */
function book_node_import_fields_alter(&$fields, $type) {
  foreach ($fields as $fieldname => $fieldinfo) {
    switch ($fieldinfo['input_format']) {
      case 'book_reference':
        $fields[$fieldname]['preprocess'][] = 'node_import_check_book_reference';
        break;

      case 'book_page_reference':
        // Because the book page reference needs to know what book we are
        // adding the page to, we can't preprocess here, but need to do
        // it in book_node_import_values_alter().
        break;
    }
  }
}

/**
 * Check whether the value is a book (by NID or Title).
 *
 * Uses: nothing.
 *
 * @ingroup node_import_preprocess
 */
function node_import_check_book_reference(&$value, $field, $options, $preview) {
  static $first_run = TRUE;

  if ($first_run) {
    foreach (node_import_extract_property(book_get_books()) as $nid => $title) {
      node_import_set_object('book', $title, $nid);
      node_import_set_object('book', $nid, $nid);
      node_import_set_object('book:title', $nid, $title);
    }
    $first_run = FALSE;
  }

  if ($value == '<none>') {
    // Not in a book.
    $value = '';
    return TRUE;
  }
  else if ($value == '<new>') {
    // A new top-level book.
    $value = 'new';
    return TRUE;
  }
  else if (($bid = node_import_get_object('book', $value)) !== NULL) {
    // An existing book.
    $value = $bid;
    return TRUE;
  }

  node_import_input_error(t('Input error: %value is not allowed for %name (not a book).', array('%value' => $value, '%name' => $field['title'])));
  return FALSE;
}

/**
 * Implementation of hook_node_import_defaults().
 */
function book_node_import_defaults($type, $defaults, $fields, $map) {
  $form = array();

  if (($node_type = node_import_type_is_node($type)) !== FALSE && book_type_is_allowed($node_type) &&
      (user_access('administer book outlines') || user_access('add content to books'))) {

    $book_options = array(
      '<new>' => '<'. t('create a new book') .'>',
      '<none>' => '<'. t('none') .'>',
    ) + node_import_extract_property(book_get_books());

    $form['book:book'] = array(
      '#title' => t('Book'),
      '#type' => 'select',
      '#options' => $book_options,
      '#default_value' => isset($defaults['book:book']) ? $defaults['book:book'] : ($type == 'node:book' ? '<new>' : '<none>'),
    );

    $form['book:weight'] = array(
      '#title' => t('Weight'),
      '#type' => 'weight',
      '#delta' => 15,
      '#default_value' => isset($defaults['book:weight']) ? $defaults['book:weight'] : '0',
    );
  }

  return $form;
}

/**
 * Implementation of hook_node_import_values_alter().
 */
function book_node_import_values_alter(&$values, $type, $defaults, $options, $fields, $preview) {
  static $virtual_book = 0;
  static $virtual_book_page = 0;

  if (($node_type = node_import_type_is_node($type)) !== FALSE && book_type_is_allowed($node_type) &&
      (user_access('administer book outlines') || user_access('add content to books'))) {

    $book = $values['book:book'];
    $parent = $values['book:parent'];
    $weight = $values['book:weight'];

    unset($values['book:book']);
    unset($values['book:parent']);
    unset($values['book:weight']);

    // Not in a book.
    if (drupal_strlen($book) == 0) {
      $values['book'] = array(
        'bid' => 0,
        'weight' => $weight,
      );
      return;
    }

    // A new top-level book.
    if ($book == 'new') {
      if ($preview) {
        drupal_set_message(t('A new book %title will be created.', array('%title' => $values['title'])));
        $virtual_book--;
        node_import_set_object('book', $values['title'], $virtual_book);
        node_import_set_object('book:title', $virtual_book, $values['title']);
      }
      else {
        // When a new book is created, the form will no longer validate
        // as book_get_books() is cached.
        global $node_import_can_continue;
        $node_import_can_continue = FALSE;
      }

      $values['book'] = array(
        'bid' => $book,
        'weight' => $weight,
      );
      return;
    }

    // A new page in an existing book.
    $plid = node_import_get_object('book:'. $book, $parent);

    if ($plid === NULL && $book > 0 && drupal_strlen($parent) > 0) {
      if (($plid = db_result(db_query("SELECT mlid FROM {menu_links} WHERE menu_name = '%s' AND module = 'book' AND (LOWER(link_title) = '%s' OR link_path = '%s')", book_menu_name($book), drupal_strtolower($parent), 'node/'. (is_numeric($parent) && intval($parent) > 0 ? $parent : ''))))) {
        node_import_set_object('book:'. $book, $parent, $plid);
      }
    }

    if ($book > 0 && isset($plid) && $plid > 0) {
      $values['book'] = array(
        'plid' => $plid,
        'weight' => $weight,
        'bid' => $book,
      );
      return;
    }
    else if ($book > 0 && drupal_strlen($parent) == 0) {
      $values['book'] = array(
        'bid' => $book,
        'weight' => $weight,
      );
      return;
    }

    if ($preview) {
      $virtual_book_page--;
      node_import_set_object('book:'. $book, $values['title'], $virtual_book_page);
      node_import_set_object('book:'. $book .':title', $virtual_book_page, $values['title']);

      if ($book < 0) {
        drupal_set_message(t('This will be a page in %title.', array('%title' => node_import_get_object('book:title', $book))));
      }
      else {
        $values['book'] = array(
          'bid' => $book,
          'weight' => $weight,
        );
      }

      if ($plid < 0) {
        drupal_set_message(t('This will be a child page of %title.', array('%title' => node_import_get_object('book:'. $book .':title', $plid))));
        return;
      }
      else if (drupal_strlen($parent) == 0) {
        drupal_set_message(t('This will be the top-level page in this book.'));
        return;
      }
   }

    node_import_input_error(t('Input error: %value is not allowed for %name (not a book page in %book).', array('%value' => $parent, '%name' => t('Parent item'), '%book' => node_import_get_object('book:title', $book))));
  }
}

/**
 * Implementation of hook_node_import_postprocess().
 */
function book_node_import_postprocess($type, $values, $options, $preview) {
  if (!$preview && isset($values['nid']) &&
      ($node_type = node_import_type_is_node($type)) !== FALSE && book_type_is_allowed($node_type) &&
      (user_access('administer book outlines') || user_access('add content to books')) &&
      isset($values['book']['plid']) && $values['book']['plid'] > 0 &&
      ($mlid = db_result(db_query("SELECT mlid FROM {menu_links} WHERE module = 'book' AND link_path = '%s'", 'node/'. $values['nid'])))) {

    $menu_link = array(
      'menu_name' => book_menu_name($values['book']['bid']),
      'weight' => $values['book']['weight'],
      'mlid' => $mlid,
      'plid' => $values['book']['plid'],
      'router_path' => 'node/%',
      'link_path' => 'node/'. $values['nid'],
      'link_title' => $values['title'],
      'module' => 'book',
    );
    menu_link_save($menu_link);
  }
}

