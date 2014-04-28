
#include "definitions.h"

int Yakse = 0;
int Xakse = 0;
//byte vram[LCD_WIDTH * LCD_HEIGHT / 8]; //frambuffer for the LCD display
int pointerX;
int pointerY;

const boolean isSender = false;
const uint64_t pipes[2] = { 0xF0F0F0F0E1LL, 0xF0F0F0F0D2LL };

void setup() 
{
  
  Serial.begin(9600);
  while (!Serial) {;}
  delay(1000);
  RFbegin();
  
  if (isSender)
  {
    openWritingPipe(pipes[0]);
    openReadingPipe(1, pipes[1]);
  }
  else
  {
    openWritingPipe(pipes[1]);
    openReadingPipe(1, pipes[0]);
  }
  
  startListening();
}

void loop() 
{
  if (isSender)
  {
    stopListening();
    Serial.println(String(getStatus(), BIN));
    Serial.println(String(readRegister(RF24_OBSERVE_TX), BIN));
    
    uint8_t num = 0x42;
    sendTransmission(&num, sizeof(uint8_t));
    
    delay(1000);
  }
  else
  {
    if (RFAvailable())
    {
      Serial.println("Recieved something!");
      delay(100);
    }
  }
}
