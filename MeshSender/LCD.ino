//initialize LCD display
void LCDInit()
{
  // Sets all the pins to the display to output
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
  LCDWriteCmd(0xA8); //sætter kontrast
  LCDWriteCmd(0x04); //sætter temperatur koefficient til 0 (don't touch)
  LCDWriteCmd(0x14); //sætter Bias voltage til 1:40, kan ændres
  LCDWriteCmd(0x20); //H = 0, skifter kommandoset til 0
  LCDWriteCmd(0x0C); //Display control, tænder alle segmenter
  
  LCDClear();
  
  digitalWrite(LCD_BACKLIGHT, HIGH);
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

void LCDClear()
{
  for(int i = 0; i < LCD_WIDTH * LCD_HEIGHT / 8; i++)
  {
      LCDWriteData(0x00);
  }
}
