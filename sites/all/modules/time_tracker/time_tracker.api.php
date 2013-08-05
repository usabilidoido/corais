<?php

/**
 * @file
 * Hooks provided by the time_tracker module.
 */

/**
 * @addtogroup hooks
 * @{
 */

/**
 * Access a time entry just before it is saved to the database. The only
 * variable, $time_entry, will be passed by reference and can be altered before
 * the record is saved.
 *
 * @param array $time_entry
 *    An array of time tracker data.
 *      - nid: (required) The nid of the node for this time entry (int)
 *      - type: The type of time entry. Will be one of 'comment', 'node' or 'edit'
 *      - teid: The time entry id if this is an update (int)
 *      - cid: The Comment id if this is attached to a comment. May not be
 *        available if the time entry is node based (also not necessary for node
 *        based time entries) (int)
 *      - uid: The user id of the person this time entry belongs to (int)
 *      - activity: The activity id (int)
 *      - timestamp: (for duration based time entries) The timestamp of the time
 *        entry. For interval based time entries, this will be overwritten with
 *        the 'start' date and time. Must be given as a php timestamp (int)
 *      - duration: (for duration based time entries) The duration of the time
 *        entry. Value must be given in hours. (float)
 *      - start: (for interval based time entries) The start time of the entry.
 *        Value must be given in hours. (float)
 *      - end: (for interval based time entries) end start time of the entry
 *         Value must be given in hours. (float)
 *      - deductions: Time that should be subtracted from the total time. This
 *        field exists to preserve the original durations of time entries.
 *        Value must be given in hours. (float)
 *      - note: (for node based time entries) A note about the work associated
 *        with the time entry (string)
 *      - locked: Should this time entry be locked for editing (boolean)
 *      - billable: Is this time entry billable? (boolean)
 *      - billed: Has this time entry been billed yet? (boolean)
 *   The type of tokens to list (e.g. 'global', 'node', or 'user'). To list all
 *   tokens, use 'all'.
 *
 */
function hook_time_tracker_time_entry_presave(&$time_entry) {

  // Change something before it's saved to the database
  if ($time_entry['type'] == 'comment') {
    $time_entry['note'] = 'comment based time entry';
  }

}


/**
 * Access a time entry just after it is saved to the database.
 *
 * @param array $time_entry
 *    An array of time tracker data.
 *      - nid: (required) The nid of the node for this time entry (int)
 *      - type: The type of time entry. Will be one of 'comment', 'node' or 'edit'
 *      - teid: The time entry id if this is an update (int)
 *      - cid: The Comment id if this is attached to a comment. May not be
 *        available if the time entry is node based (also not necessary for node
 *        based time entries) (int)
 *      - uid: The user id of the person this time entry belongs to (int)
 *      - activity: The activity id (int)
 *      - timestamp: (for duration based time entries) The timestamp of the time
 *        entry. For interval based time entries, this will be overwritten with
 *        the 'start' date and time. Must be given as a php timestamp (int)
 *      - duration: (for duration based time entries) The duration of the time
 *        entry. Value must be given in hours. (float)
 *      - start: (for interval based time entries) The start time of the entry.
 *        Value must be given in hours. (float)
 *      - end: (for interval based time entries) end start time of the entry
 *         Value must be given in hours. (float)
 *      - deductions: Time that should be subtracted from the total time. This
 *        field exists to preserve the original durations of time entries.
 *        Value must be given in hours. (float)
 *      - note: (for node based time entries) A note about the work associated
 *        with the time entry (string)
 *      - locked: Should this time entry be locked for editing (boolean)
 *      - billable: Is this time entry billable? (boolean)
 *      - billed: Has this time entry been billed yet? (boolean)
 *   The type of tokens to list (e.g. 'global', 'node', or 'user'). To list all
 *   tokens, use 'all'.
 *
 */
function hook_time_tracker_time_entry_save($time_entry) {

  // ... do something with the time entry data...

}

/**
 * @} End of "addtogroup hooks".
 */
