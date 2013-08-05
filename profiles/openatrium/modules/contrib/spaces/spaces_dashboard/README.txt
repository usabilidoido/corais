
Spaces Dashboard 3.x
--------------------

The Spaces Dashboard module provides a "Dashboard" feature that can be used in
space type. The "Dashboard" is intended to be a page, or set of pages, where
space administrator have the ability to modify the content and arrangement.

Content is made available to the Dashboard using the core blocks system. Not all
block will show up as options for users, the dashboard must be told what blocks
it should offer. The dashboard feature offers a settings page in each space
where block availability can be configured. 

By default all dashboards will inherit the dashboard settings of the dashboard
in the so-called 'site' space. To target block availability settings at the
correct space type its important to understand how a particular spaces inherits
it's settings. A particular spaces has three source of a individual settings,
what has been set for the particular space, the setting of that space's preset,
the site wide setting. To make certain blocks available to spaces of a certain
type you must add this setting to a spaces preset. If neither the space itself
nor the preset includes a setting for which dashboard blocks are available the
site setting will be used.

Note: Spaces settings inheritance & spaces presets are covered in detail in the
main spaces module README.txt
