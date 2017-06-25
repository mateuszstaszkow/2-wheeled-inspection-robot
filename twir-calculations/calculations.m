%% Dane uk³adu
M = 0; %masa wozka
m = 1; %masa wahadla
I = 0.01; %moment bezwladnosci wahadla
b = 0.1; %wsp. tarcia wozka o podloze
l = 0.137; %odleglosc do srodka masy wahadla
g = 9.8123; %wartosc przyspieszenia ziemskiego w Warszawie

%% Model uk³adu
A = [0 1 0 0
    0 (-b*(I+m*l*l))/(I*(M+m)+M*m*l*l) (m*m*g*l*l)/(I*(M+m)+M*m*l*l) 0
    0 0 0 1
    0 (-m*l*b)/(I*(M+m)+M*m*l*l) (m*g*l*(M+m))/(I*(M+m)+M*m*l*l) 0]

B = [0
    (I+m*l*l)/(I*(M+m)+M*m*l*l)
    0
    (m*l)/(I*(M+m)+M*m*l*l)]

C= [1 0 0 0
    0 0 1 0];
D = [0
    0];

%sprawdzienie obserwowalnoœci i sterowalnoœci
S=ctrb(A,B);
ster=rank(S)
O=obsv(A,C);
obser=rank(O)

sys=ss(A,B,C,D); %funkcja wyswietla macierze systemowe
bieguny=pole(sys) %obliczenie biegunow wielomianu charakterystycznego

step(sys,5); %odpowiedz ukladu otwartego na skok jednostkowy T =5sek
grid;

%% LQR

% Dobieranie jedynie wartosci elementów na diagonali macierzy Q i R w nastepujacy sposób:
% • Qii = 1/ max. akceptowalna wartosc [(Xi)^2, gdzie i to indeks dolny zmiennej ]
% • Rii = 1/ max. akceptowalna wartosc [(Ui)^2]

% stad przemieszczenie robota nie wieksze niz 8 cm,
% predkosc robota nie wieksza niz 3 m/s,
% maksymalne wychylenie robota ok. 15 stopni,
% predkosc katowa wychylenia robota ok. 30 stopni/sekunde,

%wagi poszczegolnych zmiennych
Q11 = 1/(0.08^2);
Q22 =1/(3^2);
Q33 =1/(15^2);
Q44= 1/(30^2);

Q=[Q11    0   0    0
   0    Q22   0    0
   0    0   Q33    0
   0    0     0   Q44]

%w macierzy R sa wagi sygnalu sterujacego si³y F, ktora powinna byc nie wieksza ni¿ ok. 2 N 
R=1/2^2

K = lqr(sys,Q,R)