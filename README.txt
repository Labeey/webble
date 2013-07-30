Control your Belkin Wemo Switch using the Pebble Watch. (iOS only)

Compile the "Webble" app using the pebble sdk or cloudpebble.net
If you press the UP/DOWN/SELECT button on your pebble it will send an http request to one of the php scripts (won.php, woff.php) via the httpebble
The php script will send trigger@ifttt.com an email with subject line "#OFF" or "#ON"
Create a "MAIL RECIPE" on your IFTTT account to TURN ON you registered WEMO Switch.
NOTE: I just modified the original code writtten by the author of httpebble. This is still a work in progress. it works but there is a 3-10 second delay from pressing the UP/DOWN button until the Wemo switch turns on.
 EDIT /webble/src/main.c     lines "http://YOURSITE/wemo/won.php" and http://YOURSITE/wemo/woff.php
COPY "won.php" and "woff.php" to you servers "PUBLIC_HTML/wemo/" folder and edit them. 


Keep in mind that i am NOT a programmer. i wrote this for myself if you find it useful let me know.



