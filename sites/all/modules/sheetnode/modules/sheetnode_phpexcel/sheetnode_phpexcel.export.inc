<?php

/**
 * Helper function to export a node using PHPExcel.
 */
function _sheetnode_phpexcel_export($format, $node) {
  require_once(drupal_get_path('module', 'sheetnode') . '/socialcalc.inc');
  if (empty($node)) return drupal_not_found();

  $sheets = array(); 
  if ($node->type == 'sheetnode') {
    $sheets[$node->title] = socialcalc_parse($node->sheetnode['value']);
  }
  $sheetfields = sheetnode_get_sheetfields($node->type);
  if (!empty($sheetfields)) foreach ($sheetfields as $sheetfield) {
    foreach ($node->{$sheetfield['field_name']} as $delta => $item) {
      $sheets[$sheetfield['widget']['label'] .' '. ($delta+1)] = socialcalc_parse($item['value']);
    }
  }
  if (empty($sheets)) return drupal_not_found();

  sheetnode_phpexcel_batch_export($sheets, $node->title, $format);
  /* DEBUGGING
   * Simulate Batch API with sequential processing for easier debugging.
   */
  /*
  $context = array();
  $operations = array();
  foreach ($sheets as $title => $socialcalc) {
    _sheetnode_phpexcel_batch_export_sheet($title, $socialcalc, 'test', 'xls', '', $context);
  }
  return '';
  */
}

/**
 * Batch API callback to export a single sheet to PHPExcel.
 */
function _sheetnode_phpexcel_batch_export_sheet($title, $socialcalc, $filename, $format, $destination, &$context) {
  require_once(variable_get('sheetnode_phpexcel_library_path', '') . '/Classes/PHPExcel.php');

  if (empty($context['results']['workbook'])) {
    $workbook = $context['results']['workbook'] = new PHPExcel();
    $worksheet = $workbook->getActiveSheet();
    $context['results'] += array(
      'filename' => $filename,
      'format' => $format,
      'destination' => $destination,
    );
  }
  else {
    $workbook = $context['results']['workbook'];
    $worksheet = $workbook->createSheet($workbook->getSheetCount());
  }

  _sheetnode_phpexcel_export_sheet($worksheet, $title, $socialcalc);

  $context['message'] = t('Processed sheet %sheet.', array('%sheet' => $worksheet->getTitle()));
}

/**
 * Batch API callback upon export completion.
 */
function _sheetnode_phpexcel_batch_export_finished($success, $results, $operations) {
  require_once(variable_get('sheetnode_phpexcel_library_path', '') . '/Classes/PHPExcel.php');
  @set_time_limit(0);

  if ($success) {
    // Save to temporary file.
    $plugins = sheetnode_phpexcel_get_plugins();
    $plugin = $plugins[$results['format']];
    $writer = PHPExcel_IOFactory::createWriter($results['workbook'], $plugin['php-excel-type']);
    $tempname = tempnam(file_directory_temp(), 'sheetnode_phpexcel_');
    $writer->save($tempname);

    // Save information for download link.
    $_SESSION['sheetnode_phpexcel_download'] = array(
      'tempname' => $tempname,
      'filename' => _sheetnode_phpexcel_sanitize_filename($results['filename']) . '.' . $results['format'],
      'format' => $results['format'],
      'destination' => $results['destination'],
    );
  }
}

/**
 * Template preprocessor for theme('sheetnode_phpexcel_export_ready').
 */
function sheetnode_phpexcel_preprocess_sheetnode_phpexcel_export_ready(&$vars) {
  if (empty($_SESSION['sheetnode_phpexcel_download'])) drupal_not_found();
  $download = $_SESSION['sheetnode_phpexcel_download'];

  $vars['download'] = url('sheetnode/export/download');
  $vars['filename'] = $download['filename'];
  $vars['destination'] = $download['destination'];

  // Set the page to automatically redirect to download file after 3 seconds.
  drupal_set_html_head('<meta http-equiv="refresh" content="3;url=' . check_plain($vars['download']) . '" />');
}

/**
 * Page callback for 'sheetnode/export/download'.
 */
