<?php

$max_fieldnum = db_result(db_query("SELECT MAX(answer_id) FROM {makemeeting_poll_rows}"));

?>

<script language="javascript" type="text/javascript">
  var makemeeting_fieldnum = <?php echo sizeof($element['#attributes']['answers']); ?>;
  var makemeeting_fieldname = <?php echo $max_fieldnum; ?>;

  function makemeeting_delete_row(obj) {
    if (makemeeting_fieldnum > 1) {
      $('#' + obj).remove();
      makemeeting_fieldnum--;
    }
    return false;
  }

  function makemeeting_add_row() {
    makemeeting_fieldnum++;
    makemeeting_fieldname++;
    $('#makemeeting_rows').append("<div id=\"id_field" + makemeeting_fieldname + "\">[<a href=\"#\" onclick=\"return makemeeting_delete_row('id_field" + makemeeting_fieldname + "')\">x</a>] <input type=\"text\" size=\"60\" name=\"field" + makemeeting_fieldname + "\" value=\"\"></div>\n");

    // some animation
    $('#id_field' + makemeeting_fieldname).hide().show("fast");
    return false;
  }
</script>

<div id="makemeeting_rows">
<?php

  foreach ($element['#attributes']['answers'] as $field => $value) {
    echo "<div id=\"id_" . $field . "\">[<a href=\"#\" onclick=\"return makemeeting_delete_row('id_" . $field . "')\">x</a>] <input type=\"text\" size=\"60\" name=\"" . $field . "\" value=\"" . $value . "\"></div>";
  }

  ?>
</div>
<ul>
  <li><a href="#" onclick="return makemeeting_add_row()"><?php echo t('Add extra field'); ?></a></li>
</ul>
