<?php

/**
 * @file certificate.pages.inc
 * Functions for generating certificates.
 */

/**
 * Get certificate for a specific node.
 */
function certificate_node_certificate($node) {
  global $user;

  if ($user->uid) {
    $account = $user;
  }

  $perm = certificate_can_access_certificate($node, $user);

  if ($perm != 1) {
    //$perm is a string! show error.
    return $perm;
  }

  $admin = user_access('administer certificates');
  $view_all = user_access('view all user certificates');

  // Use account of a different user if allowed.
  if (($admin || $view_all) && arg(3) > 0) {
    $account = user_load(arg(3));
  }

  return certificate_single($account, $node);
}

/**
 * Generate a single certificate.
 *
 * Check to see if user already has a certificate for a course. If so, serve it from the DB. If not, generate one and save it to the DB.
 */
function certificate_single($account, $node) {
  global $user;
  $output = '';

  // Preview.
  $preview = isset($_GET['preview']) && user_access('administer certificates');

  // See if user already has a snapshotted certificate for a course. If so, grab
  // from the DB. If not, generate a new one.
  if (variable_get('certificate_snapshots', 0) && $snapshot = certificate_snapshot_load($account, $node)) {
    $output = $snapshot['snapshot'];
  }
  else {
    $hooks = module_invoke_all('certificate_map_options');
    $sql = "select * from {certificate_node} where nid = %d or nid = %d order by nid asc";
    $result = db_query($sql, 0, $node->nid);
    $mapping = array();
    while ($row = db_fetch_object($result)) {
      $mapping[$row->mapper][$row->type] = $row->template;
    }

    foreach (array_keys($hooks) as $map_type) {
      if (!empty($mapping[$map_type])) {
        $matches = module_invoke_all('certificate_map', $node, $user, $map_type, array_keys(array_filter((array) $mapping[$map_type])), array());
        if (count($matches)) {
          $template_id = $mapping[$map_type][$matches[0]]; // First one for now.
        }
      }
    }

    drupal_alter('certificate_template_id', $template_id, $node, $account);

    $template = node_load($template_id);

    if ($template && $node) {
      // Prepend output with UTF 8 meta tag.
      // See http://code.google.com/p/wkhtmltopdf/issues/detail?id=556
      // Print module gets around this because the meta tag in Drupal is already
      // set - but here, we have to add it since we are only delivering the node
      // body.
      $output = '<html><head><meta charset="utf-8"/></head><body>' . theme('certificate_certificate', $node, $account, $template) . '</body></html>';

      // Now save this generated certificate as a 'snapshot' in the database.
      $snapshot['uid'] = $account->uid;
      $snapshot['nid'] = $node->nid;
      $date = getdate();
      $snapshot['date'] = $date[0];
      $snapshot['snapshot'] = $output;
      if (!$preview) {
        certificate_snapshot_save($snapshot);
      }
    }
    else {
      return t('Sorry, there is no certificate available.');
    }
  }

  $print_pdf_pdf_tool = variable_get('print_pdf_pdf_tool', '');
  if ($print_pdf_pdf_tool == '') {
    if ($user->uid == 1) {
      drupal_set_message('Certificate cannot be displayed because you have not selected a PDF generation tool in !link.', array('!link' => l('Printer, e-mail and PDF versions', 'admin/settings/print/pdf') . '.', 'error'));
    }
    else {
      drupal_set_message('PDF generation tool is not configured.');
    }
    return ' ';
  }

  module_load_include('pages.inc', 'print_pdf', 'print_pdf');

  if ($preview) {
    print $output;
  }
  else {
    // Rewrite image URLs using Print.
    $pattern = '!<(img\s[^>]*?)>!is';
    $output = preg_replace_callback($pattern, '_print_rewrite_urls', $output);
    global $conf;
    $conf['print_pdf_page_orientation'] = $template->certificate['orientation'];
    print_pdf_generate_html(array('node' => $node), $output, $node->title . '.pdf');
  }
}

/**
 * Preview certificate template as PDF.
 */
function certificate_preview($template_id) {
  $template = node_load($template_id);
  if ($template) {
    $output = '<html><head><meta charset="utf-8"/></head><body>' . theme('certificate_certificate', $node, $account, $template) . '</body></html>';
  }

  $print_pdf_pdf_tool = variable_get('print_pdf_pdf_tool', '');
  if ($print_pdf_pdf_tool == '') {
    drupal_set_message('Certificate cannot be displayed because you have not selected a PDF generation tool in !link.', array('!link' => l('Printer, e-mail and PDF versions', 'admin/settings/print/pdf') . '.', 'error'));
    return '';
  }

  module_load_include('pages.inc', 'print_pdf', 'print_pdf');

  // Rewrite image URLs using Print.
  $pattern = '!<(img\s[^>]*?)>!is';
  $output = preg_replace_callback($pattern, '_print_rewrite_urls', $output);
  global $conf;
  $conf['print_pdf_page_orientation'] = $template->certificate['orientation'];
  print_pdf_generate_html(array('node' => $node), $output, $template->title . '.pdf');
}

/**
 * Theme a single certificate.
 *
 * Does token replace (new style [], and old style %)
 *
 * @param $account
 *   The user account being viewed.
 *
 * @ingroup themeable
 *
 * @return The certificate HTML with all tokens translated.
 */
function theme_certificate_certificate($node, $account, $template) {
  $types = array(
    'global' => NULL,
    'node' => $node,
    'user' => $account,
    'certificate' => array(
      'node' => $node,
      'user' => $account,
    ),
  );

  // Invoke hook_certificate_body_alter() to allow all modules to alter the template body.
  drupal_alter('certificate_body', $template->body, $account, $node);

  if (module_exists('token')) {
    if (module_exists('purl')) {
      purl_disable(TRUE);
    }
    $template->body = token_replace_multiple($template->body, $types);
  }

  $template->body = check_markup($template->body, $template->format, FALSE);

  return $template->body;
}
