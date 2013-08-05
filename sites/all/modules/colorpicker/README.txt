; $Id$

This module does nothing by itself but add a new form element type called a 'colorpicker_textfield'. You can see an example of its use in the colorpicker_example.module included in the package with this module, or use something similar to the code below.

Note that a default value (in hex) is necessary for this element type to work properly, whether you use it in your own module or as a CCK field. The field can now be left blank for those modules (e.g., onepixelout) that treat a blank entry as indicating a default color.

Beginning with Version 2 of the module, each colorpicker_textfield has its own popup colorpicker, and you no longer need to include a colorpicker as a separate form element.

  $form['sample_color'] = array(
    '#type' => (module_exists('colorpicker') ? 'colorpicker_' : '') . 'textfield',
    '#title' => t('Color picker textfield'),
    '#description' => t('This is a textfield with a Farbtastic color picker'),
    '#default_value' => variable_get('sample_color', '#000000'),
  );


To add support for Colorpicker 2 in your own modules (so they can also work without Colorpicker) you can use the following code to check for the presence of Colorpicker 2 or later and assign the appropriate type to a textfield:

  $color_field_type = (function_exists('colorpicker_2_or_later') ? 'colorpicker_' : '') .'textfield';


Then you just set the #type of each color textfield to the $color_field_type like so:

  $form['sample_color'] = array(
    '#type' => $color_field_type,
    '#title' => t('Color picker textfield'),
    '#description' => t('Please enter a color value in the form #RRGGBB'),
    '#default_value' => variable_get('sample_color', '#000000'),
  );


If you want your module to support Colorpicker 1 also, add a #colorpicker attribute to your colorpicker_textfields and at least one colorpicker to the form. See the example module with Colorpicker 1 for more options. The most basic code you would use looks like this:

  $can_pick = module_exists('colorpicker');
  $color_field_type = ($can_pick ? 'colorpicker_' : '') .'textfield';


  $form['sample_color'] = array(
    '#type' => $color_field_type,
    '#title' => t('Color picker textfield'),
    '#description' => t('Please enter a color value in the form #RRGGBB'),
    '#default_value' => variable_get('sample_color', '#000000'),
    '#colorpicker' => 'sample_picker',
  );
  if ($can_pick) {
    $form['sample_picker'] = array(
      '#type' => 'colorpicker',
      '#title' => t('Color picker'),
      '#description' => t('This is a Farbtastic colorpicker associated with a textfield.'),
    );
  }
