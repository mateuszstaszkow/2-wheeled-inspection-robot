#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

typedef struct MeasuredData {
  int fx;
  int fy;
  int fz;
  int ax;
  int ay;
  int az;
  int l_pos;
  int r_pos;
  int x;
  float v;
  int dir;
  int pid;
  int battery_state;
};

ESP8266WebServer server(80);
const char* ssid = "UPC1171078";
const char* password = "FMYEJWUS";
//const char* ssid="FunBox2-3C76";
//const char* password="6DFC195A7195577F427D44FDEC";
const char* ssidTel = "internet";
const char* passwordTel = "haslohaslo";

String webSite, javaScript, XML;
MeasuredData measuredData;
char buffer_uart[100];
int powerPin = 2; // GPIO2 of ESP8266

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
          m.x = atoi(number);
          break;
        case 9:
          m.v = atoi(number)*0.036;
          break;
        case 10:
          m.dir = atoi(number);
          break;
        case 11:
          m.pid = atoi(number);
          break;
        case 12:
          m.battery_state = atoi(number);
          break;
      }
      n++;
      i2 = i+1;
    }
  }
}

void read_uart() {
//  measuredData.fx = 0;
//  measuredData.fy = 0;
//  measuredData.fz = 0;
//  measuredData.ax = 0;
//  measuredData.ay = 0;
//  measuredData.az = 0;
//  measuredData.l_pos = 0;
//  measuredData.r_pos = 0;
//  measuredData.x = 0;  
//  measuredData.v = 0;
//  measuredData.dir = 1;
  
  int ii;
  for(ii=0;ii<100;ii++) {
    buffer_uart[ii]=';';
  }
    
  char tmp;
  Serial.readBytesUntil(';',buffer_uart,100);
  
  parseData(measuredData, buffer_uart);
}

