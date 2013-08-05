
Spaces 3.x
----------
Spaces is an API module intended to make configuration options generally
avaliable only at the sitewide level to be configurable and overridden by
individual "spaces" on a Drupal site. It has been described as:

- A way to make one Drupal site act like several sites
- A way to provide much more configurable, full-feature Organic Groups or user
  homepages
- A generalized API for contextual configuration


Requirements & compatibility
---------------------------
Use of spaces requires

- CTools: http://drupal.org/project/ctools
- Features: http://drupal.org/project/features

However, you will want the following modules in order to build or use reasonably
cool features with Spaces and take advantage of most space types:

- PURL: http://drupal.org/project/purl
- Context 3.x: http://drupal.org/project/context
- Strongarm 2.x: http://drupal.org/project/strongarm
- Views 2.x: http://drupal.org/project/views

Out of the box, Spaces provides integration modules for:

- User: `spaces_user`
- Taxonomy: `spaces_taxonomy`
- OG: `spaces_og`


Coming from Spaces 2.x?
-----------------------
The Spaces 3.x branch makes significant departures from many of the concepts
in the 2.x branch. Here is a non-exhaustive list of important changes:

- Removed strict PURL dependency. Spaces can now be made active through means
  other than a PURL provider (see `spaces_user_init()`).
- Usage of CTools plugins API and export API.
- `spaces_customizers()` and `spaces_settings()` have been replaced with a
  general configuration override system using object controllers (more on this
  below).

If you are upgrading from Spaces 2.x, prepare for a rocky ride. Update scripts
are included to migrate as cleanly as possible from 2.x to 3.x but any custom
settings you have created will need to be managed manually. The update scripts
leave the `spaces_settings` table intact for this reason.

Here is a rough list of steps to consider when upgrading from 2.x to 3.x:

1. Backup everything.
2. Run `update.php`.
3. Upgrade your features to Context 3, Strongarm 2, etc.
4. Have you implemented any custom 2.x space types? You need to migrate them,
   see `API.txt` and `spaces.api.php`.
5. Have you implemented any custom 2.x spaces settings? You need to migrate
   them to use standard Drupal variables and `system_settings_form()` (see
   the `features_test` module for an example).
6. Have you implemented any custom 2.x spaces customizers? The concept may not
   transfer cleanly to Spaces 3, or if it does, it will probably make the most
   sense as a custom controller.


Core concept behind Spaces 3
----------------------------
Spaces 3 has been built outwards from the basic idea that it should be possible
for a "space" to override the values of a Drupal object that would otherwise
have a single, sitewide value. For our purposes

  A space is a configuration environment that is triggered or made active by
    some condition. For example, a "user space" might be made active when
    viewing a user's profile. Once that user space is active, any customization
    that user has made override sitewide values.

  An object is a Drupal site building or configuration structure. Examples
    include variables, contexts and views. Not included: nodes, users, taxonomy
    terms, other "content".

Let's first look at storage of overridden values. Spaces stores all of its
overrides in the `spaces_overrides` table. Here is a sample row:

    +------+----+-------------+------------------+------------------------+
    | type | id | object_type | object_id        | value                  |
    +------+----+-------------+------------------+------------------------+
    | og   | 14 | variable    | spaces_preset_og | s:13:"private_group";  |
    +------+----+-------------+------------------+------------------------+

This row describes an overridde when a certain Organic Group (node 14) is
active. In particular, the variable `spaces_preset_og` has the value
`private_group` when this space is active. More generally, `spaces_overrides`
can store any value to override the default of an object, described by
(`object_type`, `object_id`), for any space, described by (`type`, `id`).

In practice, this means that when node 14 is active

    variable_get('spaces_preset_og', NULL);
    // returns "private_group"

While when node 14 is not active

    variable_get('spaces_preset_og', NULL);
    // returns NULL or sitewide value


Controllers & contextuality
---------------------------
The example above shows that when a space is active you need to change some
basic assumptions about how Drupal works. In particular, spaces introduces
contextuality to settings and configuration.

Per-space overrides are handled by controllers. Controllers are CTools plugins
that manage retrieval and storage of overrides for a given object type. For
example, spaces comes with a variable controller and context controller. Each
controller should interface with its object's API at a retrieval point and at
a storage or save point.

    +-------------------------------------+-----------------------------------+
    |Drupal integration point             |Controller method                  |
    +-------------------------------------+-----------------------------------+
    |hook_context_reaction_fetch_alter()  |$space->controllers->context->get()|
    |spaces_form_context_ui_editor_alter()|$space->controllers->context->set()|
    +-------------------------------------+-----------------------------------+

Whenever a context's reaction value is fetched, the context controller's `get()`
method is invoked retrieving a value specific to the active space.

The controller's save integration point is triggered through a custom submit
handler to the context editor form through a `hook_form_alter()`.

