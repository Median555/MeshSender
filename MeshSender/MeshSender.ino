
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

int Yakse = 0;
int Xakse = 0;
//byte vram[LCD_WIDTH * LCD_HEIGHT / 8]; //frambuffer for the LCD display
int pointerX;
int pointerY;

const boolean isSender = true;
const uint64_t pipes[2] = { 0xF0F0F0F0E1LL, 0xF0F0F0F0D2LL };

void setup() 
{
  
  Serial.begin(9600);
  while (!Serial) {;}
  delay(500);
  
  if (!RFinit()) Serial.println("RF init fail");
  if (!RFsetChannel(69)) Serial.println("RF failed to set channel");
  
  if (!isSender) if (!RFsetThisAddress((uint8_t*)"aurx1", 5)) Serial.println("setThisAddress failed");
  
  /*if (isSender)
    if (!RFsetThisAddress((uint8_t*)"clie1", 5)) Serial.println("RF failed to set thisAddress");
  else
    if (!RFsetThisAddress((uint8_t*)"serv1", 5)) Serial.println("RF failed to set thisAddress");*/
  if (!RFsetPayloadSize(32)) Serial.println("RF failed to set payload size");
  if (!RFsetRF(NRF24DataRate1Mbps, NRF24TransmitPower0dBm)) Serial.println("RF setRF failed");
  
  delay(100);
  
  if (isSender) 
    RFspiWriteRegister(NRF24_REG_1D_FEATURE, NRF24_EN_DYN_ACK);
  else
    Serial.println("powerOnRX: " + String(RFpowerUpRX(), BIN));// Serial.println("powerOnRx failed");   
  
  Serial.println("RF init succes!");
}

void loop() 
{
  if (isSender)
  {
    uint8_t buf[32] = {'H', 'e', 'j', ' ', 'm', 'e', 'd', ' ', 'd', 'i', 'g', '!', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '};
    uint8_t i;
    
    // Collect 32 audio samples. At 109 microsecs per sample, we can achieve about 250
    // 32 byte packets transmitted per second: 6.4kHz sample rate
    //for (i = 0; i < 32; i++) buf[i] = analogRead(0); // 0 - 1023 becomes 0 - 255, top 2 bits clipped. approx 109 microsecs per sample
  
    // Now send the samples NOACK (EN_DYN_ACK must be enabled first)
    // With 2Mbps, NOACK and 32 byte payload, can send about 1900 messages per sec
    if (!RFsetTransmitAddress((uint8_t*)"aurx1", 5))
     Serial.println("setTransmitAddress failed");
    // Send the data  
    if (!RFsend((uint8_t*)"Hej med dig, din snuskemuzzz! :)", sizeof(buf), true)) // NOACK, 110 microsecs
       Serial.println("send failed");  
    // Transmission takes about 300 microsecs, of which about 130microsecs is transmitter startup time
    // and 160 microsecs is transmit time for 32 bytes+8 bytes overhead @ 2Mbps
    if (!RFwaitPacketSent())
       Serial.println("waitPacketSent failed");
    
    delay(1000);
  }
  else
  {
    uint8_t buf[32];
    uint8_t len = sizeof(buf);
  
    RFwaitAvailable();
    if (RFrecv(buf, &len)) // 140 microsecs
    {
      uint8_t i;
      for (i = 0; i < 32; i++)
      {
        Serial.print(char(buf[i])); // 15 microsecs
      }
    }
  }
}
