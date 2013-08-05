<div class="commit">
  <div class="header">
    <div class="hash">
      <label>Object:</label><?php echo $object; ?>
    </div>
    <?php if ($tag) { ?>
    <div class="tag">
      <label>Tag:</label><?php echo $tag; ?>
    </div>
    <?php } ?>
    <?php if ($tagger) { ?>
    <div class="tagger person">
      <label>Tagger:</label><?php echo $tagger; ?>
    </div>
    <?php } ?>
  </div>
  <div class="description"><?php echo $description; ?></div>
</div>