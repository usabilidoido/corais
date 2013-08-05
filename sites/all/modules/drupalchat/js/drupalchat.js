Drupal.drupalchat = Drupal.drupalchat || {};
var drupalchat = {
	username: null,
	uid: null,
	send_current_message: null,
	send_current_message_id: null,
	last_timestamp: 0,
	send_current_uid2: 0,
	attach_messages_in_queue: 0,
	running: 0,
	/*smilies: { 
    //smiley     image_url          title_text              alt_smilies           
    ":)":    [ "1.gif",        "happy",                ":-)"                 ],
    ":(":    [ "2.gif",         "sad",                  ":-("                 ],
    ";)":    [ "3.gif",        "winking",              ";-)"                 ],
    ":D":    [ "4.gif",         "big grin",             ":-D"                 ],
    "^_^":   [ "muhaha.gif",      "happy eyes"                                  ],
    ">:o":   [ "haha.gif",        "laughing eyes"                               ],
    ":3":    [ "hohoho.png",      "laughing eyes"                               ],
    ">:-(":  [ "waiting.png",     "grumpy",               "X-(","X(","x-(","x(" ],
    ":'(":   [ "crying.png",      "crying"                                      ],
    ":o":    [ "omg.png",         "shocked",              ":-o"                 ],
    "8)":    [ "cool.png",        "glass",                "8-)","B)","B-)"      ],
    "8-|":   [ "ouch.png",        "cool",                 "8-|"                 ],
    ":p":    [ "tongue.png",      "tongue",               ":-p",":P",":-P"      ],
    "O.o":   [ "stupid-idea.png", "woot?!"                                      ],
    "-_-":   [ "huh-where.png",   "dark emote"                                  ],
    ":/":    [ "oopsy.png",       "duhhh",                ":-/",":\\",":-\\"    ],
    ":*":    [ "kiss.png",        "kiss",                 ":-*"                 ],
    "<3":    [ "love.png",        "heart",                                      ],
    "3:)":   [ "very-devil-plan.png", "devil smile"                                 ],
    "O:)":   [ "idiotic-smile.png",   "angel"                                       ]
  }*/
  smilies: { /*
    smiley     image_url          title_text              alt_smilies           */
    ":)":    [ "1.gif",           "happy",                ":-)"                 ],
    ":(":    [ "2.gif",           "sad",                  ":-("                 ],
    ";)":    [ "3.gif",           "winking",              ";-)"                 ],
    ":D":    [ "4.gif",           "big grin",             ":-D"                 ],
    ";;)":   [ "5.gif",           "batting eyelashes"                           ],
    ">:D<":  [ "6.gif",           "big hug"                                     ],
    ":-/":   [ "7.gif",           "confused",             ":/"                  ],
    ":x":    [ "8.gif",           "love struck",          ":X"                  ],
    ":\">":  [ "9.gif",           "blushing"                                    ],
    ":P":    [ "10.gif",          "tongue",               ":p", ":-p", ":-P"    ],
    ":-*":   [ "11.gif",          "kiss",                 ":*"                  ],
    "=((":   [ "12.gif",          "broken heart"                                ],
    ":-O":   [ "13.gif",          "surprise",             ":O"                  ],
    "X(":    [ "14.gif",          "angry"                                       ],
    ":>":    [ "15.gif",          "smug"                                        ],
    "B-)":   [ "16.gif",          "cool"                                        ],
    ":-S":   [ "17.gif",          "worried"                                     ],
    "#:-S":  [ "18.gif",          "whew!",                "#:-s"                ],
    ">:)":   [ "19.gif",          "devil",                ">:-)"                ],
    ":((":   [ "20.gif",          "crying",               ":-((", ":'(", ":'-(" ],
    ":))":   [ "21.gif",          "laughing",             ":-))"                ],
    ":|":    [ "22.gif",          "straight face",        ":-|"                 ],
    "/:)":   [ "23.gif",          "raised eyebrow",       "/:-)"                ],
    "=))":   [ "24.gif",          "rolling on the floor"                        ],
    "O:-)":  [ "25.gif",          "angel",                "O:)"                 ],
    ":-B":   [ "26.gif",          "nerd"                                        ],
    "=;":    [ "27.gif",          "talk to the hand"                            ],
    "I-)":   [ "28.gif",          "sleepy"                                      ],
    "8-|":   [ "29.gif",          "rolling eyes"                                ],
    "L-)":   [ "30.gif",          "loser"                                       ],
    ":-&":   [ "31.gif",          "sick"                                        ],
    ":-$":   [ "32.gif",          "don't tell anyone"                           ],
    "[-(":   [ "33.gif",          "not talking"                                 ],
    ":O)":   [ "34.gif",          "clown"                                       ],
    "8-}":   [ "35.gif",          "silly"                                       ],
    "<:-P":  [ "36.gif",          "party",                "<:-p"                ],
    "(:|":   [ "37.gif",          "yawn"                                        ],
    "=P~":   [ "38.gif",          "drooling"                                    ],
    ":-?":   [ "39.gif",          "thinking"                                    ],
    "#-o":   [ "40.gif",          "d'oh",                 "#-O"                 ],
    "=D>":   [ "41.gif",          "applause"                                    ],
    ":-SS":  [ "42.gif",          "nailbiting",           ":-ss"                ],
    "@-)":   [ "43.gif",          "hypnotized"                                  ],
    ":^o":   [ "44.gif",          "liar"                                        ],
    ":-w":   [ "45.gif",          "waiting",              ":-W"                 ],
    ":-<":   [ "46.gif",          "sigh"                                        ],
    ">:P":   [ "47.gif",          "phbbbbt",              ">:p"                 ],
    "<):)":  [ "48.gif",          "cowboy"                                      ],
    ":@)":   [ "49.gif",          "pig"                                         ],
    "3:-O":  [ "50.gif",          "cow",                  "3:-o"                ],
    ":(|)":  [ "51.gif",          "monkey"                                      ],
    "~:>":   [ "52.gif",          "chicken"                                     ],
    "@};-":  [ "53.gif",          "rose"                                        ],
    "%%-":   [ "54.gif",          "good luck"                                   ],
    "**==":  [ "55.gif",          "flag"                                        ],
    "(~~)":  [ "56.gif",          "pumpkin"                                     ],
    "~O)":   [ "57.gif",          "coffee"                                      ],
    "*-:)":  [ "58.gif",          "idea"                                        ],
    "8-X":   [ "59.gif",          "skull"                                       ],
    "=:)":   [ "60.gif",          "bug"                                         ],
    ">-)":   [ "61.gif",          "alien"                                       ],
    ":-L":   [ "62.gif",          "frustrated",           ":L"                  ],
    "[-O<":  [ "63.gif",          "praying"                                     ],
    "$-)":   [ "64.gif",          "money eyes"                                  ],
    ":-\"":  [ "65.gif",          "whistling"                                   ],
    "b-(":   [ "66.gif",          "feeling beat up"                             ],
    ":)>-":  [ "67.gif",          "peace sign"                                  ],
    "[-X":   [ "68.gif",          "shame on you"                                ],
    "\\:D/": [ "69.gif",          "dancing"                                     ],
    ">:/":   [ "70.gif",          "bring it on"                                 ],
    ";))":   [ "71.gif",          "hee hee"                                     ],
    "o->":   [ "72.gif",          "hiro"                                        ],
    "o=>":   [ "73.gif",          "billy"                                       ],
    "o-+":   [ "74.gif",          "april"                                       ],
    "(%)":   [ "75.gif",          "yin yang"                                    ],
    ":-@":   [ "76.gif",          "chatterbox"                                  ],
    "^:)^":  [ "77.gif",          "not worthy"                                  ],
    ":-j":   [ "78.gif",          "oh go on"                                    ],
    "(*)":   [ "79.gif",          "star"                                        ],
    ":)]":   [ "100.gif",         "on the phone"                                ],
    ":-c":   [ "101.gif",         "call me"                                     ],
    "~X(":   [ "102.gif",         "at wits' end"                                ],
    ":-h":   [ "103.gif",         "wave"                                        ],
    ":-t":   [ "104.gif",         "time out"                                    ],
    "8->":   [ "105.gif",         "daydreaming"                                 ],
    ":-??":  [ "106.gif",         "I don't know"                                ],
    "%-(":   [ "107.gif",         "not listening"                               ],
    ":o3":   [ "108.gif",         "puppy dog eyes"                              ],
    "X_X":   [ "109.gif",         "I don't want to see",  "x_x"                 ],
    ":!!":   [ "110.gif",         "hurry up!"                                   ],
    "\\m/":  [ "111.gif",         "rock on!"                                    ],
    ":-q":   [ "112.gif",         "thumbs down"                                 ],
    ":-bd":  [ "113.gif",         "thumbs up"                                   ],
    "^#(^":  [ "114.gif",         "it wasn't me"                                ],
    ":bz":   [ "115.gif",         "bee"                                         ],
    ":ar!":  [ "pirate.gif",      "pirate"                                      ],
    "[..]":  [ "transformer.gif", "transformer"                                 ]
  }
};

