<?php

/**
 * @file
 * Explanation of the Node import hooks.
 */

/**
 * List the available content types.
 *
 * Use this hook to return a list of available types of content.
 * You are not limited to returning node content types, but you
 * can return any type of content you want.
 *
 * @return
 *   Array ($type => $info).
 *
 * @par Content type
 *
 * The key of the returned array is used as internal content type
 * ($type) and will be passed to all other hooks. For example "user",
 * "node:story", ...
 *
 * This last example shows how supported/node.inc handles different
 * content types.
 *
 * \par Content information
 *
 * The value of the returned array is an array with a number of
 * keys that describes how the content type can be created:
 *
 * - \b "title" : Human readable name (required).
 *
 * - \b "can_create" : Either a boolean (TRUE or FALSE) or a
 *   callback function that checks whether the user can create
 *   the specific type of content.
 *
 * - \b "create" : Callback function that creates the object.
 *
 * You may set other data for this content type as well, for example
 * supported/taxonomy.inc sets a "vocabulary" key to avoid having to
 * lookup the vocabulary a term belongs to.
 *
 * \par can_create callback
 *
 * @code
 * $can_create = function callback($type);
 * @endcode
 *
 * The callback should return TRUE if the current user can create
 * given content type and FALSE otherwise.
 *
 * \par create callback
 *
 * @code
 * $output = function callback($type, $values, $preview);
 * @endcode
 *
 * In case of $preview == TRUE, $output should be a HTML string that shows
 * a preview of the content the user is trying to import.
 *
 * In case of $preview == FALSE, $output should be an object id of the
 * created object. For example the nid, uid, vid, ...
 *
 * @see node_node_import_types(),
 *      user_node_import_types(),
 *      taxonomy_node_import_types().
 *
 * @ingroup node_import_hooks
 */
function hook_node_import_types() {
}

/**
 * Change the list of available content types.
 *
 * @param $types
 *   Array of content types as returned by hook_node_import_types().
 *
 * @return
 *   Just like with any Drupal drupal_alter() hook, you need to change the
 *   array passed to this function directly and not return anything.
 *
 * @ingroup node_import_hooks
 */
function hook_node_import_types_alter(&$types) {
}

