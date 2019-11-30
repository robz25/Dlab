/*
robz25 2019

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

This code is based in original ideas and some portions of examples from libraries.
*/
#include <Adafruit_NeoPixel.h>
//#include <EEPROM.h>

#define PIN     14

#define NUMstrip 10

// NeoPixel brightness, 0 (min) to 255 (max)
#define BRIGHTNESS 150

Adafruit_NeoPixel strip(NUMstrip, PIN, NEO_GRB + NEO_KHZ800);
#define DELAYVAL 10 // Time (in milliseconds) to pause between strip

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

#ifndef APSSID
#define APSSID "Tech Jam Trophy"
#define APPSK  "winner19"//minimo 8 caracteres
#endif

byte select = 0;
int configDone = 0;
int configWaitTime = 35000;
int ledCount = 0;
unsigned long ellapsed;
int address = 0;
bool presionado = false;

const char* html =
#include "index.h"
  ;

const char* color =
#include "color.h"
  ;

/* Set these to your desired credentials. */
const char *ssid = APSSID;
const char *password = APPSK;

ESP8266WebServer server(80);
String header;

void stripOff(){
  for(int i=0; i<NUMstrip; i++) { // For each pixel...
    strip.setPixelColor(i, strip.Color(0, 0, 0));
    strip.show();  
  }
}

unsigned int rgbColour[3];

void handleRoot() {
  configDone = 1;
  stripOff();
  server.send(200, "text/html", html);
  strip.setPixelColor(9, strip.Color(0, 255, 0));
  strip.setPixelColor(8, strip.Color(0, 255, 0));
  strip.show();
}

void colour() {
  stripOff();
  server.send(200, "text/html", color);
  strip.setPixelColor(4, strip.Color(0, 0, 200));
  strip.setPixelColor(3, strip.Color(0, 0, 200));
  strip.show();
}

void akamaiColors(){
  select=1;
  presionado = true;
  Serial.println("entro 1");
}

void rgbStrand(){
  select=2;
  presionado = true;
  //goto mainLoop;
  Serial.println("entro 2");
}

void slowFade(){
  select=3;
  presionado = true;
  Serial.println("entro 3");
}

void fastFade(){
  select=4;
  presionado = true;
  Serial.println("entro 4");
}

void setup() {
  strip.begin();
  strip.show();            // Turn OFF all strip ASAP
  strip.setBrightness(BRIGHTNESS);
  pinMode(LED_BUILTIN, OUTPUT);
  delay(1000);
  Serial.begin(115200);
  Serial.println();
  Serial.print("Configuring access point...");
  /* You can remove the password parameter if you want the AP to be open. */
  //WiFi.softAP(ssid);
  WiFi.softAP(ssid, password);

  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
  server.on("/", handleRoot);
  server.on("/maco", colour);
  server.on("/akco", akamaiColors);
  server.on("/rgb", rgbStrand);
  server.on("/sf", slowFade);
  server.on("/ff", fastFade);
  server.begin();
  Serial.println("HTTP server started");
  ellapsed = millis();
  Serial.println("SSID: Tech Jam Trophy\nPass: winner19\nOnce powered you have 30 seconds to connect\nAdafruit Feather Huzzah with ESp8266 inside\nMay be programmed using Arduino\nMade at Danny's L@B\nEnjoy! :)");
}

