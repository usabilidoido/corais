<?php

/**
 * Dummy example course object.
 */
class CourseObjectTest extends CourseObject {

  public static function getMaxOccurences() {
    return 3;
  }

}

/**
 * Dummy example course object.
 */
class CourseObjectGradedTest extends CourseObject {

  function isGraded() {
    return TRUE;
  }

}
