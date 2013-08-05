<table>
  <tr>
    <th colspan="3"><?php echo t("Poll title"); ?></th>
  </tr>
  <?php
  foreach ($data as $row) {
  echo "<tr>";
  echo "<td>" . $row['title'] . "</td>";
  echo "<td>";
  echo l(t("Show poll"), 'makemeeting/' . $row['url']);
  echo "</td>";
  echo "<td>";
  echo l(t("Edit poll"), 'makemeeting/' . $row['admin_url']);
  echo "</td>";
  echo "</tr>";
}
  ?>
</table>

