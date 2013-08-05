INSTALLATION
------------------------------------------------------------------------------
Enable module from the modules administration page.

CONFIGURATION & SETUP
------------------------------------------------------------------------------
Edit your theme's page.tpl.php file and add this where you want the into to be
displayed:

  <?php echo $user_intro; ?>

Edit the same page.tpl.php to add this where you want the "show user intro"
link to show up:

  <?php echo $user_intro_show; ?>

The module comes with two template files (found in user_intro/templates) that
generate the output for the above two variables:

  user_intro.tpl.php - The actual intro box.
  user_intro_show.tpl.php - The link to show the intro box.

If you want to modify these files, you should copy them to your theme's folder
and make changes there. Remember to clear your theme registry (or the site's
cache) to make Drupal use these (now overridden) files.