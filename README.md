Wstęp
=====

Roboty inspekcyjne znajdują zastosowanie najczęściej w miejscach trudno
dostępnych dla człowieka. Można również spotkać małe roboty
przeszukujące korytarze w biurowcach w godzinach nocnych. Ich zadaniem
jest odwiedzenie lub zeskanowanie każdego miejsca w budynku i upewnienie
się czy wszystko jest zachowane w ustalonym porządku.

Problem stabilizacji położenia w pionie dotyczy wielu gałęzi przemysłu,
na przykład takich jak: produkcja jednoosobowych transportowych pojazdów
typu Segway na lotniskach i w centrach handlowych, układy balansujące na
nierównych powierzchniach, czy nawet roboty humanoidalne.

Połączenie tych dwóch cech czyni robota pomocnym w trudno dostępnych
miejscach zarówno jeżeli chodzi o toksyczność jak i bardzo nieregularną
powierzchnię jezdną.

Dwukołowy robot balansujący działa na zasadzie wahadła odwróconego. Jego
zadaniem jest ciągłe utrzymywanie pionu z możliwie jak najmniejszymi
odchyłami. Jest to układ niestabilny i wymaga automatycznej regulacji -
zadanie to spełnia regulator liniowo-kwadratowy-Gaussa.\
W układzie za sprzężenia zwrotne odpowiadają czujniki położenia, a
dokładniej przyspieszenia - akcelerometr i prędkości kątowej - żyroskop.
Do sterowania silnikami jest użyte sprzężenie zwrotne w postaci pomiaru
położenia kół przez enkodery magnetyczne. Elektronika znajduje się na
wytrawionych płytkach PCB. Układ jest zasilany z akumulatora
litowo-polimerowego. Robot jest zaprojektowany tak, aby każdy element
był jak najlżejszy i można było go łatwo wymienić. Stąd duża ilość śrub
i mocowań na wcisk. Przy każdym mikrokontrolerze znajduje się złącze
programatora, co umożliwia bezproblemowe aktualizowanie oprogramowania.
Do komunikacji robota z otoczeniem służą czujniki ultradźwiękowe. Moduł
WiFi umożliwia komunikację z komputerem PC i odczyt wszystkich pomiarów.

Postawienie zadania
-------------------

Zadaniem pracy jest przedstawienie konstrukcji autonomicznego i
balansującego robota inspekcyjnego oraz przedstawienie działania za
pośrednictwem wykresów i tabel.

Budowa robota to złożone zadanie i wymaga podzielenia na mniejsze
elementy konstrukcji:

1.  Przeczytanie literatury na temat:

    -   balansujących i inspekcyjnych robotów komercyjnych,

    -   dokumentacji technicznych mikrokontrolerów serii AVR i ARM,
        czujników ultradźwiękowych, akcelerometrów i żyroskopów, modułów
        WiFi, enkoderów, silników,

    -   dekodowania sygnałów z wyżej wymienionych czujników,

    -   szeregowej komunikacji przewodowej i bezprzewodowej,

    -   druku 3D,

    -   technologii wytwarzania płytek elektronicznych,

2.  Zaprojektowanie:

    -   modelu matematycznego i modelu sterowania w środowisku Matlab i
        Simulink,

    -   elektroniki w środowisku Cadsoft EAGLE,

    -   modelu 3D w programie AutoCAD, podzielenie na części i
        wyeksportowanie do formatu, STL,

3.  Zakup elementów elektronicznych,

4.  Wydrukowanie części na drukarce 3D i sklejenie ich acetonem,

5.  Wytrawienie płytek PCB w nadsiarczanie sodu,

6.  Lutowanie elementów elektronicznych do płytek,

7.  Skonfigurowanie środowisk programistycznych,

8.  Wykonanie szkieletów kodu,

9.  Wzajemne skomunikowanie mikrokontrolerów,

10. Oprogramowanie czujników,

11. Implementacja:

    -   sterownika silników DC,

    -   komunikacji mikrokontrolera z komputerem PC,

    -   regulacji położenia w pionie (LQG),

    -   systemu skanowania pomieszczeń,

12. Testy i analiza,

13. Wnioski.

Mechanika
=========

Robot został zaprojektowany tak, aby był jak najlżejszy. Ułatwia to
sterowanie jego położeniem przy odpowiednio dobranych silnikach. Środek
ciężkości znajduje się jak najwyżej, co przekłada się na ułatwienie
sterowania położenia w pionie - układ gwałtowniej odpowiada na zadane
wartości.

Parametry ogólne
----------------

Masa: 1 kg\
Wymiary: 95 mm x 180 mm x 306 mm\
Wysokość środka masy: 162.6 mm\
Moment bezwładności dla osi położonej w środku masy: 0.01 $kgm^2$\

![image](\ImgPath/koncepcja.jpg)

[model3d]

Model 3D
--------

Model został zbudowany w programie Autodesk AutoCAD 2017. Następnie
został podzielony na części w taki sposób, aby wydruk 3D był jak
najmniej kłopotliwy. Każda część jest nie większa niż 100 mm x 180 mm x
80 mm. Następnie modele zostały wyeksportowane do formatu STL.

![image](\ImgPath/druki3d.PNG)

[druki3d]

Obudowa
-------

Masa: 402 g\
Wymiary: 95 mm x 180 mm x 306 mm\

Obudowa została w całości wydrukowana na drukarce 3D, a poszczególne
elementy są połączone acetonem. Proces adhezyjnego klejenia części jest
możliwy poprzez użycie jako materiału do drukowania tworzywa ABS -
kopolimeru akrylonitrylo-butadieno-styrenowego, który reagując z
acetonem rozpuszcza się, umożliwiając, poprzez zbliżenie do niego innego
materiału na przyklejenie się. Obudowa składa się z czterech segmentów -
kolejno od dołu:

-   segment silników - odpowiadający za napęd robota, silniki są
    przykręcone do ścian bocznych za pomocą śrub i znajdują się w
    specjalnie zaprojektowanych miejscach, aby ograniczyć ich ruch.
    Takie zamontowanie umożliwia ich łatwą i szybką wymianę. W ścianach
    bocznych umieszczono łożyska, w których znajdują się wały silników,

-   część akumulatora - odpowiadająca za zasilanie, akumulator ma swój
    koszyk, co umożliwia łatwą wymianę i odpowiednie usztywnienie
    pakietu ogniw,

-   część elektroniczna: sciany z otworami na wciśnięcie płytki PCB i z
    koszykiem na żyroskop/akcelerometr na spodzie najwyższej półki,

-   część komunikacyjna: znajdują się tu miejsca na 3 czujniki
    ultradźwiękowe oraz na moduł Wi-Fi.

![image](\ImgPath/model3d.PNG)

[model3d]

Napęd
-----

Do napędu robota zostały użyte silniki prądu stałego firmy Pololu na
napięcie 12 V z przekładnią 9.68 : 1 (25Dx48L) i enkoderem CPR 48,
których wały opierają się na łożyskach kulkowych umieszczonych w
ścianach bocznych.\
\
Parametry silnika:

-   masa: 0.095 kg,

-   wymiary: front $\phi$25 mm, długość korpusu 78.7 mm,

-   moment obrotowy: 0.15 Nm,

-   prędkość obrotowa: 770 obr/min,

-   średni pobór prądu: 200 mA,

-   maksymalny pobór prądu: 2100 mA,

-   rozdzielczość enkodera: 48 impulsów na obrót (po przekładni 464).

