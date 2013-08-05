 <!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8" />
<title><?php print $head_title ?></title>
<?php print $styles ?>
<?php print $scripts?>
</head>

<body>
<div id="skip-nav" class="offscreen">
  <a href="#main-content"><?php print t('Skip to main content'); ?></a>
</div>
    <div id="main">
    	<div id="logo-mark">
      <?php if (!empty($logo)): ?>
          <?php print l('<img src="'. $logo .'" alt="'. t('Home') .'" />', variable_get('site_frontpage',''), array('html' => TRUE, 'attributes' => array('rel' => "home", 'id' => "logo")));?>
        			<?php endif; ?>            
    	</div>
        <div id="header">   
    	</div>
        </a>
        <div id="left">
        	<div id="nav">
                <div id="top-nav">
                </div>
                <div id="content-nav">
                	<?php print $left ?>
                </div>
                <div id="bottom-nav">
                </div>
            </div>
        </div>
        <div id="right">
        	<div id="main-content">
            <a name="main-content"></a>
            	<div id="content-print-links">
					<?php print $links; ?>
                </div>
            	<div id="content-header">
                	<?php print $messages; ?>
                    <?php if ($tabs): print '<div id="tabs-wrapper" class="clear-block">'; endif; ?>          
                    <?php if ($tabs): print $tabs .'</div>'; endif; ?>
                    <?php if (isset($tabs2)): print $tabs2; endif; ?>
                    <h1><?php 
                    if(isset($node)){
                        if($node->field_longer_title[0]['value'] != ''){
                            print $node->field_longer_title[0]['value'];
                        }else{
                            print $node->title;
                        }
                    }else{
                        print $title; 
                    }
                    ?></h1>
                </div>
                <div id="content-divider">
                </div>
                <div id="content">
                	<?php print $content ?>
                </div>
                <div id="content-footer-divider">
                </div>
                <div id="content-footer">
                	<?php print $footer . $footer_message ?> 
                </div>
            </div>
        </div>
    </div>
    <?php print $closure; ?>
</body>
</html>
