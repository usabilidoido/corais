The "Update status advanced settings" module extends the administrative
interface for Drupal core's "Update status" module. It provides
additional configuration options on the "Settings" tab of the
"Available updates" report, located at: admin/reports/updates/settings

  Administer >> Reports >> Available updates >> Settings 

These advanced settings allow site administrators fine-grained control
to ignore certain projects on the available updates report, even when
a newer version is available. Administrators can also choose to ignore
a specific recommended release of a given project.

A more appropriate name for this module might be "Update status
advanced noise filter", since that's really what it does -- it lets
administrators filter out noise in their available updates report so
that they can focus on the signal of important updates they should
know about.

There are many reasons why an administrator might need to ignore a
certain "noisy" project and/or a specific version. For example:

- The administrator has made local modifications to a module, and does
  not wish to be notified about newer releases all the time.

- The maintainer of the module is not very careful, and frequently
  puts out releases that the site administrator does not wish to
  upgrade to.

- The maintainer might have released a newer official version which is
  incompatible with other modules that the site is depending on. For
  example, perhaps the site cannot upgrade to CCK version 5.x-1.5,
  since that breaks compatibility with specific CCK field modules in
  use on the site which the administrator cannot upgrade yet.

- The maintainer of the module might have released a new official
  version with known bugs and the administrator does not wish to upgrade.

If the site administrator chooses to ignore a project or specific
release of a project, there is a text area to optionally enter a note
indicating why. If this note is defined for a project


Send feature requests and bug reports to the issue queue for the
Update status advanced settings module:
http://drupal.org/node/add/project_issue/update_advanced


Written by: Derek Wright ("dww") http://drupal.org/user/46549

Inspired by similar functionality in the "Update status" module for
Drupal version 5.x in the contributions repository, by Earl Miles and
Derek Wright (http://drupal.org/project/update_status).

$Id: README.txt,v 1.2 2007/10/20 22:34:21 dww Exp $
