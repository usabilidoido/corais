$Id: README.txt,v 1.6.2.13 2010/10/02 01:54:39 auzigog Exp $

Introduction
------------
The Meetings module allows for the creation and management of meetings including agendas, notes, invitations, attendance, time, location, and more. This module is useful for any group of people who frequently hold meetings and would like to effectively track the agendas and notes from those meetings.

The Meetings module can be used on any Drupal site, but is particularly useful when combined with other project management tools, such as [Open Atrium](http://openatrium.com/).


Features
--------
  * Adds a meeting content type that includes: agendas, notes, time, location, and invitation (if Team Notifications is installed).
  * Provide views for upcoming and recent meetings.
  * Allow for meeting attendance to be taken and viewed.
  * Permissions to manage each aspect of meetings.


Dependencies
------------
Make sure you have these installed before you install Meetings. <!-- sorted by ascending order of popularity -->

  * [Notifications Team](http://drupal.org/project/notifications_team) <!-- 366 -->
  * [Strongarm](http://drupal.org/project/strongarm) <!-- 175 -->
  * [Features](http://drupal.org/project/features) <!-- 104 -->
  * [Notifications](http://drupal.org/project/notifications) <!-- 98 -->
  * [Messaging](http://drupal.org/project/messaging) <!-- 82 -->
  * [VotingAPI](http://drupal.org/project/votingapi) <!-- 35 -->
  * [jQuery UI](http://drupal.org/project/jquery_ui) <!-- 26 -->
  * [Chaos Tools (CTools)](http://drupal.org/project/ctools) <!-- 23 -->
  * [Date](http://drupal.org/project/date) <!-- 11 -->
  * [FileField](http://drupal.org/project/filefield) <!-- 8 -->
  * [Token](http://drupal.org/project/token) <!-- 4 -->
  * [CCK](http://drupal.org/project/cck) <!-- 3 -->
  * [Views](http://drupal.org/project/views) <!-- 2 -->


Installation
------------
Manual:

  * Download all of the dependencies listed above
  * Download the latest release of the module
  * Copy meetings directory to your modules directory (usually `/sites/all/modules/`)
  * Enable the Meetings feature at `/admin/build/features`
  * Set permissions as appropriate for your site

Using a drush `.make` file:

  * *This method will automatically download the many required dependency modules and install the Meetings module*
  * Install [drush](http://drupal.org/project/drush) and [drush_make](http://drupal.org/project/drush_make)
  * Download the [meetings.make](http://drupalcode.org/viewvc/drupal/contributions/modules/meetings/meetings.make?view=co&pathrev=DRUPAL-6--1) file to your site root
  * From the terminal, navigate to your site root. Example: `cd ~/Sites/drupal`
  * Execute `drush make --no-core meetings.make` from the terminal
  * Enable the Meetings feature at `/admin/build/features`
  * Set permissions as appropriate for your site

*Permissions:* The following permissions are automatically enabled for the `authenticated` role. Be sure to disable these if you do not want all authenticated users to have this level of access.

  * meetings: edit meeting agenda
  * meetings: edit meeting notes
  * meetings: view attendance for meetings
  * features: edit any meetings content
  * features: edit own meeting content
  * notifications_content: subscribe to content
  * notifications_team: subscribe other users


Usage
-----
Create a meeting at `/node/add/meeting` and set the title, date, location and agenda as appropriate. Invite other users to the meeting.

When the meeting is occurring, most users can (by default) edit the node to add notes. If the user has permissions, they can also click the Attendance tab on the meeting to take attendance based on the users who were invited to the meeting.

Multiple views and blocks are provided to display the upcoming and recent meetings.

**Organic Groups:** If you have [Organic Groups](http://drupal.org/project/og) module installed, invitations and attendance will automatically be based on the group associated with that meeting.

**Repeating meetings:** If you install the [Node Repeat](http://drupal.org/project/node_repeat) module, you will automatically be able to schedule repeated meetings.


Testing
-------
I would be vary grateful for anyone who would be willing to test the most recent release or the [development version](http://ftp.drupal.org/files/projects/meetings-6.x-1.x-dev.tar.gz). I haven't got around to writing SimpleTests for this module *yet*. You can help out by trying to view/edit a meeting in the following situations: attendance, with OG enabled, without OG enabled, viewing meetings as authenticated user, unauthenticated user, group member, group owner, node creator, non-node creator.

Please report all successes or problems to the [issue queue](http://drupal.org/project/issues/meetings). 


Development
-----------
This module was initially developed for [Google Summer of Code 2010](http://drupal.org/node/782294) by [auzigog](http://drupal.org/user/542166) (Jeremy Blanchard). Further development is occurring as part of a set of grassroots project management tools being developed at [Activism Labs](http://activismlabs.org).

Development is occurring in the [DRUPAL-6--1 branch](http://drupalcode.org/viewvc/drupal/contributions/modules/meetings/?pathrev=DRUPAL-6--1).

http://activismlabs.org

