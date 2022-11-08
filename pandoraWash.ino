/*
  SantoCyber 
  Complete project details at https://github.com/santocyber/pandoraWash
  
*/

#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <TFT_eSPI.h> // Hardware-specific library
#include <SPI.h>

TFT_eSPI tft = TFT_eSPI();  

// Wifi network station credentials
#define WIFI_SSID "InternetSA"
#define WIFI_PASSWORD "cadebabaca"
// Telegram BOT Token (Get from Botfather)
#define BOTtoken "5436108629:AAGxKKaYRn5JDtmyOLR8mhCDQeY59HgWQcc"

WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);

String id, text, welcome, from_name;//Váriaveis para armazenamento do ID e TEXTO gerado pelo Usuario
unsigned long tempo;
unsigned long lastTimeBotRan; // last time messages' scan has been done
const unsigned long botRequestDelay = 1000; // mean time between scan messages

int ledPin = 25;
int solenoidePin = 15;
int drenagemPin = 13;
int bldcPin = 12;
const int buttonPin = 0;
const int button2Pin = 35;
int valorbutton = 0;

//config time

uint32_t targetTime = 0;                    // for next 1 second timeout
static uint8_t conv2d(const char* p); // Forward declaration needed for IDE 1.6.x
uint8_t hh = conv2d(__TIME__), mm = conv2d(__TIME__ + 3), ss = conv2d(__TIME__ + 6); // Get H, M, S from compile time
byte omm = 99, oss = 99;
byte xcolon = 0, xsecs = 0;
unsigned int colour = 0;



void ciclo(){
         tft.setTextSize(2);
         tft.fillScreen(TFT_BLUE);
         Serial.println("Iniciando ciclo");
         digitalWrite(ledPin, HIGH);
         Serial.println("Ligando valvula solenoide");
         bot.sendMessage(id, "ligando valvula", "");
         tft.drawString("ligando valvula", tft.width() / 6, tft.height() / 6);
        digitalWrite(solenoidePin, HIGH);
         delay(15000);
         digitalWrite(solenoidePin, LOW);

         Serial.println("Ligando motor  lavagem");
bot.sendMessage(id,"ligando motor lavagem");
tft.fillScreen(TFT_BLUE);
       tft.drawString("lavando", tft.width() / 6, tft.height() / 6);
       analogWrite(bldcPin, 255);
 delay(15000);
 analogWrite(bldcPin, 0);
 
Serial.println("Ligando drenagem");
bot.sendMessage(id,"ligando drenagem");
tft.fillScreen(TFT_BLUE);
       tft.drawString("drenando agua", tft.width() / 6, tft.height() / 6);
        digitalWrite(drenagemPin, HIGH);
 delay(15000);
  digitalWrite(drenagemPin, LOW);
 

 
Serial.println("Ligando motor fullspeed e drenagem");
bot.sendMessage(id,"ligando motor fullspeed e drenagem");
tft.fillScreen(TFT_BLUE);
       tft.drawString("ligando motor \n full", tft.width() / 6, tft.height() / 6);
       analogWrite(bldcPin, 1023);
       digitalWrite(drenagemPin, HIGH);
 delay(25000);
 analogWrite(bldcPin, 0);
 digitalWrite(drenagemPin, LOW);
 
Serial.println("Roupa limpinha");
bot.sendMessage(id,"Roupa limpinha");
tft.fillScreen(TFT_BLUE);
tft.drawString("Roupa limpinha", tft.width() / 6, tft.height() / 6);
digitalWrite(ledPin, LOW);
delay(15000);
tft.fillScreen(TFT_BLACK);
cuco();

}



void handleNewMessages(int numNewMessages) {
  Serial.println("handleNewMessages");
  Serial.println(String(numNewMessages));
   
  

  for (int i=0; i<numNewMessages; i++) {
    // Chat id of the requester
    String id = String(bot.messages[i].chat_id);

    // Print the received message
    String text = bot.messages[i].text;
    Serial.println(text);


    } }


