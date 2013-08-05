
Spaces OG
---------

Spaces integration of Organic Groups. Offers simplified Organic Groups privacy
presets and overrides on a per group level.


Installation
------------

- Install module and its dependencies.
- Go to admin/build/spaces and pick the default preset. Disable presets that
  users that can edit a group content type should not be able to pick.
- Go to admin/settings/purl and review settings. Per default Spaces OG assumes
  that a group should be identified by a unique path prefix (Types 'path' is
  enabled and the Modifier type 'path' is selected for Group spaces. If these
  settings do not fit your purposes, please refer to the Purl documentation for
  further information.
- Go to admin/content/node-settings/rebuild and rebuild your node access table
  if you have not rebuilt it after installing OG Access in a previous step.