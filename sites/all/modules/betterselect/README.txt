
CONTENTS OF THIS FILE
---------------------

 * Introduction
 * Installation


INTRODUCTION
------------

Authors:
* Jeff Robbins (jjeff)
* Károly Négyesi (chx)
* Angela Byron (webchick)
* Stein Setvik (setvik)
* Mark Theunissen (Mark Theunissen)
* Nicolas Borda (ipwa)

This Module Made by Robots: http://www.lullabot.com

Multiselect HTML select elements are hard for users. Selecting more than one
requiress Ctrl+clicking (or Cmd+clicking on Macs) and explaining this to users
who sometimes have trouble even clicking the mouse at all is no one's idea of
fun.

Better Select module attempts to override all multiselect HTML elements in Drupal
and replaces them with checkboxes, in an auto-scrolling div so they don't take up
much room on the page.

Some select lists may not be compatible with Better Select. In this case, they
will not be converted. If you'd like a certain list in Better Select format,
please make an issue with details of the list. Don't forget to search for
the issue first, in case someone has already made it.


INSTALLATION
------------

1. Copy the betterselect directory to your sites/SITENAME/modules directory.

2. Enable the module at Administer >> Site building >> Modules.

3. Configure the module at admin/settings/betterselect.

That's it! This module will automatically attempt to convert any multiselect
HTML elements to use the new stylized checkboxes.
