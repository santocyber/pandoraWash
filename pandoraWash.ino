/*
  SantoCyber 
  Complete project details at https://github.com/santocyber/pandoraLUX
  
*/

#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>   
#include <ArduinoJson.h>
#include <SPI.h>
#include <WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>


//Configura fuso horario em segundos
  // Set offset time in seconds to adjust for your timezone, for example:
  // GMT +1 = 3600
  // GMT +8 = 28800
  // GMT -1 = -3600
  // GMT 0 = 0

#define NTP_OFFSET  19800 // In seconds
#define NTP_INTERVAL 60 * 1000    // In miliseconds
#define NTP_ADDRESS  "pool.ntp.org"

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, NTP_ADDRESS, NTP_OFFSET, NTP_INTERVAL);

//define saidas do display
#define OLED_MOSI  23
#define OLED_CLK   18
#define OLED_DC    4
#define OLED_CS    5
#define OLED_RESET 2
Adafruit_SSD1306 display(OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);



// Replace with your network credentials
const char* ssid = "InternetSA";
const char* password = "cadebabaca";



int solenoidePin = 6;
int drenagemPin = 7;
int bldcPin = 8;
const int buttonPin = 0;
int valorbutton = 0;



// Initialize Telegram BOT
#define BOTtoken "5436108629:AAGxKKaYRn5JDtmyOLR8mhCDQeY59HgWQcc"  // your Bot Token (Get from Botfather)

// Use @myidbot to find out the chat ID of an individual or a group
// Also note that you need to click "start" on a bot before it can
// message you
#define CHAT_ID "-878096558"

WiFiClientSecure secured_client;
WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);

// Checks for new messages every 1 second.
int botRequestDelay = 1000;
unsigned long lastTimeBotRan;


// Inicia variáveis de tempo
unsigned long millisTarefa1 = millis();
unsigned long millisTarefa2 = millis();
unsigned long millisTarefa3 = millis();
unsigned long millisTarefa4 = millis();






//Configura os voids


void tarefa1(){
  // Verifica se já passou 200 milisegundos
  if((millis() - millisTarefa1) < 200){
    // Acende o pino 7 valvula solenoide
    digitalWrite(solenoidePin, HIGH);
    
  }else{
    // Apaga o led do pino 7
    digitalWrite(solenoidePin, LOW);
  }
  // Verifica se já passou 400 milisegundos reinicia funcao
  if((millis() - millisTarefa1) > 400){
    millisTarefa1 = millis();
  }}

  void tarefa2(){
  // Verifica se já passou 200 milisegundos
  if((millis() - millisTarefa2) < 200){
    // Acende o pino 8 bomba drenagem
    digitalWrite(drenagemPin, HIGH);
    
  }else{
    // Apaga o led do pino 7
    digitalWrite(drenagemPin, LOW);
  }
  // Verifica se já passou 400 milisegundos reinicia funcao
  if((millis() - millisTarefa2) > 400){
    millisTarefa1 = millis();
  }}

  //liga o motor devagar
  void tarefa3(){
  // Verifica se já passou 200 milisegundos
  if((millis() - millisTarefa3) < 200){
    // Acende o controlador no maximo
    analogWrite(bldcPin, 255);
    
  }else{
    // Apaga o led do pino 7
    analogWrite(bldcPin, 0);
  }
  // Verifica se já passou 400 milisegundos reinicia funcao
  if((millis() - millisTarefa3) > 400){
    millisTarefa1 = millis();
  }}
//liga o motor fullspeed
    void tarefa4(){
  // Verifica se já passou 200 milisegundos
  if((millis() - millisTarefa4) < 200){
    // Acende o controlador no maximo
    analogWrite(bldcPin, 1023);
    
  }else{
    // Apaga o led do pino 7
    analogWrite(bldcPin, 0);
  }
  // Verifica se já passou 400 milisegundos reinicia funcao
  if((millis() - millisTarefa4) > 400){
    millisTarefa1 = millis();
  }}



// Config do que acontece quando recebe msg

