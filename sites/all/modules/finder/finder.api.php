<?php

/**
 * @file
 * Documents finder's hooks for api reference.
 */

/**
 * Act on finders.
 *
 * @param &$object
 *   The finder or finder element.
 * @param $op
 *   The operation, indicates where/when this is being invoked.  Possible values:
 *   - "finder_load": The Finder has been loaded from the database, modules can
 *     make changes to the Finder here.
 *   - "finder_presave": The Finder is about to be inserted or updated into the
 *     database.
 *   - "finder_insert": The Finder has been created in the database.
 *   - "finder_update": The Finder has been changed in the database.
 *   - "finder_delete": The Finder is being deleted.
 *   - "finder_admin_edit": An admin is about to load the edit page for the
 *     Finder.
 *   - "finder_admin_delete": An admin is about to load the delete page for the
 *     Finder.
 *   - "finder_element_load": The Finder Element has been loaded from the
 *     database, modules can make changes to the Element here.
 *   - "finder_element_presave": The Finder Element is about to be inserted or
 *     updated into the database.
 *   - "finder_element_insert": The Finder Element has been created in the
 *     database.
 *   - "finder_element_update": The Finder Element has been changed in the
 *     database.
 *   - "finder_element_delete": The Finder Element is being deleted.
 *   - "finder_admin_element_edit": An admin is about to load the edit page for
 *     the Finder Element.
 *   - "finder_admin_element_delete": An admin is about to load the delete page
 *     for the Finder Element.
 *   - "finder_page": The Finder page is being displayed.
 *   - "finder_results": The Finder results are being displayed.
 *   - "finder_block": The Finder block is being displayed.
 *   - "finder_view": The Finder is being displayed.
 *   - "finder_find": The Finder is preparing a query.
 *   - "finder_form": The Finder is preparing a Finder form.
 *   - "finder_admin_list": The Finder is being listed in the admin table.
 *   - "finder_export": The Finder is about to be exported.
 *   - "finder_clone": The Finder is being cloned, such as during an import.
 * @param $a3
 *   - For "finder_results", passes in the $form_state.
 *   - For "finder_find", passes in the $mode string which can be 'choices'
 *     (for radio/checkbox/select/autocomplete options) or 'results' (for a
 *     submitted search).
 *   - For "finder_form", passes in the $form array.
 *   - For "finder_admin_edit", "finder_admin_delete",
 *     "finder_admin_element_edit", and "finder_admin_element_delete", passes
 *     in the $form_state.
 *   - For "finder_view" passes in the $display param from finder_view().
 * @param $a4
 *   - For "finder_find", passes in the $finder_element_id useful when $mode
 *     is 'choices'.
 *   - For "finder_form", passes in the $form_state array.
 *   - For "finder_admin_element_edit" and "finder_admin_element_delete",
 *     passes in the $finder.
 * @return
 *   The returned value of the invoked hooks:
 *   - Currently no operations support a return value.
 */
function hook_finderapi(&$object, $op, $a3 = NULL, $a4 = NULL) {
  // no example code
}

/**
 * Enable new kinds of element types.
 *
 * @return
 *  An array, where the keys are the IDs of the element type, and the values
 *  are an array where the keys are parameters '#title' and '#module', where
 *  the value of '#title' is a translatable string and the value of '#module'
 *  is the string name of your module.
 * @see finder_optionwidgets_finder_element_handlers()
 */
function hook_finder_element_handlers() {
  // no example code
}

/**
 * Build the form element for a finder element.
 *
 * This function will be called in the module specified in #module for a
 * particular element in hook_finder_element_handlers().
 *
 * @param $element
 *   The finder element object.
 * @param &$form_element
 *   The Forms API form element.
 * @see finder_optionwidgets_finder_element()
 */
function hook_finder_element($element, &$form_element) {
  // no example code
}

