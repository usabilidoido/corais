<?php


/**
 * @file
 * Rules integration of the mimemail and the PDF Views module. 
 *
 */

/**
 * Implementation of hook_rules_action_info()
 */
function views_pdf_rules_action_info() {
  
  $items = array();
  
  $items['views_pdf_rules_action_save'] = array(
    'label' => t('Save PDF as file on server'),
    'module' => 'Views PDF',
    'eval input' => array('views_pdf', 'path', 'arguments'),
  
  );
  
  return $items;
}



function views_pdf_rules_action_save_form($settings = array(), &$form) {
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

  $form['settings']['views_pdf'] = array(
    '#type' => 'select',
    '#title' => t('View'),
    '#options' => $view_options,
    '#default_value' => $settings['views_pdf'],
  );

  $form['settings']['arguments'] = array(
    '#type' => 'textfield',
    '#title' => t('Arguments'),
    '#default_value' => $settings['arguments'],
    '#description' => t('Separate different arguments by commas.'),
  );


  $form['settings']['path'] = array(
    '#type' => 'textfield',
    '#title' => t('Path'),
    '#default_value' => $settings['path'],
    '#description' => t('Enter an absolute path where the view should be saved. You may use some tokens.'),
  );
}


function views_pdf_rules_action_save($settings) {

  $view_name = $settings['views_pdf'];
  $arguments = $settings['arguments'];
  $path = $settings['path'];
  if (!empty($view_name)) {
    $view = views_get_view($view_name);

    $view->set_arguments(explode(',', $arguments));

    // Try to get pdf display
    if (!$view->set_display('pdf')) {
      // Try the display type
      if (!$view->set_display('pdf_1')) {
        // There is definitly no pdf display
        return;
      }      
    }
     
    $view->pre_execute();
    foreach ($view->display as $id => $display) {
      if ($display->display_plugin == 'pdf' && isset($display->handler)) {
        $display->handler->execute($path, 'F');
      }
    }
            
  }
}
