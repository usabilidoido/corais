
  
  <?php if (!empty($comment->svg)): ?>
    <div class='comment_draw'>
      <div id="inputdraw<?php print $comment->cid; ?>"></div>
      <textarea style="width: 100%; height: 14px;"><?php print $comment->svg; ?></textarea>
<script type="text/javascript">
new InputDraw("/<?php module_invoke("comment_draw"); print drupal_get_path("module","comment_draw"); ?>/inputdraw/inputdraw.non-commercial.v1.5.swf", "inputdraw<?php print $comment->cid; ?>", {src_svg: '<?php print $comment->svg; ?>', width:"300", height:"500", animation:60, color: "#FFFFFF", grid_style: "2 0.6 rgb(211,221,244)" });
</script>
    </div>
  <?php endif; ?>