![image](\ImgPath/silnikidc.jpg)

[schematKomunikacji]

Do wału silnika zostały dopasowane łożyska kulkowe o średnicach 4 mm i
12 mm oraz grubości 4 mm. Dzięki nim następuje bezpieczniejsze
przełożenie napędu na koła z minimalnym tarciem. Obniżają nacisk na wał
silnika.

![image](\ImgPath/lozysko.jpg)

[schematKomunikacji]

Koła
----

W konstrukcji zostały użyte lekkie poliuretanowe koła z plastikową
piastą, charakteryzują się niską ceną i dostateczną przyczepnością.\
Parametry koła:

-   średnica opony: 70 mm,

-   szerokość opony: 24 mm ,

-   średnica otworu: 4 mm ,

-   średnica plastikowej piasty: 35 mm ,

-   waga: 30g.

Do połączenia kół z krótkim wałem silnika zostały skonstruowane
specjalne łączniki na śruby. Ich działanie polega na zaciśnięciu bocznej
śruby o średnicy 2,5 mm do granic możliwości. Wymagane jest w tym
momencie specjalne ułożenie, w środku łącznika, wału silnika, którego
przekrój nie jest idealnym okręgiem, lecz zawiera delikatne
zeszlifowanie (kształtem przypomina literę D). Po dociśnięciu śruby w
miejsce tego szlifu jakikolwiek luz zostanie wyeliminowany.

![image](\ImgPath/hub.jpg)

[schematKomunikacji]

Po testach wytrzymałościowych okazało się, że plastikowe tworzywo ABS
nie wytrzymało dużych naprężeń śruby na wał silnika i uległo
odkształceniu, dlatego zostały zastosowane uniwersalne aluminiowe huby
mocujące z otworem na 4 mm.

![image](\ImgPath/huby.jpg)

[schematKomunikacji]

Matematyczny model układu
-------------------------

Robot jest zbudowany symetrycznie w odniesieniu do szerokości i
długości, więc po zastosowaniu uproszczenia środek masy jedynie w
odniesieniu do wysokości wymaga przeprowadzenia dokładnych obliczeń.
Reszta wartości będzie się pokrywała ze środkiem geometrycznym bryły.
Przy obliczeniach położenie i masa przewodów zostały ujęte w dużym
przybliżeniu.\

Środek masy i moment bezwładności zostały obliczone poprzez podzielenie
robota na obiekty i wyliczenie dla każdego środka masy i momentu
bezwładności, a następnie zsumowanie wszystkich składowych w programie
Microsoft Excel 2013.

![image](\ImgPath/wektory.jpg)

[schematKomunikacji]

Obliczone dane bryły:

-   wysokość środka masy obudowy: 162.6 mm,

-   moment bezwładności obudowy: 0.01 $kg*m^2$.

Obliczenia wykonane w programie Microsoft Excel 2013:

![image](\ImgPath/i.PNG)

[schematKomunikacji]

Wyznaczenie równań liniowych układu
-----------------------------------

Jest to układ odwróconego wahadła, który przypomina patyk znajdujący się
na wózku, na który działa siła z zewnątrz. Układ taki wygląda
następująco:\

![image](\ImgPath/wahadlo.jpg)

[schematKomunikacji]

Równania opisujące model:\
\
$F=(M+m)\ddot{x}+b\dot{x}+ml\ddot{\theta}cos\theta-ml\dot{\theta}^2sin\theta$\
$-ml\ddot{x}cos\theta=(I+ml^2)\ddot{\theta}+mglsin\theta$\
\
Dane układu:

-   M - masa wózka: 0 kg (wózek słuzył tylko do zilustrowania sytuacji),

-   m - masa odwróconego wahadła: 1 kg,

-   I - moment bezwładności odwróconego wahadła: 0.01 $kgm^2$,

-   F - siła działająca na wózek: nie większa niż 4 N (założenie),

-   x - położenie wózka - zmienna stanu mierzona w metrach,

-   $\theta$ - wychylenie wahadła od pionu - zmienna stanu mierzona w
    stopniach,

-   b - współczynnik tarcia wózka o podłoże: ok. 0.1 N/m/s,

-   l - odległość od środka masy: 0.137 m,

-   g - przyspieszenie ziemskie w Warszawie: 9.8123 m/$s^2$.

W przypadku robota balansującego na dwóch kołach równanie upraszacza się
do postaci:\
\
$F=m\ddot{x}+b\dot{x}+ml\ddot{\theta}cos\theta-ml\dot{\theta}^2sin\theta$\
$-ml\ddot{x}cos\theta=(I+ml^2)\ddot{\theta}+mglsin\theta$\
\
Powyższe równania są nieliniowe, więc po linearyzacji w otoczeniu punktu
$\theta=\pi$ otrzymaNO następujące równania ($\theta=\pi+\phi$, gdzie
$\phi$ reprezentuje małe odchylenia wahadła, a u to wejście układu):\
\
$u=m\ddot{x}+b\dot{x}-ml\ddot{\phi}$\
$ml\ddot{x}=(I+ml^2)\ddot{\phi}-mgl\phi$\

Opis układu w przestrzeni zmiennych stanu:\
\
$\dot{x}=Ax+Bu$\
$y=Cx+Du$\
\
\
$
     \begin{bmatrix}
       \dot{x} \\
       \ddot{x} \\
       \dot{\phi} \\
       \ddot{\phi}
     \end{bmatrix}
     =
     \begin{bmatrix}
       0 & 1 & 0 & 0          \\
       0 & \frac{-(I+ml^2)b}{Im} & \frac{mgl^2}{I} & 0 \\
       0 & 0 & 0 & 1 \\
       0 & \frac{-lb}{I} & \frac{mgl}{I} & 0 
     \end{bmatrix}
     \begin{bmatrix}
       x \\
       \dot{x} \\
       \phi \\
       \dot{\phi}
     \end{bmatrix}
     +
     \begin{bmatrix}
       0 \\
       \frac{I+ml^2}{Im} \\
       0 \\
       \frac{l}{I}
     \end{bmatrix}
     u\\
     y=
     \begin{bmatrix}
       1 & 0 & 0 & 0 \\
       0 & 0 & 1 & 0 
     \end{bmatrix}
     \begin{bmatrix}
       x \\
       \dot{x} \\
       \phi \\
       \dot{\phi}
     \end{bmatrix}
     +
     \begin{bmatrix}
       0 \\
       0 
     \end{bmatrix}
     u
$\
\

Po wstawieniu liczb:\
\
$
     \begin{bmatrix}
       \dot{x} \\
       \ddot{x} \\
       \dot{\phi} \\
       \ddot{\phi}
     \end{bmatrix}
     =
     \begin{bmatrix}
       0 & 1 & 0 & 0          \\
       0 & -0.2877 & 18.4167 & 0 \\
       0 & 0 & 0 & 1 \\
       0 & -1.37 & 134.4285 & 0 
     \end{bmatrix}
     \begin{bmatrix}
       x \\
       \dot{x} \\
       \phi \\
       \dot{\phi}
     \end{bmatrix}
     +
     \begin{bmatrix}
       0 \\
       2.8769 \\
       0 \\
       13.7
     \end{bmatrix}
     u\\
     y=
     \begin{bmatrix}
       1 & 0 & 0 & 0 \\
       0 & 0 & 1 & 0 
     \end{bmatrix}
     \begin{bmatrix}
       x \\
       \dot{x} \\
       \phi \\
       \dot{\phi}
     \end{bmatrix}
     +
     \begin{bmatrix}
       0 \\
       0 
     \end{bmatrix}
     u
