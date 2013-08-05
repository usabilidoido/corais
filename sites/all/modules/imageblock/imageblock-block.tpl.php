<?php

/**
 * @file imageblock-block.tpl.php
 *
 * Theme implementation to display a block.
 *
 * Available variables:
 * - $block: Block object.
 * - $image: Block image content.
 * - $content: Block content.
 *
 * @see template_preprocess()
 * @see template_preprocess_imageblock_block()
 */
?>
<?php if ($image): ?>
  <div class="block-image">
    <?php print $image ?>
  </div>
<?php endif; ?>

<?php if ($content): ?>
  <div class="block-body">
    <?php print $content ?>
  </div>
<?php endif; ?>
