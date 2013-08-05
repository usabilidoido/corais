<?php

/**
 * Implementation of hook_context_default_contexts().
 */
function atrium_book_context_default_contexts() {
  $export = array();

  $context = new stdClass;
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'book_casetracker';
  $context->description = '';
  $context->tag = 'Casetracker';
  $context->conditions = array(
    'bookroot' => array(
      'values' => array(
        'casetracker_basic_project' => 'casetracker_basic_project',
      ),
      'options' => array(
        'node_form' => 1,
      ),
    ),
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(
        'diff-inline' => array(
          'module' => 'diff',
          'delta' => 'inline',
          'region' => 'right',
          'weight' => 0,
        ),
        'atrium_book-book' => array(
          'module' => 'atrium_book',
          'delta' => 'book',
          'region' => 'right',
          'weight' => 1,
        ),
      ),
    ),
    'menu' => 'casetracker',
  );
  $context->condition_mode = 0;

  // Translatables
  // Included for use with string extractors like potx.
  t('Casetracker');
  $export['book_casetracker'] = $context;

  $context = new stdClass;
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'book_flag_knowledge';
  $context->description = '';
  $context->tag = 'Book';
  $context->conditions = array(
    'node' => array(
      'values' => array(
        'book' => 'book',
      ),
      'options' => array(
        'node_form' => '1',
      ),
    ),
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(
        'views-13e90326bc004b54d98353a711f648c2' => array(
          'module' => 'views',
          'delta' => '13e90326bc004b54d98353a711f648c2',
          'region' => 'left',
          'weight' => 5,
        ),
        'views-book_users-block_4' => array(
          'module' => 'views',
          'delta' => 'book_users-block_4',
          'region' => 'right',
          'weight' => 1,
        ),
        'views-book_users-block_2' => array(
          'module' => 'views',
          'delta' => 'book_users-block_2',
          'region' => 'right',
          'weight' => 4,
        ),
        'views-book_users-block_3' => array(
          'module' => 'views',
          'delta' => 'book_users-block_3',
          'region' => 'right',
          'weight' => 6,
        ),
      ),
    ),
  );
  $context->condition_mode = 0;

  // Translatables
  // Included for use with string extractors like potx.
  t('Book');
  $export['book_flag_knowledge'] = $context;

  $context = new stdClass;
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'book_listing';
  $context->description = '';
  $context->tag = 'Book';
  $context->conditions = array(
    'bookroot' => array(
      'values' => array(
        'book' => 'book',
      ),
      'options' => array(
        'node_form' => '1',
      ),
    ),
    'node' => array(
      'values' => array(
        'book' => 'book',
        'toolkit' => 'toolkit',
      ),
      'options' => array(
        'node_form' => '1',
      ),
    ),
    'path' => array(
      'values' => array(
        'knowledge' => 'knowledge',
      ),
    ),
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(
        'views-add_task_group-block_1' => array(
          'module' => 'views',
          'delta' => 'add_task_group-block_1',
          'region' => 'page_tools',
          'weight' => 0,
        ),
        'block-1' => array(
          'module' => 'block',
          'delta' => '1',
          'region' => 'left',
          'weight' => 0,
        ),
        'views-my_flagged-block_4' => array(
          'module' => 'views',
          'delta' => 'my_flagged-block_4',
          'region' => 'left',
          'weight' => 1,
        ),
        'atrium_book-book_all' => array(
          'module' => 'atrium_book',
          'delta' => 'book_all',
          'region' => 'left',
          'weight' => 2,
        ),
        'views-cards-block_1' => array(
          'module' => 'views',
          'delta' => 'cards-block_1',
          'region' => 'left',
          'weight' => 3,
        ),
        'creativecommons-site_license' => array(
          'module' => 'creativecommons',
          'delta' => 'site_license',
          'region' => 'left',
          'weight' => 4,
        ),
        'block-4' => array(
          'module' => 'block',
          'delta' => '4',
          'region' => 'right',
          'weight' => 0,
        ),
        'diff-inline' => array(
          'module' => 'diff',
          'delta' => 'inline',
          'region' => 'right',
          'weight' => 2,
        ),
        'views-comments-block_1' => array(
          'module' => 'views',
          'delta' => 'comments-block_1',
          'region' => 'right',
          'weight' => 3,
        ),
        'commentblock-comment_form' => array(
          'module' => 'commentblock',
          'delta' => 'comment_form',
          'region' => 'right',
          'weight' => 4,
        ),
        'views-xref-block_1' => array(
          'module' => 'views',
          'delta' => 'xref-block_1',
          'region' => 'footer',
          'weight' => 0,
        ),
      ),
      'layout' => 'columns',
    ),
    'menu' => 'knowledge',
  );
  $context->condition_mode = 0;

  // Translatables
  // Included for use with string extractors like potx.
  t('Book');
  $export['book_listing'] = $context;

  $context = new stdClass;
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'book_talk';
  $context->description = '';
  $context->tag = 'Book';
  $context->conditions = array(
    'path' => array(
      'values' => array(
        '*/talk' => '*/talk',
      ),
    ),
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(),
      'layout' => 'default',
    ),
    'menu' => 'texts',
  );
  $context->condition_mode = 0;

  // Translatables
  // Included for use with string extractors like potx.
  t('Book');
  $export['book_talk'] = $context;

  $context = new stdClass;
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'bookhome';
  $context->description = 'Book home';
  $context->tag = 'Book';
  $context->conditions = array(
    'path' => array(
      'values' => array(
        'knowledge' => 'knowledge',
      ),
    ),
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(
        'finder-finder_4' => array(
          'module' => 'finder',
          'delta' => 'finder_4',
          'region' => 'right',
          'weight' => 1,
        ),
      ),
      'layout' => 'columns',
    ),
    'menu' => 'notebook',
  );
  $context->condition_mode = 0;

  // Translatables
  // Included for use with string extractors like potx.
  t('Book');
  t('Book home');
  $export['bookhome'] = $context;

  return $export;
}
