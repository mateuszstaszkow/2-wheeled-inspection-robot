#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

#define USART_BUFFER_SIZE 200
#define MAX_CONNECTION_TRY_COUNT 20
#define MOBILE_IP_ADDRESS 90
#define CMPS_TO_KMPH 0.036
#define HCSR_CONSTANT 0.017

// minification online: https://kangax.github.io/html-minifier/
const char webSite[] PROGMEM = {"<!DOCTYPE html><title>Dwukołowy robot inspekcyjny</title><meta charset=UTF-8><style>.content-container{width:800px;float:left}.minor-header{border-bottom:3px solid #000;font-size:1.5em;font-weight:700}.reg-header{margin-top:3px;font-size:1.1em;font-weight:700}.header-info{font-weight:100}.measurement-left-block{width:49.75%;float:left;border-right:2px solid #000}.measurement-right-block{width:49.75%;float:left;border-left:2px solid #000}.reg-left-block{width:33%;float:left;border-right:2px solid #000}.reg-mid-block{width:33%;float:left;border-right:2px solid #000;border-left:2px solid #000}.reg-right-block{width:33%;float:left;border-left:2px solid #000}.measurement-container{clear:both;margin-bottom:15px;background-color:#696969}.measurement-container:hover{background-color:gray}.measurement-footer{border-top:3px solid #000;clear:both}.scan-button{background-color:green;border:none;color:#fff;padding:15px 40px;text-align:center;text-decoration:none;display:inline-block;font-size:25px;cursor:pointer}.scan-left-block{width:79%;float:left}.scan-right-block{width:21%;background-color:#000;float:left;text-align:right}.footer{position:fixed;right:0;bottom:0;left:0;padding:1rem;background-color:#111;text-align:center;z-index:1000}.plots-container{width:600px;height:664px;float:left;margin-left:30px;margin-top:82px}.plot-panel{width:100%;height:214px;margin-bottom:10px}.scan-container{width:600px;height:664px;float:left;margin-left:30px;margin-top:82px;position:relative}body{background-color:#000;font-family:Calibri;color:#fff;text-align:center}p{margin-top:5px;margin-bottom:5px}button{margin-top:2px;margin-bottom:3px;font-size:11px;font-weight:700;padding:4px;padding-left:15px;padding-right:15px;cursor:pointer}</style><script src=https://cdn.plot.ly/plotly-latest.min.js></script><script>function showMap(){document.getElementById(\"plotcontainer\").style.display=\"none\",document.getElementById(\"scancontainer\").style.display=\"block\"}function showPlots(){document.getElementById(\"plotcontainer\").style.display=\"block\",document.getElementById(\"scancontainer\").style.display=\"none\"}function setButtonStop(){document.getElementById(\"battery-switch\").innerHTML=\"STOP\",document.getElementById(\"battery-switch\").style.background=\"Red\",document.getElementById(\"battery-switch\").style.color=\"White\"}function setButtonScan(){document.getElementById(\"battery-switch\").innerHTML=\"SKANUJ\",document.getElementById(\"battery-switch\").style.background=\"Green\",document.getElementById(\"battery-switch\").style.color=\"White\"}function setPowerButton(){powerOn&&!busy?setButtonStop():powerOn||busy||setButtonScan()}function powerSwitch(){powerOn||busy?powerOn&&!busy&&(busy=!0,document.getElementById(\"battery-switch\").style.background=\"Brown\",document.getElementById(\"battery-switch\").style.color=\"Gray\",xmlHttpOff.open(\"PUT\",\"/off\",!0),xmlHttpOff.onreadystatechange=handleSwitchOff,xmlHttpOff.send(null)):(busy=!0,document.getElementById(\"battery-switch\").style.background=\"DarkOliveGreen\",document.getElementById(\"battery-switch\").style.color=\"Gray\",xmlHttpOn.open(\"PUT\",\"/on\",!0),xmlHttpOn.onreadystatechange=handleSwitchOn,xmlHttpOn.send(null))}function getPID(e,t){xmlHttpOff.open(\"PUT\",\"/pid?part=\"+e+\"&value=\"+t.value.toString(),!0),xmlHttpOff.send(null)}function createXmlHttpObject(){return xmlHttp=window.XMLHttpRequest?new XMLHttpRequest:new ActiveXObject(\"Microsoft.XMLHTTP\")}function get_firstchild(e){for(var t=e.firstChild;1!=t.nodeType;)t=t.nextSibling;return t}function process(){setPowerButton(),plotMeasurements(),0!=xmlHttp.readyState&&4!=xmlHttp.readyState||(xmlHttp.open(\"PUT\",\"xml\",!0),xmlHttp.onreadystatechange=handleServerResponse,xmlHttp.send(null)),setTimeout(\"process()\",200)}function handleSwitchOn(){4==xmlHttpOn.readyState&&200==xmlHttpOn.status&&(powerOn=!0,setButtonStop(),showMap(),busy=!1)}function handleSwitchOff(){4==xmlHttpOff.readyState&&200==xmlHttpOff.status&&(powerOn=!1,setButtonScan(),busy=!1)}function handleServerResponse(){4==xmlHttp.readyState&&200==xmlHttp.status&&(xmlResponse=xmlHttp.responseXML,xmldoc=xmlResponse.getElementsByTagName(\"response-time\"),message=xmldoc[0].firstChild.nodeValue,document.getElementById(\"runtime\").innerHTML=message,xmldoc=xmlResponse.getElementsByTagName(\"response-fx\"),message=xmldoc[0].firstChild.nodeValue,document.getElementById(\"fx\").innerHTML=message,xmldoc=xmlResponse.getElementsByTagName(\"response-fy\"),message=xmldoc[0].firstChild.nodeValue,document.getElementById(\"fy\").innerHTML=message,xmldoc=xmlResponse.getElementsByTagName(\"response-fz\"),message=xmldoc[0].firstChild.nodeValue,document.getElementById(\"fz\").innerHTML=message,xmldoc=xmlResponse.getElementsByTagName(\"response-ax\"),message=xmldoc[0].firstChild.nodeValue,document.getElementById(\"ax\").innerHTML=message,xmldoc=xmlResponse.getElementsByTagName(\"response-ay\"),message=xmldoc[0].firstChild.nodeValue,document.getElementById(\"ay\").innerHTML=message,xmldoc=xmlResponse.getElementsByTagName(\"response-az\"),message=xmldoc[0].firstChild.nodeValue,document.getElementById(\"az\").innerHTML=message,xmldoc=xmlResponse.getElementsByTagName(\"response-lpos\"),message=xmldoc[0].firstChild.nodeValue,document.getElementById(\"lpos\").innerHTML=message,xmldoc=xmlResponse.getElementsByTagName(\"response-rpos\"),message=xmldoc[0].firstChild.nodeValue,document.getElementById(\"rpos\").innerHTML=message,xmldoc=xmlResponse.getElementsByTagName(\"response-xl\"),message=xmldoc[0].firstChild.nodeValue,document.getElementById(\"xl\").innerHTML=message,xmldoc=xmlResponse.getElementsByTagName(\"response-vl\"),message=xmldoc[0].firstChild.nodeValue,document.getElementById(\"vl\").innerHTML=message,xmldoc=xmlResponse.getElementsByTagName(\"response-nap\"),message=xmldoc[0].firstChild.nodeValue,document.getElementById(\"nap\").innerHTML=message,xmldoc=xmlResponse.getElementsByTagName(\"response-dir\"),message=xmldoc[0].firstChild.nodeValue,document.getElementById(\"dir\").innerHTML=message,xmldoc=xmlResponse.getElementsByTagName(\"response-pid\"),message=xmldoc[0].firstChild.nodeValue,document.getElementById(\"pid\").innerHTML=message,xmldoc=xmlResponse.getElementsByTagName(\"response-turn\"),message=xmldoc[0].firstChild.nodeValue.replace(/\s+/g,\"\"),\"1\"===message?powerOn=!0:\"0\"===message&&(powerOn=!1),xmldoc=xmlResponse.getElementsByTagName(\"response-dist-m\"),message=xmldoc[0].firstChild.nodeValue,document.getElementById(\"mdist\").innerHTML=message,xmldoc=xmlResponse.getElementsByTagName(\"response-balance-error\"),message=xmldoc[0].firstChild.nodeValue,document.getElementById(\"errbalance\").innerHTML=message,xmldoc=xmlResponse.getElementsByTagName(\"response-pos-error\"),message=xmldoc[0].firstChild.nodeValue,document.getElementById(\"errpos\").innerHTML=message,xmldoc=xmlResponse.getElementsByTagName(\"response-pos-pid\"),message=xmldoc[0].firstChild.nodeValue,document.getElementById(\"pidpos\").innerHTML=message,xmldoc=xmlResponse.getElementsByTagName(\"response-motor-error\"),message=xmldoc[0].firstChild.nodeValue,document.getElementById(\"errmotor\").innerHTML=message,xmldoc=xmlResponse.getElementsByTagName(\"response-motor-pid\"),message=xmldoc[0].firstChild.nodeValue,document.getElementById(\"pidmotor\").innerHTML=message,xmldoc=xmlResponse.getElementsByTagName(\"response-xr\"),message=xmldoc[0].firstChild.nodeValue,document.getElementById(\"xr\").innerHTML=message,xmldoc=xmlResponse.getElementsByTagName(\"response-vr\"),message=xmldoc[0].firstChild.nodeValue,document.getElementById(\"vr\").innerHTML=message,xmldoc=xmlResponse.getElementsByTagName(\"response-dfz\"),message=xmldoc[0].firstChild.nodeValue,dfz=message,xmldoc=xmlResponse.getElementsByTagName(\"rssi\"),message=xmldoc[0].firstChild.nodeValue,document.getElementById(\"rssi\").innerHTML=message)}function createBalancePlotMeasurements(){var e=document.getElementById(\"balanceplot\"),t=document.getElementById(\"fx\").innerHTML,n=document.getElementById(\"ax\").innerHTML,l=document.getElementById(\"errbalance\").innerHTML,s=document.getElementById(\"pid\").innerHTML,m={title:\"Wykres wejścia i wyjścia - regulator kąta wychylenia\",xaxis:{title:\"Czas\",showline:!0,mirror:\"allticks\",ticks:\"inside\"},margin:{l:40,b:35,t:35}},o=[{x:timeTable,y:[t],name:\"fx\",type:\"scatter\"},{x:timeTable,y:[n],name:\"ax\",type:\"scatter\"},{x:timeTable,y:[l],name:\"Uchyb/100\",type:\"scatter\"},{x:timeTable,y:[s],name:\"Wyjście\",type:\"scatter\"}];Plotly.plot(e,o,m)}function createLinearPlotMeasurements(){var e=document.getElementById(\"linearplot\"),t=document.getElementById(\"ax\").innerHTML,n=document.getElementById(\"errpos\").innerHTML,l=document.getElementById(\"pidpos\").innerHTML,s={title:\"Wykres wejścia i wyjścia - regulator prędkości liniowej\",xaxis:{title:\"Czas\",showline:!0,mirror:\"allticks\",ticks:\"inside\"},margin:{l:40,b:35,t:35}},m=[{x:timeTable,y:[t],name:\"ax\",type:\"scatter\"},{x:timeTable,y:[n],name:\"Uchyb/100\",type:\"scatter\"},{x:timeTable,y:[l],name:\"Wyjście\",type:\"scatter\"}];Plotly.plot(e,m,s)}function createMotorPlotMeasurements(){var e=document.getElementById(\"motorplot\"),t=document.getElementById(\"xl\").innerHTML,n=document.getElementById(\"xr\").innerHTML,l=document.getElementById(\"errmotor\").innerHTML,s=document.getElementById(\"pidmotor\").innerHTML,m={title:\"Wykres wejścia i wyjścia - regulator silników\",xaxis:{title:\"Czas\",showline:!0,mirror:\"allticks\",ticks:\"inside\"},margin:{l:40,b:35,t:35}},o=[{x:timeTable,y:[t],name:\"Lewe koło\",type:\"scatter\"},{x:timeTable,y:[n],name:\"Prawe koło\",type:\"scatter\"},{x:timeTable,y:[l],name:\"Uchyb/100\",type:\"scatter\"},{x:timeTable,y:[s],name:\"Wyjście\",type:\"scatter\"}];Plotly.plot(e,o,m)}function createDistancePlotMeasurements(){var e=document.getElementById(\"distplot\"),t=document.getElementById(\"mdist\").innerHTML,n={title:\"Wykres odległości robota od przeszkód\",xaxis:{title:\"Czas\",showline:!0,mirror:\"allticks\",ticks:\"inside\"},margin:{l:40,b:35,t:35}},l=[{x:timeTable,y:[t],name:\"Odległość\",type:\"scatter\"},{x:timeTable,y:[dfz],name:\"dfz\",type:\"scatter\"}];Plotly.plot(e,l,n),distancePlotExists=!0}function createRobot(){var e=document.getElementById(\"robot\").getContext(\"2d\");e.beginPath(),e.fillStyle=\"red\",e.fillRect(22,40,75,40),e.beginPath(),e.moveTo(32,50),e.lineTo(12,50),e.lineTo(32,30),e.fill(),e.beginPath(),e.fillRect(46,30,28,20),e.beginPath(),e.moveTo(87,50),e.lineTo(107,50),e.lineTo(87,30),e.fill()}function createMap(){var e=document.getElementById(\"map\").getContext(\"2d\");e.font=\"18px Helvetica\",e.fillText(\"Wizualizacja pomiarów odległości od przeszkód\",110,25),createRobot()}function start(){createBalancePlotMeasurements(),createLinearPlotMeasurements(),createMotorPlotMeasurements(),createMap(),createDistancePlotMeasurements(),showPlots(),process()}function plotBalanceMeasurements(){var e=document.getElementById(\"balanceplot\"),t=document.getElementById(\"fx\").innerHTML,n=document.getElementById(\"ax\").innerHTML,l=document.getElementById(\"errbalance\").innerHTML,s=document.getElementById(\"pid\").innerHTML,m={x:[timeTable,timeTable,timeTable,timeTable],y:[[t],[n],[l],[s]]};Plotly.extendTraces(e,m,[0,1,2,3])}function plotLinearMeasurements(){var e=document.getElementById(\"linearplot\"),t=document.getElementById(\"ax\").innerHTML,n=document.getElementById(\"errpos\").innerHTML,l=document.getElementById(\"pidpos\").innerHTML,s={x:[timeTable,timeTable,timeTable],y:[[t],[n],[l]]};Plotly.extendTraces(e,s,[0,1,2])}function plotMotorMeasurements(){var e=document.getElementById(\"motorplot\"),t=document.getElementById(\"xl\").innerHTML,n=document.getElementById(\"xr\").innerHTML,l=document.getElementById(\"errmotor\").innerHTML,s=document.getElementById(\"pidmotor\").innerHTML,m={x:[timeTable,timeTable,timeTable,timeTable],y:[[t],[n],[l],[s]]};Plotly.extendTraces(e,m,[0,1,2,3])}function plotDistanceMeasurements(){var e=document.getElementById(\"distplot\"),t=document.getElementById(\"mdist\").innerHTML,n={x:[timeTable,timeTable],y:[[t],[dfz]]};Plotly.extendTraces(e,n,[0,1])}function rotateRobot(){var e=document.getElementById(\"robot\").getContext(\"2d\");e.clearRect(0,0,120,120),e.translate(60,60),e.rotate(dfz*DEGREES_TO_RADIANS),e.translate(-60,-60),createRobot()}function mapDistance(){var e=Number(document.getElementById(\"fz\").innerHTML)%360*DEGREES_TO_RADIANS,t=.8*Number(document.getElementById(\"mdist\").innerHTML)+50;return t>200&&(t=200),t<0&&(t=0),localX=t*Math.sin(e),localY=t*Math.cos(e),[300+localX,221.5-localY]}function updateMap(){var e=document.getElementById(\"map\").getContext(\"2d\"),t=mapDistance();e.beginPath(),e.fillStyle=\"black\",e.fillRect(t[0]-1,t[1]-1,3,3)}function hasRotated(){var e=Number(document.getElementById(\"mdist\").innerHTML),t=!1;return e!=lastDist&&(t=!0),lastDist=e,t}function plotMeasurements(){timeTable=[timeUnit+=.2],plotBalanceMeasurements(),plotLinearMeasurements(),plotMotorMeasurements(),dfz!=lastDfz&&rotateRobot(),lastDfz=dfz,powerOn&&(plotDistanceMeasurements(),hasRotated()&&updateMap())}var DEGREES_TO_RADIANS=Math.PI/180,RADIANS_TO_DEGREES=180/Math.PI,xmlHttp=createXmlHttpObject(),xmlHttpOn=createXmlHttpObject(),xmlHttpOff=createXmlHttpObject(),powerOn=!1,busy=!1,timeUnit=0,timeTable=[timeUnit],lastDist=0,dfz=0,lastDfz=0</script><body onload=start()><div class=content-container><h1>DWUKOŁOWY ROBOT INSPEKCYJNY</h1><div class=measurement-container style=height:109px><div class=measurement-left-block><div class=minor-header>Pozycja kątowa:</div><p>FX:<a id=fx></a><p>FY:<a id=fy></a><p>FZ:<a id=fz></a></div><div class=measurement-right-block><div class=minor-header>Przyspieszenie kątowe:</div><p>AX:<a id=ax></a> 1/s2<p>AY:<a id=ay></a> 1/s2<p>AZ:<a id=az></a> 1/s2</div></div><div class=measurement-container style=height:61px><div class=scan-left-block><div class=minor-header>Skanowanie pomieszczeń:</div><div class=measurement-left-block style=width:49.685%><p>Odległość od przeszkody:<a id=mdist></a> cm</div><div class=measurement-right-block style=width:49.685%><button onclick=showMap()>WYŚWIETL MAPĘ</button></div></div><div class=scan-right-block><div class=scan-button id=battery-switch onclick=powerSwitch()>SKANUJ</div></div></div><div class=measurement-container style=height:238px><div class=minor-header>Teoria sterowania</div><div class=reg-left-block><div class=reg-header>Balansowanie:</div><p>Wartość P: 0.019<p>Wartość I: 0.011<p>Wartość D: 0.000<p>Nasycenie: 1000<p>Wejście:<a id=errbalance></a><p>Wyjście:<a id=pid></a></div><div class=reg-mid-block><div class=reg-header>Położenie liniowe:</div><p>Wartość P: 12.000<p>Wartość I: 0.000<p>Wartość D: 0.000<p>Nasycenie: 5000<p>Wejście:<a id=errpos></a><p>Wyjście:<a id=pidpos></a></div><div class=reg-right-block><div class=reg-header>Sterownik silników:</div><p>Wartość P: 0.800<p>Wartość I: 0.000<p>Wartość D: 0.000<p>Nasycenie: 1000<p>Wejście:<a id=errmotor></a><p>Wyjście:<a id=pidmotor></a></div><div class=measurement-footer><button onclick=showPlots()>WYŚWIETL WYKRESY</button></div></div><div class=measurement-container style=height:109px><div class=minor-header>Pozycja liniowa: <span class=header-info>(aktualny kierunek:<a id=dir></a>)</span></div><div class=measurement-left-block><p>Kąt lewego koła:<a id=lpos></a> stopni<p>Droga lewego koła:<a id=xl></a> cm<p>Prędkość liniowa wg lewego koła:<a id=vl></a> km/h</div><div class=measurement-right-block><p>Kąt prawego koła:<a id=rpos></a> stopni<p>Droga prawego koła:<a id=xr></a> cm<p>Prędkość liniowa wg prawego koła:<a id=vr></a> km/h</div></div><div class=measurement-container style=height:85px><div class=minor-header>Pozostałe informacje:</div><div class=measurement-left-block><p>Napięcie ESP8266:<a id=nap></a> V<p>Czas działania:<a id=runtime></a></div><div class=measurement-right-block><p>Siła sygnału WiFi:<a id=rssi></a> dBm<div style=height:29px></div></div></div></div><div class=plots-container id=plotcontainer><div class=plot-panel id=balanceplot></div><div class=plot-panel id=linearplot></div><div class=plot-panel id=motorplot></div></div><div class=scan-container id=scancontainer><canvas height=120 id=robot style=background-color:#fff;color:red;z-index:10;position:absolute;left:240px;top:177px width=120>Your browser does not support the HTML5 canvas tag.</canvas><canvas height=443 id=map style=background-color:#fff;color:#000 width=600>Your browser does not support the HTML5 canvas tag.</canvas><div class=plot-panel id=distplot></div></div><div class=footer>Mateusz Staszków - Praca inżynierska \"Dwukołowy robot inspekcyjny\", Politechnika Warszawska 2017</div>"};
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

