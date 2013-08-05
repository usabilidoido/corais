<?php
// $Id: images.previewlist.imagefield.inc,v 1.6 2009/03/15 21:54:57 grandcat Exp $

/*
* Provide a list (queue) of uploaded images for CCK ImageField; all captions can be edited at the same time
*/
function fupload_list_images_imagefield($form_state, $node_type) {
  global $user;
  $count = 0;
  
  $node_type = str_replace("-", "_", $node_type);
  $image_node_types = variable_get('image_node_types', array());
  // get some infos about all fields of this content type and, of course, of our imagefield (e.x. estrictions)
  $field_name = $image_node_types[$node_type]['fieldname'];
  $fields = content_types($node_type);
  $field_image = $fields['fields'][$field_name];
  
  // form a more useful list of fields which should be displayed in preview list
  $fields_preview_list = array('node' => array(), 'imagefield' => array(), 'taxonomy' => array(), 'cck' => array());
  foreach ($field_image['widget']['fupload_previewlist_field_settings'] as $key) {
    $elements = explode("_", $key, 2);
    if ($field_image['widget']['fupload_previewlist_field_settings'][$key] != FALSE)
      array_push($fields_preview_list[$elements[0]], $elements[1]);
  }
  
  // checks which fields should be displayed (not for all fields)
  
  // prepare token
  $token_installed = module_exists("token");
  
  // prepare taxonomy
  $form_taxonomy = array();
  if (module_exists("taxonomy") && count($fields_preview_list['taxonomy'])) {
    $form_taxonomy = array('type' => array('#value' => $node_type), '#node' => (object) array('type' => $node_type));
    taxonomy_form_alter($form_taxonomy, array(), $node_type .'_node_form');
    
    // delete all elements without the ones we need
    foreach ($form_taxonomy['taxonomy'] as $key => $value) {
      // taxonomy normal
      if (!in_array($key, $fields_preview_list['taxonomy']) && is_numeric($key)) {
        unset($form_taxonomy['taxonomy'][$key]);
      }
    }
    foreach ($form_taxonomy['taxonomy']['tags'] as $key => $value) {
      // taxonomy tags
      if (!in_array($key, $fields_preview_list['taxonomy'])) {        
        unset($form_taxonomy['taxonomy']['tags'][$key]);
      }
    }
  }
  
  // get our preview images
  $result = db_query("SELECT p.nid, p.fid, f.filename, f.filepath FROM {fupload_previewlist} AS p JOIN {files} AS f ON p.fid = f.fid WHERE p.fieldname = '%s' AND p.uid = %d", $field_name, $user->uid);
  while ($image = db_fetch_object($result)) {
    $count += 1;
    $image_items[$image->fid] = $image->nid;
    
    // get title and body fields contents if available and storage mode is single
    if ($field_image['widget']['fupload_mode'] == "single") {
      $node = db_query("SELECT title, body FROM {node_revisions} WHERE nid = %d", $image->nid);
      $node = db_fetch_object($node);
    }
            
    $form[$image->fid] = array(
        '#type' => 'fieldset',
        '#title' => t('Image @count', array('@count' => $count)),
        '#collapsible' => FALSE,
        '#collapsed' => FALSE,
    );
    $form[$image->fid]['preview_image'] = array(
        '#prefix' => '<div class="image_fupload_preview">',
        '#value' => theme('imagecache', $image_node_types[$node_type]['imagecache_preset'], $image->filepath, check_plain($image->filename), check_plain($image->filename), _fupload_get_attributes_array($field_image['widget']['fupload_previewlist_img_attributes'])),
        '#suffix' => '</div>',
        '#weight' => -16,
    );

    // node title field
    if (in_array("title", $fields_preview_list['node'])) {
      $form[$image->fid]['node_title_' .$image->fid] = array(
        '#type' => 'textfield',
        '#title' => check_plain($fields['title_label']),
        '#default_value' => isset($form_state['values']['node_title_' .$image->fid]) ? $form_state['values']['node_title_' .$image->fid] : $node->title,
        '#size' => 60,
        '#required' => TRUE,
        '#disabled' => FALSE,
        '#weight' => $fields['extra']['title']['weight'],
      );
    }
    
    // filefield / imageField fields
    if (in_array("title", $fields_preview_list['imagefield']) && $field_image['widget']['custom_title']) {
      if ($token_installed && !empty($field_image['widget']['title'])) {
        $default_value = token_replace($field_image['widget']['title']);
      } else {
        if (!empty($field_image['widget']['title'])) {
          $default_value = $field_image['widget']['title'];
        } else {
          $default_value = $image->filename;
        }        
      }        
      
      $form[$image->fid]['imagefield_title_' .$image->fid] = array(
        '#type' => 'textfield',
        '#title' => $field_image['widget']['label'] .' ' .t('title'),
        '#default_value' => isset($form_state['values']['imagefield_title_' .$image->fid]) ? $form_state['values']['imagefield_title_' .$image->fid] : $default_value,
        '#size' => 60,
        '#required' => FALSE,
        '#disabled' => FALSE,
        '#weight' => $field_image['widget']['weight'],
      );
    }    
    if (in_array("alt", $fields_preview_list['imagefield']) && $field_image['widget']['custom_alt']) {
      if ($token_installed && !empty($field_image['widget']['alt'])) {
        $default_value = token_replace($field_image['widget']['alt']);
      } else {
        if (!empty($field_image['widget']['alt'])) {
          $default_value = $field_image['widget']['alt'];
        } else {
          $default_value = $image->filename;
        }        
      }        
      
      $form[$image->fid]['imagefield_alt_' .$image->fid] = array(
        '#type' => 'textfield',
        '#title' => $field_image['widget']['label'] .' ' .t('alt'),
        '#default_value' => isset($form_state['values']['imagefield_alt_' .$image->fid]) ? $form_state['values']['imagefield_alt_' .$image->fid] : $default_value,
        '#size' => 60,
        '#required' => FALSE,
        '#disabled' => FALSE,
        '#weight' => $field_image['widget']['weight'] + 0.3,
      );
    }    
    if (in_array("description", $fields_preview_list['imagefield'])) {    
      $form[$image->fid]['imagefield_description_' .$image->fid] = array(
        '#type' => 'textfield',
        '#title' => $field_image['widget']['label'] .' ' .t('description'),
        '#default_value' => $form_state['values']['imagefield_description_' .$image->fid],
        '#size' => 60,
        '#required' => FALSE,
        '#disabled' => FALSE,
        '#weight' => $field_image['widget']['weight'] + 0.5,
      );
    }

    // node body field
    if (in_array("description", $fields_preview_list['node'])) {    
      $form[$image->fid]['node_body_' .$image->fid] = array(
        '#type' => 'textarea',
        '#title' => check_plain($fields['extra']['body_field']['label']),
        '#default_value' => isset($form_state['values']['node_body_' .$image->fid]) ? $form_state['values']['node_body_' .$image->fid] : '',
        '#rows' => 5,           
        //'#required' => ($type->min_word_count > 0),
        // will be checked later via validation hook
        '#disabled' => FALSE,
        '#weight' => $fields['extra']['body_field']['weight'],
      );
      $form[$image->fid]['format_' .$image->fid] = filter_form(FILTER_FORMAT_DEFAULT, $fields['extra']['body_field']['weight'], array('format_' .$image->fid));
      $form[$image->fid]['format_' .$image->fid]['#weight'] = $fields['extra']['body_field']['weight'] + 0.1;
    }
    
    // taxonomy field
    if (count($form_taxonomy)) {
      $form_taxonomy['taxonomy']['#weight'] = $fields['extra']['taxonomy']['weight'];
      $form[$image->fid]['taxonomy_' .$image->fid] = $form_taxonomy['taxonomy'];
    }

    // ckk fields here ... in a future version
    
  }
  
  // any images uploaded
  if ($count) {
    // some images in queue
    $form['text'] = array('#value' => t('In this step, you can edit the captions of all uploaded images. To complete this task, click the button "Done Editing" at the bottom of this page.'), '#weight' => -19);    
    $form['node_type'] = array('#type' => 'value', '#value' => $node_type);
    $form['field_name'] = array('#type' => 'value', '#value' => $field_name);
    $form['storage_mode'] = array('#type' => 'value', '#value' => $field_image['widget']['fupload_mode']);
    $form['image_items'] = array('#type' => 'value', '#value' => $image_items);
    $form['#validate'][] = 'fupload_list_images_imagefield_validate'; // some additonal validation of body field and cck
    $form['submit'] = array('#type' => 'submit', '#value' => t('Done Editing'));
    
    // set redirect url 
    if ($field_image['widget']['fupload_previewlist_redirecturl']) {
      // alternative redirect url provided
      $form['#redirect'] = $field_image['widget']['fupload_previewlist_redirecturl'];
    } else {
      if ($field_image['widget']['fupload_mode'] == "multiple") {
        $form['#redirect'] = 'node/' .array_pop($image_items); // redirect to updated node
      } else {
        $form['#redirect'] = 'node/add/' .str_replace("_", "-", $node_type); // redirect to first upload page again
      }      
    }
    
  } else {
    // no images in queue
    $form['text'] = array('#value' => t('No images yet in queue.'), '#weight' => -19);
    drupal_set_message(t('No images have been found in queue, probably no images have been uploaded yet. Please return to !upload_page if you want to upload some images.', array('!upload_page' => l(t('image upload page'), 'node/add/' .str_replace("_", "-", $node_type)))), 'warning');
  }
  
  return $form;
}

