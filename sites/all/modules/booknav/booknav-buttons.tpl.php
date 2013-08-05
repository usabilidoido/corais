<?php
// $Id: booknav-buttons.tpl.php,v 1.0 2010/05/24 14:29:09 fuzzy76 Exp $

/**
 * @file booknav-buttons.tpl.php
 * Default theme implementation for block with a simle two-button navigation
 * for books
 *
 * Available variables:
 * - $prev_url: URL to the previous node.
 * - $prev_title: Title of the previous node.
 * - $next_url: URL to the next node.
 * - $next_title: Title of the next node.
 * - $has_links: Flags TRUE whenever the previous or next data has a value.
 *
 * @see template_preprocess_booknav_buttons()
 */
?>
<?php if ($has_links): ?>
  <div class="booknav-buttons">
    <?php if ($prev_url) : ?>
      <a href="<?php print $prev_url; ?>" class="page-previous" title="<?php print t('Go to previous page'); ?>"><?php print t('‹'); ?></a>
    <?php endif; ?>
    <?php if ($next_url) : ?>
      <a href="<?php print $next_url; ?>" class="page-next" title="<?php print t('Go to next page'); ?>"><?php print t('›'); ?></a>
    <?php endif; ?>
  </div>
<?php endif; ?>
