<?php
/*
 * The following variables should all pertain to a currency and a time window
 * $data(
 *   'active_users' = number of users not blocked who have ever logged in.
 *   'promiscuity' = index pertaining to whole system
 *   'total_trades' = number completed transactions on the system
 *   'total_volume' = sum of completed transactions
 */
?>

Active users: <?php print $data['active_users']; ?><br />
Promiscuity index: <?php print $data['promiscuity']; ?><br />
Total trades: <?php print $data['total_trades']; ?><br />
Total volume: <?php print $data['total_volume']; ?>