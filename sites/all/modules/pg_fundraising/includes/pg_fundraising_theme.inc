<?php
// $Id: pg_fundraising_theme.inc,v 1.1.2.3 2011/02/18 02:15:35 dokumori Exp $
/**
 * @file
 * All theme-related functions for this module are found here
 * 
 */

function theme_pg_fundraising_complete($t) {
  $header = array(
    array('data' => $t->title, 'colspan' => '2'),
  );

  $rows = array();

  $row = array();
  $row[] = t('Price');
  $row[] = t('!currency !amount', array(
    '!amount' => $t->amount,
    '!currency' => variable_get('pg_simplepay_symbol', '$')
    )
  );
  $rows[] = $row;

  $row = array();
  $row[] = t('Status');
  $row[] = pgapi_get_status($t->status);
  $rows[] = $row;

  $row = array();
  $row[] = t('By method');
  $row[] = module_invoke($t->method, 'pgapi_gw', 'display name');
  $rows[] = $row;

  $row = array();
  $row[] = t('Node URL');
  $node = node_load($t->extra['nid']);
  $row[] = l($node->title, 'node/'. $node->nid);
  $rows[] = $row;
  
  $output = theme('table', $header, $rows);
  return $output;
}

/**
 * Theme 
 */
function theme_pg_fundraising_fail($t) {
  $header = array(
    array( 'data' => $t->title, 'colspan' => '2'),
  );

  $rows = array();

  $row = array();
  $row[] = t('Price');
  $row[] = t('!currency !amount', array(
    '!amount' => $t->amount,
    '!currency' => variable_get('pg_simplepay_symbol', '$')
    )
  );
  $rows[] = $row;

  $row = array();
  $row[] = t('Status');
  $row[] = pgapi_get_status($t->status);
  $rows[] = $row;

  $row = array();
  $row[] = t('By method');
  $row[] = module_invoke($t->method, 'pgapi_gw', 'display name');
  $rows[] = $row;

  $row = array();
  $row[] = t('Reason');
  $row[] = pgapi_get_status($t->description);
  $rows[] = $row;

  $output = theme('table', $header, $rows);
  return $output;
}

function theme_pg_fundraising_donation_menu_box($images, $amount_arr, $descriptions) {
  $output = '';
  $i = 1;
  $count = count($images);
  foreach ($images as $key => $image) {
    if ($i == $count) {
      $position = ' last';
    }
    elseif ($i == 1) {
      $position = ' first';
    }
    else {
      $position = "";
    }
    
    $amount = "<div class=\"donation-menu-amount donation-menu-amount-$i\">". $amount_arr[$key] .'</div>';
    $description = "<div class=\"donation-menu-description donation-menu-description-$i\">". $descriptions[$key] .'</div>';
    
    $output .= sprintf("<div class=\"menu-box menu-box-$i fundraising-donation-menu-box$position\">%s%s%s</div>", $image, $amount, $description);
    $i++;
  }
  return $output;
}

/**
 * Preprocess function for donation menu
 */
function template_preprocess_pg_fundraising_donation_menu(&$variables) {
  
  $tmp_node = $variables['node'];
  $imagecache_preset = variable_get('pg_fundraising_imagecache', '');

  $count = count($tmp_node->field_pg_fundraising_image);

  $donation_menu_components = '';

  for ($i = 0; $i < $count ; $i ++) { 

    //images
    $image = theme('imagecache', $imagecache_preset, $tmp_node->field_pg_fundraising_image[$i]['filepath'], $tmp_node->field_pg_fundraising_image[$i]['data']['title'], $tmp_node->field_pg_fundraising_image[$i]['data']['alt']);
    $variables['images'][$i] = $image;
    
    //amount
    $variables['amount'][$i] = $tmp_node->field_pg_fundraising_amount[$i]['safe'];
    // $amount = "<div class=\"donation-menu-amount donation-menu-amount-$i\">". $tmp_node->field_pg_fundraising_amount[$i]['safe'] .'</div>';
       
    // description
    $variables['descriptions'][$i] = $tmp_node->field_pg_fundraising_description[$i]['safe'];
    // $description = "<div class=\"donation-menu-description donation-menu-description-$i\">". $tmp_node->field_pg_fundraising_description[$i]['safe'] .'</div>';
  
    // position
    $variables['position'][$i] = $position;
   
   //construct a box
     //$donation_menu_components .= sprintf("<div class=\"menu-box-$i fundraising-donation-menu-box-$position\">%s%s%s</div>", $image, $amount, $description);
    
  }  
  
  //$variables['donation_menu'] = $donation_menu_components;

  
  //TODO: individual field values
  
}

