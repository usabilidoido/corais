<?php

/**
 * @file
 * Default theme implementation to configure blocks.
 *
 * Available variables:
 * - $block_regions: An array of regions. Keyed by name with the title as value.
 * - $block_listing: An array of blocks keyed by region and then delta.
 * - $form_submit: Form submit button.
 * - $throttle: TRUE or FALSE depending on throttle module being enabled.
 *
 * Each $block_listing[$region] contains an array of blocks for that region.
 *
 * Each $data in $block_listing[$region] contains:
 * - $data->region_title: Region title for the listed block.
 * - $data->block_title: Block title.
 * - $data->region_select: Drop-down menu for assigning a region.
 * - $data->weight_select: Drop-down menu for setting weights.
 * - $data->throttle_check: Checkbox to enable throttling.
 * - $data->configure_link: Block configuration link.
 * - $data->delete_link: For deleting user added blocks.
 *
 * @see template_preprocess_block_admin_display_form()
 * @see theme_block_admin_display()
 */
?>
<?php
  // Add table javascript.
  drupal_add_js('misc/tableheader.js');
  drupal_add_js(drupal_get_path('module', 'taskbar_ui') .'/taskbar_ui.js');
  drupal_add_css(drupal_get_path('module', 'taskbar_ui') .'/taskbar_ui.css');
  foreach ($item_regions as $name => $title) {
    drupal_add_tabledrag('items', 'match', 'sibling', 'item-region-select', 'item-region-'. $name, NULL, FALSE);
    drupal_add_tabledrag('items', 'order', 'sibling', 'item-weight', 'item-weight-'. $name);
  }
?>
<table id="items" class="sticky-enabled">
  <thead>
    <tr>
      <th><?php print t('Item'); ?></th>
      <th><?php print t('Region'); ?></th>
      <th><?php print t('Weight'); ?></th>
      <th colspan="3"><?php print t('Operations'); ?></th>
    </tr>
  </thead>
  <tbody>
    <?php $row = 0; ?>
    <?php foreach ($item_regions as $region => $title): ?>
      <tr class="region region-<?php print $region?>">
        <td colspan="6" class="region"><?php print $title; ?></td>
      </tr>
      <tr class="region-message region-<?php print $region?>-message <?php print empty($item_listing[$region]) ? 'region-empty' : 'region-populated'; ?>">
        <td colspan="6"><em><?php print t('No item in this region'); ?></em></td>
      </tr>
      <?php foreach ($item_listing[$region] as $name => $item): ?>
      <tr class="draggable <?php print $row % 2 == 0 ? 'odd' : 'even'; ?><?php print $item->row_class ? ' '. $item->row_class : ''; ?>">
        <td class="item"><?php print $item->item_title; ?></td>
        <td><?php print $item->region_select; ?></td>
        <td><?php print $item->weight_select; ?></td>
        <td><?php print $item->configure_link; ?></td>
        <td><?php print $item->export_link; ?></td>
        <td><?php print $item->delete_link; ?></td>
      </tr>
      <?php $row++; ?>
      <?php endforeach; ?>
    <?php endforeach; ?>
  </tbody>
</table>

<?php print $form_submit; ?>