void buildWebsite(){
  buildJavascript();
  webSite="<!DOCTYPE HTML>\n";
  webSite+=" <head>\n";
  webSite+="  <title>Dwukołowy robot inspekcyjny</title>\n";
  webSite+="  <meta charset=\"utf-8\">\n";
  webSite+="  <style>\n";
  webSite+="    body { background-color: #000; font-family: Calibri; Color: #fff; text-align: center; }\n";
  webSite+="  </style>\n";
  webSite+="  <script src=\"https://cdn.plot.ly/plotly-latest.min.js\"></script>\n";
  webSite+="</head>\n";
  webSite+=javaScript;
  webSite+="<BODY onload='createPlotMeasurements()'>\n";
  webSite+="  <div style=\"width: 800px; float: left\">\n";
  webSite+="    <h1>Dwukołowy robot inspekcyjny</h1>\n";
  webSite+="    <div style=\"background-color: gray\">\n";
  webSite+="      <h2>Pozycja kątowa:</h2>\n";
  webSite+="      <p>X: <A id='fx'></A>,</p>\n";
  webSite+="      <p>Y: <A id='fy'></A>,</p>\n";
  webSite+="      <p>Z: <A id='fz'></A>,</p>\n";
  webSite+="    </div>\n";
  webSite+="    <div style=\"background-color: DimGray\">\n";
  webSite+="      <h2>Przyspieszenie kątowe poszczególnych osi:</h2>\n";
  webSite+="      <p>X: <A id='ax'></A> 1/s2,</p>\n";
  webSite+="      <p>Y: <A id='ay'></A> 1/s2,</p>\n";
  webSite+="      <p>Z: <A id='az'></A> 1/s2,</p>\n";
  webSite+="    </div>\n";
  webSite+="    <div style=\"background-color: gray\">\n";
  webSite+="      <h2>Pozycja liniowa: </h2>\n";
  webSite+="      <p>Położenie lewego koła: <A id='lpos'></A>,</p>\n";
  webSite+="      <p>Położenie prawego koła: <A id='rpos'></A>,</p>\n";
  webSite+="      <p>Przesunięcie robota: <A id='x'></A> cm,</p>\n";
  webSite+="      <p>Prędkość liniowa robota: <A id='v'></A> km/h,</p>\n";
  webSite+="      <p>Aktualny kierunek poruszania się robota: <A id='dir'></A>,</p>\n";
  webSite+="      <p>Wyjście sterowania: <A id='pid'></A>,</p>\n";
  webSite+="    </div>\n";
  webSite+="    <div style=\"background-color: DimGray\">\n";
  webSite+="      <h2>Pozostałe informacje:</h2>\n";
  webSite+="      <p>Napięcie ESP8266: <A id='nap'></A> V</p>\n";
  webSite+="      <p>Czas działania: <A id='runtime'></A></p>\n";
  webSite+="      <p>Stan baterii: <A id='battery'></A> %</p>\n";
  webSite+="      <p>Siła sygnału WiFi: <A id='rssi'></A> dBm</p>\n";
  webSite+="    </div>\n";
  webSite+="  </div>\n";
  webSite+="  <div id=\"plotcontainer\" style=\"width:600px; height:800px; float: left; padding: 60px;\">\n";
  webSite+="    <h2>Wykres wejścia sterowania do wyjścia: </h2>\n";
  webSite+="    <div id=\"pidplot\" style=\"width:600px; height:400px;\"></div>\n";
  webSite+="    <div id=\"battery-switch-container\" style=\"width:600px; height:132px;\">\n";
  webSite+="      <div id=\"battery-switch\" onclick=\"powerSwitch()\" style=\"background-color: green; border: none; color: white; padding: 30px 50px; text-align: center; text-decoration: none; display: inline-block; font-size: 25px; margin: 20px 2px; cursor: pointer;\">\n";
  webSite+="        START\n";
  webSite+="      </div>\n";
  webSite+="    </div>\n";
  webSite+="    <div id=\"pid-container\" style=\"background-color:DimGray; text-align:center; width:600px; height:170px; float:left;\">\n";
  webSite+="      <h2>Wartości regulatora PID: </h2><br>\n";
  webSite+="      <div style=\"width:300px; height:100px; margin: 0 auto; text-align:center;\">\n";
  webSite+="        <div style=\"float:left; padding:3px;\"> P: <input type=\"text\" onkeyup=\"getPID('p',this)\" id=\"p\" size=\"5\" value=\"7\"></div>\n";
  webSite+="        <div style=\"float:left; padding:3px;\"> I: <input type=\"text\" onkeyup=\"getPID('i',this)\" id=\"i\" size=\"5\" value=\"0.05\"></div>\n";
  webSite+="        <div style=\"float:left; padding:3px;\"> D: <input type=\"text\" onkeyup=\"getPID('d',this)\" id=\"d\" size=\"5\" value=\"0.5\"></div>\n";
  webSite+="      </div>\n";
  webSite+="    </div>\n";
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
//  javaScript+=" var message = "";\n";
//  javaScript+=" switch(regulatorPart) {\n";
//  javaScript+="   case 'p':\n";
//  javaScript+="     message += (\"p\" + pidValue);\n";
//  javaScript+="     break;\n";
//  javaScript+="   case 'i':\n";
//  javaScript+="     message += (\"i\" + pidValue);\n";
//  javaScript+="     break;\n";
//  javaScript+="   case 'd':\n";
//  javaScript+="     message += (\"d\" + pidValue);\n";
//  javaScript+="     break;\n";
//  javaScript+=" }\n";
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
  javaScript+="    document.getElementById(\"battery-switch\").innerHTML = \"START\";\n";
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

  javaScript+="   xmldoc = xmlResponse.getElementsByTagName('response-x');\n";
  javaScript+="   message = xmldoc[0].firstChild.nodeValue;\n";
  javaScript+="   document.getElementById('x').innerHTML=message;\n";

  javaScript+="   xmldoc = xmlResponse.getElementsByTagName('response-v');\n";
  javaScript+="   message = xmldoc[0].firstChild.nodeValue;\n";
  javaScript+="   document.getElementById('v').innerHTML=message;\n";

  javaScript+="   xmldoc = xmlResponse.getElementsByTagName('response-nap');\n";
  javaScript+="   message = xmldoc[0].firstChild.nodeValue;\n";
  javaScript+="   document.getElementById('nap').innerHTML=message;\n";

  javaScript+="   xmldoc = xmlResponse.getElementsByTagName('response-dir');\n";
  javaScript+="   message = xmldoc[0].firstChild.nodeValue;\n";
  javaScript+="   document.getElementById('dir').innerHTML=message;\n";

  javaScript+="   xmldoc = xmlResponse.getElementsByTagName('response-pid');\n";
  javaScript+="   message = xmldoc[0].firstChild.nodeValue;\n";
  javaScript+="   document.getElementById('pid').innerHTML=message;\n";

  javaScript+="   xmldoc = xmlResponse.getElementsByTagName('response-battery');\n";
  javaScript+="   message = xmldoc[0].firstChild.nodeValue;\n";
  javaScript+="   document.getElementById('battery').innerHTML=message;\n";

  javaScript+="   xmldoc = xmlResponse.getElementsByTagName('rssi');\n";
  javaScript+="   message = xmldoc[0].firstChild.nodeValue;\n";
  javaScript+="   document.getElementById('rssi').innerHTML=message;\n";
  
  javaScript+=" }\n";
  javaScript+="}\n";
  javaScript+="  var timeUnit = 0;\n";

  javaScript+="function createPlotMeasurements(){\n";
  javaScript+="  plot = document.getElementById(\"pidplot\");\n";
  javaScript+="  fx = document.getElementById(\"fx\").innerHTML;\n";
  javaScript+="  ax = document.getElementById(\"ax\").innerHTML;\n";
  javaScript+="  pid = document.getElementById(\"pid\").innerHTML;\n";
  javaScript+="  timeUnit += 0.2;\n";
  javaScript+="  var angle = {\n";
  javaScript+="  x: [timeUnit],\n";
  javaScript+="  y: [fx],\n";
  javaScript+="  name: 'Wejscie',\n";
  javaScript+="  type: \"scatter\"\n";
  javaScript+="  };\n";
  javaScript+="  var angularAcceleration = {\n";
  javaScript+="  x: [timeUnit],\n";
  javaScript+="  y: [ax],\n";
  javaScript+="  name: 'ax',\n";
  javaScript+="  type: \"scatter\"\n";
  javaScript+="  };\n";
  javaScript+="  var controlOutput = {\n";
  javaScript+="  x: [timeUnit],\n";
  javaScript+="  y: [pid],\n";
  javaScript+="  name: 'Wyjscie',\n";
  javaScript+="  type: \"scatter\"\n";
  javaScript+="  };\n";
  javaScript+="  var data = [angle, controlOutput, angularAcceleration];\n";
  javaScript+="  Plotly.plot(plot, data);\n";
  javaScript+="\n";
  javaScript+="  process();\n";
  javaScript+="}\n";
  
  javaScript+="function plotMeasurements(){\n";
  javaScript+="  plot = document.getElementById(\"pidplot\");\n";
  javaScript+="  fx = document.getElementById(\"fx\").innerHTML;\n";
  javaScript+="  ax = document.getElementById(\"ax\").innerHTML;\n";
  javaScript+="  pid = document.getElementById(\"pid\").innerHTML;\n";
  javaScript+="  timeUnit += 0.2;\n";
  javaScript+="  var dataY = [[fx], [pid], [ax]];\n";
  javaScript+="  var dataX = [[timeUnit], [timeUnit], [timeUnit]];\n";
  javaScript+="  var extendedPlot = {\n";
  javaScript+="  x: dataX,\n";
  javaScript+="  y: dataY,\n";
  javaScript+="  };\n";
  javaScript+="  Plotly.extendTraces(plot, extendedPlot, [0, 1, 2]);\n";
  javaScript+="}\n";
  
  javaScript+="</SCRIPT>\n";
}

