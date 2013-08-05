<?php
/**
 * @file
 *   Taskbar API documentation
 *
 * Activity provider.
 */

/**
 * Hook to register a source of taskbar activity.
 *
 * Returns an array where each element is a keyed array:
 * - name: source machine name
 * - description:
 * - callbacks:
 *   - count: counts the number of activities. Arguments:
 *     - $unread: boolean, count only unread activities. Default: FALSE
 *   - list: gets recent activities. Argument:
 *     - $count: number of activities to get. Default: 5
 *     - $start: start from. Default: 0
 *   - mark: mark all activities as read
 * - file: (optional)
 */
function hook_taskbar_activity_source() {
  return array(
    array(
      'name' => 'my_source',
      'description' => 'My source name',
      'callbacks' => array(
        'count' => 'my_module_taskbar_activity_count',
        'list' => 'my_module_taskbar_activity_list',
        'mark' => 'my_module_taskbar_activity_mark',
      ),
      'file' => 'my_module.taskbar.inc',
      'path' => drupal_get_path('module', 'my_module'),
    ),
  );
}
