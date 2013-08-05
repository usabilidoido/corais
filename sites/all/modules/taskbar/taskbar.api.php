<?php
/**
 * @file
 *   Taskbar API documentation
 *
 * == Anatomy of the taskbar
 *
 * A taskbar contains unlimited number of links, ajaxified blocks and activity systems.
 * There 3 types of item are default providers, you can add more providers through
 * the extensible hook system.
 * 
 *
 * == Anatomy of a provider
 *
 * A provider is... (TBC)
 * - ajax: provider has extra data that can be fetched via Ajax
 * - realtime: taskbar status should automatically be refreshed when having an item of this provider
 *
 * == Anatomy of an item
 * An item:
 * - refresh: when displaying extra content, taskbar should automatically refresh it.
 *   This option only available for items whose provider supports Ajax.
 */

/**
 * Hook to register taskbar style.
 *
 * Returns array where each item is a keyed array:
 * - name: style machine name
 * - description:
 * - base: (optional) base style
 * - path: path to {$name}.css
 */
function hook_taskbar_style() {
  return array(
    array(
      'name' => 'my_style',
      'description' => 'My superb taskbar style',
      'base' => 'base',
      'path' => drupal_get_path('module', 'my_module') . '/my_taskbar_style',
    ),
  );
}

/**
 * Hook to register taskbar task provider.
 *
 * - callback: $args[0] is the item, $args[1] is one of
 *   - status: renders an item in the taskbar
 *     returns an array with (content, extra, count, #commands)
 *   - display: render when clicking on the item if ajax is enabled
 *     returns an array with (content, extra, count, #commands)
 *   $args[2] is $full_page = FALSE whether this item is rendered for fullpage display
 *   (so that JS, CSS are added if neccessary).
 *   Detail information about the returned array:
 *   - content: html code to display in the taskbar
 *   - extra: html code to display when the item is active (on click/hover)
 *   - count: number to display in a special format. Do not display if empty (or equals to zero)
 *   - #commands: array of command to execute (using the same syntax as Ctools Ajax responder)
 * - early prepare: A callback run during init phase of site per item using this
 *    provider. Allows provider to provide anything that needs to be done early,
 *    like add JS and CSS files.
 */
function hook_taskbar_provider() {
  return array(
    array(
      'name' => 'my_module_link',
      'description' => 'My superb link provider for Taskbar',
      'ajax' => FALSE,
      'realtime' => FALSE,
      'callback' => 'my_module_taskbar_link',
      'early prepare' => 'my_module_taskbar_early_prepare',
      'file' => 'my_module.taskbar.inc',
      'path' => drupal_get_path('module', 'my_module'),
    ),
  );
}

/**
 * Hook to alter provider
 */
function hook_taskbar_provider_alter(&$providers) {
}

/**
 * Hook to notify item deletion
 */
function hook_taskbar_item_delete($item_name) {
}

/**
 * Hook to define new items.
 *
 * Should contain an array of objects, where each object has the following keys.
 * - name: Machine/key of the item.
 * - title: Administer title of the item.
 * - provider: what provider to use for this item.
 * - region: what area to display it (left, right, center, custom).
 * - status: whether item is disabled or enabled by default.
 * - weight: where to position the item in relevance to others.
 * - settings: provider/item specific additional information.
 */
function hook_taskbar_default_items() {
}

/**
 * Hook to alter in-code defined new items.
 */
function hook_taskbar_default_items_alter($items) {
}

/**
 * Hook to alter items after load.
 */
function hook_taskbar_items_alter(&$items) {
}

/**
 * Hook to alter item content just before render.
 */
function hook_taskbar_item_content_alter(&$content, &$item) {
}

/**
 * \defgroup Polling system
 * @{
 * @function taskbar_polling_update($uid, $item_name)
 *   Send an instant notification on the item $item_name
 *   @param $uid can be an integer or an array of integers
 * @}
 */
