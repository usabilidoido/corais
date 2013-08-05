<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
    <meta http-equiv="Content-Type" content="text/html; charset=UTF-8" />
    <title><?php print $head_title ?></title>
    <?php print $head ?>
    <?php print $styles ?>
	<?php print $scripts?>
</head>
<body topmargin="0px" leftmargin="0px">
<div id="skip-nav">
  <a href="#main-content"><?php print t('Skip to main content'); ?></a>
</div>
	<div id="main_container">
        <div id="top_background">
            <div id="top_background_container">
                <div id="link_top">
                    <div id="link_top_text">
                    <!--Search: <font size ="-4" face="times new roman"><u>The Site</u> | <u>Departments
                    </u> | <u>People</u> | <u>Penn State</u></font>--->
                    </div>
                </div>
                <div id="banner">
               </div>
             </div>
        </div>
        <div id="content_container">
            <div id="content_area">
                 <div id="left_nav">
                 <?php print $left ?>
                </div>
                <div id="seperation_tear">
                </div>
                <div id="right_content_background">
                    <div id="right_content">
										<?php print $messages; ?>
					<?php if ($tabs): print '<div id="tabs-wrapper" class="clear-block">'; endif; ?>          
        			<?php if ($tabs): print $tabs .'</div>'; endif; ?>
        			<?php if (isset($tabs2)): print $tabs2; endif; ?>
                    <?php print '<h1 name="main-content" id="main-content">'. $title .'</h1>'; ?>
                    <?php print $content ?>
                    </div>
                </div>
                <div id="footer_message_container">
                    <div id="footer_message">
                    <?php print $footer_message . $footer ?>
                    </div>
                </div>
                <div id="footer">
                </div>
            </div>  
            <div id="bottom_padding">
			<?php print $closure?>
            </div>                                     
        </div>
    </div>
</body>
</html>
