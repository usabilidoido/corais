<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8" />
  <title><?php print $head_title ?></title>
  <?php print $head ?>
  <?php print $styles ?>
  <?php print $scripts ?>
<!--[if IE]>
<style type="text/css"> 
/* place css fixes for all versions of IE in this conditional comment */

#mainContent { zoom: 1; }
/* the above proprietary zoom property gives IE the hasLayout it needs to avoid several bugs */
</style>
<![endif]--></head>

<body>
<div id="skip-nav" class="offscreen">
  <a href="#main-content"><?php print t('Skip to main content'); ?></a>
</div>
 <div id="mini-bar">
      <div class="prev-btn"></div>
      <div class="next-btn"></div>
      <?php print $header;?><div id="badge"></div>
  </div>
<div id="cupholder">
  <div id="now-showing">NOW SHOWING:<br /><span class="now-showing"><?php print strtoupper($title);?></span></div>
 
  </div>
<div id="container">

  <div id="header">
  
  <div id="open-close"></div>
  <div id="header-rt"></div>
 
  <!-- end #header --></div>
    <div id="main-menu">
    <div id="menu-lt-image"></div>
    <?php print $header;?>
  <!-- end #header --></div>
  <div>
  <div id="sub-nav">
    <?php print $left?>
  <!-- end #sidebar1 --></div>

  <div id="mainContent">
  <div id="content-bar">
    
  </div>
  
    <div class="pad">
      <a name="main-content"></a>
    <?php if ($tabs != ""): ?>
            <div class="tabs"><?php print $tabs ?></div>
          <?php endif; ?>
    <h1><?php print $title?></h1>
        <?php if ($show_messages && $messages != ""): ?>
          <?php print $messages ?>
        <?php endif; ?>

        <?php if ($help != ""): ?>
            <div id="help"><?php print $help ?></div>
        <?php endif; ?>

      <!-- start main content -->
      <?php print $content; ?>
	<!-- end .pad--></div>
    <div id="footer">
    <div id="info-tab"></div>
    <div class="footer-copy">
    <?php print $footer_message . $footer ?>
    </div>
  <!-- end #footer --></div><div id="footer-btm"></div>
 
    <!-- end #mainContent --></div>
	<!-- This clearing element should immediately follow the #mainContent div in order to force the #container div to contain all child floats --><br class="clearfloat" />
  </div>
  
<!-- end #container --></div>
<?php print $closure;?>
</body>
</html>