function _sheetnode_phpexcel_export_download() {
  if (empty($_SESSION['sheetnode_phpexcel_download'])) drupal_not_found();
  $download = $_SESSION['sheetnode_phpexcel_download'];

  $plugins = sheetnode_phpexcel_get_plugins();
  $plugin = $plugins[$download['format']];
  $headers = array(
    'Content-Type: ' . $plugin['content-type'],
    'Content-Disposition: attachment; filename="' . $download['filename'] . '"',
    'Cache-Control: max-age=0',
  );
  file_transfer($download['tempname'], $headers);
}
 
/**
 * Helper function to export a single spreadsheet.
 */
function _sheetnode_phpexcel_export_sheet($worksheet, $title, $socialcalc) {
  require_once(drupal_get_path('module', 'sheetnode') . '/socialcalc.inc');
  @set_time_limit(0);

  $sc = $socialcalc['sheet'];
  $workbook = $worksheet->getParent();

  // Title
  try {
    $worksheet->SetTitle($title);
  }
  catch (Exception $e) {
    watchdog('sheetnode_phpexcel', 'Error setting worksheet title to "!title": @error', array('!title' => $title, '@error' => $e->getMessage()), WATCHDOG_WARNING);
  }

  // Names
  if (!empty($sc['names'])) foreach ($sc['names'] as $name => $info) {
    $definition = ltrim($info['definition'], '=');
    @list($sheetname, $reference) = explode('!', $definition, 2);
    if (empty($reference)) {
      $reference = $definition;
      $external = $worksheet;
    }
    else {
      $sheetname = trim($sheetname, '"\'');
      list($value, $title2) = sheetnode_find_sheet($sheetname);
      if ($value) {
        $external = $workbook->getSheetByName($title2);
        if (!$external) {
          $external = $workbook->createSheet($workbook->getSheetCount());
          $socialcalc2 = socialcalc_parse($value);
          _sheetnode_phpexcel_export_sheet($external, $title2, $socialcalc2);
        }
      }
    }
    // TODO: PHPExcel does not support non-range names.
    try {
      $range = PHPExcel_Cell::rangeBoundaries($reference);
    }
    catch (Exception $e) {
      watchdog('sheetnode_phpexcel', 'Could not export name %name with value %definition because non-range names are not yet supported.', array(
        '%name' => $name,
        '%definition' => $definition,
      ));
      continue;
    }
    if (!empty($external)) {
      $workbook->addNamedRange(new PHPExcel_NamedRange($name, $external, $reference));
    }
    else {
      watchdog('sheetnode_phpexcel', 'Could not export name %name with value %definition because %sheetname was not found.', array(
        '%name' => $name,
        '%definition' => $definition,
        '%sheetname' => $sheetname,
      ));
    }
  }

  // Cells
  if ($sc['cells']) foreach ($sc['cells'] as $coord => $c) {
    // Get cached style if any.
    static $styles = array();
    $hash = _sheetnode_phpexcel_export_get_style_hash($c);
    if (empty($styles[$hash])) {
      $styles[$hash] = $style = new PHPExcel_Style();
    }
    else {
      $style = NULL; // mark that we don't need a new style.
    }

    // Value and format
    _sheetnode_phpexcel_export_cell_value_and_format($c, $coord, $sc, $worksheet, $style); 

    // Comment
    if (!empty($c['comment'])) {
      $worksheet->getComment($coord)->getText()->createTextRun($c['comment']);
    }
    
    if (!empty($style)) {
      // Font
      $font = $style->getFont();
      if (!empty($c['font'])) {
        $scf = socialcalc_cellformat_parsefont($c, $sc);
        if (!empty($scf['family'])) $font->setName($scf['family']);
        if (!empty($scf['size'])) $font->setSize(floatval($scf['size']));
        if (!empty($scf['bold'])) $font->setBold(true);
        if (!empty($scf['italic'])) $font->setItalic(true);
      }
      if (!empty($c['color'])) {
        $scc = socialcalc_cellformat_parsecolor($c, $sc, 'color');
        $rgb = sprintf('%02X%02X%02X', $scc['r'], $scc['g'], $scc['b']);
        $font->getColor()->setRGB($rgb);
      }
  
      // Borders
      foreach (array(
        'br' => 'getRight', 
        'bl' => 'getLeft', 
        'bt' => 'getTop', 
        'bb' => 'getBottom',
      ) as $pos => $method) {
        $border = socialcalc_cellformat_parseborder($c, $sc, $pos);
        if (empty($border)) continue;
        $borderobj = $style->getBorders()->$method();
        $thickness = str_replace('px', '', $border['thickness']); // TODO: what about other units?
        if ($thickness > 0 && $thickness < 7) {
          $thickness = 'thin';
        }
        else if ($thickness > 7 && $thickness < 15) {
          $thickness = 'medium';
        }
        else if ($thickness > 15) {
          $thickness = 'thick';
        }
        $borderstyles = array(
          'thin' => array(
            'solid' => PHPExcel_Style_Border::BORDER_THIN,
            'dashed' => PHPExcel_Style_Border::BORDER_DASHED,
            'dotted' => PHPExcel_Style_Border::BORDER_DOTTED,
            'double' => PHPExcel_Style_Border::BORDER_DOUBLE,
          ),
          'medium' => array(
            'solid' => PHPExcel_Style_Border::BORDER_MEDIUM,
            'dashed' => PHPExcel_Style_Border::BORDER_MEDIUMDASHED,
          ),
          'thick' => array(
            'solid' => PHPExcel_Style_Border::BORDER_THICK,
          ),
        );
        // TODO: what about other combinations?
        $borderstyle = isset($borderstyles[$thickness][$border['style']]) ? $borderstyles[$thickness][$border['style']] : PHPExcel_Style_Border::BORDER_THIN;
        $borderobj->setBorderStyle($borderstyle);
        $scc = $border['color'];
        $rgb = sprintf('%02X%02X%02X', $scc['r'], $scc['g'], $scc['b']);
        $borderobj->getColor()->setRGB($rgb);
      }

      // Background color
      if (!empty($c['bgcolor'])) {
        $scc = socialcalc_cellformat_parsecolor($c, $sc, 'bgcolor');
        $rgb = sprintf('%02X%02X%02X', $scc['r'], $scc['g'], $scc['b']);
        $style->getFill()->setFillType(PHPExcel_Style_Fill::FILL_SOLID);
        $style->getFill()->getStartColor()->setRGB($rgb);
      }

      // Alignment
      if (!empty($c['cellformat'])) {
        $alignments = array(
          'left' => PHPExcel_Style_Alignment::HORIZONTAL_LEFT,
          'right' => PHPExcel_Style_Alignment::HORIZONTAL_RIGHT,
          'center' => PHPExcel_Style_Alignment::HORIZONTAL_CENTER,
        );
        $alignment = isset($alignments[$sc['cellformats'][$c['cellformat']]]) ? $alignments[$sc['cellformats'][$c['cellformat']]] : NULL;
        if ($alignment) {
          $style->getAlignment()->setHorizontal($alignment);
        }
      }

      // Vertical Alignment
      $layout = socialcalc_cellformat_parselayout($c, $sc);
      if (!empty($layout['alignvert'])) {
        $valignments = array(
          'top' => PHPExcel_Style_Alignment::VERTICAL_TOP,
          'bottom' => PHPExcel_Style_Alignment::VERTICAL_BOTTOM,
          'middle' => PHPExcel_Style_Alignment::VERTICAL_CENTER,
        );
        $valignment = isset($valignments[$layout['alignvert']]) ? $valignments[$layout['alignvert']] : NULL;
        if ($valignment) {
          $style->getAlignment()->setVertical($valignment);
        }
      }
    }
    $worksheet->duplicateStyle($styles[$hash], $coord);
 
    // Merged regions
    if (@$c['colspan'] > 1 || @$c['rowspan'] > 1) {
      $coord2 = socialcalc_cr_to_coord($c['pos'][0]+max(@$c['colspan']-1, 0), $c['pos'][1]+max(@$c['rowspan']-1, 0));
      $worksheet->mergeCells($coord .':'. $coord2);
    }
  }

  // Columns
  $font = $worksheet->getDefaultStyle()->getFont();
  if (!empty($sc['colattribs']['width'])) foreach($sc['colattribs']['width'] as $c => $width) {
    $worksheet->getColumnDimension(PHPExcel_Cell::stringFromColumnIndex($c-1))->setWidth(PHPExcel_Shared_Drawing::pixelsToCellDimension($width, $font));
  }

  // Rows
  if (!empty($sc['rowattribs']['height'])) foreach($sc['rowattribs']['height'] as $r => $height) {
    $worksheet->getRowDimension($r)->setRowHeight($height);
  }

  // Test formulas and replace with calculated value upon failure.
  foreach ($worksheet->getCellCollection() as $cellID) {
    $cell = $worksheet->getCell($cellID);
    if ($cell->getDatatype() == PHPExcel_Cell_DataType::TYPE_FORMULA) {
      try {
        $cell->getCalculatedValue(); // don't care about return value
      }
      catch (Exception $e) {
        $coord = $cell->getCoordinate();
        $cell->setValue($sc['cells'][$coord]['datavalue']);
        $comment = t(variable_get('sheetnode_phpexcel_bad_formula', 'Could not export formula !formula.'), array('!formula' => $sc['cells'][$coord]['formula']));
        $worksheet->getComment($coord)->getText()->createTextRun($comment);
      }
    }
  }

  // Reset style cache.
  unset($styles);
}

