<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
	<meta http-equiv="Content-Type" content="text/html;charset=utf-8" />
<title><?php print $head_title; ?></title>
  <?php print $head; ?>
  <?php print $styles; ?>
  <?php print $scripts; ?>
</head>
<body>
<div id="skip-nav" class="offscreen">
  <a href="#main-content"><?php print t('Skip to main content'); ?></a>
</div>
<div id="body_container" align="center">
	<!-- top container-->
	<div id="top_container_wrapper">
		<div id="top_container"></div>
	</div>
	<!-- middle container-->
	<div id="middle_container">
		<!-- left container-->
		<div id="left_container">
		<?php 		
			print $left; 
		?>
		</div>
		<!-- center container-->
		<div id="center_container">
		<a name="main-content"></a>
		<?php 	
				
				print $messages;
				print $tabs;
				if(!isset($node)){
					print '<h2 style="margin-top:0px;">' . $title . '</h2>';
				}
				
				print $content;
		?>
		</div>
			</div>
	<!-- bottom container-->
	<div id="bottom_container" align="center" style="padding-top:10px;">
		<?php print $footer_message . $footer; ?>
	</div>
</div>
<?php print $closure; ?>
</body>
</html>