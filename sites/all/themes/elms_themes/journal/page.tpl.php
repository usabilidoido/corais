<?php
// $Id: page.tpl.php,v 1.11.2.1 2009/04/30 00:13:31 goba Exp $

/**
 * @file page.tpl.php
 *
 * Theme implementation to display a single Drupal page.
 *
 * Available variables:
 *
 * General utility variables:
 * - $base_path: The base URL path of the Drupal installation. At the very
 *   least, this will always default to /.
 * - $css: An array of CSS files for the current page.
 * - $directory: The directory the theme is located in, e.g. themes/garland or
 *   themes/garland/minelli.
 * - $is_front: TRUE if the current page is the front page. Used to toggle the mission statement.
 * - $logged_in: TRUE if the user is registered and signed in.
 * - $is_admin: TRUE if the user has permission to access administration pages.
 *
 * Page metadata:
 * - $language: (object) The language the site is being displayed in.
 *   $language->language contains its textual representation.
 *   $language->dir contains the language direction. It will either be 'ltr' or 'rtl'.
 * - $head_title: A modified version of the page title, for use in the TITLE tag.
 * - $head: Markup for the HEAD section (including meta tags, keyword tags, and
 *   so on).
 * - $styles: Style tags necessary to import all CSS files for the page.
 * - $scripts: Script tags necessary to load the JavaScript files and settings
 *   for the page.
 * - $body_classes: A set of CSS classes for the BODY tag. This contains flags
 *   indicating the current layout (multiple columns, single column), the current
 *   path, whether the user is logged in, and so on.
 *
 * Site identity:
 * - $front_page: The URL of the front page. Use this instead of $base_path,
 *   when linking to the front page. This includes the language domain or prefix.
 * - $logo: The path to the logo image, as defined in theme configuration.
 * - $site_name: The name of the site, empty when display has been disabled
 *   in theme settings.
 * - $site_slogan: The slogan of the site, empty when display has been disabled
 *   in theme settings.
 * - $mission: The text of the site mission, empty when display has been disabled
 *   in theme settings.
 *
 * Navigation:
 * - $search_box: HTML to display the search box, empty if search has been disabled.
 * - $primary_links (array): An array containing primary navigation links for the
 *   site, if they have been configured.
 * - $secondary_links (array): An array containing secondary navigation links for
 *   the site, if they have been configured.
 *
 * Page content (in order of occurrance in the default page.tpl.php):
 * - $left: The HTML for the left sidebar.
 *
 * - $breadcrumb: The breadcrumb trail for the current page.
 * - $title: The page title, for use in the actual HTML content.
 * - $help: Dynamic help text, mostly for admin pages.
 * - $messages: HTML for status and error messages. Should be displayed prominently.
 * - $tabs: Tabs linking to any sub-pages beneath the current page (e.g., the view
 *   and edit tabs when displaying a node).
 *
 * - $content: The main content of the current Drupal page.
 *
 * - $right: The HTML for the right sidebar.
 *
 * Footer/closing data:
 * - $feed_icons: A string of all feed icons for the current page.
 * - $footer_message: The footer message as defined in the admin settings.
 * - $footer : The footer region.
 * - $closure: Final closing markup from any modules that have altered the page.
 *   This variable should always be output last, after all other dynamic content.
 *
 * @see template_preprocess()
 * @see template_preprocess_page()
 */
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN"
  "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="<?php print $language->language ?>" lang="<?php print $language->language ?>" dir="<?php print $language->dir ?>">

<head>
  <?php print $head; ?>
  <title><?php print $head_title; ?></title>
  <?php print $styles; ?>
  <?php print $scripts; ?>

	<script type="text/javascript"><?php /* Needed to avoid Flash of Unstyled Content in IE */ ?> </script>
