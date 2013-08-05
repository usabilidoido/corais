<?php

/**
 * @file search-results-location.tpl.php
 * (copy of) Default theme implementation for displaying search results.
 *
 * This file is only needed for Drupal 5 compatibility. In Drupal 6, the default
 * implementation works fine.
 */
?>
<dl class="search-results <?php print $type; ?>-results">
  <?php print $search_results; ?>
</dl>
<?php print $pager; ?>
