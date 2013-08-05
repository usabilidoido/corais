
PluginManager is designed to retrieving a list of all modules and themes 
available at drupal.org. It then allows the administrator to automatically
retrieve and install the modules and themes they desire.

--Thanks--
Greggles and KBahey - Both mentored me (and this project) during Google's Summer of Code 2008.
Jacob Singh - Improving / Rewriting much of the plugin manager for D7 core.
Jabapyth - Co-maintaining the Plugin Manager module.
FrankT - Providing the German Translation.

--Instructions--
This module has no dependencies.  Simply extract it into place (as you would
any other module) and enable it on the modules page.  This should add a new
group named "Plugin Manager" to the admin page.  Within this group are options
which allow you to search for missing dependencies (for other modules), search
for a plugin (module or theme) by name or by category.

When you find a plugin that you would like to install, clicking "Add" adds it
to the queue (a list of changes to be performed.)  No actual installations
are done until the you navigate to the "Install" menu item.

--Changelog--
6.x-1.9 -> 6.x-1.10

#341761 by JoshuaRogers: Update module dependency not in .info file.
#356416 by FrankT: Added German translation.
#363909 by Jabapyth: internationalization problem
#401616 by JoshuaRogers: Remove md5sum checking.
#446692 by Jabapyth: plugin_manager_cache not created correctly and causes later failures.
#450896 by JoshuaRogers: Runs out of memory while parsing XML.
#450938 by Jabapyth: Add dependency checking.
#474428 by Jabapyth: ajax search bug
#573344 by JoshuaRogers: Stopped "Project tags" from being displayed when viewing categories.
#573410 by JoshuaRogers: Add more content to the readme.txt file.