</head>
<body class="<?php print $body_classes; ?>">
    <div id="top_bar">
       <?php if (!empty($search)): ?>
       <div id="search"><?php print $search; ?></div>
       <?php endif; ?>

	</div>

	
    <div id="main_wrapper">

		<div id="site_title">
    <?php if (!empty($logo)): ?>
          		<a href="<?php print variable_get('site_frontpage',''); ?>" title="<?php print t('Home'); ?>" rel="home" id="logo">
            	<img src="<?php print $logo; ?>" alt="<?php print t('Home'); ?>" />
          		</a>
        	<?php endif; ?>
    <span class="sitename"><?php print l($site_name, variable_get('site_frontpage',''));?></span>
    <span class="siteslogan"><?php print $site_slogan;?></span>
			

		</div> <!-- /site_title -->
		<div id="top_cover">
    		<div id="main_navigation">
    		</div> <!-- main_navigation -->
		
        	<div id="top_paper"></div> <!-- /top_paper -->
        	<div id="grunge_top"></div> <!-- /grunge_top -->
	
    	</div> <!-- /top_cover -->

	
    	<div id="book_middle">
            
            <div id="content_wrapper" class="clearfix">
            	
                
                <div id="left_wrapper">


                
                
                	<?php if (!empty($left)): ?>
        				<div id="sidebar-left" class="column sidebar clearfix">
        
						<div id="colleft">
							<div id="panel">
          						<?php print $left; ?>

							</div> <!-- /panel -->
						</div> <!-- /colleft -->
						
                        <div id="togglePanel"><span></span></div> <!-- /togglePanel -->
                        </div> <!-- /sidebar-left -->
      				<?php endif; ?>
				</div> <!-- /left_wrapper -->




			<div id="right_wrapper">
      <?php if ($primary_links): ?>
  					<div id="superfish"><?php print theme('links', $primary_links); ?></div>
				<?php endif; ?>
           		<div id="section_title">
      				<?php if (!empty($title)): ?><h1 class="title" id="page-title"><?php print $title; ?></h1><?php endif; ?>

            	</div> <!-- /section_title -->
            
				<div id="main" class="column">
                	<div id="main-squeeze">

       				 <div id="content">
         				<?php if (!empty($tabs)): ?><div class="tabs"><?php print $tabs; ?></div><?php endif; ?>
          				<?php if (!empty($messages)): print $messages; endif; ?>
          				<?php if (!empty($help)): print $help; endif; ?>
         
         
          				<div id="content-content" class="clear-block">
                   		 	
                            
                            <?php print $content; ?>

                    
                    		
                            <?php if(!empty($vocab)): ?>
                    			<div id="vocab_wrapper" class="vocab_wrapper">
                    				<div class="msg_list">
										<div class="msg_head">
                        					<img alt="vocab" title="vocab" src="sites/all/themes/journal/images/vocab.png" />
                                		</div> <!-- /msg_head -->
								
                                		<div class="msg_body">
                            			<?php print $vocab; ?>
                            
                           	 			</div> <!-- /msg_body -->
									</div> <!-- /msg_list -->
									<div class="msg_bottom"></div>
                       		 </div> <!-- /vocab_wrapper -->
                            <?php endif; ?>
 

          </div> <!-- /content-content -->
          <?php print $feed_icons; ?>
        </div> <!-- /content -->

      </div></div> <!-- /main-squeeze /main -->


			</div> <!-- /right_wrapper -->
			</div> <!-- /content_wrapper -->



		</div> <!-- /book_middle -->

<div id="book_bottom">
<div id="grunge_bottom">
</div> <!-- /grunge_bottom -->
</div> <!-- /book_bottom -->



</div> <!-- /main_wrapper -->

<div id="footer-wrapper">
        
      <div id="footer">
        <?php print $footer_message; ?>
        <?php if (!empty($footer)): print $footer; endif; ?>
      </div> <!-- /footer -->
    </div> <!-- /footer-wrapper -->

    <?php print $closure; ?>
    



    
</body>
</html>
