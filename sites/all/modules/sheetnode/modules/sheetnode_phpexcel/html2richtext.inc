<?php

require_once(variable_get('sheetnode_phpexcel_pdf_renderer_path', '') . '/tcpdf.php');

/**
 * Class to convert an HTML fragment to a fully-parsed array of styles.
 * Uses TCDPF which is included in PHPExcel.
 */
class Sheetnode_PHPExcel_HTML2RichText extends TCPDF {
  var $html;
  public function Sheetnode_PHPExcel_HTML2RichText($html) {
    $this->html = $html;
    $this->setPageUnit('px');
  }
  public function convert($cell) {
    $dom = $this->getHtmlDomArray($this->html);
    if (empty($dom)) return FALSE;
    $rtf = new PHPExcel_RichText();
    $run = $rtf->createTextRun();
    foreach ($dom as $entry) {
      if ($entry['tag']) { // HTML tag
        if (!$entry['opening']) continue;
        // Defer the work to third-party implementations.
        module_invoke_all('sheetnode_phpexcel_html2richtext', $run, $cell, $entry, $this);
      }
      else { // text
        if (empty($entry['value'])) continue;
        $run->setText($entry['value']);
        $run = $rtf->createTextRun();
      }
      drupal_alter('sheetnode_phpexcel_html2richtext', $run, $cell, $entry, $this);
    }
    return $rtf;
  }
}

