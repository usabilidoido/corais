// $Id: README.txt,v 1.1 2009/07/20 19:49:00 cleverage Exp $

README
--------------------------------------------------------------------------
This module adds to each node a field containing the date when it has been
published.

Without this, the only dates available are the creation date and the
update date.

This new field can be used for example in any templates or in any views.

This module integrates well also with Scheduler
<http://drupal.org/project/scheduler> for example.

This module is written by Clever Age
Website: <http://www.clever-age.org>

INSTALLATION
--------------------------------------------------------------------------
1. Copy the publication_date folder to your modules directory
2. Go to admin > Site building > Modules, and enable this module. 

This will add a new field $node->published_at, containing 'false' if the
node is not published yet, or containing the publication date otherwise.
You can then use this field in any templates.

Moreover, this field is available in the views parameters.

Note about nodes that have been creating BEFORE the installation of this
module: for these nodes, since we can not know the exact date of the
publication, $node->published_at will contain the creation date.
 