$(document).ready(function(){
	/*YUI().use('gallery-storage-lite', function (Y) {
	    Y.StorageLite.on('storage-lite:ready', function () {
	        if (drupalchat_getCookie('DRUPALCHAT_NEWLOGIN') != 1) {
	          if(Y.StorageLite.getItem('username')!=null) {
	        	drupalchat.username = Y.StorageLite.getItem('username');
	          }
	          if(Y.StorageLite.getItem('uid')!=null) {
	        	drupalchat.uid = Y.StorageLite.getItem('uid');
	          }
	          if(Y.StorageLite.getItem('send_current_message')!=null) {
	        	drupalchat.send_current_message = Y.StorageLite.getItem('send_current_message');
	          }	
	          if(Y.StorageLite.getItem('last_timestamp')!=null) {
	        	drupalchat.last_timestamp = Y.StorageLite.getItem('last_timestamp');
	          }
	          if(Y.StorageLite.getItem('send_current_uid2')!=null) {
	        	drupalchat.send_current_uid2 = Y.StorageLite.getItem('send_current_uid2');
	          }
	          if(Y.StorageLite.getItem('attach_messages_in_queue')!=null) {
	        	drupalchat.attach_messages_in_queue = Y.StorageLite.getItem('attach_messages_in_queue');
	          }
	          if(Y.StorageLite.getItem('running')!=null) {
	        	drupalchat.running = Y.StorageLite.getItem('running');
	          }
	          if(Y.StorageLite.getItem('drupalchat')!=null) {
	        	if(Y.StorageLite.getItem('drupalchat').length > 4) {
	        		$('#drupalchat').html(Y.StorageLite.getItem('drupalchat'));
	        	}
	          }
	          if((drupalchat.send_current_uid2!=null) && ($("#chatbox_" + drupalchat.send_current_uid2 + " .chatboxcontent").length>0)) {
				$("#chatbox_" + drupalchat.send_current_uid2 + " .chatboxcontent").scrollTop($("#chatbox_" + drupalchat.send_current_uid2 + " .chatboxcontent")[0].scrollHeight);
			  }  
	        }
	        else {
	          drupalchat_setCookie('DRUPALCHAT_NEWLOGIN', 2, 0);
	        }

	    });

	});
	*/
	if (drupalchat_getCookie('DRUPALCHAT_NEWLOGIN') != 1) {
	  if($.drupalchatjStorage.get('username')!=null) {
	    drupalchat.username = $.drupalchatjStorage.get('username');
	  }
	  if($.drupalchatjStorage.get('uid')!=null) {
	    drupalchat.uid = $.drupalchatjStorage.get('uid');
	  }
	  if($.drupalchatjStorage.get('send_current_message')!=null) {
	    drupalchat.send_current_message = $.drupalchatjStorage.get('send_current_message');
	  }	
	  if($.drupalchatjStorage.get('last_timestamp')!=null) {
	    drupalchat.last_timestamp = $.drupalchatjStorage.get('last_timestamp');
	  }
	  if($.drupalchatjStorage.get('send_current_uid2')!=null) {
	    drupalchat.send_current_uid2 = $.drupalchatjStorage.get('send_current_uid2');
	  }
	  if($.drupalchatjStorage.get('attach_messages_in_queue')!=null) {
	    drupalchat.attach_messages_in_queue = $.drupalchatjStorage.get('attach_messages_in_queue');
	  }
	  if($.drupalchatjStorage.get('running')!=null) {
	    drupalchat.running = $.drupalchatjStorage.get('running');
	  }
	  if($.drupalchatjStorage.get('drupalchat')!=null) {
	    if($.drupalchatjStorage.get('drupalchat').length > 4) {
	      $('#drupalchat').html($.drupalchatjStorage.get('drupalchat'));
		  if(Drupal.settings.drupalchat.polling_method == '2') {
		    $('#chatpanel .subpanel ul').empty();
			$('#chatpanel .online-count').html($('#chatpanel .subpanel ul > li').size());
		  }
	    }
	  }
	  if((drupalchat.send_current_uid2!=null) && ($("#chatbox_" + drupalchat.send_current_uid2 + " .chatboxcontent").length>0)) {
	    $("#chatbox_" + drupalchat.send_current_uid2 + " .chatboxcontent").scrollTop($("#chatbox_" + drupalchat.send_current_uid2 + " .chatboxcontent")[0].scrollHeight);
	  }
	}
	else {
	  drupalchat_setCookie('DRUPALCHAT_NEWLOGIN', 2, 0);
	}
	//Load smileys.
	emotify.emoticons( Drupal.settings.drupalchat.smileyURL, drupalchat.smilies );
	//Adjust panel height
	$.fn.adjustPanel = function(){
	    $(this).find("ul, .subpanel").css({ 'height' : 'auto'}); //Reset sub-panel and ul height
	
	    var windowHeight = $(window).height(); //Get the height of the browser viewport
	    var panelsub = $(this).find(".subpanel").height(); //Get the height of sub-panel
	    var panelAdjust = windowHeight - 100; //Viewport height - 100px (Sets max height of sub-panel)
	    var ulAdjust =  panelAdjust - 25; //Calculate ul size after adjusting sub-panel
	
	    if (panelsub > panelAdjust) {	 //If sub-panel is taller than max height...
	        $(this).find(".subpanel").css({ 'height' : panelAdjust}); //Adjust sub-panel to max height
	        $(this).find("ul").css({ 'height' : panelAdjust}); ////Adjust subpanel ul to new size
	    }
	    else { //If sub-panel is smaller than max height...
	    	$(this).find("ul").css({ 'height' : 'auto'}); //Set sub-panel ul to auto (default size)
	    }
	};
	
	//Execute function on load
	$("#chatpanel").adjustPanel(); //Run the adjustPanel function on #chatpanel
	
	//Each time the viewport is adjusted/resized, execute the function
	$(window).resize(function () {
	    $("#chatpanel").adjustPanel();
	});
	
	//Add sound effect SWF file to document
	$("<div id=\"drupalchatbeep-wrapper\"><object id=\"drupalchatbeep\" classid=\"clsid:D27CDB6E-AE6D-11cf-96B8-444553540000\" width=\"1\" height=\"1\"><param name=\"movie\" value=\"" + Drupal.settings.drupalchat.sound + "?random="+(Math.random()+new Date().getMilliseconds())+ "\"/><!--[if !IE]>--><object type=\"application/x-shockwave-flash\" data=\"" + Drupal.settings.drupalchat.sound + "\" width=\"1\" height=\"1\"></object><!--<![endif]--></object></div>").appendTo($("body"));
	swfobject.registerObject("drupalchatbeep", "9");
	
	//Click event on subpanels	
	$("#mainpanel li a.subpanel_toggle, .chatbox a.chatboxhead").live('click', function() { //If clicked on the first link of #chatpanel...
	    if($(this).next(".subpanel").is(':visible')){ //If subpanel is already active...
	        $(this).next(".subpanel").hide(); //Hide active subpanel
	        $("#drupalchat li a").removeClass('active'); //Remove active class on the subpanel trigger
	    }
	    else { //if subpanel is not active...
	        $(".subpanel").hide(); //Hide all subpanels
	        $(this).next(".subpanel").toggle(); //Toggle the subpanel to make active
	        $("#drupalchat li a").removeClass('active'); //Remove active class on all subpanel trigger
	        $(this).toggleClass('active'); //Toggle the active class on the subpanel trigger
	        // Chat box functions
	        var isTextarea = $(this).next(".subpanel").children(".chatboxinput").children(".chatboxtextarea");
	        if (isTextarea.length > 0) { 
	        	isTextarea[0].focus();
	        	$(this).next(".subpanel").children(".chatboxcontent").scrollTop($(this).next(".subpanel").children(".chatboxcontent")[0].scrollHeight);
	        }
	    }
	    return false; //Prevent browser jump to link anchor
	});
		
	$('.subpanel .subpanel_title').live('click', function() { //Click anywhere and...
	    $(".subpanel").hide(); //hide subpanel
	    $("#drupalchat li a").removeClass('active'); //remove active class on subpanel trigger
	});	
	$('.subpanel ul').click(function(e) {
//	    e.stopPropagation(); //Prevents the subpanel ul from closing on click
	});

	$("#chatpanel .subpanel li:not(.link) a").live('click', function() {
		chatWith($(this).attr("class"), $(this).text());
	});	
	
	$(".chatbox .subpanel_title span:not(.min)").live('click', function () {
		closeChatBox($(this).attr('class'));
	});
	
	if (Drupal.settings.drupalchat.status == 1) {
		$(".chat_options .chat_loading").removeClass('chat_loading').addClass('status-2').html(Drupal.settings.drupalchat.goIdle);
		$("#chatpanel .icon").attr("src", Drupal.settings.drupalchat.images + "chat-1.png");
	}
	else if (Drupal.settings.drupalchat.status == 2) {
		$(".chat_options .chat_loading").removeClass('chat_loading').addClass('status-1').html(Drupal.settings.drupalchat.goOnline);
		$("#chatpanel .icon").attr("src", Drupal.settings.drupalchat.images + "chat-2.png");
	}
		
	$(".chat_options .status-1").live('click', function() {
		$(".chat_options .status-1").removeClass('status-1').addClass('chat_loading');
		$.post(Drupal.settings.drupalchat.statusUrl, {status: "1"}, function(data){
			$(".chat_options .chat_loading").removeClass('chat_loading').addClass('status-2').html(Drupal.settings.drupalchat.goIdle);
			$("#chatpanel .icon").attr("src", Drupal.settings.drupalchat.images + "chat-1.png");
		});			
	});
	$(".chat_options .status-2").live('click', function() {
		$(".chat_options .status-2").removeClass('status-2').addClass('chat_loading');
		$.post(Drupal.settings.drupalchat.statusUrl, {status: "2"}, function(data){
			$(".chat_options .chat_loading").removeClass('chat_loading').addClass('status-1').html(Drupal.settings.drupalchat.goOnline);
			$("#chatpanel .icon").attr("src", Drupal.settings.drupalchat.images + "chat-2.png");
		});			
	});	
	
	$(".chat_options .options").live('click', function() {
		/* alert('Under Construction');*/
	});
	
	/* Hide options for now */
	$(".chat_options .options").hide();
	
  
	// Add short delay before first poll call. This avoids Chrome loading-icon bug. 
  setTimeout(function () {
    chatPoll();
  }, 500);
});