$

Elektronika
===========

Zasilanie
---------

### Akumulator

Do zasilania został użyty pakiet ogniw litowo-polimerowych firmy Redox.
Akumulator został wybrany ze względu na to, że dostarcza dużo energii
przy małych rozmiarach, a także może być stale obciążony dużym prądem.\
Dane techniczne:

-   napięcie nominalne: 11,1 V (pełne naładowanie - 12,6 V),

-   pojemność: 2200 mAh,

-   prąd rozładowania: 30 C (66 A),

-   wymiary: 115 x 34 x 20 mm,

-   masa: 172 g.

W najbardziej pesymistycznym przypadku robot może pobierać do 5 A
ciągłego prądu. Przy pojemności 2200 mAh daje to minimalny czas pracy
około 26 minut. Natomiast w przypadku zwykłej jazdy, bez przeszkód robot
może pobrać ok. 1 A, co przekłada się na pięciokrotnie dłuższe
działanie. W 130 minut powinno dać się zmapować kilka razy w ciągu nocy
całe piętro budynku.

![image](\ImgPath/lipo.jpg)

[schematKomunikacji]

Napięcie z akumulatora jest bezpośrednio przekazywane tylko do silników
(na dwukanałowy mostek), a na układy logiczne dostarczane jest poprzez
stabilizatory liniowe 7805 na 5 V i LD1117VC33 na 3.3 V. Stabilizatory
są w obudowach TO-220, co ułatwia ich chłodzenie. Zasilają części
logiczne, więc wbudowana blaszka odprowadzająca ciepło wystarczy, żeby
cały układ się nie przegrzał. Do stabilizatorów zostały dołączone
kondensatory MKT 330 nF i ceramiczne 100 nF zgodnie z notą katalogową.

![image](\ImgPath/7805.PNG)

[schematKomunikacji]

![image](\ImgPath/7805e.PNG)

[schematKomunikacji]

![image](\ImgPath/ld1117.PNG)

[schematKomunikacji]

![image](\ImgPath/ld1117e.PNG)

[schematKomunikacji]

Napięcie 5 V jest kierowane do sterownika silników z mikrokontrolerem
ATmega168PA-PU, a także do zasilania płytki NUCLEO z mikrokontrolerem
STM32F103RBT6. Napięcie 3,3 V służy do zasilenia niektórych czujników i
układów, na przykład modułu WiFi ESP8266.

### Ładowarka

Ładowanie pakietów ogniw litowo-polimerowych wymaga stosowania ładowarek
mikroprocesorowych z odpowiednim algorytmem ładowania, wyposażonych w
układy balancerów, które równomiernie ładują każde ogniwo. W celu
uniknięcia trwałych uszkodzeń ogniwa powinno się nie rozładowywać go
poniżej 3V, czyli w przypadku całego pakietu napięcie nie powinno
osiągnąć wartości poniżej 9 V.

W celu zachowania powyższych zasad została zastosowana ładowarka
sieciowa firmy Redox. Specyfikacja:

-   Napięcie wejściowe: 100 - 240 V AC 50 / 60 Hz,

-   Prąd ładowania: do 1000 mA,

-   Obsługiwane pakiety: 2-3 ogniwowe litowo-polimerowe (7,4 V i 11,1
    V),

-   Wbudowany balancer ogniw,

-   Długość przewodów: 150 cm,

-   Wymiary: 96 x 55 x 33 mm.

![image](\ImgPath/ladowarka.jpg)

[schematKomunikacji]

Zabezpieczenie elektroniki
--------------------------

Elektronika może ulec uszkodzeniu ze względu na wiele czynników: silne
pole magnetyczne, wysoka temperatura, za duży prąd lub napięcie. W
układzie zostało zastosowanych kilka zabezpieczeń:

-   Akumulator jest podłączony do przełącznika na prąd do 15 A, który
    służy jako główny włącznik robota,

-   Dodatni sygnał z akumulatora przechodzi przez szeregowo wpięty
    rezystor 0,1 $\Omega$ 5 W, który ogranicza przepływ prądu do 7 A, ze
    względu na swoją wytrzymałość. Służy on również do pomiaru prądu
    przez przetwornik analogowo-cyfrowy mikrokontrolera ATMega168PA-PU,
    jeżeli prąd osiągnie kosztownie wysoką wartość, to układ scalony ma
    możliwość obniżenia go do bezpiecznej normy,

-   do dwukanałowego mostka w sterowniku silników, został dołączony
    radiator w formie dwóch lekkich blaszek aluminiowych (fragmentów
    ościeżnicy)

Sterownik silników prądu stałego
--------------------------------

Do sterowania silników DC został wybrany mikrokontroler ATmega168PA-PU w
obudowie DIP28. Sterowanie odbywa się za pomocą nadawania sygnału PWM na
piny dwukanałowego mostka L298N. Do układu mostków i mikrokontrolera
zostały dołączone filtry poprzez umieszczenie kondensatorów i dławików
zgodnie z notami katalogowymi. Dodatkowo do L298N zostały dołączone
diody, które były wymagane do prawidłowego działania układu.\
Do łatwego przeprogramowywania mikrokontrolera na płytce znajduje się
złącze programatora zgodnego z USBASP o 10 pinowym standardzie KANDA.

![image](\ImgPath/silnikie.PNG)

[schematKomunikacji]

### Mikrokontroler ATMega168PA-PU

Mikrokontroler ATMega168PA-PU 8-bitowy z rodziny AVR w obudowie DIP28
został wybrany ze względu na niskie zapotrzebowanie mocy obliczeniowej w
układzie sterownika silników, a co się z tym wiąże układ może być bardzo
tani.\
Dane techniczne:

-   taktowanie: do 20 MHz,

-   pamięć Flash: 16 kB,

-   pamięć RAM: 1 kB,

-   23 linie wyjścia/wejścia,

-   2 timery 8-bitowe,

-   1 timer 16-bitowy,

-   6 kanałów 10-bitowego przetwornika analogowo-cyfrowego,

-   sprzętowe interfejsy komunikacyjne: USART, SPI, 2-wire (I2C).

![image](\ImgPath/atmega.jpg)

[schematKomunikacji]

![image](\ImgPath/atmegapins.PNG)

[schematKomunikacji]

![image](\ImgPath/kanda.jpg)

[schematKomunikacji]

![image](\ImgPath/programatoravr.jpg)

[schematKomunikacji]

Mikrokontroler jest programowany poprzez dzisięciopinowe gniazdo KANDA
połączone z programatorem zgodnym z USBasp. Programator ma wyjście Mini
USB, co pozwala na bezproblemowe podłączenie do komputera PC.\
Do prawidłowego działania układu wymaganych jest kilka dodatkowych
elementów, zgodnie z notą katalogową:

-   kondensatory ceramiczne 100 nF między piny AREF a GND, VCC a GND i
    AVCC a GND,

-   kondensator elektrolityczny 16 V między piny VCC a GND,

-   dławik między piny AVCC i VCC,

-   rezystor 10 $k\Omega$ 0,25 W podciągający pin RESET do VCC,

-   rezonator kwarcowy 20 MHz do pinów XTAL1 i XTAL2 z dołączonymi
    kondensatorami ceramicznymi 12 pF.

![image](\ImgPath/usbasp.jpg)

[schematKomunikacji]

