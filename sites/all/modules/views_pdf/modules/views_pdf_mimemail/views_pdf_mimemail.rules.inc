<?php


/**
 * @file
 * Rules integration of the mimemail and the PDF Views module. 
 *
 */

/**
 * Implementation of hook_rules_action_info_alter()
 */
function views_pdf_mimemail_rules_action_info_alter(&$actions) {
  $actions['mimemail_rules_action_mail_to_user']['eval input'][] = 'views_pdf_attachments';
  $actions['mimemail_rules_action_mail']['eval input'][] = 'views_pdf_attachments';
  $actions['mimemail_rules_action_mail_to_users_of_role']['eval input'][] = 'views_pdf_attachments';
  
  $actions['views_pdf_mimemail_rules_action_mail_to_user'] = $actions['mimemail_rules_action_mail_to_user'];
  unset($actions['mimemail_rules_action_mail_to_user']);
  
  $actions['views_pdf_mimemail_rules_action_mail'] = $actions['mimemail_rules_action_mail'];
  unset($actions['mimemail_rules_action_mail']);
  
  $actions['views_pdf_mimemail_rules_action_mail_to_users_of_role'] = $actions['mimemail_rules_action_mail_to_users_of_role'];
  unset($actions['mimemail_rules_action_mail_to_users_of_role']);
  
}

/**
 * Sends a PDF to a user.
 */
function views_pdf_mimemail_rules_action_mail_to_user($user, $settings) {
  // Process settings.
  $settings = views_pdf_mimemail_rules_form_settings_process($settings);
  $from = $settings['from'];
  $subject = $settings['subject'];
  $body = $settings['message_html'];
  $plaintext = $settings['message_plaintext'];
  $attachments = $settings['attachments'];
  $mailkey = 'mimemail_rules_action_mail';

  // If recipient field is empty send it to given user object.
  $to = empty($settings['to']) ? $user->mail : implode(',', $settings['to']);

  $headers = array();
  if (!empty($settings['bcc'])) {
    $headers['Bcc'] = implode(',', $settings['bcc']);
  }
  if (!empty($settings['cc'])) {
    $headers['Cc'] = implode(',', $settings['cc']);
  }

  $status = mimemail($from, $to, $subject, $body, NULL , $headers, $plaintext, $attachments, $mailkey);

  if (!empty($status)) {
    $recipients = array_merge(explode(',', $to), $settings['bcc'], $settings['cc']);
    watchdog('rules', 'HTML mail successfully sent to %recipient', array('%recipient' => implode(', ', $recipients)));
  }
}

/**
 * Sends a e-mail with a PDF.
 */
function views_pdf_mimemail_rules_action_mail($user, $settings) {
  views_pdf_mimemail_rules_action_mail_to_user(NULL, $settings);
}

/**
 * Sends a PDF to all user of specific role
 */
function views_pdf_mimemail_rules_action_mail_to_users_of_role($settings) {
  $recipients = array_filter(array_keys(array_filter($settings['recipients'])));

  // All authenticated users, which is everybody.
  if (in_array(DRUPAL_AUTHENTICATED_RID, $recipients)) {
    $result = db_query('SELECT mail FROM {users} WHERE uid > 0');
  }
  else {
    $rids = implode(',', $recipients);
    // Avoid sending emails to members of two or more target role groups.
    $result = db_query('SELECT DISTINCT u.mail FROM {users} u INNER JOIN {users_roles} r ON u.uid = r.uid WHERE r.rid IN ('. $rids .')');
  }

  // Process settings.
  $settings = views_pdf_mimemail_rules_form_settings_process($settings);
  $from = $settings['from'];
  $subject = $settings['subject'];
  $body = $settings['message_html'];
  $plaintext = $settings['message_plaintext'];
  $attachments = $settings['attachments'];
  $mailkey = 'mimemail_rules_action_role_mail';

  // Now, actually send the mails.
  $status = TRUE;
  while (($account = db_fetch_object($result)) && !empty($status)) {
    $status = mimemail($from, $account->mail, $subject, $body, NULL, array(), $plaintext, $attachments, $mailkey);
  }
  if (!empty($status)) {
    $roles = array_intersect_key(user_roles(TRUE), drupal_map_assoc($recipients));
    watchdog('rules', 'HTML mail successfully sent to role(s) %roles.', array('%roles' => implode(', ', $roles)));
  }
}

/**
 * Process the form settings array to produce the nessecary information. This 
 * includes also the PDF.
 *
 */