/**
 * Helper function to get a style's hash.
 */
function _sheetnode_phpexcel_export_get_style_hash($c) {
  $styles = array();
  foreach (array('font', 'color', 'bgcolor', 'cellformat', 'alignvert', 'bt', 'bb', 'bl', 'br', 'nontextvalueformat') as $attrib) {
    if (!empty($c[$attrib])) {
      $styles[] = $attrib . ':' . $c[$attrib];
    }
  }
  return md5(implode(',', $styles));
}

/**
 * Helper function to export a cell.
 */
function _sheetnode_phpexcel_export_cell_value_and_format($c, $coord, $sc, $worksheet, $style) {
  // Taken from socialcalc-3.js:SocialCalc.FormatValueForDisplay().
  $cell = $worksheet->getCell($coord);

  $valuetype = empty($c['valuetype']) ? NULL : substr($c['valuetype'], 0, 1);
  $valuesubtype = empty($c['valuetype']) ? NULL : substr($c['valuetype'], 1);
  $displayvalue = @$c['datavalue'];
  $valueformat = NULL;
  
  // Error.
  if (!empty($c['errors']) || $valuetype == 'e') {
    $displayvalue = !empty($c['errors']) ? $c['errors'] : (!empty($valuesubtype) ? $valuesubtype : t('Unknown error'));
    $cell->setValueExplicit($displayvalue, PHPExcel_Cell_DataType::TYPE_ERROR);
  }
  // Text.
  else if ($valuetype == 't') {
    // Get formatting.
    $valueformat = !empty($c['textvalueformat']) ? $sc['valueformats'][$c['textvalueformat']] : (
                   !empty($sc['defaulttextvalueformat']) ? $sc['valueformats'][$sc['defaulttextvalueformat']] : NULL);
    if (in_array(strtolower($valueformat), array('auto', 'none', 'general'))) {
      $valueformat = NULL;
    }

    // Default text formatting based on cell type.
    if (empty($valueformat)) switch ($valuesubtype) {
    case 'h':
      $valueformat = 'text-html';
      break;
    case 'w':
    case 'r':
      $valueformat = 'text-wiki';
      break;
    case 'l':
      $valueformat = 'text-link';
      break;
    default:
      $valueformat = 'text-plain';
      break;
    }

    // Set the value.
    if ($c['datatype'] == 'f') {
      if (strpos($c['formula'], '!') !== FALSE) {
        _sheetnode_phpexcel_export_reference($c, $cell, $worksheet);
      }
      else {
        $cell->setValue('='. $c['formula']);
      }
    }
    else switch ($valueformat) {
    case 'text-plain':
      $cell->setValueExplicit($displayvalue, PHPExcel_Cell_DataType::TYPE_STRING);
      break;
    case 'text-html':
      $html2rtf = new Sheetnode_PHPExcel_HTML2RichText($displayvalue);
      $rtf = $html2rtf->convert($cell);
      if ($rtf) {
        $cell->setValue($rtf);
      }
      else {
        $stripped = preg_replace('/^\s+|\s+$/', '', strip_tags($displayvalue));
        $cell->setValueExplicit($stripped, PHPExcel_Cell_DataType::TYPE_STRING);
      }
      break;
    case 'text-url':
    case 'text-link':
      $matches = array();
      if (preg_match('/^(.*)<(.*)>$/', $displayvalue, $matches)) {
        $text = trim($matches[1], "\r\n\t \"");
        $url = $matches[2];
      }
      else {
        $url = $displayvalue;
        $parse_url = parse_url($url);
        $text = $parse_url['host'] . $parse_url['path'];
      }
      $cell->setValueExplicit($text, PHPExcel_Cell_DataType::TYPE_STRING);
      $cell->getHyperlink()->setUrl($url);
      break;
    case 'text-wiki':
      // TODO
      break;
    case 'text-image':
      if (function_exists('curl_init')) {
        // Download the file using cURL.
        $ch = curl_init($displayvalue);
        $filename = tempnam(file_directory_temp(), 'sheetnode_phpexcel_');
        $fp = fopen($filename, 'wb');
        $options = array(CURLOPT_FILE => $fp,
                         CURLOPT_HEADER => 0,
                         CURLOPT_FOLLOWLOCATION => 1,
                         CURLOPT_TIMEOUT => 60);
        curl_setopt_array($ch, $options);
        curl_exec($ch);
        curl_close($ch);
        fclose($fp);
        $image_info = image_get_info($filename);
        if (!empty($image_info)) {
          // Insert the image in the PHPExcel file.
          $objDrawing = new PHPExcel_Worksheet_Drawing();
          $objDrawing->setPath($filename);
          $objDrawing->setCoordinates($coord);
          $objDrawing->setWidth($image_info['width']);
          $objDrawing->setWorksheet($worksheet);
        }
      }
      break;
    case 'hidden':
      // TODO 
      break;
    case 'formula':
      // TODO
      break;
    default:
      if (substr($valueformat, 0, 12) == 'text-custom:') {
        // TODO
      }
      else if (substr($valueformat, 0, 6) == 'custom') {
        // TODO
      }
      else {
        $cell->setValue($displayvalue);
      }
      break;
    }
  }
  // Number.
  else if ($valuetype == 'n') {
    // Get formatting.
    $valueformat = !empty($c['nontextvalueformat']) ? $sc['valueformats'][$c['nontextvalueformat']] : (
                   !empty($sc['defaultnontextvalueformat']) ? $sc['valueformats'][$sc['defaultnontextvalueformat']] : NULL);
    if (in_array(strtolower($valueformat), array('auto', 'none'))) {
      $valueformat = NULL;
    }

    // Default number formatting based on cell type.
    if (empty($valueformat)) switch ($valuesubtype) {
    case '%':
      $valueformat = SocialCalc_Constants::$defaultFormatp; 
      break;
    case '$':
      $valueformat = SocialCalc_Constants::$defaultFormatc; 
      break;
    case 'dt':
      $valueformat = SocialCalc_Constants::$defaultFormatdt; 
      break;
    case 'd':
      $valueformat = SocialCalc_Constants::$defaultFormatd; 
      break;
    case 't':
      $valueformat = SocialCalc_Constants::$defaultFormatt; 
      break;
    case 'l':
      $valueformat = 'logical';
      break;
    default:
      $valueformat = PHPExcel_Style_NumberFormat::FORMAT_GENERAL; 
      break;
    }

    // Set the value.
    if ($c['datatype'] == 'f') {
      if (strpos($c['formula'], '!') !== FALSE) {
        _sheetnode_phpexcel_export_reference($c, $cell, $worksheet);
      }
      else {
        $cell->setValue('='. $c['formula']);
      }
    }
    else if ($valueformat == 'logical') {
      $displayvalue = empty($displayvalue) ? SocialCalc_Constants::$defaultDisplayFALSE : SocialCalc_Constants::$defaultDisplayTRUE;
      $cell->setValue($displayvalue);
      $valueformat = NULL;
    }
    else if ($valueformat == 'formula') {
      $cell->setValueExplicit('='. $c['formula'], PHPExcel_Cell_DataType::TYPE_STRING);
    }
    else if ($valueformat == 'forcetext') {
      $cell->setValueExplicit($displayvalue, PHPExcel_Cell_DataType::TYPE_STRING);
    }
    else {
      $cell->setValueExplicit($displayvalue, PHPExcel_Cell_DataType::TYPE_NUMERIC);
    }

    // Set the formatting.
    if ($valueformat == 'hidden') {
      // TODO
    }
    else if (!empty($valueformat) && !empty($style)) {
      $style->getNumberFormat()->setFormatCode($valueformat);
    }
  }
  // Unknown.
  else {
    $cell->setValueExplicit('', PHPExcel_Cell_DataType::TYPE_NULL);
  }
}

