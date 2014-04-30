
#include "definitions.h"

typedef enum
{
  NRF24DataRate1Mbps = 0,   ///< 1 Mbps
  NRF24DataRate2Mbps,       ///< 2 Mbps
  NRF24DataRate250kbps      ///< 250 kbps
} NRF24DataRate;

typedef enum
{
  NRF24TransmitPowerm18dBm = 0,   ///< -18 dBm
  NRF24TransmitPowerm12dBm,       ///< -12 dBm
  NRF24TransmitPowerm6dBm,        ///< -6 dBm
  NRF24TransmitPower0dBm          ///< 0 dBm
} NRF24TransmitPower;

typedef enum
{
  RFReciever = 0,
  RFTransmitter
} RFRole;

int Yakse = 0;
int Xakse = 0;
//byte vram[LCD_WIDTH * LCD_HEIGHT / 8]; //frambuffer for the LCD display
int pointerX;
int pointerY;

int role = RFTransmitter;

void setup() 
{
  
  Serial.begin(9600);
  while (!Serial) {;}
  delay(500);
  
  if (!RFinit()) Serial.println("RF init fail");
  if (!RFsetChannel(69)) Serial.println("RF failed to set channel");
  
  if (role == RFReciever) if (!RFsetThisAddress((uint8_t*)"dev00", 5)) Serial.println("setThisAddress failed");
  
  if (!RFsetPayloadSize(32)) Serial.println("RF failed to set payload size");
  if (!RFsetRF(NRF24DataRate1Mbps, NRF24TransmitPower0dBm)) Serial.println("RF setRF failed");
  
  if (role == RFTransmitter)
  {
    RFspiWriteRegister(NRF24_REG_1D_FEATURE, NRF24_EN_DYN_ACK);
  }
  
<<<<<<< HEAD
  /*
  LCDInit();
  
  DrawClear(vram);
  DrawString(vram, "Hej verden!", 10, 10);
  LCDUpdate(vram);*/
=======
  if (role == RFReciever)
  {
    RFpowerUpRX();
  }
  
  delay(100);
  Serial.println("RF init succes!");
>>>>>>> origin/RF-devlopment
}

int count = 0;

void loop() 
{
  //transmitter
  if (role == RFTransmitter)
  {
    Serial.println("Trying to send...");
    
    uint8_t buf[32];
    uint8_t i;
  
    // Now send the samples NOACK (EN_DYN_ACK must be enabled first)
    // With 2Mbps, NOACK and 32 byte payload, can send about 1900 messages per sec
    
    // Send the data  
    if (!RFsetTransmitAddress((uint8_t*)"dev00", 5)) Serial.println("setTransmitAddress failed");
    if (!RFsend((uint8_t*)"Hej med dig, din snuskemuzzz! :)", sizeof(buf), true)) Serial.println("send failed"); //NOACK
    if (!RFwaitPacketSent()) Serial.println("waitPacketSent failed");
    
    delay(1000);
  }
  
  
  //receiver
  if (role == RFReciever)
  {
    Serial.println("Trying to listen...");
    uint8_t buf[32];
    uint8_t len = sizeof(buf);
  
    RFwaitAvailableTimeout(1500);
    if (RFrecv(buf, &len)) // 140 microsecs
    {
      uint8_t i;
      Serial.print(String(role) + " recieved:");
      for (i = 0; i < 32; i++)
      {
        Serial.print(char(buf[i])); // 15 microsecs
      }
      Serial.println();
    }
  }
  
  
  count++;
  if (count == 3)
  {
    count = 0;
    if (role == RFTransmitter)
    {
      //change to reciever
      role = RFReciever;
      if (!RFsetThisAddress((uint8_t*)"dev00", 5)) Serial.println("setThisAddress failed");
      RFpowerUpRX();
    }
    else if (role == RFReciever)
    {
      //change to transmitter
      role = RFTransmitter;
      RFspiWriteRegister(NRF24_REG_1D_FEATURE, NRF24_EN_DYN_ACK); //enable NOACK
    }
  }
}
