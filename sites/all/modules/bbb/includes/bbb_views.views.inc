<?php

/**
 * @file
 * Big Blue Button - Enables universities and colleges to deliver a high-quality
 * learning experience.
 *
 * @author
 * Stefan Auditor <stefan.auditor@erdfisch.de>
 */

/**
 * Implement hook_views_data().
 */
function bbb_views_views_data() {
  // Group
  $data['bbb_meetings']['table']['group'] = t('Big Blue Button');
  // Joins
  $data['bbb_meetings']['table']['join']['node'] = array(
    'left_field' => 'nid',
    'field' => 'nid',
    'type' => 'INNER'
  );
  // Mappings
  $data['bbb_meetings']['meetingToken'] = array(
    'title' => t('Meeting Token'),
    'help' => t('The unique meeting token.'),
    'field' => array(
        'handler' => 'views_handler_field',
    ),
    'filter' => array(
        'handler' => 'views_handler_filter_string',
    ),
    'sort' => array(
        'handler' => 'views_handler_sort',
    ),
  );
  $data['bbb_meetings']['name'] = array(
    'title' => t('Username'),
    'help' => t('The username displayed in the meeting.'),
    'field' => array(
        'handler' => 'views_handler_field',
    ),
    'filter' => array(
        'handler' => 'views_handler_filter_string',
    ),
    'sort' => array(
        'handler' => 'views_handler_sort',
    ),
  );
  $data['bbb_meetings']['meetingID'] = array(
    'title' => t('Meeting ID'),
    'help' => t('The unique meeting ID.'),
    'field' => array(
        'handler' => 'views_handler_field',
    ),
    'filter' => array(
        'handler' => 'views_handler_filter_string',
    ),
    'sort' => array(
        'handler' => 'views_handler_sort',
    ),
  );
  $data['bbb_meetings']['attendeePW'] = array(
    'title' => t('Attendee password'),
    'help' => t('The password needed to join a meeting.'),
    'field' => array(
        'handler' => 'views_handler_field',
    ),
  );
  $data['bbb_meetings']['moderatorPW'] = array(
    'title' => t('Moderator password'),
    'help' => t('The password needed to moderate a meeting.'),
    'field' => array(
        'handler' => 'views_handler_field',
    ),
  );
  $data['bbb_meetings']['welcome'] = array(
    'title' => t('Welcome message'),
    'help' => t('The message displayed when joining a meeting.'),
    'field' => array(
        'handler' => 'views_handler_field',
    ),
  );
  $data['bbb_meetings']['dialNumber'] = array(
    'title' => t('Dial number'),
    'help' => t('The number to dial to join the meeting by phone.'),
    'field' => array(
        'handler' => 'views_handler_field',
    ),
  );
  $data['bbb_meetings']['logoutURL'] = array(
    'title' => t('Logout URL'),
    'help' => t('The URL the user gets redirected to when leaving the meeting.'),
    'field' => array(
        'handler' => 'views_handler_field_url',
    ),
  );
  $data['bbb_meetings']['created'] = array(
    'title' => t('Created'),
    'help' => t('The creation timestamp.'),
    'field' => array(
        'handler' => 'views_handler_field_date',
    ),
    'filter' => array(
        'handler' => 'views_handler_filter_date',
    ),
    'sort' => array(
        'handler' => 'views_handler_sort_date',
    ),
  );
  $data['bbb_meetings']['join_attendee'] = array(
    'field' => array(
      'title' => t('Join'),
      'help' => t('Provide a simple link to join as attendee.'),
      'handler' => 'views_handler_field_bbb_join_attendee',
      'notafield' => TRUE,
    ),
  );
  $data['bbb_meetings']['join_moderator'] = array(
    'field' => array(
      'title' => t('Join moderator'),
      'help' => t('Provide a simple link to join as moderator.'),
      'handler' => 'views_handler_field_bbb_join_moderator',
      'notafield' => TRUE,
    ),
  );

  // Group
  $data['bbb_stats']['table']['group'] = t('Big Blue Button Stats');
  // Joins
  $data['bbb_stats']['table']['join']['node'] = array(
    'left_field' => 'nid',
    'field' => 'nid',
    'type' => 'INNER'
  );
  $data['bbb_stats']['nid'] = array(
    'title' => t('Node'),
    'help' => t('Relation to the node table'),
    'relationship' => array(
        'base' => 'node',
        'field' => 'nid',
        'handler' => 'views_handler_relationship',
        'label' => t('Node'),
    ),
  );
  $data['bbb_stats']['table']['join']['users'] = array(
    'left_field' => 'uid',
    'field' => 'uid',
    'type' => 'INNER'
  );
  // Mappings
  $data['bbb_stats']['mode'] = array(
    'title' => t('Modus'),
    'help' => t("The user's mode in the meeting."),
    'field' => array(
        'handler' => 'views_handler_field',
    ),
    'filter' => array(
        'handler' => 'views_handler_filter_string',
    ),
    'sort' => array(
        'handler' => 'views_handler_sort',
    ),
  );
  $data['bbb_stats']['joined'] = array(
    'title' => t('Joined'),
    'help' => t('The timestamp a user joined the meeting.'),
    'field' => array(
        'handler' => 'views_handler_field_date',
    ),
    'filter' => array(
        'handler' => 'views_handler_filter_date',
    ),
    'sort' => array(
        'handler' => 'views_handler_sort_date',
    ),
  );

  return $data;
}
