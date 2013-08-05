
Finder README

CONTENTS OF THIS FILE
----------------------

  * Introduction
  * Installation
  * Configuration


INTRODUCTION
------------
Finder allows Drupal site administrators to create flexible search forms to
find objects such as Nodes or Users based on the values of a field.
Maintainer: Daniel Braksator (http://drupal.org/user/134005)
Project page: http://drupal.org/project/finder.


INSTALLATION
------------
1. Copy finder folder to modules (usually sites/all/modules) directory.
2. At admin/build/modules enable the Finder module.


CONFIGURATION
-------------
1. Enable permissions at admin/user/permissions. WARNING: Users who can
   'administer finder PHP settings' will be able to execute PHP code on your
   site.
2. Create finders and their form elements at admin/build/finder.
3. Activate the finder's block or navigate to the finder's path.