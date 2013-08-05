$Id: README.txt,v 1.1 2010/11/26 10:01:29 siliconmeadow Exp $
Empty paragraph killer - sometimes users are overzealous with the
carriage return

-- SUMMARY --

Empty paragraph killer is a filter module, helpful on sites which use
WYSIWYG editors.

People often hit the return key twice at the end of a paragraph. Most,
if not all site layouts manage the paragraph spacing, so the extra empty
paragraphs created can detract from the look and feel of a site. This
module filters out the empty paragraphs of all user-entered data on a
site. It does so by following the fundamental Drupal way -
non-destructively.

-- REQUIREMENTS --

A WYSIWYG editor. If you are using Drupal without one, the "Line break
converter" in core is sufficient enough and you will achieve little or
no benefit in using this module.

-- INSTALLATION --

* Install as usual, see http://drupal.org/node/70151 for further
information.

-- CONFIGURATION --

* Go to your input formats (/admin/settings/filters)
* Click the configure link next to the desired input format
* Click the checkbox next to 'Empty paragraph filter'.

You may also need to increase the weight of this filter so that it is
triggered after the HTML filter, Line break converter and HTML corrector
if these filters are present (under the 'Rearrange' tab).

-- CONTACT --

Created & maintained by:
* Richard Sheppard (siliconmeadow) - http://drupal.org/user/55284
  
Sponsored by:
* Aroq Ltd - an online publisher providing authoritative and timely
  global business information for busy executives via four dedicated
  industry specific websites: http://www.just-auto.com,
  http://www.just-style.com, http://www.just-food.com &
  http://www.just-drinks.com, all of which are in the process of being
  converted to Drupal.
