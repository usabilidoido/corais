<?php

/**
 * Parent class for poll tracking.
 */
class CourseObjectPoll extends CourseObjectNode {

  function getTakeType() {
    return 'content';
  }

  /**
   * Take or view the results of a poll. Wrapper for poll_view().
   */
  function take() {
    poll_view($this->node);
    return drupal_render($this->node->content['body']);
  }

  public function create() {
    $poll = new stdClass;
    $poll->choice[0]['chtext'] = 'Yes';
    $poll->choice[1]['chtext'] = 'No';
    $poll->type = 'poll';
    $poll->title = $this->getTitle();
    $poll->uid = $this->user->uid;
    $poll->active = 1;
    node_save($poll);
    $this->setNode($poll);
  }

  public function getReports() {
    return array(
      'results' => array(
        'title' => 'Results',
      ),
      'all' => array(
        'title' => 'List votes',
      ),
    );
  }

  public function getReport($key) {
    module_load_include('inc', 'poll', 'poll.pages');
    switch ($key) {
      case 'results':
        return array(
          'title' => t('Poll results'),
          'content' => poll_view_results($this->node, NULL, NULL, NULL),
        );
      case 'all':
        return array(
          'title' => 'All votes',
          'content' => poll_votes($this->node),
        );
    }
  }

  /**
   * Remove poll votes for this user.
   */
  function unEnroll() {
    db_query('DELETE FROM {poll_votes} WHERE nid = %d AND uid = %d', $this->node->nid, $this->getCourse()->getUser()->uid);
    db_query("UPDATE {poll_choices} SET chvotes = chvotes - 1 WHERE nid = %d AND chorder = %d", $this->node->nid, $this->node->vote);
  }

  function getNodeTypes() {
    return array('poll');
  }

  function getCloneAbility() {
    return TRUE;
  }

}
