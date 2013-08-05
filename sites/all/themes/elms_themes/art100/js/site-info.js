	$(document).ready(function()
	{					
		$('#site_info_tab').toggle(
		function()
		{
			$(this).parent('#site_info').animate({height: '105px', marginTop: '-105px'},500);
			$('#site_info_tab').css('background', 'url(sites/all/themes/art100/images/info_down.png) 10px 50%  no-repeat #191919');
			
	

			
		},
		function()
		{
			
			$(this).parent('#site_info').animate({height: '20px', marginTop: '-20px'},500);
						$('#site_info_tab').css('background', 'url(sites/all/themes/art100/images/info_up.png) 10px 50%  no-repeat #191919');

		});
		
			
		
												
								
	
	
	});