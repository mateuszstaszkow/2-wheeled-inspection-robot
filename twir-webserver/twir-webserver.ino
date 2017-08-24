#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
//#include <avr/pgmspace.h>

#define USART_BUFFER_SIZE 100

//const char flashWebSite[] PROGMEM = {
//    ""
//  }

typedef struct MeasuredData {
  int fx;
  int fy;
  int fz;
  int ax;
  int ay;
  int az;
  int l_pos;
  int r_pos;
  int xl;
  int xr;
  float vl;
  float vr;
  int dir;
  int pid;
  int turn;
  int dist_m;
  int balance_error;
  int pos_error;
  int pos_pid;
  int motor_error;
  int motor_pid;
  int battery_state;
};

ESP8266WebServer server(80);
const char* ssid = "UPC1171078";
const char* password = "FMYEJWUS";
const char* ssidTel = "internet";
const char* passwordTel = "haslohaslo";

String webSite, javaScript, plotJS, XML;
MeasuredData measuredData;
char buffer_uart[USART_BUFFER_SIZE];

void parseData(MeasuredData &m, char* buffer) {
  int n=0;
  int i2=0;
  char number[10];
  
  for(int i=0; buffer[i] != ';'; i++) {
    if(buffer[i]==',') {
      snprintf(number, i-i2+1, buffer+i2);
      switch(n) {
        case 0:
          m.fx = atoi(number);
          break;
        case 1:
          m.fy = atoi(number);
          break;
        case 2:
          m.fz = atoi(number);
          break;
        case 3:
          m.ax = atoi(number);
          break;
        case 4:
          m.ay = atoi(number);
          break;
        case 5:
          m.az = atoi(number);
          break;
        case 6:
          m.l_pos = atoi(number);
          break;
        case 7:
          m.r_pos = atoi(number);
          break;
        case 8:
          m.xl = atoi(number);
          break;
        case 9:
          m.vl = atoi(number)*0.036;
          break;
        case 10:
          m.dir = atoi(number);
          break;
        case 11:
          m.pid = atoi(number);
          break;
        case 12:
          m.turn = atoi(number);
          break;
        case 13:
          m.dist_m = atoi(number);
          break;
        case 14:
          m.balance_error = atoi(number);
          break;
        case 15:
          m.pos_error = atoi(number);
          break;
        case 16:
          m.pos_pid = atoi(number);
          break;
        case 17:
          m.motor_error = atoi(number);
          break;
        case 18:
          m.motor_pid = atoi(number);
          break;
        case 19:
          m.xr = atoi(number);
          break;
        case 20:
          m.vr = atoi(number)*0.036;
          break;
        case 21:
          m.battery_state = atoi(number);
          break;
      }
      n++;
      i2 = i+1;
    }
  }
}

void read_uart() {  
  int i;
  for(i = 0; i < USART_BUFFER_SIZE; i++) {
    buffer_uart[i]=';';
  }
    
  Serial.readBytesUntil(';', buffer_uart, USART_BUFFER_SIZE);
  parseData(measuredData, buffer_uart);
}

