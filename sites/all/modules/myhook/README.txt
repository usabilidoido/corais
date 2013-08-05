// $Id: README.txt,v 1.1 2010/08/23 01:24:21 ufku Exp $

MyHook
http://drupal.org/project/myhook
====================================
Allows site administrators define custom hooks.


CONFIGURING AND USING
---------------------
1. Go to admin/settings/myhook and define your custom hooks as if your module's name is "myhook".
Ex:

/**
 * Implementation of hook_nodeapi().
 */
function myhook_nodeapi(&$node, $op, $a3 = NULL, $a4 = NULL) {
  switch ($op) {
    case 'presave':
      if ($node->nid && $node->moderate) {
        // Reset votes when node is updated:
        $node->score = 0;
        $node->users = '';
        $node->votes = 0;
      }
      break;
    case 'insert':
    case 'update':
      if ($node->moderate && user_access('access submission queue')) {
        drupal_set_message(t('The post is queued for approval'));
      }
      elseif ($node->moderate) {
        drupal_set_message(t('The post is queued for approval. The editors will decide whether it should be published.'));
      }
      break;
    case 'view':
      $node->content['my_additional_field'] = array(
        '#value' => theme('mymodule_my_additional_field', $additional_field),
        '#weight' => 10,
      );
      break;
  }
}


More hooks at http://api.drupal.org/api/group/hooks/6
