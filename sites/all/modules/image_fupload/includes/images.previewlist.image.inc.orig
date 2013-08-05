<?php
// $Id: images.previewlist.image.inc,v 1.3 2009/03/14 10:26:19 grandcat Exp $

/*
* Provide a list (queue) of uploaded images; all captions can be edited at the same time
*/
function fupload_list_images_image($form_state, $node_type) {
  global $user;
  $count = 0;  
  
  $node_type = str_replace("-", "_", $node_type);
  $image_node_types = variable_get('image_node_types', array());
  $type = node_get_types('type', $node_type);
  $field_name = $image_node_types[$node_type]['fieldname'];
  $field_settings = variable_get('fupload_previewlist_field_settings', array('title' => 'Title', 'body' => 'Body'));
  
  $result = db_query("SELECT nid, title FROM {node} WHERE type = '%s' AND uid = %d AND status = '0' AND created > %d", $node_type, $user->uid, time() - 86400); // only entries which are one day or newer
  while ($node = db_fetch_object($result)) {
    $count += 1;
    // Get all node infos, also image paths
    $node_image = node_load($node->nid);
    
    // only use this image node if it was produced by image_fupload
    if ($node_image->body == image_fupload_image_status($field_name, IMAGE_NOT_COMPLETED)) {
      $image_nodes[] = $node_image->nid;
    
      $form[$node_image->nid] = array(
        '#type' => 'fieldset',
        '#title' => t('Image @count', array('@count' => $count)),
        '#collapsible' => FALSE,
        '#collapsed' => FALSE,
      );
      $form[$node_image->nid]['preview_image'] = array(
        '#prefix' => '<div class="image_fupload_preview">',
        '#value' => _fupload_imagepreview($node_image, $node_type),
        '#suffix' => '</div>',
        '#weight' => 1,        
      );      
      $form[$node_image->nid]['title_' .$node_image->nid] = array(
        '#type' => 'textfield',
        '#title' => check_plain($type->title_label),
        '#default_value' => isset($form_state['values']['title_' .$node_image->nid]) ? $form_state['values']['title_' .$node_image->nid] : $node_image->title,
        '#size' => 60,
        '#required' => $field_settings['title'], // disabled fields don't send any data via form_state [#227966] =/
        '#disabled' => !$field_settings['title'],
        '#weight' => 3,
      );
      if ($type->has_body) {
        $form[$node_image->nid]['body_' .$node_image->nid] = array(
          '#type' => 'textarea',
          '#title' => check_plain($type->body_label),
          '#default_value' => isset($form_state['values']['body_' .$node_image->nid]) ? $form_state['values']['body_' .$node_image->nid] : '',
          '#rows' => 5,           
          //'#required' => ($type->min_word_count > 0),
          // will be checked later via validation hook
          '#disabled' => !$field_settings['body'],
          '#weight' => 5,
        );
        $form[$node_image->nid]['format_' .$node_image->nid] = filter_form($node_image->format, NULL, array('format_' .$node_image->nid));
        $form[$node_image->nid]['format_' .$node_image->nid]['#weight'] = 6;
      }
    }
  }
  
  if (!empty($image_nodes)) {
    // are there any images to edit?
    $form['text'] = array('#value' => t('In this step, you can edit the captions of all uploaded images. To complete this task, click the button "Done Editing" at the bottom of this page.'), '#weight' => -19);
    $form['#redirect'] = 'node/add/' .arg(2); // redirect to first upload page again
    $form['field_name'] = array('#type' => 'value', '#value' => $field_name);
    $form['image_nodes'] = array('#type' => 'value', '#value' => implode(';', $image_nodes));
    $form['#validate'][] = 'fupload_list_images_image_validate'; // some additonal validation of body field
    $form['submit'] = array('#type' => 'submit', '#value' => t('Done Editing'));
  } else {
    // no images in queue
    $form['text'] = array('#value' => t('No images yet in queue.'), '#weight' => -19);
    drupal_set_message(t('No images have been found in queue, probably no images have been uploaded yet. Please return to !upload_page if you want to upload some images.', array('!upload_page' => l(t('image upload page'), 'node/add/' .arg(2)))), 'warning');
  }
  
  // variable_set('image_node_types', array('imagefield_bild' => array('title' => 'ImageField', 'fieldname' => 'field_imagefile', 'image_selection' => '', 'imagecache_preset' => 'fupload_preview', 'image_dimension' => '127x200')));  
  return $form;
}

