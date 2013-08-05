
Description
-----------------
Adds a 'Taxonomy' tab to all group home pages where privileged users may
add/edit their own vocabularies/ terms.

Features
--------
* Fine grained permissions control who can see the Taxonomy tab, create or edit
vocabulary and terms.
* Privileged user can assign/ remove a vocabulary from a group right inside
the admin/taxonomy page.
* Rules module integrations.

Themes example
-------------------
You might want to group your terms by vocab on the node display.
Here is a snippit from groups.drupal.org for node.tpl.php which does so:

<?php if ($links || $terms) { ?>
<div class="links">
   <div class="left">
    <?php if ($terms  && !$is_preview) { ?>
       <?php
        // Group terms by vocab.
        foreach ($node->taxonomy as $tid => $term) {
            $vocabs[$term->vid][$term->tid] = l($term->name, "taxonomy/term/$tid");
          }
          krsort($vocabs);

          // Render a line for each vocab. performs a query to get vocab name.
          foreach ($vocabs as $vid => $vocab) {
              $fullvocab = taxonomy_get_vocabulary($vid);
              $output .= "<div id=\"node-vocab-$vid\">". $fullvocab->name. ': '. theme('links', $vocab). "</div>";
            }
         ?>
       <div class="terms"><?php print $output; ?></div>
    <?php } ?>
   </div>
   <?php if ($links) { ?>
     <div class="right">¬ª <?php print $links?></div>
   <?php } ?>&nbsp;
</div>
<?php }; ?>

Author
------------------
Moshe Weitzman <weitzman AT tejasa.com
D6 Update: Darren Ferguson <darren.ferguson AT openband DOT net>
D6 further update: Amitai Burstein (http://drupal.org/user/57511)

Sponsors
------------------
Buyblue - http://buyblue.org
Green Party of Canada - http://www.greenparty.ca
D6 Update: OpenBand <http://www.openband.net/>