void buildWebsite(){
  buildJavascript();
  webSite="<!DOCTYPE HTML>\n";
  webSite+=" <head>\n";
  webSite+="  <title>Dwukołowy robot inspekcyjny</title>\n";
  webSite+="  <meta charset=\"utf-8\">\n";
  webSite+="  <style>\n";
  webSite+="    body {\n";
  webSite+="      background-color: #000;\n"; 
  webSite+="      font-family: Calibri;\n"; 
  webSite+="      color: #fff;\n"; 
  webSite+="      text-align: center;\n"; 
  webSite+="    }\n";
  webSite+="    .footer {\n";
  webSite+="      position: fixed;\n"; 
  webSite+="      right: 0;\n"; 
  webSite+="      bottom: 0;\n"; 
  webSite+="      left: 0;\n"; 
  webSite+="      padding: 1rem;\n"; 
  webSite+="      background-color: #111;\n"; 
  webSite+="      text-align: center;\n"; 
  webSite+="    }\n";
  webSite+="    .minor-header {\n";
  webSite+="      border-bottom: 3px solid black;\n"; 
  webSite+="      font-size: 1.5em;\n"; 
  webSite+="      font-weight: bold;\n"; 
  webSite+="    }\n";
  webSite+="    .reg-header {\n";
  webSite+="      margin-top: 3px;\n";  
  webSite+="      font-size: 1.1em;\n"; 
  webSite+="      font-weight: bold;\n"; 
  webSite+="    }\n";
  webSite+="    .measurement-left-block {\n";
  webSite+="      width: 49.75%;\n"; 
  webSite+="      float: left;\n"; 
  webSite+="      border-right: 2px solid black;\n"; 
  webSite+="    }\n";
  webSite+="    .measurement-right-block {\n";
  webSite+="      width: 49.75%;\n"; 
  webSite+="      float: left;\n"; 
  webSite+="      border-left: 2px solid black;\n"; 
  webSite+="    }\n";
  webSite+="    .reg-left-block {\n";
  webSite+="      width: 33%;\n"; 
  webSite+="      float: left;\n"; 
  webSite+="      border-right: 2px solid black;\n"; 
  webSite+="    }\n";
  webSite+="    .reg-mid-block {\n";
  webSite+="      width: 33%;\n"; 
  webSite+="      float: left;\n"; 
  webSite+="      border-right: 2px solid black;\n"; 
  webSite+="      border-left: 2px solid black;\n"; 
  webSite+="    }\n";
  webSite+="    .reg-right-block {\n";
  webSite+="      width: 33%;\n"; 
  webSite+="      float: left;\n"; 
  webSite+="      border-left: 2px solid black;\n"; 
  webSite+="    }\n";
  webSite+="    .measurement-container {\n";
  webSite+="      clear: both;\n"; 
  webSite+="      margin-bottom: 15px;\n"; 
  webSite+="      background-color: DimGray;\n"; 
  webSite+="    }\n";
  webSite+="    .measurement-container:hover {\n";
  webSite+="      background-color: gray;\n"; 
  webSite+="    }\n";
  webSite+="    .scan-button {\n";
  webSite+="      background-color: green;\n"; 
  webSite+="      border: none;\n"; 
  webSite+="      color: white;\n"; 
  webSite+="      padding: 15px 40px;\n"; 
  webSite+="      text-align: center;\n"; 
  webSite+="      text-decoration: none;\n"; 
  webSite+="      display: inline-block;\n"; 
  webSite+="      font-size: 25px;\n"; 
  webSite+="      cursor: pointer;\n"; 
  webSite+="    }\n";
  webSite+="    p {\n";
  webSite+="      margin-top: 5px;\n"; 
  webSite+="      margin-bottom: 5px;\n"; 
  webSite+="    }\n";
  webSite+="    button {\n";
  webSite+="      margin-top: 2px;\n"; 
  webSite+="      margin-bottom: 3px;\n";
  webSite+="      font-size: 11px;\n"; 
  webSite+="      font-weight: bold;\n"; 
  webSite+="      padding: 4px;\n"; 
  webSite+="      padding-left: 15px;\n"; 
  webSite+="      padding-right: 15px;\n"; 
  webSite+="      cursor: pointer;\n"; 
  webSite+="    }\n";
  webSite+="  </style>\n";
  webSite+="  <script src=\"https://cdn.plot.ly/plotly-latest.min.js\"></script>\n";
  webSite+="</head>\n";
  
  webSite+=javaScript;
  webSite+=plotJS;
  
  webSite+="<body onload='start()'>\n";
  webSite+="  <div style=\"width: 800px; float: left\">\n";
  webSite+="    <h1>DWUKOŁOWY ROBOT INSPEKCYJNY</h1>\n";
  
  webSite+="    <div class='measurement-container' style=\"height: 109px;\">\n";
  webSite+="      <div class='measurement-left-block'>\n";
  webSite+="        <div class='minor-header'>Pozycja kątowa:</div>\n";
  webSite+="        <p>FX: <A id='fx'></A></p>\n";
  webSite+="        <p>FY: <A id='fy'></A></p>\n";
  webSite+="        <p>FZ: <A id='fz'></A></p>\n";
  webSite+="      </div>\n";
  webSite+="      <div class='measurement-right-block'>\n";
  webSite+="        <div class='minor-header'>Przyspieszenie kątowe:</div>\n";
  webSite+="        <p>AX: <A id='ax'></A> 1/s2</p>\n";
  webSite+="        <p>AY: <A id='ay'></A> 1/s2</p>\n";
  webSite+="        <p>AZ: <A id='az'></A> 1/s2</p>\n";
  webSite+="      </div>\n";
  webSite+="    </div>\n";
  
  webSite+="    <div class='measurement-container' style=\"height: 61px;\">\n";
  webSite+="      <div style='width: 79%; float: left;'>\n";
  webSite+="        <div class='minor-header'>Skanowanie pomieszczeń:</div>\n";
  webSite+="        <div class='measurement-left-block' style='width: 49.685%;'>\n";
  webSite+="          <p>Odległość od przeszkody: <A id='mdist'></A> cm</p>\n";
  webSite+="        </div>\n";
  webSite+="        <div class='measurement-right-block' style='width: 49.685%;'>\n";
  webSite+="          <button>WYŚWIETL MAPĘ</button>\n";
  webSite+="        </div>\n";
  webSite+="      </div>\n";
  webSite+="      <div style='width: 21%; background-color: black; float: left; text-align: right;'>\n";
  webSite+="        <div id=\"battery-switch\" onclick=\"powerSwitch()\" class='scan-button'>\n";
  webSite+="          SKANUJ\n";
  webSite+="        </div>\n";
  webSite+="      </div>\n";
  webSite+="    </div>\n";
  
  webSite+="    <div class='measurement-container' style=\"height: 238px;\">\n";
  webSite+="      <div class='minor-header'>Teoria sterowania</div>\n";
  webSite+="      <div class='reg-left-block'>\n";
  webSite+="        <div class='reg-header'>Balansowanie:</div>\n";
  webSite+="        <p>Wartość P: 0.019</p>\n";
  webSite+="        <p>Wartość I: 0.011</p>\n";
  webSite+="        <p>Wartość D: 0.000</p>\n";
  webSite+="        <p>Nasycenie: 1000</p>\n";
  webSite+="        <p>Wejście: <A id='errbalance'></A></p>\n";
  webSite+="        <p>Wyjście: <A id='pid'></A></p>\n";
  webSite+="      </div>\n";
  webSite+="      <div class='reg-mid-block'>\n";
  webSite+="        <div class='reg-header'>Położenie liniowe:</div>\n";
  webSite+="        <p>Wartość P: 12.000</p>\n";
  webSite+="        <p>Wartość I: 0.000</p>\n";
  webSite+="        <p>Wartość D: 0.000</p>\n";
  webSite+="        <p>Nasycenie: 5000</p>\n";
  webSite+="        <p>Wejście: <A id='errpos'></A></p>\n";
  webSite+="        <p>Wyjście: <A id='pidpos'></A></p>\n";
  webSite+="      </div>\n";
  webSite+="      <div class='reg-right-block'>\n";
  webSite+="        <div class='reg-header'>Sterownik silników:</div>\n";
  webSite+="        <p>Wartość P: 0.800</p>\n";
  webSite+="        <p>Wartość I: 0.000</p>\n";
  webSite+="        <p>Wartość D: 0.000</p>\n";
  webSite+="        <p>Nasycenie: 1000</p>\n";
  webSite+="        <p>Wejście: <A id='errmotor'></A></p>\n";
  webSite+="        <p>Wyjście: <A id='pidmotor'></A></p>\n";
  webSite+="      </div>\n";
  webSite+="      <div style='border-top: 3px solid black; clear: both;'>\n";
  webSite+="        <button style='margin-top: 2px;'>WYŚWIETL WYKRESY</button>\n";
  webSite+="      </div>\n";
  webSite+="    </div>\n";
  
  webSite+="    <div class='measurement-container' style=\"height: 109px;\">\n";
  webSite+="      <div class='minor-header'>Pozycja liniowa: <span style='font-weight: 100;'>(aktualny kierunek: <A id='dir'></A>)</span> </div>\n";
  webSite+="      <div class='measurement-left-block'>\n";
  webSite+="        <p>Kąt lewego koła: <A id='lpos'></A> stopni</p>\n";
  webSite+="        <p>Droga lewego koła: <A id='xl'></A> cm</p>\n";
  webSite+="        <p>Prędkość liniowa wg lewego koła: <A id='vl'></A> km/h</p>\n";
  webSite+="      </div>\n";
  webSite+="      <div class='measurement-right-block'>\n";
  webSite+="        <p>Kąt prawego koła: <A id='rpos'></A> stopni</p>\n";
  webSite+="        <p>Droga prawego koła: <A id='xr'></A> cm</p>\n";
  webSite+="        <p>Prędkość liniowa wg prawego koła: <A id='vr'></A> km/h</p>\n";
  webSite+="      </div>\n";
  webSite+="    </div>\n";
  
  webSite+="    <div class='measurement-container' style=\"height: 85px;\">\n";
  webSite+="      <div class='minor-header'>Pozostałe informacje:</div>\n";
  webSite+="      <div class='measurement-left-block'>\n";
  webSite+="        <p>Napięcie ESP8266: <A id='nap'></A> V</p>\n";
  webSite+="        <p>Czas działania: <A id='runtime'></A></p>\n";
  webSite+="      </div>\n";
  webSite+="      <div class='measurement-right-block'>\n";
  webSite+="        <p>Siła sygnału WiFi: <A id='rssi'></A> dBm</p>\n";
  webSite+="        <div style='height: 29px;'></div>\n";
  webSite+="      </div>\n";
  webSite+="    </div>\n";
  webSite+="  </div>\n";
  
  webSite+="  <div id=\"plotcontainer\" style=\"width:600px; height:664px; float: left; margin-left: 50px; margin-top: 80px;\">\n";
  webSite+="    <div id=\"balanceplot\" style=\"width: 100%; height: 214px; margin-bottom: 10px;\"></div>\n";
  webSite+="    <div id=\"linearplot\" style=\"width: 100%; height: 214px; margin-bottom: 10px;\"></div>\n";
  webSite+="    <div id=\"motorplot\" style=\"width: 100%; height: 214px; margin-bottom: 10px;\"></div>\n";
//  webSite+="    <div id=\"pid-container\" style=\"background-color:DimGray; text-align:center; width:600px; height:170px; float:left;\">\n";
//  webSite+="      <div class='minor-header'>Wartości regulatora PID: </div><br>\n";
//  webSite+="      <div style=\"width:300px; height:100px; margin: 0 auto; text-align:center;\">\n";
//  webSite+="        <div style=\"float:left; padding:3px;\"> P: <input type=\"text\" onkeyup=\"getPID('p',this)\" id=\"p\" size=\"5\" value=\"7\"></div>\n";
//  webSite+="        <div style=\"float:left; padding:3px;\"> I: <input type=\"text\" onkeyup=\"getPID('i',this)\" id=\"i\" size=\"5\" value=\"0.05\"></div>\n";
//  webSite+="        <div style=\"float:left; padding:3px;\"> D: <input type=\"text\" onkeyup=\"getPID('d',this)\" id=\"d\" size=\"5\" value=\"0.5\"></div>\n";
//  webSite+="      </div>\n";
//  webSite+="    </div>\n";
  webSite+="  </div>\n";
  webSite+="  <div class='footer'>\n";
  webSite+="    Mateusz Staszków - Praca inżynierska \"Dwukołowy robot inspekcyjny\", Politechnika Warszawska 2017\n";
  webSite+="  </div>\n";
  webSite+="</BODY>\n";
  webSite+="</HTML>\n";
}

