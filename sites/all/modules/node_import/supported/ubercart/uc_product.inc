<?php

/**
 * @file
 * Support for the Ubercart Product module.
 */

if (!function_exists('uc_product_is_product')) {
  function uc_product_is_product($type) {
    return uc_product_node_is_product($type);
  }
}

/**
 * Implementation of hook_node_import_fields().
 */
function uc_product_node_import_fields($type) {
  $fields = array();

  if (($node_type = node_import_type_is_node($type)) !== FALSE && uc_product_is_product($node_type)) {
    $fields['model'] = array(
      'title' => t('SKU'),
      'group' => t('Product information'),
      'weight' => 1,
    );
    $fields['list_price'] = array(
      'title' => t('List price'),
      'group' => t('Product information'),
      'weight' => 2,
    );
    $fields['cost'] = array(
      'title' => t('Cost'),
      'group' => t('Product information'),
      'weight' => 3,
    );
    $fields['sell_price'] = array(
      'title' => t('Sell price'),
      'group' => t('Product information'),
      'weight' => 4,
    );
    $fields['shippable'] = array(
      'title' => t('Shippable'),
      'group' => t('Product information'),
      'input_format' => 'boolean',
      'weight' => 5,
    );
    $fields['weight'] = array(
      'title' => t('Weight'),
      'group' => t('Product information'),
      'weight' => 6,
    );
    $fields['weight_units'] = array(
      'title' => t('Weight unit'),
      'group' => t('Product information'),
      'allowed_values' => array('lb' => t('Pounds'), 'kg' => t('Kilograms'), 'oz' => t('Ounces'), 'g' => t('Grams')),
      'default_value' => variable_get('uc_weight_unit', 'lb'),
      'weight' => 7,
    );
    $fields['length_units'] = array(
      'title' => t('Dimensions unit'),
      'group' => t('Product information'),
      'allowed_values' => array('in' => t('Inches'), 'ft' => t('Feet'), 'cm' => t('Centimeters'), 'mm' => t('Millimeters')),
      'default_value' => variable_get('uc_length_unit', 'in'),
      'weight' => 8,
    );
    $fields['length'] = array(
      'title' => t('Length'),
      'group' => t('Product information'),
      'weight' => 9,
    );
    $fields['width'] = array(
      'title' => t('Width'),
      'group' => t('Product information'),
      'weight' => 10,
    );
    $fields['height'] = array(
      'title' => t('Height'),
      'group' => t('Product information'),
      'weight' => 11,
    );
    $fields['pkg_qty'] = array(
      'title' => t('Package quantity'),
      'group' => t('Product information'),
      'weight' => 12,
    );
    $fields['default_qty'] = array(
      'title' => t('Default cart quantity'),
      'group' => t('Product information'),
      'default_value' => 1,
      'weight' => 13,
    );
    $fields['ordering'] = array(
      'title' => t('List position'),
      'group' => t('Product information'),
      'input_type' => 'weight',
      'delta' => 25,
      'default_value' => 0,
      'weight' => 14,
    );
  }

  return $fields;
}

