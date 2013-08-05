$(document).ready(function(){	
		$("#info-tab").click(function () {
      	$(".footer-copy").toggle("slow");
  	  	});
		$("#open-close").toggle(function(){
			$("#header").animate({marginTop: '-263px'}, 500);
			$("#open-close").animate({marginRight:'-64px'}, 100).animate({marginTop: '293px'}, 500);

		},function(){
			$("#header").animate({marginTop: '0'}, 500);
			$("#open-close").animate({marginTop: '30px'}, 800).animate({marginRight:'-74px'}, 200);
			
		});
});

