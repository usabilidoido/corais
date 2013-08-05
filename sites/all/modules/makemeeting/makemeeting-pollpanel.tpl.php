<?php 

// arrays for datas

$options_ids = array();
$vote_stat = array();

?>

<div class="description">
  <?php echo check_plain($element['#description']); ?>
</div>
<table id="pollpanel">
  <tr>
    <th></th>

    <?php

// showing the days

    foreach ($element['#attributes']['days_and_options'] as $days => $options_array) {
      $start = strpos($days, "_");
      $date_str = substr($days, $start + 1);
      echo "<th colspan=\"" . sizeof($options_array) . "\" class=\"bleft\">" . $date_str . "</th>\n";
    }
    ?>

  </tr>
  <tr>
    <td></td>

    <?php

// showing the options of days

    foreach ($element['#attributes']['days_and_options'] as $days => $options_array) {
      foreach ($options_array as $k => $option) {
        $start = strpos($option, "_");
        $option_str = substr($option, $start + 1);
        $option_id = substr($option, 0, $start);
        $option_ids[] = $option_id;

        echo "<td class=\"option ct\">" . check_plain($option_str) . "</td>\n";
      }
    }
    ?>

  </tr>

  <?

// showing the previous answers (if not hidden)

  if ($element['#attributes']['secure'] == 0) {
    foreach ($element['#attributes']['answers'] as $username => $vote_arr) {
      echo "<tr>\n";
      echo "<td class=\"ct\"><b>" . check_plain($username) . "</b></td>\n";
      foreach ($option_ids as $option_id) {
        if (array_key_exists($option_id, $vote_arr)) {
          if ($vote_arr[$option_id] == 1) {
            echo "<td class=\"voted_ok ct\">" . t('Yes') . "</td>\n";
          }
          else {
            if ($vote_arr[$option_id] == 2) {
              echo "<td class=\"voted_maybe ct\">" . t('Maybe') . "</td>\n";
            }
            else {
              if ($vote_arr[$option_id] == 0) {
                echo "<td class=\"voted_no ct\">" . t('No') . "</td>\n";
              }
            }
          }

          $vote_stat[$option_id][$vote_arr[$option_id]]++;
        }
        else {
          echo "<td class=\"voted_no ct\">&nbsp;-</td>\n";
          $vote_stat[$option_id][0]++;
        }
      }
      echo "</tr>\n";
    }
  }
  else {
    echo "<tr><td colspan=\"" . (sizeof($option_ids) + 1) . "\">" . t("You're not allowed to see the previous votes.") . "</td></tr>\n";
  }

  ?>

  <tr>
    <?php
      global $user;
      if ($element['#value']['name'] == '') {
        $element['#value']['name'] = $user->name;
      }
    ?>
      <td><?php echo t('Enter your name:'); ?><br/>
        <input type="text" name="your_name" value="<?php echo $element['#value']['name'] ?>">
      </td>

    <?php

// showing the user input row

    foreach ($element['#attributes']['days_and_options'] as $days => $options_array) {
      $start_day = strpos($days, "_");
      $day_str = substr($days, $start_day + 1);
      $day_id = substr($days, 0, $start_day);
      foreach ($options_array as $k => $option) {
        $start_option = strpos($option, "_");
        $option_id = substr($option, 0, $start_option);
        $option_str = substr($option, $start_option + 1);

        if ($element['#attributes']['maybe_option'] == 1) {
          echo "
        <td>
          <input type=\"radio\" name=\"option_" . $option_id . "\" value=\"1\"> " . t('Yes') . " <br />
          <input type=\"radio\" name=\"option_" . $option_id . "\" value=\"0\"> " . t('No') . " <br />
          <input type=\"radio\" name=\"option_" . $option_id . "\" value=\"2\"> " . t('Maybe') . "
        </td> ";
        }
        else {
          if ($element['#attributes']['multiple_allowed'] == 0) {
            echo "<td class=\"ct\"><input type=\"radio\" name=\"option_$day_id\" value=\"$option_id\"></td>\n";
          }
          else {
            echo "<td class=\"ct\"><input type=\"checkbox\" name=\"option_" . $option_id . "\" value=\"1\"></td>\n";
          }
        }
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

          if (!isset($value_arr[1])) {
            $value_arr[1] = 0;
          }
          if (!isset($value_arr[0])) {
            $value_arr[0] = 0;
          }
          if (!isset($value_arr[2])) {
            $value_arr[2] = 0;
          }

          $yes = $value_arr[1];
          $no = $value_arr[0];
          $maybe = $value_arr[2];

          if ($element['#attributes']['maybe_option'] == 1) {
            if ($no > $yes + $maybe) {
              $style = "red";
            }
            else {
              if ($yes > $no + $maybe) {
                $style = "green";
              }
              else
              {
                $style = "yellow";
              }
            }

          }
          else {
            if ($yes > $no) {
              $style = "green";
            }
            else {
              if ($yes == $no) {
                $style = "yellow";
              }
              else
              {
                $style = "red";
              }
            }
          }

          echo "<td class=\"ct $style\">" . t("Yes: @num", array("@num" => $yes)) . "<br /> \n";
          echo t("No: @num", array("@num" => $no)) . "<br /> \n";

          if ($element['#attributes']['maybe_option'] == 1) {
            echo t("Maybe: @num", array("@num" => $maybe)) . "<br /> \n";
          }
        }
      }
      else {
        echo "<td colspan=\"" . (sizeof($option_ids)) . "\">" . t("No votes yet.") . "</td>\n";
      }
    }

    ?>

  </tr>
</table>