/**
 * Enables new types of findable Drupal objects.
 *
 * @return
 *   An array, where the keys are the IDs of the base objects, and the values
 *   are an array where the keys are parameters '#title' and '#module', where
 *   the value of '#title' is a translatable string and the value of '#module'
 *   is the string name of your module.
 * @see finder_node_finder_base_handlers()
 */
function hook_finder_base_handlers() {
  // no example code
}

/**
 * Fetch an array of choices or results.
 *
 * This function will be called in the base handler module, it must return an
 * array of results. See finder_node_finder_find() and
 * finder_views_finder_find() for two different approaches to this function.
 * The former uses Finder's internal query engine as well as a helper function
 * it shares with Finder User, the latter loads a View and makes some
 * programmatic changes to fetch the results.
 *
 * @param $finder
 *   The finder object.
 * @param $finder_element_id
 *   If $mode is 'choices', this is the finder element id to get choices for.
 * @param $keywords
 *   An array keyed by finder_element_id, where the values are any
 *   str/num/bool/null or an array of such values to be OR'd together.
 * @param $mode
 *   'choices' or 'results' depending on what we are fetching.
 * @param $match
 *   The match method, see finder_match_operator().
 * @param $pager
 *   Used to limit choices or results per page.
 * @return
 *   An array of choices/results.
 * @see finder_node_finder_find()
 * @see finder_node_finder_views()
 */
function hook_finder_find($finder, $finder_element_id, $keywords, $mode, $match, $pager) {
  // no example code
}

/**
 * Generate finder results output.
 *
 * @param $finder
 *   The finder object.
 * @return
 *   Themed output of finder results.
 * @see finder_node_finder_result()
 */
function hook_finder_result($finder, $keywords, $result_array, $form_state) {
  // no example code
}

/**
 * Perform alterations before a finder form is rendered.
 *
 * This is a good place to add custom FAPI values for #validate, #submit, and
 * #theme.
 *
 * @see hook_form_FORM_ID_alter()
 */
function hook_form_finder_form_alter(&$form, &$form_state) {
  // no example code
}

/**
 * Alter the form on the finder administration page that lets you add a new finder.
 *
 * This is usually not necessary.
 *
 * @see hook_form_FORM_ID_alter()
 */
function hook_form_finder_admin_add_form_alter(&$form, &$form_state) {
  // no example code
}

/**
 * Alter the form to configure a finder.
 *
 * Useful for base handler modules to add additional options and validation.
 * Any fields added under $form['settings'] will be automatically saved by
 * Finder and later accessible at $finder->settings.
 *
 * @param $finder
 *   The finder object.
 *
 * @see hook_form_FORM_ID_alter()
 */
function hook_form_finder_admin_edit_alter(&$form, &$form_state, $finder) {
  // no example code
}

/**
 * Alter the finder delete confirmation form.
 *
 * This is usually not necessary.
 *
 * @param $finder
 *   The finder object.
 *
 * @see hook_form_FORM_ID_alter()
 */
function hook_form_finder_admin_delete_alter(&$form, &$form_state, $finder) {
  // no example code
}

/**
 * Alter the form to configure a finder element.
 *
 * Useful for base and element handler modules to add additional options and
 * validation. Any fields added under $form['settings'] will be automatically
 * saved by Finder and later accessible at $element->settings.
 *
 * @param $finder
 *   The finder object.
 * @param $finder_element_id
 *   The finder element ID.
 *
 * @see hook_form_FORM_ID_alter()
 */
function hook_form_finder_admin_element_edit_alter(&$form, &$form_state, $finder, $finder_element_id) {
  // no example code
}

/**
 * Alter the form to configure a finder element delete confirmation form.
 *
 * This is usually not necessary.
 *
 * @param $finder
 *   The finder object.
 * @param $finder_element_id
 *   The finder element ID.
 *
 * @see hook_form_FORM_ID_alter()
 */
function hook_form_finder_admin_element_delete_alter(&$form, &$form_state, $finder, $finder_element_id) {
  // no example code
}

