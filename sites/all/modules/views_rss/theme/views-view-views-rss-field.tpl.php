<?php
// Received full markup, nothing to theme.
if (!empty($markup)) {
  print $markup;
  return;
}
print "<$element";
// Process element arguments if provided.
if (!empty($arguments) && is_array($arguments)) {
  foreach ($arguments as $key => $argument_value) {
    print " $key=\"$argument_value\"";
  }
}
// Process element value if provided and close element.
if (!empty($value)) {
  print ">$value</$element>\n";
}
// Process element's sub-elements if provided.
elseif (!empty($subelements) && is_array($subelements)) {
  print ">\n";
  // Subelements have already been themed in preprocess function.
  foreach ($subelements as $subelement) {
    print $subelement;
  }
  print "</$element>\n";
}
// Otherwise make it a self-closing element.
else {
  print " />\n";
}
