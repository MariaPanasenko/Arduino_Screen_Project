#include "U8glib.h"
#include <OneWire.h>
#include <DallasTemperature.h>


#define ONE_WIRE_BUS 2
U8GLIB_ST7920_128X64_4X u8g(10);


OneWire oneWire(ONE_WIRE_BUS);	
DallasTemperature sensors(&oneWire);

  
  
  const uint8_t arrowy[] U8G_PROGMEM = {0x00,0x00, 0x80,0xc0,0xe0,0xc0,0x80,};
  const uint8_t arrowx[] U8G_PROGMEM = {0x4,0xe, 0x1f};
  
        
//  int i = 8;
  int interx = 500;

  void setup() {
     u8g.setRot180();
    Serial.begin(9600);
    sensors.begin();
    int deviceCount = sensors.getDeviceCount();
  }
  
  void loop() {
        int Y_MIN = 20;
        int ZOOM;
        int val1 = 0;
        int pot1 = A0;
        int val2 = 0;
        int pot2 = A1;
        int Ny = 55; // pixel vertical
        int Nx = 126; //pixel horisontal

        float Tmin;
        float Tmax;
        float tempC;
        int T[118];
        float Ttemp;
        float arrT[118];

      int i = 0;
       // coefficient
      
  do{
      sensors.requestTemperatures(); 
      tempC = sensors.getTempCByIndex(0);

      
      arrT[i] = tempC;
      val1 = analogRead(pot1);
      Tmin = map(val1, 0, 1023, 0,21);
      Serial.print(" Tmin is ");
      Serial.println(Tmin);
      val2 = analogRead(pot2);
      Tmax = map(val2, 0, 1023, 20, 30);
      Serial.print(" Tmax is ");
      float k = (Tmax - Tmin) / Ny; 
      Ttemp = Ny - ((arrT[i] - Tmin) / k); // pixel position on screen
      
      Serial.print(" k is ");
      Serial.println(k);
      T[i] = round(Ttemp);
      Serial.print(" T is ");
      Serial.println(T[i]);
      Serial.println((interx*i)/1000.00);
      Serial.print(" arrT[i] ");
      Serial.println(arrT[i]);
      Serial.println("");
      delay(interx);
      
      float gradient = 60.00*1000.0*((arrT[i] - arrT[0])/((i*interx - 0.00))); //per minute
      Serial.print(" gradient ");
      Serial.print(gradient);
      Serial.println(" ");
       u8g.firstPage();
      do{
       draw(Tmax, Tmin);
       
       u8g.setPrintPos(30,20);
        u8g.print("Gradient ");
       u8g.setPrintPos(65,20);
        u8g.print(gradient);
        u8g.setPrintPos(25,5);
        u8g.print("Temperature ");
        u8g.setPrintPos(70,5);
        u8g.print(tempC);
       for(int j = 0; j <= i; j++){
       u8g.drawPixel(j+10, T[j]); 
   }
     }while( u8g.nextPage() );
     i += 1;
      }while(i < 118);
        clearY();
    delay(500);  
  }
  
  
  void draw(float Tmaxt, float Tmint){
        u8g.drawBitmapP( 125, 54, 1, 7, arrowy); //y
        u8g.drawBitmapP( 5, 0, 1, 3, arrowx); //x
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
  
  void clearY(){
  u8g.firstPage();
      do{ }while( u8g.nextPage() );
}