void buildJavascript(){
  javaScript="<SCRIPT>\n";
  javaScript+="var xmlHttp=createXmlHttpObject();\n";
  javaScript+="var xmlHttpOn=createXmlHttpObject();\n";
  javaScript+="var xmlHttpOff=createXmlHttpObject();\n";
  javaScript+="var powerOn = false;\n";
  javaScript+="var busy = false;\n";

  javaScript+="function setPowerButton() {\n";
  javaScript+="  if(powerOn && !busy) {\n";
  javaScript+="    document.getElementById(\"battery-switch\").innerHTML = \"STOP\";\n";
  javaScript+="    document.getElementById(\"battery-switch\").style.background = \"Red\"\n";
  javaScript+="    document.getElementById(\"battery-switch\").style.color = \"White\"\n";
  javaScript+="  }\n";
  javaScript+="  else if(!powerOn && !busy) {\n";
  javaScript+="    document.getElementById(\"battery-switch\").innerHTML = \"SKANUJ\";\n";
  javaScript+="    document.getElementById(\"battery-switch\").style.background = \"Green\"\n";
  javaScript+="    document.getElementById(\"battery-switch\").style.color = \"White\"\n";
  javaScript+="  }\n";
  javaScript+="}\n";

  javaScript+="function powerSwitch() {\n";
  javaScript+="  if(!powerOn && !busy) {\n";
  javaScript+="    busy = true;\n";
  javaScript+="    document.getElementById(\"battery-switch\").style.background = \"DarkOliveGreen\"\n";
  javaScript+="    document.getElementById(\"battery-switch\").style.color = \"Gray\"\n";
  javaScript+="    xmlHttpOn.open('PUT','/on',true);\n";
  javaScript+="    xmlHttpOn.onreadystatechange=handleSwitchOn;\n";
  javaScript+="    xmlHttpOn.send(null);\n";
  javaScript+="  }\n";
  javaScript+="  else if(powerOn && !busy) {\n";
  javaScript+="    busy = true;\n";
  javaScript+="    document.getElementById(\"battery-switch\").style.background = \"Brown\"\n";
  javaScript+="    document.getElementById(\"battery-switch\").style.color = \"Gray\"\n";
  javaScript+="    xmlHttpOff.open('PUT','/off',true);\n";
  javaScript+="    xmlHttpOff.onreadystatechange=handleSwitchOff;\n";
  javaScript+="    xmlHttpOff.send(null);\n";
  javaScript+="  }\n";
  javaScript+="}\n";

  javaScript+="function getPID(regulatorPart, pidValue) {\n";
  javaScript+=" xmlHttpOff.open('PUT','/pid?part=' + regulatorPart + '&value=' + pidValue.value.toString(), true);\n";
  javaScript+=" xmlHttpOff.send(null);\n";
  javaScript+="}\n";

  javaScript+="function createXmlHttpObject(){\n";
  javaScript+=" if(window.XMLHttpRequest){\n";
  javaScript+="    xmlHttp=new XMLHttpRequest();\n";
  javaScript+=" }else{\n";
  javaScript+="    xmlHttp=new ActiveXObject('Microsoft.XMLHTTP');\n";
  javaScript+=" }\n";
  javaScript+=" return xmlHttp;\n";
  javaScript+="}\n";

  javaScript+="function get_firstchild(n) {\n";
  javaScript+="  var x = n.firstChild;\n";
  javaScript+="  while (x.nodeType != 1) {\n";
  javaScript+="      x = x.nextSibling;\n";
  javaScript+="  }\n";
  javaScript+="  return x;\n";
  javaScript+="}\n";

  javaScript+="function process(){\n";
  javaScript+=" setPowerButton();\n";
  javaScript+=" plotMeasurements();\n";
  javaScript+=" if(xmlHttp.readyState==0 || xmlHttp.readyState==4){\n";
  javaScript+="   xmlHttp.open('PUT','xml',true);\n";
  javaScript+="   xmlHttp.onreadystatechange=handleServerResponse;\n";
  javaScript+="   xmlHttp.send(null);\n";
  javaScript+=" }\n";
  javaScript+=" setTimeout('process()',200);\n";
  javaScript+="}\n";

  javaScript+="function handleSwitchOn(){\n";
  javaScript+="  if(xmlHttpOn.readyState==4 && xmlHttpOn.status==200){\n";
  javaScript+="    powerOn = true;\n";
  javaScript+="    document.getElementById(\"battery-switch\").innerHTML = \"STOP\";\n";
  javaScript+="    document.getElementById(\"battery-switch\").style.background = \"Red\"\n";
  javaScript+="    document.getElementById(\"battery-switch\").style.color = \"White\"\n";
  javaScript+="    busy = false;\n";
  javaScript+="  }\n";
  javaScript+="}\n";

  javaScript+="function handleSwitchOff(){\n";
  javaScript+="  if(xmlHttpOff.readyState==4 && xmlHttpOff.status==200){\n";
  javaScript+="    powerOn = false;\n";
  javaScript+="    document.getElementById(\"battery-switch\").innerHTML = \"SKANUJ\";\n";
  javaScript+="    document.getElementById(\"battery-switch\").style.background = \"Green\"\n";
  javaScript+="    document.getElementById(\"battery-switch\").style.color = \"White\"\n";
  javaScript+="    busy = false;\n";
  javaScript+="  }\n";
  javaScript+="}\n";
  
  javaScript+="function handleServerResponse(){\n";
  javaScript+=" if(xmlHttp.readyState==4 && xmlHttp.status==200){\n";
  javaScript+="   xmlResponse=xmlHttp.responseXML;\n";
  
  javaScript+="   xmldoc = xmlResponse.getElementsByTagName('response-time');\n";
  javaScript+="   message = xmldoc[0].firstChild.nodeValue;\n";
  javaScript+="   document.getElementById('runtime').innerHTML=message;\n";

  javaScript+="   xmldoc = xmlResponse.getElementsByTagName('response-fx');\n";
  javaScript+="   message = xmldoc[0].firstChild.nodeValue;\n";
  javaScript+="   document.getElementById('fx').innerHTML=message;\n";

  javaScript+="   xmldoc = xmlResponse.getElementsByTagName('response-fy');\n";
  javaScript+="   message = xmldoc[0].firstChild.nodeValue;\n";
  javaScript+="   document.getElementById('fy').innerHTML=message;\n";

  javaScript+="   xmldoc = xmlResponse.getElementsByTagName('response-fz');\n";
  javaScript+="   message = xmldoc[0].firstChild.nodeValue;\n";
  javaScript+="   document.getElementById('fz').innerHTML=message;\n";

  javaScript+="   xmldoc = xmlResponse.getElementsByTagName('response-ax');\n";
  javaScript+="   message = xmldoc[0].firstChild.nodeValue;\n";
  javaScript+="   document.getElementById('ax').innerHTML=message;\n";

  javaScript+="   xmldoc = xmlResponse.getElementsByTagName('response-ay');\n";
  javaScript+="   message = xmldoc[0].firstChild.nodeValue;\n";
  javaScript+="   document.getElementById('ay').innerHTML=message;\n";

  javaScript+="   xmldoc = xmlResponse.getElementsByTagName('response-az');\n";
  javaScript+="   message = xmldoc[0].firstChild.nodeValue;\n";
  javaScript+="   document.getElementById('az').innerHTML=message;\n";

  javaScript+="   xmldoc = xmlResponse.getElementsByTagName('response-lpos');\n";
  javaScript+="   message = xmldoc[0].firstChild.nodeValue;\n";
  javaScript+="   document.getElementById('lpos').innerHTML=message;\n";

  javaScript+="   xmldoc = xmlResponse.getElementsByTagName('response-rpos');\n";
  javaScript+="   message = xmldoc[0].firstChild.nodeValue;\n";
  javaScript+="   document.getElementById('rpos').innerHTML=message;\n";

  javaScript+="   xmldoc = xmlResponse.getElementsByTagName('response-xl');\n";
  javaScript+="   message = xmldoc[0].firstChild.nodeValue;\n";
  javaScript+="   document.getElementById('xl').innerHTML=message;\n";

  javaScript+="   xmldoc = xmlResponse.getElementsByTagName('response-vl');\n";
  javaScript+="   message = xmldoc[0].firstChild.nodeValue;\n";
  javaScript+="   document.getElementById('vl').innerHTML=message;\n";

  javaScript+="   xmldoc = xmlResponse.getElementsByTagName('response-nap');\n";
  javaScript+="   message = xmldoc[0].firstChild.nodeValue;\n";
  javaScript+="   document.getElementById('nap').innerHTML=message;\n";

  javaScript+="   xmldoc = xmlResponse.getElementsByTagName('response-dir');\n";
  javaScript+="   message = xmldoc[0].firstChild.nodeValue;\n";
  javaScript+="   document.getElementById('dir').innerHTML=message;\n";

  javaScript+="   xmldoc = xmlResponse.getElementsByTagName('response-pid');\n";
  javaScript+="   message = xmldoc[0].firstChild.nodeValue;\n";
  javaScript+="   document.getElementById('pid').innerHTML=message;\n";

  javaScript+="   xmldoc = xmlResponse.getElementsByTagName('response-turn');\n";
  javaScript+="   message = xmldoc[0].firstChild.nodeValue.replace(/\\s+/g, '');\n";
  javaScript+="   if(message === '1') powerOn = true;\n";
  javaScript+="   else if(message === '0') powerOn = false;\n";

  javaScript+="   xmldoc = xmlResponse.getElementsByTagName('response-dist-m');\n";
  javaScript+="   message = xmldoc[0].firstChild.nodeValue;\n";
  javaScript+="   document.getElementById('mdist').innerHTML=message;\n";

  javaScript+="   xmldoc = xmlResponse.getElementsByTagName('response-balance-error');\n";
  javaScript+="   message = xmldoc[0].firstChild.nodeValue;\n";
  javaScript+="   document.getElementById('errbalance').innerHTML=message;\n";

  javaScript+="   xmldoc = xmlResponse.getElementsByTagName('response-pos-error');\n";
  javaScript+="   message = xmldoc[0].firstChild.nodeValue;\n";
  javaScript+="   document.getElementById('errpos').innerHTML=message;\n";

  javaScript+="   xmldoc = xmlResponse.getElementsByTagName('response-pos-pid');\n";
  javaScript+="   message = xmldoc[0].firstChild.nodeValue;\n";
  javaScript+="   document.getElementById('pidpos').innerHTML=message;\n";

  javaScript+="   xmldoc = xmlResponse.getElementsByTagName('response-motor-error');\n";
  javaScript+="   message = xmldoc[0].firstChild.nodeValue;\n";
  javaScript+="   document.getElementById('errmotor').innerHTML=message;\n";

  javaScript+="   xmldoc = xmlResponse.getElementsByTagName('response-motor-pid');\n";
  javaScript+="   message = xmldoc[0].firstChild.nodeValue;\n";
  javaScript+="   document.getElementById('pidmotor').innerHTML=message;\n";

  javaScript+="   xmldoc = xmlResponse.getElementsByTagName('response-xr');\n";
  javaScript+="   message = xmldoc[0].firstChild.nodeValue;\n";
  javaScript+="   document.getElementById('xr').innerHTML=message;\n";

  javaScript+="   xmldoc = xmlResponse.getElementsByTagName('response-vr');\n";
  javaScript+="   message = xmldoc[0].firstChild.nodeValue;\n";
  javaScript+="   document.getElementById('vr').innerHTML=message;\n";

//  javaScript+="   xmldoc = xmlResponse.getElementsByTagName('response-battery');\n";
//  javaScript+="   message = xmldoc[0].firstChild.nodeValue;\n";
//  javaScript+="   document.getElementById('battery').innerHTML=message;\n";

  javaScript+="   xmldoc = xmlResponse.getElementsByTagName('rssi');\n";
  javaScript+="   message = xmldoc[0].firstChild.nodeValue;\n";
  javaScript+="   document.getElementById('rssi').innerHTML=message;\n";
  
  javaScript+=" }\n";
  javaScript+="}\n";
  javaScript+="  var timeUnit = 0;\n";
  javaScript+="  var timeTable = [timeUnit];\n";

  plotJS+="function createBalancePlotMeasurements(){\n";
  plotJS+="  var plot = document.getElementById(\"balanceplot\");\n";
  plotJS+="  var fx = document.getElementById(\"fx\").innerHTML;\n";
  plotJS+="  var ax = document.getElementById(\"ax\").innerHTML;\n";
  plotJS+="  var error = document.getElementById(\"errbalance\").innerHTML;\n";
  plotJS+="  var pid = document.getElementById(\"pid\").innerHTML;\n";
  plotJS+="  var angle = {\n";
  plotJS+="   x: timeTable,\n";
  plotJS+="   y: [fx],\n";
  plotJS+="   name: 'fx',\n";
  plotJS+="   type: \"scatter\"\n";
  plotJS+="  };\n";
  plotJS+="  var angularAcceleration = {\n";
  plotJS+="   x: timeTable,\n";
  plotJS+="   y: [ax],\n";
  plotJS+="   name: 'ax',\n";
  plotJS+="   type: \"scatter\"\n";
  plotJS+="  };\n";
  plotJS+="  var controlInput = {\n";
  plotJS+="   x: timeTable,\n";
  plotJS+="   y: [error],\n";
  plotJS+="   name: 'Uchyb',\n";
  plotJS+="   type: \"scatter\"\n";
  plotJS+="  };\n";
  plotJS+="  var controlOutput = {\n";
  plotJS+="   x: timeTable,\n";
  plotJS+="   y: [pid],\n";
  plotJS+="   name: 'Wyjście',\n";
  plotJS+="   type: \"scatter\"\n";
  plotJS+="  };\n";
  plotJS+="  var layout = {\n";
  plotJS+="    title: 'Wykres wejścia i wyjścia - regulator kąta wychylenia',\n";
  plotJS+="    xaxis: {title: 'Czas', showline: true, mirror: 'allticks', ticks: 'inside'},\n";
  plotJS+="    margin: {l: 40, b: 35, t: 35}\n";
  plotJS+="  }\n";
  plotJS+="  var data = [angle, angularAcceleration, controlInput, controlOutput];\n";
  plotJS+="  Plotly.plot(plot, data, layout);\n";
  plotJS+="}\n";

  plotJS+="function createLinearPlotMeasurements(){\n";
  plotJS+="  var plot = document.getElementById(\"linearplot\");\n";
  plotJS+="  var ax = document.getElementById(\"ax\").innerHTML;\n";
  plotJS+="  var error = document.getElementById(\"errpos\").innerHTML;\n";
  plotJS+="  var pid = document.getElementById(\"pidpos\").innerHTML;\n";
  plotJS+="  var angularAcceleration = {\n";
  plotJS+="   x: timeTable,\n";
  plotJS+="   y: [ax],\n";
  plotJS+="   name: 'ax',\n";
  plotJS+="   type: \"scatter\"\n";
  plotJS+="  };\n";
  plotJS+="  var controlInput = {\n";
  plotJS+="   x: timeTable,\n";
  plotJS+="   y: [error],\n";
  plotJS+="   name: 'Uchyb',\n";
  plotJS+="   type: \"scatter\"\n";
  plotJS+="  };\n";
  plotJS+="  var controlOutput = {\n";
  plotJS+="   x: timeTable,\n";
  plotJS+="   y: [pid],\n";
  plotJS+="   name: 'Wyjście',\n";
  plotJS+="   type: \"scatter\"\n";
  plotJS+="  };\n";
  plotJS+="  var layout = {\n";
  plotJS+="    title: 'Wykres wejścia i wyjścia - regulator prędkości liniowej',\n";
  plotJS+="    xaxis: {title: 'Czas', showline: true, mirror: 'allticks', ticks: 'inside'},\n";
  plotJS+="    margin: {l: 40, b: 35, t: 35}\n";
  plotJS+="  }\n";
  plotJS+="  var data = [angularAcceleration, controlInput, controlOutput];\n";
  plotJS+="  Plotly.plot(plot, data, layout);\n";
  plotJS+="}\n";

  plotJS+="function createMotorPlotMeasurements(){\n";
  plotJS+="  var plot = document.getElementById(\"motorplot\");\n";
  plotJS+="  var xl = document.getElementById(\"xl\").innerHTML;\n";
  plotJS+="  var xr = document.getElementById(\"xr\").innerHTML;\n";
  plotJS+="  var error = document.getElementById(\"errmotor\").innerHTML;\n";
  plotJS+="  var pid = document.getElementById(\"pidmotor\").innerHTML;\n";
  plotJS+="  var leftPosition = {\n";
  plotJS+="   x: timeTable,\n";
  plotJS+="   y: [xl],\n";
  plotJS+="   name: 'left pos',\n";
  plotJS+="   type: \"scatter\"\n";
  plotJS+="  };\n";
  plotJS+="  var rightPosition = {\n";
  plotJS+="   x: timeTable,\n";
  plotJS+="   y: [xr],\n";
  plotJS+="   name: 'right pos',\n";
  plotJS+="   type: \"scatter\"\n";
  plotJS+="  };\n";
  plotJS+="  var controlInput = {\n";
  plotJS+="   x: timeTable,\n";
  plotJS+="   y: [error],\n";
  plotJS+="   name: 'Uchyb',\n";
  plotJS+="   type: \"scatter\"\n";
  plotJS+="  };\n";
  plotJS+="  var controlOutput = {\n";
  plotJS+="   x: timeTable,\n";
  plotJS+="   y: [pid],\n";
  plotJS+="   name: 'Wyjście',\n";
  plotJS+="   type: \"scatter\"\n";
  plotJS+="  };\n";
  plotJS+="  var layout = {\n";
  plotJS+="    title: 'Wykres wejścia i wyjścia - regulator silników',\n";
  plotJS+="    xaxis: {title: 'Czas', showline: true, mirror: 'allticks', ticks: 'inside'},\n";
  plotJS+="    margin: {l: 40, b: 35, t: 35}\n";
  plotJS+="  }\n";
  plotJS+="  var data = [leftPosition, rightPosition, controlInput, controlOutput];\n";
  plotJS+="  Plotly.plot(plot, data, layout);\n";
  plotJS+="}\n";

  plotJS+="function start(){\n";
  plotJS+="  createBalancePlotMeasurements();\n";
  plotJS+="  createLinearPlotMeasurements();\n";
  plotJS+="  createMotorPlotMeasurements();\n";
  plotJS+="  process();\n";
  plotJS+="}\n";

  plotJS+="function plotBalanceMeasurements(){\n";
  plotJS+="  var plot = document.getElementById(\"balanceplot\");\n";
  plotJS+="  var fx = document.getElementById(\"fx\").innerHTML;\n";
  plotJS+="  var ax = document.getElementById(\"ax\").innerHTML;\n";
  plotJS+="  var error = document.getElementById(\"errbalance\").innerHTML;\n";
  plotJS+="  var pid = document.getElementById(\"pid\").innerHTML;\n";
  plotJS+="  var dataY = [[fx], [ax], [error], [pid]];\n";
  plotJS+="  var dataX = [timeTable, timeTable, timeTable, timeTable];\n";
  plotJS+="  var extendedPlot = {\n";
  plotJS+="   x: dataX,\n";
  plotJS+="   y: dataY,\n";
  plotJS+="  };\n";
  plotJS+="  Plotly.extendTraces(plot, extendedPlot, [0, 1, 2, 3]);\n";
  plotJS+="}\n";

  plotJS+="function plotLinearMeasurements(){\n";
  plotJS+="  var plot = document.getElementById(\"linearplot\");\n";
  plotJS+="  var ax = document.getElementById(\"ax\").innerHTML;\n";
  plotJS+="  var error = document.getElementById(\"errpos\").innerHTML;\n";
  plotJS+="  var pid = document.getElementById(\"pidpos\").innerHTML;\n";
  plotJS+="  var dataY = [[ax], [error], [pid]];\n";
  plotJS+="  var dataX = [timeTable, timeTable, timeTable];\n";
  plotJS+="  var extendedPlot = {\n";
  plotJS+="   x: dataX,\n";
  plotJS+="   y: dataY,\n";
  plotJS+="  };\n";
  plotJS+="  Plotly.extendTraces(plot, extendedPlot, [0, 1, 2]);\n";
  plotJS+="}\n";

  plotJS+="function plotMotorMeasurements(){\n";
  plotJS+="  var plot = document.getElementById(\"motorplot\");\n";
  plotJS+="  var xl = document.getElementById(\"xl\").innerHTML;\n";
  plotJS+="  var xr = document.getElementById(\"xr\").innerHTML;\n";
  plotJS+="  var error = document.getElementById(\"errmotor\").innerHTML;\n";
  plotJS+="  pid = document.getElementById(\"pidmotor\").innerHTML;\n";
  plotJS+="  var dataY = [[xl], [xr], [error], [pid]];\n";
  plotJS+="  var dataX = [timeTable, timeTable, timeTable, timeTable];\n";
  plotJS+="  var extendedPlot = {\n";
  plotJS+="   x: dataX,\n";
  plotJS+="   y: dataY,\n";
  plotJS+="  };\n";
  plotJS+="  Plotly.extendTraces(plot, extendedPlot, [0, 1, 2, 3]);\n";
  plotJS+="}\n";
  
  plotJS+="function plotMeasurements(){\n";
  plotJS+="  timeUnit += 0.2;\n";
  plotJS+="  timeTable = [timeUnit];\n";
//  plotJS+="  plotBalanceMeasurements();\n";
//  plotJS+="  plotLinearMeasurements();\n";
//  plotJS+="  plotMotorMeasurements();\n";
  plotJS+="}\n";
  
  plotJS+="</SCRIPT>\n";
}