function fupload_list_images_image_validate($form, $form_state) {
  // only validate body field, rest is already validated; code partially taken from function "node_validate" (D6)
  // get nids of images and start batch process (validation)
  $type = node_get_types('type', str_replace("-", "_", check_plain(arg(2))));
  $image_nids = explode(';', $form_state['values']['image_nodes']);
  
  for ($i = 0; $i < count($image_nids); $i++) {  
    if (!empty($type->min_word_count) && isset($form_state['values']['body_' .$image_nids[$i]]) && count(explode(' ', $form_state['values']['body_' .$image_nids[$i]])) < $type->min_word_count) {
      form_set_error('body_' .$image_nids[$i], t('The body of the @img_number. image is too short. You need at least %words words.', array('%words' => $type->min_word_count, '@img_number' => $i + 1)));
    }  
  } 
}

function fupload_list_images_image_submit($form, $form_state) {  
  // get nids of images and start batch process (saving)
  $image_nids = explode(';', $form_state['values']['image_nodes']); 
  // name of image field
  $field_name = $form_state['values']['field_name'];
  for ($i = 0; $i < count($image_nids); $i++) { 
    // load full node object  
    $node = node_load($image_nids[$i]);    
    // new changes to node object
    $node->status = 1; // publish node
    $node->title = !empty($form_state['values']['title_' .$image_nids[$i]]) ? $form_state['values']['title_' .$image_nids[$i]] : $node->title; // work around [#227966]
    $node->body = $form_state['values']['body_' .$image_nids[$i]];
    if ($node->body == image_fupload_image_status($field_name, IMAGE_NOT_COMPLETED) || !isset($form_state['values']['body_' .$image_nids[$i]])) // nothing changed ==> empty body field
      $node->body = "";
    $node->teaser = node_teaser($node->body, $form_state['values']['format_' .$image_nids[$i]]);    
    $node->format = $form_state['values']['format_' .$image_nids[$i]];    
    // save new node object
    node_save($node);
  }
  drupal_set_message(t('All images have been saved and published.'));
  drupal_set_message(t('Additonal images can be selected and uploaded now.'));
  drupal_redirect_form($form);  
}

function _fupload_imagepreview($node_image, $node_type) {
  $image_node_types = variable_get('image_node_types', array());
  $attributes = variable_get('fupload_previewlist_img_attributes', '');
  /** need to split image module and other cck related imagemodules to be able to provide the right
     *     preview handling 
     */
  switch ($node_type) {    
	case 'image':
      // image module
      if (!empty($image_node_types['image']['imagecache_preset'])) {
        // using ImageCache
        $content = theme('imagecache', $image_node_types['image']['imagecache_preset'], $node_image->images['_original'], $node_image->title, $node_image->title, $attributes);
      } else {
        // using a ready-to-use Image size of Image module
        $image = $node_image->images[$image_node_types['image']['image_selection']];
        $content = theme('fupload_imagepreview_image', $image, image_get_info($image), $node_image, $attributes);
      }
      break;
    
    default:
      // imagefield cck module
      if (!empty($image_node_types[$node_type]['imagecache_preset'])) {
        // using ImageCache
        $field_name = $image_node_types[$node_type]['fieldname'];
        $image = $node_image->$field_name;
        
        $content = theme('imagecache', $image_node_types[$node_type]['imagecache_preset'], $image[0]['filepath'], $node_image->title, $node_image->title, $attributes);
      }
      break;
  }
  
  return $content;
}

/**
* Custom theme for preview image (not imagecache image)
**/

function theme_fupload_imagepreview_image($image, $image_info, $node_image, $attributes) {
  $content = '<img src="' .file_create_url(file_create_path($image)). '" width="' .$image_info['width']. '" height="' .$image_info['height']. '" title="' .$node_image->title. '" alt="' .$node_image->title. '" class="image_fupload_preview_image"' .(!empty($attributes) ? (" " .$attributes) : "").' />';
  return $content;
}