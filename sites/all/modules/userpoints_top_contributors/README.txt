The User Points Top Contributors module creates blocks and pages that
list the maximum points earned by contributors.

The module does the following:
 - Creates a block and page that shows the community's Top Contributors.
 - Creates a block and page that shows the community's Top Contributors in a
   defined period (like this week).
 - Adds details links to the Top Contributors list so User Points Admins can
   quickly access a user's point details.
 - Modifies the user profile page to show a user's current point balance and
   their lifetime point balance.
 - Adds links to a user's profile page (for User Points Admins) so they can see
   a user's details or manage their point totals.

To install the module:
 - Copy the module to your /sites/all/modules/contrib directory.
 - Enable the module at: /admin/build/modules
 - Grant users "view userpoints" permissions at: /admin/users/permissions
 - Run cron to populate the top contributors database tables at: /cron.php
 
 Module provided by WebWise Solutions: http://www.webwiseone.com