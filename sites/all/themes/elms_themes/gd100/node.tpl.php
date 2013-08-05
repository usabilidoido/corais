<div class="node<?php if ($sticky) { print " sticky"; } ?><?php if (!$status) { print " node-unpublished"; } ?>">
 <span class="taxonomy"><?php print $terms?></span>
 <span class="submitted"><?php print $submitted?></span>
 <div class="content"><?php print $content?></div>
  <?php print $links ?>
</div>