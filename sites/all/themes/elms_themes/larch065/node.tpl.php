  <div class="node<?php if ($sticky) { print " sticky"; } ?><?php if (!$status) { print " node-unpublished"; } ?>">
<?php 
	$title_helper = '';
	if(isset($node->parent)){ 
		if($node->nolevel == 0){
			$level = 1;
			$tmpnode = node_load($node->parent);
			while($tmpnode->parent != 0){
				$tmpnode = node_load($tmpnode->parent);
				$level++;
			}
			//need to account for etext's
			$title_helper = variable_get("course_designer_level_0" . $level,'');
		}
		if($node->nonumber == 0){
		//calculate the current page number
			$current = 0;
			$tmpnode = $node;
			$tmpnode = node_load($tmpnode->parent);
			if($tmpnode->type == 'heading'){
				$counting_root = $tmpnode->parent;
			}else{
				$counting_root = $tmpnode->nid;
			}
			$tmpnode = $node;
			//this will calculate the current page number we are on
			while($tmpnode->nid != $counting_root){
				$tmpnode = book_prev($tmpnode);
				$tmpnode = node_load($tmpnode->nid);
				if($tmpnode->type != 'heading'){
					$current++;
				}
			}
			$title_helper.= ' ' . $current . '. ';
		}
		$output = _course_designer_book_navigation($node) . '<br>';
		if(strpos($output,'Resource') == 0 && strpos($output,'Lesson') == 0 && strpos($output,'0 of 0') == 0){
			print $output;
		}
	}
?>
	   <span class="taxonomy"><?php //print $terms?></span>
		<?php
		 if ($page == 0) { ?> 
			<h1 class="title"><a href="<?php print $node_url?>"><?php print $title_helper . ' ' . $title?></a></h1>
		<?php }else{ ?>
			<h1 class="title"><?php print $title_helper . ' ' . $title?></h1>
		<?php } ?>
		<span class="submitted"><?php print $submitted?></span>
		<div class="content"><br /><?php print $content?><br /><br /></div>
  </div>