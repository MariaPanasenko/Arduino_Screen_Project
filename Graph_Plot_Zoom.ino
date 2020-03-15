#include "U8glib.h"
#include <OneWire.h>
#include <DallasTemperature.h>

#define ONE_WIRE_BUS 2
U8GLIB_ST7920_128X64_4X u8g(10);

OneWire oneWire(ONE_WIRE_BUS);	
DallasTemperature sensors(&oneWire);

const uint8_t arrowy[] U8G_PROGMEM = {0x00, 0x00, 0x80, 0xc0, 0xe0, 0xc0, 0x80};
const uint8_t arrowx[] U8G_PROGMEM = {0x4, 0xe, 0x1f};
  
int INTERX = 500;
int Y_MIN = 20;
int BOTTOM_MIN = 0;
int BOTTOM_MAX = 21;
int TOP_MIN = 26;
int TOP_MAX = 36;

int val1 = 0;
int pot1 = A0;
int val2 = 0;
int pot2 = A1;
int Ny = 55;  // number of vertical pixels on the screen
int Nx = 126; // number of horizontal pixels on the screen

float Tmin;
float Tmax;
float tempC;
float Ttemp;

int i; // current time index in piexels on the screen

void setup() {
  u8g.setRot180();
  Serial.begin(9600);
  sensors.begin();
  int deviceCount = sensors.getDeviceCount();
}

void loop() {
  val1 = 0;
  pot1 = A0;
  val2 = 0;
  pot2 = A1;
  i = 0;
  
  float temp_raw[118];   // array of readed tempetatures
  int temp_rounded[118]; // array of rounded calculated temperatures
  float Tmin_prev = 0.0;
  float Tmax_prev = 0.0;
      
  do {
    sensors.requestTemperatures(); 
    tempC = sensors.getTempCByIndex(0);
    
    temp_raw[i] = tempC;
    val1 = analogRead(pot1);                           // potentiometer data
    Tmin = map(val1, 0, 1023, BOTTOM_MIN, BOTTOM_MAX); // convert potentiometer readings into temperatutes values
    val2 = analogRead(pot2);
    Tmax = map(val2, 0, 1023, TOP_MIN, TOP_MAX); 
    
    float k = (Tmax - Tmin) / Ny;
    Ttemp = Ny - round((temp_raw[i] - Tmin) / k); // pixel position on the screen
    Serial.print("Ttemp: ");
    Serial.println(Ttemp);

    temp_rounded[i] = round(Ttemp);
    
    if (Tmin_prev != Tmin || Tmax_prev != Tmax) {
      Serial.print("The bounds are changed");
      for (int y = 0; y < i-1; y++) {
        temp_rounded[y] = Ny - round((temp_raw[y] - Tmin) / k);
      }
    }
    delay(INTERX);
    
    Tmin_prev = Tmin;
    Tmax_prev = Tmax;
      
    float gradient = 60.0 * 1000.0 * ( (temp_raw[i] - temp_raw[0]) / (i * INTERX) ); // per minute
    u8g.firstPage();
    
    do {
      draw(Tmax, Tmin); // draw numbers (no algoritm usage)
       
      u8g.setPrintPos(30,20);
      u8g.print("Gradient ");
      u8g.setPrintPos(65,20);
      u8g.print(gradient);
      u8g.setPrintPos(25,5);
      u8g.print("Temperature ");
      u8g.setPrintPos(70,5);
      u8g.print(tempC);
      
      for(int j = 0; j <= i; j++) {
        u8g.drawPixel(j+10, temp_rounded[j]); //printing pixel on screen 
      }
    } while(u8g.nextPage());
    
    i += 1;
  } while (i < 118); // the end of screen
  
  clearY(); // crear screen after last position on it
  delay(500);
}
  
  
void draw(float Tmaxt, float Tmint) { //drawing static elements
  u8g.drawBitmapP( 125, 54, 1, 7, arrowy); // y
  u8g.drawBitmapP( 5, 0, 1, 3, arrowx); // x
  u8g.setFont(u8g_font_4x6);
  u8g.setPrintPos(0,5);
  u8g.print("T");
  u8g.setPrintPos(0,12);
  
  int maxt = Tmaxt;
  
  u8g.print(maxt);
  u8g.setPrintPos(0,60);
  
  int mint = Tmint;
  
  u8g.print(mint);
  u8g.drawLine(10, 3, 10, 60); // y
  u8g.drawLine(8, 58, 125, 58); // x
}
  
void clearY() {
  u8g.firstPage();
  do {} while(u8g.nextPage());
}