/**
 * Helper to export referenced sheet.
 */
function _sheetnode_phpexcel_export_reference($c, $cell, $worksheet) {
  list($sheetname, $reference) = explode('!', $c['formula'], 2);
  $sheetname = trim($sheetname, '"\'');
  list($value, $title) = sheetnode_find_sheet($sheetname);
  if ($value) {
    $workbook = $worksheet->getParent();
    $external = $workbook->getSheetByName($title);
    if (!$external) {
      $external = $workbook->createSheet($workbook->getSheetCount());
      $socialcalc = socialcalc_parse($value);
      _sheetnode_phpexcel_export_sheet($external, $title, $socialcalc);
    }
    $cell->setValue('=\'' . $title . '\'!' . $reference);
  }
  else {
    $cell->setValue($c['datavalue']);
    $comment = t(variable_get('sheetnode_phpexcel_bad_formula', 'Could not export formula !formula.'), array('!formula' => $c['formula']));
    $worksheet->getComment($cell->getCoordinate())->getText()->createTextRun($comment);
  }
}

/**
 * Class to convert an HTML fragment to a fully-parsed array of styles.
 * Uses TCDPF which is included in PHPExcel.
 */
if (class_exists('TCPDF')) {
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
}

/**
 * Implementation of hook_sheetnode_phpexcel_html2richtext.
 *
 * Handles standard HTML tags and converts them to styling commands on the current rich-text run.
 *
 * @param $run PHPExcel_RichText_Run instance
 * @param $cell PHPExcel_Cell instance
 * @param $entry array entry of DOM as returned by TCPDF::getHtmlDomArray()
 * @param $converter Sheetnode_PHPExcel_HTML2RichText instance
 */
