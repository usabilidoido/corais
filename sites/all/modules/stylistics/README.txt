// $Id: README.txt,v 1.1 2010/06/19 15:43:12 adaddinsane Exp $

Stylistics - Javascript and CSS Reloader
========================================

This is a utility module for developers deals with a specific problem: If a
module loads its own Javascript and/or CSS files in hook_form_alter() - and
then there is an error when the form is submitted the hook will not be called
again (the form is cached) which means that, on page reload, the Javascript
and CSS files are not reloaded.

The module, attachments (http://drupal.org/project/attachments), had this
problem but it can also be found in the Node Expire module (which is no longer
being maintained but is the best of the node expiration modules) and it's bound
to exist in other modules.

There are two ways of using this module:

    * A developer can simply insert the necessary settings for the Stylistics 
      module into their hook_form_alter(), instead of loading their JS and CSS
      files at that point and let the Stylistics module handle all the file
      loading whether first load, reload or on error;
    * Create some override code which adds the JS and CSS files for some other
      modules but only to be loaded on form error (to avoid problems with JS
      settings being loaded twice);

How it works
------------
The Stylistics module hooks into every form, adding a "pre_render" routine. In
the pre_render routine it searches the whole form array looking for a
"#stylistics" element (though normally this would probably be at the top level
of the form structure). The structure of the "#stylistics" array looks like this:

$form['#stylistics'] = array(
  '<modulename>' => array(
    'js' => array(
       <array of drupal_add_js() arguments>,
       <array of drupal_add_js() arguments>,
       ...,
    );
    'css' => array(
       <array of drupal_add_css() arguments>,
       <array of drupal_add_css() arguments>,
       ...,
    );
    'js_skip' => array(
       <array of drupal_add_js() arguments>,
       <array of drupal_add_js() arguments>,
       ...,
    );
  ),
  'jquery_ui' => array(
     <name of jquery_ui package>,
     <name of jquery_ui package>,
     ...,
  );
);

So rather than using:

drupal_add_js(drupal_get_path('module', 'mymodule') . '/mymodule.js);
drupal_add_css(drupal_get_path('module', 'mymodule') . '/mymodule.css);

You would use:

$form['#stylistics']['mymodule']['js'][] = array(drupal_get_path('module', 'mymodule') . '/mymodule.js);
$form['#stylistics']['mymodule']['css'][] = array(drupal_get_path('module', 'mymodule') . '/mymodule.css);

The reason for including the modulename is so that the module code can test
isset($form['#stylistics']['mymodule']) to avoid multiple loading of any JS
settings or inline code (which is a bad thing, particularly for Javascript
settings). It doesn't matter at all for jquery_ui packages which is why it's
out on its own.

Other people's modules
----------------------
The 'js_skip' array does not load any Javascript in its list the first time
the form is loaded, but does on any subsequent page load. This would normally
be used if you wanted to ensure that Javascript settings or inline code are
loaded correctly. The situation would be that there's a faulty module, such
as the Node Expire module, which uses hook_form_alter() to load its Javascript
settings. Stylistics must not load the settings the first time the form is
rendered, because that will corrupt the settings (already loaded by Node
Expire), but every time thereafter, if there's a form submission error, the
settings must be reloaded.

It doesn't matter if there's multiple attempts to load normal JS, CSS and
jquery_ui files, Drupal ignores the multiple requests and just does it once.
The Javascript settings and possibly inline Javascript are the problems.

Steve Turnbull (adaddinsane)
