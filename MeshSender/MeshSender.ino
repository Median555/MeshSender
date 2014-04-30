
#include "definitions.h"
#include <Time.h>

int Yakse = 0;
int Xakse = 0;
byte vram[LCD_WIDTH * LCD_HEIGHT / 8]; //frambuffer for the LCD display
int pointerX;
int pointerY;

  

void setup() 
{
  //RFbegin();
  Serial.begin(9600);
  /*while (!Serial) {;}
  Serial.println(String(getStatus(), BIN)); 
  */
  setSyncProvider(getTeensy3Time);
  
  
  LCDInit();
  
  
  //DrawClear(vram);
  //DrawString(vram, "Hej verden!", 10, 10);
  //LCDUpdate(vram);
}

void loop() 
{
  /*if (Serial.available()) 
  {
    time_t t = processSyncMessage();
    if (t != 0) 
    {
      Teensy3Clock.set(t); // set the RTC
      setTime(t);
    }
  }*/
  digitalClockDisplay();  
  delay(1000);

  /*
  point(pointerX,pointerY); //opretter variabler for x og y koordinaterne til funktionen point
  
  Yakse = analogRead(A9);  // Reads the analog inputs value on pin A9 in volt and converts it to digital.
  Xakse = analogRead(A8);
    if(Yakse>700){
      int sensorValue = analogRead (A8);
    float voltage = sensorValue * (5.0 / 1023.0);
      Serial.print("Spaening:");
      Serial.println(voltage,DEC);
    }
     if(Yakse>750){
      point(pointerX,pointerY--); // Decrease pointerY with 1 when Yakse> 750
    }
    if(Yakse<250){
      point(pointerX,pointerY++); // Increase pointerY with 1 when Yakse < 250
    }
    if(Xakse>700){
      point(pointerX++,pointerY); // Increase pointerX with 1 when Xakse > 800
    }
    if(Xakse<250){
    point(pointerX--,pointerY);  // Decreases pointerX with 1 when Xakse < 250
    }
    if(Yakse > 700 && Xakse > 700) {
      point(pointerX++,pointerY--);
    }
    if(Yakse > 660 && Xakse > 210) {
      point(pointerX--,pointerY--);
    }
    if(Yakse < 250 && Xakse < 250) {
      point(pointerX--,pointerY++);
    }
    if(Yakse < 250 && Xakse > 700) {
      point(pointerX++,pointerY--);
    }
  delay(25);*/
}
