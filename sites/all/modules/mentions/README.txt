// $Id: README.txt,v 1.7 2009/07/23 05:44:01 deciphered Exp $

The Mentions module offers Twitter like functionality, recording all references
to a user's username - using the [@username] or [@#uid] filter format - from
various locations, providing a centralized page to track all mentions.

Mentions was written and is maintained by Stuart Clark (deciphered).
- http://stuar.tc/lark


Features
-------------------

* Tracks Mentions from supported modules:
  * Drupal core Node module.
  * Drupal core Comment module.
  * Facebook-style Statuses (Microblog) module.
    * http://drupal.org/project/facebook_status
* An Input filter to convert [@username] or [@#uid] to the user's profile.
* Customizable input ([@username], [@#uid]) and output (@username) patterns,
  including support for the Token module.
* Views integration, including a default page view for tracking mentions.
  * http://[www.yoursite.com/path/to/drupal]/user/[#uid]/mentions


Recommended Modules
-------------------

* Token - http://drupal.org/project/token
* Views - http://drupal.org/project/views


Usage/Configuration
-------------------

Once installed, the Mentions filter needs to be enabled on your desired Input
formats, this can be done via the Input formats page.
* http://[www.yoursite.com/path/to/drupal]/admin/settings/filters

Custimization settings for input and output patterns are available on the
Input formats 'Configure' page of any input type enabled to use the Mention
filter.
* http://[www.yoursite.com/path/to/drupal]/admin/settings/filters/[id]/configure

Note: Customization of input and output patterns are global for all Input
formats.
