$(document).ready(function(){
	//hide the all of the element with class msg_body
	//toggle the componenet with class msg_body
	
	
	$("#gallery_tab").click(function(){
									 		
		$("#gallery_tab").animate( {height: '0px', paddingTop: '0px'},200);
		$("#gallery_exit").fadeIn(500);

		$("#gallery_content").animate( {height: '500px'},1000);

		$("#content").animate( {opacity: '0.25'} , 1000);
		



		
	});
	
	$("#gallery_exit").click(function(){
		$("#gallery_exit").fadeOut(500);

		$("#gallery_content").animate( {height: '0px'},1000);
		$("#gallery_tab").animate( {height: '20px', paddingTop: '5px'},300);
		$("#content").animate( {opacity: '1'} , 1000);



		
	});
});// JavaScript Document