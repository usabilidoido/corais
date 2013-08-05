<script>
  $(document).ready(function() {
    jQuery.jcalendar.setLanguageStrings(
            ['<?php echo t('Su'); ?>', '<?php echo t('Mon'); ?>', '<?php echo t('Tue'); ?>', '<?php echo t('Wed'); ?>', '<?php echo t('Th'); ?>', '<?php echo t('Fr'); ?>', '<?php echo t('Sat'); ?>'],
            ['<?php echo ('January'); ?>', '<?php echo t('Febuary'); ?>', '<?php echo t('March'); ?>', '<?php echo t('April'); ?>', '<?php echo t('May'); ?>', '<?php echo t('June'); ?>', '<?php echo t('July'); ?>', '<?php echo t('August'); ?>', '<?php echo t('September'); ?>', '<?php echo t('October') ?>', '<?php echo t('November'); ?>', '<?php echo t('December'); ?>'],
            {p:'&laquo;', n:'&raquo;', t:'<?php echo t('Today'); ?>', b:'<?php echo t('Add date to list'); ?>'}
    );
    $('span.jcalendar').jcalendar();
  });
</script>

<fieldset>
  <legend><?php echo t('Select date'); ?></legend>
  <table width="100%">
    <tr>
      <td>
    <span class="jcalendar">
      
      <div class="jcalendar-selects">
        <select name="day" id="day" class="jcalendar-select-day">
          <option value="0"></option>
          <?php
                for ($i = 1; $i <= 31; $i++) {
          echo "<option value=\"" . $i . "\">" . $i . "</option>";
        }
          ?>
        </select>
        <select name="month" id="month" class="jcalendar-select-month">
          <option value="0"></option>
          <option value="1"><?php echo t('Jan'); ?></option>
          <option value="2"><?php echo t('Feb'); ?></option>
          <option value="3"><?php echo t('Mar'); ?></option>
          <option value="4"><?php echo t('Apr'); ?></option>
          <option value="5"><?php echo t('May'); ?></option>
          <option value="6"><?php echo t('Jun'); ?></option>
          <option value="7"><?php echo t('Jul'); ?></option>
          <option value="8"><?php echo t('Aug'); ?></option>
          <option value="9"><?php echo t('Sep'); ?></option>
          <option value="10"><?php echo t('Oct'); ?></option>
          <option value="11"><?php echo t('Nov'); ?></option>
          <option value="12"><?php echo t('Dec'); ?></option>
        </select>
        <select name="year" id="year" class="jcalendar-select-year">
          <option value="0"></option>
          <?php
                $this_year = date("Y");
          for ($i = $this_year; $i <= $this_year + 10; $i++) {
            echo "<option value=\"" . $i . "\">" . $i . "</option>";
          }
          ?>
        </select>
      </div>
    </span>
      </td>
    </tr>
    <tr>
      <td>
        <?php echo t('Selected date(s):'); ?>
        <div class="description"><?php echo $element['#description']; ?></div>
        <div id="selected_dates">
          <table>
          <?php
          if (is_array($element["#attributes"]['selected_dates_and_options'])) {
            $arr = $element["#attributes"]['selected_dates_and_options'];
            $rowCount = 1;
            $rows_num = sizeof($arr) / 2;
            $days = array();
            for ($i = 0; $i < $rows_num; $i++) {
              $collCount = 1;
              echo "<tr id=\"jcalendar_row_" . $rowCount . "\">
        <td>[<a href=\"javascript:jcalendar_remove_row('" . $rowCount . "')\">X</a>]&nbsp;<span id=\"daystr_" . $rowCount . "\">" . $arr['date_' . $rowCount] . "</span><input name=\"day_" . $rowCount . "\" value=\"" . $arr['date_' . $rowCount] . "\" type=\"hidden\">:</td> \n";

              foreach ($arr['option_' . $rowCount] as $option) {
                $suffix_str = $rowCount . "_" . $collCount++;
                echo "\n<td id=\"jcell_" . $suffix_str . "\">
            <input name=\"day_option_" . $suffix_str . "\" id=\"jtfield_" . $suffix_str . "\" value=\"" . $option . "\" type=\"text\">
          </td> \n";
              }

              $days[] = $arr['date_' . $rowCount];

              echo "</tr> \n";
              $rowCount++;
            }

            // set javascript variables
            echo "
    <script>
      counter = " . $rowCount . ";
      rowCounter = " . ($rowCount - 1) . ";
      collCounter = " . ($collCount - 1) . ";";
            foreach ($days as $day) {
              echo "addedDates['" . $day . "'] = 1;";
            }
            echo "
    </script>";
          }
            ?>
          </table>
        </div>
        <div id="selected_navlinks">
          <ul>
            <li><a href="javascript:jcalendar_add_collumn()"><?php echo t('Add collumn'); ?></a></li>
            <li><a href="javascript:jcalendar_remove_collumn()"><?php echo t('Remove last collumn'); ?></a></li>
            <li><a href="javascript:jcalendar_copy_first()"><?php echo t('Copy first row'); ?></a></li>
          </ul>
        </div>
      </td>
    </tr>
  </table>
</fieldset>

