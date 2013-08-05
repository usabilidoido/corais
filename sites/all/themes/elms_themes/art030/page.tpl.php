<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8" />
<title><?php print $head_title ?></title>
<?php print $head ?>
<?php print $styles ?>
<?php print $scripts ?>
</head>
<body>
<div id="skip-nav" class="offscreen">
  <a href="#main-content"><?php print t('Skip to main content'); ?></a>
</div>
<div class="ctr-wrapper">
<div class="wrapper">

    <div class="header-wrap">
    	<div class="header-lt"></div>
        <div class="header-mid">
        	<div class="mark"></div>
            <div class="nav-wrap">
            <?php print $header; ?>
            <?php print $superfish; ?>
            </div><!-- end .nav-wrap -->
        </div>
        <div class="header-rt"></div>
    </div><!-- end .header-wrap -->
    <div class="pre-cnt"></div>
    <div class="col-wrap">
        
        <div class="side-block-wrap">
         	<div class="side-block-top"></div>
            <div class="side-block-mid">
            <?php print $left; ?>
            <?php print $right; ?>
            </div>
            <div class="side-block-btm"></div>
         </div><!-- end .side-block -->
        
        <div class="cnt-wrap">
            <div class="cnt-bar"><!-- bg should be cnt_top.png -->
                <div class="cnt-bar-lt"></div>
                <div class="cnt-bar-mid">
                </div>
                <div class="cnt-bar-rt"></div>
            </div><!-- end .cnt-toolbar -->
            <div class="cnt-main">
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
            <a name="main-content"></a>
            <div class="cnt-image-gallery"><div class="cnt-image-gallery-inner"><?php print $image_gallery; ?></div></div>
      		<h2><?php print $title; ?></h2>
      		<?php print $content; ?>
      		<?php print $feed_icons; ?>
            <div class="footer"><?php print $footer_message . $footer ?></div>
            </div>
            <div class="cnt-btm"></div>
            
             
            
        </div><!-- end .cnt-wrap -->
        
         
        
	</div><!-- end .col-wrap -->

</div><!-- end .wrapper -->


</div><!-- end .ctr-wrapper -->
<?php print $closure;?>   
</body>
</html>