function chatWith(chatboxtitle, chatboxname) {
	createChatBox(chatboxtitle, chatboxname);
	$("#chatbox_"+chatboxtitle+" a:first").click(); //Toggle the subpanel to make active
	$("#chatbox_"+chatboxtitle+" .chatboxtextarea").focus();
}

function createChatBox(chatboxtitle, chatboxname, chatboxblink) {
	if ($("#chatbox_"+chatboxtitle).length > 0) {
		if ($("#chatbox_"+chatboxtitle).css('display') == 'none') {
			$("#chatbox_"+chatboxtitle).css('display', 'block');
		}
		$("#chatbox_"+chatboxtitle+" .chatboxtextarea").focus();
		return;
	}

	$(" <li />" ).attr("id","chatbox_"+chatboxtitle)
	.addClass("chatbox")
	.html('<a href="#" class="chatboxhead"><span class="subpanel_title_text">'+chatboxname+'</span></a><div class="subpanel"><div class="subpanel_title"><span class="'+chatboxtitle+'">x</span><span class="min">_</span><div class="status-1"></div>'+chatboxname+'</div><div class="chatboxcontent"></div><div class="drupalchat_userOffline">'+chatboxname+' is currently offline.</div><div class="chatboxinput"><textarea class="chatboxtextarea" onkeydown="javascript:return checkChatBoxInputKey(event,this,\''+chatboxtitle+'\');"></textarea></div></div>')
	.prependTo($( "#mainpanel" ));
	
	if (chatboxblink == 1) {
		$('#chatbox_'+chatboxtitle+' .chatboxhead').addClass("chatboxblink");
	}

	$("#chatbox_"+chatboxtitle+" .chatboxtextarea").blur(function(){
		$("#chatbox_"+chatboxtitle+" .chatboxtextarea").removeClass('chatboxtextareaselected');
	}).focus(function(){
		$('#chatbox_'+chatboxtitle+' .chatboxhead').removeClass('chatboxblink');
		$("#chatbox_"+chatboxtitle+" .chatboxtextarea").addClass('chatboxtextareaselected');
	});

	$("#chatbox_"+chatboxtitle).click(function() {
		if ($('#chatbox_'+chatboxtitle+' .chatboxcontent').css('display') != 'none') {
			$("#chatbox_"+chatboxtitle+" .chatboxtextarea").focus();
		}
	});

	$("#chatbox_"+chatboxtitle).show();
}