void cuco(){
if (targetTime < millis()) {
    // Set next update for 1 second later
    targetTime = millis() + 1000;

    // Adjust the time values by adding 1 second
    ss++;              // Advance second
    if (ss == 60) {    // Check for roll-over
      ss = 0;          // Reset seconds to zero
      omm = mm;        // Save last minute time for display update
      mm++;            // Advance minute
      if (mm > 59) {   // Check for roll-over
        mm = 0;
        hh++;          // Advance hour
        if (hh > 23) { // Check for 24hr roll-over (could roll-over on 13)
          hh = 0;      // 0 for 24 hour clock, set to 1 for 12 hour clock
        }
      }
    }


    // Update digital time
    int xpos = 0;
    int ypos = 25; // Top left corner ot clock text, about half way down
    int ysecs = ypos + 10;
     tft.setTextSize(1);

    

    if (omm != mm) { // Redraw hours and minutes time every minute
      omm = mm;
      // Draw hours and minutes
      if (hh < 10) xpos += tft.drawChar('0', xpos, ypos, 8); // Add hours leading zero for 24 hr clock
      xpos += tft.drawNumber(hh, xpos, ypos, 8);             // Draw hours
      xcolon = xpos; // Save colon coord for later to flash on/off later
      xpos += tft.drawChar(':', xpos, ypos - 8, 8);
      if (mm < 10) xpos += tft.drawChar('0', xpos, ypos, 8); // Add minutes leading zero
      xpos += tft.drawNumber(mm, xpos, ypos, 8);             // Draw minutes
      xsecs = xpos; // Sae seconds 'x' position for later display updates
    }
    if (oss != ss) { // Redraw seconds time every second
      oss = ss;
      xpos = xsecs;

      if (ss % 2) { // Flash the colons on/off
        tft.setTextColor(0x39C4, TFT_BLACK);        // Set colour to grey to dim colon
        tft.drawChar(':', xcolon, ypos - 8, 8);     // Hour:minute colon
        xpos += tft.drawChar(':', xsecs, ysecs, 6); // Seconds colon
        tft.setTextColor(TFT_YELLOW, TFT_BLACK);    // Set colour back to yellow
      }
      else {
        tft.drawChar(':', xcolon, ypos - 8, 8);     // Hour:minute colon
        xpos += tft.drawChar(':', xsecs, ysecs, 6); // Seconds colon
      }

      //Draw seconds
      if (ss < 10) xpos += tft.drawChar('0', xpos, ysecs, 6); // Add leading zero
      tft.drawNumber(ss, xpos, ysecs, 6);                     // Draw seconds
    }
  }}





void setup()
{

pinMode(buttonPin, INPUT); // initialize digital ledPin as an output.
pinMode(button2Pin, INPUT); // initialize digital ledPin as an output.
pinMode(solenoidePin, OUTPUT); // initialize digital ledPin as an output.
pinMode(drenagemPin, OUTPUT); // initialize digital ledPin as an output.
pinMode(bldcPin, OUTPUT); // initialize digital ledPin as an output.
pinMode(ledPin, OUTPUT); // initialize digital ledPin as an output.


digitalWrite(ledPin, LOW);
digitalWrite(solenoidePin, HIGH); // initialize pin as off (active LOW)
digitalWrite(solenoidePin, LOW); // initialize pin as off (active LOW)
digitalWrite(drenagemPin, LOW); // initialize pin as off (active LOW)
analogWrite(bldcPin, 0); // initialize pin as off (active LOW)

  
   WiFi.mode(WIFI_STA);//Define o WiFi como Estaçao
   connect();//Funçao para Conectar ao WiFi
   
//inicia tft
  tft.init();
  tft.setRotation(1);
  tft.setTextSize(2);
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_YELLOW, TFT_BLACK);

Serial.begin(115200);
  Serial.println();
   // attempt to connect to Wifi network:
  Serial.print("Connecting to Wifi SSID ");
  Serial.print(WIFI_SSID);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  client.setCACert(TELEGRAM_CERTIFICATE_ROOT); // Add root certificate for api.telegram.org
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }
  Serial.print("\nWiFi connected. IP address: ");
  Serial.println(WiFi.localIP());


  Serial.print("Retrieving time: ");
  configTime(0, 0, "pool.ntp.org"); // get UTC time via NTP
  time_t now = time(nullptr);
  while (now < 24 * 3600)
  {
    Serial.print(".");
    delay(1000);
    now = time(nullptr);
  }
  Serial.println(now);

   
}