void buildXML(){  
  String move_direction = "";
  if(measuredData.dir == 1) move_direction = "przód";
  else if(measuredData.dir == 0) move_direction = "tył";

  float voltage = (float)ESP.getVcc()/19859.0;
  int voltage_high = voltage;
  int voltage_low = voltage*1000 - voltage_high*1000;
  long rssi = WiFi.RSSI();
  
  XML ="<?xml version='1.0'?>";
  XML+="  <Data>"; 
  XML+="    <response-time>";
  XML+=       millis2time();
  XML+="    </response-time>";
  XML+="    <response-fx>";
  XML+=       (String)measuredData.fx;
  XML+="    </response-fx>";
  XML+="    <response-fy>";
  XML+=       (String)measuredData.fy;
  XML+="    </response-fy>";
  XML+="    <response-fz>";
  XML+=       (String)measuredData.fz;
  XML+="    </response-fz>";
  XML+="    <response-ax>";
  XML+=       (String)measuredData.ax;
  XML+="    </response-ax>";
  XML+="    <response-ay>";
  XML+=       (String)measuredData.ay;
  XML+="    </response-ay>";
  XML+="    <response-az>";
  XML+=       (String)measuredData.az;
  XML+="    </response-az>";
  XML+="    <response-lpos>";
  XML+=       (String)measuredData.l_pos;
  XML+="    </response-lpos>";
  XML+="    <response-rpos>";
  XML+=       (String)measuredData.r_pos;
  XML+="    </response-rpos>";
  XML+="    <response-xl>";
  XML+=       (String)measuredData.xl;
  XML+="    </response-xl>";
  XML+="    <response-vl>";
  XML+=       (String)measuredData.vl;
  XML+="    </response-vl>";
  XML+="    <response-pid>";
  XML+=       (String)measuredData.pid;
  XML+="    </response-pid>";
  XML+="    <response-nap>";
  XML+=       (String)voltage;
  XML+="    </response-nap>";
  XML+="    <response-dir>";
  XML+=       move_direction;
  XML+="    </response-dir>";
  XML+="    <response-turn>";
  XML+=       (String)measuredData.turn;
  XML+="    </response-turn>";
  XML+="    <response-dist-m>";
  XML+=       (String)measuredData.dist_m;
  XML+="    </response-dist-m>";
  XML+="    <response-balance-error>";
  XML+=       (String)measuredData.balance_error;
  XML+="    </response-balance-error>";
  XML+="    <response-pos-error>";
  XML+=       (String)measuredData.pos_error;
  XML+="    </response-pos-error>";
  XML+="    <response-pos-pid>";
  XML+=       (String)measuredData.pos_pid;
  XML+="    </response-pos-pid>";
  XML+="    <response-motor-error>";
  XML+=       (String)measuredData.motor_error;
  XML+="    </response-motor-error>";
  XML+="    <response-motor-pid>";
  XML+=       (String)measuredData.motor_pid;
  XML+="    </response-motor-pid>";
  XML+="    <response-xr>";
  XML+=       (String)measuredData.xr;
  XML+="    </response-xr>";
  XML+="    <response-vr>";
  XML+=       (String)measuredData.vr;
  XML+="    </response-vr>";
  XML+="    <response-battery>";
  XML+=       (String)measuredData.battery_state;
  XML+="    </response-battery>";
  XML+="    <rssi>";
  XML+=       (String)rssi;
  XML+="    </rssi>";
  XML+="  </Data>"; 
}

