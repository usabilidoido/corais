<br/>
<b>Admin links</b>
<ul>
  <li><?php echo l(t("Admin page URL: !url", array("!url" => url('makemeeting/' . $node->poll_admin_url, array("absolute" => true)))), "makemeeting/" . $node->poll_admin_url); ?></li>
  <li><?php echo l(t("Show poll"), 'makemeeting/' . $node->poll_url); ?></li>
  <li><?php echo l(t("Show log"), 'makemeeting/' . $node->poll_admin_url . '/log'); ?></li>
  <li><?php echo l(t("Send poll link to friends"), 'makemeeting/' . $node->poll_url . '/sendfw'); ?></li>
</ul>
<hr/>

<?php echo $admin_form; ?>
