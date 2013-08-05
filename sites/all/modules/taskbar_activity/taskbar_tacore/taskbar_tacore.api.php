<?php
/**
 * @file
 *   Taskbar API documentation
 *
 * == Anatomy of a tacore activity $activity
 *
 * $activity->message (string)
 *   untranslated activity message
 * $activity->data (array)
 *   replacement for holders in the message
 * $activity->recipient (int)
 *   activity recipient
 * $activity->type (string)
 *   type of the activity, could be anything ('node insert', 'node update', 'comment reply'...)
 * $activity->uid (int)
 *   id of the primary user related to the activity
 * $activity->entity_id (int)
 *   id of the primary object related to the activity.
 *   If type='comment reply' then entity_id could be the id of the replied comment.
 *   The couple (type, uid, entity_id) is used to determined if an activity is duplicate.
 * $activity->unread (boolean)
 * $activity->created (int)
 * $activity->aid (int)
 *
 */
