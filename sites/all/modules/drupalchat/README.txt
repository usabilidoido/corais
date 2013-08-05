
DrupalChat provides one on one chat. It provides a static sleek chat bar at 
the bottom of the web browser. There is a selectable list of Online Users like
in Facebook chat. Once you choose a particular user to chat with, it creates a
new tab in the chat bar along with an attached pop-up chat window.

REQUIREMENTS
============
- This module is written for Drupal 6.x.
- The 6.x-2.0 alpha branch of jQuery Update module (jquery_update).

INSTALLATION
============

1. Download the module from Drupal.org/project/drupalchat and save it to your
   modules folder.
2. Enable the DrupalChat module at admin/build/modules.
3. Set the access drupalchat permissions for authenticated users at
   admin/user/permissions.

INTEGRATION WITH USER RELATIONSHIPS
===================================
1. Enable the DrupalChat UR Integration module.
2. Go to the admin/settings/drupalchat and fill in the required details after
   configuring up the User Relationships module.

ADMINISTER THE DRUPALCHAT MODULE
=================================
Go to admin/settings/drupalchat. The long polling technique can be activated at
this page instead of default Normal AJAX technique.
   