<table>
  <thead>
  <th><?php echo t('Name'); ?></th>
  <th><?php echo t('Date'); ?></th>
  </thead>

  <?php

  foreach ($rows as $row) {
    echo "
    <tr>
      <td>" . $row['username'] . "</td>
      <td>" . $row['dt'] . "</td>
    </tr>
  ";
  }

  ?>

</table>
