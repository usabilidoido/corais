<?php 
?>
<div class="icons-explanation">
    <ul>
        <li class="correct"><? print t('Indicates correct answer.'); ?></li>
        <li class="wrong"><? print t('Indicates wrong answer'); ?></li>
        <li class="not-evaluated"><? print t('Not evaluated'); ?></li>
    </ul>
</div>

<?php print $revisions; ?>

<!-- start accumulated -->
<div class="accumulated">
  
  <!-- start row -->
  <div class="row">
    
    <div class="column">
      <div class="text"><?php print $accumulated['data']['takes']['completed'];?> <?php print t('completed takes'); ?></div>
      <div class="textsub">
        <?php print $accumulated['data']['takes']['passed'];?> <?php print t('passed'); ?>, <?php print $accumulated['data']['takes']['failed'];?> <?php print t('failed'); ?><br />
        <i>(<?php print $accumulated['data']['takes']['in_progress'];?> <?php print t('in progress, not included'); ?>)</i>
      </div>
    </div>
    
    <div class="column">
      <div class="text"><?php print t('Average score'); ?>: <?php print $accumulated['data']['score']['average'];?>%</div>
      <div class="textsub">
        <?php print t('Pass score'); ?>: <?php print $accumulated['data']['score']['pass_rate'];?>%<br />
        <?php print t('Lowest'); ?>: <?php print $accumulated['data']['score']['low'];?>%<br />
        <?php print t('Highest'); ?>: <?php print $accumulated['data']['score']['high'];?>%
      </div>
    </div>
    
    <div class="column">
      <div class="text"><?php print t('Average time'); ?>: <?php print format_interval($accumulated['data']['time']['average']);?></div>
      <div class="textsub">
        <?php print t('Longest'); ?>: <?php print format_interval($accumulated['data']['time']['slowest']);?><br />
        <?php print t('Fastest'); ?>: <?php print format_interval($accumulated['data']['time']['fastest']);?>
      </div>
    </div>
    
  </div>
  <!-- end row -->
  
</div>
<!-- end accumulated -->

<!-- start questions -->
<div class="questions">
<?php
    foreach ($questions as $question ) {
        print $question->output;
    }
?>
</div>
<!-- end questions -->