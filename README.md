# openideal-root

Ideal root module allows create task record in root app when some idea node added in openideal site. Used [hook_node_insert](https://api.drupal.org/api/drupal/modules%21node%21node.api.php/function/hook_node_insert/7.x) and [root](https://github.com/linnovate/root) api.

###Installation
Copy `ideal_root` folder in drupal modules directory `sites/all/modules` and activate it in admin modules section

###Settings
Module setting page located in admin configuration section.
You need to provide root server api url, eg `http://192.168.56.101:3100/api`, user email and password. 
**Note that currently password will be stored in db in not encrypted format.** 
During submit server connection and username/password will be verified, so make sure they are correct at this moment.
