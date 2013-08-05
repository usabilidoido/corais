<?php
// $Id: pg_fundraising.admin.inc,v 1.1.2.3 2011/02/18 02:15:35 dokumori Exp $
/**
 * @file
 *   admin-related forms
 */

/**
 * Fundraising admin page
 */
function pg_fundraising_admin() {

  $imagecache_presets = imagecache_presets();
  $options = array();
  foreach ($imagecache_presets as $preset) {
    $options[$preset['presetname']] = $preset['presetname'];
  }

  $form['pg_fundraising_imagecache'] = array(
    '#type'          => 'select',
    '#title'         => t('ImageCache preset'),
    '#default_value' => variable_get('pg_fundraising_imagecache', 'donation_menu_item_220x165'),
    '#options'       => $options,
    '#description'   => t('Select which ImageCache preset to use for donation menu photos'),
  );

  //currency symbol should be set per-node
  
  $form['pg_fundraising_position'] = array(
    '#type'          => 'select',
    '#title'         => t('Symbol position'),
    '#default_value' => variable_get('pg_fundraising_position', TRUE),
    '#options'       => array(
            TRUE  => t('Left'),
            FALSE => t('Right'),
    ),
    '#description'   => t('This option places the currency symbol of the left or right side of the price.'),
  );
  // TODO: make this configurable per node 
  // $form['pg_fundraising_symbol'] = array(
  //   '#type'          => 'textfield',
  //   '#title'         => t('Symbol'),
  //   '#default_value' => variable_get('pg_fundraising_symbol','£'),
  //   '#description'   => t("Please enter symbol of the fundraising currency."),
  // );
  $form['pg_fundraising_thousands'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Thousands separator'),
    '#default_value' => variable_get('pg_fundraising_thousands', ','),
    '#size'          => 3,
    '#maxlength'     => 5,
    '#description'   => t('Enter the sign for the thousands separator.')
  );
  $form['pg_fundraising_decimal'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Decimal separator'),
    '#default_value' => variable_get('pg_fundraising_decimal', '.'),
    '#size'          => 3,
    '#maxlength'     => 5,
    '#description'   => t('Enter the sign to seperate real numbers from floating numbers.')
  );
  $form['pg_fundraising_decimal_places'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Number of places after the decimal separator'),
    '#default_value' => variable_get('pg_fundraising_decimal_places', 2),
    '#size'          => 3,
    '#maxlength'     => 5,
    '#description'   => t('How many slots are needed after the decimal?')
  );
  //TODO: make this a per-node option
  $form['pg_fundraising']['tax_feature'] = array(
    '#type'          => 'fieldset',
    '#title'         => 'Default tax-related options',
    '#collapsible'   => FALSE
  
  );
  
  foreach (module_implements('pg_fundraising_tax') as $module) {
    if ($info = module_invoke($module, 'pg_fundraising_tax', 'tax option')) {
      $radios_options[$module] = $info['name'];
    } 
  }
  
  $form['pg_fundraising']['tax_feature']['pg_fundraising_tax_feature'] = array(
    '#type'          => 'radios',
    '#default_value' => variable_get('pg_fundraising_tax_feature', ''),
    '#options' => $radios_options,
  );

  return system_settings_form($form);
}