void loop()
{
   if (millis() - tempo > 2000)//Faz a verificaçao das funçoes a cada 2 Segundos
   {
      connect();//Funçao para verificar se ainda há conexao
      readTel();//Funçao para ler o telegram
      tempo = millis();//Reseta o tempo
   }


   cuco();
//funcao ativa ciclo botao bultin

valorbutton = digitalRead(buttonPin);//Lê o valor do sensor ldr e armazena na variável valorldr
if ((valorbutton) == LOW) { 
 Serial.println(valorbutton);
readTel();
ciclo();
bot.sendMessage(id, "Button pressionado", "");//Envia uma Mensagem para a pessoa que enviou o Comando.

  }
  

   
}


void connect()//Funçao para Conectar ao wifi e verificar à conexao.
{
   if (WiFi.status() != WL_CONNECTED)//Caso nao esteja conectado ao WiFi, Ira conectarse
   {
      WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
      delay(2000);
   }
}


void readTel()//Funçao que faz a leitura do Telegram.
{
   int newmsg = bot.getUpdates(bot.last_message_received + 1);

   for (int i = 0; i < newmsg; i++)//Caso haja X mensagens novas, fara este loop X Vezes.
   {
      id = bot.messages[i].chat_id;//Armazenara o ID do Usuario à Váriavel.
      text = bot.messages[i].text;//Armazenara o TEXTO do Usuario à Váriavel.
      //text.toUpperCase();//Converte a STRING_TEXT inteiramente em Maiuscúla.
      from_name = bot.messages[i].from_name;
      bot.messages[i].type == "channel_post";
      

      if (text.indexOf("LAVAON") > -1)//Caso o texto recebido contenha "ON"
      {
         
       

         ciclo();
        
         cuco();
      }

      else if (text.indexOf("LAVAOFF") > -1)//Caso o texto recebido contenha "OFF"
      {
         bot.sendMessage(id, "Lava OFF", "");//Envia uma Mensagem para a pessoa que enviou o Comando.
         tft.setTextSize(3);
         tft.fillScreen(TFT_RED);
         tft.drawString("Lava OFF", tft.width() / 1, tft.height() / 1);
         delay(5000);
         cuco();
      }

      else if (text.indexOf("START") > -1)//Caso o texto recebido contenha "START"
      {
         bot.sendSimpleMessage(id, id, "");//Envia uma mensagem com seu ID.
      }
      else if (text.indexOf("OLA") > -1)//Caso o texto recebido contenha "START"
      {
      if (from_name == "")
      from_name = "Guest";

      welcome = "Bem vindo, " + from_name + ".\n";
      welcome += "Esse eh a PandoraBOT\n\n";
      welcome += "/LAVAON : Para ligar a maquina de lavar\n";
      welcome += "/LAVAOFF : Para desligar a maquina de lavar\n";
      welcome += "/OLA : Abre esse menu\n";
      bot.sendMessage(id, welcome, "Markdown");      
      }
      else if   (text = "/Ola" "Ola" "OLA")//Caso o texto recebido contenha "START"
      {
     if (from_name == "")
      from_name = "Guest";

      welcome = "Bem vindo, " + from_name + ".\n";
      welcome += "Esse eh a PandoraBOT\n\n";
      welcome += "/LAVAON : to switch the Led ON\n";
      welcome += "/LAVAOFF : to switch the Led OFF\n";
      welcome += "/OLA : Returns current status of LED\n";
      bot.sendMessage(id, welcome, "Markdown");  
      }

      else if (text.indexOf("/Ola") > -1)//Caso o texto recebido contenha "START"
      {
         bot.sendSimpleMessage(id, id, "");//Envia uma mensagem com seu ID.
      }
      else//Caso o texto recebido nao for nenhum dos acima, Envia uma mensagem de erro.
      {
    //     bot.sendSimpleMessage(id, "Comando Invalido", "");
      }
   }

}



// Function to extract numbers from compile time string
static uint8_t conv2d(const char* p) {
  uint8_t v = 0;
  if ('0' <= *p && *p <= '9')
    v = *p - '0';
  return 10 * v + *++p - '0';
}