/**
 * List the available fields for given content type.
 *
 * Use this hook to list the content fields that are available for
 * given content type.
 *
 * @param $type
 *   String. The content type as returned as a key by
 *   hook_node_import_types().
 *
 * @return
 *   Array of ($fieldname => $fieldinfo).
 *
 * \par Field name
 *
 * $name is a unique identifier for the field. Normally you would
 * use the same string as the form element.
 *
 * \par Field information
 *
 * $info is an array describing how the field should be handled by
 * Node import. There are a lot of options, and probably you will
 * not need them all. The more you can specify here, the less you
 * have to do in hook_node_import_values_alter().
 *
 * It can have following keys:
 *
 * - \b "title" : Human readable name (required).
 *
 * - \b "tips" : A list of short informative tips for the format
 *   of this field, similar to hook_filter_tips(). The different
 *   tips will be displayed as an itemized list.
 *
 *   Defaults to array().
 *
 * - \b "group" : Human readable group name. May be used to group
 *   fields together such as "Publishing options" or "Revision
 *   information".
 *
 *   Defaults to ''.
 *
 * - \b "module" : String. Module that added the field.
 *
 *   Defaults to ''.
 *
 * - \b "weight" : Integer. Weight of the form elements.
 *
 *   Defaults to 0.
 *
 * - \b "is_mappable" : Boolean. Whether the field can be mapped by
 *   the current user. Rather set this to FALSE then omitting the
 *   field altogether.
 *
 *   Defaults to TRUE.
 *
 * - \b "map_required" : Boolean. Whether the field is required.
 *   For example the "title" field in supported/node.inc needs
 *   to be mapped for each row.
 *
 *   Defaults to FALSE.
 *
 * - \b "input_format" : String. Modules may set this to a string
 *   describing the expected format for the input. Examples
 *   are 'user' (a uid), 'date', 'boolean', ... Other modules
 *   may add preprocessing functions based on this to the
 *   field definition.
 *
 *   Defaults to '' (empty string).
 *
 * - \b "has_multiple" : Boolean. Whether the field can have
 *   multiple values. For example the multiple select
 *   vocabularies in supported/taxonomy.inc.
 *
 *   Node import will allow multiple file columns to be mapped to
 *   this field in this case and will present the user with a
 *   "Multiple values are separated by" option.
 *
 *   Defaults to FALSE.
 *
 * - \b "multiple_separator" : String. There are two ways to
 *   specify multiple values for a field (think taxonomy terms or
 *   any CCK field that has multiple values):
 *
 *   -# either the user maps more than one file column to the same
 *      field,
 *   -# or he maps one file column and wants to split that with a
 *      separator.
 *
 *   By default Node import will present the user with an option
 *   to set the separator, but you may set a suggested separator
 *   here (such as "," for free tagging vocabularies instead of
 *   the default "||").
 *
 *   Defaults to variable_get('node_import:multiple_separator', '||').
 *
 * - \b "is_checkboxes" : Boolean. If "has_multiple" is set
 *   to TRUE, this boolean says how to interpret the multiple
 *   values.
 *
 *   If TRUE, the array of multiple values will be converted to
 *   something a checkboxes form element understands:
 *   ($idx => $value) => ($value => 1). If FALSE this conversion is
 *   not made.
 *
 *   Defaults to FALSE.
 *
 * - \b "has_hierarchy" : Boolean. Whether the field value is a
 *   hierarchy. Typical examples can be found in taxonomy or
 *   menu hierarchies.
 *
 *   Defaults to FALSE.
 *
 * - \b "hierarchy_separator" : String. There are two ways to specify
 *   hierarchy values for a field:
 *
 *   -# either the user maps more than one file column to the same
 *      field,
 *
 *   -# or he maps one file column wants to split that with a
 *      separator.
 *
 *   By default Node import will present the user with an option
 *   to set the separator, but you may set a suggested separator
 *   here (such as "/" for path aliases instead of the
 *   default ">>").
 *
 *   Defaults to variable_get('node_import:hierarchy_separator', '>>').
 *
 * - \b "hierarchy_reverse" : Boolean. If a hierarchical value is
 *   mapped to more than one file column (option 1 above), normally
 *   the order of hierarchy is "parent >> child" which means that
 *   the "parent" needs to be mapped to an earlier file column
 *   than the "child", eg:
 *     @code
 *     parent, child
 *     @endcode
 *   If this boolean is set to TRUE then this order is reversed
 *   and so a file like:
 *     @code
 *     child, parent
 *     @endcode
 *   will still be mapped to a "parent >> child" hierarchy.
 *
 *   Node import will allow the user to set this on the options
 *   page.
 *
 *   Defaults to FALSE.
 *
 * - \b "allowed_values" : Array of ($key => $title) of the
 *   allowed values for the field. A mapped value from the file
 *   can either be $key or $title. If the mapped value is in
 *   the list, $key will be assigned as value for the field.
 *
 *   If set, the node_import_check_values() preprocessor will
 *   be automatically added to the preprocessor list.
 *
 *   Defaults to array().
 *
 * - \b "allow_empty" : Boolean. Whether an empty value ('') is
 *   allowed for this field. If the mapped value is empty,
 *   normally node_import will not consider this as a valid
 *   value, but will rather use the default value in that
 *   case. If set to TRUE, a empty mapped value ('') will be
 *   used and not the default value.
 *
 *   Defaults to FALSE.
 *
 * - \b "preprocess" : Array of callback functions. Each of the functions
 *   can preprocess the mapped value. This is a way to validate and
 *   preprocess the input. These preprocess functions can
 *   be used in companion with the hook_node_import_values_alter().
 *
 *   The signature of the callback is:
 *     @code
 *       $return = $function(&$value, $field, $options, $preview);
 *     @endcode
 *   and it should alter the $value passed. Note that if the field
 *   "has_hierachy", the value passed will be an array (grandparent,
 *   parent, child).
 *
 *   The $return value should be FALSE if there is an input error,
 *   NULL if there was not, but not a valid value could be found or
 *   TRUE if a valid value could be found and so other preprocess
 *   functions can be skipped.
 *
 *   See @ref node_import_preprocess for examples.
 *
 *   Defaults to array().
 *
 * \par Multiple and hierarchical fields
 *
 * When a field is both "has_multiple" and "has_hierarchy" the
 * user can map multiple columns to the field, but if he does, the
 * has_multiple will take precedence.
 *
 * If the user maps the field to exactly one column, the expected
 * format is:
 *   @code
 *   "parent1 >> value1 || parent2 >> value2"
 *   @endcode
 *
 * If the user maps the field to more than one column, it is expected
 * that each column contains a value (which can be hierarchical), so:
 *   @code
 *   "parent1 >> value1" , "parent2 >> value 2"
 *   @endcode
 *
 * @ingroup node_import_hooks
 */
