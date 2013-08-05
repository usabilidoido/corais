<?php

/**
 * @file
 * Big Blue Button - Enables universities and colleges to deliver a high-quality
 * learning experience.
 *
 * @author
 * Stefan Auditor <stefan.auditor@erdfisch.de>
 *
 * NOTE: Parameters marked with (NFS) are not yet fully supported. You can pass
 * them in to the controller, but they may not have any actual effect on the
 * conference. This is more a limitation based on support by the actual BBB
 * application more than the API itself.
 */

// Debug mode
define('BBB_API_DEBUG', FALSE);

// Define API URL
define('BBB_API_BASE_URL', variable_get('bbb_base_url', 'http://YOUR_BIGBLUEBUTTON_SERVER.TLD/bigbluebutton/api/'));
// Define API calls
define('BBB_API_CREATE',             'create');
define('BBB_API_JOIN',               'join');
define('BBB_API_IS_MEETING_RUNNING', 'isMeetingRunning');
define('BBB_API_END_MEETING',        'end');
define('BBB_API_GET_MEETING_INFO',   'getMeetingInfo');
define('BBB_API_GET_MEETINGS',       'getMeetings');

/**
 * Create Meeting (create)
 *
 * @param ARRAY $params
 *   Associative array of additional url parameters. Components:
 *   - name: STRING (REQUIRED) A name for the meeting.
 *   - meetingID: STRING A meeting ID that can be used to identify this meeting
 *     by the third party application. This must be unique to the server that
 *     you are calling. If you supply a non-unique meeting ID, you will still
 *     have a successful call, but will receive a warning message in the
 *     response.
 *   - attendeePW: STRING The password that will be required for attendees to
 *     join the meeting. This is optional, and if not supplied, BBB will assign
 *     a random password.
 *   - moderatorPW: STRING The password that will be required for moderators to
 *     join the meeting or for certain administrative actions (i.e. ending a
 *     meeting). This is optional, and if not supplied, BBB will assign a
 *     random password.
 *   - welcome: STRING A welcome message that gets displayed on the chat window
 *     when the participant joins. You can include keywords (%%CONFNAME%%,
 *     %%DIALNUM%%, %%CONFNUM%%) which will be substituted automatically. You
 *     can set a default welcome message on bigbluebutton.properties
 *   - dialNumber: STRING The dial access number that participants can call in
 *     using regular phone. You can set a default dial number on
 *     bigbluebutton.properties
 *   - voiceBridge: STRING Voice conference number that participants enter to
 *     join the voice conference. Note that if you don't specify a voiceBridge
 *     parameter, the meetingID parameter is used. The default pattern for this
 *     is a 5-digit number, because in the default Asterisk configuration, this
 *     is the PIN that a dial-in user must enter to join the conference. If you
 *     want to change this pattern, you have to edit
 *     /etc/asterisk/bbb_extensions.conf
 *   - logoutURL: STRING The URL that the BigBlueButton client will go to after
 *     users click the OK button on the 'You have been logged out message'.
 *     This overrides, the value for bigbluebutton.web.loggedOutUrl if defined
 *     in bigbluebutton.properties
 *   - maxParticipants: NUMBER The maximum number of participants to allow into
 *     the meeting (including moderators). After this number of participants
 *     have joined, BBB will return an appropriate error for other users trying
 *     to join the meeting. A negative number indicates that an unlimited
 *     number of participants should be allowed (this is the default setting).
 * @return OBJECT or FALSE
 *   - meetingID: STRING The meeting ID supplied by the third party app. It
 *     must be used in conjunction with a password in subsequent calls for
 *     joining or otherwise modifying a meeting's status.
 *   - attendeePW: STRING The password that will be required for attendees to
 *     join the meeting. If you did not supply one, BBB will assign a random
 *     password.
 *   - moderatorPW: STRING The password that will be required for moderators to
 *     join the meeting or for certain administrative actions (i.e. ending a
 *     meeting). If you did not supply one, BBB will assign a random password.
 *
 * @see http://code.google.com/p/bigbluebutton/wiki/API#Create_Meeting_(create)
 */