String XML;
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
          m.vl = atoi(number) * CMPS_TO_KMPH;
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
          m.dist_m = atoi(number) * HCSR_CONSTANT;
          break;
        case 14:
          m.balance_error = atoi(number)/100;
          break;
        case 15:
          m.pos_error = atoi(number)/100;
          break;
        case 16:
          m.pos_pid = atoi(number);
          break;
        case 17:
          m.motor_error = atoi(number)/100;
          break;
        case 18:
          m.motor_pid = atoi(number);
          break;
        case 19:
          m.xr = atoi(number);
          break;
        case 20:
          m.vr = atoi(number) * CMPS_TO_KMPH;
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

void buildXML(){  
  String move_direction = "";
  if(measuredData.dir == 1) move_direction = "przód";
  else if(measuredData.dir == 0) move_direction = "tył";

  float voltage = (float)ESP.getVcc()/19859.0;
  int voltage_high = voltage;
  int voltage_low = voltage*1000 - voltage_high*1000;
  long rssi = WiFi.RSSI();

  static int lastFz = 0;
  int dFz = measuredData.fz - lastFz;
  lastFz = measuredData.fz;
  
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
  XML+="    <response-dfz>";
  XML+=       (String)dFz;
  XML+="    </response-dfz>";
  XML+="    <response-battery>";
  XML+=       (String)measuredData.battery_state;
  XML+="    </response-battery>";
  XML+="    <rssi>";
  XML+=       (String)rssi;
  XML+="    </rssi>";
  XML+="  </Data>"; 
}

