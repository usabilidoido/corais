<?php // $Id: views-view-views-rss-fields.tpl.php,v 1.1.2.1 2010/03/02 21:00:40 alexb Exp $ ?>
<?php print "<?xml"; ?> version="1.0" encoding="utf-8" <?php print "?>"; ?>

<rss version="2.0" <?php print $namespaces ?>>
  <channel>
    <title><?php print $viewtitle; ?></title>
    <description><?php print $description; ?></description>
    <link><?php print $link ?></link>
    <?php print $rows ?>
  </channel>
</rss>