String millis2time(){
  String Time="";
  unsigned long ss;
  byte mm,hh;
  ss=millis()/1000;
  hh=ss/3600;
  mm=(ss-hh*3600)/60;
  ss=(ss-hh*3600)-mm*60;
  if(hh<10)Time+="0";
  Time+=(String)hh+":";
  if(mm<10)Time+="0";
  Time+=(String)mm+":";
  if(ss<10)Time+="0";
  Time+=(String)ss;
  return Time;
}

void handleWebsite(){
  buildWebsite();
  server.send(200,"text/html",webSite);
  Serial.println(webSite);
}

void handleXML(){
  buildXML();
  server.send(200,"text/xml",XML);
}

void handleOn() {
  Serial.println("n;");
  server.send(200,"text/xml","<battery>on</battery>");
}

void handleOff() {
  Serial.println("f;");
  server.send(200,"text/xml","<battery>off</battery>");
}

void handlePID() {
  String message = "";
  String part = server.arg("part");
  String value = server.arg("value");
  if ((part != "") && (value != "")) {
    message += part;
    value = (String)value.toFloat();
    if(value == "") value = "0";
    message += (value + ";");
  }
  Serial.println(message);
  server.send(200, "text/plain", "ok"); 
}

void serialPrintWithArg(char* preCharTab, const char* postCharTab) {
  char result[100];
  strcpy(result, preCharTab);
  strcat(result, postCharTab);
  Serial.println(result);
}

