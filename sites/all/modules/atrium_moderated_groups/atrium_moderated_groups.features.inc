<?php

/**
 * Implementation of hook_ctools_plugin_api().
 */
function atrium_moderated_groups_ctools_plugin_api() {
  list($module, $api) = func_get_args();
  if ($module == "spaces" && $api == "spaces") {
    return array("version" => 3);
  }
}