function hook_node_import_fields($type) {
}

/**
 * Change the list of available fields for given content type.
 *
 * A typical example for use of this function is in
 * supported/auto_nodetitle.inc where the 'title' field that is
 * supplied in supported/node.inc is altered to change whether it
 * is required and what the default value for it is.
 *
 * @param $fields
 *   Array of fields as returned by hook_node_import_fields().
 *
 * @param $type
 *   String. The content type as returned as a key by
 *   hook_node_import_types().
 *
 * @return
 *   Just like with any Drupal drupal_alter() hook, you need to change the
 *   array passed to this function directly and not return anything.
 *
 * @ingroup node_import_hooks
 */
function hook_node_import_fields_alter(&$fields, $type) {
}

/**
 * List the FAPI elements to set the default values for each field.
 *
 * Use this hook to allow users to set the default value for each
 * of the content fields. You can also use this hook to set some
 * static default values for each object by returning a form
 * element like:
 * @code
 *   $form['some field'] = array(
 *     '#type' => 'value',
 *     '#value' => 'some value',
 *   );
 * @endcode
 *
 * See hook_node_import_values() for an alternative way to set
 * static (or dynamic) default values.
 *
 * @param $type
 *   String. The content type as returned as a key by
 *   hook_node_import_types().
 *
 * @param $defaults
 *   Array of the currently filled in values for the defaults page.
 *
 * @param $fields
 *   Array of fields available for this type.
 *
 * @param $map
 *   Array of how the file columns are mapped to the fields.
 *
 * @return
 *   Array of form elements.
 *
 * @ingroup node_import_hooks
 */
function hook_node_import_defaults($type, $defaults, $fields, $map) {
}

/**
 * Change the FAPI elements to set the default values for each field.
 *
 * @param $form
 *   The form array to change.
 *
 * @param $type
 *   String. The content type as returned as a key by
 *   hook_node_import_types().
 *
 * @param $defaults
 *   Array of the currently filled in values for the defaults page.
 *
 * @param $fields
 *   Array of fields available for this type.
 *
 * @param $map
 *   Array of how the file columns are mapped to the fields.
 *
 * @return
 *   Just like with any Drupal drupal_alter() hook, you need to change the
 *   array passed to this function directly and not return anything.
 *
 * @ingroup node_import_hooks
 */
function hook_node_import_defaults_alter(&$form, $type, $defaults, $fields, $map) {
}

/**
 * List the FAPI elements to set the options for each field.
 *
 * Use this hook to allow users to set additional options for each
 * of the content fields.
 *
 * Some field options such as multiple separator and hierachical
 * separator are added automatically.
 *
 * @param $type
 *   String. The content type as returned as a key by
 *   hook_node_import_types().
 *
 * @param $options
 *   Array of the currently filled in values for the options page.
 *
 * @param $fields
 *   Array of fields available for this type.
 *
 * @param $map
 *   Array of how the file columns are mapped to the fields.
 *
 * @return
 *   Array of form elements.
 *
 * @ingroup node_import_hooks
 */
function hook_node_import_options($type, $options, $fields, $map) {
}

/**
 * Change the FAPI elements to set the options for each field.
 *
 * @param $form
 *   The form array to change.
 *
 * @param $type
 *   String. The content type as returned as a key by
 *   hook_node_import_types().
 *
 * @param $options
 *   Array of the currently filled in values for the options page.
 *
 * @param $fields
 *   Array of fields available for this type.
 *
 * @param $map
 *   Array of how the file columns are mapped to the fields.
 *
 * @return
 *   Just like with any Drupal drupal_alter() hook, you need to change the
 *   array passed to this function directly and not return anything.
 *
 * @ingroup node_import_hooks
 */
