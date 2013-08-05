// $Id: README.txt,v 1.22.2.3 2010/04/28 02:36:01 jmiccolis Exp $

CONTENTS OF THIS FILE
---------------------

 * Introduction
 * Features
 * Installation
 * Case Tracker URLs
 * Case Tracker Caveats!


INTRODUCTION
------------

Current Maintainer: jmiccolis <http://drupal.org/user/31731>

Previous Maintainer: zero2one <http://drupal.org/user/105066>
Original Sponsor: Digital202
Original Developers: India-based team
Oversight: DaveNotik (http://drupal.org/user/18129/contact), killes, zero2one

This module enables teams to track outstanding cases which need resolution.

This is a rewrite of the project.module and is very similar to that module
but varies in important ways. The project.module is specific to software
development and the need for a more generic issue tracker had been expressed.
As such, the casetracker.module only includes relevant functionality, but
also uses regular Drupal comments and integrates cleanly with Views, Organic
Groups, Mailhander, CCK, and more.


FEATURES
------------

Case Tracker enables teams to track outstanding cases. A case could be a bug
report, a feature request, or a general task. You can also define new case
types. Using Case Tracker, you can set the status of cases and their priority.

Case Tracker lets you have multiple projects and each case is tied to a project. 
The module includes default Project and Case node types. However, you can also
define existing node types to act as Project and Case node types in the
administrative interface. 

Case Tracker includes three modules:
 * Case Tracker: Enables the handling of projects and their cases
 * Case Tracker Basic: Enables a basic project node type for use with Case Tracker
 * Case Tracker Actions: Provides actions for Case Tracker

Case Tracker comes with a default interface that is powered by the Views module,
which can be customized and extended.

Using the administrative interface, you can use Case Tracker to:
 * Assign a user to all new cases by default
 * Assign a default case priority, status, and type to all cases
 * Define existing node types to act as Project and Case node types
 * Define new case states. Case state realms include priority, status, and type

Users can be assigned the following permissions at admin/user/permissions:
 * "administer case tracker"
 * "assign cases"
 * "create projects"
 * "create cases"
 * "edit own projects"
 * "edit own cases"

INSTALLATION
------------

1. Copy the files to your sites/all/modules/ directory.

2. Enable the casetracker module at admin/modules.

3. Assign the project and case node type and other relevant case options at
   admin/settings/casetracker. Case Tracker ships with simplistic "Project"
   and "Case" types in its casetracker_basic.module; although you can use
   these, you will get stronger flexibility by assigning it to a
   content type of your own creation, or an Organic Group.

4. Customize case types, priorities, and states at admin/casetracker.

5. Enable permissions in admin/access.

Note: for more project.module-like functionality, try installing the
comment_upload.module and enabling comment attachments for case nodes.


CASE TRACKER URLS
-----------------

The project based URLs we provide are briefly described below. These displays
are completely powered by the Views module and can be completely disabled or
reworked for completely different displays.

  /casetracker/
    Displays a list of all cases.

  /casetracker/my
    Displays a list of cases assigned to the current user.

  /casetracker/projects/
    Displays a list of all projects.


CASE TRACKER CAVEATS!
---------------------

Some common gotchas which are, at the moment, "by design":

 * The "Last modified" value of Case Tracker cases is determined by the
   timestamp of the last comment attached to them (or, in the absence of
   a single comment, the node creation time). This requires that the
   comment.module (and node_comment_statistics table) are enabled and
   created. We CAN think of some use cases for not requiring comments on
   a case, but we think them edge cases and not enough to cater to. If
   you feel otherwise, don't hesitate to voice your opinion.

 * Case Tracker does not provide any access control to nodes or fields.
   There are other fine modules which provide varying kinds of access control
   which can be used with Case Tracker.


