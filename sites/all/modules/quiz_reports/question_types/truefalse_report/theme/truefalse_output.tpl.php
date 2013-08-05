<?php 
$specific = $statistics['specific'];
?>
<table border="0" cellspacing="0" cellpadding="0" class="horizontal-bars">
    <tr>
      <td class="option"><?php print t('True');?></td>
      <td class="meta<?php print $specific['correct'] == '1' ? ' correct' : '' ?>"></td>
      <td class="result<?php print $specific['correct'] == '1' ? ' correct' : '' ?>">
        <span class="bar" style="width:<?php print $specific['percentage_true']?>%;"><? if ( $specific['percentage_true'] > 95 ) : ?>
          <span class="percent"><?php print $specific['percentage_true']?>% (<?php print $specific['true'];?>)</span><? endif; ?>
        </span> 
        <? if ( $specific['percentage_true'] <= 95 ) : ?>
        <span class="percent"><?php print $specific['percentage_true']?>% (<?php print $specific['true'];?>)</span>
        <? endif; ?>
      </td>
    </tr>
    <tr>
        <td class="option"><?php print t('False');?></td>
        <td class="meta<?php print $specific['correct'] == '2' ? ' correct' : '' ?>"></td>
        <td class="result<?php print $specific['correct'] == '2' ? ' correct' : '' ?>"><span class="bar" style="width:<?php print $specific['percentage_false']?>%;"><? if ( $specific['percentage_false'] > 95 ) : ?><span class="percent"><?php print $statistics['percentage_false']?>% (<?php print $specific['true'];?>)</span><? endif; ?></span> <? if ( $specific['percentage_false'] <= 95 ) : ?><span class="percent"><?php print $specific['percentage_false']?>% (<?php print $specific['false'];?>)</span><? endif; ?></td>
    </tr>
    <tr>
        <td colspan="2"></td>
        <td class="grid">
          <div><div><div><div><div><div><div><div><div><div><div></div></div></div></div></div></div></div></div></div></div></div>
        </td>
      </tr>
</table>