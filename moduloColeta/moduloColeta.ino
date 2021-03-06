/* Ultima alteracao em 27/6/2016 */
/* Trabalhado com http POST header. */
///http://automatobr.blogspot.com.br/2014/03/dividindo-um-programa-sketch-do-arduino.html

#include <TimerOne.h>
#include <LiquidCrystal.h>
#include <Ethernet.h>
#include <SPI.h>
#include <avr/pgmspace.h>  //esta lib eh para economizar espaco com chars longas

//define o numero de amostras de cada captura
#define AMOSTRAS 64
//define a frequencia da rede (pode ser 50)
#define FREQ 60
// define os limites superior e infoerir para deteçcao de pico de tensao
#define TENSAO_LIMIT_S 640
#define TENSAO_LIMIT_I 400


// initialize the library with the numbers of the interface pins
//http://www.hobbytronics.co.uk/arduino-lcd-keypad-shield
// usando esta pinagem para arduinoMEga
LiquidCrystal lcd(41, 40, 33, 32, 31, 30);


//variaveis para medida de tempo
unsigned long microseconds,duration,T,tAmostra, tAmostraMinusADTime;
unsigned int vetorA[AMOSTRAS],vetormA[AMOSTRAS],vetorV[AMOSTRAS];
float vetorASemDC[AMOSTRAS],vetormASemDC[AMOSTRAS],vetorVSemDC[AMOSTRAS];
unsigned int timeOffset = 0;

uint16_t tempoLoop = 250,tmpVar;
byte sensorA = A1 ,sensormA = A0, sensorV = A3,led = 13;
boolean estadoLed = true,mAdbg=false,Adbg=false,Vdbg=false,Sdbg=false,rmsTestdbg=false,overflowDBG=false;
char cmd,tmpBuf[9];
//calibraço feita em 1/9/15 com multimetro do prof. Trentin
//utilizando rotina switch/case para ajuste fino de cada um dos ganhos.
//float ganhoA=8.52,ganhomA=156,ganhoV=79850;//ganhoV=52250;
float ganhoA=9,ganhomA=150,ganhoV=75850;//ganhoV=52250; //CHANGE IN 6/11/15

//inicializa com variavel rmsAnterior com corrente mnima
float rmsAnterior=0.001;
boolean houveAlteracaoRMS=false;
//variaveis para controle e teste das diferenças
byte numVezesDiferente=0,limiteDiferencas=3;

//http://www.microsmart.co.za/technical/2014/03/01/advanced-arduino-adc/
// Define various ADC prescaler
const unsigned char PS_16 = (1 << ADPS2);
const unsigned char PS_32 = (1 << ADPS2) | (1 << ADPS0);
const unsigned char PS_64 = (1 << ADPS2) | (1 << ADPS1);
const unsigned char PS_128 = (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);

//inicializaao REDE
byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(172,20,6,254); //meu IP 
IPAddress server(172,20,6,40); // ip do servidor
char serverName[] = "172.20.6.40";
byte serverPort = 80;
char pageName[] = "/captura.php";
EthernetClient client; //inicia objeto que vai conectar na rede
// set this to the number of milliseconds delay
// this is 30 seconds
#define delayMillis 30000UL


