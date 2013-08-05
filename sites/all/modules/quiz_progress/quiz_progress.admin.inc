<?php

function quiz_progress_config_form() {
  $form['quiz_progress_output_style'] = array(
    '#title' => t('Output style'),
    '#type' => 'radios',
    '#options' => array(
      'number' => t('Question numbers'),
      'title' => t('Questions title')
    ),
    '#default_value' => variable_get('quiz_progress_output_style', 'number'),
  );
  $form['quiz_progress_number_style_col_size'] = array(
    '#title' => t('Column size for "<em>question numbers</em>" style output'),
    '#description' => t('Counts of question numbers to be displayed per row in questions numbers stype output.'),
    '#type' => 'textfield',
    '#default_value' => variable_get('quiz_progress_number_style_col_size', 4),
    '#maxlength' => 3,
    '#size' => 4,
  );
  $form['quiz_progress_title_style_length'] = array(
    '#title' => t('Maximum length of title for "<em>question numbers</em>" style output'),
    '#description' => t('Counts of question numbers to be displayed per row in questions numbers stype output.'),
    '#type' => 'textfield',
    '#default_value' => variable_get('quiz_progress_title_style_length', 32),
    '#maxlength' => 3,
    '#size' => 4,
  );
  $form['quiz_progress_output_color'] = array(
    '#type' => 'fieldset',
    '#title' => t('Color'),
    '#description' => t('Hexa color code to hightlight upcoming, skipped and answered questions. Eg #FFFFFF for white. Leave this field empty to use default color defined in your theme.'),
  );
  // Answered questions
  $form['quiz_progress_output_color']['quiz_progress_output_bg_color_answered'] = array(
    '#title' => t('Answered questions background color'),
    '#type' => 'textfield',
    '#default_value' => variable_get('quiz_progress_output_bg_color_answered', ''),
    '#maxlength' => 7,
    '#size' => 10,
  );
  $form['quiz_progress_output_color']['quiz_progress_output_text_color_answered'] = array(
    '#title' => t('Answered questions text color'),
    '#type' => 'textfield',
    '#default_value' => variable_get('quiz_progress_output_text_color_answered', '#33A333'),
    '#maxlength' => 7,
    '#size' => 10,
  );
  // skipped questions
  $form['quiz_progress_output_color']['quiz_progress_output_bg_color_skipped'] = array(
    '#title' => t('Skipped questions background color'),
    '#type' => 'textfield',
    '#default_value' => variable_get('quiz_progress_output_bg_color_skipped', ''),
    '#maxlength' => 7,
    '#size' => 10,
  );
  $form['quiz_progress_output_color']['quiz_progress_output_text_color_skipped'] = array(
    '#title' => t('Skipped questions text color'),
    '#type' => 'textfield',
    '#default_value' => variable_get('quiz_progress_output_text_color_skipped', '#A30000'),
    '#maxlength' => 7,
    '#size' => 10,
  );
  // upcoming questions
  $form['quiz_progress_output_color']['quiz_progress_output_bg_color_queued'] = array(
    '#title' => t('Upcoming questions Background color'),
    '#type' => 'textfield',
    '#default_value' => variable_get('quiz_progress_output_bg_color_queued', ''),
    '#maxlength' => 7,
    '#size' => 10,
  );
  $form['quiz_progress_output_color']['quiz_progress_output_text_color_queued'] = array(
    '#title' => t('Upcoming questions Background color'),
    '#type' => 'textfield',
    '#default_value' => variable_get('quiz_progress_output_text_color_queued', '#027AC6'),
    '#maxlength' => 7,
    '#size' => 10,
  );
  return system_settings_form($form);
}
