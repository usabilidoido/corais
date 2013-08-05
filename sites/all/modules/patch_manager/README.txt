<?php

Patch manager provides a developer tool for managing patches
to core and contributed modules.


CONTENTS OF THIS FILE
---------------------

 * Requirements
 * Patch naming
 * Configuration
 * More Information

REQUIREMENTS
------------

The patch_manager module requires the following modules:
  views
  cck (text submodule)
  filefield
  views_bulk_operations (optional)
  filefield_paths (optional)

It helps to have a working patch binary on your system that
is executable by the webserver user.


PATCH NAMING
------------

If you have filefield_paths installed (with token, and pathauto),
this will allow you to automatically rename patch files. The
Drupal suggested naming scheme is:
  module-description-issue-comment.patch

With tokens, this would be:
  [field_module-raw]-[title]-[field_drupal_issue-raw].patch
  
You can set this up here:
  /admin/content/node-type/patch/fields/field_patch


CONFIGURATION
-------------

Access the configuration page to set the path to your patch
binary. This will allow you to patch and reverse patches through
the administration interface.
  /admin/build/patch

You can start adding patches immediately, as you would a normal node:
  /node/add/patch

You can modify the default fields to store extra information via
the content module:
  /admin/content/node-type/patch/fields
  
Everything is powered by Views, so you can modify or create new views:
  /admin/build/views/edit/patches


MORE INFORMATION
----------------

See the project page at:
  http://drupal.org/project/patch_manager

