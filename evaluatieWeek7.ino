// Libraries.
#include <WiFi.h>
#include <HTTPClient.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Definities.
#define SCREEN_WIDTH 128 // OLED display width, in pixels.
#define SCREEN_HEIGHT 64 // OLED display height, in pixels.
#define OLED_RESET -1 // Reset pin # (or -1 if sharing Arduino reset pin) // Onze OLED's hebben geen reset.

// Setup.
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
HTTPClient http;

// Variabelen.
const char* ssid = "WiFi-2.4-FE3E";
const char* password = "D9A4F32A84";

// put your setup code here, to run once:
void setup()
{
   Serial.begin(9600); // Seriele communicatie opzetten.
   
   // WiFi instellen.
   delay(4000);
   WiFi.begin(ssid, password);
   while(WiFi.status() != WL_CONNECTED)
   {
      delay(1000);
      Serial.println("Connecting to WiFi..");
   }
   Serial.println("Connected to the WiFi network");
   
   // OLED initialiseren.
   if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) // Adres van OLED is 0x3C. 
   {
      Serial.println(F("SSD1306 allocation failed"));
      for(;;); // Don't proceed, loop forever.
   }
   display.display(); // Display logo.
   delay(2000); // Pause for 2 seconds.
   display.clearDisplay(); // Clear the buffer.
}

// put your main code here, to run repeatedly:
void loop()
{
   if((WiFi.status() == WL_CONNECTED)) // Check the current connection status.
   {
      display.clearDisplay();
      WiSy();
      http.begin("http://api.thingspeak.com/apps/thinghttp/send_request?api_key=H2OT5QSJWN5Q57PS"); // Specify the brussels time URL.
      int httpCode = http.GET(); // Make the request.
      Brutime(httpCode);
      http.end(); // Free the resources.
      http.begin("http://api.thingspeak.com/apps/thinghttp/send_request?api_key=V67WSCVDOQDGTF4E"); // Specify the corona site URL.
      httpCode = http.GET();
      covid(httpCode);
      http.end();
   }else
   {
      display.clearDisplay();
      display.setTextSize(1); // Normal 1:1 pixel scale.
      display.setCursor(0, 0);
      display.println(F("No WiFi"));
   }
   delay(8000);
}

// Brussel tijd display.
void Brutime(int httpConf)
{
   if(httpConf > 0) // Check for the returning code.
   {
      String payload = http.getString();
      String Btijd = payload.substring(40, 44); // Brussel tijd uit payload halen.
      Btijd.concat(' ');
      Btijd.concat(payload.substring(48, 50));
      Btijd.concat('\0'); // NULL karakter aan strings toevoegen.
      
      // Display tijd via Serial.
      Serial.print("Brussel time: ");
      Serial.println(Btijd);
      
      // Display tijd via OLED.
      display.setCursor(80, 0);
      display.println(Btijd);
      display.display();
   }else
   {
      Serial.println("No Clock.");
      display.println(F("Problem with HTTP request."));
   }
}

// Corona data display.
void covid(int httpConf)
{
   if(httpConf > 0) // Check for the returning code.
   { 
      String payload = http.getString();
      // Declareren strings voor COVID cases en opsplitsen van payload data.
      String total = payload.substring(0, 7), deaths = payload.substring(15, 21), recov = payload.substring(21, 28);
      total.concat('\0'); // NULL karakter aan strings toevoegen.
      deaths.concat('\0');
      recov.concat('\0');
      
      // Doorsturen data via serial.
      Serial.println(httpConf); // httpCode heeft een waarde van 200 als hij succesvol is.
      Serial.println("Total cases of COVID:");
      Serial.println(total);
      Serial.println("Deceased:");
      Serial.println(deaths);
      Serial.println("Recovered:");
      Serial.println(recov);
      
      // Display data via OLED.
      display.setTextColor(SSD1306_BLACK, SSD1306_WHITE); // Draw inverse text. Zwarte tekst op lichte achtergrond.
      display.println(F("Total COVID-cases:"));
      display.setTextColor(SSD1306_WHITE); // Lichte tekst op zwarte achtergrond.
      display.println(total);
      display.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
      display.println(F("Deceased:"));
      display.setTextColor(SSD1306_WHITE);
      display.println(deaths);
      display.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
      display.println(F("Recovered:"));
      display.setTextColor(SSD1306_WHITE);
      display.println(recov);
      display.display(); // Update scherm met nieuwe data.
   }else
   {
      Serial.println("Error with Data request.");
      display.println(F("Problem with HTTP request."));
   }
}

// Teken wifi symbool.
void WiSy()
{
   int16_t i, j = 5;
   
   display.setCursor(0, 0);
   display.setTextSize(1); // Normal 1:1 pixel scale.
   display.setTextColor(SSD1306_WHITE);
   display.println(F("W"));
   display.drawPixel(8, 6, SSD1306_WHITE);
   for(i = 10; i <= 14; i += 2)
   {
      display.drawLine(i, 6, i, j, SSD1306_WHITE);
      j -= 2;
   }
   display.display();
}
