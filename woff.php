<?php
$headers = 'From: email@yoursite.com' . "\r\n" .
'Reply-To: email@yoursite.com' . "\r\n" .
'X-Mailer: PHP/' . phpversion();
mail('trigger@ifttt.com','#OFF','OFF',$headers);
echo 'Wemo Turned OFF!';
?>