$Id: README.txt,v 1.1 2008/06/08 19:43:00 rellis Exp $

= Overview

This is a simple module to implement an "Other" option for taxonomy
select lists. It allows users with the right permissions to create
terms.

Only single-select vocabularies are supported.

New terms aren't created until the associated node is published.

The administrator has the option to show/hide user-added terms
in the select list.

= Use case example

You have a taxonomy vocabulary called 'Organization type' and you
want to allow users to be able to choose '- Other -' and add their
own type. You want to use a simple select list and to keep the
list small to encourage people to use the types you've provided.

= Install

* Install the module, e.g. in ./sites/all/modules/taxonomy_other

* Enable 'Taxonomy Other' in admin/build/modules

* Set permissions for 'add other terms' in admin/user/permissions

* Choose a vocabulary to edit in admin/content/taxonomy

* Change the vocabulary's settings:
   [x] Users can add terms
   [] Show user-added terms

* Go to an edit or create content page for a content type that uses
  your vocabulary; there should now be an '- Other -' option
  in the select list...

