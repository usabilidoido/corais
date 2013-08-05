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
  
  <!-- assumes id="main-content" on the content wrapper -->
    <!-- assumes id="nav" on the primary links wrapper -->
   <a href="#nav"><?php print t('Skip to navigation'); ?></a>

  <?php if ($search_box): ?>
    <!-- assumes id="search-box" on the search box wrapper -->
    <a href="#search-box"><?php print t('Skip to search'); ?></a>
  <?php endif; ?>
</div>
</div>
<div class="wrap">
	
    <div class="wrap-inner">
     
     <div class="header-top">
     	<div class="header-left"></div>
        <div class="header-mid"><?php if($logo) print '<img src="'. $logo .'" alt="Site Logo" />'; ?></div>
        <div class="header-right"></div>
     </div>
     <div class="content-wrap">
         <div class="content-inner-wrap">
             <div class="content-inner">
             	 
                 <div class="col-left">
                    <div id="main-navigation" class="main-navigation">
                        <?php if ($primary_links): ?>
                          <div id="nav">
                             <?php print theme('links', $primary_links, array('class' => 'links primary-links')) ?>
                          </div>
                        <?php endif; ?>
                        
                        <?php if ($search_box): ?>
                          <div id="search-box">
                            <?php print $search_box; ?>
                          </div>
                        <?php endif; ?>
                        <?php print $left; ?>
            <?php print $right; ?>
                    </div>
                 </div>
                 <div id="main-content" class="col-right">
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
      		<h1><?php print $title; ?></h1>
      		<?php print $content; ?>
      		<?php print $feed_icons; ?>
                 </div>
                 <div class="footer-copy"><?php print $footer_message . $footer ?></div>
             </div><!-- end .content-wrap -->
         </div><!-- end .content-inner-wrap -->
     </div><!-- end .content-inner -->
	<div class="footer-wrap">
    	
    	<div class="footer-left"></div>
        <div class="footer-mid"></div>
        <div class="footer-right"></div>
        <div class="footer-bottom"></div>
    </div>
    </div><!-- end .wrap-inner -->

</div><!-- end .wrap-inner -->
<?php print $closure;?>  
</body>
</html>