Układ ATMega168PA-PU jest wykorzystywany do sterowania prędkościami
silników za pomocą regulatora PID i sprzężenia zwrotnego w postaci
odczytu położenia kół z enkoderów magnetycznych znajdujących się przy
silnikach. Wartość zadana skorygowana o uchyb jest przekazywana w
postaci sygnału PWM na wejścia dwukanałowego mostka H w postaci układu
scalonego L298N. Na przetwornik analogowo cyfrowy ADC0 jest podawane
napięcie na rezystorze wejściowym 0,1 $\Omega$ wpiętym szeregowo przy
dodatnim złączu akumulatora, co pozwala na pomiar całkowitego prądu
płynącego w układzie. W przypadku osiągnięcia dużych wartości, pobór
prądu zostanie programowo zmniejszony. Mikrokontroler zawiera również
kilka połączeń z drugim mikrokontrolerem STM32F103RBT6 za pomocą pinów
PC1-PC3, zawierających informacje o kierunku i szybkości jazdy.

![image](\ImgPath/atmegapol.PNG)

[schematKomunikacji]

### Dwukanałowy mostek H - układ L298N

Sam układ scalony L298N, nie w gotowym module, został zastosowany ze
względu na niewielką cenę. Do prawidłowego działania należało jedynie
dokupić, według noty katalogowej, 8 diod prostowniczych, 1 kondensator
elektrolityczny i 1 kondensator ceramiczny 100 nF.

![image](\ImgPath/l298n.jpg)

[schematKomunikacji]

![image](\ImgPath/l298npol.PNG)

[schematKomunikacji]

### Enkodery magnetyczne CPR 48

Zastosowane enkodery są zintegrowane z używanymi silnikami. Ich
działanie jest oparte na zjawisku Halla - sensory wykrywają impulsy na
obracającej się tarczy magnetycznej umieszczonej z tyłu silnika. Używane
enkodery kwadraturowe posiadają rozdzielczość 48 impulsów na obrót, co
po przemnożeniu przez wartość przekładni 9,7:1 - daje wynik prawie 466
impulsów na obrót.

![image](\ImgPath/enkoder.jpg)

[schematKomunikacji]

Wyprowadzenia silników:

-   czerwony - zasilanie silnika 1,

-   czarny - zasilanie silnika 2,

-   zielony - potencjał masy enkodera,

-   niebieski - zasilanie enkodera, tolerancja od 3.5 V do 20 V,

-   żółty - wyjście A enkodera,

-   biały - wyjście B enkodera,

### Wykonanie płytki PCB

Oprócz schematu połączeń elementów elektronicznych w programie EAGLE
został również wykonany projekt płytki PCB:

![image](\ImgPath/silnikip.PNG)

[schematKomunikacji]

![image](\ImgPath/silnikid.PNG)

[schematKomunikacji]

Projekt płytki PCB został wyeksportowany do pliku PDF, aby uniknąć
jakiegokolwiek przeskalowania elementów. Wydruk został przeprowadzony na
drukarce laserowej na papierze kredowym. Wykorzystanie tego typu papieru
umożliwia przeniesienie toneru na laminat pokryty miedzią za pomocą
techniki termotransferu. Wydruk i papier zostały wyprasowane w
temperaturze około 220 C przez około 7 minut. Tak wysoka temperatura
była konieczna ze względu na użytą płytkę pokrytą dwustronnie miedzią -
większa ilość metalu pochłania więcej ciepła. Następnie wyprasowane
elementy zostały zanurzone na 15 minut w wodzie, a po rozmiękczeniu
papieru został on delikatnie usunięty z płytki. Po powyższych
czynnościach został usunięty osiadły kamień za pomocą zwykłego noża, a
także zostały poprawione wszelkie niedoskonałości powstałe podczas
termotransferu. W pełni przygotowana płytka została poddana trawieniu w
roztworze nadsiarczanu sodu przez ponad 30 minut, pozwoliło to na
usunięcie miedzi z miejsc niepokrytych tonerem. Na koniec zostały
wywiercone otwory do montażu elementów elektronicznych przewlekanych
przy użyciu wiertła o średnicy 1 mm.

Po ukończeniu płytki, przewody i uprzednio przetestowane elementy
elektroniczne zostały przylutowane. Po skończonym montażu elementów,
wszystkie ścieżki zostały sprawdzone za pomocą multimetru, czy ciągłość
obwodu jest zachowana tam, gdzie powinna, a także czy nie zachodzą
zwarcia. Następnie został przylutowany akumulator i nastąpiła
konieczność sprawdzenia wszystkich poziomów napięć na płytce. Po
upewnieniu się, że wszystko jest wykonane prawidłowo w podstawce
precyzyjnej został umieszczony mikrokontroler, a sama płytka
przymocowana do obudowy robota.

![image](\ImgPath/sterownikdcprzed.jpg)

[schematKomunikacji]

![image](\ImgPath/sterownikdcpo.jpg)

[schematKomunikacji]

Układ regulacji położenia i skanowania pomieszczeń
--------------------------------------------------

Do wykonywania większości obliczeń został wybrany mikrokontroler
STM32F103RBTF na płytce NUCLEO. Układ scalony zbiera informacje ze
wszystkich czujników robota: enkoderów magnetycznych, akcelerometru,
żyroskopu, czujników ultradźwiękowych i modułu WiFi. Następnie
wykorzystuje te dane do obliczenia aktualnej prędkości i kierunku
obrotów silników, aby utrzymać pion i zeskanować całe pomieszczenie.
Przeprogramowywanie mikrokontrolera jest ułatwione poprzez wbudowany
programator na płytce NUCLEO.

![image](\ImgPath/sterowaniee.PNG)

[schematKomunikacji]

### Mikrokontroler STM32F103RBT6

Mikrokontroler STM32F103RBT6 32-bitowej architektury ARM na płytce
NUCLEO z wyprowadzeniami pinów układu scalonego na złącza goldpin i
zintegrowany z programatorem został wybrany ze względu na łatwość
montażu i programowania. Architektura używanego mikrokontrolera jest
oprarta na rdzeniu Cortex M3, co oznacza dość dużą moc obliczenią przy
rozsądnej cenie.\
Dane techniczne NUCLEO-F103RB:

-   częstotliwość taktowania: 72 MHz,

-   pamięć programu Flash: 128 kB,

-   pamięć SRAM: 20 kB,

-   2 przetworniki analogowo-cyfrowe: 12-bitowe, 16-kanałowe,

-   7 timerów,

-   interfejsy: 3 USART, 2 SPI 18 Mbit/s, 2 I2C, USB Full Speed, CAN
    2,0B,

-   debugger ST-Link/V2 umieszczony na płytce,

![image](\ImgPath/stm32.jpg)

[schematKomunikacji]

![image](\ImgPath/nucleo.jpg)

[schematKomunikacji]

![image](\ImgPath/f103rb.PNG)

[schematKomunikacji]

Mikrokontroler jest programowany przez złącze Mini USB znajdujące się na
płytce. Układ scalony STM32F103RBT6 jest wykorzystywany do zbierania
sygnałów ze wszystkich czujników:

-   enkodery magnetyczne - sprzężenie zwrotne w postaci informacji o
    położeniu kół do układu regulacji położenia w pionie,

-   akcelerometr i żyroskop - sprzężenie zwrotne w postaci informacji o
    przyspieszeniu kątowym i kącie pochylenia robota do układu regulacji
    położenia w pionie,

-   czujniki ultradźwiękowe - informacje o położeniu przeszkód na drodze
    - do układu skanowania pomieszczeń,

