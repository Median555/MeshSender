
#include "definitions.h"

int Yakse = 0;
int Xakse = 0;
byte vram[LCD_WIDTH * LCD_HEIGHT / 8]; //frambuffer for the LCD display
int pointerX;
int pointerY;

void setup() 
{
  RFbegin();
  Serial.begin(9600);
  while (!Serial) {;}
  Serial.println(String(getStatus(), BIN));
  
  LCDInit();
  DrawClear(vram);
  DrawString(vram, "Hej verden!", 10, 10);
  LCDUpdate(vram);
  
  pointerX = LCD_WIDTH >> 1;
  pointerY = LCD_HEIGHT >> 1;
}

int last1 = 0;
int last2 = 0;

void loop() 
{
  
  if (last1 != joystickSector(1))
  {
    last1 = joystickSector(1);
    Serial.println(last1);
  }
  
  if (last2 != joystickSector(2))
  {
    last2 = joystickSector(2);
    Serial.println(last2);
  }
  
  int sector = joystickSector(1);
  
  if (sector == 0) pointerX++;
  if (sector == 4) pointerX--;
  
  if (sector == 2) pointerY--;
  if (sector == 6) pointerY++;
  
  DrawPoint(vram, pointerX, pointerY);
  
  LCDUpdate(vram);
  
  delay(25);
}
