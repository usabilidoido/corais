<?php

$vote_stat = array();

?>

<div class="description">
  <?php echo check_plain($element['#description']); ?>
</div>

<table id="pollpanel">
  <tr>
    <th></th>

<?php

    $answers = array();
    foreach ($element['#attributes']['answers'] as $fieldname => $answer) {
      $answer_id = str_replace("field", "", $fieldname);
      $answers[$answer_id] = $answer;
      echo "<th class=\"bleft\">" . $answer . "</th>";
    }

    ?>
  </tr>

<?php

  if ($element['#attributes']['secure'] == 0) {
    foreach ($element['#attributes']['votes'] as $username => $vote_arr) {
      echo "<tr>\n";
      echo "<td class=\"ct\"><b>" . check_plain($username) . "</b></td>\n";
      foreach ($answers as $answer_id => $answer) {
        if (array_key_exists($answer_id, $vote_arr)) {
          if ($vote_arr[$answer_id] == 1)
            echo "<td class=\"voted_ok ct\">" . t('Yes') . "</td>\n";
          else if ($vote_arr[$option_id] == 0)
            echo "<td class=\"voted_no ct\">" . t('No') . "</td>\n";

          $vote_stat[$answer_id][$vote_arr[$answer_id]]++;
        }
        else {
          echo "<td class=\"voted_no ct\">&nbsp;-</td>\n";
          $vote_stat[$answer_id][0]++;
        }
      }
      echo "</tr>\n";
    }
  } else {
    echo "<tr><td colspan=\"" . (sizeof($answers) + 1) . "\">" . t("You're not allowed to see the previous votes.") . "</td></tr>\n";
  }

  ?>
  <tr>
    <td><?php echo t('Enter your name:'); ?><br/>
      <input type="text" name="your_name" value="<?php echo $element['#value']['name'] ?>">
    </td>

<?php

// showing the user input row
    foreach ($answers as $answer_id => $answer) {
      if ($element['#attributes']['multiple_allowed'] == 0) {
        echo "<td class=\"ct\"><input type=\"radio\" name=\"vote\" value=\"" . $answer_id . "\"></td>\n";
      }
      else {
        echo "<td class=\"ct\"><input type=\"checkbox\" name=\"vote[]\" value=\"" . $answer_id . "\"></td>\n";
      }
    }
    ?>

  </tr>
  <tr>

<?php

// showing some stats (if the answers aren't hidden)

    if ($element['#attributes']['secure'] == 0) {
      echo "<td class=\"ct\">
      <b>" . t("Summary:") . "</b>
      </td>";
      if (sizeof($vote_stat) > 0) {
        foreach ($vote_stat as $option_id => $value_arr) {

          if (!isset($value_arr[1]))
            $value_arr[1] = 0;
          if (!isset($value_arr[0]))
            $value_arr[0] = 0;

          $yes = $value_arr[1];
          $no = $value_arr[0];

          if ($yes > $no)
            $style = "green";
          else if ($yes == $no)
            $style = "yellow";
          else
            $style = "red";

          echo "<td class=\"ct $style\">" . t("Yes: @num", array("@num" => $yes)) . "<br /> \n";
          echo t("No: @num", array("@num" => $no)) . "<br /> \n";
        }
      }
      else {
        echo "<td colspan=\"" . (sizeof($option_ids)) . "\">" . t("No votes yet.") . "</td>\n";
      }
    }

    ?>

  </tr>
</table>