function bbb_api_create($params = array()) {
  $query_string = bbb_api_generate_querystring(BBB_API_CREATE, $params);
  $request = BBB_API_BASE_URL . BBB_API_CREATE . '?' . $query_string;
  return bbb_api_execute_query($request);
}

/**
 * Join Meeting (join)
 *
 * @param ARRAY $params
 *   Associative array of additional url parameters. Components:
 *   - fullName: STRING (REQUIRED) The full name that is to be used to identify
 *     this user to other conference attendees.
 *   - meetingID: STRING (REQUIRED) If you specified a meeting ID when calling
 *     create, then you can use either the generated meeting token or your
 *     specified meeting ID to find meetings. This parameter takes your meeting
 *     ID.
 *   - password: STRING (REQUIRED) The password that this attendee is using. If
 *     the moderator password is supplied, he will be given moderator status
 *     (and the same for attendee password, etc)
 *   - userID: STRING An identifier for this user that will help your
 *     application to identify which person this is. This user ID will be
 *     returned for this user in the getMeetingInfo API call so that you can
 *     check
 *  - webVoiceConf: STRING If you want to pass in a custom voice-extension when
 *    a user joins the voice conference using voip. This is useful if you want
 *    to collect more info in you Call Detail Records about the user joining the
 *    conference. You need to modify your /etc/asterisk/bbb-extensions.conf to
 *    handle this new extensions.
 * @return
 *   There is no XML response for this call if it is properly formatted. You
 *   should simply redirect the user to the call URL, and they will be entered
 *   into the meeting.
 *   NOTE: there is the possibility that a user is sent to a join URL that is
 *   invalid right now. For instance, if the client application forms the URL
 *   incorrectly, or sends the user to join a meeting that hasn't been created,
 *   then the user will end up seeing the XML that would be returned as an
 *   error. We need to figure out a better way of handling this.
 * @see http://code.google.com/p/bigbluebutton/wiki/API#Join_Meeting_(join)
 */
function bbb_api_join($params) {
  $query_string = bbb_api_generate_querystring(BBB_API_JOIN, $params);
  return BBB_API_BASE_URL . BBB_API_JOIN . '?' . $query_string;
}

/**
 * Is meeting running (isMeetingRunning)
 *
 * This call enables you to simply check on whether or not a meeting is running
 * by looking it up with your meeting ID.
 *
 * @param ARRAY $params
 *   Associative array of additional url parameters. Components:
 *   - meetingID: STRING (REQUIRED) The meeting ID that identifies the meeting
 *     you are attempting to check on.
 * @return BOOLEAN
 *   A string of either “true” or “false” that signals whether a meeting with
 *   this ID or token is currently running.
 * @see http://code.google.com/p/bigbluebutton/wiki/API#Is_meeting_running_(isMeetingRunning)
 */
function bbb_api_isMeetingRunning($params) {
  $query_string = bbb_api_generate_querystring(BBB_API_IS_MEETING_RUNNING, $params);
  $request = BBB_API_BASE_URL . BBB_API_IS_MEETING_RUNNING . '?' . $query_string;
  return bbb_api_execute_query($request);
}

/**
 * End Meeting (end)
 *
 * Use this to forcibly end a meeting and kick all participants out of the
 * meeting.
 *
 * @param ARRAY $params
 *   Associative array of additional url parameters. Components:
 *   - meetingID: STRING The meeting ID that identifies the meeting you are
 *     attempting to end.
 *   - password: STRING The moderator password for this meeting. You can not
 *     end a meeting using the attendee password.
 *  @return OBJECT or FALSE
 *    This call returns no additional information other than the standard
 *    returncode parameter of "SUCCESS" or "FAILED". It will return a
 *    messageKey and message for additional information.
 * @see http://code.google.com/p/bigbluebutton/wiki/API#End_Meeting_(end)
 */