function chatPoll() {
	if(Drupal.settings.drupalchat.polling_method == '0') {
		setInterval(function() {
			$.post(Drupal.settings.drupalchat.pollUrl,
				{ 
			   		drupalchat_last_timestamp: drupalchat.last_timestamp
				},
				processChatData
			);
		}, (Drupal.settings.drupalchat.refresh_rate) * 1000);		
	}
	else if(Drupal.settings.drupalchat.polling_method == '1') {
		$.post(Drupal.settings.drupalchat.pollUrl,
			{
	   		drupalchat_last_timestamp: drupalchat.last_timestamp
			},
			processChatData
		);
	}
}

function processChatData(data) {
	var drupalchat_messages = Drupal.parseJson(data);
	if((!drupalchat_messages.status || drupalchat_messages.status == 0)) {
		if((drupalchat_messages.hasOwnProperty("messages")) && (drupalchat_messages.messages.length > 0)) {
			// Play new message sound effect
			var obj = swfobject.getObjectById("drupalchatbeep");
			if (obj && Drupal.settings.drupalchat.notificationSound == "1") {
			  try {
			    obj.drupalchatbeep(); // e.g. an external interface call
		      }
			  catch(e) {
			  }
			}			
		}
		$.each(drupalchat_messages.messages, function(index, value) {
			var drupalselfmessage = false;
			if(value.uid1 == Drupal.settings.drupalchat.uid) {
			  drupalselfmessage = true;
			}
			//Add div if required.
			if(value.uid2=="c-0") {
			  drupalchatroom = true;
			}
            else {
			  drupalchatroom = false;
            }			
			chatboxtitle = (drupalchatroom || drupalselfmessage)?value.uid2:value.uid1;
			
			if ($("#chatbox_"+chatboxtitle).length <= 0) {
				createChatBox(chatboxtitle, drupalchatroom?"Public Chatroom":value.name, 1);
			}
			else if ($("#chatbox_"+chatboxtitle+" .subpanel").is(':hidden')) {
				if ($("#chatbox_"+chatboxtitle).css('display') == 'none') {
					$("#chatbox_"+chatboxtitle).css('display','block');
				}
				$('#chatbox_'+chatboxtitle+' .chatboxhead').addClass("chatboxblink");
				$("#chatbox_"+chatboxtitle+" .chatboxtextarea").blur(function(){
					$("#chatbox_"+chatboxtitle+" .chatboxtextarea").removeClass('chatboxtextareaselected');
				}).focus(function(){
					$('#chatbox_'+chatboxtitle+' .chatboxhead').removeClass('chatboxblink');
					$("#chatbox_"+chatboxtitle+" .chatboxtextarea").addClass('chatboxtextareaselected');
				});
				//$("#chatbox_"+chatboxtitle+" a:first").click(); //Toggle the subpanel to make active
				//$("#chatbox_"+chatboxtitle+" .chatboxtextarea").focus();
			}	
            if(value.uid1 == Drupal.settings.drupalchat.uid) {
              value.name = Drupal.settings.drupalchat.username;
			  if ($("."+value.message_id)[0]){
				return;
              }
            }			
			value.message = value.message.replace(/{{drupalchat_newline}}/g,"<br />");
			value.message = emotify(value.message);
			if ($("#chatbox_"+chatboxtitle+" .chatboxcontent .chatboxusername a:last").html() == value.name) {
				$("#chatbox_"+chatboxtitle+" .chatboxcontent").append('<p>'+value.message+'</p>');
			}
			else {
				var currentTime = new Date();
				var hours = currentTime.getHours();
				var minutes = currentTime.getMinutes();
				if (hours < 10) {
					hours = "0" + hours;
				}
				if (minutes < 10) {
					minutes = "0" + minutes;
				}				
				$("#chatbox_"+chatboxtitle+" .chatboxcontent").append('<div class="chatboxusername"><span class="chatboxtime">'+hours+':'+minutes+'</span><a href="'+Drupal.settings.drupalchat.profilePath+'/'+value.uid1+'">'+value.name+'</a></div><p>'+value.message+'</p>');
			}
			$("#chatbox_"+chatboxtitle+" .chatboxcontent").scrollTop($("#chatbox_"+chatboxtitle+" .chatboxcontent")[0].scrollHeight);
			
      $.titleAlert(Drupal.settings.drupalchat.newMessage, {requireBlur:true, stopOnFocus:true, interval:800});
		});
		
	  $('#chatpanel .subpanel ul').empty();
	  $('li[id^="chatbox_"]').each(function(){
	    Drupal.drupalchat.changeStatus(this.id,0);
	  });
	  $.each(drupalchat_messages.buddylist, function(key, value) {
		  if (key != 'total') {
			  if (key != Drupal.settings.drupalchat.uid) {
			  	$('#chatpanel .subpanel ul').append('<li class="status-' + value.status + '"><a class="' + key + '" href="#">' + value.name + '</a></li>');
			    Drupal.drupalchat.changeStatus('chatbox_'+key,1);
			  }
		  }
		  else {
			  $('#chatpanel .online-count').html(value);
		  }
		});
	  $('#chatpanel .subpanel ul li:last-child').addClass('last');
	  
	  if ($('#chatpanel .subpanel ul li').length <= 0) {
	  	$('#chatpanel .subpanel ul').append(Drupal.settings.drupalchat.noUsers);
	  }
	  
	  //Update Timestamp.
	  drupalchat.last_timestamp = drupalchat_messages.last_timestamp;  
	}
	if (Drupal.settings.drupalchat.polling_method != '0') {
		chatPoll();
	}
}

