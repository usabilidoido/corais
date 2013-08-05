<?php

/**
 * @file
 * The template for the info box
 *
 * Available variables
 *  $classes        A string of class names to add attributes to the box, e.g.
 *                  hidden for initial hiding
 *  $add_link       A full <a> attribute to create a new note. Only set, if
 *                  the user has sufficient permission.
 *  $overview_link  A full <a> attribute that links to an overview page. Only
 *                  set if the views module is installed and enabled.
 *
 * This template can be overridden by placing a file with the same name into
 * the theme directory.
 *
 * @author berliner
 */

?>

<div id="sticky-notes-info-box-wrapper">
  <div id="sticky-notes-info-box" class="<?php print $classes; ?>">
    <div class="content">

      <?php if ($add_link): ?>
        <div id="sticky-notes-add-note-button">
          <?php print $add_link; ?>
        </div>
      <?php endif; ?>

      <div id="sticky-notes-page-count"></div>

      <div id="sticky-notes-options">
        <a id="sticky-notes-options-display-expose" href="#"><?php print t('Exposé'); ?></a>
        <?php if ($toggle_visibility_state): ?>
          |
          <a id="sticky-notes-options-display-hidden" href="#"><?php print t('Hide notes'); ?></a>
          <a id="sticky-notes-options-display-normal" href="#"><?php print t('Show notes'); ?></a>
        <?php endif; ?>
      </div>

      <?php if ($overview_link): ?>
        <div class="sticky-notes-options-overview-link">
          <?php print $overview_link; ?>
        </div>
      <?php endif; ?>

    </div>
  </div>
</div>
