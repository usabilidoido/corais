Userpoints Karma integrates the vote/up down module with the userpoints module
to provide a node/comment rating karma system.

The module allows a voter using the vote/up down widget to add or subtract points
to the author of a given node or comment, thereby awarding or punishing the author
for what they wrote. The the number of points is configurable by the site's admin.
Also, the node types are configurable, so that is shown on nodes or comments of
a certain type, and not others.

The module can notify users when they login with a total of points gained/lost due
to others voting up their nodes or comments.

Installation
------------
In order to install this module, you have to install version 6.x-2.x-dev of the vote
up/down module, and enable the node and comment modules within that module.

After you install the module, you must change the "tag" for vote up/down to match
the tag in karma points. This is necessary only if you have other voting modules
that are active on the site (e.g. fivestar).

Configuration
-------------
The following configuration options are available for this module:

* Users can or cannot change their vote once they have voted.
* There can be a cost incurred by voting. This cost can be positive (user gains points
  by voting) or negative (deducted from voter's userpoints balance). The cost can be
  from a separate category than the main one.
* If it is a cost, the voting widget can be hidden from users who do not have a sufficient
  balance to vote.
* Users can be allowed or disallowed from voting on their own content.

The module imposes the following restriction on the vote up down module that you show be
aware of:

* Voting is limited to logged in users only, since points cannot be accrued by anonymous.

Sites
-----
Sites that use this module:

* Wall Street Oasis (sponsor).

Author
------
Khalid Baheyeldin of http://2bits.com.

The author can also be contacted for paid customizations of this module as well as Drupal consulting,
installation, development, and customizations.