function hook_node_import_options_alter(&$form, $type, $options, $fields, $map) {
}

/**
 * List the (static) values to use to create the content type.
 *
 * @param $type
 *   String. The content type as returned as a key by
 *   hook_node_import_types().
 *
 * @param $defaults
 *   Array of the currently filled in values for the defaults page.
 *
 * @param $options
 *   Array of the currently filled in values for the options page.
 *
 * @param $fields
 *   Array of fields available for this type.
 *
 * @param $preview
 *   Boolean. TRUE if we will create a preview with these values,
 *   FALSE if we will create the node with these values.
 *
 * @return
 *   Array of values to submit to the form.
 *
 * @ingroup node_import_hooks
 */
function hook_node_import_values($type, $defaults, $options, $fields, $preview) {
}

/**
 * Change the list of values to use to create the content type.
 *
 * @param $values
 *   Array of values as will be passed to the create function. This
 *   does not only contain the values of hook_node_import_values()
 *   but the already mapped values as well.
 *
 * @param $type
 *   String. The content type as returned as a key by
 *   hook_node_import_types().
 *
 * @param $options
 *   Array of filled in options.
 *
 * @param $preview
 *   Boolean.
 *
 * @return
 *   Just like with any Drupal _alter() hook, you need to change the
 *   array passed to this function.
 *
 * @ingroup node_import_hooks
 */
function hook_node_import_values_alter(&$values, $type, $defaults, $options, $fields, $preview) {
}

/**
 * This hook is invoked after the form to create the $type has been
 * submitted. You can use this hook to do stuff after the node has
 * (or has not) been created, eg use the nid to store some additional
 * information in the db tables.
 *
 * @param $type
 *   String. The content type as returned as a key by
 *   hook_node_import_types().
 *
 * @param $values
 *   Array of the submitted values.
 *
 * @param $options
 *   Array of filled in options.
 *
 * @param $preview
 *   Boolean.
 *
 * @return
 *   Nothing.
 *
 * @ingroup node_import_hooks
 */
function hook_node_import_postprocess($type, $values, $options, $preview) {
}

/**
 * This hook is invoked when a task is loaded, inserted, deleted,
 * resumed or suspended.
 *
 * It allows other modules to act on these events.
 *
 * @param $task
 *   Task object or task ID (for 'delete').
 *
 * @param $op
 *   Operation. One of: 'load', 'insert', 'delete', 'continue', 'pause'.
 *
 * @return
 *   Nothing.
 *
 * @ingroup node_import_hooks
 */
function hook_node_import_task($task, $op) {
}

/**
 * This hook allows users to change some of the options the user is
 * presented with on the wizard form.
 *
 * @param $op
 *   String.
 *
 *   - \b 'record separators'
 *   - \b 'field separators'
 *   - \b 'text delimiters'
 *   - \b 'escape characters'
 *     Extend the options presented on the "Set file options" page.
 *
 *   - \b 'file formats'
 *     Extend the available file formats.
 *
 *   - \b 'date input formats'
 *     Builtin date formats.
 *
 * @return
 *   For all $ops except 'file formats' you need to return an array
 *   ($key => $title).
 *
 *   For the 'file formats' $op you need to return:
 *   ($key => $info) where $key is a unique key to identify the predefined
 *   file format and $info is an array with following keys:
 *
 *   - \b 'title' : Human readable form. Required.
 *
 *   - \b 'record separator' : The $char to use as record separator.
 *
 *   - \b 'field separator' : The $char to use as field separator.
 *
 *   - \b 'text delimiter' : The $char to use as text delimiter.
 *
 *   - \b 'escape character' : The $char to use as escape character.
 *
 * @ingroup node_import_hooks
 */
function hook_node_import_format_options($op) {
}

/**
 * Change the format options.
 *
 * @param $formats
 *   Return value of hook_node_import_format_options().
 *
 * @param $op
 *   See hook_node_import_format_options().
 *
 * @return
 *   Just like with any Drupal _alter() hook, you need to change the
 *   array passed to this function.
 *
 * @ingroup node_import_hooks
 */
function hook_node_import_format_options_alter(&$formats, $op) {
}