Currently, our rule of thumb is that while retrieval may be contextual, actual
save API functions should not be overridden. In general, you should always be
able to retrieve and save the original values of an object in addition to
manipulating space overrides from the API.


Presets, levels of configuration
--------------------------------
Spaces presets are sets of configuration that apply to whole sets of spaces. For
example you may not want to make the same set of customizations for every new
user you add to the site. You can use a preset like "member" or "guest" to
capture a variety of settings and have new users use one of the presets.

With presets in the picture, `variable_get('foo', NULL)` can actually return one
of three possible values when a space is active:

1. `space`: is the override value for the active space. If the active space has
    saved an override for `foo`, this is what you will get.
2. `preset` is the override value for the preset. If the active space has not
    saved a value for `foo` the variable controller will fall back to the preset
    if it has a value for `foo`.
3. `original` is the sitewide, canonical value for `foo`. If neither the space
    nor the preset have an override for `foo`, you will get the sitewide value
    like a call to `variable_get()` when no spaces are active.

This cascading of values applies to all object types with a spaces controller.

**Aside**

This architecture *strongly* implies that it could or should be possible to
stack configuration overrides n levels rather than the current fixed number. In
such a scenario, presets would themselves become stacked spaces, and the picture
would become even simpler:

- Fixed stacking model (where > can be read as "inherits from"):

        space > preset > original

- Arbitrary stacking model (where space 0 is the preset space)

        space n > space n-1 > ... > space 1 > space 0 > original

This model is very attractive but requires some serious study before it can be
realized. Spaces 3 currently implements the fixed stacking model.


Managing and editing presets
----------------------------
The Spaces UI module allow provides the facility to manage presets. This
includes creating new presets, reverting overrides and editing preset metadata.
There is not a single UI for creating and editing all the elements of a
spaces preset. Presets can contain many settings about which the spaces module
actually knows nearly nothing, making it impossible to provide a useful
interface. Presets are meant to be edited through actual instance of a space.

For example, to change the features a preset has enabled you would:

1. Goto, or create, a space which uses the preset you wish to change.
2. Set the space to have the desired set of enabled feature.
3. Goto the "overrides" page (generally rendered as a tab) for the space.
4. Click the checkbox for the `space_menu_items` variable.
5. Click "Save to preset".

At this point any new groups created with this same preset will have the
configuration you've just specified. If the preset you've edited is provided
in code the interface at "admin > site building > spaces" (`admin/build/space`)
will show them as overriden.


Other functionality
-------------------
There is quite a bit of functionality in Spaces that does not fit neatly into
the picture of each space as a "configuration override environemnt." This
functionality has survived to support the users and code implemented around
existing user stories that spaces currently serves. In the future the
functionality may be further abstracted out so that Spaces can play a much more
minimal and possibly flexible role.

1. Features can be set to a state per space (defaults to enabled/disabled, but
overridable by extending classes) that determine their behavior within a space.
In particular, features tend to hide or show menu items, alter access to parts
of the menu tree, etc.
2. Access to a space has several levels - space types can control degrees of
admin access, feature access and basic access to a space.
3. Spaces can determine routing of certain pages - for example, some nodes may
only be viewed when a certain space is active or a certain administrative page
may drop all active spaces.

Note that none of these user stories are necessarily implied by the
configuration override framework introduced above.


Features
--------
Spaces integrates with the Features built using the Features module. Features
that are compatible with spaces must declare themselves as such by including the
`spaces[types]` key in their `.info` file.

For example a "MyBlog" module may include the following snippet in
`myblog.info` to declare that is is spaces-enabled;

    name = "MyBlog"
    package = "Features"
    ...
    spaces[types] = "all"

`spaces[types]` may be declared to be one of the following values:

- `all` indicates that this feature is compatible with all space types.
- an array of space types where this feature may be enabled. Note that you may
  also include the faux space type `site` in this array, indicating that this
  feature may be enabled even and/or possibly only when no spaces are active.

For example, if my feature `my_cool_gallery` should only be available when
outside of any spaces, I would use the following entry:

    spaces[types][] = "site"

If it can be enabled both outside of any spaces and inside of user spaces, but
not group spaces, I could use:

    spaces[types][] = "site"
    spaces[types][] = "user"


Settings
--------
Settings can be defined for spaces features simply as Drupal variables by
implementing a `system_settings_form()` at `features/[my-feature]`. If spaces
finds such a page defined by your feature, it will expose it as a link in
the features form for any of the space types where the feature is enabled.


Creating your own space types or extending existing ones
--------------------------------------------------------
Please see `API.txt` for instructions on how you can create your own space types
or use your own class to extend or replace one of the existing classes.


Maintainers
-----------
- alex_b (Alex Barth)
- jmiccolis (Jeff Miccolis)
- yhahn (Young Hahn)
- Ian Ward
