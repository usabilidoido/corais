<?php

/**
 * @file node.tpl.php
 *
 * Theme implementation to display a node.
 *
 * Available variables:
 * - $title: the (sanitized) title of the node.
 * - $content: Node body or teaser depending on $teaser flag.
 * - $picture: The authors picture of the node output from
 *   theme_user_picture().
 * - $date: Formatted creation date (use $created to reformat with format_date()).
 * - $links: Themed links like "Read more", "Add new comment", etc. output from theme_links().
 * - $name: Themed username of node author output from theme_username().
 * - $node_url: Direct url of the current node.
 * - $terms: the themed list of taxonomy term links output from theme_links().
 * - $submitted: themed submission information output from
 *   theme_node_submitted().
 *
 * Other variables:
 * - $node: Full node object. Contains data that may not be safe.
 * - $type: Node type, i.e. story, page, blog, etc.
 * - $comment_count: Number of comments attached to the node.
 * - $uid: User ID of the node author.
 * - $created: Time the node was published formatted in Unix timestamp.
 * - $zebra: Outputs either "even" or "odd". Useful for zebra striping in
 *   teaser listings.
 * - $id: Position of the node. Increments each time it's output.
 *
 * Node status variables:
 * - $teaser: Flag for the teaser state.
 * - $page: Flag for the full page state.
 * - $promote: Flag for front page promotion state.
 * - $sticky: Flags for sticky post setting.
 * - $status: Flag for published status.
 * - $comment: State of comment settings for the node.
 * - $readmore: Flags true if the teaser content of the node cannot hold the
 *   main body content.
 * - $is_front: Flags true when presented in the front page.
 * - $logged_in: Flags true when the current user is a logged-in member.
 * - $is_admin: Flags true when the current user is an administrator.
 *
 * variables specific to this module.
 * $expires: formatted date of expiry (optional). Based on $node->expires
 * $end: whether the node owner fixed a date for node expiry (otherwise the date was automatic)
 * $want: whether this proposition is an offer or want (boolean)
 * $categories an array of terms keyed by link to taxonomy/term/$tid
 * $types an array of terms keyed by link to taxonomy/term/$tid
 * $proposition_class is simply 'offer' or 'want
 */
?>
<!--node-proposition.tpl.php-->
<style>.links ul li{display:block}</style>
<div class = "node node-proposition <?php print $subtype; ?>">
<?php if ($teaser) { ?>
  <h3><a href="<?php print $node_url; ?>"><?php print $title; ?></a></h3>
<?php }
else{ 
  $title = $node->want ? t('Want') : t('Offer');
  $title .= ' : '. $node->title;
  drupal_set_title($title);
}
?>
<p><?php print $content; ?>
<?php if ($date) : ?>
  <br /><?php if ($expires) print t('Expires:') . ' '. $expires; ?>
<?php endif; ?></p>

<div class="links"><?php print $links; ?></div>

</div>
<!--node-proposition.tpl.php-->
