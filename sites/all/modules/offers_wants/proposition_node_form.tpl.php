<?php
  $form = &$variables['form'];
 /*
 * preprocess functions are: template_preprocess
 *
 * variables are:
 * $template_files
 * $form
 * $zebra
 * $id
 * $directory
 * $is_admin
 * $is_front
 * $logged_in
 * $db_is_active
 * $user
 * and the $form contains many things, particularly
 *   $form['want'] //may be hidden
 *   $form['title']
 *   $form['menu']
 *   $form['body'] //may not be used
 * $categories
 * $types
 *
 */
  unset($form['menu']);
  unset($form['comment_settings']);

  $buttons = drupal_render($form['buttons']);
  //render the things first that go at the bottom
  if (isset($form['options'])) {
    $admin .= '    <div class="options">';
    $admin .= drupal_render($form['options']);
    $admin .= "    </div>\n";
  }
  if (isset($form['author'])) {
    $admin .= "    <div class=\"authored\">\n";
    $admin .= drupal_render($form['author']);
    $admin .= "    </div>\n";
  }
  $categories = drupal_render($vocabs[variable_get('offers_wants_cat_vid', 0)]);
  $types = drupal_render($vocabs[variable_get('offers_wants_type_vid', 0)]);
?>
<div class="node-form">

  <div style="width:49%;float:left">
  <?php print drupal_render($form['want']); ?>
    <?php print drupal_render($form['title']); ?>
    <?php print drupal_render($form['body']); ?>
    <?php print drupal_render($form['expires']); ?>
  </div>
  <div style="width:49%;float:right">
    <?php print $categories; ?>
    <?php print $types; ?>
  </div>

<br style = "clear:both;" />
  <?php //if there's anything else left
    print drupal_render($form);
  ?> 
  <?php print $admin; ?>
  <?php print $buttons; ?>
</div>