function closeChatBox(chatboxtitle) {
	$('#chatbox_'+chatboxtitle).css('display','none');
}

Drupal.drupalchat.changeStatus = function(id, state) {
	if(state == 1) {
	  jQuery('#' + id + ' .subpanel_title > div').removeClass('status-0').addClass('status-1');
	  jQuery('#' + id + ' .drupalchat_userOffline').css('display','none');
	}
	else if(state == 0) {
	  jQuery('#' + id + ' .subpanel_title > div').removeClass('status-1').addClass('status-0');
	  jQuery('#' + id + ' .drupalchat_userOffline').css('display','block');
	}
  };
  
Drupal.drupalchat.sendMessages = function () {
	$.post(Drupal.settings.drupalchat.sendUrl, {
	    drupalchat_message_id: drupalchat.send_current_message_id,
   		drupalchat_uid2: drupalchat.send_current_uid2, 
   		drupalchat_message: drupalchat.send_current_message 
	});
}
	
function checkChatBoxInputKey(event, chatboxtextarea, chatboxtitle) {
	if(event.keyCode == 13 && event.shiftKey == 0)  {
		
		message = $(chatboxtextarea).val();
		message = message.replace(/^\s+|\s+$/g,"");
		message = message.substr(0,255);
		$(chatboxtextarea).val('');
		$(chatboxtextarea).focus();
		$(chatboxtextarea).css('height','44px');
		
		var currentTime = new Date();
		var hours = currentTime.getHours();
		var minutes = currentTime.getMinutes();
		if (hours < 10) {
			hours = "0" + hours;
		}
		if (minutes < 10) {
			minutes = "0" + minutes;
		}

		
		if (message != '') {
			if(Drupal.settings.drupalchat.polling_method == '0') {
				drupalchat.send_current_uid2 = chatboxtitle;
				if (drupalchat.attach_messages_in_queue == 0) {
					setTimeout(function() { 
						Drupal.drupalchat.sendMessages();
						drupalchat.attach_messages_in_queue = 0;
					}, (Drupal.settings.drupalchat.send_rate) * 1000);
			
					drupalchat.send_current_message = message;
					var d = new Date();
			        drupalchat.send_current_message_id = Drupal.settings.drupalchat.uid + '_' + drupalchat.send_current_uid2 + '_' + d.getTime();  
					drupalchat.attach_messages_in_queue = 1;
				}
				else {
					drupalchat.send_current_message += '{{drupalchat_newline}}' + message;
				}
			}
			else {
				drupalchat.send_current_uid2 = chatboxtitle;
				drupalchat.send_current_message = message;
				var d = new Date();
		        drupalchat.send_current_message_id = 'm_' + Drupal.settings.drupalchat.uid + '_' + drupalchat.send_current_uid2 + '_' + d.getTime();
				Drupal.drupalchat.sendMessages();				
			}
			message = message.replace(/</g,"&lt;").replace(/>/g,"&gt;").replace(/\"/g,"&quot;");
			message = emotify(message);
			if ($("#chatbox_"+chatboxtitle+" .chatboxcontent .chatboxusername a:last").html() == Drupal.settings.drupalchat.username) {
				$("#chatbox_"+chatboxtitle+" .chatboxcontent").append('<p class=\''+drupalchat.send_current_message_id+'\'>'+message+'</p>');
			}
			else {
				$("#chatbox_"+chatboxtitle+" .chatboxcontent").append('<div class="chatboxusername"><span class="chatboxtime">'+hours+':'+minutes+'</span><a href="'+Drupal.settings.drupalchat.profilePath+'/'+Drupal.settings.drupalchat.uid+'">'+Drupal.settings.drupalchat.username+'</a></div><p class=\''+drupalchat.send_current_message_id+'\'>'+message+'</p>');
			}

			$("#chatbox_"+chatboxtitle+" .chatboxcontent").scrollTop($("#chatbox_"+chatboxtitle+" .chatboxcontent")[0].scrollHeight);
		}
		return false;
	}

	var adjustedHeight = chatboxtextarea.clientHeight;
	var maxHeight = 94;

	if (maxHeight > adjustedHeight) {
		adjustedHeight = Math.max(chatboxtextarea.scrollHeight, adjustedHeight);
		if (maxHeight)
			adjustedHeight = Math.min(maxHeight, adjustedHeight);
		if (adjustedHeight > chatboxtextarea.clientHeight)
			$(chatboxtextarea).css('height', adjustedHeight+8 + 'px');
	} else {
		$(chatboxtextarea).css('overflow', 'auto');
	}
}
$(window).unload(function(){
	/*
	YUI().use('gallery-storage-lite', function (Y) {

	    Y.StorageLite.on('storage-lite:ready', function () {
	    	Y.StorageLite.setItem('username', drupalchat.username);
	    	Y.StorageLite.setItem('uid', drupalchat.uid);
	    	Y.StorageLite.setItem('send_current_message', drupalchat.send_current_message);
	    	Y.StorageLite.setItem('last_timestamp', drupalchat.last_timestamp);
	    	Y.StorageLite.setItem('send_current_uid2', drupalchat.send_current_uid2);
	    	Y.StorageLite.setItem('attach_messages_in_queue', drupalchat.attach_messages_in_queue);
	    	Y.StorageLite.setItem('running', drupalchat.running);
	    	//alert($('#drupalchat').html());
	    	Y.StorageLite.setItem('drupalchat', $('#drupalchat').html());
	        });
	    });

	*/
  $.drupalchatjStorage.set('username', drupalchat.username);
  $.drupalchatjStorage.set('uid', drupalchat.uid);
  $.drupalchatjStorage.set('send_current_message', drupalchat.send_current_message);
  $.drupalchatjStorage.set('last_timestamp', drupalchat.last_timestamp);
  $.drupalchatjStorage.set('send_current_uid2', drupalchat.send_current_uid2);
  $.drupalchatjStorage.set('attach_messages_in_queue', drupalchat.attach_messages_in_queue);
  $.drupalchatjStorage.set('running', drupalchat.running);
  //alert($('#drupalchat').html());
  $.drupalchatjStorage.set('drupalchat', $('#drupalchat').html());

});
function drupalchat_getCookie(c_name)
{
var i,x,y,ARRcookies=document.cookie.split(";");
for (i=0;i<ARRcookies.length;i++)
  {
  x=ARRcookies[i].substr(0,ARRcookies[i].indexOf("="));
  y=ARRcookies[i].substr(ARRcookies[i].indexOf("=")+1);
  x=x.replace(/^\s+|\s+$/g,"");
  if (x==c_name)
    {
    return unescape(y);
    }
  }
}
function drupalchat_setCookie(c_name,value,exdays)
{
var exdate=new Date();
exdate.setDate(exdate.getDate() + exdays);
var c_value=escape(value) + ((exdays==null) ? "" : "; expires="+exdate.toUTCString());
document.cookie=c_name + "=" + c_value;
}
