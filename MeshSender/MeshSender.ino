
#include "definitions.h"

int Yakse = 0;
int Xakse = 0;
byte vram[LCD_WIDTH * LCD_HEIGHT / 8]; //frambuffer for the LCD display
byte vram1[LCD_WIDTH * LCD_HEIGHT / 8]; //frambuffer nyt billede til display
int pointerX;
int pointerY;
int Caps = 4;
int val = 0;

unsigned long lastKeyboardInputTime = millis();
char lastKeyboardInput = -1;

const String keyboardLayout[] = {"ijkl", "qrst" , "mnop", "efgh", "abcd", "., !", "zæøå","uvxy"};  //Small letters
const String keyboardLayoutBig[] = {"IJKL", "QRST" , "MNOP", "EFGH", "ABCD", "., !", "ZÆØÅ","UVXY"};  //Big letters
String text = "";
//const String keyboardChar[] = {"abcdefghijklmnopqrstuvxyzæøå"};

void setup() 
{
  RFbegin();
  Serial.begin(9600);
  pinMode(Caps, INPUT);
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
    /* Snake
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
  //Joystick 1 drawing points in Section - Game Snake
  int sector = joystickSector(1);
   
  if (sector == 0 || sector == 1 || sector == 7) pointerX++;
  if (sector == 4 || sector == 3 || sector == 5) pointerX--;
  
  if (sector == 2 || sector == 1 || sector == 3) pointerY--;
  if (sector == 6 || sector == 5 || sector == 7 ) pointerY++;
  
  DrawPoint(vram, pointerX, pointerY);
  //Joystick 1 draw keyboard if you go direction x*/
  
  byte vram2[LCD_WIDTH * LCD_HEIGHT / 8];
    DrawClear(vram2);
  int sector = joystickSector(1, 8); //Make the variable sector = JoystickSector and as parameter choose joystick 1 and splits it up in 8 sections.
    DrawClear(vram);
  
  if(digitalRead(Caps)){ //KIG HER!
  if (sector >= 0 && sector < 8) DrawKeyboard(vram, keyboardLayout[sector], LCD_WIDTH/2, LCD_HEIGHT/2); //Draw the string keyboardlayout on the buffer vram, on in the middle.
  
  if (sector != -1)
  {
    int sector2 = joystickSector(2, 4);
    if (sector2 != -1)
    {
      char keyboardInput = keyboardLayout[sector].charAt(sector2);
      if (millis() > lastKeyboardInputTime + ((lastKeyboardInput == keyboardInput)?400:10) )
      {
        text += keyboardInput;
        lastKeyboardInput = keyboardInput;
        lastKeyboardInputTime = millis();
        //DrawClear(vram1);
        char charBuffer[86];
        text.toCharArray(charBuffer, 86);
        
        DrawClear(vram1);
        DrawString(vram1, charBuffer, 0, 0);
        //Serial.println(text);
      }
      //DrawKeyboard(vram1, keyboardChar[sector2],0,0);
    }
  }
 } 
  if (digitalWrite(Caps, LOW))
  {
  if (sector >= 0 && sector < 8) DrawKeyboard(vram, keyboardLayoutBig[sector], LCD_WIDTH/2, LCD_HEIGHT/2); //Draw the string keyboardlayout on the buffer vram, on in the middle.
  
  if (sector != -1)
  {
    int sector2 = joystickSector(2, 4);
    if (sector2 != -1)
    {
      char keyboardInput = keyboardLayoutBig[sector].charAt(sector2);
      if (millis() > lastKeyboardInputTime + ((lastKeyboardInput == keyboardInput)?400:10) )
      {
        text += keyboardInput;
        lastKeyboardInput = keyboardInput;
        lastKeyboardInputTime = millis();
        //DrawClear(vram1);
        char charBuffer[86];
        text.toCharArray(charBuffer, 86);
        
        DrawClear(vram1);
        DrawString(vram1, charBuffer, 0, 0);
        //Serial.println(text);
      }
      //DrawKeyboard(vram1, keyboardChar[sector2],0,0);
    }
  }
    else
    {
      lastKeyboardInput = -1;
  }
 }

  mergeBuffers(vram2, vram, vram1, LCD_WIDTH * LCD_HEIGHT / 8);
  LCDUpdate(vram2);
  
  //delay(50);
  }

