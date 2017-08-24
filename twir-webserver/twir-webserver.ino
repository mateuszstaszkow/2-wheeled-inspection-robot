#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

#define USART_BUFFER_SIZE 100
#define MAX_CONNECTION_TRY_COUNT 20
#define MOBILE_IP_ADDRESS 90

// minification online: https://www.willpeavy.com/minifier/
const char webSite[] PROGMEM = {"<!DOCTYPE HTML> <head> <title>Dwukołowy robot inspekcyjny</title> <meta charset='UTF-8'> <style>.content-container{width: 800px; float: left;}.minor-header{border-bottom: 3px solid black; font-size: 1.5em; font-weight: bold;}.reg-header{margin-top: 3px; font-size: 1.1em; font-weight: bold;}.header-info{font-weight: 100;}.measurement-left-block{width: 49.75%; float: left; border-right: 2px solid black;}.measurement-right-block{width: 49.75%; float: left; border-left: 2px solid black;}.reg-left-block{width: 33%; float: left; border-right: 2px solid black;}.reg-mid-block{width: 33%; float: left; border-right: 2px solid black; border-left: 2px solid black;}.reg-right-block{width: 33%; float: left; border-left: 2px solid black;}.measurement-container{clear: both; margin-bottom: 15px; background-color: DimGray;}.measurement-container:hover{background-color: gray;}.measurement-footer{border-top: 3px solid black; clear: both;}.scan-button{background-color: green; border: none; color: white; padding: 15px 40px; text-align: center; text-decoration: none; display: inline-block; font-size: 25px; cursor: pointer;}.scan-left-block{width: 79%; float: left;}.scan-right-block{width: 21%; background-color: black; float: left; text-align: right;}.footer{position: fixed; right: 0; bottom: 0; left: 0; padding: 1rem; background-color: #111; text-align: center; z-index: 1000;}.plots-container{width: 600px; height: 664px; float: left; margin-left: 30px; margin-top: 82px;}.plot-panel{width: 100%; height: 214px; margin-bottom: 10px;}.scan-container{width: 600px; height: 664px; float: left; margin-left: 30px; margin-top: 82px; position: relative;}body{background-color: #000; font-family: Calibri; color: #fff; text-align: center;}p{margin-top: 5px; margin-bottom: 5px;}button{margin-top: 2px; margin-bottom: 3px; font-size: 11px; font-weight: bold; padding: 4px; padding-left: 15px; padding-right: 15px; cursor: pointer;}</style> <script src='https://cdn.plot.ly/plotly-latest.min.js'></script></head><script>var deg2rad=Math.PI/180; var rad2deg=180/Math.PI; var xmlHttp=createXmlHttpObject(); var xmlHttpOn=createXmlHttpObject(); var xmlHttpOff=createXmlHttpObject(); var powerOn=false; var busy=false; var timeUnit=0; var timeTable=[timeUnit]; var lastDist=0; var lastFz=0; var dfz=0; var robotAngle=0; function showMap(){document.getElementById('plotcontainer').style.display='none';document.getElementById('scancontainer').style.display='block';}function showPlots(){document.getElementById('plotcontainer').style.display='block';document.getElementById('scancontainer').style.display='none';}function setButtonStop(){document.getElementById('battery-switch').innerHTML='STOP'; document.getElementById('battery-switch').style.background='Red'; document.getElementById('battery-switch').style.color='White';}function setButtonScan(){document.getElementById('battery-switch').innerHTML='SKANUJ';document.getElementById('battery-switch').style.background='Green';document.getElementById('battery-switch').style.color='White';}function setPowerButton(){if(powerOn && !busy){setButtonStop();}else if(!powerOn && !busy){setButtonScan();}}function powerSwitch(){if(!powerOn && !busy){busy=true; document.getElementById('battery-switch').style.background='DarkOliveGreen'; document.getElementById('battery-switch').style.color='Gray'; xmlHttpOn.open('PUT','/on',true); xmlHttpOn.onreadystatechange=handleSwitchOn; xmlHttpOn.send(null);}else if(powerOn && !busy){busy=true; document.getElementById('battery-switch').style.background='Brown'; document.getElementById('battery-switch').style.color='Gray'; xmlHttpOff.open('PUT','/off',true); xmlHttpOff.onreadystatechange=handleSwitchOff; xmlHttpOff.send(null);}}function getPID(regulatorPart, pidValue){xmlHttpOff.open('PUT','/pid?part=' + regulatorPart + '&value=' + pidValue.value.toString(), true); xmlHttpOff.send(null);}function createXmlHttpObject(){if(window.XMLHttpRequest){xmlHttp=new XMLHttpRequest();}else{xmlHttp=new ActiveXObject('Microsoft.XMLHTTP');}return xmlHttp;}function get_firstchild(n){var x=n.firstChild; while (x.nodeType !=1){x=x.nextSibling;}return x;}function process(){setPowerButton(); plotMeasurements(); if(xmlHttp.readyState==0 || xmlHttp.readyState==4){xmlHttp.open('PUT','xml',true); xmlHttp.onreadystatechange=handleServerResponse; xmlHttp.send(null);}setTimeout('process()',200);}function handleSwitchOn(){if(xmlHttpOn.readyState==4 && xmlHttpOn.status==200){powerOn=true; setButtonStop(); showMap(); busy=false;}}function handleSwitchOff(){if(xmlHttpOff.readyState==4 && xmlHttpOff.status==200){powerOn=false; setButtonScan(); busy=false;}}function handleServerResponse(){if(xmlHttp.readyState==4 && xmlHttp.status==200){xmlResponse=xmlHttp.responseXML; xmldoc=xmlResponse.getElementsByTagName('response-time'); message=xmldoc[0].firstChild.nodeValue; document.getElementById('runtime').innerHTML=message; xmldoc=xmlResponse.getElementsByTagName('response-fx'); message=xmldoc[0].firstChild.nodeValue; document.getElementById('fx').innerHTML=message; xmldoc=xmlResponse.getElementsByTagName('response-fy'); message=xmldoc[0].firstChild.nodeValue; document.getElementById('fy').innerHTML=message; xmldoc=xmlResponse.getElementsByTagName('response-fz'); message=xmldoc[0].firstChild.nodeValue; document.getElementById('fz').innerHTML=message; xmldoc=xmlResponse.getElementsByTagName('response-ax'); message=xmldoc[0].firstChild.nodeValue; document.getElementById('ax').innerHTML=message; xmldoc=xmlResponse.getElementsByTagName('response-ay'); message=xmldoc[0].firstChild.nodeValue; document.getElementById('ay').innerHTML=message; xmldoc=xmlResponse.getElementsByTagName('response-az'); message=xmldoc[0].firstChild.nodeValue; document.getElementById('az').innerHTML=message; xmldoc=xmlResponse.getElementsByTagName('response-lpos'); message=xmldoc[0].firstChild.nodeValue; document.getElementById('lpos').innerHTML=message; xmldoc=xmlResponse.getElementsByTagName('response-rpos'); message=xmldoc[0].firstChild.nodeValue; document.getElementById('rpos').innerHTML=message; xmldoc=xmlResponse.getElementsByTagName('response-xl'); message=xmldoc[0].firstChild.nodeValue; document.getElementById('xl').innerHTML=message; xmldoc=xmlResponse.getElementsByTagName('response-vl'); message=xmldoc[0].firstChild.nodeValue; document.getElementById('vl').innerHTML=message; xmldoc=xmlResponse.getElementsByTagName('response-nap'); message=xmldoc[0].firstChild.nodeValue; document.getElementById('nap').innerHTML=message; xmldoc=xmlResponse.getElementsByTagName('response-dir'); message=xmldoc[0].firstChild.nodeValue; document.getElementById('dir').innerHTML=message; xmldoc=xmlResponse.getElementsByTagName('response-pid'); message=xmldoc[0].firstChild.nodeValue; document.getElementById('pid').innerHTML=message; xmldoc=xmlResponse.getElementsByTagName('response-turn'); message=xmldoc[0].firstChild.nodeValue.replace(/\s+/g, ''); if(message==='1') powerOn=true; else if(message==='0') powerOn=false; xmldoc=xmlResponse.getElementsByTagName('response-dist-m'); message=xmldoc[0].firstChild.nodeValue; document.getElementById('mdist').innerHTML=message; xmldoc=xmlResponse.getElementsByTagName('response-balance-error'); message=xmldoc[0].firstChild.nodeValue; document.getElementById('errbalance').innerHTML=message; xmldoc=xmlResponse.getElementsByTagName('response-pos-error'); message=xmldoc[0].firstChild.nodeValue; document.getElementById('errpos').innerHTML=message; xmldoc=xmlResponse.getElementsByTagName('response-pos-pid'); message=xmldoc[0].firstChild.nodeValue; document.getElementById('pidpos').innerHTML=message; xmldoc=xmlResponse.getElementsByTagName('response-motor-error'); message=xmldoc[0].firstChild.nodeValue; document.getElementById('errmotor').innerHTML=message; xmldoc=xmlResponse.getElementsByTagName('response-motor-pid'); message=xmldoc[0].firstChild.nodeValue; document.getElementById('pidmotor').innerHTML=message; xmldoc=xmlResponse.getElementsByTagName('response-xr'); message=xmldoc[0].firstChild.nodeValue; document.getElementById('xr').innerHTML=message; xmldoc=xmlResponse.getElementsByTagName('response-vr'); message=xmldoc[0].firstChild.nodeValue; document.getElementById('vr').innerHTML=message;xmldoc=xmlResponse.getElementsByTagName('rssi'); message=xmldoc[0].firstChild.nodeValue; document.getElementById('rssi').innerHTML=message;}}function createBalancePlotMeasurements(){var plot=document.getElementById('balanceplot'); var fx=document.getElementById('fx').innerHTML; var ax=document.getElementById('ax').innerHTML; var error=document.getElementById('errbalance').innerHTML; var pid=document.getElementById('pid').innerHTML; var angle={x: timeTable, y: [fx], name: 'fx', type: 'scatter'}; var angularAcceleration={x: timeTable, y: [ax], name: 'ax', type: 'scatter'}; var controlInput={x: timeTable, y: [error], name: 'Uchyb', type: 'scatter'}; var controlOutput={x: timeTable, y: [pid], name: 'Wyjście', type: 'scatter'}; var layout={title: 'Wykres wejścia i wyjścia - regulator kąta wychylenia', xaxis:{title: 'Czas', showline: true, mirror: 'allticks', ticks: 'inside'}, margin:{l: 40, b: 35, t: 35}}; var data=[angle, angularAcceleration, controlInput, controlOutput]; Plotly.plot(plot, data, layout);}function createLinearPlotMeasurements(){var plot=document.getElementById('linearplot'); var ax=document.getElementById('ax').innerHTML; var error=document.getElementById('errpos').innerHTML; var pid=document.getElementById('pidpos').innerHTML; var angularAcceleration={x: timeTable, y: [ax], name: 'ax', type: 'scatter'}; var controlInput={x: timeTable, y: [error], name: 'Uchyb', type: 'scatter'}; var controlOutput={x: timeTable, y: [pid], name: 'Wyjście', type: 'scatter'}; var layout={title: 'Wykres wejścia i wyjścia - regulator prędkości liniowej', xaxis:{title: 'Czas', showline: true, mirror: 'allticks', ticks: 'inside'}, margin:{l: 40, b: 35, t: 35}}; var data=[angularAcceleration, controlInput, controlOutput]; Plotly.plot(plot, data, layout);}function createMotorPlotMeasurements(){var plot=document.getElementById('motorplot'); var xl=document.getElementById('xl').innerHTML; var xr=document.getElementById('xr').innerHTML; var error=document.getElementById('errmotor').innerHTML; var pid=document.getElementById('pidmotor').innerHTML; var leftPosition={x: timeTable, y: [xl], name: 'Lewe koło', type: 'scatter'}; var rightPosition={x: timeTable, y: [xr], name: 'Prawe koło', type: 'scatter'}; var controlInput={x: timeTable, y: [error], name: 'Uchyb', type: 'scatter'}; var controlOutput={x: timeTable, y: [pid], name: 'Wyjście', type: 'scatter'}; var layout={title: 'Wykres wejścia i wyjścia - regulator silników', xaxis:{title: 'Czas', showline: true, mirror: 'allticks', ticks: 'inside'}, margin:{l: 40, b: 35, t: 35}}; var data=[leftPosition, rightPosition, controlInput, controlOutput]; Plotly.plot(plot, data, layout);}function createDistancePlotMeasurements(){var plot=document.getElementById('distplot'); var mdist=document.getElementById('mdist').innerHTML;var fz=document.getElementById('fz').innerHTML;lastFz=fz; var middleDistance={x: timeTable, y: [mdist], name: 'Odległość', type: 'scatter'};var angle={x: timeTable, y: [fz], name: 'dfz', type: 'scatter'}; var layout={title: 'Wykres odległości robota od przeszkód', xaxis:{title: 'Czas', showline: true, mirror: 'allticks', ticks: 'inside'}, margin:{l: 40, b: 35, t: 35}}; var data=[middleDistance, angle]; Plotly.plot(plot, data, layout);distancePlotExists=true;}function createRobot(){var c=document.getElementById('robot');var ctx=c.getContext('2d');ctx.beginPath();ctx.fillStyle='red';ctx.fillRect(22,40,75,40);ctx.beginPath();ctx.moveTo(32,50);ctx.lineTo(12,50);ctx.lineTo(32,30);ctx.fill();ctx.beginPath();ctx.fillRect(46,30,28,20);ctx.beginPath();ctx.moveTo(87,50);ctx.lineTo(107,50);ctx.lineTo(87,30);ctx.fill();}function createMap(){var c=document.getElementById('map');var ctx=c.getContext('2d');ctx.font='18px Helvetica';ctx.fillText('Wizualizacja pomiarów odległości od przeszkód',110,25);createRobot();}function start(){createBalancePlotMeasurements(); createLinearPlotMeasurements(); createMotorPlotMeasurements();createMap();createDistancePlotMeasurements();showPlots(); process();}function plotBalanceMeasurements(){var plot=document.getElementById('balanceplot'); var fx=document.getElementById('fx').innerHTML; var ax=document.getElementById('ax').innerHTML; var error=document.getElementById('errbalance').innerHTML; var pid=document.getElementById('pid').innerHTML; var dataY=[[fx], [ax], [error], [pid]]; var dataX=[timeTable, timeTable, timeTable, timeTable]; var extendedPlot={x: dataX, y: dataY}; Plotly.extendTraces(plot, extendedPlot, [0, 1, 2, 3]);}function plotLinearMeasurements(){var plot=document.getElementById('linearplot'); var ax=document.getElementById('ax').innerHTML; var error=document.getElementById('errpos').innerHTML; var pid=document.getElementById('pidpos').innerHTML; var dataY=[[ax], [error], [pid]]; var dataX=[timeTable, timeTable, timeTable]; var extendedPlot={x: dataX, y: dataY}; Plotly.extendTraces(plot, extendedPlot, [0, 1, 2]);}function plotMotorMeasurements(){var plot=document.getElementById('motorplot'); var xl=document.getElementById('xl').innerHTML; var xr=document.getElementById('xr').innerHTML; var error=document.getElementById('errmotor').innerHTML; var pid=document.getElementById('pidmotor').innerHTML; var dataY=[[xl], [xr], [error], [pid]]; var dataX=[timeTable, timeTable, timeTable, timeTable]; var extendedPlot={x: dataX, y: dataY}; Plotly.extendTraces(plot, extendedPlot, [0, 1, 2, 3]);}function plotDistanceMeasurements(){var plot=document.getElementById('distplot'); var mdist=document.getElementById('mdist').innerHTML;var currFz=Number(document.getElementById('fz').innerHTML);dfz=currFz - lastFz;last_fz=currFz; var dataY=[[mdist], [dfz]]; var dataX=[timeTable, timeTable]; var extendedPlot={x: dataX, y: dataY}; Plotly.extendTraces(plot, extendedPlot, [0, 1]);}function rotateRobot(){var c=document.getElementById('robot');var ctx=c.getContext('2d');var angle=dfz*deg2rad/100;robotAngle +=angle;ctx.clearRect(0, 0, 120, 120);ctx.translate(60, 60);ctx.rotate(angle);ctx.translate(-60, -60);createRobot();}function mapDistance(){var angle=robotAngle % (360 * deg2rad);var mdist=0.8 * Number(document.getElementById('mdist').innerHTML) + 50;if(mdist > 200) mdist=200;if(mdist < 0) mdist=0;localX=mdist * Math.sin(angle);localY=mdist * Math.cos(angle);return [300 + localX, 221.5 - localY];}function updateMap(){var c=document.getElementById('map');var ctx=c.getContext('2d');var position=mapDistance();ctx.beginPath();ctx.fillStyle='black';ctx.fillRect(position[0] - 1, position[1] - 1, 3, 3);}function hasRotated(){var mdist=Number(document.getElementById('mdist').innerHTML);var different=false;if(mdist !=lastDist) different=true;lastDist=mdist;return different;}function plotMeasurements(){timeUnit +=0.2; timeTable=[timeUnit]; plotBalanceMeasurements(); plotLinearMeasurements(); plotMotorMeasurements();if(!powerOn) return;plotDistanceMeasurements();if(!hasRotated()) return;rotateRobot();updateMap();}</script><body onload='start()'> <div class='content-container'> <h1>DWUKOŁOWY ROBOT INSPEKCYJNY</h1> <div class='measurement-container' style='height: 109px;'> <div class='measurement-left-block'> <div class='minor-header'>Pozycja kątowa:</div><p>FX: <A id='fx'></A></p><p>FY: <A id='fy'></A></p><p>FZ: <A id='fz'></A></p></div><div class='measurement-right-block'> <div class='minor-header'>Przyspieszenie kątowe:</div><p>AX: <A id='ax'></A> 1/s2</p><p>AY: <A id='ay'></A> 1/s2</p><p>AZ: <A id='az'></A> 1/s2</p></div></div><div class='measurement-container' style='height: 61px;'> <div class='scan-left-block'> <div class='minor-header'>Skanowanie pomieszczeń:</div><div class='measurement-left-block' style='width: 49.685%;'> <p>Odległość od przeszkody: <A id='mdist'></A> cm</p></div><div class='measurement-right-block' style='width: 49.685%;'> <button onclick='showMap()'>WYŚWIETL MAPĘ</button> </div></div><div class='scan-right-block'> <div id='battery-switch' onclick='powerSwitch()' class='scan-button'> SKANUJ </div></div></div><div class='measurement-container' style='height: 238px;'> <div class='minor-header'>Teoria sterowania</div><div class='reg-left-block'> <div class='reg-header'>Balansowanie:</div><p>Wartość P: 0.019</p><p>Wartość I: 0.011</p><p>Wartość D: 0.000</p><p>Nasycenie: 1000</p><p>Wejście: <A id='errbalance'></A></p><p>Wyjście: <A id='pid'></A></p></div><div class='reg-mid-block'> <div class='reg-header'>Położenie liniowe:</div><p>Wartość P: 12.000</p><p>Wartość I: 0.000</p><p>Wartość D: 0.000</p><p>Nasycenie: 5000</p><p>Wejście: <A id='errpos'></A></p><p>Wyjście: <A id='pidpos'></A></p></div><div class='reg-right-block'> <div class='reg-header'>Sterownik silników:</div><p>Wartość P: 0.800</p><p>Wartość I: 0.000</p><p>Wartość D: 0.000</p><p>Nasycenie: 1000</p><p>Wejście: <A id='errmotor'></A></p><p>Wyjście: <A id='pidmotor'></A></p></div><div class='measurement-footer'> <button onclick='showPlots()'>WYŚWIETL WYKRESY</button> </div></div><div class='measurement-container' style='height: 109px;'> <div class='minor-header'>Pozycja liniowa: <span class='header-info'>(aktualny kierunek: <A id='dir'></A>)</span> </div><div class='measurement-left-block'> <p>Kąt lewego koła: <A id='lpos'></A> stopni</p><p>Droga lewego koła: <A id='xl'></A> cm</p><p>Prędkość liniowa wg lewego koła: <A id='vl'></A> km/h</p></div><div class='measurement-right-block'> <p>Kąt prawego koła: <A id='rpos'></A> stopni</p><p>Droga prawego koła: <A id='xr'></A> cm</p><p>Prędkość liniowa wg prawego koła: <A id='vr'></A> km/h</p></div></div><div class='measurement-container' style='height: 85px;'> <div class='minor-header'>Pozostałe informacje:</div><div class='measurement-left-block'> <p>Napięcie ESP8266: <A id='nap'></A> V</p><p>Czas działania: <A id='runtime'></A></p></div><div class='measurement-right-block'> <p>Siła sygnału WiFi: <A id='rssi'></A> dBm</p><div style='height: 29px;'></div></div></div></div><div id='plotcontainer' class='plots-container'> <div id='balanceplot' class='plot-panel'></div><div id='linearplot' class='plot-panel'></div><div id='motorplot' class='plot-panel'></div></div><div id='scancontainer' class='scan-container'><canvas id='robot' width='120' height='120' style='background-color: white; color: red; z-index: 10; position: absolute; left: 240px; top: 177px;'> Your browser does not support the HTML5 canvas tag.</canvas><canvas id='map' width='600' height='443' style='background-color: white; color: black;'> Your browser does not support the HTML5 canvas tag.</canvas><div id='distplot' class='plot-panel'></div></div><div class='footer'> Mateusz Staszków - Praca inżynierska 'Dwukołowy robot inspekcyjny', Politechnika Warszawska 2017 </div></BODY></HTML>"};

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