/**
 * Alter the keywords array used in finder_find().
 *
 * @param &$keywords
 *   An array keyed by finder_element_id, where the values are any
 *   str/num/bool/null or an array of such values to be OR'd together.
 * @param $finder
 *   The finder object.
 * @param $finder_element_id
 *   If $mode is 'choices', this is the finder element id to get choices for.
 * @param $mode
 *   'choices' or 'results' depending on what we are fetching.
 * @param $match
 *   The match method, see finder_match_operator().
 * @param $pager
 *   Used to limit choices or results per page.
 */
function hook_finder_find_keywords_alter(&$keywords, $finder, $finder_element_id, $mode, $match, $pager) {
  // no example code
}

/**
 * Alter the choices or results returned by finder_find().
 *
 * @param &$options
 *   The array of choices or results.
 * @param $finder
 *   The finder object.
 * @param $finder_element_id
 *   If $mode is 'choices', this is the finder element id to get choices for.
 * @param &$keywords
 *   An array keyed by finder_element_id, where the values are any
 *   str/num/bool/null or an array of such values to be OR'd together.
 * @param $mode
 *   'choices' or 'results' depending on what we are fetching.
 * @param $match
 *   The match method, see finder_match_operator().
 * @param $pager
 *   Used to limit choices or results per page.
 */
function hook_finder_find_options_alter(&$options, $finder, $finder_element_id, $keywords, $mode, $match, $pager) {
  // no example code
}

/**
 * Alter the query array before finder_query() builds it into SQL.
 *
 * @param &$query
 *   The query array.
 */
function hook_finder_query_alter(&$query) {
  // no example code
}

/**
 * Alter the query array before finder_query() executes the SQL.
 *
 * @param &$query
 *   The query array.
 */
function hook_finder_query_built_alter(&$query) {
  // no example code
}

/**
 * Alter the operators array returned by finder_match_operator().
 *
 * @param &$operators
 *   The operators array.
 */
function hook_finder_match_operators_alter(&$operators) {
  // no example code
}

/**
 * Alter the form state before it is set by finder_form_state().
 *
 * This is a chance to undo any changes made by the finder form submit
 * function. If you set $form_state['storage']['finished'] to FALSE here then
 * it will prevent any automatic redirects and allow you to make multistep
 * forms.
 *
 * @param &$form_state
 *   The Forms API form state.
 * @param $finder_id
 *   The finder's ID.
 */
function hook_finder_form_state_alter(&$form_state, $finder_id) {
  // no example code
}

/**
 * Alter the result or do a custom redirect before redirecting to first result.
 *
 * @param &$result
 *   The result array.
 * @param $finder
 *   The finder object.
 */
function hook_finder_goto_alter(&$result, $finder) {
  // no example code
}

/**
 * Redirect to the first result.
 *
 * The base handler module is expected to redirect the user based on the
 * $result by implementing this function.
 *
 * @param $finder
 *   The finder object.
 * @param $result
 *   The result array.
 */
function hook_finder_goto($finder, $result) {
  // no example code
}

/**
 * Alter the default output of an export string.
 *
 * @param &$out
 *   The fully assembled default string.
 * @param $tab
 *   The tab variable from finder_export.
 * @param $key
 *   The key variable from finder_export.
 * @param $value
 *   The value variable from finder_export.
 * @param $iteration
 *   The iteration variable from finder_export.
 */
function hook_finder_export_alter(&$out, $tab, $key, $value, $iteration) {
  $out = $tab ."  '". $key ."' => ". finder_export($value, $iteration) .",\n";
}

/**
 * Provide or alter the path to a views result item.
 *
 * @param &$path
 *   A raw path that can be put into url() or l() that can be used to link to
 *   or redirect to the object.  If set to FALSE will prevent redirect.
 * @param $table
 *   Base table for this type of object.
 * @param $id
 *   The value of the primary key for this record.
 *
 * @see finder_views_path().
 * @see finder_views_finder_goto().
 */
function hook_finder_views_path_alter(&$path, $table, $id) {
  // no example code
}