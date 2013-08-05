
To enable the use of thickbox iframe copy thickbox_addhook.patch
to the thickbox module's folder and run

patch -p0 < thickbox_addhook.patch

Alternately add the following line to function _thickbox_doheader()

  module_invoke_all('thickbox_js');

immediately above

  drupal_add_js($path .'/thickbox.js');


convert_name2uid.php is an example script for converting large numbers of nodes, comments etc
Use at your own risk.
