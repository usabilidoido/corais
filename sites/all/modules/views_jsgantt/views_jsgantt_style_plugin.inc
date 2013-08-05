<?php
// $Id: views_jsgantt_style_plugin.inc,v 1.4.2.6 2010/10/07 15:48:54 danepowell Exp $

/**
 * @file
 */

class views_jsgantt_style_plugin extends views_plugin_style {
  /**
   * Set default options
   */
  function option_definition() {
    $options = parent::option_definition();
    $options['tasktitle'] = array('default' => '');
    $options['startdate'] = array('default' => '');
    $options['enddate'] = array('default' => '');
    $options['parentid'] = array('default' => '');
    $options['parenttitle'] = array('default' => '');
    $options['progress'] = array('default' => '');
    $options['resource'] = array('resource' => '');
    $options['colorsource'] = array('default' => '');
    $options['colorby'] = array('default' => '');
    $options['colorlist'] = array('default' => '');
    $options['dependson'] = array('default' => '');
    $options['format'] = array('default' => '');
    return $options;
  }

  /**
   * Provide form
   */
  function options_form(&$form, &$form_state) {
    $optional = array('' => t('<None>'));
    $options = $this->display->handler->get_field_labels();
    $optional += $options;
    $form['tasktitle'] = array(
      '#title' => t('Task title field'),
      '#description' => t('Pick the field that defines task titles.'),
      '#type' => 'select',
      '#options' => $options,
      '#default_value' => $this->options['tasktitle'],
    );
    $form['startdate'] = array(
      '#title' => t('Start date field'),
      '#description' => t('Pick the date field that defines start dates.'),
      '#type' => 'select',
      '#options' => $options,
      '#default_value' => $this->options['startdate'],
    );
    $form['enddate'] = array(
      '#title' => t('End date field'),
      '#description' => t('Pick the field that defines end dates. Note that if this field is empty, the task will appear as a milestone.'),
      '#type' => 'select',
      '#options' => $options,
      '#default_value' => $this->options['enddate'],
    );
    $form['progress'] = array(
      '#title' => t('Progress field'),
      '#description' => t('Pick the field that defines progress percentage.'),
      '#type' => 'select',
      '#options' => $optional,
      '#default_value' => $this->options['progress'],
    );
    $form['resource'] = array(
      '#title' => t('Caption field'),            
      '#description' => t('Pick the field that defines the caption for a task.'),
      '#type' => 'select',
      '#options' => $optional,
      '#default_value' => $this->options['resource'],
    );
    $form['parentid'] = array(
      '#title' => t('Parent ID field'),
      '#description' => t('Pick the field that defines the nid of parents of tasks.'),
      '#type' => 'select',
      '#options' => $optional,
      '#default_value' => $this->options['parentid'],
    );
    $form['parenttitle'] = array(
      '#title' => t('Parent title field'),
      '#description' => t('Pick the field that defines the title of parents of tasks.'),
      '#type' => 'select',
      '#options' => $optional,
      '#default_value' => $this->options['parenttitle'],
    );
    $form['dependson'] = array(
      '#title' => t('Depends on ID field'),
      '#description' => t('Pick the field that defines the nid of task dependencies.'),
      '#type' => 'select',
      '#options' => $optional,
      '#default_value' => $this->options['dependson'],
    );
    $form['colorsource'] = array(
      '#type' => 'select',
      '#title' => t('Color Source'),
      '#options' => array(
        'progress' => t('Color based on relative progress (orange = overdue, blue = on-schedule)'),
        'fields' => t('Color based on a field'),
      ),
      '#default_value' => $this->options['colorsource'],
      '#multiple' => FALSE,
    );
    $form['colorby'] = array(
      '#title' => t('Color by field'),
      '#description' => t('Pick the field that defines the colors of tasks.'),
      '#type' => 'select',
      '#options' => $optional,
      '#default_value' => $this->options['colorby'],
      '#process' => array('views_process_dependency'),
      '#dependency' => array('edit-style-options-colorsource' => array('fields')),
    );
    $form['colorlist'] = array(
      '#title' => t('Color list'),
      '#description' => t('An array of field values and corresponding HTML colors. Enter one value per line, in the format <em>key</em>|<em>color</em>. If the "Color by" field is a select list, then each <em>key</em> must be an option key, mostly likely an integer. <em>color</em> must be an HTML color code in the form <em>33ffab</em>'),
      '#type' => 'textarea',
      '#default_value' => $this->options['colorlist'],
      '#process' => array('views_process_dependency'),
      '#dependency' => array('edit-style-options-colorsource' => array('fields')),
    );
    $form['format'] = array(
      '#title' => t('Default display format'),
      '#description' => t('Pick default display format.'),
      '#type' => 'select',
      '#options' => array('day' => 'Day', 'week' => 'Week', 'month' => 'Month', 'quarter' => 'Quarter'),
      '#default_value' => $this->options['format'],
    );
  }

  function render() {
    if (isset($this->view->live_preview) && $this->view->live_preview) {
      return t('jsGantt not compatible with live preview.');
    }
    $this->options['tasktitle'] = $this->view->display_handler->get_handler('field', $this->options['tasktitle'])->field_alias;
    $this->options['startdate'] = $this->view->display_handler->get_handler('field', $this->options['startdate'])->field_alias;
    $this->options['enddate'] = $this->view->display_handler->get_handler('field', $this->options['enddate'])->field_alias;
    $this->options['progress'] = $this->view->display_handler->get_handler('field', $this->options['progress'])->field_alias;
    $this->options['parentid'] = $this->view->display_handler->get_handler('field', $this->options['parentid'])->field_alias;
    $this->options['parenttitle'] = $this->view->display_handler->get_handler('field', $this->options['parenttitle'])->field_alias;
    $this->options['colorby'] = $this->view->display_handler->get_handler('field', $this->options['colorby'])->field_alias;
    $this->options['dependson'] = $this->view->display_handler->get_handler('field', $this->options['dependson'])->field;
    return theme($this->theme_functions(), $this->view, $this->options);
  }
}
