; $Id: README.txt,v 1.1.2.1 2008/12/14 18:19:49 binarybill Exp $

About
-------------------------
This module creates a CCK field that allows the user to input a GMap Macro.  It then outputs the translated Google Map.  If there are location(s), this module will also add the markers to the map.  The user can optionally choose the marker style.

Project page:
http://drupal.org/project/gmapfield


Dependencies
-------------------------
Drupal Modules:
* CCK
* GMap
* Location (suggested)


Install
-------------------------
Install the module just like any other:
http://drupal.org/node/70151


Use
-------------------------
Just like any other CCK module:
http://drupal.org/project/cck


Theming
-------------------------
Function to display window in the markers that are added to map:
theme_gmapfield_markerwindow($location = NULL, $node = NULL) 


TO DO
-------------------------
See project page:
http://drupal.org/project/gmapfield