void setup(){
  Serial.begin(9600);
  pinMode(sensorA, INPUT);     
  pinMode(sensormA, INPUT);     
  pinMode(sensorV, INPUT);     

  initDisplay();

  // set up the ADC
  analogReference(EXTERNAL); //usando uma referncia externa - de 3.3V
  ADCSRA &= ~PS_128;  // remove bits set by Arduino library
  // you can choose a prescaler from above.
  // PS_16, PS_32, PS_64 or PS_128
  ADCSRA |= PS_32;    // set our own prescaler here

  //este teste define o periodo do ciclo conforme a frequencia da rede
  if(FREQ == 60){
    Serial.println("60");
    T = 16666; //tempo do ciclo em microssegundos 
  }
  else if(FREQ == 50){
    Serial.println("50");
    T = 20000; //tempo do ciclo em microssegundos 
  }
  //executa a primeira leitura de cada canal - isto  necessrio porque a primeira leitura  mais lenta que as outras.
  tmpVar =  analogRead(sensorA);
  tmpVar =  analogRead(sensormA);
  tmpVar =  analogRead(sensorV);

  //calcula o tempo de TRES leituras do AD no modo ACELERADO
  microseconds = micros();
  tmpVar =  analogRead(sensorA);
  tmpVar =  analogRead(sensormA);
  tmpVar =  analogRead(sensorV);
  duration = micros() - microseconds;

  tAmostra = T/AMOSTRAS; //calcula o maximo de tempo de cada amostra dentro do ciclo T amostra 
  // +2 para 64 e 128 (usando preescaler de 32) com 2 leituras
  // -1 para 64 e 128 (usando preescaler de 32) com 3 leituras (media de 16.58
  tAmostraMinusADTime = tAmostra-duration-1; //subtrai o tempo de cada amostra do tempo de converso.

  //mostra valores iniciais...
  Serial.print("T: ");
  Serial.print(T,10);
  Serial.println("uS");
  Serial.print("tAmostra: ");
  Serial.print(tAmostra);
  Serial.println("uS");
  Serial.print("tLeiutraAD: ");
  Serial.print(duration);
  Serial.println("uS");
  Serial.print("tAmostraMinusADTime: ");
  Serial.print(tAmostraMinusADTime);
  Serial.println("uS");

  // initialize the digital pin as an output.
  pinMode(led, OUTPUT);     
  Serial.println("---------------------------------------------------------"); 

  //inicializa contador do temporizador para leitura em modo ISR
  Timer1.initialize(100000); // set a timer of length 100000 microseconds (or 0.1 sec - or 10Hz => the led will blink 5 times, 5 cycles of on-and-off, per second)
  Timer1.attachInterrupt( timerIsr ); // attach the service routine here

  //ativaçao da rede
  Serial.print(F("Starting ethernet..."));
  Ethernet.begin(mac, ip);

  //tempo para interface levantar
  delay(100);
}


void loop(){
  //verifica se servidor mandou algo para o mim
  if(client.available()){
    char c = client.read();
    Serial.print(c);
  }

  //verifica se o firmware recebeu algum comando via serial (debugs e calibraço)
  if(Serial.available()){
    cmd = Serial.read();

    switch(cmd){
    case '+':
      ganhoV+=1000;
      Serial.println(ganhoV);
      break;
    case '-':
      ganhoV-=1000;
      Serial.println(ganhoV);
      break;
    case 'a':
      mAdbg = !mAdbg;
      break;
    case 'b':
      Adbg = !Adbg;
      break;
    case 'c':
      Vdbg = !Vdbg;
      break;
    case 's':
      Sdbg = !Sdbg;
      break;
    case 't':
      rmsTestdbg = !rmsTestdbg;
      break;
    case 'm':
      //chama função memoriaLivre()
      Serial.print(F("memoria livre: "));
      Serial.println(memoriaLivre(),DEC);
      break;
    case 'o':
      overflowDBG = !overflowDBG;
      break;
    case 'r':
      testaSobreTensao();
       break;
    default:
      Serial.println(F("Ativar/desativar debug a(mA), b(A), c(V), m (mem livre), s(Serial),t(alteracaoRMS),+/- (ajuste ganho), o(overflow), r(testa rede)"));
      break;  
    }
  }

  fazLeitura();

  testaSobreTensao();

  delay(250);
}


//**********************************
//************* funcoes para teste de memoria e de alive
//**********************************
void timerIsr(){
  estadoLed = !estadoLed;
  digitalWrite(led, estadoLed);
}

void teste(){
  Serial.print("Estou vivo ainda, com: "); 
  Serial.print(memoriaLivre());
  Serial.println(" de memoria livre;"); 
}






