// Example testing sketch for various DHT humidity/temperature sensors
// Written by ladyada, public domain

// REQUIRES the following Arduino libraries:
// - DHT Sensor Library: https://github.com/adafruit/DHT-sensor-library
// - Adafruit Unified Sensor Lib: https://github.com/adafruit/Adafruit_Sensor

#include "DHT.h"

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define DHTPIN 2     // Digital pin connected to the DHT sensor
// Feather HUZZAH ESP8266 note: use pins 3, 4, 5, 12, 13 or 14 --
// Pin 15 can work but DHT must be disconnected during program upload.

// Uncomment whatever type you're using!
#define DHTTYPE DHT11   // DHT 11
//#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
//#define DHTTYPE DHT21   // DHT 21 (AM2301)

// Connect pin 1 (on the left) of the sensor to +5V
// NOTE: If using a board with 3.3V logic like an Arduino Due connect pin 1
// to 3.3V instead of 5V!
// Connect pin 2 of the sensor to whatever your DHTPIN is
// Connect pin 3 (on the right) of the sensor to GROUND (if your sensor has 3 pins)
// Connect pin 4 (on the right) of the sensor to GROUND and leave the pin 3 EMPTY (if your sensor has 4 pins)
// Connect a 10K resistor from pin 2 (data) to pin 1 (power) of the sensor

// Initialize DHT sensor.
// Note that older versions of this library took an optional third parameter to
// tweak the timings for faster processors.  This parameter is no longer needed
// as the current DHT reading algorithm adjusts itself to work on faster procs.
DHT dht(DHTPIN, DHTTYPE);

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

#define CHAR_WIDTH 16
#define CHAR_HEIGHT 16

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
// The pins for I2C are defined by the Wire-library. 
// On an arduino UNO:       A4(SDA), A5(SCL)
// On an arduino MEGA 2560: 20(SDA), 21(SCL)
// On an arduino LEONARDO:   2(SDA),  3(SCL), ...
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// 湿
static const unsigned char PROGMEM shi_cn_bmp[] = {0b00000000,0b00000000,0b01100011,0b11111100,0b00110010,0b00000100,0b00010010,0b00000100,0b00000011,0b11111100,0b01100010,0b00000100,0b00110010,0b00000100,0b00010011,0b11111100,0b00000000,0b10010000,0b00110100,0b10010010,0b00100110,0b10010110,0b00100010,0b10010100,0b01100010,0b10010000,0b01100000,0b10010000,0b01001111,0b11111110,0b00000000,0b00000000};
// 温
static const unsigned char PROGMEM wen_cn_bmp[] = {0b00000000,0b00000000,0b01100011,0b11111100,0b00011010,0b00001100,0b00000010,0b00001100,0b00000011,0b11111100,0b01100010,0b00001100,0b00110011,0b11111100,0b00010000,0b00000000,0b00000000,0b00000000,0b00010111,0b11111100,0b00010110,0b10010100,0b00110110,0b10010100,0b00100110,0b10010100,0b01100110,0b10010100,0b01001111,0b11111110,0b00000000,0b00000000}; 
// 度
static const unsigned char PROGMEM du_cn_bmp[] = {0b00000000,0b10000000,0b00000000,0b10000000,0b00111111,0b11111110,0b00100010,0b00010000,0b00100010,0b00010000,0b00111111,0b11111110,0b00100010,0b00010000,0b00100011,0b11110000,0b00100000,0b00000000,0b00101111,0b11111100,0b00100110,0b00001000,0b00100011,0b00110000,0b01100001,0b11100000,0b01000011,0b11110000,0b01011110,0b00011110,0b00000000,0b00000000};
static const unsigned char PROGMEM degree_celsius_bmp[] = {0b00000000,0b00000000,0b00100000,0b00000000,0b01010001,0b11011000,0b01010010,0b00000110,0b00100110,0b00000010,0b00000100,0b00000010,0b00001100,0b00000010,0b00001100,0b00000000,0b00001100,0b00000000,0b00001100,0b00000000,0b00001100,0b00000000,0b00000100,0b00000000,0b00000110,0b00000010,0b00000010,0b00000100,0b00000001,0b11111000,0b00000000,0b00000000};

