<?php
// $Id: flat-book-node-export-html.tpl.php,v 1.3 2010/08/27 15:41:45 criticalpatch Exp $
/**
 * @file
 * Default theme implementation for rendering a flattened node and
 * its children.
 *
 * @see book-node-export-html.tpl.php
 * Where it is collected and printed out.
 *
 * Available variables:
 * - $flatten_depth: Depth at which this menu tree was flattened.
 * - $depth: Depth of the current node inside the outline.
 * - $title: Node title.
 * - $content: Node content.
 * - $children: All the child nodes recursively rendered through this file.
 * - $root_nid: Node id of the root node displayed on this page
 * - $is_admin: True if edit links need to be displayed
 * - $edit_link: If $is_admin is true, the edit link markup
 * - $outline_link: If $is_admin is true, the outline link markup
 *
 * @see template_preprocess_book_node_export_html()
 */
?>
<div id="booknode-<?php print $node->nid; ?>" class="section-<?php print $depth; ?>">
  <?php if($is_admin): ?>

    <div class="outlinelink" style="float:right">[<?php print $outline_link ?>]</div>
    <div class="editlink" style="float:right">[<?php print $edit_link ?>]</div>
  <?php endif ?>

  <h2 class="book-heading"><?php print $title; ?></h2>
  <?php print $content; ?>
  <div class="back-to-top"><a href="#node-<?php print $root_nid; ?>"><?php print t('Back to Top'); ?></a></div>
  <?php print $children; ?>
</div>