function fupload_list_images_imagefield_validate($form, $form_state) {
  // validation of some special parts  
  $i = 0;  
  $storage_mode = $form_state['values']['storage_mode'];
  $type = node_get_types('type', $form_state['values']['node_type']);
  
  foreach ($form_state['values']['image_items'] as $fid => $nid) {
    $i++;
    // load saved node(s) 
    if ($storage_mode == "multiple" && empty($node))
      $node = node_load($nid);
    if ($storage_mode == "single")
      $node = node_load($nid);   
    
    // map some data to our $node to be able to do validation
    // taxonomy
    $node->taxonomy = $form_state['values']['taxonomy_' .$fid];

    // body field: if min_word_count set, check reality    
    if (!empty($type->min_word_count) && isset($form_state['values']['node_body_' .$fid]) && count(explode(' ', $form_state['values']['node_body_' .$fid])) < $type->min_word_count) {
      form_set_error('node_body_' .$fid, t('The field %body_label of the @img_number. image is too short. You need at least %words words.', array('%body_label' => $type->body_label, '@img_number' => $i, '%words' => $type->min_word_count)));
    }
    
    // little hack: prevent that body field will be checked again 
    unset($node->body);
    // call hook_validation
    node_validate($node);
  }
  
}

function fupload_list_images_imagefield_submit($form, $form_state) { 
  global $user;
  // define some basic variables
  $imagefield_fields = array("title", "description", "alt");
  $storage_mode = $form_state['values']['storage_mode'];
  
  // get fids (--> nids) of images and start batch process (saving) 
  foreach ($form_state['values']['image_items'] as $fid => $nid) {    
    // split up storage modes to provide a better handling
    // storage mode: multiple images per node
    if ($storage_mode == "multiple") {
      // only load node once
      if (!isset($node))
        $node = node_load($nid);      
      
      $image_field = $node->$form_state['values']['field_name'];
      for ($x = 0; $x < count($image_field); $x++) {
        if ($image_field[$x]['fid'] == $fid) {
          // found our image to be edited          
          foreach ($imagefield_fields as $key => $value) {
            if (isset($form_state['values']['imagefield_' .$value .'_' .$fid]))
              $image_field[$x]['data'][$value] = $form_state['values']['imagefield_' .$value .'_' .$fid];
          }
                    
          break;
        }
      }
      // map our data to $node
      $node->$form_state['values']['field_name'] = $image_field;      
    }
    
    // storage mode: single image per node
    if ($storage_mode == "single") {
      $node = node_load($nid);
      
      // new changes to node object
      // basic fields (core modules of drupal)
      if (isset($form_state['values']['node_title_' .$fid]))
        $node->title = !empty($form_state['values']['node_title_' .$fid]) ? $form_state['values']['node_title_' .$fid] : $node->title; // work around [#227966]
      if (isset($form_state['values']['node_body_' .$fid]))
        $node->body = !empty($form_state['values']['node_body_' .$fid]) ? $form_state['values']['node_body_' .$fid] : $node->body;
      $node->teaser = node_teaser($node->body, $form_state['values']['format_' .$fid]);
      if (isset($form_state['values']['format_' .$fid]))
        $node->format = isset($form_state['values']['format_' .$fid]) ? $form_state['values']['format_' .$fid] : $node->format;      
      if (isset($form_state['values']['taxonomy_' .$fid]))
        $node->taxonomy = isset($form_state['values']['taxonomy_' .$fid]) ? array_merge($form_state['values']['taxonomy_' .$fid], $node->taxonomy) : $node->taxonomy;
      
      // imagefield (cck)
      $image_field = $node->$form_state['values']['field_name'];        
      foreach ($imagefield_fields as $key => $value) {
        if (isset($form_state['values']['imagefield_' .$value .'_' .$fid]))
          $image_field[0]['data'][$value] = $form_state['values']['imagefield_' .$value .'_' .$fid];
      }
      $node->$form_state['values']['field_name'] = $image_field;
      
      // cck fields (contributed module)
      // ... will follow in future version
      
      // save changes to node
      node_save($node);
    }
  }
  
  // multiple mode: map our new data into node
  if ($storage_mode == "multiple")
    node_save($node);
  
  // delete preview image list for this user
  if (!form_get_errors())
    db_query("DELETE FROM {fupload_previewlist} WHERE fieldname = '%s' AND uid = %d", $form_state['values']['field_name'], $user->uid);
  
  // show message and redirect user to upload page
  drupal_set_message(t('All images have been saved and published.'));
  drupal_redirect_form($form); 
}