<?php

/**
 * @file
 * The template for an individaul sticky note
 *
 * Available variables
 *  $node                 The full node object with some sticky notes specific
 *                        attributes.
 *    position_x          The x position.
 *    position_y          The y position.
 *    position_z          The z position.
 *  $note_can_be_modified Flags a note to be editable.
 *  $note_can_be_deleted  Flags a note to be deletable.
 *  $priority_image       The image that represents the priority.
 *  $content              The output save content of the note.
 *  $author               The output save author of the note.
 *  $sticky_note_width    The default width of a sticky note.
 *  $sticky_note_height   The default height of a sticky note.
 *
 * This template can be overridden by placing a file with the same name into
 * the theme directory.
 *
 * @author berliner
 */

?>

<div id="sticky-note-<?php print $node->nid; ?>" class="sticky-notes-note-item-wrapper <?php print $edit_link ? 'editable' : ''; ?>" style="width: <?php print $sticky_note_width; ?>px; height: <?php print $sticky_note_height; ?>px; position: absolute; top: <?php print $node->position_y; ?>px; left: <?php print $node->position_x; ?>px; z-index: <?php print $node->position_z; ?>;">
  
  <?php if ($priority_image): ?>
    <div class="sticky-notes-note-item-priority"><?php print $priority_image; ?></div>
  <?php endif; ?>
  
  <?php if ($edit_link || $delete_link): ?>
    <div class="sticky-notes-note-item-actions-wrapper">
      <div class="sticky-notes-note-item-actions">
        <?php if ($edit_link): ?>
          <?php print $edit_link; ?>
        <?php endif; ?>
        <?php if ($delete_link): ?>
          <?php print $delete_link; ?>
        <?php endif; ?>
      </div>
    </div>
  <?php endif; ?>
  
  <div class="sticky-notes-note-item" style="width: <?php print $sticky_note_width - 4; ?>px; height: <?php print $sticky_note_height - 4; ?>px; background-color: <?php print $sticky_note_note_color; ?>">
    <span class="sticky-note-nid"><?php print $node->nid; ?></span>
    <div class="sticky-notes-note-item-body" style="color: <?php print $sticky_note_text_color; ?>;">
      <?php print $content; ?>
    </div>
    <div class="sticky-notes-note-item-author" style="background-color: <?php print $sticky_note_note_color; ?>; color: <?php print $sticky_note_text_color; ?>;">
      <?php if (theme_get_setting('toggle_node_info_sticky_notes')) : ?>
        <?php print $author; ?>, <?php print format_date($node->created, 'small'); ?>
      <?php endif; ?>
    </div>
  </div>

</div>