-   moduł WiFi - odbieranie informacji o błędach z komputera PC.

Zadaniem mikrokontrolera jest przede wszystkim wykorzystać informacje o
przebytej drodze i pochyleniu robota do utrzymania pozycji pionowej za
pomocą regulatora liniowo-kwadratowego. Sygnały z czujników jako zmienne
stanu dodatkowo przechodzą przez filtr Kalmana, co zwiększa ich
dokładność. Drugim zadaniem układu scalonego jest zebranie informacji o
aktualnej pozycji przeszkód znajdujących się przed robotem i wysłanie
ich przez moduł WiFi do sieci lokalnej (co może być odebrane przez
dowolne urządzenie obsługujące protokół HTTP). Po każdorazowym wykonaniu
tych czynności obliczone wartości prędkości i kierunku obrotu kół są
wysyłane do sterownika silników.\
Połączenie płytki NUCLEO z resztą układu nie wymagało żadnych
dodatkowych elementów pasywnych.

### Żyroskop i akcelerometr - układ MPU6050

Użyty moduł firmy Invensense zawiera wbudowany cyfrowy żyroskop i
akcelerometr. Jego zastosowanie wynika z niskiej ceny i dostatecznych
parametrów pracy:

-   napięcie zasilania: 3,3V - 5V,

-   zakresy pracy żyroskopu: 250 dps, 500 dps, 1000 dps, 2500 dps (dps -
    degrees per second, stopni/sekundę),

-   zakresy pracy akcelerometru: 2g, 4g, 8g, 16g,

-   wymiary: 20mm x 16mm,

-   waga: 0,9 g.

![image](\ImgPath/mpu6050.jpg)

[schematKomunikacji]

Komunikacja z modułem odbywa się za pośrednictwem magistrali szeregowej
I2C. Ścieżki SDA i SCL zostały podciągnięte do napięcia zasilania, aby
uniknąć stanów nieokreślonych na pinach.

![image](\ImgPath/mpue.PNG)

[schematKomunikacji]

![image](\ImgPath/mpuumieszczenie.jpg)

[schematKomunikacji]

### Czujniki ultradźwiękowe HC-SR04

Użyte czujniki HC-SR04 zostały zakupione razem z uchwytami montażowymi.
Ich zastosowanie wynika z niskiej ceny i dostatecznych parametrów pracy:

-   napięcie zasilania: 5 V,

-   średni pobór prądu: 15 mA,

-   zakres pomiarowy: od 2 cm do 200 cm,

-   częstotliwość pracy: 40 kHz,

-   wymiary: 45 x 20 x 15 mm.

![image](\ImgPath/hcsr04.jpg)

[schematKomunikacji]

Komunikacja z czujnikami odbywa się za pośrednictwem dwóch pinów: ECHO i
TRIG. Wynika to z działania układów. Po podaniu stanu wysokiego na pin
TRIG w postaci impulsu trwającego moduł wykonuje pomiar odległości.

Aby rozpocząć pomiar należy podać na pin TRIG impuls napięciowy (stan
wysoki 5V) przez . Moduł dokonuje pomiaru odległości przy pomocy fali
dźwiękowej o częstotliwości 40 kHz. Na pinie ECHO otrzymywany jest
sygnał, w którym odległość od przeszkody jest zależna od czasu trwania
stanu wysokiego. Odległość w milimetrach od przeszkody wynosi:\
$d = 0,17t_{ECHO}$,\
gdzie:\
d - odległość mierzona,\
$t_{echo}$ - czas trwania stanu wysokiego na pinie ECHO.\
\
Wzór jest wyprowadzony z prostej zależności:\
$d = (t_{echo} × V_{sound})/2$,\
gdzie:\
$V_{sound}$ - prędkość rozchodzenia się dźwięku w powietrzu - 340 m/s.

![image](\ImgPath/hcsrumieszczenie.jpg)

[schematKomunikacji]

W zestawie znajdują się specjalne uchwyty montażowe, do których zostały
zaprojektowane mocowania na szczycie konstrukcji robota. Model w 3D
został zaprojektowany w ten sposób, aby czujniki ultradźwiękowe były
ustawione: idealnie na wprost jazdy, pod kątem 45 w prawo i 45 w lewo.
Czujnik najlepiej działa w polu do 30 od osi prostopadłej do modułu.
Stąd od odległości ok. 24 cm od konstrukcji robot będzie miał widoczność
w pełnym zakresie 180.

![image](\ImgPath/hcsrzasieg.PNG)

[schematKomunikacji]

![image](\ImgPath/widocznosc.jpg)

[schematKomunikacji]

4![image](\ImgPath/hcsr04e.PNG)

[schematKomunikacji]

### Moduł WiFi ESP-01 8266

Moduł został wybrany ze względu na niską cenę, powszechne użycie i du żą
moc obliczeniową. Dane techniczne:

-   zasilanie: 3,3 V,

-   pamieć Flash: 1 MB,

-   2 GPIO - wyjścia/wejścia cyfrowe,

-   1 UART,

-   wbudowana antena PCB,

-   wymiary: 24,8 x 16 mm.

![image](\ImgPath/esp.jpg)

[schematKomunikacji]

Komunikacja z modułem odbywa się za pośrednictwem interfejsu UART - piny
RX i TX. Do prawidłowego działania układu został dołączony kondensator
ceramiczny między napięcie zasilania a masę. Piny Reset i CH\_PD zostały
podciągnięte do napięcia zasilania modułu, aby dodatkowo ustabilizować
pracę urządzenia.

![image](\ImgPath/espe.PNG)

[schematKomunikacji]

![image](\ImgPath/esp01.jpg)

[schematKomunikacji]

Do zaprogramowania modułu należy użyć pinów UART i zewrzeć pino GPIO0 do
masy. Z powodu napięcia zasilania ESP8266 3,3 V konieczne jest
zastosowanie konwertera napięć, aby podłączyć moduł przez interfejs USB
do komputera PC. Dla podwyższonej wygody w tym wypadku został użyty
konwerter UART-USB oparty na układzie PL2303, który może pracować na
dwóch napięciach 3,3 V i 5 V (wybór napięcia za pomocą zworki).\
W ESP8266 został wgrany firmware NodeMCU w wersji:
nodemcu\_float\_0.9.6-dev\_20150704 za pomocą programu ESP8266Flasher.
Nowy firmware umożliwia programowanie modułu za pomocą języka
skryptowego Lua, a także w środowisku Arduino, z wykorzystaniem
biblioteki esp8266 by ESP8266 Community w wersji 2.3.0-rc2

![image](\ImgPath/konwerter.jpg)

[schematKomunikacji]

### Wykonanie płytki PCB

Płytka PCB układu regulacji położenia i skanowania pomieszczeń została
wykonana w analogiczny sposób jak płytka sterownika silników prądu
stałego. Zamiast podstawki precyzyjnej pod mikrokontroler zostały
zamontowane żeńskie gniazda goldpin, w których można umieścić płytkę
NUCLEO-F103RB.

![image](\ImgPath/sterowaniep.PNG)

[schematKomunikacji]

![image](\ImgPath/sterowanied.PNG)

[schematKomunikacji]

![image](\ImgPath/sterowanie1.jpg)

[schematKomunikacji]

![image](\ImgPath/sterowanie2.jpg)

[schematKomunikacji]

![image](\ImgPath/sterowanie3.jpg)

[schematKomunikacji]

Teoria Sterowania
=================

