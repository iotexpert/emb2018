#include "wiced_resource.h"

const char examples_DIR_apps_DIR_snips_DIR_aws_iot_app_DIR_resources_aws_config_html_data[10891] = "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n" \
"<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Strict//EN\" \"http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd\">\n" \
"<html xmlns=\"http://www.w3.org/1999/xhtml\">\n" \
"<head>\n" \
"<meta http-equiv=\"Content-Type\" content=\"text/html;charset=utf-8\" />\n" \
"<title>Cypress WICED AWS IOT Service</title>\n" \
"<style type=\"text/css\">\n" \
".wrapper {\n" \
"  background: linear-gradient(45deg,rgba(125,185,232,1) 0,rgba(30,87,153,1) 100%);\n" \
"  position: absolute;\n" \
"  width: 100%;\n" \
"  height: 100%;\n" \
"  overflow: hidden;\n" \
"}\n" \
".wrapper.form-success .container h1 {\n" \
"  -webkit-transform: translateY(85px);\n" \
"      -ms-transform: translateY(85px);\n" \
"          transform: translateY(85px);\n" \
"}\n" \
".container {\n" \
"  margin: 0 auto;\n" \
"  text-align: center;\n" \
"}\n" \
"button, .btn {\n" \
"  -webkit-appearance: none;\n" \
"  -moz-appearance: none;\n" \
"  appearance: none;\n" \
"  outline: 0;\n" \
"  background-color: #16b;\n" \
"  text-shadow: 0 1px 1px rgba(0,0,0,0.75);\n" \
"  color: white;\n" \
"  border: 0;\n" \
"  padding: 10px 15px;\n" \
"  color: #53e3a6;\n" \
"  border-radius: 7px;\n" \
"  width: 15%;\n" \
"  cursor: pointer;\n" \
"  font-size: 14px;\n" \
"  -webkit-transition-duration: 0.25s;\n" \
"          transition-duration: 0.25s;\n" \
"}\n" \
"\n" \
"</style>\n" \
"\n" \
"<script type=\"text/javascript\">\n" \
"      var progress_run_id = null;\n" \
"      var control = null;\n" \
"      var cert_transferred = false;\n" \
"      var key_transferred = false;\n" \
"      var trigger_for_upgrade = null;\n" \
"      function progress( prog, current_pos, full_length, trans ) {\n" \
"        var node = document.getElementById(prog);\n" \
"        var prg_width = parseInt(document.getElementById(\'progress_plane\').style.width);\n" \
"        var transferring_stage = current_pos * 100 / full_length;\n" \
"        transferring_stage = Math.round(transferring_stage * 10) / 10;\n" \
"        var new_progress_bar_position = current_pos * prg_width/full_length;;\n" \
"        var w    = node.style.width.match(/\\\\d+/);\n" \
"        if (current_pos == full_length){\n" \
"          w = 0;\n" \
"          document.getElementById(trans).innerHTML=\"Transfer completed\";\n" \
"          node.style.width = 100 + \'%\';\n" \
"\n" \
"          if(prog == \"progress_certificate\")\n" \
"            cert_transferred = true;\n" \
"          else\n" \
"            key_transferred = true;\n" \
"\n" \
"          if(cert_transferred == true && key_transferred == true)\n" \
"          {\n" \
"              document.getElementById(\"reboot\").innerHTML=\"Configure WiFi to complete the procedure\";\n" \
"          }\n" \
"        }\n" \
"        else\n" \
"        {\n" \
"          node.style.width = new_progress_bar_position  + \'%\';\n" \
"          console.log(\"Updating progress\");\n" \
"          document.getElementById(trans).innerHTML=(\"Transferring \"+ transferring_stage.toString() + \"%\" );\n" \
"        }\n" \
"      }\n" \
"      function send_file_in_chunks(id){\n" \
"        var blobs = [];\n" \
"        var blob;\n" \
"        var xhr;\n" \
"        var total = 0;\n" \
"        var chunk_count = 0;\n" \
"        var image = \"certificate\";\n" \
"        var button = \"upload_certificate_button\";\n" \
"        var prog = \"progress_certificate\";\n" \
"        var trans = \"certificate_transfer_complete\";\n" \
"        if(id == 2)\n" \
"        {\n" \
"            image = \"key\";\n" \
"            button = \"upload_key_button\";\n" \
"            prog = \"progress_key\";\n" \
"            trans = \"key_transfer_complete\";\n" \
"        }\n" \
"        var file = document.getElementById(image).files[0];\n" \
"        var bytes_per_chunk = 1024;\n" \
"        var start = 0;\n" \
"        var end = bytes_per_chunk;\n" \
"        var size = file.size;\n" \
"        var upgrade_chunk_url_with_query;\n" \
"        var current_pos = 0;\n" \
"        if (window.XMLHttpRequest)\n" \
"        {\n" \
"          xhr = new XMLHttpRequest();\n" \
"        }\n" \
"        else\n" \
"        {\n" \
"          xhr = new ActiveXObject(\"Microsoft.XMLHTTP\");\n" \
"        }\n" \
"        upgrade_chunk_url_with_query = \"upgrade_chunk.html\" + \"?\" + \"file=\" + image + \"&\" + \"offset=\" + current_pos + \"&\" + \"filesize=\" + size;\n" \
"        xhr.open(\'POST\', upgrade_chunk_url_with_query , true);\n" \
"        xhr.timeout = 2000;\n" \
"        xhr.onreadystatechange = send_function;\n" \
"        function send_function()\n" \
"        {\n" \
"          if ((xhr.readyState==4 && xhr.status==200))\n" \
"          {\n" \
"            current_pos = parseInt(xhr.responseText);\n" \
"            console.log(\"Response received\" + current_pos);\n" \
"            progress(prog, current_pos, file.size, trans );\n" \
"            if( current_pos != file.size)\n" \
"            {\n" \
"              if (window.XMLHttpRequest)\n" \
"              {\n" \
"                xhr = new XMLHttpRequest();\n" \
"              }\n" \
"              else\n" \
"              {\n" \
"                xhr = new ActiveXObject(\"Microsoft.XMLHTTP\");\n" \
"              }\n" \
"              xhr.onreadystatechange = send_function;\n" \
"              upgrade_chunk_url_with_query = \"upgrade_chunk.html\" + \"?\" + \"file=\" + image + \"&\" + \"offset=\" + current_pos + \"&\" + \"filesize=\" + file.size;\n" \
"              xhr.open(\'POST\', upgrade_chunk_url_with_query, true);\n" \
"              xhr.timeout = 4000;\n" \
"              chunk_count++;\n" \
"              console.log(\"Sending\" + chunk_count);\n" \
"              xhr.send(file.slice(current_pos, ( current_pos + 1024 )));\n" \
"            }\n" \
"          }\n" \
"          else\n" \
"          {\n" \
"            if ( (xhr.readyState==4 && xhr.status==0) )\n" \
"            {\n" \
"              if (window.XMLHttpRequest)\n" \
"              {\n" \
"                xhr = new XMLHttpRequest();\n" \
"              }\n" \
"              else\n" \
"              {\n" \
"                xhr = new ActiveXObject(\"Microsoft.XMLHTTP\");\n" \
"              }\n" \
"              xhr.onreadystatechange = send_function;\n" \
"              upgrade_chunk_url_with_query = \"upgrade_chunk.html\" + \"?\" + \"file=\" + image + \"&\" + \"offset=\" + current_pos + \"&\" + \"filesize=\" + file.size;\n" \
"              xhr.open(\'POST\', upgrade_chunk_url_with_query, true);\n" \
"              xhr.timeout = 4000;\n" \
"              console.log(\"Resending after comms failure\" + chunk_count + total );\n" \
"              xhr.send(file.slice(current_pos, ( current_pos + 1024 )));\n" \
"            }\n" \
"            else\n" \
"            {\n" \
"              console.log(\"Status =\" + xhr.readyState + \"  \" + xhr.status );\n" \
"            }\n" \
"          }\n" \
"        }\n" \
"        xhr.send(file.slice(0, 1024));\n" \
"      }\n" \
"      trigger_for_upgrade = document.getElementById(\"upgrade_certificate_button\");\n" \
"      control = document.getElementById(\"certificate\");\n" \
"      console.log(\"Here 2\");\n" \
"</script>\n" \
"<script>\n" \
"        function update_settings(){\n" \
"            var thing, url, msg;\n" \
"            thing = document.getElementById(\'thing\').value;\n" \
"            msg = document.getElementById(\"save_message\");\n" \
"            url = \"configuration_settings.html?mode=set\" + \"&thing=\" + thing;\n" \
"            var xhttp = new XMLHttpRequest();\n" \
"              xhttp.onreadystatechange = function() {\n" \
"                if (xhttp.readyState == 4 && xhttp.status == 200) {\n" \
"                  msg.innerHTML = \"Settings saved successfully\";\n" \
"                  setTimeout(function(){\n" \
"                    msg.innerHTML = \"\";\n" \
"                  },5000);\n" \
"                }\n" \
"              }\n" \
"              xhttp.open(\"POST\", url, true);\n" \
"              xhttp.send();\n" \
"        }\n" \
"\n" \
"        function get_settings(){\n" \
"            var url = \"configuration_settings.html?mode=get\";\n" \
"            var xhttp = new XMLHttpRequest();\n" \
"            xhttp.onreadystatechange = function() {\n" \
"                if (xhttp.readyState == 4 && xhttp.status == 200) {\n" \
"                  document.getElementById(\'thing\').value = xhttp.responseText;\n" \
"                  setTimeout(function(){\n" \
"                    msg.innerHTML = \"\";\n" \
"                  },5000);\n" \
"                }\n" \
"              }\n" \
"              xhttp.open(\"POST\", url, true);\n" \
"              xhttp.send();\n" \
"        }\n" \
"\n" \
"</script>\n" \
"</head>\n" \
"\n" \
"<div width=\"98%\">\n" \
"<table border=\'0\' cellpadding=\'0\' cellspacing=\'0\' width=\"98%\">\n" \
" <tr style=\"border-collapse: collapse; padding: 0;\">\n" \
"   <td style=\"width:20px\"></td>\n" \
"   <td style=\"width:152px\"><img src=\"../../images/cypresslogo.png\" alt=\"Cypress Logo\" /></td>\n" \
"   <td style=\"vertical-align:middle; text-align:center; font: bold 25px/100% Verdana, Arial, Helvetica, sans-serif;background-image:url(\'../../images/cypresslogo_line.png\');\">\n" \
"   WICED&trade; AWS IOT Service\n" \
"   </td>\n" \
"   <td style=\"width:137px;background-image:url(\'../../images/cypresslogo_line.png\');\"></td>\n" \
" </tr>\n" \
" <tr><td>&nbsp;&nbsp;&nbsp;</td></tr>\n" \
"</table>\n" \
"\n" \
"</div>\n" \
"<div class=\"wrapper\">\n" \
"<div class=\"container\">\n" \
"   <body onload=\"get_settings()\">\n" \
"   <div id=\"settings\" class=\"settings\">\n" \
"   <fieldset align =\"center\" style = \"border: 1px solid rgb(255,232,57);width: 60%;margin:auto;margin-top:3%; border-radius: 5px;padding: 15px;\">\n" \
"      <legend style=\"font: bold 15px/100% Verdana, Arial, Helvetica, sans-serif;\">Configuration Settings : </legend>\n" \
"        <label for=\"thing\" style=\"padding-right:1%;font: bold 15px/100% Verdana, Arial, Helvetica, sans-serif;\">Thing Name</label>\n" \
"        <input id=\"thing\" type=\"text\" style=\"margin-right:8%\" placeholder=\"thing-name\" />\n" \
"        <br/>\n" \
"        <br/>\n" \
"        <Button id=\"save_settings\" class=\"save_settings\" onclick=\"update_settings()\">Save Settings</Button>\n" \
"        <p id=\"save_message\"></p>\n" \
"        </fieldset>\n" \
"        </div>\n" \
"      <br/>\n" \
"      <br/>\n" \
"\n" \
"    <fieldset align =\"center\" style = \"border: 1px solid rgb(255,232,57);width: 60%;margin:auto;\">\n" \
"    <legend style=\"font: bold 15px/100% Verdana, Arial, Helvetica, sans-serif;\"> Upload Certificate and Key :</legend>\n" \
"    <div class=\"upload_certificate\" style=\"padding-top:35px\">\n" \
"      <input class=\"file_selector\" type=\"file\" name=\"Select certificate\" id=\"certificate\"></input>\n" \
"      <button class= \"upload_certificate_button\" type=\"button\" id=\"upgrade_certificate_button\" onclick=\"send_file_in_chunks(1)\" >Upload Certificate</button>\n" \
"      <div id=\"progress_plane\" style=\" background-color:rgba(165, 167, 169, 0.39); border-radius: 10px; margin-left:27%; margin-top:2%; border: 1px solid rgba(0, 58, 255, 0.93); width:45%; height:15px; font-family: arial, arial, arial; top:10; left:10; position:relative;\"/>\n" \
"      <div id=\"progress_certificate\" style=\" border-radius: 10px; height:100%; width:0px; background-color:#0065E9; top:10; left:10;\"></div>\n" \
"      <h5 class=\"one\" id = \"certificate_transfer_complete\"></h5>\n" \
"      </div>\n" \
"      <br/>\n" \
"      <br/>\n" \
"\n" \
"      <div class=\"upload_key\">\n" \
"      <input class=\"file_selector\" type=\"file\" name=\"Select key\" id=\"key\"></input>\n" \
"      <button class= \"upload_key_button\" type=\"button\" id=\"upload_key_button\" onclick=\"send_file_in_chunks(2)\" >Upload key</button>\n" \
"      <div id=\"progress_plane\" style=\" background-color:rgba(165, 167, 169, 0.39); border-radius: 10px;margin-left:27%; margin-top:2%; border: 1px solid rgba(0, 58, 255, 0.93); width:45%; height:15px; font-family: arial, arial, arial; top:10; left:10; position:relative;\"/>\n" \
"      <div id=\"progress_key\" style=\" border-radius: 10px; height:100%; width:0px; background-color:#0065E9; top:10; left:10;\"></div>\n" \
"      <h5 class=\"one\" id = \"key_transfer_complete\"></h5>\n" \
"      </div>\n" \
"\n" \
"      <br/>\n" \
"      <br/>\n" \
"      <br/>\n" \
"      <h4 class=\"one\" id = \"reboot\"></h4>\n" \
"      <input type=\"button\" class =\"btn\" value=\"Wi-Fi Setup >\" onclick=\"document.location.href=\'scan_page_outer.html\'\" />\n" \
"      <br/>\n" \
"      <br/>\n" \
"      </frameset>\n" \
"      </body>\n" \
"\n" \
"      </div>\n" \
"      </div>\n" \
"</html>\n" \
"";
const resource_hnd_t examples_DIR_apps_DIR_snips_DIR_aws_iot_app_DIR_resources_aws_config_html = { RESOURCE_IN_MEMORY, 10890, { .mem = { examples_DIR_apps_DIR_snips_DIR_aws_iot_app_DIR_resources_aws_config_html_data }}};
