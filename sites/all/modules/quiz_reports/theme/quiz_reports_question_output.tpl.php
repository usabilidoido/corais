<?php #dpm($question); ?>
<div class="question-wrapper <?php print $question->type;?>">
  <div class="question">
    <span class="text"><?php print ($question->weight+1)?>. <?php print $question->title; ?></span>
  </div>
  <div class="generic">
    <span class="type item"><?php print $question->type_name;?></span>
    <span class="item"><? print t('Taken:'); ?> <? print $question->statistics['generic']['taken']; ?></span>
    <span class="item"><? print t('Correct:'); ?> <? print $question->statistics['generic']['correct_percentage']; ?>% (<? print $question->statistics['generic']['correct']; ?>)</span>
    <span class="item"><? print t('Wrong:'); ?> <? print $question->statistics['generic']['incorrect_percentage']; ?>% (<? print $question->statistics['generic']['incorrect']; ?>)</span>
    <span class="item"><? print t('Skipped:'); ?> <? print $question->statistics['generic']['skipped_percentage']; ?>% (<? print $question->statistics['generic']['skipped']; ?>)</span>
  </div>
  <div class="result">
    <?php print $question_output;?>
  </div>
</div>