Wykorzystując obliczony model matematyczny układu został zaprojektowany
układ sterowania robota w programach Matlab i Simulink. Dobrze
zamodelowany układ pozwoli znacznie szybciej dobrać prawidłowe wartości
macierzy Q i R w regulatorze liniowo-kwadratowym LQR.

Wyznaczenie parametrów układu
-----------------------------

Używając gotowych funkcji w Matlabie obliczyłem rzędy macierzy
obserwowalności i sterowalności - wynoszą 4. Oznacza to, że
skonstruowany układ jest sterowalny i obserwowalny.\
Układ bez pętli sterowania jest niestabilny - posiada dodatni biegun:\
\
$\begin{bmatrix}
0 \\
-11.6894\\
-0.1000\\
11.5017
\end{bmatrix}$  \
\
Niestabilność układu można zilustrować zasymulowaną odpowiedzią skokową,
po zadaniu impulsu dąży do nieskończoności.\

![image](\ImgPath/step.jpg)

[schematKomunikacji]

Projekt regulatora liniowo-kwadratowego (LQR)
---------------------------------------------

Zastosowanie regulatora LQR zapewnia optymalne sterowanie, poprzez
zapewnienie dopasowanego wzmocnienia w pętli sprzężenia zwrotnego, ale
wymaga dokładnie wyliczonego modelu układu. Założenie projektowe to czas
ustalenia odpowiedzi poniżej 1 sekundy.\
Regulator został zamodelowany w programie Simulink:

![image](\ImgPath/lqr.PNG)

[schematKomunikacji]

Macierz wzmocnień K sprzężenia zwrotnego jest obliczana rozwiązując
równanie różniczkowe Riccatiego o postaci:\
\
$K=R^{-1}B^TP(t)$\
$\dot{P}(t)=A^TP(t)+P(t)A-P(t)BR^{-1}B^TP(t)+Q$\

Gdzie Q i R to macierze wagowe wskaźnika J. Wartości macierzy Q i R
zostały dobrane tak, aby spełnić założenie projektowe:

-   Przemieszczenie robota w przybliżeniu nie większe niż 8 cm:\
    $Q_{11} = 1/0.08^2$,

-   Prędkość robota nie większa niż 3 m/s: $Q_{22} = 1/3^2$,

-   Mały kąt wychylenia wahadła nie większy niż 15: $Q_{33} = 1/15^2$,

-   Prędkość kątowa wychylenia wahadła nie większa niż 30/s:\
    $Q_{44} = 1/30^2$.

Wartości pozostałych elementów macierzy Q przyjąłem jako zerowe –
uzyskując w ten sposób macierz symetryczną. Podobnie dla macierzy R,
która ma wymiary 1x1, jest to kwadrat odwrotności największej siły
zadziałającej na wahadło czyli ok. 2 N, więc $R = [1/2^2]$.\

Odpowiedź układu na skok jednostkowy:

![image](\ImgPath/lqrstep.PNG)

[schematKomunikacji]

Układ jest stabilny i spełnia założenia projetkowe.\
\
Obliczone Macierze Q i R:\
\
$Q=\begin{bmatrix}
       156.2500 & 0 & 0 & 0          \\
       0 & 0.1111 & 0 & 0 \\
       0 & 0 & 0.0044 & 0 \\
       0 & 0 & 0 & 0.0011 
     \end{bmatrix}
$ , $R = [0.2500]$\
\
\
Obliczone wzmocnienie LQR:\

$K=\begin{bmatrix}
       -25.0000 & -12.6331 & 40.5490 & 4.9073
     \end{bmatrix}
$\
\
Symulacja udowadnia, że teoretycznie jest możliwe ustablizowanie takiego
obiektu w pionie.

Oprogramowanie
==============

Robot został podzielony na kilka modułów, stąd konieczność napisania
wielu programów. Każdą część można traktować jako osobny element, co
umożliwia wykonywanie testów pojedynczych modułów, bez konieczności
podłączania ich do reszty podzespołów. Takie podejście ułatwia wykrycie
błędu w całym systemie i zapewnia lepszą diagnostykę.\
Łączna objętość kodu to 2538 linii.

Projekt sterowania położenia w pionie (Matlab, Simulink)
--------------------------------------------------------

Projekt sterowania został napisany w Matlabie i Simulinku, ponieważ
umożliwia to łatwe zaprojektowanie systemu za pomocą bloków oraz
testowanie różnych wariantów i parametrów sterowania.\
Cały program został zapisany w jednym pliku (62 linie). Składowe
programu lqr.m:

-   wyliczenia macierzy układu przedstawionych w rozdziale 2.7,

-   obliczenia sterowalności, obserwowalności, biegunów i zasymulowania
    odpowiedzi skokowej układu, przedstawionych w rozdziale 4.1,

-   obliczenia wzmocnienia LQR i zasymulowania działania na
    zamodelowanym układzie robota (rozdział 4.2).

Do obliczenia sterowalności, obserwowalności, biegunów, wzmocnienia LQR
i zasymulowania odpowiedzi skokowej zostały wykorzystane funkcje z
biblioteki Matlaba, odpowiednio:

-   ctrb() - sterowalność, zwraca rząd macierzy sterowalności,

-   obser() - obserwowalność, zwraca rząd macierzy obserwowalności,

-   pole() - oblicza bieguny układu,

-   lqr() - oblicza wzmocnienie LQR,

-   step() - symulacja odpowiedzi skokowej układu.

Sterownik silników prądu stałego (C, AVR)
-----------------------------------------

Program został napisany w środowisku Eclipse Mars, z pluginem AVR, w
języku ANSI C. Zegar mikrokontrolera ATMega168PA-PU został ustawiony na
20 MHz, generowany z zewnętrznego rezonatora kwarcowego.\
Użyte biblioteki:

-   avr/interrupt.h - biblioteka AVR do obsługi przerwań,

-   avr/iom168.h - biblioteka AVR do pinów wejścia/wyjścia dla
    mikrokontrolera ATmega168,

-   util/delay.h - dla funkcji \_delay\_ms(), która pozwala wprowadzić
    opóźnienia do programu.

Cały program został zapisany w jednym pliku - main.c (157 linii).
Składowe programu:

1.  Dyrektywy preprocesora dla użytych pinów i pomocniczych makr.\
    Wpisanie wartości sprzętowych, takich jak, użyte piny w dyrektywach
    preprocesora umożliwia łatwe przeprowadzenie zmian w programie, w
    przypadku zmiany któregoś pinu i sprawia, że program staje się
    bardziej uniwersalny.

2.  Deklaracje zmiennych globalnych, słowo kluczowe volatile zapobiega
    optymalizacji zmiennych przez kompilator, co jest konieczne, w
    przypadku, gdy zmienne są używane w przerwaniach.

3.  Funkcja uint16\_t measure(uint8\_t channel) umożliwiająca pomiar
    napięcia dla dowolnego pinu ADC mikrokmontrolera (zakres od 0 do 5),
    za pomocą działań na bitach.

4.  Funkcja void hardware\_setup() ustawiająca porty jako
    wejście/wyjście, konfigurująca ADC, TIMER2, i wyjścia na mostek
    L298N oraz uruchamia globalną flagę przerwania.\
    Konfiguracja ADC: ADEN - włączenie ADC, ADPSX - prescaler (w tym
    przypadku ustawiony na 8), REFS0 - ustawienie napięcia odniesienia
    jako AVCC z dołączonym kondensatorem.\
    Konfiguracja TIMER2: WGM21 - ustawienie trybu CTC, CS21 - prescaler
    8, OCR2A - dodatkowy podział częstotliwości przez 200, OCIE2A -
    ustawienie porównania do rejestru OCR2A\
    sei() - umożliwia używanie przerwań w programie.

