$Id: README.txt,v 1.1.2.5 2010/02/05 20:15:06 yhahn Exp $

Persistent URL for Drupal 6.x


Installation
------------

PURL can be installed like any other Drupal module -- place it in the
`sites/all/modules` (or a site or profile specific module directory) directory
for your site and enable it on the `admin/build/modules` page.

PURL is an API module. It does absolutely nothing for the end user out of the
box without other modules that take advantage of its API.


Core concepts behind PURL
-------------------------

The mission of PURL is to provide an API for other modules to manipulate and
respond to portions of an HTTP request that are not captured by the core
Drupal menu system. By default, the Drupal menu system reacts to only one part
of a request: $_GET['q']. PURL aims to be a pluggable system for reacting to
parts of $GET['q'] but many others as well.

Other than the "normal" drupal path, here are some parts of a request that a
PURL provider may respond to:

  Mozilla    http:// foobar.baz.com / group-a / node/5 ? foo=bar
    |                   |               |         |         |
    |                   |               |         |         |
  User agent     Subdomain/Domain     Prefix      |    Query string
                                                  |
                                        ("Normal" drupal path)

Any modules using the PURL API must define one or more providers.

- A provider is a single concept for responding to or modifying a request.
  Examples: `spaces_og` activates group contexts, `views_modes` sets an active
  views style plugin, `locale` (if it were to use PURL) would activate the
  active language.

- A method is the means by which a provider is activated and modifies a
  request. The parts of the request, like user agent, prefix, query string,
  etc. are all exapmles of methods.

- A modifier is an id/value pair designated by a provider to trigger a response
  if found in the provider's method. Often modifiers map string aliases to an
  id, like ['mygroup', 5] (where 'mygroup' is the group's path and 5 is the
  group's node id). Other times, there is no reasonable mapping and a provider
  will want the literal value found in the request. These modifiers simply use
  the same string for the id and value, e.g. ['mozilla', 'mozilla']. 

One of PURLs goals is to make it possible for providers to be written to be
independent of the method that it uses. For example, `spaces_og` can activate
a group space when it finds a group identifier as a path prefix,
or a subdomain, or a specified query string, etc. depending on the method that
has been assigned to it by PURL.

The big picture is that PURL allows administrators to assign each provider a
method, and any time a valid modifier is found in a request for that given
method the provider is given a chance to respond via a callback function.

Example provider/method setup:

+---------------+--------------------------------+----------------------+
| Provider      | Method                         | Example modifier     |
+---------------+--------------------------------+----------------------+
| spaces_og     | Path prefix                    | ['mygroup', 5]       |
| views_modes   | Query string, key: 'viewstyle' | ['list', 'list']     |
| iphone_custom | Subdomain                      | ['iphone', 'iphone'] |
+---------------+--------------------------------+----------------------+

A sample URL which would trigger *all* of the providers above:

  http://iphone.foobar.com/mygroup/page-listing?viewstyle=list

**Responding**

When a modifier for a provider is found in a request, the provider's registered
callback is called with the ID for the given modifier. For example, in the
example above, the callback for provider `spaces_og` will be passed `5`, the id
corresponding to the `mygroup`, and it is then the provider's job to do whatever
it wants to do with that information. `spaces_og`, for example, will load node
`5` and set it as the active group context.

Depending on the method (e.g. any that involve $_GET['q']), PURL may remove the
modifier for the rest of the page load so that the request is passed cleanly to
the rest of the Drupal menu stack. While the original request above would have
had the path `mygroup/page-listing`, PURL will strip `mygroup` leaving the rest
of Drupal to think that the page's path is `page-listing`.

**Modifying**

Depending on the PURL method, outgoing requests may be automatically rewritten
to sustain the modifier found in the incoming request. In the example above,
any paths pushed through `url()` will be given the additional path prefix of
`mygroup`. Thus all links on the page and even requests like form autocomplete
AJAX calls will be prefixed.


API usage: General
------------------

These instructions are for general usage of the PURL API.

Since the scope of PURL goes beyond $_GET['q'], PURL provides several
additional options to the `$options` array passed to `url()` and its derivate
`l()`. These options can also be passed to `purl_goto()`, a wrapper around
`drupal_goto()` that allows an `$options` array to be passed (which
`drupal_goto()` does not allow out of the box).

PURL extends the `$options` array in four ways:

1. If `$options['purl']['disabled']` is true none of the detected and removed
   path modifications will be re-instated. This allows you to drop all PURL
   modifications. Example:

   // On a page with PURL modifiers like `mygroup/node/43?viewstyle=list`,
   // generate a URL to node/43 that drops all PURL modifiers. The resulting
   // URL will point at just `node/43`.

   l('Foobar', 'node/43', array('purl' => array('disabled' => TRUE)));

2. $options['purl']['remove'] can be set to an array of providers which should
   be dropped, while others are maintained. Setting this when
   $options['purl']['disabled'] is true is redundant. Example:

   // On a page with PURL modifiers like `mygroup/node/43?viewstyle=list`,
   // generate a URL to node/43 that drops only the specified PURL modifier.
   // The resulting URL will point at `node/43?viewstyle=list`.

   l('Foobar', 'node/43', array('purl' => array('remove' => array('spaces_og'))));

3. $options['purl']['provider'] and $options['purl']['id'] can be used
   together to set a specific modification to the link.

   // Generate a URL that includes a specific PURL modifier. Note that this
   // should always be used in favor of generating an absolute URL manually
   // as callers should not assume that a provider is using a specific method.
   // Assuming that the modifier is ['mygroup', 5], The resulting URL will
   // point at `mygroup/node/43`

   l('Foobar', 'node/43', array('purl' => array('provider' => 'spaces_og', 'id' => 5)));

4. $options['purl]['add'] can be used to add a set of id's and provider's to
   the link.

   // Generate a URL that adds one or more PURL modifiers, including any that
   // are active for the current request. On a page with PURL modifiers like
   // `mygroup/node/43`, the following will result in a URL that points at
   // `mygroup/node/43?viewstyle=list`.

   l('Foobar', 'node/43', array('purl' => array('add' => array('provider' => 'views_modes', 'id' => 'list'))));

The `l()` function is used in In all of the examples, but the same options
array can be passed to `url()` or `purl_goto()` to capture the equivalent
behavior. For example:

   // Go to `mygroup/node/43`.

   purl_goto('node/43', array('purl' => array('provider' => 'spaces_og', 'id' => 5)));


API usage: Providers
--------------------

These instructions are for modules that would like to respond to or modify
requests using PURL. First, read `purl.api.php` for documentation on the
various hooks called by PURL.

1. Add an implementation of `hook_purl_provider()` to your module.

2. Add a callback function that will be passed the ID of any modifiers found
   by PURL to your module.

3. If your module expects a static set of modifiers or will store and retrieve
   modifiers using its own storage, implement hook_purl_modifiers() to tell
   PURL about modifiers that are valid for your provider.

   OR

   If you would like to store modifiers in PURL's database table, you may want
   to add the elements provided by `purl_form()` to the form for your node,
   user, term, or other element associated with your modifier. You will need to
   implement basic CRUD for the PURL modifier in your form's submit handler -
   see `purl_save()`.

4. Go to `admin/settings/purl` and choose a method to use with your provider.

5. Make a request to your Drupal site that uses a valid modifier and test.


Maintainers
-----------
yhahn (Young Hahn)
jmiccolis (Jeff Miccolis)


Contributors
------------
Ian Ward
dmitrig01 (Dmitri Gaskin)
