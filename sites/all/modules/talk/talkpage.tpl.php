<?php // $Id$ ?>
<?php
/**
 * @file
 * Overridable template file for talk page.
 *
 * Available variables:
 *   $node represents the node whose comments we're displaying.
 *   $comments represents the rendered comments.
 *   $comment_link represents an "add new comment" link.
 *   $add_comments is TRUE if the user has permission to add comments.
 *   $redisplay is TRUE if the "add new comment" link should be redisplayed at the bottom of the page.
 *   $title represents the title of the talk page. Defaults to "Talk".
 */
?>
<?php if ($comment_link): ?>
  <p>
    <?php print $comment_link; ?>
  </p>
  <br />
<?php endif; ?>
<?php print $comments; ?>
<?php if ($comment_link && $redisplay): ?>
  <p>
  <?php print $comment_link; ?>
  </p>
<?php endif; ?>