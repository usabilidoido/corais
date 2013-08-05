
-- SUMMARY --

OG User Roles allows group administrators of organic groups to grant additional
user roles to individual members of a certain group.

Any additional permissions only apply within the context of this group and not
globally. This means that the additional user roles to grant are determined and
assigned by the requested page; e.g. node/123 belongs to group XYZ for which
the user was granted additional roles. If the user goes to another page that
does not belong to the same group, the additional user roles are no longer
assigned.

Additional user roles can only be granted, not revoked.

Site administrators may also configure a default user role for new group members
or a default user role for new group admins.

For a full description of the module, visit the project page:
  http://drupal.org/project/og_user_roles

To submit bug reports and feature suggestions, or to track changes:
  http://drupal.org/project/issues/og_user_roles


-- REQUIREMENTS --

* Organic Groups and OG Views modules.


-- INSTALLATION --

* Install as usual, see http://drupal.org/node/70151 for further information.


-- CONFIGURATION --

* Configure user permissions in Administer >> User management >> Permissions >>
  og_user_roles module:

  - configure member roles

    Users in roles with the "configure member roles" permission are able to
    assign user roles to group members.

* Configure user permissions in Administer >> User management >> Permissions >>
  og module:

  - administer organic groups

    Users in roles with the "administer organic groups" permission are able to
    configure global OG User Roles settings that apply to all groups.

* Go to Administer >> Organic Groups >> User roles and configure:

  - Assignable roles

    Enable the user roles group admins should be able to grant group members.
    Make sure you granted appropriate permissions to the selected user roles.
    The enabled user roles will be assignable to other users in a group by
    non-site admins.  Thereby you should be conservative in what permissions you
    grant.  E.g. 'create' permissions for a certain content-type or similar.

  - Default role for new group administrators

    Optionally select a user role to automatically assign to new group admins.
    The user role is specific to the groups in which the user is a group
    administrator.  That is, the user will only be granted the role in the group
    he/she is an administrator for.

  - Default role for new group members

    Optionally select a user role to automatically assign to new group members.

  Note that all default roles only apply to new members that join a group and
  not to existing group admins and members.


-- USAGE --

* As a group administrator, go to the group home page.  In the group navigation
  menu block you should see a link titled:

    # subscribers

  That will display the group member list on which you should see the tab
  "Configure roles", which allows group administrators having the "configure
  member roles" permission to assign additional user roles to individual group
  members.


-- NOTES --

* Additionally granted user roles and permissions only take effect within the
  context of a group.  If you want to grant site-wide permissions to a user,
  then you have to grant the regular user role on user/#/edit.


-- CONTACT --

Current maintainers:
* Ron Parker (SomebodySysop) - http://drupal.org/user/61637
* Daniel F. Kudwien (sun) - http://drupal.org/user/54136

Major rewrite into 4.x by Daniel F. Kudwien (sun).

This project has been sponsored by:
* IofC - Initiatives of Change
  Visit http://www.iofc.org for more information.

* Software Creations
  Visit http://www.scbbs.com for more information.

* UNLEASHED MIND
  Specialized in consulting and planning of Drupal powered sites, UNLEASHED
  MIND offers installation, development, theming, customization, and hosting
  to get you started. Visit http://www.unleashedmind.com for more information.

