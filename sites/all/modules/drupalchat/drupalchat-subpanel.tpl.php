<?php 
?>

<a href="#" class="<?php print $subpanel['name']; ?> subpanel_toggle">
  <span class="subpanel_title_text">
    <?php if ($subpanel['icon']):?>
      <?php print $subpanel['icon']; ?>
    <?php endif; ?>
    <?php if ($subpanel['text']):?>
      <?php print $subpanel['text']; ?>
    <?php endif; ?> 
  </span>
</a>
<div class="subpanel">
  <?php if ($subpanel['header']):?>
    <div class="subpanel_title">
      <?php print $subpanel['header']; ?>
      <span class="min">_</span>
    </div>
  <?php endif; ?>
  <?php if ($subpanel['contents']):?>
    <?php print $subpanel['contents']; ?>
  <?php endif; ?> 
</div>