void loop() {
  ESP.wdtFeed();
  Serial.println("The cake is a lie!");
  //yield()
  if(!configDone){
    if(millis() - ellapsed > configWaitTime*ledCount/10){
      strip.setPixelColor(ledCount, strip.Color(125, 0, 0));
      strip.show();
      ledCount++;
    }
    if(millis() - ellapsed > configWaitTime){
      configDone = 1;
      select = 2;
    }
  }
    
  server.handleClient();
  switch(select){
    case 1://Akamai colors
    presionado = false;
    for(int i=9; i>=0; i--) { // azul
      strip.setPixelColor(i, strip.Color(0, 123, 234));
      strip.show();   // Send the updated pixel colors to the hardware.
      delay(50); // Pause before next pass through loop
    }
    delay(2500);
    for(int i=0; i<NUMstrip; i++) {//naranja
      if(presionado == true)break;
      strip.setPixelColor(i, strip.Color(255, 50, 20));
      strip.show();   // Send the updated pixel colors to the hardware.
      delay(50); // Pause before next pass through loop
    }
    delay(2500);
    break;

    case 2://RGB strand
    presionado = false;
    colorWipe(strip.Color(255,   0,   0)     , 50); // Red
    colorWipe(strip.Color(  0, 255,   0)     , 50); // Green
    colorWipe(strip.Color(  0,   0, 255)     , 50); // Blue
    colorWipe(strip.Color(  0,   0,   0, 255), 50); // True white (not RGB white)

    whiteOverRainbow(75, 5);
    ESP.wdtFeed();//reiniciar WDT
    pulseWhite(5);

    rainbowFade2White(3, 3, 1);
    break;

    case 3:
    presionado = false;
    rgbColour[0] = 255;
    rgbColour[1] = 0;
    rgbColour[2] = 0;
  for (int decColour = 0; decColour < 3; decColour += 1) {
    int incColour = decColour == 2 ? 0 : decColour + 1;
  
    for(int i = 0; i < 255; i += 1) {
      rgbColour[decColour] -= 1;
      rgbColour[incColour] += 1;
      if(presionado == true)break;
      setColourRgb(rgbColour[0], rgbColour[1], rgbColour[2]);
      delay(5);
    }
  }
    break;
    
    case 4:
    presionado = false;
    rgbColour[0] = 255;
    rgbColour[1] = 0;
    rgbColour[2] = 0;
    for (int decColour = 0; decColour < 3; decColour += 1) {
      int incColour = decColour == 2 ? 0 : decColour + 1;
      // cross-fade the two colours.
      for(int i = 0; i < 255; i += 1) {
        rgbColour[decColour] -= 1;
        rgbColour[incColour] += 1;
        if(presionado == true)break;
        setColourRgbFast(rgbColour[0], rgbColour[1], rgbColour[2]);
        delay(5);
      }
    }

    break;

    case 5:
    presionado = false;
    break;

    default:
    break;    
    }
}


void setColourRgb(unsigned int red, unsigned int green, unsigned int blue) {
    for(int i=0; i<NUMstrip; i++) { // For each pixel...
    if(presionado == true)break;
    strip.setPixelColor(i, strip.Color(red, green, blue));
    strip.show();   // Send the updated pixel colors to the hardware.
    delay(DELAYVAL); // Pause before next pass through loop
  }
}

void setColourRgbFast(unsigned int red, unsigned int green, unsigned int blue) {
    //if(presionado == true)break;
    for(int i=0; i<NUMstrip; i++) { // For each pixel...
    if(presionado == true)break;
    strip.setPixelColor(i, strip.Color(red, green, blue));
    strip.show();   // Send the updated pixel colors to the hardware.
    delayMicroseconds(30); // Pause before next pass through loop
  }
}

//RGB strand
void colorWipe(uint32_t color, int wait) {
  for(int i=0; i<strip.numPixels(); i++) { // For each pixel in strip...
    if(presionado == true)break;
    strip.setPixelColor(i, color);         //  Set pixel's color (in RAM)
    strip.show();                          //  Update strip to match
    delay(wait);                           //  Pause for a moment
  }
}

