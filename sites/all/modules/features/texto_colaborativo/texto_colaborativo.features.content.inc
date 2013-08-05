<?php

/**
 * Implementation of hook_content_default_fields().
 */
function texto_colaborativo_content_default_fields() {
  $fields = array();

  // Exported field: field_etherpad_antigo
  $fields['text-field_etherpad_antigo'] = array(
    'field_name' => 'field_etherpad_antigo',
    'type_name' => 'text',
    'display_settings' => array(
      'weight' => '103',
      'parent' => '',
      'label' => array(
        'format' => 'inline',
      ),
      'teaser' => array(
        'format' => 'default',
        'exclude' => 0,
      ),
      'full' => array(
        'format' => 'default',
        'exclude' => 0,
      ),
      '5' => array(
        'format' => 'default',
        'exclude' => 0,
      ),
      '4' => array(
        'format' => 'default',
        'exclude' => 0,
      ),
      '2' => array(
        'format' => 'default',
        'exclude' => 0,
      ),
      '3' => array(
        'format' => 'default',
        'exclude' => 0,
      ),
      'token' => array(
        'format' => 'default',
        'exclude' => 0,
      ),
    ),
    'widget_active' => '1',
    'type' => 'link',
    'required' => '0',
    'multiple' => '0',
    'module' => 'link',
    'active' => '1',
    'attributes' => array(
      'target' => 'user',
      'rel' => '',
      'class' => '',
      'title' => '',
    ),
    'display' => array(
      'url_cutoff' => '80',
    ),
    'url' => 0,
    'title' => 'value',
    'title_value' => 'Histórico de alterações antes da migração para o sistema novo no dia 04/03/2012',
    'enable_tokens' => 0,
    'validate_url' => 1,
    'widget' => array(
      'default_value' => array(
        '0' => array(
          'url' => '',
        ),
      ),
      'default_value_php' => NULL,
      'label' => 'Etherpad Antigo',
      'weight' => '103',
      'description' => 'Link para o Etherpad antigo. Para versões do texto antes de 04/03/2012',
      'type' => 'link',
      'module' => 'link',
    ),
  );

  // Exported field: field_etherpad_novo
  $fields['text-field_etherpad_novo'] = array(
    'field_name' => 'field_etherpad_novo',
    'type_name' => 'text',
    'display_settings' => array(
      'weight' => '102',
      'parent' => '',
      'label' => array(
        'format' => 'hidden',
      ),
      'teaser' => array(
        'format' => 'hidden',
        'exclude' => 0,
      ),
      'full' => array(
        'format' => 'default',
        'exclude' => 0,
      ),
      '5' => array(
        'format' => 'default',
        'exclude' => 0,
      ),
      '4' => array(
        'format' => 'default',
        'exclude' => 0,
      ),
      '2' => array(
        'format' => 'default',
        'exclude' => 0,
      ),
      '3' => array(
        'format' => 'default',
        'exclude' => 0,
      ),
      'token' => array(
        'format' => 'default',
        'exclude' => 0,
      ),
    ),
    'widget_active' => '1',
    'type' => 'etherpad',
    'required' => '0',
    'multiple' => '0',
    'module' => 'etherpad',
    'active' => '1',
    'attributes' => array(
      'class' => '',
      'width' => '100%',
      'height' => '700',
    ),
    'etherpad_url' => 'http://etherpad.usabilidoido.com.br:8080/p/',
    'widget' => array(
      'default_value' => array(
        '0' => array(
          'etherpad_url' => 'http://etherpad.faberludens.com.br:8080/p/3e1cc224b8d26ccabd23525f08ab9af46bb3a3d0',
          'etherpad_text' => 'default value for now until we have a function to generate one',
          'attributes' => 'a:3:{s:5:"class";s:0:"";s:5:"width";s:4:"100%";s:6:"height";s:3:"700";}',
          '_error_element' => 'value',
        ),
      ),
      'default_value_php' => NULL,
      'label' => 'Etherpad Novo',
      'weight' => '102',
      'description' => 'Este aqui é a nova ferramenta de textos colaborativos hospedada pelo Instituto Faber-Ludens. ',
      'type' => 'etherpad',
      'module' => 'etherpad',
    ),
  );

  // Translatables
  // Included for use with string extractors like potx.
  t('Etherpad Antigo');
  t('Etherpad Novo');

  return $fields;
}