5.  Funkcja void gather\_data(), która odbiera dane od mikrokontrolera
    STM32F103RBT6,

6.  Funkcja void set\_direction() - ustala kierunek obrotu silników na
    podstawie danych odebranych funkcją gather\_data(),

7.  Procedura obsługi przerwania ISR( TIMER2\_COMPA\_vect ) do obsługi
    programowego PWM,

8.  Główna funkcja programu - int main(void), która zajmuje się
    ustawianiem odpowiedniego kierunku obrotów silnika i utrzymywania
    prędkości silników za pomocą sygnału PWM. Konieczne było
    zastosowanie opóźnienia 2 ms, aby umożliwić stabilną pracę
    mikrokontrolera.

Obsługa czujników i sterowanie położenia w pionie (C, StdPeriph)
----------------------------------------------------------------

Główny program sterujący został zaimplementowany w środowisku System
Workbench for STM32 opartym na programie Eclipse. Kod został napisany w
języku ANSI C.\
Struktura plików programu:

-   stm32f1xx\_it.h - plik wygenerowany przez środowisko System
    Workbench do kompilacji projektu (61 linii),

-   syscalls.c - plik wygenerowany przez środowisko System Workbench do
    minimalnej obsługi zawołań (calls), 204 linie kodu,

-   system\_stm32f10x.c - plik wygenerowany przez środowisko System
    Workbench, główny kod konfiguracyjny dla mikrokontrolera
    (częstotliwość taktowania, początkowe ustawienie głównych
    rejestrów), 379 linii,

-   HAL\_MPU6050.h - plik nagłówkowy biblioteki do obsługi I2C (MIT
    license Copyright (c) 2012 Harinadha Reddy Chintalapalli)
    zawierający konfigurację sprzętową - wybór pinów i kanału I2C
    mikrokontrolera (65 linii),

-   MPU6050.h - plik nagłówkowy biblioteki do obsługi I2C i modułu
    akcelerometra i żyroskopu MPU6050 (MIT license Copyright (c) 2012
    Harinadha Reddy Chintalapalli), zawiera definicję wszystkich
    rejestrów MPU6050 (428 linii),

-   MPU6050.c - zawiera kod źródłowy dla biblioteki obsługującej I2C i
    moduł MPU6050 (MIT license Copyright (c) 2012 Harinadha Reddy
    Chintalapalli), obsługuje całą komunikację z MPU6050 i udostępnia
    funkcję void MPU6050\_GetRawAccelGyro(s16\* AccelGyro), która
    pobiera dane akcelerometru i żyroskopu (465 linii),

-   esp8266.h - plik nagłówkowy do obsługi modułu Wifi ESP8266 (21
    linii),

-   esp8266.c - zawiera kod źródłowy części obsługującej moduł WiFi
    ESP8266, czyli konfigurację UART (53 linie),

-   main.c - plik z programem głównym, zawiera obsługę enkoderów,
    komunikację z ATmega168PA-PU i MPU6050 oraz sterowanie położenia w
    pionie (408 linie).

Program składa się z 482 linii autorskiego kodu, 644 linii kodu
wygenerowanego i 958 linii kodu bibliotecznego, co przekłada się na
łączną sumę 2084 linii. Po skompilowaniu program zużywa 35056 bajtów
miejsca w pamięci flash mikrokontrolera.

### Plik HAL\_MPU6050.h

Komunikacja z modułem MPU6050 została skonfigurowana dla kanału
pierwszego magistrali I2C mikrokontrolera STM32F103RBT6. Prędkość
transmisji danych to 100 kHz.

### Plik esp8266.c

Komunikacja z modułem WiFi ESP8266 została skonfigurowana dla kanału
trzeciego USART, baud rate 115200, 8 bitów danych, brak kontroli
parzystości i 1 bit stopu. Transmisja jest inicjalizowana przez wysłanie
pakietu zawierającego same zera.\
Zaimplementowanie funkcji int \_\_io\_putchar(int c) pozwala na używanie
funkcji formatowanego wydruku danych printf() dla transmisji USART.

### Plik main.c

Plik główny programu zbiera dane ze wszystkich czujników co wiąże się z
użyciem kilku bibliotek:

-   math.h - biblioteka matematyczna, wykorzystywana do obliczania
    dwuargumentowego arcusa tangensa,

-   stdbool.h - umożliwia używanie w języku C logicznego typu danych
    bool,

-   stm32f10x.h - biblioteka środowiska System Workbench for STM32
    zawierająca definicje rejestrów i obsługi peryferiów mikrokontrolera
    STM32F103RBT6,

-   MPU6050.h - biblioteka do obsługi modułu żyroskopu i akcelerometra
    MPU6050,

-   esp8266.h - biblioteka do modułu WiFi opisana w części 5.3.2.

Kod składa się z kilku części:

1.  Deklaracja stałych mechanicznych, matematycznych i elektronicznych:

    -   ROTATION\_CONSTANT wyliczone doświadczalnie na podstawie
        pomiarów odległości z enkoderów, tak aby dać jak
        najdokładniejszy wynik,

    -   WHEEL\_DIAMETER - średnica kół w centymetrach,

    -   PI - liczba PI z dokładnościa do 6 miejsc po przecinku,

    -   MPU\_CONSTANT - stała, przez która należy podzielić wyniki z
        MPU6050, aby otrzymać jednostki w układzie SI,

    -   ACCELEROMETER\_SENSITIVITY - czułość akcelerometru,

    -   GYROSCOPE\_SENSITIVITY - czułość żyroskopu,

    -   dt - czas pomiędzy odczytami pomiarów z MPU6050.

2.  Deklaracje struktur, zmiennych i flag globalnych.

3.  Funkcja void delay\_ms(int time) - wprowadza opóźnienia do programu.

4.  Funkcja void SysTick\_Handler() - obsługa przerwania od zegara,
    częstotliwość przerwania zdefiniowana w funkcji
    atmega\_communication\_init() przy wywołaniu funkcji
    SysTick\_Config(). W programie funkcja SysTick\_Handler() wykonuje
    się co 1 ms. Wystawia flagi do obliczeń w pętli głównej programu.

5.  Funkcja void global\_variables\_init() - inicjalizuje wszystkie
    zmienne globalne.

6.  Funkcja void set\_pwm(TIM\_OCInitTypeDef channel, int pwm\_nr, int
    duty\_cycle) - ustawia wypełnienie sprzętowego PWM dla konkretnego
    pinu,

7.  Funkcja float check\_velocity(int encoder\_counter) do dokładnego
    pomiaru prędkości chwilowej,

8.  Funkcja void encoder\_init() - inicjalizuje porty, zegary i
    przerwania reagujące na zbocza narastające do obsługi enkoderów.

9.  Funkcja void atmega\_communication\_init() - uruchamia przerwanie
    obsługiwane przez funkcję SysTick\_Handler(), inicjalizuje porty,
    zegary, timery (tryb sprzętowej modulacji szerokości impulsu - PWM)
    do komunikacji z mikrokontrolerem ATmega168.

10. Funkcja void mpu\_read\_data() - odczytuje surowe dane z modułu
    MPU6050 i zapisuje je do globalnej struktury measuredData.

11. Funkcje void EXTI9\_5\_IRQHandler() i void EXTI15\_10\_IRQHandler()
    to procedury obsługi przerwania dla enkoderów, wystawiają flagi do
    obliczeń w pętli głównej programu.

