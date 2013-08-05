;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Dirty Forms
;;
;; Project homepage: http://drupal.org/project/dirtyforms
;; Module Author: markus_petrux (http://drupal.org/user/39593)
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

OVERVIEW
========

The Dirty Forms module provides a javascript behavior that checks forms before
users leave the page, warning them if they are about to loose changes and
providing an option to remain on the page.

This feature is based on the onBeforeUnload event of the window object, and it
is only supported by IE4+ and Mozilla Firefox. Other browsers are not affected.
For further information, please, consult the project page of the onBeforeUnload
API module.


REQUIREMENTS
============

- http://drupal.org/project/onbeforeunload (onBeforeUnload API).


INSTALLATION
============

- Copy all contents of this package to your modules directory preserving
  subdirectory structure.

- Goto Administer > Site building > Modules to install this module.

- Goto Administer > Site configuration > Dirty Forms to configure the module
  settings. Here you can provide rules to exclude forms from the dirtyForms
  behavior.