void handleWebsite(){
  server.send_P(200,"text/html",webSite);
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

void configureMobileNetwork() {
  IPAddress ip(192, 168, 43, MOBILE_IP_ADDRESS);
  IPAddress gateway(192, 168, 43, 1);
  IPAddress subnet(255, 255, 255, 0);
  WiFi.config(ip, gateway, subnet);
}

void connectToNetwork() {
  int tryCount = 0;
  while((WiFi.status() != WL_CONNECTED) && (tryCount != MAX_CONNECTION_TRY_COUNT)) {
    delay(500);
    tryCount++;
  }
}

void connectToHomeHotSpot() {
  if(WiFi.status() != WL_CONNECTED) {
    WiFi.disconnect();
    serialPrintWithArg("Trying to connect to SSID: ", ssid);
    WiFi.begin(ssid, password);
    connectToNetwork();
  }
  if(WiFi.status() != WL_CONNECTED) {
    serialPrintWithArg("Could not connect to SSID: ", ssid);
  }
}

void connectToMobileHotSpot() {
  if(WiFi.status() != WL_CONNECTED) {
    WiFi.disconnect();
    configureMobileNetwork();
    serialPrintWithArg("Trying to connect to SSID: ", ssidTel);
    WiFi.begin(ssidTel, passwordTel);
    connectToNetwork();
  }
  if(WiFi.status() != WL_CONNECTED) {
    serialPrintWithArg("Could not connect to SSID: ", ssidTel);
  }
}

void configureBackend() {
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
}

void setup() {  
  Serial.println("\n\nBOOTING ESP8266 ...");
  Serial.begin(115200);  

  while(WiFi.status() != WL_CONNECTED) {
    connectToHomeHotSpot();
    connectToMobileHotSpot();
  }

  configureBackend();
  server.begin();  
}

void loop() {
  read_uart();
  server.handleClient();
}
