<?php
/**
 * @file
 * Administrative functions to configure DrupalChat.
 */ 

/**
 * Callback for admin/settings/drupalchat.
 */
function drupalchat_settings_form() {
  drupal_add_css('misc/farbtastic/farbtastic.css');
  drupal_add_js('misc/farbtastic/farbtastic.js');
  drupal_add_js(drupal_get_path('module', 'drupalchat') . '/drupalchat.admin.js', 'module', 'footer');
  /*
  drupalchat_yui_path();
  if ((variable_get('drupalchat_yui_use_cdn', 1) == 2) && (!variable_get('drupalchat_yui_path', NULL))) {
    drupal_set_message(t("You need to download the latest stable release of the !library and extract the entire folder into sites/all/libraries.", array('!library' => l(t('YUI library'), 'http://developer.yahoo.com/yui/3/', array('attributes' => array('target' => '_blank'))))));
  }
  */
  $seconds = array(1 => 1, 2 => 2, 3 => 3, 4 => 4, 5 => 5, 6 => 6, 7=>7, 8 => 8, 9 => 9, 10 => 10, 11 => 11, 12 => 12, 13 => 13, 14 => 14, 15 => 15, 16 => 16, 17 => 17, 18 => 18, 19 => 19, 20 => 20);
  $themes = _drupalchat_load_themes(drupal_get_path('module', 'drupalchat') . '/themes', 'css');
  $polling_method = variable_get('drupalchat_polling_method', DRUPALCHAT_LONGPOLL);
  
  if ($polling_method == DRUPALCHAT_LONGPOLL && ini_get('max_execution_time') < 30) {
    drupal_set_message(t('For DrupalChat Long Polling to be effective, please set max_execution_time to above 30 in your server php.ini file.'), 'warning');
  }
  
  $form = array();
  /*$form['libraries'] = array(
    '#type' => 'fieldset',
    '#title' => t('Library info'),
    '#collapsible' => TRUE,
  );
  $form['libraries']['yui'] = array(
    '#type' => 'fieldset',
    '#title' => t('YUI'),
    '#collapsible' => TRUE,
  );
  $form['libraries']['yui']['drupalchat_yui_use_cdn'] = array(
    '#type' => 'select',
    '#title' => t('Load YUI library from Yahoo CDN'),
    '#collapsible' => TRUE,
	'#options' => array(1 => 'Yes', 2 => 'No, load from my server'),
    '#default_value' => variable_get('drupalchat_yui_use_cdn', 1),
  );
  $form['libraries']['yui']['drupalchat_yui_path'] = array(
    '#type' => 'textfield',
    '#title' => t('YUI library path on your server'),
    '#description' => t("You need to download the latest stable release of the !library and extract the entire folder into sites/all/libraries. The path will be automatically discovered; you may override the directory path here if you wish to use another version; do not add slashes before or after the path.", array('!library' => l(t('YUI library'), 'http://developer.yahoo.com/yui/3/', array('attributes' => array('target' => '_blank'))))),
    '#default_value' => variable_get('drupalchat_yui_path',NULL),
  );
  */
  $form['drupalchat_theme'] = array(
    '#type' => 'select',
    '#title' => t('Theme'),
    '#description' => t('All themes from inside the <em>themes</em> folder will be displayed here.'),
    '#options' => $themes,
    '#default_value' => variable_get('drupalchat_theme', 'light'),
  );
  $form['drupalchat_notification_sound'] = array(
    '#type' => 'select',
    '#title' => t('Notification Sound'),
    '#description' => t('Select whether to play notification sound when a new message is received.'),
    '#options' => array(1 => 'Yes', 2 => 'No'),
    '#default_value' => variable_get('drupalchat_notification_sound', 1),
  );
  $form['drupalchat_user_picture'] = array(
    '#type' => 'select',
    '#title' => t('User Pictures'),
    '#description' => t('Select whether to show user pictures in chat.'),
    '#options' => array(1 => 'Yes', 2 => 'No'),
    '#default_value' => variable_get('drupalchat_user_picture', 1),
  );
  $form['drupalchat_enable_smiley'] = array(
    '#type' => 'select',
    '#title' => t('Enable Smileys'),
    '#description' => t('Select whether to show smileys.'),
    '#options' => array(1 => 'Yes', 2 => 'No'),
    '#default_value' => variable_get('drupalchat_enable_smiley', 1),
  );
  $form['drupalchat_polling_method'] = array(
		'#type' => 'radios',
		'#title' => t('Choose Polling Method'),
		'#default_value' => variable_get('drupalchat_polling_method', DRUPALCHAT_LONGPOLL),
		'#options' => array(DRUPALCHAT_COMMERCIAL => t('iFlyChat server'), DRUPALCHAT_AJAX => t('Normal AJAX'), DRUPALCHAT_LONGPOLL => t('Long Polling'), DRUPALCHAT_NODEJS => t('Node.js (currently under development)'),),
		'#description' => t('Decide the server backend for Drupal Chat.'),
  );
  $form['drupalchat_external_api_key'] = array(
	'#type' => 'textfield',
    '#title' => t('iFlyChat API Key'),
    '#description' => t("Please enter the API key by registering at !elink.", array('!elink' => l(t('iFlyChat.com'), 'https://iflychat.com', array('attributes' => array('target' => '_blank'))))),
    '#default_value' => variable_get('drupalchat_external_api_key', NULL),
  );
  $form['drupalchat_log_messages'] = array(
    '#type' => 'select',
    '#title' => t('Log chat messages'),
    '#description' => t('Select whether to log chat messages, which can be later viewed in !inbox_link.', array('!inbox_link' => l(t('message inbox'),'drupalchat/messages/inbox'))),
    '#options' => array(1 => 'Yes', 2 => 'No'),
    '#default_value' => variable_get('drupalchat_log_messages', 1),
  );
  $form['drupalchat_anon_prefix'] = array(
	'#type' => 'textfield',
    '#title' => t('Prefix to be used with anonymous users'),
    '#description' => t("Please specify the prefix to be used with anonymous users. It shouldn't be long. Ideally it should be between 4 to 7 characters."),
    '#default_value' => variable_get('drupalchat_anon_prefix', 'Guest'),
  );
  $form['drupalchat_anon_use_name'] = array(
    '#type' => 'select',
	'#title' => t('Use random name or number for anonymous user'),
	'#description' => t('Select whether to use random generated name or number to assign to a new anonymous user'),
	'#options' => array(1 => 'Name', 2 => 'Number'),
	'#default_value' => variable_get('drupalchat_anon_use_name', 1),
  );
  $form['drupalchat_anon_name_set'] = array(
    '#type' => 'select',
	'#title' => t('Select country to use for generating names of anonymous users'),
	'#options' => array('usa' => 'United States'),
	'#default_value' => variable_get('drupalchat_anon_name_set', 'usa'),
	'#wysiwyg' => false,
  );
  
  $result = drupal_http_request(DRUPALCHAT_EXTERNAL_A_HOST . ':' . DRUPALCHAT_EXTERNAL_A_PORT .  '/anam/c', array('Content-Type' => 'application/json'));
  if($result->code == 200) {
    $result = json_decode($result->data);
	foreach($result as $k=>$v) {
	  if($k != 'usa') {
	    $form['drupalchat_anon_name_set']['#options'][$k] = $v;
	  }
	}
  }
  
  $form['drupalchat_load_chat_async'] = array(
    '#type' => 'select',
    '#title' => t('Load chat asynchronously'),
    '#description' => t('Select whether to load chat asynchronously or not.'),
    '#options' => array(1 => 'Yes (Recommended)', 2 => 'No'),
    '#default_value' => variable_get('drupalchat_load_chat_async', 1),
  );
  
  $form['drupalchat_minimize_chat_user_list'] = array(
    '#type' => 'select',
    '#title' => t('Minimize online user list by default'),
    '#description' => t('Select whether to minimize online user list in chat by default.'),
    '#options' => array(1 => 'Yes', 2 => 'No'),
    '#default_value' => variable_get('drupalchat_minimize_chat_user_list', 2),
  );
  
  $form['drupalchat_pc'] = array(
    '#type' => 'fieldset',
    '#title' => t('Chat Moderation'),
    '#collapsible' => TRUE,
  );
  $form['drupalchat_pc']['drupalchat_enable_chatroom'] = array(
    '#type' => 'radios',
	'#title' => t('Enable Public Chatroom'),
	'#default_value' => variable_get('drupalchat_enable_chatroom', 1),
	'#options' => array(1 => 'Yes', 2 => 'No'),
  );
  $form['drupalchat_pc']['drupalchat_stop_word_list'] = array(
    '#type' => 'textarea',
	'#title' => t('Stop Words (separated by comma)'),
	'#default_value' => variable_get('drupalchat_stop_word_list', 'asshole,assholes,bastard,beastial,beastiality,beastility,bestial,bestiality,bitch,bitcher,bitchers,bitches,bitchin,bitching,blowjob,blowjobs,bullshit,clit,cock,cocks,cocksuck,cocksucked,cocksucker,cocksucking,cocksucks,cum,cummer,cumming,cums,cumshot,cunillingus,cunnilingus,cunt,cuntlick,cuntlicker,cuntlicking,cunts,cyberfuc,cyberfuck,cyberfucked,cyberfucker,cyberfuckers,cyberfucking,damn,dildo,dildos,dick,dink,dinks,ejaculate,ejaculated,ejaculates,ejaculating,ejaculatings,ejaculation,fag,fagging,faggot,faggs,fagot,fagots,fags,fart,farted,farting,fartings,farts,farty,felatio,fellatio,fingerfuck,fingerfucked,fingerfucker,fingerfuckers,fingerfucking,fingerfucks,fistfuck,fistfucked,fistfucker,fistfuckers,fistfucking,fistfuckings,fistfucks,fuck,fucked,fucker,fuckers,fuckin,fucking,fuckings,fuckme,fucks,fuk,fuks,gangbang,gangbanged,gangbangs,gaysex,goddamn,hardcoresex,horniest,horny,hotsex,jism,jiz,jizm,kock,kondum,kondums,kum,kumer,kummer,kumming,kums,kunilingus,lust,lusting,mothafuck,mothafucka,mothafuckas,mothafuckaz,mothafucked,mothafucker,mothafuckers,mothafuckin,mothafucking,mothafuckings,mothafucks,motherfuck,motherfucked,motherfucker,motherfuckers,motherfuckin,motherfucking,motherfuckings,motherfucks,niger,nigger,niggers,orgasim,orgasims,orgasm,orgasms,phonesex,phuk,phuked,phuking,phukked,phukking,phuks,phuq,pis,piss,pisser,pissed,pisser,pissers,pises,pisses,pisin,pissin,pising,pissing,pisof,pissoff,porn,porno,pornography,pornos,prick,pricks,pussies,pusies,pussy,pusy,pussys,pusys,slut,sluts,smut,spunk'),
	'#wysiwyg' => false,
  );
  $form['drupalchat_pc']['drupalchat_use_stop_word_list'] = array(
    '#type' => 'select',
    '#title' => t('Use Stop Words to filter chat'),
    '#description' => t('Select whether to use stop words(entered above) for filtering'),
    '#options' => array('1' => t('Don\'t filter'), '2' => t('Filter in public chatroom'), '3' => t('Filter in private chats'), '4' => t('Filter in all rooms')),
    '#default_value' => variable_get('drupalchat_use_stop_word_list', '1'),
  );
  $form['drupalchat_pc']['drupalchat_stop_links'] = array(
    '#type' => 'select',
    '#title' => t('Block hyperlinks'),
    '#description' => t('Select whether to block hyperlinks posted in chats'),
    '#options' => array('1' => t('Don\'t block'), '2' => t('Block in public chatroom'), '3' => t('Block in private chats'), '4' => t('Block in all rooms')),
    '#default_value' => variable_get('drupalchat_stop_links', '1'),
  );
  $form['drupalchat_pc']['drupalchat_allow_anon_links'] = array(
    '#type' => 'select',
    '#title' => t('Apply above defined block hyperlinks settings only to anonymous users'),
    '#description' => t('Select whether to apply above defined block hyperlinks setting only to anonymous users.'),
    '#options' => array('1' => t('Yes, apply only to anonymous users'), '2' => t('No, apply to all users'),),
    '#default_value' => variable_get('drupalchat_allow_anon_links', '1'),
  );
  $form['drupalchat_pc']['drupalchat_allow_single_message_delete'] = array(
    '#type' => 'select',
    '#title' => t('Allow users to delete messages selectively when in private conversation'),
    '#description' => t('Select whether to apply above defined block hyperlinks setting only to anonymous users.'),
    '#options' => array('1' => t('Allow all users'), '2' => t('Allow only moderators'), '3' => t('Disable'),),
    '#default_value' => variable_get('drupalchat_allow_single_message_delete', '1'),
  );
  $form['drupalchat_pc']['drupalchat_allow_clear_room_history'] = array(
    '#type' => 'select',
    '#title' => t('Allow users to clear all messages in a room'),
    '#description' => t('Select whether to allow users to clear all messages in a room'),
    '#options' => array('1' => t('Allow all users'), '2' => t('Allow only moderators'), '3' => t('Disable'),),
    '#default_value' => variable_get('drupalchat_allow_clear_room_history', '1'),
  );
  $form['drupalchat_path'] = array(
    '#type' => 'fieldset',
    '#title' => t('DrupalChat Visibility'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $access = user_access('use PHP for settings');
  $options = array(
    0 => t('All pages except those listed'),
    1 => t('Only the listed pages'),
  );
  $description = t("Specify pages by using their paths. Enter one path per line. The '*' character is a wildcard. Example paths are %blog for the blog page and %blog-wildcard for every personal blog. %front is the front page.", array('%blog' => 'blog', '%blog-wildcard' => 'blog/*', '%front' => '<front>'));

  if (module_exists('php') && $access) {
    $options += array(2 => t('Pages on which this PHP code returns <code>TRUE</code> (experts only)'));
    $title = t('Pages or PHP code');
    $description .= ' ' . t('If the PHP option is chosen, enter PHP code between %php. Note that executing incorrect PHP code can break your Drupal site.', array('%php' => '<?php ?>'));
  }
  else {
    $title = t('Pages');
  }
  $form['drupalchat_path']['drupalchat_path_visibility'] = array(
    '#type' => 'radios',
    '#title' => t('Show DrupalChat on specific pages'),
    '#options' => $options,
    '#default_value' => variable_get('drupalchat_path_visibility', 0),
  );
  $form['drupalchat_path']['drupalchat_path_pages'] = array(
    '#type' => 'textarea',
    '#title' => '<span>' . $title . '</span>',
    '#default_value' => variable_get('drupalchat_path_pages', NULL),
    '#description' => $description,
  );
  
  $form['drupalchat_css'] = array(
    '#type' => 'fieldset',
    '#title' => t('Theme Customization'),
    '#collapsible' => TRUE,
  );
  $form['drupalchat_css']['drupalchat_chat_topbar_color'] = array(
    '#type' => 'textfield',
    '#title' => t('Chat Top Bar Color'),
    '#description' => t("Choose the color of the top bar in the chat."),
    '#default_value' => variable_get('drupalchat_chat_topbar_color', '#222222'),
	'#suffix' => '<div id="drupalchat-colorpicker1"></div>', 
  );
  $form['drupalchat_css']['drupalchat_chat_topbar_text_color'] = array(
    '#type' => 'textfield',
    '#title' => t('Chat Top Bar Text Color'),
    '#description' => t("Choose the color of the text in top bar in the chat."),
    '#default_value' => variable_get('drupalchat_chat_topbar_text_color', '#FFFFFF'),
	'#suffix' => '<div id="drupalchat-colorpicker2"></div>', 
  );
  $form['drupalchat_css']['drupalchat_font_color'] = array(
    '#type' => 'textfield',
    '#title' => t('Chat Font Color'),
    '#description' => t("Choose the color of the text in the chat."),
    '#default_value' => variable_get('drupalchat_font_color', '#222222'),
	'#suffix' => '<div id="drupalchat-colorpicker3"></div>', 
  );
  $form['drupalchat_css']['drupalchat_public_chatroom_header'] = array(
    '#type' => 'textfield',
    '#title' => t('Public Chatroom Header'),
    '#description' => t("This is the text that will appear in header of public chatroom."),
    '#default_value' => variable_get('drupalchat_public_chatroom_header', 'Public Chatroom'),
  );
  $form['drupalchat_css']['drupalchat_chat_list_header'] = array(
    '#type' => 'textfield',
    '#title' => t('Chat List Header'),
    '#description' => t("This is the text that will appear in header of chat list."),
    '#default_value' => variable_get('drupalchat_chat_list_header', 'Chat'),
  );
  $form['drupalchat_user_latency'] = array(
    '#type' => 'select',
    '#title' => t('Chat List Latency'),
    '#description' => t('The delay, in seconds, after which the user will be shown offline in the chat list(i.e. removed from the chat list) from the time he/she goes offline. Increase this value if you find the chat list is unstable and keeps on changing a lot (for example - when a user navigates from one page to another he/she goes offline and then comes back online again). Decrease it if you find that the users are shown in the chat list for too long after they have left your website.'),
  	'#options' => $seconds,
    '#default_value' => variable_get('drupalchat_user_latency', 2),
  );
  $form['drupalchat_refresh_rate'] = array(
    '#type' => 'select',
    '#title' => t('Normal AJAX Refesh Rate'),
    '#description' => t('The time interval, in seconds, after which DrupalChat checks for new messages.'),
  	'#options' => $seconds,
    '#default_value' => variable_get('drupalchat_refresh_rate', 2),
		'#disabled' => variable_get('drupalchat_polling_method', DRUPALCHAT_LONGPOLL) == DRUPALCHAT_LONGPOLL ? TRUE : FALSE,  
  );
  $form['drupalchat_send_rate'] = array(
    '#type' => 'select',
    '#title' => t('Normal AJAX Send message delay'),
    '#description' => t('The delay, in seconds, to send a message to the server.'),
  	'#options' => $seconds,
    '#default_value' => variable_get('drupalchat_send_rate', 2),
  	'#disabled' => variable_get('drupalchat_polling_method', DRUPALCHAT_LONGPOLL) == DRUPALCHAT_LONGPOLL ? TRUE : FALSE,  
  );
  $form['drupalchat_chatlist_cont'] = array(
    '#type' => 'fieldset',
    '#title' => t('DrupalChat User Online List Control'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['drupalchat_chatlist_cont']['drupalchat_show_admin_list'] = array(
    '#type' => 'select',
    '#title' => t('Show only admins in chat list'),
    '#description' => t('Select whether to show only users with administer_drupalchat permission in the online user list (to other regular users).'),
    '#options' => array(1 => 'Yes', 2 => 'No'),
    '#default_value' => variable_get('drupalchat_show_admin_list', 2),
  );
  $form['drupalchat_chatlist_cont']['drupalchat_rel'] = array(
    '#type' => 'radios',
    '#title' => t('Relationship method'),
    '#default_value' => variable_get('drupalchat_rel', DRUPALCHAT_REL_AUTH),
    '#options' => array(
      DRUPALCHAT_REL_AUTH => t('All users'),
    ),
    '#description' => t('This determines the method for creating the chat buddylist.'),
  );
  if (module_exists('user_relationships_api')) {
    $form['drupalchat_chatlist_cont']['drupalchat_rel']['#options'][DRUPALCHAT_REL_UR] = t('User Relationship module');
  }
  if (module_exists('flag_friend')) {
    $form['drupalchat_chatlist_cont']['drupalchat_rel']['#options'][DRUPALCHAT_REL_FF] = t('Flag Friend module');
  }
  $form['drupalchat_chatlist_cont']['drupalchat_ur_name'] = array(
    '#type' => 'textfield',
    '#title' => t('User Relationships Role Names to integrate with'),
    '#description' => t('The singular form of User Relationships Role Names (e.g. buddy, friend, coworker, spouse) separated by comma.'),
    '#default_value' => variable_get('drupalchat_ur_name', NULL),
	  '#autocomplete_path' => 'drupalchat/ur-autocomplete',
    //'#disabled' => variable_get('drupalchat_rel', DRUPALCHAT_REL_AUTH) == DRUPALCHAT_REL_UR ? FALSE : TRUE,
  );
  
  return system_settings_form($form);
}

function drupalchat_settings_form_validate($form, &$form_state) { 
  if ($form_state['values']['drupalchat_rel'] == DRUPALCHAT_REL_UR) {
    if (!$form_state['values']['drupalchat_ur_name']) {
      form_set_error('drupalchat_ur_name', t('You must provide the user relationship name in the Drupal UR Settings section.'));  
    }
    if ($form_state['values']['drupalchat_ur_name']) {
	  $array = drupal_explode_tags($form_state['values']['drupalchat_ur_name']);
	  $error = array();
	  foreach($array as $key) {
	    $result = db_query("SELECT COUNT(*) as count FROM {user_relationship_types} WHERE name = '%s'", array($key));
		while ($row = db_fetch_object($result)) {
		  if(!$row->count)
            $error[] = $key;
		}		
      }
	  if(!empty($error))
	    form_set_error('drupalchat_ur_name', t('User Relationship type %type was not found.', array('%type' => drupal_implode_tags($error))));	  
    }
  }
  if(($form_state['values']['drupalchat_polling_method'] == DRUPALCHAT_COMMERCIAL) && (!$form_state['values']['drupalchat_external_api_key'])) {
    form_set_error('drupalchat_external_api_key', t('Please enter API key.'));
  }
  
  if(($form_state['values']['drupalchat_polling_method'] == DRUPALCHAT_COMMERCIAL) && ($form_state['values']['drupalchat_external_api_key'])) {
    $data = array(
      'api_key' => $form_state['values']['drupalchat_external_api_key'],
	    'enable_chatroom' => $form_state['values']['drupalchat_enable_chatroom'],
	    'theme' => $form_state['values']['drupalchat_theme'],
	    'notify_sound' => $form_state['values']['drupalchat_notification_sound'],
	    'smileys' => $form_state['values']['drupalchat_enable_smiley'],
	    'log_chat' => $form_state['values']['drupalchat_log_messages'],
	    'chat_topbar_color' => $form_state['values']['drupalchat_chat_topbar_color'],
	    'chat_topbar_text_color' => $form_state['values']['drupalchat_chat_topbar_text_color'],
	    'font_color' => $form_state['values']['drupalchat_font_color'],
	    'chat_list_header' => $form_state['values']['drupalchat_chat_list_header'],
	    'public_chatroom_header' => $form_state['values']['drupalchat_public_chatroom_header'],
	    'rel' => $form_state['values']['drupalchat_rel'],
	    'version' => '6.x-2.0-beta20',
	    'show_admin_list' => $form_state['values']['drupalchat_show_admin_list'],
      'clear' => $form_state['values']['drupalchat_allow_single_message_delete'],
      'delmessage' => $form_state['values']['drupalchat_allow_clear_room_history'],
    );
    if($form_state['values']['drupalchat_rel'] > DRUPALCHAT_REL_AUTH) {
      $new_valid_uids = _drupalchat_get_buddylist($user->uid, $form_state['values']['drupalchat_ur_name']);
      if(!isset($_SESSION['drupalchat_valid_uids']) || ($_SESSION['drupalchat_valid_uids'] != $new_valid_uids)) {
        $data['valid_uids'] = $new_valid_uids;
	    $_SESSION['drupalchat_valid_uids'] = $new_valid_uids;
      }
	  else {
	    $data['valid_uids'] = $new_valid_uids;
	  }
    }
	$data = json_encode($data);
    $options = array(
      'method' => 'POST',
      'data' => $data,
      'timeout' => 15,
      'headers' => array('Content-Type' => 'application/json'),
    );
	$result = drupal_http_request(DRUPALCHAT_EXTERNAL_A_HOST . ':' . DRUPALCHAT_EXTERNAL_A_PORT .  '/z/', array('Content-Type' => 'application/json'), 'POST', $data, 15);
	if($result->code == 200) {
	  $result = json_decode($result->data);
	}
	else {
	  form_set_error('drupalchat_external_api_key', "Unable to connect to iFlyChat server. Error code - " . $result->code . ". Error message - " . $result->error . ".");
	}
	
  }
}

function _drupalchat_load_themes($outerDir, $x) { 
    $dirs = array_diff(scandir($outerDir), array('.', '..')); 
    
    $dir_array = array(); 
    foreach ($dirs as $d) { 
        if (is_dir($outerDir . "/" . $d)) {
          if ($innerDir = _drupalchat_load_themes($outerDir . '/' . $d, $x)) {
            $dir_array[$d] = $innerDir;  
          }
        }
        elseif (($x)?ereg($x.'$',$d) : 1) {
          $name = _drupalchat_remove_extension($d); 
          $dir_array[$name] = $name; 
        }
    }
    return $dir_array;
}

function _drupalchat_remove_extension($strName) { 
  $ext = strrchr($strName, '.'); 

  if($ext !== false) { 
    $strName = substr($strName, 0, -strlen($ext)); 
  } 
  return $strName; 
}
function _drupalchat_autodiscover_path($match, $filename) {
  $path = '';
  $files = drupal_system_listing($match, 'libraries', 'basename', 0);
  if (isset($files[$filename])) {
    $path = dirname($files[$filename]->filename);
  }
  return $path;
}