12. Funkcja void hardware\_setup() - uruchamia wszystkie funkcje
    konfiguracyjne.

13. Funkcja void export\_data\_wifi() - wysyła dane ze struktury
    measuredData przez USART do modułu WiFi ESP8266.

14. Funkcja void export\_data\_atmega() - wysyła dane do sterownika
    silników (mikrokontroler ATmega168) przy pomocy 5 linii, na których
    jest wysyłany sygnał prostokątny ze zmiennym wypełnieniem. Sygnały
    są odbierane za pomocą przetwornika analogowo-cyfrowego.

15. Funkcja void ComplementaryFilter(float \*pitch) - implementacja
    filtru komplementarnego do filtrowania odczytu kąta pochylenia
    robota,

16. Funkcja główna int main(void) - zawiera definicje zmiennych
    lokalnych (głównie do regulatora PID) i pętlę główną programu, w
    której odbywa się sprawdzanie poszczególnych flag wystawianych w
    przerwaniach i wykonywanie kodu przeznaczonego do poszczególnych
    zadań:

    -   flagi l\_cnt\_flag i r\_cnt\_flag - obliczenie pozycji kół i
        kierunku poruszania się,

    -   flaga velocity\_flag - obliczenie drogi i prędkości średniej
        robota,

    -   flaga mpu\_flag - odczyt danych z MPU6050, obliczenie wyjścia
        regulatora PID i przekształcenie go na wypełnienie sygnału PWM
        oraz wysłanie wyników do sterownika silników,

    -   flaga uart\_flag - wysłanie danych przez USART do modułu ESP8266

W kodzie ze szczególną uwagą zadbano o wydajność:

-   zastosowanie sprzętowej modulacji szerokości impulsu (PWM)
    praktycznie w ogóle nie obciąża mikrokontrolera,

-   stosowanie zmiennych całkowitych ograniczając działania na liczbach
    zmiennoprzecinkowych,

-   stosowanie zmiennych typu unsigned, co przyspiesza obliczenia
    dwukrotnie,

-   wystawianie flag w przerwaniach i wykonywanie zadań w pętli głównej
    programu.

Obsługa modułu WiFi, serwer WWW (C++, HTML, CSS, JS, XML, Arduino)
------------------------------------------------------------------

### Kod C++

Program został napisany w środowisku Arduino 1.8.2, z wykorzystaniem
dodatkowego zestawu płytek esp8266 by ESP8266 Community w wersji
2.3.0-rc2, a dokładnie płytki Generic ESP8266 Module. Cały kod został
zapisany w jednym pliku - ajax.ino (325 linii) w języku C++.
Wykorzystane biblioteki:

-   ESP8266WiFi.h - umożliwia podłączenie się do konkretnej sieci WiFi,

-   ESP8266WebServer.h - umożliwia utworzenie serwera.

Składowe programu:

1.  Deklaracja zmiennych i stałych, a także serwera na porcie 80 - co
    oznacza konieczność obsługi protokołu HTTP przez klienta.

2.  Funkcja void parseData(MeasuredData &m, char\* buffer), która
    przetwarza wartości odebrane z mikrokontrolera STM32F103RBT6 -
    umieszcza odpowiednie dane w odpowiednich miejscach struktury
    MeasuredData.

3.  Funkcja void read\_uart() - odczytuje wartości wysłane z
    mikrokontrolera STM32F103RBT6. Format wysyłanych danych:\
    “x0,x1,x2,x3,x4,x5,x6,x7,x8,x9,x10,;”, w przypadku napotkania
    średnika odczytywanie się zakończy. Następnie jest wywoływana
    funkcja parseData().

4.  Funkcja void buildWebsite() - buduje stronę za pomocą kodu HTML i
    CSS.

5.  Funkcja void buildJavascript() - buduje stronę za pomocą kodu
    JavaScript.

6.  Funkcja void buildXML() - zbiera dane odczytane z interfejsu UART
    oraz sprawdza czas wykonywania programu i napięcie zasilania modułu
    za pomocą ADC - w Arduino jest to gotowa funkcja ESP.getVcc().
    Zebrane pomiary są zapisywane do dokumentu XML.

7.  Funkcja String millis2time() - oblicza czas wykonywania programu za
    pomocą gotowej funkcji w Arduino - millis() oraz zamienia go na
    godziny, minuty i sekundy.

8.  Funkcje void handleWebsite() i void handleXML() wysyłają dokumenty
    HTML i XML na server.

9.  Funkcja void setup() konfiguruje UART (baud rate 115200) do
    komunikacji z mikrokontrolerem STM32F103RBT6, podejmuje próbę
    połączenia z określoną siecią Wifi i wyświetla komunikaty.
    Najważniejszym komunikatem jest wyświetlenie WiFi.localIP(), czyli
    dynamicznie przydzielonego adresu IP w sieci lokalnej, dzięki
    któremu można się połączyć z serwerem. Aby sprawdzić IP konieczne
    jest podłączenie modułu do jakiegokolwiek terminala (baud rate
    115200), zresetowanie ESP8266 i sprawdzenie komunikatu.

10. Funkcja void loop() - główna pętla programu, zajmuje się
    odczytywaniem danych z UART i obsługą klienta (wyświetleniem
    zawartości strony).

### Kod HTML, CSS, JavaScript i XML

Dokument HTML został napisany w oparciu o języki HTML, CSS, JavaScript.
Dane są aktualizowane asynchronicznie przy pomocy dokumentu XML, dzięki
technice AJAX - Asynchronous JavaScript and XML. Dzięki takiemu
rozwiązaniu w polach wyświetlają się aktualne pomiary bez konieczności
odświeżania całej strony.\
Struktura dokumentu HTML:

1.  Część HEAD napisana w HTML i CSS. Został zastosowany system
    kodowania Unicode UTF-8 i ustawiony główny styl strony (font, tło).

2.  Część SCRIPT zawierająca kod JavaScript.\
    Funkcja createXmlHttpObject() - tworzy nowy obiekt XMLHttpRequest do
    obsługi zapytań asynchronicznych.\
    Funkcja process() - wykonywana co 200 ms, sprawdza czy obiekt
    XMLHttpRequest jest otwarty, jeżeli nie, to go otwiera przy pomocy
    metody PUT i przypisuje funkcję callback handleServerResponse(), gdy
    obiekt XMLHttpRequest zmieni stan na gotowy.\
    Funkcja handleServerResponse() - otwiera otrzymany dokument XML od
    obiektu XMLHttpRequest. Przegląda cały dokument i przypisuje
    odpowiednie wartości do pól w części BODY.

3.  część BODY napisana w HTML i CSS. Przy załadowaniu strony jest
    wywoływana funkcja JavaScript process(), która rozpoczyna proces
    przetwarzania otrzymywanych dokumentów XML. Dalej jest zapisana
    struktura strony podzielona na części DIV, w których znajdują się
    pola $<$A$>$$<$/A$>$ o unikalnych identyfikatorach, pozwalających
    skryptom JavaScript na lokalizację odpowiednich pól.

    Wykorzystywana wersja XML to 1.0. Dokument XML ma prostą strukturę -
    wszystkie znaczniki znajdują się w głównym znaczniku
    $<$Donnees$>$$<$/Donnees$>$, a każdy znacznik wewnątrz jednoznacznie
    identyfikuje pomiar.

Wnioski
=======

<span>**TODO**</span>

<span>99</span>
