<?php  
$specific = $statistics['specific'];
$options = $statistics['specific']['options'] ? $statistics['specific']['options'] : array();
$properties = $statistics['specific']['properties'];
?>
<table border="0" cellspacing="0" cellpadding="0" class="horizontal-bars">
<? foreach ($options as $option) : ?>
    <tr>
      <td class="option"><?php print $option['title']?></td>
      <td class="meta<?php print $option['correct'] ? ' correct' : '' ?>"></td>
      <td class="result<?php print $option['correct'] ? ' correct' : '' ?>"><span class="bar" style="width:<?php print $option['percentage']?>%;"><? if ( $option['percentage'] > 95 ) : ?><span class="percent"><?php print $option['percentage']?>% (<?php print $option['count'];?>)</span><? endif; ?></span> <? if ( $option['percentage'] <= 95 ) : ?><span class="percent"><?php print $option['percentage']?>% (<?php print $option['count'];?>)</span><? endif; ?></td>
    </tr>
<? endforeach; ?>
    <tr>
      <td colspan="2"></td>
      <td class="grid">
        <div><div><div><div><div><div><div><div><div><div><div></div></div></div></div></div></div></div></div></div></div></div>
      </td>
    </tr>
    <tr>
      <td colspan="2"></td>
      <td>
        <input type="checkbox" <? if ($properties['choice_multi']) : ?>checked="checked"<? endif;?> disabled="disabled"> <?php print t('Multiple answers'); ?>
        <input type="checkbox" <? if ($properties['choice_random']) : ?>checked="checked"<? endif;?> disabled="disabled"> <?php print t('Random order'); ?>
        <input type="checkbox" <? if ($properties['choice_boolean']) : ?>checked="checked"<? endif;?> disabled="disabled"> <?php print t('Simple scoring'); ?>
      </td>
    </tr>
</table>