void whiteOverRainbow(int whiteSpeed, int whiteLength) {
  if(whiteLength >= strip.numPixels()) whiteLength = strip.numPixels() - 1;
  int      head          = whiteLength - 1;
  int      tail          = 0;
  int      loops         = 3;
  int      loopNum       = 0;
  uint32_t lastTime      = millis();
  uint32_t firstPixelHue = 0;

  for(;;) { // Repeat forever (or until a 'break' or 'return')
    if(presionado == true)break;
    for(int i=0; i<strip.numPixels(); i++) {  // For each pixel in strip...
      if(presionado == true)break;
      if(((i >= tail) && (i <= head)) ||      //  If between head & tail...
         ((tail > head) && ((i >= tail) || (i <= head)))) {
        strip.setPixelColor(i, strip.Color(0, 0, 0, 255)); // Set white
      } else {                                             // else set rainbow
        int pixelHue = firstPixelHue + (i * 65536L / strip.numPixels());
        strip.setPixelColor(i, strip.gamma32(strip.ColorHSV(pixelHue)));
      }
    }

    strip.show();
    
    firstPixelHue += 40; // Advance just a little along the color wheel

    if((millis() - lastTime) > whiteSpeed) { // Time to update head/tail?
      if(++head >= strip.numPixels()) {      // Advance head, wrap around
        head = 0;
        if(++loopNum >= loops) return;
      }
      if(++tail >= strip.numPixels()) {      // Advance tail, wrap around
        tail = 0;
      }
      lastTime = millis();                   // Save time of last movement
    }
  }
}

void pulseWhite(uint8_t wait) {
  for(int j=0; j<256; j++) { // Ramp up from 0 to 255
    if(presionado == true)break;
    // Fill entire strip with white at gamma-corrected brightness level 'j':
    strip.fill(strip.Color(0, 0, 0, strip.gamma8(j)));
    strip.show();
    delay(wait);
  }

  for(int j=255; j>=0; j--) { // Ramp down from 255 to 0
    if(presionado == true)break;
    strip.fill(strip.Color(0, 0, 0, strip.gamma8(j)));
    strip.show();
    delay(wait);
  }
}

void rainbowFade2White(int wait, int rainbowLoops, int whiteLoops) {
  int fadeVal=0, fadeMax=100;
  // Hue of first pixel runs 'rainbowLoops' complete loops through the color
  // wheel. Color wheel has a range of 65536 but it's OK if we roll over, so
  // just count from 0 to rainbowLoops*65536, using steps of 256 so we
  // advance around the wheel at a decent clip.
  for(uint32_t firstPixelHue = 0; firstPixelHue < rainbowLoops*65536;
    firstPixelHue += 256) {
    for(int i=0; i<strip.numPixels(); i++) { // For each pixel in strip...
      if(presionado == true)break;
      // Offset pixel hue by an amount to make one full revolution of the
      // color wheel (range of 65536) along the length of the strip
      // (strip.numPixels() steps):
      uint32_t pixelHue = firstPixelHue + (i * 65536L / strip.numPixels());
      // strip.ColorHSV() can take 1 or 3 arguments: a hue (0 to 65535) or
      // optionally add saturation and value (brightness) (each 0 to 255).
      // Here we're using just the three-argument variant, though the
      // second value (saturation) is a constant 255.
      strip.setPixelColor(i, strip.gamma32(strip.ColorHSV(pixelHue, 255,
        255 * fadeVal / fadeMax)));
    }
    strip.show();
    delay(wait);
    if(firstPixelHue < 65536) {                              // First loop,
      if(fadeVal < fadeMax) fadeVal++;                       // fade in
    } else if(firstPixelHue >= ((rainbowLoops-1) * 65536)) { // Last loop,
      if(fadeVal > 0) fadeVal--;                             // fade out
    } else {
      fadeVal = fadeMax; // Interim loop, make sure fade is at max
    }
  }
  for(int k=0; k<whiteLoops; k++) {
    for(int j=0; j<256; j++) { // Ramp up 0 to 255
      // Fill entire strip with white at gamma-corrected brightness level 'j':
      strip.fill(strip.Color(0, 0, 0, strip.gamma8(j)));
      strip.show();
  }
  delay(1000); // Pause 1 second
  for(int j=255; j>=0; j--) { // Ramp down 255 to 0
    strip.fill(strip.Color(0, 0, 0, strip.gamma8(j)));
    strip.show();
   }
  }
 
  delay(500); // Pause 1/2 second
}
