
Graphmind

Troubleshooting:
If you have any kind of problems with the module (especially with loading the 
  map) check this list first:
- have the latest module (Graphmind)
- have the latest stable Services module (currently is 6.x-2.2)
- have the latest Amfphp module (currently is 6.x-1.0-beta2)
- have the Amfphp 3rd party library (http://goo.gl/txBp)
- have the following service submodules enabled: node_service, system_service, 
  user_service, file_service, views_service, services_keyauth (they should be 
  enabled on install by default)
- On Services > Settings > General: in the selectlist choose Key authentication, then: ONLY check [Use sessid]
- amfphp gateway is ok: /?q=services/amfphp gives: "amfphp and this gateway are installed correctly..."
- you are the admin user (uid = 1)
- if you are not the admin than you have the following permissions: get own user data, load node data, access content
- clean url is does NOT matter
- have flash player (> 10.0)
- the mindmap node in Drupal is published and you have edit/view access


Howto make Graphmind content types:
-----------------------------------
First of all, this is a hack. It might break after upgrade, and it may not work properly.
To make this work, you need a module which depends on the graphmind_service module.

Step 1: create a node-{$content_type}.tpl.php file with the following content:

    $path = drupal_get_path('module', 'graphmind_service');
    include $path . '/node-graphmind.tpl.php';

Step 2: implement hook_init() with and make sure that it contains the following code:

    if (arg(0) == 'node') {
      $node = node_load((int) arg(1));
      if ($node->type == "{$content_type}") {
        global $user;
        $user->graphmindEditable = node_access('update', $node) ? 1 : 0;
      }
    }

Step 3: implement hook_theme()

    function mymodule_theme() {
      return array(
        'node' => array(
          'arguments' => array('node' => NULL, 'teaser' => FALSE, 'page' => FALSE),
          'template' => 'node',
        ),
      );
    }

Step 4: implement hook_node_info()

    function mymodule_node_info() {
      return array(
        "{$content_type}" => array(
          'name' => t('Content type name'),
          'module' => 'node',
          'description' => t('Content type description'),
          'body_label' => t('FreeMind XML'),
        ),  
      );
    }

Step 5: implement hook_perm() with the usual content type permissions

Step 6: implement hook_nodeapi()

    function mymodule_nodeapi(&$node, $op, $a3 = NULL, $a4 = NULL) {
      if ($node->type == "{$content_type}") {
        $node->type = 'graphmind';
        graphmind_services_nodeapi($node, $op, $a3, $a4);
        $node->type = "{$content_type}";
      }
    }

Step 7: implement hook_form_alter()

    function mymodule_form_alter(&$form, $form_state, $form_id) {
      switch ($form_id) {
        case "{$content_type}_node_form":
          graphmind_service_form_graphmind_node_form_alter($form, $form_state);
          break;
      }
    }
