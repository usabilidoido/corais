<?php
// $Id: xmlsitemap_engines.admin.inc,v 1.1.2.19 2010/01/27 00:33:17 davereid Exp $


/**
 * @file
 * XML sitemap engines settings UI.
 */

/**
 * @addtogroup xmlsitemap
 * @{
 */

/*****************************************************************************
 * Menu callbacks / form builders, submit/validate functions.
 ****************************************************************************/

/**
 * Form builder; return the search engine settings form.
 */
function xmlsitemap_engines_settings() {
  $form['submission'] = array(
    '#type' => 'fieldset',
    '#title' => t('Submission settings'),
    '#collapsible' => TRUE,
  );
  $form['submission']['xmlsitemap_engines_submit'] = array(
    '#type' => 'checkbox',
    '#title' => t('Submit the sitemap when content changes'),
    '#description' => t('If enabled, the search engines will only be notified of changes to the sitemap whenever the sitemap content is changed.'),
    '#default_value' => variable_get('xmlsitemap_engines_submit', FALSE),
  );
  $form['submission']['xmlsitemap_engines_cron_submit_frequency'] = array(
    '#type' => 'select',
    '#title' => t('Frequency of sitemap submission'),
    '#description' => t('The rate at which the sitemap is submitted to the search engines.'),
    '#default_value' => variable_get('xmlsitemap_engines_cron_submit_frequency', 3600),
    '#options' => array(
      '29030400' => t('Yearly'),
      '14515200' => t('Every 6 months'),
      '2419200' => t('Monthly'),
      '1296000' => t('Every 15 days'),
      '604800' => t('Weekly'),
      '86400' => t('Daily'),
      '3600' => t('Hourly'),
      '-1' => t('Never'),
    ),
  );
  // Ask.com fields.
  $form['ask'] = array(
    '#type' => 'fieldset',
    '#title' => t('Ask.com'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['ask']['xmlsitemap_engines_ask_submit'] = array(
    '#type' => 'checkbox',
    '#title' => t('Submit the sitemap to Ask.com'),
    '#default_value' => variable_get('xmlsitemap_engines_ask_submit', FALSE),
  );
  $form['ask']['xmlsitemap_engines_ask_url'] = array(
    '#type' => 'textfield',
    '#title' => t('Submission URL'),
    '#description' => t('The URL to submit the sitemap to. Use %placeholder as placeholder for the sitemap URL.', array('%placeholder' => '[sitemap]')),
    '#value' => variable_get('xmlsitemap_engines_ask_url', 'http://submissions.ask.com/ping?sitemap=[sitemap]'),
    '#disabled' => TRUE,
  );
  // Google fields.
  $form['google'] = array(
    '#type' => 'fieldset',
    '#title' => t('Google'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['google']['xmlsitemap_engines_google_submit'] = array(
    '#type' => 'checkbox',
    '#title' => t('Submit the sitemap to Google'),
    '#default_value' => variable_get('xmlsitemap_engines_google_submit', FALSE),
  );
  $form['google']['xmlsitemap_engines_google_url'] = array(
    '#type' => 'textfield',
    '#title' => t('Submission URL'),
    '#description' => t('The URL to submit the sitemap to. Use %placeholder like placeholder for the sitemap URL.', array('%placeholder' => '[sitemap]')),
    '#value' => variable_get('xmlsitemap_engines_google_url', 'http://www.google.com/webmasters/tools/ping?sitemap=[sitemap]'),
    '#disabled' => TRUE,
  );
  // Moreover.com fields.
  $form['moreover'] = array(
    '#type' => 'fieldset',
    '#title' => t('Moreover.com'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['moreover']['xmlsitemap_engines_moreover_submit'] = array(
    '#type' => 'checkbox',
    '#title' => t('Submit the sitemap to Moreover.com'),
    '#default_value' => variable_get('xmlsitemap_engines_moreover_submit', FALSE),
  );
  $form['moreover']['xmlsitemap_engines_moreover_url'] = array(
    '#type' => 'textfield',
    '#title' => t('Submission URL'),
    '#description' => t('The URL to submit the sitemap to. Use %placeholder like placeholder for the sitemap URL.', array('%placeholder' => '[sitemap]')),
    '#value' => variable_get('xmlsitemap_engines_moreover_url', 'http://api.moreover.com/ping?u=[sitemap]'),
    '#disabled' => TRUE,
  );
  // Bing fields.
  $form['bing'] = array(
    '#type' => 'fieldset',
    '#title' => t('Bing (formerly Live Search)'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['bing']['xmlsitemap_engines_bing_submit'] = array(
    '#type' => 'checkbox',
    '#title' => t('Submit the sitemap to Bing'),
    '#default_value' => variable_get('xmlsitemap_engines_bing_submit', FALSE),
  );
  $form['bing']['xmlsitemap_engines_bing_url'] = array(
    '#type' => 'textfield',
    '#title' => t('Submission URL'),
    '#description' => t('The URL to submit the sitemap to. Use %placeholder like placeholder for the sitemap URL.', array('%placeholder' => '[sitemap]')),
    '#value' => variable_get('xmlsitemap_engines_bing_url', 'http://www.bing.com/webmaster/ping.aspx?siteMap=[sitemap]'),
    '#disabled' => TRUE,
  );
  // Yahoo! fields.
  $form['yahoo'] = array(
    '#type' => 'fieldset',
    '#title' => t('Yahoo!'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['yahoo']['xmlsitemap_engines_yahoo_submit'] = array(
    '#type' => 'checkbox',
    '#title' => t('Submit the sitemap to Yahoo!'),
    '#default_value' => variable_get('xmlsitemap_engines_yahoo_submit', FALSE),
  );
  $form['yahoo']['xmlsitemap_engines_yahoo_url'] = array(
    '#type' => 'textfield',
    '#title' => t('Submission URL'),
    '#description' => t('The URL to submit the sitemap to. Use %placeholder like placeholder for the sitemap URL.', array('%placeholder' => '[sitemap]')),
    '#value' => variable_get('xmlsitemap_engines_yahoo_url', 'http://search.yahooapis.com/SiteExplorerService/V1/ping?sitemap=[sitemap]'),
    '#disabled' => TRUE,
  );
  return system_settings_form($form);
}

/**
 * @} End of "addtogroup xmlsitemap".
 */
