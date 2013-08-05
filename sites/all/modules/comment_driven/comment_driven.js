// $Id: comment_driven.js,v 1.8 2010/03/27 23:02:25 arhak Exp $

Drupal.comment_driven = Drupal.comment_driven || {};

Drupal.comment_driven.node_wrap = function() {
  // the comment_form is within id=comment_driven
  // everything else belongs to node_form
  if ($('#comment_driven').size() == 0) return;
  
  // #weight was already provided to make
  // id=comment_driven ligther and id=driven_node_container heavier
  // but we wan't to reorder them both within the form
  // therefore, we'll bring out the the two major pieces (comment_form and empty fieldset)
  // to be able to select everything else to go into id=driven_node_container
  //
  // note that the second one added will be preppended on top
  if (Drupal.settings.comment_driven.node_form_bellow) {
    $('#driven_node_container').prependTo('#comment-form');
    $('#comment_driven').prependTo('#comment-form');
  }
  else {
    $('#comment_driven').prependTo('#comment-form');
    $('#driven_node_container').prependTo('#comment-form');
  }
  
  // support Open Atrium [#740310]
  // Open Atrium excludes collapse.js and also uses tao/templates/object.tpl.php
  // which wraps the fielset content into a .fieldset-content instead of a .fieldset-wrapper
  var theme_support = Drupal.settings.comment_driven.theme_support;
  var fieldset_wrapper_selector = '#driven_node_container .fieldset-wrapper';
  switch (theme_support) {
    case 'disabled':
      // take no action
      break;
    case 'atrium': // Open Atrium themes: Tao and its sub-themes: Rubik, Ginkgo
      fieldset_wrapper_selector = '#driven_node_container .fieldset-content';
      // object.tpl.php doesn't print anything when empty($content)
      $('#driven_node_container').append('<div class="fieldset-content clear-block"></div>');
      // no break;
    default:
      // bring every node_form element into the fieldset
      // don't rely on class=node-form being set by a theme function
      $('#comment-form > div:not(#comment_driven, #driven_node_container)').prependTo(fieldset_wrapper_selector);
      // make the fieldset visible
      $('#driven_node_container').css('display', 'block');
      break;
  }
};

// just once (not a behavior)
$(document).ready(Drupal.comment_driven.node_wrap);
