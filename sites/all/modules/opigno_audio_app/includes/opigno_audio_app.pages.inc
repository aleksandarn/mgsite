<?php

/**
 * @file
 * Defines all page callbacks.
 */

function opigno_audio_app_sort_course_audios_form($form, $form_state, $node) {
  $form['table'] = array(
    '#tree' => TRUE,
  );

  $form['gid'] = array(
    '#type' => 'hidden',
    '#value' => $node->nid,
  );

  $i=0;
  foreach (opigno_audio_app_get_course_audios($node) as $audio_nid) {
    $audio = node_load($audio_nid);
    $form['table']["quiz_{$audio_nid}"]['nid'] = array(
      '#type' => 'hidden',
      '#value' => $audio_nid,
    );
    $form['table']["quiz_{$audio_nid}"]['title'] = array(
      '#markup' => check_plain($audio->title),
    );
    $form['table']["quiz_{$audio_nid}"]['weight'] = array(
      '#type' => 'weight',
      '#delta' => 50,
      '#default_value' => opigno_audio_app_get_course_audio_weight($node->nid, $audio_nid),
      '#attributes' => array('class' => array('opigno-audio-app-sort-course-audios-weight')),
    );
    $i++;
  }

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t("Save order"),
  );

  return $form;
}

function opigno_audio_app_sort_course_audios_form_submit($form, $form_state) {
  foreach ($form_state['values']['table'] as $values) {
    opigno_audio_app_set_course_audio_weight($form_state['values']['gid'], $values['nid'], $values['weight']);
  }
}