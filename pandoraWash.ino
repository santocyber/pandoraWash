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


const int D4 = 2;

int solenoidePin = 6;
int drenagemPin = 7;
int bldcPin = 8;
const int buttonPin = 0;
int valorbutton = 0;

//config time

uint32_t targetTime = 0;                    // for next 1 second timeout
static uint8_t conv2d(const char* p); // Forward declaration needed for IDE 1.6.x
uint8_t hh = conv2d(__TIME__), mm = conv2d(__TIME__ + 3), ss = conv2d(__TIME__ + 6); // Get H, M, S from compile time
byte omm = 99, oss = 99;
byte xcolon = 0, xsecs = 0;
unsigned int colour = 0;

// Inicia variáveis de tempo
unsigned long millisTarefa1 = millis();
unsigned long millisTarefa2 = millis();
unsigned long millisTarefa3 = millis();
unsigned long millisTarefa4 = millis();




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
  if((millis() - millisTarefa1) > 5000){
    millisTarefa1 = millis();
  }}

  void tarefa2(){
  // Verifica se já passou 200 milisegundos
  if((millis() - millisTarefa2) < 5000){
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



void ciclo(){

Serial.println("Iniciando ciclo");
         Serial.println("Ligando valvula solenoide");
         bot.sendMessage(id, "ligando valvula", "");
         tft.drawString("ligando valvula", tft.width() / 6, tft.height() / 6);
        digitalWrite(solenoidePin, HIGH);
         delay(5000);
         digitalWrite(solenoidePin, LOW);

         Serial.println("Ligando motor  lavagem");
bot.sendMessage(id,"ligando motor lavagem");
       tft.drawString("lavando", tft.width() / 5, tft.height() / 5);
       analogWrite(bldcPin, 255);
 delay(5000);
 analogWrite(bldcPin, 0);
 
Serial.println("Ligando drenagem");
bot.sendMessage(id,"ligando drenagem");
       tft.drawString("drenando agua", tft.width() / 4, tft.height() / 4);
        digitalWrite(drenagemPin, HIGH);
 delay(5000);
  digitalWrite(drenagemPin, LOW);
 

 
Serial.println("Ligando motor fullspeed e drenagem");
bot.sendMessage(id,"ligando motor fullspeed e drenagem");
       tft.drawString("ligando motor", tft.width() / 3, tft.height() / 3);
       analogWrite(bldcPin, 1023);
       digitalWrite(drenagemPin, HIGH);
 delay(5000);
 analogWrite(bldcPin, 0);
 digitalWrite(drenagemPin, LOW);
 
Serial.println("Roupa limpinha");
bot.sendMessage(id,"Roupa limpinha");
tft.drawString("Roupa limpinha", tft.width() / 2, tft.height() / 2);


}




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
   pinMode(D4, OUTPUT);//LED conectado à saida
   WiFi.mode(WIFI_STA);//Define o WiFi como Estaçao
   connect();//Funçao para Conectar ao WiFi
   
//inicia tft
 tft.init();
    tft.setRotation(1);
  tft.fillScreen(TFT_BLACK);

  tft.setTextSize(1);
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
         tft.setTextSize(2);
         tft.fillScreen(TFT_BLUE);
       

         ciclo();
         delay(5000);
         cuco();
      }

      else if (text.indexOf("LAVAOFF") > -1)//Caso o texto recebido contenha "OFF"
      {
         digitalWrite(D4, 1);//Desliga o LED
         bot.sendMessage(id, "Lava OFF", "");//Envia uma Mensagem para a pessoa que enviou o Comando.
         tft.setTextSize(3);
         tft.fillScreen(TFT_YELLOW);
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
      else if   (text = "/Ola")//Caso o texto recebido contenha "START"
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
