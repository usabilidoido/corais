
INTRODUCTION
------------

Current maintainer: Ivan Boothe <ivan@rootwork.org>

This module adds SlideShare (http://www.slideshare.net) support to the 
Embedded Media Field module (http://drupal.org/project/emfield), exposing a 
third-party provider to Embedded Video Field sub-module.

SlideShare slides can thus be embedded in any content type with an Embedded 
Video Field by pasting the URL of a SlideShare presentation into that field.

This module provides both a thumbnail and a Flash embed representation of the 
presentations, and provides support for listings via Views module.

REQUIREMENTS
------------

The Embedded Media Field module and its dependency, CCK (content.module) must 
both be installed and enabled, as must the Embedded Media Field sub-module
Embedded Video Field.

You must acquire a free SlideShare API from 
http://www.slideshare.net/applyforapi in order for the embedding to function.

USAGE
-----

After enabling Slideshare module and its requirements (see above):

1. Create or modify a content type that includes an Embedded Video Field.

2. Select SlideShare as a supported provider for that field.

SlideShare presentations may then be embedded by copying a SlideShare 
presentation's URL and pasting it into that field. The presentation will 
automatically be formatted and embedded from the URL.

See INSTALL.txt for a step-by-step guide.

FUTURE DEVELOPMENT
------------------
The Drupal 6 version of this module is integrated with Embedded Media Field,
while development for the Drupal 7 version will integrate it with the D7-only
Media module (http://drupal.org/project/media).

Suggestions for Drupal 7 code and/or patches are fully welcome in the issue 
queue!

SPONSORS
--------

This project has been sponsored by:

* ROOTWORK.ORG - Continuing Drupal 6 development and Drupal 7 development.
  Rootwork develops websites and online strategy for nonprofits and social 
  change. Visit http://rootwork.org for more information.

* NUVOLE.ORG - Original Drupal 5 and Drupal 6 development.
  Nuvole is an international-minded company specialized in online 
  communication through open-source tools. YouthAgora.org supported 
  the original module development.
