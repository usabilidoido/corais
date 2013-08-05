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

<!-- ---------------- make some css that makes the skip bar visible upon focus with the keyboard, and take out psu stuff -->

	<!-- ARTIFICIAL RESIZABLE BACKGROUND -->
	<img alt="Background Image of a studio" id="bg" src="<?php print $base_path . $directory ?>/images/bg.jpg" /> 
    
    
   <div id="all"> 
	
    <div id="top">
    	<?php if (!empty($logo)): ?>
<a href="<?php print variable_get('site_frontpage',''); ?>" title="<?php print t('Home'); ?>" rel="home" id="logo"> <img src="<?php print $logo; ?>" alt="<?php print t('Home'); ?>" />
</a> <?php endif; ?>
    </div>
    
    <div id="middle-container">
    
    <div id="woody-container"><div id="woody"></div></div>
        
      <div id="middle">
          <div id="middle-top">
            <div id="middle-top-accent"></div>
            <div id="middle-top-content-left"></div>
            <div id="middle-top-content">
            	<div id="middle-top-content-navigation">
					<div id="middle-top-content-navigation-menu"><?php print $header;?></div>
                    
                </div>
                <div id="middle-top-content-title"><div id="site-title"><?php print $site_name ?></div><div id="site-sub-title"><?php print $site_slogan ?></div></div>
                <div id="middle-top-content-header">
				 <div id="node-title"><?php print $title?></div>
            </div>
            <div id="middle-top-content-right"></div>
          </div>
          <div id="middle-content-container">
          	<div id="middle-content-navigation-container">
            	
                <div id="middle-content-navigation"><?php print $left?></div>
                <div id="middle-content-navigation-images"><?php print $right?></div>
            </div>
          
            <div id="middle-content">
            
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
			<?php print $content; ?>
      		<?php print $feed_icons; ?>

            </div>
          </div>
      </div>
    </div>  
      
    <div id="bottom">
    	<div id="bottom-left"></div>
        <div id="bottom-middle">
        	<div id="bottom-middle-content">
            <?php print $footer_message . $footer ?>
            <!--
            Copyright 2007-2009 The Penn State College of Arts and Architecture and its licensors. All rights reserved.<br />
            Penn State is an Equal Opportunity Employer.<br />
            Course Developed by the College of Arts and Architecture's e-Learning Institute.
            -->
            
            </div>
        </div>
        <div id="bottom-right"></div>
        <div id="bottom-bottom"></div>
    </div>
  </div>
<?php print $closure;?>   
</body>
</html>
