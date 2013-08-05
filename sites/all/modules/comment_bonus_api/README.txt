Comment Bonus API
=================

This module enhances the standard Drupal comment module's API by providing
hooks in comment_render(), which allow you to manipulate comment threads in
your own way.

It also provides a better comment-wrapper template with the ability to place
form on top of comments and other useful stuff (like in Drupal 7).

You need this module if:

* You use the Ajax Comments module. This module is a prerequisite.
* You wish to use some of the more advanced custom rendering of comments. This
is excellent for additional theme enhancements with more control provided by 
comment-wrapper.tpl.php.


Notes:
Themes such as Adaptivetheme, Zen, Genesis, Fusion and others that come with 
their own comment-wrapper.tpl.php file won't work out of the box and need to
use either the template from the module or remove their own
comment-wrapper.tpl.php file or fold in the differences (replace print $content
with the relevant changes).

The base template in the module originates from the Zen implementation, and not
all themes will print $classes. For example, of the above themes only
Adaptivetheme and Zen do, while Fusion prints its $skinr class directly in the
template, so that needs to be accounted for.

Developer Hooks:
hook_post_comment_render() - Invoked at the very bottom of
comment_bonus_api_comment_render(), which you can think of as having a hook at
the tail end of the stock Drupal comment_render() function. This allows for
any additional modifications to the comments prior to a final return.
