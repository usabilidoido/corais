<?php
$generic = $statistics['generic'];
$specific = $statistics['specific'];
$takes = $specific['takes'];
?>
<table border="0" cellspacing="0" cellpadding="0" class="horizontal-bars">
  <tr>
    <td class="option">75%-100%</td>
    <td class="meta"></td>
    <td class="result"><span class="bar" style="width:<?php print $takes['percent_100']?>%;"><? if ( $takes['percent_100'] > 95 ) : ?><span class="percent"><?php print $takes['percent_100']?>% (<?php print $takes['100'];?>)</span><? endif; ?></span> <? if ( $takes['percent_100'] <= 95 ) : ?><span class="percent"><?php print $takes['percent_100']?>% (<?php print $takes['100'];?>)</span><? endif; ?></td>
  </tr>
  <tr>
    <td class="option">50%-75%</td>
    <td class="meta"></td>
    <td class="result"><span class="bar" style="width:<?php print $takes['percent_75']?>%;"><? if ( $takes['percent_75'] > 95 ) : ?><span class="percent"><?php print $takes['percent_75']?>% (<?php print $takes['75'];?>)</span><? endif; ?></span> <? if ( $takes['percent_75'] <= 95 ) : ?><span class="percent"><?php print $takes['percent_75']?>% (<?php print $takes['75'];?>)</span><? endif; ?></td>
  </tr>
  <tr>
    <td class="option">25%-50%</td>
    <td class="meta"></td>
    <td class="result"><span class="bar" style="width:<?php print $takes['percent_50']?>%;"><? if ( $takes['percent_50'] > 95 ) : ?><span class="percent"><?php print $takes['percent_50']?>% (<?php print $takes['50'];?>)</span><? endif; ?></span> <? if ( $takes['percent_50'] <= 95 ) : ?><span class="percent"><?php print $takes['percent_50']?>% (<?php print $takes['50'];?>)</span><? endif; ?></td>
  </tr>
  <tr>
    <td class="option">0-25%</td>
    <td class="meta"></td>
    <td class="result"><span class="bar" style="width:<?php print $takes['percent_25']?>%;"><? if ( $takes['percent_25'] > 95 ) : ?><span class="percent"><?php print $takes['percent_25']?>% (<?php print $takes['25'];?>)</span><? endif; ?></span> <? if ( $takes['percent_25'] <= 95 ) : ?><span class="percent"><?php print $takes['percent_25']?>% (<?php print $takes['25'];?>)</span><? endif; ?></td>
  </tr>
  <tr>
    <td class="option"><?php print t('Not evaluated'); ?></td>
    <td class="meta"></td>
    <td class="result neutral"><span class="bar" style="width:<?php print $takes['percent_not_evaluated']?>%;"><? if ( $takes['percent_not_evaluated'] > 95 ) : ?><span class="percent"><?php print $takes['percent_not_evaluated']?>% (<?php print $takes['75'];?>)</span><? endif; ?></span> <? if ( $takes['percent_not_evaluated'] <= 95 ) : ?><span class="percent"><?php print $takes['percent_not_evaluated']?>% (<?php print $takes['not_evaluated'];?>)</span><? endif; ?></td>
  </tr>
  <tr>
    <td colspan="2"></td>
    <td class="grid">
      <div><div><div><div><div><div><div><div><div><div><div></div></div></div></div></div></div></div></div></div></div></div>
    </td>
  </tr>
</table>

<div class="answers">
<? foreach ($specific['answers'] as $key=>$answer ) : ?>
    <div class="answer">
        <div class="info">
            <span class="index">#<? print ($key+1); ?></span>
            <? if (!$answer['is_evaluated']) : ?>
            <span class="icon"><img src="<? print base_path() . drupal_get_path( 'module', 'quiz_stats' ); ?>/theme/icons/icon_not_evaluated.png" alt="Answer is not evaluated" /></span>
            <? else : ?>
                <? if ($answer['score'] == $generic['max_score']) : ?>
                    <span class="icon"><img src="<? print base_path() . drupal_get_path( 'module', 'quiz_stats' ); ?>/theme/icons/icon_correct.png" alt="Answer is correct" /></span>
                <? else: ?>
                    <span class="icon"><img src="<? print base_path() . drupal_get_path( 'module', 'quiz_stats' ); ?>/theme/icons/icon_wrong.png" alt="Answer is incorrect" /></span>
                <? endif; ?>
                <span class="score">Score: <?php print $answer['score']; ?>/<?php print $generic['max_score']?></span>
            <? endif; ?>
        </div>
        <div class="output"><? if ($answer['short_answer'] ) { print '<div class="short-answer">' . $answer['short_answer'] . '</div>'; print '<div class="long-answer">' . $answer['answer'] . '</div>'; } else print $answer['answer']; ?></div>
    </div>
<? endforeach; ?>
</div>