<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8" />
<title><?php print $head_title ?></title>
  <?php print $head ?>
  <?php print $styles ?>
  <?php print $scripts ?>
<!--[if IE 5]>
<style type="text/css"> 
/* place css box model fixes for IE 5* in this conditional comment */
#left-col { width: 230px; }
</style>
<![endif]--><!--[if IE]>
<style type="text/css"> 
/* place css fixes for all versions of IE in this conditional comment */
#left-col { padding-top: 30px; }
#main-content { zoom: 1; }
/* the above proprietary zoom property gives IE the hasLayout it needs to avoid several bugs */
</style>
<![endif]-->

</head>

<body>
<div id="skip-nav" class="offscreen">
  <a href="#main-content"><?php print t('Skip to main content'); ?></a>
</div>
<div id="container">
	<div id="bg-top">
    </div>
  <div id="header">
    <h1><?php print l(t($site_name),'')?></h1>
    <h2><?php print $site_slogan?></h2>
  <!-- end #header --></div>
  <div id="cnt-top"></div>
  <div id="left-col"><?php print $left?><!-- end #sidebar1 --></div>
  <div id="main-content">
  <a name="main-content"></a>
  <h1><?php print $title?></h1>
    <?php if ($tabs != ""): ?>
            <div class="tabs"><?php print $tabs ?></div>
          <?php endif; ?>

        <?php if ($show_messages && $messages != ""): ?>
          <?php print $messages ?>
        <?php endif; ?>

        <?php if ($help != ""): ?>
            <div id="help"><?php print $help ?></div>
        <?php endif; ?>

      <!-- start main content -->
      <?php print $content; ?>
	<!-- end #mainContent --></div>
	<!-- This clearing element should immediately follow the #mainContent div in order to force the #container div to contain all child floats --><br class="clearfloat" />
  <div id="footer">
  <!-- end #footer --></div>
<!-- end #container --></div>
<div id="footer-btm">
  <!-- end #footer --></div>
  <div id="footer-info"><?php print $footer_message?></div><?php print $footer?>
  <?php print $closure;?>
</body>
</html>
