<?php
// $Id: node-currency.tpl.php,v 1.1.2.4 2010/12/22 19:30:29 matslats Exp $

if (!$node->sub) $division = t('integer');
elseif (!isset($node->data['divisions']) || !count($node->data['divisions'])) $division = t('centiles');
else {
   $vals = $node->data['divisions'];
   $divisions = array();
   foreach ($vals as $val) {
     $divisions[] = trim(strrchr($val, '|'));
     $division  = str_replace('|', '', implode(', ', $divisions));
   }
}
?>
<div class="node node-currency">
  <?php if ($teaser) { ?>
    <h2><?php print l($node->title, 'node/'.$node->nid); ?></h2>
    <p><?php print $content; ?></p>
  <?php } elseif ($page) { ?>
    <h3><?php print t('Rationale'); ?></h3>
    <p style=""color:#<?php print $node->data['color']; ?>">
    <?php print theme('image', $node->icon, 'icon', 'icon'); ?>
    <?php print t('created by !user', array('!user' => theme('username', user_load($node->uid)))); ?><br />
    <?php print $content; ?></p>


    <h4><?php print t('Range'); ?></h4>
    <?php print theme('currency_range', $node); ?>

    <?php if (isset($node->data['divisions']) && count($node->data['divisions'])) { ?>
      <h4><?php print t('Divisions'); ?></h4>
      <?php print theme('currency_division_array', $node->data['divisions']); ?>
    <?php } ?>

    <?php if ($node->value) { ?>
      <h4><?php print t('Relative value'); ?>
      <p><?php print $node->value ?></p>
    <?php } ?>
  <?php } ?>
</div>
