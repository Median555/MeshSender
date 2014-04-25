
#include "definitions.h"

int Yakse = 0;
int Xakse = 0;
byte vram[LCD_WIDTH * LCD_HEIGHT / 8]; //frambuffer for the LCD display
byte vram1[LCD_WIDTH * LCD_HEIGHT / 8]; //frambuffer nyt billede til display
int pointerX;
int pointerY;

const String keyboardLayout[] = {"abcd", "efgh", "ijkl", "mnop", "qrst", "uvxy", "zæøå", ".,_!"};


void setup() 
{
  RFbegin();
  Serial.begin(9600);
  //while (!Serial) {;}
  Serial.println(String(getStatus(), BIN));
  
  LCDInit();
  DrawClear(vram);
  //DrawString(vram, "Hej verden!", 10, 10);
  LCDUpdate(vram);
  
  pointerX = LCD_WIDTH >> 1;
  pointerY = LCD_HEIGHT >> 1;
}

int last1 = 0;
int last2 = 0;

void loop() 
{
    /*
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
  //Joystick 1 drawing points in Section
  int sector = joystickSector(1);
   
  if (sector == 0 || sector == 1 || sector == 7) pointerX++;
  if (sector == 4 || sector == 3 || sector == 5) pointerX--;
  
  if (sector == 2 || sector == 1 || sector == 3) pointerY--;
  if (sector == 6 || sector == 5 || sector == 7 ) pointerY++;
  
  DrawPoint(vram, pointerX, pointerY);
  //Joystick 1 draw keyboard if you go direction x*/
  
  
  int sector = joystickSector(1);
  DrawClear(vram);
  if (sector >= 0 && sector < 8) DrawKeyboard(vram, keyboardLayout[sector], LCD_WIDTH/2, LCD_HEIGHT/2);
  
  LCDUpdate(vram);
  
  //delay(50);
}
