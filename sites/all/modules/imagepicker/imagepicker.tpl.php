<?php
// $Id: imagepicker.tpl.php,v 1.4.2.2 2010/09/08 10:56:16 hutch Exp $
// $Name: DRUPAL-6--2-10 $

/**
 * @file
 * template for imagepicker iframe, new version
 */

?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" lang="<?php print $language->language ?>" xml:lang="<?php print $language->language ?>">
<head>
  <title><?php echo $head_title ?></title>
  <?php echo $styles ?>
  <?php echo $scripts ?>
  <script type="text/javascript"><?php /* Needed to avoid Flash of Unstyle Content in IE */ ?> </script>
</head>
<body id="imagepicker">
  <table border="0" cellpadding="0" cellspacing="0" id="content">
  <tr>
    <td valign="top">
      <div id="main">
        <div class="tabs"><?php echo $tabs ?></div>
        <div id="imgp">
          <?php echo $messages ?>
          <?php echo $content; ?>
        </div>
      </div>
    </td>
  </tr>
  </table>
</body>
</html>
