Imagepicker postlet module for Drupal 6.x

This module depends on the Imagepicker module

imagepicker.module,v 1.21.2.28 or later

If you have any questions or suggestions please contact me at
http://drupal.org/user/52366

To install this module, enable in the usual way:
Go to Administer > Site building > Modules and select it,
then hit Save Configuration

Then go to Admin > Site configuration > Imagepicker and
set it up to suit your needs.

See
 * http://www.postlet.com/
 * http://sourceforge.net/projects/postlet/
for further details of the Postlet Java Applet that powers this module.

It provides an upload facility that allows multipile selection and
uploading of images. It uses whatever default setting a user may have
and applies them to all images.

Usage can be restricted by role and size adjusted to suit your template.

The output is themed. Copy function theme_imagepicker_postlet_view() over
to your template.php, rename according to the name of your template, eg
function mytheme_imagepicker_postlet_view()
and tweak the postlet output there.