void buildXML(){  
  String move_direction;
  if(measuredData.dir == 1) move_direction = "przód";
  else if(measuredData.dir == 0) move_direction = "tył";

  float voltage = (float)ESP.getVcc()/19859.0;
  int voltage_high = voltage;
  int voltage_low = voltage*1000 - voltage_high*1000;
  long rssi = WiFi.RSSI();
  
  XML ="<?xml version='1.0'?>";
  XML+="  <Donnees>"; 
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
  XML+="    <response-x>";
  XML+=       (String)measuredData.x;
  XML+="    </response-x>";
  XML+="    <response-v>";
  XML+=       (String)measuredData.v;
  XML+="    </response-v>";
  XML+="    <response-pid>";
  XML+=       (String)measuredData.pid;
  XML+="    </response-pid>";
  XML+="    <response-nap>";
  XML+=       (String)voltage;
  XML+="    </response-nap>";
  XML+="    <response-dir>";
  XML+=       move_direction;
  XML+="    </response-dir>";
  XML+="    <response-battery>";
  XML+=       (String)measuredData.battery_state;
  XML+="    </response-battery>";
  XML+="    <rssi>";
  XML+=       (String)rssi;
  XML+="    </rssi>";
  XML+="  </Donnees>"; 
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
}

void handleXML(){
  buildXML();
  server.send(200,"text/xml",XML);
}

void handleOn() {
  digitalWrite(powerPin, HIGH);
  Serial.println("n;");
  server.send(200,"text/xml","<battery>on</battery>");
}

void handleOff() {
  digitalWrite(powerPin, LOW);
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
  pinMode(powerPin, OUTPUT);
  digitalWrite(powerPin, LOW);
  
  Serial.begin(115200);  
  IPAddress ip(192, 168, 43, 90);
  IPAddress gateway(192, 168, 43, 1);
  IPAddress subnet(255, 255, 255, 0);
  WiFi.config(ip, gateway, subnet);

  int tryCount;
  Serial.println("\n\nBOOTING ESP8266 ...");

  serialPrintWithArg("Trying to connect to SSID: ", ssid);
  WiFi.begin(ssid, password);
  for(tryCount = 0; tryCount < 1000; tryCount++) {
    if(WiFi.status() != WL_CONNECTED) delay(500);
  }
  if(tryCount == 1000) {
    WiFi.disconnect();
    serialPrintWithArg("Could not connect to SSID: ", ssid);
    serialPrintWithArg("Trying to connect to SSID: ", ssidTel);
    WiFi.begin(ssidTel, passwordTel);
    for(tryCount = 0; tryCount < 1000; tryCount++) {
      if(WiFi.status() != WL_CONNECTED) delay(500);
    }
  }
  if(tryCount == 1000) serialPrintWithArg("Could not connect to SSID: ", ssidTel);

  while(WiFi.status() != WL_CONNECTED) delay(500);
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
