<?php

/**
 * @file
 * Hooks provided by Spaces Dashboard.
 */

/**
 * Alter the default dashboard access settings for a given block.
 *
 * @param &$access
 *   An associative array of block `bid`, ie. "{$module}-{$delta}", to access
 *   boolean value. Blocks with a TRUE value are accessible, blocks with a
 *   FALSE value are inaccessible.
 * @return
 *   The return value is unused. Your hook implementation should alter the
 *   $access parameter directly.
 */
function hook_spaces_dashboard_access_alter(&$access) {
  // Don't allow access to any blocks from the menu module on the dashboard.
  if ($block->module === 'menu') {
    $access = FALSE;
  }
}
