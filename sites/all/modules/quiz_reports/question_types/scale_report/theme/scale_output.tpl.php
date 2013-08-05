<?php
$options = $statistics['specific']['options'];
$data = $statistics['specific']['data'];
?>
<table border="0" cellspacing="0" cellpadding="0" class="horizontal-bars">
<? foreach ($options as $key=>$value) : ?>
  <tr>
    <td class="option"><?php print $value?></td>
    <td class="meta"></td>
    <td class="result">
      <span class="bar" style="width:<?php print $data['percentage_' . $key]?>%;">
        <? if ( $data['percentage_' . $key] > 95 ) : ?><span class="percent"><?php print $data['percentage_' . $key]?>% (<?php print $data[$key];?>)</span><? endif; ?>
      </span> 
      <? if ( $data['percentage_' . $key] <= 95 ) : ?><span class="percent"> <?php print $data['percentage_' . $key]?>% (<?php print $data[$key];?>)</span><? endif; ?>
    </td>
  </tr>
<? endforeach; ?>
  <tr>
    <td colspan="2"></td>
    <td class="grid">
      <div><div><div><div><div><div><div><div><div><div><div></div></div></div></div></div></div></div></div></div></div></div>
    </td>
  </tr>
</table>