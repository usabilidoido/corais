<?php

class CourseObjectBook extends CourseObjectNode {

  function getNodeTypes() {
    return array('book');
  }

  /**
   * Make the book.
   */
  public function create() {
    $node = new stdClass;
    $node->type = $this->getComponent();
    $node->title = $this->getTitle();
    $node->uid = $this->user->uid;
    $node->book['bid'] = 'new';
    node_save($node);
    $this->setNode($node);
  }

  function optionsDefinition() {
    $defaults = parent::optionsDefinition();
    $defaults['book_tracking'] = 'all';
    $defaults['outline_list_item_type'] = 'active_tree';
    return $defaults;
  }

  function optionsForm(&$form, &$form_state) {
    $config = $this->optionsDefinition();
    parent::optionsForm($form, $form_state);
    $form['book_tracking'] = array(
      '#title' => t('Completion criteria'),
      '#type' => 'select',
      '#options' => array(
        'one' => 'View any page',
        'all' => 'View all pages',
      ),
      '#default_value' => $config['book_tracking'],
    );

    // Add a book-specific configuration for course outline list item type, only
    // if the standard course list outline handler is selected.
    if ($this->getCourse()->getNode()->course['outline'] == 'course_outline_list') {
      $form['outline_list_item_type'] = array(
        '#title' => t('Course outline list item type'),
        '#type' => 'select',
        '#options' => array(
          'all_pages' => t('All book pages as an expanded, nested list'),
          'active_tree' => t('Only the active book menu tree items, with a count indicator'),
          'count' => t('A count indicator only'),
        ),
        '#default_value' => $config['outline_list_item_type'],
      );
    }
  }

  /**
   * Grade (track) the book based on the fulfillment data.
   */
  function grade() {
    if (course_book_count($this->node->nid) == 0) {
      // Book has no pages. Complete object.
      $this->getFulfillment()->setComplete(1)->save();
      return;
    }

    if ($this->getOption('book_tracking') == 'all') {
      $mlids = array_keys(book_toc($this->node->nid, array(), 99));
      $viewed = array_keys(array_filter($this->getFulfillment()->getOption('book_fulfillment')));
      if (!array_diff($mlids, $viewed)) {
        $this->getFulfillment()->setComplete(1)->save();
      }
    }
    elseif ($this->getOption('book_tracking') == 'one') {
      $this->getFulfillment()->setComplete(1)->save();
    }
  }

  /**
   * Overrides navigation links.
   */
  public function overrideNavigation() {
    $links = parent::overrideNavigation();

    $node = node_load(arg(1));
    if (isset($node->book)) {
      $book_link = $node->book;
      if ($prev = book_prev($book_link)) {
        $links['prev'] = l('Previous', $prev['href']);
      }
      if ($next = book_next($book_link)) {
        $links['next'] = l('Next', $next['href']);
      }
    }

    return $links;
  }

  /**
   * Overrides a course outline list item.
   */
  public function overrideOutlineListItem(&$item) {
    // Check that course list outline handler is selected.
    if ($this->getCourse()->getNode()->course['outline'] == 'course') {
      $type = $this->getOption('outline_list_item_type');
      // Override the list item by reference.
      course_book_override_outline_list_item($item, $this, $type);
    }
  }

  public function getCloneAbility() {
    return t('%title will only clone the first page.', array('%title' => $this->getTitle()));
  }

}
