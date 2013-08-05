<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" lang="en">
<head> 
<meta http-equiv="Content-Type" content="text/html; charset=ISO-8859-1" />
<title><?php print $head_title ?></title>
<?php print $head ?>
  <?php print $styles ?>
  <?php print $scripts ?>
  <script type="text/javascript"><?php /* Needed to avoid Flash of Unstyle Content in IE */ ?> </script>
</head>
<body>
<div id="skip-nav" class="offscreen">
  <a href="#main-content"><?php print t('Skip to main content'); ?></a>
</div>
	<div id="container"> 
	<div id="maincontainer">
    <div id="header">
		<?php if (!empty($header)): ?>
		<?php print $header; ?>
		<?php endif; ?>
        </div>
		<div id="sidebar-left">
				<div id="leftcontainer">
					<div id="logo-image">
                    <?php if (!empty($logo)): ?>
          <?php print l('<img src="'. $logo .'" alt="'. t('Home') .'" />', variable_get('site_frontpage',''), array('html' => TRUE, 'attributes' => array('rel' => "home", 'id' => "logo")));?>
        			<?php endif; ?> 
                   </div>
					<div id="menu">
					<?php print $left; ?>
					</div>
				</div>
		<div id="main">
			<div id="rightcontainer">
				  <div id="contentbanner">
				  	<p class="contentbanner"></p>

				  </div>
					<div id="content">
                    <a name="main-content"></a>
                    <?php if ($tabs != ""): ?>
        			<div class="tabs"><?php print $tabs ?></div>
       			 <?php endif; ?> 
						<?php if ($title != ""): ?>
	        	<h1 class="title"><?php print $title ?></h1>
        <?php endif; ?>
        <?php if ($show_messages && $messages != ""): ?>
        	<?php print $messages ?>
        <?php endif; ?>
        <?php if ($help != ""): ?>
        	<div id="help"><?php print $help ?></div>
        <?php endif; ?>
        <?php print $content; ?>
        <?php print $feed_icons; ?>
					</div>
				</div>
			</div>
		</div>
	
		<div class="clear"></div>
		<div id="endcontainer"></div> 
		<div class="clear"></div>
	
		<div id="footer">
			<?php print $footer_message . $footer; ?>
		</div>
	</div>
	</div>
    <?php print $closure; ?>
</body>
</html>