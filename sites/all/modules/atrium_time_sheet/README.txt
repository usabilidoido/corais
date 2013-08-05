This is a Feature specifically for Open Atrium and the Time Sheet submodule in the Time Tracker project (drupal.org/project/time_tracker).

To enable, download the files into a folder called atrium_time_sheet (if you clone the repo using git, just rename the Atrium-Time-Sheet folder). Put that folder into sites/all/modules and goto admin/build/modules and enable as you would a regular module.

Once enabled, the Feature provides a spaces-aware menu link for the features menu in Open Atrium (the menu at the top of the page with the icons: dashboard, members, cases etc). The Feature needs to be enabled on a group by group basis by clicking on "Settings" then "Customize Features". Atrium Time Sheet should appear in the list of features. Choose 'enable' from the list and save and the Time Sheet menu item should appear. You can also enable the time sheet for non-group pages (e.g. the "home" page) using the same method. If you want the Feature enabled by default on new Groups, click "Save to Private Group" instead of "Save for [your-group-name]". You can also change the defaults through Customize Features > Overrides.

*** IMPORTANT NOTE ***

This Feature used to be a part of the project "Atrium-Time-Tracker" (https://github.com/fuseinteractive/Atrium-Time-Tracker). It has been moved into it's own project and been made space-aware to allow users to enable the time sheet feature on a group by group basis. For upgrade instructions see the Atrium-Time-Tracker README.txt file.

Keep in mind that after you upgrade, the Time Sheet link will disappear. To get it back, you will need to "Customize Features" for each group (instructions above) that you want the link to be available for.
