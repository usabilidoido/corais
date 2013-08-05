<?php echo $show_form ?>
<ul>
  <li><?php echo l(t("Create new poll"), "node/add/makemeeting"); ?></li>
  <li><?php echo l(t("Use this URL: !url", array("!url" => url('makemeeting/' . $node->poll_url, array("absolute" => true)))), "makemeeting/" . $node->poll_url); ?></li>

  <?php

  if (variable_get('makemeeting_send_email_enabled', '1') == 1) {
    echo "<li>";
    echo l(t('Send this poll to your friends'), "makemeeting/" . $node->poll_url . "/sendfw");
    echo "</li>";
  }

  ?>

</ul>

