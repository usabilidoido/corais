<?php if (empty($hide)): ?>

<?php if (!empty($pre_object)) print $pre_object ?>

<?php if ($comment->bestreply) $attr['class'] .= " bestreply"; ?>

<div <?php if (!empty($attr)) print drupal_attributes($attr) ?>>
  <?php if (!empty($submitted)): ?>
    <div class='<?php print $hook ?>-submitted clear-block'>
      <?php print $picture ?>
      <?php print $submitted ?>
    </div>
  <?php endif; ?>

  <?php if (!empty($title)): ?>
    <h2 class='<?php print $hook ?>-title'>
      <?php if (!empty($new)): ?><a id='new' class='new'><?php print('New') ?></a><?php endif; ?>
      <?php print $title ?> 
	  <?php if ($comment->bestreply) print " " . t("decided"); ?>
    </h2>
  <?php endif; ?>

  <?php if (!empty($content)): ?>
    <div class='<?php print $hook ?>-content clear-block <?php if (!empty($is_prose)) print 'prose' ?>'>
      <?php print $content ?>
    </div>
  <?php endif; ?>

  <?php if (!empty($comment->svg)): ?>
    <div class='comment_draw'>
      <div id="inputdraw<?php print $comment->cid; ?>"></div>
      <textarea style="width: 100%; height: 14px;"><?php print $comment->svg; ?></textarea>
<script type="text/javascript">
new InputDraw("/<?php module_invoke("comment_draw"); print(drupal_get_path("module","comment_draw")); ?>/inputdraw/inputdraw.non-commercial.v1.5.swf", "inputdraw<?php print $comment->cid; ?>", {src_svg: '<?php print $comment->svg; ?>', width:"300", height:"500", animation:60, color: "#FFFFFF", grid_style: "2 0.6 rgb(211,221,244)" });
</script>
    </div>
  <?php endif; ?>

  <?php if (!empty($links)): ?>
    <div class='<?php print $hook ?>-links clear-block'><?php print $links ?></div>
  <?php endif; ?>
</div>

<?php if (!empty($post_object)) print $post_object ?>

<?php endif; ?>