void setup() {  
  Serial.begin(115200);  

  while(WiFi.status() != WL_CONNECTED) {
    int tryCount = 0;
    Serial.println("\n\nBOOTING ESP8266 ...");
  
    serialPrintWithArg("Trying to connect to SSID: ", ssid);
    WiFi.begin(ssid, password);
    while((WiFi.status() != WL_CONNECTED) && (tryCount != 20)) {
      delay(500);
      tryCount++;
    }
    
    if(tryCount == 20) {
      WiFi.disconnect();
      serialPrintWithArg("Could not connect to SSID: ", ssid);
      IPAddress ip(192, 168, 43, 90);
      IPAddress gateway(192, 168, 43, 1);
      IPAddress subnet(255, 255, 255, 0);
      WiFi.config(ip, gateway, subnet);
      serialPrintWithArg("Trying to connect to SSID: ", ssidTel);
      WiFi.begin(ssidTel, passwordTel);
      tryCount = 0;
      while((WiFi.status() != WL_CONNECTED) && (tryCount != 20)) {
        delay(500);
        tryCount++;
      }
    }
    if(tryCount == 20) {
      serialPrintWithArg("Could not connect to SSID: ", ssidTel);
    }
  }

  WiFi.mode(WIFI_STA);
  Serial.print("Connected to SSID: ");
  Serial.println(WiFi.SSID());
  Serial.print("Robot IP address: ");
  Serial.println(WiFi.localIP());
  
  server.on("/",handleWebsite);
  server.on("/xml",handleXML);
  server.on("/on",handleOn);
  server.on("/off",handleOff);
  server.on("/pid",handlePID);
  server.begin();  
}

void loop() {
  read_uart();
  server.handleClient();
}