void handleNewMessages(int numNewMessages) {
  Serial.println("handleNewMessages");
  Serial.println(String(numNewMessages));
   
  

  for (int i=0; i<numNewMessages; i++) {
    // Chat id of the requester
    String chat_id = String(bot.messages[i].chat_id);
    if (chat_id != CHAT_ID){
      bot.sendMessage(chat_id, "Unauthorized user", "");
      continue;
    }
    
    // Print the received message
    String text = bot.messages[i].text;
    Serial.println(text);

    String from_name = bot.messages[i].from_name;

    if (text == "/start") {
      String welcome = "Bem vindo, " + from_name + ".\n";
      welcome += "Use os comandos a seguir para controlar a Pandora .\n\n";
      welcome += "/lavaroupa para ligar a maquina \n";
      welcome += "/solenoide para ligar a maquina \n";
      welcome += "/drenagem para ligar a maquina \n";
      welcome += "/bldcfull para ligar a maquina \n";
      welcome += "/lava_off to turn GPIO OFF \n";
      welcome += "/state to request current GPIO state \n";
      bot.sendMessage(chat_id, welcome, "");

    }

    if (text == "/lavaroupa") {
      bot.sendMessage(chat_id, "Iniciando ciclo de lavagem", "");
Serial.println("Iniciando ciclo");
tarefa1();
Serial.println("Ligando valvula solenoide");
bot.sendMessage(chat_id, "liberando agua", "");
delay(2000);
tarefa3();
Serial.println("Ligando motor bldc");
delay(2000);
tarefa2();
Serial.println("Ligando drenagem");
bot.sendMessage(chat_id,"ligando drenagem");
delay(2000);
tarefa4();
tarefa2();
Serial.println("Ligando motor fullspeed e drenagem");
bot.sendMessage(chat_id,"ligando motor fullspeed e drenagem");
delay(2000);
Serial.println("Roupa limpinha");
bot.sendMessage(chat_id,"Roupa limpinha");
display.println("Roupa limpinha");
delay(12000);
    }


 if (text == "/solenoide") {
tarefa1();
Serial.println("Ligando valvula solenoide");
bot.sendMessage(chat_id, "liberando agua", "");
delay(2000);
}
if (text == "/drenagem") {
tarefa2();
Serial.println("Ligando drenagem");
bot.sendMessage(chat_id, "ligando drenagem", "");
delay(2000);
}
if (text == "/bldcfull") {
tarefa4();
Serial.println("Ligando bldcfull");
bot.sendMessage(chat_id, "ligando bldcfull", "");
delay(12000);
}





    
          if (text == "/lava_off") {
      bot.sendMessage(chat_id, "Desligando todos os sensores", "");
      Serial.println("Desligando sensores");
      digitalWrite(drenagemPin, LOW);
       digitalWrite(solenoidePin, LOW);
        analogWrite(bldcPin, 0);
    }
    
    if (text == "/state") {
      if (digitalRead(solenoidePin)){
        bot.sendMessage(chat_id, "Solenoide is ON", "");
      }
      else{
        bot.sendMessage(chat_id, "Solenoide is OFF", "");
      }

      if (digitalRead(drenagemPin)){
        bot.sendMessage(chat_id, "Drenagem is ON", "");
      }
      else{
        bot.sendMessage(chat_id, "Drenagem is OFF", "");
      }

      if (digitalRead(bldcPin)){
        bot.sendMessage(chat_id, "BLDC is ON", "");
      }
      else{
        bot.sendMessage(chat_id, "BLDC is OFF", "");
      }
    }
  }
}







void setup() {
  Serial.begin(9600);

  
  

  
//inicia Pinos

pinMode(drenagemPin, OUTPUT); //Define ldr (pino analógico A0) como saída
pinMode(solenoidePin, OUTPUT); //Define ldr (pino analógico A0) como saída
pinMode(bldcPin, OUTPUT); //Define ldr (pino analógico A0) como saída
pinMode(buttonPin, INPUT); //Define ldr (pino analógico A0) como saída

Serial.println();
Serial.println();
Serial.print("Connecting to ");
Serial.println(ssid);
WiFi.begin(ssid, password);
secured_client.setCACert(TELEGRAM_CERTIFICATE_ROOT); // Add root certificate for api.telegram.org

while (WiFi.status() != WL_CONNECTED)
{
delay(500);
Serial.print(".");
}
Serial.println("");
Serial.println("WiFi connected.");
Serial.println("IP address: ");
Serial.println(WiFi.localIP());
timeClient.begin();
display.begin(SSD1306_SWITCHCAPVCC);

}

void loop() {

  
//Mostra hora na tela
timeClient.update();
String formattedTime = timeClient.getFormattedTime();
display.clearDisplay();
display.setTextSize(3);                         // set these parameters according to your need..
display.setCursor(0, 0);
display.println(formattedTime);

delay(2000); // Aguarda 2 segundos





//verifica mensagem no telgram
 if (millis() > lastTimeBotRan + botRequestDelay)  {
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);

    while(numNewMessages) {
      Serial.println("got response");
      handleNewMessages(numNewMessages);
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }
    lastTimeBotRan = millis();
  }
  
  
valorbutton = digitalRead(buttonPin);//Lê o valor do sensor ldr e armazena na variável valorldr
Serial.println(valorbutton);//Imprime na serial os dados de valorldr


if ((valorbutton) == LOW) { //Se o valor do button for LOW
Serial.println("Iniciando ciclo");
tarefa1();
Serial.println("Ligando valvula solenoide");
delay(2000);
tarefa3();
Serial.println("Ligando motor bldc");
delay(2000);
tarefa2();
Serial.println("Ligando drenagem");
bot.sendMessage(CHAT_ID,"ligando drenagem");
delay(2000);
tarefa4();
tarefa2();
Serial.println("Ligando motor fullspeed e drenagem");
bot.sendMessage(CHAT_ID,"ligando motor fullspeed e drenagem");
delay(2000);
Serial.println("Roupa limpinha");
bot.sendMessage(CHAT_ID,"Roupa limpinha");
display.println("Roupa limpinha");
  }

  else { //Senão:

   Serial.println("Fim de operacao");
   delay(2000);
   display.println(formattedTime);
   delay(2000);
   Serial.println(formattedTime);
   delay(2000);
   
  }





}
