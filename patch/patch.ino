#include <TFT_eSPI.h>
#include "logo.h"       
#include <SPI.h>
#include <string>
#include "Roboto.h"
#define GFXFF 1
using fs::FS;
#include <WiFi.h>
#include <WebServer.h>

const char* ssid = "Patchogochi";  // Enter SSID here
const char* password = "12345678";  //Enter Password here

IPAddress local_ip(192,168,1,1);
IPAddress gateway(192,168,1,1);
IPAddress subnet(255,255,255,0);

WebServer server(80);

TFT_eSPI tft = TFT_eSPI();

int positionx = 1;
bool xrev = false;

int state = 1;

int positiony = 1;
bool yrev = false;

int cornerhits = 0;

// convert betweene rgb and 5:6:5 color format compatible with the lcd library
 uint16_t rgb565(uint8_t r, uint8_t g, uint8_t b) {
        return ((r >> 3) << 11) | ((g >> 2) << 5) | (b >> 3);
}

uint16_t currentcolor = rgb565(252,186,3);

void setup() {
  tft.init();
  Serial.begin(115200);
    tft.fillScreen(TFT_BLACK);
    tft.setRotation(3);
    tft.setTextSize(1);
    tft.fillScreen(TFT_BLACK);
    tft.setTextColor(rgb565(252,186,3));
    tft.setFreeFont(&Roboto_25);
    tft.drawString("mrepp miaow :3", 5, 5, GFXFF); 
    tft.setFreeFont(&Roboto_15);
    tft.drawString("beep beep, faggot alert.", 5, 35, GFXFF); 
    tft.drawXBitmap(240, 172, logo, logoWidth, logoHeight, TFT_BLACK, rgb565(252,186,3));
  WiFi.softAP(ssid, password);
  WiFi.softAPConfig(local_ip, gateway, subnet);
  delay(100);
  server.on("/", handle_OnConnect);
  server.on("/screennone", setNoneScreen);
  server.on("/screentv", setTVScreen);
  server.onNotFound(handle_OnConnect);
  server.begin();
  delay(5000);
  randomSeed(analogRead(0));
}

void setNoneScreen() {
  if(state != 1) {
    tft.fillScreen(TFT_BLACK);
    tft.setRotation(3);
    tft.setTextSize(1);
    tft.fillScreen(TFT_BLACK);
    tft.setTextColor(rgb565(252,186,3));
    tft.setFreeFont(&Roboto_25);
    tft.drawString("mrepp miaow :3", 5, 5, GFXFF); 
    tft.setFreeFont(&Roboto_15);
    tft.drawString("beep beep, faggot alert.", 5, 35, GFXFF); 
    tft.drawXBitmap(240, 172, logo, logoWidth, logoHeight, TFT_BLACK, rgb565(252,186,3));
    delay(100);
    state = 1;
  }
}

void setTVScreen() {
  if(state != 2) {
    tft.fillScreen(TFT_BLACK);
    state = 2;
  }
}

void handle_OnConnect() {\
  server.send(200, "text/html", "<h1>select the screen of choice!</h1> <br> <a href=\"/screennone\">no screen</a> <br> <a href=\"/screentv\">dvd screen</a> "); 
}

void loop() {
  server.handleClient();

  stateHandler();
}

void stateHandler() {
  switch(state) {
    case 1:
      break;
    case 2:
      
  // this code is janky as all fuck, i'm very much aware
  // but it does work, c is hard :(((

  // these two if else statements below control movement
  // xrev and yrev will return true if the img has bounced off the right or bottom sides of the screen
  // thus, these statements will result in the program going in the opposite direction when this happens
  
  if(xrev) {
    positionx--;
  } else {
    //positionx++;
    positionx++;
  }

  if(yrev) {
    //positiony--;
    positiony--;
  } else {
    positiony++;
  }

  // where the real shitty coding begins meow
  // these statements are for when it bounces off sides of the screen
  // it'll switch xrev or yrev around
  // generate a random rgb value, set the current color and text color to it
  // and in these first two, it'll check the y position to see if it's hit a corner
  
  if(positionx >= 245) {
    xrev = true;
    int rnc = random(255);
    int gnc = random(255);
    int bnc = random(255);
    currentcolor = rgb565(rnc,gnc,bnc);
    tft.setTextColor(currentcolor);
    if(positiony==177||positiony==0) {
      cornerhits++;
    }
  } else if(positionx <= 0) {
    xrev = false;
    int rnc = random(255);
    int gnc = random(255);
    int bnc = random(255);
    currentcolor = rgb565(rnc,gnc,bnc);
    tft.setTextColor(currentcolor);
    if(positiony==177||positiony==0) {
      cornerhits++;
    }
  }

  if(positiony >= 177) {
    yrev = true;
    int rnc = random(255);
    int gnc = random(255);
    int bnc = random(255);
    currentcolor = rgb565(rnc,gnc,bnc);
    tft.setTextColor(currentcolor);
  } else if(positiony <= 0) {
    yrev = false;
    int rnc = random(255);
    int gnc = random(255);
    int bnc = random(255);
    currentcolor = rgb565(rnc,gnc,bnc);
    tft.setTextColor(currentcolor);
  }

  // prevents the image staying on the screen and causing a wierd trail
  // although the trail kinda looks cool, try taking this off and seeing what happens :3
  tft.drawRect((positionx-1), (positiony-1), 78, 67, TFT_BLACK);

  // the corner hits counter
  // probably not the best way of rendering it by far, could do with a bit more optimisation
  tft.drawRoundRect(-10, 195, 150, 25, 5, currentcolor);
  tft.drawString("corner hits: " + String(cornerhits), 5, 200, GFXFF); 

  // then draws the new sprite
  tft.drawXBitmap(positionx, positiony, logo, logoWidth, logoHeight, TFT_BLACK, currentcolor);
  break;
  }
}