function views_pdf_mimemail_rules_form_settings_process($settings) {
  $settings = mimemail_rules_form_settings_process($settings);
  
  // Add option for direct include of PDFs from Views PDF
  if (isset($settings['views_pdf_attachments'])) {
    $views_pdf_attachments = array();
    foreach (explode("\n", $settings['views_pdf_attachments']) as $attachment) {
      $splits = explode('||||', $attachment);
      $view_name = trim($splits[0]);
      $arguments = $splits[1];
      if (!empty($view_name)) {
        $view = views_get_view($view_name);
    
        $view->set_arguments($arguments);
    
        // Try to get pdf display
        if (!$view->set_display('pdf')) {
          // Try the display type
          if (!$view->set_display('pdf_1')) {
            // There is definitly no pdf display
            continue;
          }      
        }
         
        $view->pre_execute();
        foreach ($view->display as $id => $display) {
          if ($display->display_plugin == 'pdf' && isset($display->handler)) {
            $settings['attachments'][] = array(
              'filecontent' => $display->handler->execute($tmp_path, 'S'),
              'filename' => $view_name . '_' . $id . '.pdf',
              'filemime' => 'application/pdf',
            );
          }
        }
                
      }
  
    }
  }
  
  return $settings;
  
}

/**
 * Callback function for the settings form of the rule.
 */
function views_pdf_mimemail_rules_action_mail_to_user_form($settings = array(), &$form) {
  mimemail_rules_action_mail_to_user_form($settings, $form);
  views_pdf_mimemail_rules_action_additional_form($settings, $form);
}

/**
 * Callback function for the settings form of the rule.
 */
function views_pdf_mimemail_rules_action_mail_form($settings = array(), &$form) {
  mimemail_rules_action_mail_form($settings, $form);
  views_pdf_mimemail_rules_action_additional_form($settings, $form);
}

/**
 * Callback function for the settings form of the rule.
 */
function views_pdf_mimemail_rules_action_mail_to_users_of_role_form($settings = array(), &$form) {
  mimemail_rules_action_mail_to_users_of_role_form($settings, $form);
  views_pdf_mimemail_rules_action_additional_form($settings, $form);
}

/**
 * Adds the additional used setting form fields for the sending of the PDF to the 
 * user.
 */
function views_pdf_mimemail_rules_action_additional_form($settings = array(), &$form) {   
    $views = views_get_all_views();
    
    $view_options = array( ' ' => ' ');
    
    // Filter only views with a pdf display
    foreach ($views as $key => $view) {
      foreach ($view->display as $id => $display) {
        if ($display->display_plugin == 'pdf') {
          $view_options[$key] = $view->name;
        }
      }
    }
    
    $views_pdf_attachments = array();
    foreach (explode("\n", $settings['views_pdf_attachments']) as $attachment) {
      $splits = explode('||||', $attachment);
      $views_pdf_attachments[] = array(
        'view' => $splits[0],
        'arguments' => $splits[1],
      );
    }
        
    if (!isset($views_pdf_attachments)) {
      $views_pdf_attachments = array();
    }
    
    // Remove empty rows:
    else {
      foreach ($views_pdf_attachments as $key => $value) {
        $value['view'] = trim($value['view']);
        if (empty($value['view'])) {
          unset($views_pdf_attachments[$key]);
        }
      }
    }
  
    $form['settings']['views_pdf_attachments'] = array(
      '#title' => t('Views PDF Attachments'),
      '#type' => 'fieldset',
      '#collapsed' => TRUE,
      '#collapsible' => TRUE,
      '#description' => t('Here you can add Views PDF attachments to the e-mail. In the arguement field you can use also replacement patterns.'),
    );
    $form['settings']['views_pdf_attachments']['#element_validate'][] = 'views_pdf_mimemail_rules_action_mail_to_user_form_views_pdf_validate';
    
    $form['settings']['views_pdf_attachments']['#theme'] = 'views_pdf_mimemail_rules_action_mail_form_theme';
  
    // add always 3 empty rows (so we need no ajax things here)
    $views_pdf_attachments[] = array();
    $views_pdf_attachments[] = array();
    $views_pdf_attachments[] = array();
  
    foreach ($views_pdf_attachments as $attachment) {
      $form['settings']['views_pdf_attachments'][] = array(
        'view' => array(
          '#type' => 'select',
          '#title' => t('View'),
          '#options' => $view_options,
          '#default_value' => $attachment['view'],
        ),
        'arguments' => array(
          '#type' => 'textfield',
          '#title' => t('Arguments'),
          '#default_value' => $attachment['arguments'],
        ),
      );
    }

}


/**
 * Transform the user input into a savable array.
 */
function views_pdf_mimemail_rules_action_mail_to_user_form_views_pdf_validate($element, &$form_state) {

  $views_pdf_attachments = array();
  foreach ($form_state['values']['settings']['views_pdf_attachments'] as $attachment) {
    if (!empty($attachment['view'])) {
      $views_pdf_attachments[] = $attachment['view'] . '||||' . $attachment['arguments'];
    }
  }
  
  $form_state['values']['settings']['views_pdf_attachments'] = implode("\n", $views_pdf_attachments);
  
}