function sheetnode_phpexcel_sheetnode_phpexcel_html2richtext($run, $cell, $entry, $converter) {
  // Set style based on tag.
  switch ($entry['value']) {
  case 'strong':
  case 'b':
    $run->getFont()->setBold(TRUE);
    break;
  case 'em':
  case 'i':
    $run->getFont()->setItalic(TRUE);
    break;
  case 'u':
    $run->getFont()->setUnderline(PHPExcel_Style_Font::UNDERLINE_SINGLE);
    break;
  case 'strike':
    $run->getFont()->setStrikethrough(TRUE);
    break;
  case 'sub':
    $run->getFont()->setSubScript(TRUE);
    break;
  case 'sup':
    $run->getFont()->setSuperScript(TRUE);
    break;
  case 'a':
    if (!empty($entry['attribute']['href'])) {
      $cell->getHyperlink()->setUrl($entry['attribute']['href']);
    }
    break;
  }
  // Set style based on tag attributes if any.
  // Set style base on CSS style if any.
  if (!empty($entry['style'])) foreach ($entry['style'] as $key => $value) switch ($key) {
  case 'color':
    $color = $converter->convertHTMLColorToDec($value);
    $rgb = sprintf('%02X%02X%02X', $color['R'], $color['G'], $color['B']);
    $run->getFont()->getColor()->setRGB($rgb);
    break;
  case 'font-family':
    $run->getFont()->setName($value);
    break;
  case 'font-size':
    $run->getFont()->setSize($converter->getHTMLUnitToUnits($value, 1, 'pt', TRUE));
    break;
  case 'font-style':
    switch ($value) {
    case 'italic':
      $run->getFont()->setItalic(TRUE);
      break;
    }
    break;
  case 'text-decoration':
    switch ($value) {
    case 'underline':
      $run->getFont()->setUnderline(PHPExcel_Style_Font::UNDERLINE_SINGLE);
      break;
    }
    break;
  case 'font-weight':
    if ($value == 'bold' || $value == 'bolder' || $value >= 700) {
      $run->getFont()->setBold(TRUE);
    }
    break;
  case 'background-color':
    // TODO: Text runs don't support background colors.
    break; 
  }
}

