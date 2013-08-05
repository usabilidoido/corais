<div class="commit">
  <div class="header">
    <div class="hash">
      <label>Commit:</label><?php echo $hash; ?>
    </div>
    <?php if ($parents) { ?>
    <div class="parent">
      <label>Parent:</label><?php echo $parents; ?>
    </div>
    <?php } ?>
    <?php if ($tree) { ?>
    <div class="tree">
      <label>Tree:</label><?php echo $tree; ?>
    </div>
    <?php } ?>
    <?php if ($author) { ?>
    <div class="author person">
      <label>Author:</label><?php echo $author; ?>
    </div>
    <?php } ?>
    <?php if ($committers) { ?>
    <div class="committer person">
      <label>Committer:</label><?php echo $committers; ?>
    </div>
    <?php } ?>
    <?php if ($refs) { ?>
    <div class="refs">
      <label>References:</label><?php echo $refs; ?>
    </div>
    <?php } ?>
  </div>
  <div class="description"><?php echo $description; ?></div>
  <?php if ($changes) { ?>
  <div class="changes"><?php echo $changes; ?></div>
  <?php } ?>
</div>