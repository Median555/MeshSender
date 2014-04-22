
#include "font.h"

#define LCD_RST 12
#define LCD_SCE 11
#define LCD_DC 10
#define LCD_DN 9
#define LCD_SCLK 8
#define LCD_BACKLIGHT 6
#define DELAY 50
#define LCD_WIDTH 84
#define LCD_HEIGHT 48


int Yakse = 0;
int Xakse = 0;
byte vram[LCD_WIDTH * LCD_HEIGHT / 8]; //frambuffer for the LCD display
int pointerX;
int pointerY;

//initialize LCD display
void LCDInit()
{
  // "Her sætter vi alle pins vi skal bruge på vores Teensy 3.1 til
  // output istedet for input som de er fra standard.
  pinMode(LCD_RST, OUTPUT);
  pinMode(LCD_SCE, OUTPUT);
  pinMode(LCD_DC, OUTPUT);
  pinMode(LCD_DN, OUTPUT);
  pinMode(LCD_SCLK, OUTPUT);
  pinMode(LCD_BACKLIGHT, OUTPUT);
  
  //toggle reset to ensure reset
  digitalWrite(LCD_RST, LOW);
  digitalWrite(LCD_RST, HIGH);
  
  LCDWriteCmd(0x21); //H = 1, skifter kommando-set til 1
  LCDWriteCmd(0x9F); //sætter kontrast
  LCDWriteCmd(0x04); //sætter temperatur koefficient til 0 (don't touch)
  LCDWriteCmd(0x14); //sætter Bias voltage til 1:40, kan ændres
  LCDWriteCmd(0x20); //H = 0, skifter kommandoset til 0
  LCDWriteCmd(0x0C); //Display control, tænder alle segmenter
  
  LCDClear();
  
  digitalWrite(LCD_BACKLIGHT, HIGH);
}


//Converts Ascii-code to letters
/*void LcdWriteCharacter(char character)
{
  for(int i=0; i<5; i++) LCDWriteData(ASCII[character - 0x20][i]);
  LCDWriteData(0x00); //makes a space
}

//Write a string of characters to the display
void LcdWriteString(char *characters) 
{
  while(*characters) LcdWriteCharacter(*characters++);
}*/


//Write a string of characters to the display at (x, y)
void DrawString(byte *buffer, char *characters, uint8_t x, uint8_t y) 
{
  char* startAdress = characters; //store the location of the beginning of the string
  int xOffsetStart; 
  int xOffset = xOffsetStart = (y / 8) * LCD_WIDTH; //the index in the buffer, offset from the x-cordinate
  uint8_t yOffset = y % 8; //vertical offset on the line
  
  while(*characters) //until we reach the end of the characters
  {
      //write out each character
      for(int i=0; i<5; i++)
          buffer[x + xOffset++] |= (ASCII[*characters - 0x20][i]) << yOffset; //'add' the character, nudge it down
      xOffset++; //make a space
      *characters++;
  }
  
  //reset to beginning
  xOffset = xOffsetStart;
  characters = startAdress;
  while(*characters) //until we reach the end of the characters
  {
      //write out each character
      for(int i=0; i<5; i++)
          buffer[x + LCD_WIDTH + xOffset++] |= (ASCII[*characters - 0x20][i]) >> (8 - yOffset); //'add' the character, nudgde it the opposite way
      xOffset++; //make a space
      *characters++;
  }
  
}

//write data to the display
void LCDWriteData(byte dat) 
{
 digitalWrite(LCD_DC, HIGH); //DC pin set HIGH to send data
 digitalWrite(LCD_SCE, LOW);
 shiftOut(LCD_DN, LCD_SCLK, MSBFIRST,dat);
 digitalWrite(LCD_SCE, HIGH);
}

//write cmd to the display
void LCDWriteCmd(byte cmd) //Den funktion vi bruge når vi skal skrive commands til vores skærm
{
 digitalWrite(LCD_DC,LOW); //DC pin set LOW to send commands
 digitalWrite(LCD_SCE, LOW);
 shiftOut(LCD_DN, LCD_SCLK, MSBFIRST,cmd);
 digitalWrite(LCD_SCE, HIGH);
}

//@TODO: make this work :|
void LCDLine(int x0, int y0, int x1, int y1)
{
  int dx = abs(x1 - x0);
  int dy = abs(y1 - y0);
  int sx = (x0 < x1)? 1 : -1;
  int sy = (y0 < y1)? 1 : -1;
  int err = dx - dy;
  int e2;
  
  while(1)
  {
    //point(x0, y0);
    if (x0 == x1 && y0 == y1) break;
    e2 = 2 * err;
    if (e2 > -dy)
    {
      err -= dy;
      x0 += dx;
    }
    if (e2 < dx)
    {
      err += dx;
      y0 += sy;
    }
  }
}

void LCDClear()
{
  for(int i = 0; i < LCD_WIDTH * LCD_HEIGHT / 8; i++)
  {
      LCDWriteData(0x00);
  }
}

void DrawClear(byte *buffer)
{
  for(int i = 0; i < LCD_WIDTH * LCD_HEIGHT / 8; i++)
  {
      buffer[i] = 0;
  }
}

//draw buffer to display
void LCDUpdate(byte *buffer)
{
  //set cursor to (0, 0)
  LCDWriteCmd(0x20);
  LCDWriteCmd(0x40);
  LCDWriteCmd(0x80);
  //send buffer to display
  for (int i = 0; i < LCD_WIDTH * LCD_HEIGHT / 8; i++)
  {
   LCDWriteData(buffer[i]); 
  }
}


//draw a single black point
void DrawPoint(byte *buffer, int x, int y)
{
  if (x < 0 || x > LCD_WIDTH - 1) return;
  if (y < 0 || y > LCD_HEIGHT - 1) return;
  int i = x + (y/8)* LCD_WIDTH;
  buffer[i] |= 1 << (y%8);
}


void setup() 
{
  Serial.begin(9600);
  
  LCDInit();
  
  DrawClear(vram);
  DrawString(vram, "Hej verden!", 10, 10);
  LCDUpdate(vram);
}

void loop() 
{
  
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