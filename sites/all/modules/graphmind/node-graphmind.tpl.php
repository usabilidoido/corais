<?php
// $Id: node-graphmind.tpl.php,v 1.1.2.8 2010/10/23 07:28:04 itarato Exp $

global $base_path;
$path = drupal_get_path('module', 'graphmind_service');

?>
<div id="node-<?php print $node->nid; ?>" class="node<?php if ($sticky) { print ' sticky'; } ?><?php if (!$status) { print ' node-unpublished'; } ?>">

<?php print $picture ?>

<?php if ($page == 0): ?>
  <h2><a href="<?php print $node_url ?>" title="<?php print $title ?>"><?php print $title ?></a></h2>
<?php endif; ?>

<?php if ($page): ?>
  <object classid="clsid:D27CDB6E-AE6D-11cf-96B8-444553540000"
    id="GraphMind" width="100%" height="600"
    codebase="http://fpdownload.macromedia.com/get/flashplayer/current/swflash.cab">
    <param name="movie" value="<?php echo $base_path.$path; ?>/graphmind/GraphMind.swf" />
    <param name="quality" value="high" />
    <param name="bgcolor" value="#869ca7" />
    <param name="allowScriptAccess" value="sameDomain" />
    <param name="allowFullScreen" value="true" />
    <param name="flashvars" value="<?php echo $graphmind['flashvars']; ?>" />
    <embed src="<?php echo $base_path . $path; ?>/graphmind/GraphMind.swf" quality="high" bgcolor="#869ca7"
      width="100%" height="600" name="GraphMind" align="middle"
      play="true"
      loop="false"
      quality="high"
      allowScriptAccess="sameDomain"
      allowFullScreen="true"
      type="application/x-shockwave-flash"
      pluginspage="http://www.adobe.com/go/getflashplayer"
      flashvars="<?php echo $graphmind['flashvars']; ?>"/>
  </object>
<?php endif; ?>

  <div class="clear-block">
    <div class="meta">
      <?php if ($taxonomy): ?>
        <div class="terms"><?php print $terms ?></div>
      <?php endif;?>
    </div>

    <div class="content">
      <?php print $content; ?>
    </div>

    <?php if ($links): ?>
      <div class="links"><?php print $links; ?></div>
    <?php endif; ?>
  </div>

</div>