static const unsigned char PROGMEM c1[] = {0b00000000,0b00000000,0b01111101,0b11111110,0b00011000,0b00110000,0b00011000,0b00100000,0b00011001,0b11111110,0b00011001,0b10000110,0b00011001,0b10110110,0b00011001,0b10110110,0b00011001,0b10110110,0b00011001,0b10110110,0b00011001,0b10100110,0b00011001,0b10100110,0b00011000,0b01101000,0b00011001,0b11000110,0b01110011,0b00000010,0b00000000,0b00000000};
static const unsigned char PROGMEM c2[] = {0b00000000,0b00000000,0b00010011,0b11111000,0b00010000,0b10001000,0b00110000,0b10001000,0b00100100,0b10001000,0b01011100,0b10001000,0b01111000,0b10011110,0b00010000,0b11000100,0b00110001,0b11000100,0b01111101,0b00101100,0b01110001,0b00111000,0b00000011,0b00011000,0b00011110,0b00111000,0b01110110,0b01100110,0b00000100,0b11000010,0b00000000,0b00000000};
static const unsigned char PROGMEM c3[] = {0b00000000,0b00000000,0b00111111,0b11111100,0b00111111,0b11111100,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b01111111,0b11111110,0b00000010,0b00000000,0b00000110,0b00000000,0b00000100,0b00110000,0b00001100,0b00010000,0b00001000,0b00001000,0b00010000,0b00001100,0b00111111,0b11111100,0b00110000,0b00000110,0b00000000,0b00000000};
static const unsigned char PROGMEM c4[] = {0b00001000,0b00000000,0b00011000,0b00000000,0b00011000,0b00000000,0b00011000,0b01111110,0b01111111,0b01000110,0b00011001,0b01000110,0b00011001,0b01000110,0b00010001,0b01000110,0b00010001,0b01000110,0b00010001,0b01000110,0b00010001,0b01000110,0b00110001,0b01000110,0b00100001,0b01000110,0b01100011,0b01111110,0b01001110,0b01000110,0b00000000,0b00000000};


void showSplashScreen(){
  
  int offset = 32;
  int offsetY = 0;
  for(offsetY=32;offsetY>-32;offsetY--){
    display.clearDisplay();
    display.drawBitmap(offset+0,offsetY,c1,CHAR_WIDTH,CHAR_HEIGHT,1);
    display.drawBitmap(offset+16,offsetY,c2,CHAR_WIDTH,CHAR_HEIGHT,1);
    display.drawBitmap(offset+32,offsetY,c3,CHAR_WIDTH,CHAR_HEIGHT,1);
    display.drawBitmap(offset+48,offsetY,c4,CHAR_WIDTH,CHAR_HEIGHT,1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(16,offsetY+20);
    display.print("https://yunp.top");
    display.display();
    if(offsetY==0){
      delay(2000);
    }
  }
}

void setup() {
  dht.begin();
  display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS);

  showSplashScreen();
}

void loop() {
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f = dht.readTemperature(true);

  // Compute heat index in Fahrenheit (the default)
  float hif = dht.computeHeatIndex(f, h);
  // Compute heat index in Celsius (isFahreheit = false)
  float hic = dht.computeHeatIndex(t, h, false);

  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  
  display.drawBitmap(0,0,wen_cn_bmp,CHAR_WIDTH,CHAR_HEIGHT,1);
  display.drawBitmap(16,0,du_cn_bmp,CHAR_WIDTH,CHAR_HEIGHT,1);
  display.setCursor(32, 0);
  display.print(t);
  display.drawBitmap(display.getCursorX(),0,degree_celsius_bmp,CHAR_WIDTH,CHAR_HEIGHT,1);
  display.drawBitmap(0,16,shi_cn_bmp,CHAR_WIDTH,CHAR_HEIGHT,1);
  display.drawBitmap(16,16,du_cn_bmp,CHAR_WIDTH,CHAR_HEIGHT,1);
  display.setCursor(32, 16);
  display.print(h);
  display.print("%");
  display.display();
  delay(1000);
}
