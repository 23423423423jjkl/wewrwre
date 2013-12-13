<?php

/**
 * @file
 * Default template for feed displays that use the YML style.
 *
 * @ingroup views_templates
 */
?>
<?php print "<?xml"; ?> version="1.0" encoding="utf-8" <?php print "?>"; ?>
<?php print $doctype; ?>
<yml_catalog date="<?php print $date; ?>">
  <shop>
    <?php print $name; ?>
    <?php print $company; ?>
    <?php print $url; ?>
    <?php print $platform; ?>
    <?php print $version; ?>
    <?php print $agency; ?>
    <?php print $email; ?>
    <?php print $currencies; ?>
    <?php print $categories; ?>
    <?php print $local_delivery_cost; ?>
    <offers>
      <?php print $rows; ?>
    </offers>
  </shop>
</yml_catalog>