function bbb_api_end($params) {
  $query_string = bbb_api_generate_querystring(BBB_API_END_MEETING, $params);
  return BBB_API_BASE_URL . BBB_API_END_MEETING . '?' . $query_string;
}

/**
 * Get Meeting Info (getMeetingInfo)
 *
 * This call will return all of a meeting's information, including the list of
 * attendees as well as start and end times.
 *
 * @param ARRAY $params
 *   Associative array of additional url parameters. Components:
 *   - meetingID: STRING If you specified a meeting ID when calling create,
 *     then you can use either the generated meeting token or your specified
 *     meeting ID to find meetings. This parameter takes your meeting ID.
 *   - password: STRING (REQUIRED) The moderator password for this meeting. You
 *     can not get the meeting information using the attendee password.
 * @return OBJECT or FALSE
 * @see http://code.google.com/p/bigbluebutton/wiki/API#Get_Meeting_Info_(getMeetingInfo)
 */
function bbb_api_getMeetingInfo($params) {
  $query_string = bbb_api_generate_querystring(BBB_API_GET_MEETING_INFO, $params);
  $request = BBB_API_BASE_URL . BBB_API_GET_MEETING_INFO . '?' . $query_string;
  return bbb_api_execute_query($request);
}

/**
 * Get Meetings (getMeetings)
 *
 * This call will return a list of all the meetings found on this server.
 *
 * @param ARRAY $params
 *   Associative array of additional url parameters. Components:
 *   - random: STRING (REQUIRED) This can be any value. It must be supplied so
 *     that we can use your security salt and do a checksum verification that
 *     you are the server's owner. This is for security purposes. If you do not
 *     supply the random parameter, the call will fail immediately.
 * @return OBJECT or FALSE
 * @see http://code.google.com/p/bigbluebutton/wiki/API#Get_Meetings_(getMeetings)
 */
function bbb_api_getMeetings($params) {
  $query_string = bbb_api_generate_querystring(BBB_API_GET_MEETINGS, $params);
  $request = BBB_API_BASE_URL . BBB_API_GET_MEETINGS . '?' . $query_string;
  return bbb_api_execute_query($request);
}

/* Helper functions */

/**
 * Generate a signed query string
 */
function bbb_api_generate_querystring($call, $params = array()) {
  $query = array();
  // Create the entire query string for your API call without the checksum
  // parameter URL encoding the parameters
  foreach ($params as $key => $value) {
    $query[] = $key . '=' . drupal_urlencode(trim($value));
  }
  // Putting it together
  $query_string = implode('&', $query);
  return $query_string . '&checksum=' . sha1($call . $query_string . BBB_API_SECURITY_SALT);
}

/**
 *
 */
function bbb_api_execute_query($request) {
  bbb_api_debug($request); //debug
  $xml = @simplexml_load_file($request);
  $response = bbb_api_parse_response($xml);
  bbb_api_debug($response); //debug
  if ($response->returncode == 'SUCCESS') {
    unset($response->returncode);
    if ($response->running) {
      // Switch to boolean
      $response->running = (strtoupper($response->running) == 'TRUE') ? TRUE : FALSE;
    }
    return $response;
  }
  else {
    watchdog('big blue button', '@message_key: @message', array('@message_key' => $response->messageKey, '@message' => t($response->message)), WATCHDOG_ERROR);
    return FALSE;
  }
}

/**
 * Parse xml response
 *
 * @param XML
 * @return OBJECT
 */
function bbb_api_parse_response($xml) {
  //TODO: Refactor
  $response = new StdClass;
  if ($xml) {
    foreach ($xml as $element => $node) {
      $response->$element = (string) $node;
    }
  }
  return $response;
}

/**
 * Custom debugging
 */
function bbb_api_debug($var) {
  if (BBB_API_DEBUG) {
    debug($var);
    watchdog('big blue button debug', '%message', array('%message' => '<pre>' . print_r($var, 1) . '</pre>'), WATCHDOG_DEBUG);
  }
}