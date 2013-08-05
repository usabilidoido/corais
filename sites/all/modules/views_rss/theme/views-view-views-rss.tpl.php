<?php
/**
 * @file views-view-views-rss.tpl.php
 * Default template for feed displays that use the RSS Fields style.
 */
?>
<?php print "<?xml"; ?> version="1.0" encoding="utf-8" <?php print "?>"; ?>
<rss version="2.0" <?php print $namespaces; ?>>
  <channel>
    <?php print $channel; ?>
    <?php print $rows; ?>
  </channel>
</rss>
