#include <SPI.h>

const uint8_t RFcePin = 9, RFcsnPin = 10;
const uint64_t commonAddress = 0xF0F0F0F042LL; //this address is common for all devices

/*--------------------------------------------------------*/

void ce(uint8_t state)
{
  digitalWrite(RFcePin, state);
}

void csn(uint8_t state)
{
  SPI.setBitOrder(MSBFIRST);
  SPI.setDataMode(SPI_MODE0);
  SPI.setClockDivider(SPI_CLOCK_DIV4);
  
  digitalWrite(RFcsnPin, state);
}

/*--------------------------------------------------------*/

void RFbegin()
{
  pinMode(RFcePin, OUTPUT);
  pinMode(RFcsnPin, OUTPUT);
  
  SPI.begin();
  
  ce(LOW); //enable the chip @TODO: can be removed?
  csn(HIGH); //always enable the SPI interface
  
  delay(5); //let the radio settle
  
  //ARD: set Auto-retransmit delay to 2 ms
  //ARC: set Auto-retransmit count to max of 15
  writeRegister(RF24_SETUP_RETR, (B0110 << RF24_ARD) | (B1111 << RF24_ARC));
  
  //set PA level to 0 dBm and data rate to 1 Mbps
  uint8_t RFSetup = readRegister((uint8_t)RF24_RF_SETUP); //get the current setup
  RFSetup &= ~(1 << 3); //RF_DR = 1 Mbps
  RFSetup |= (1 << 1) | (1 << 2); //RF_PWR = 0 dBm
  writeRegister(RF24_RF_SETUP, RFSetup); //write the register back
  
  //enable CRC and set CRCO = 2 bytes
  uint8_t currentConfig = readRegister(RF24_CONFIG);
  currentConfig |= 1 << 3; //EN_CRC = 1
  currentConfig |= 1 << 2; //CRCO = 2 bytes
  writeRegister(RF24_CONFIG, currentConfig);
  
  //disable dynamic payload for all pipes
  writeRegister(RF24_DYNPD, 0);
  
  //clear status register
  writeRegister(RF24_STATUS, 0x70);
  //0x70 = (1 << RX_DR) | (1 << TX_DS) | (1 << MAX_RT)
  
  //set channel
  setChannel(42);
  
  
  //open the reading pipes
  writeRegister(RF24_RX_ADDR_P0, reinterpret_cast<uint8_t*>(commonAddress), 5); //write common address to RX pipe 0
  //writeRegister(RF24_RX_ADDR_P1, reinterpret_cast<uint8_t*>(0xF042CC1337LL), 5); //write own addres to RX pipe 1
  
  writeRegister(RF24_RX_PW_P0, 32); //write static payload length to pipe0
  
  uint8_t previous = readRegister(RF24_EN_RXADDR);
  writeRegister(RF24_EN_RXADDR, previous | (1 << 0)); //enable the reading pipes
  
  
  //open writing pipe
  writeRegister(RF24_TX_ADDR, reinterpret_cast<uint8_t*>(commonAddress), 5);
  
  
  //flush all
  flushRX();
  flushTX();
}

/*--------------------------------------------------------*/

void setChannel(uint8_t newChannel)
{
  //make sure we don't write outside of the channel spectrum
  writeRegister(RF24_RF_CH, min(newChannel, 127));
}

/*--------------------------------------------------------*/

//read the register and the status
uint8_t readRegister(uint8_t reg, uint8_t * buffer, uint8_t length)
{
  uint8_t recievedStatus;
  
  csn(LOW); //disable chip activities
  
  recievedStatus = SPI.transfer(RF24_R_REGISTER | (RF24_REGISTER_MASK & reg)); //recieve status and request register
  while(length--) *buffer = SPI.transfer(0xFF); //recieve register
  
  csn(HIGH); //re-enable chip
  
  return recievedStatus;
}

/*--------------------------------------------------------*/

//read a single register, disregard the status
uint8_t readRegister(uint8_t reg)
{
  uint8_t out;
  
  csn(LOW); //disable chip activities
  
  SPI.transfer(RF24_R_REGISTER | (RF24_REGISTER_MASK & reg)); //request register
  out = SPI.transfer(0xFF); //recieve register
  
  csn(HIGH); //re-enable chip
  
  return out;
}

/*--------------------------------------------------------*/

//write buffer to a register, returns status
uint8_t writeRegister(uint8_t reg, uint8_t * buffer, uint8_t length)
{
  uint8_t statusOut;
  
  csn(LOW);
  statusOut = SPI.transfer(RF24_W_REGISTER | (RF24_REGISTER_MASK | reg));
  while(length--) //for the length of the buffer
    SPI.transfer(*buffer++); //write the pointed @ value and increment
  csn(HIGH);
  
  return statusOut;
}

/*--------------------------------------------------------*/

//write a single value to reg, return the status
uint8_t writeRegister(uint8_t reg, uint8_t value)
{
  uint8_t statusOut;
  
  csn(LOW);
  statusOut = SPI.transfer(RF24_W_REGISTER | (RF24_REGISTER_MASK | reg));
  SPI.transfer(value); //trasfer the value
  csn(HIGH);
  
  return statusOut;
}

/*--------------------------------------------------------*/

//return status
int getStatus()
{
  int statusOut;
  
  csn(LOW);
  statusOut = SPI.transfer(RF24_NOP);
  csn(HIGH);
  
  return statusOut;
}

/*--------------------------------------------------------*/

uint8_t flushRX()
{
  uint8_t statusOut;
  
  csn(LOW);
  statusOut = SPI.transfer(RF24_FLUSH_RX);
  csn(HIGH);
  
  return statusOut;
}

uint8_t flushTX()
{
  uint8_t statusOut;
  
  csn(LOW);
  statusOut = SPI.transfer(RF24_FLUSH_TX);
  csn(HIGH);
  
  return statusOut;
}

/*--------------------------------------------------------*/

void startListening()
{
  //power up, set as PRX
  uint8_t currentConfig = readRegister(RF24_CONFIG);
  currentConfig |= (1 << RF24_PWR_UP) | (1 << RF24_PRIM_RX);
  writeRegister(RF24_CONFIG, currentConfig);
  
  //clear interrupt flags
  writeRegister(RF24_STATUS, (1 << RF24_RX_DR) | (1 << RF24_TX_DS) | (1 << RF24_MAX_RT));
  
  //flush buffers
  flushRX();
  flushTX();
  
  //activate radio
  ce(HIGH);
  
  //wait for RX settle
  delayMicroseconds(130);
}

/*--------------------------------------------------------*/

void stopListening()
{
  ce(LOW);
  flushRX();
  flushTX();
}

/*--------------------------------------------------------*/

uint8_t sendTrasmission(const void *buffer, uint8_t length)
{
  uint8_t statusOut;
  
  //switch to transmitter
  uint8_t currentConfig = readRegister(RF24_CONFIG);
  currentConfig |= (1 << RF24_PWR_UP);
  currentConfig &= ~(1 << RF24_PRIM_RX);
  writeRegister(RF24_CONFIG, currentConfig);
  
  //TX settle
  delayMicroseconds(150);
  
  //write payload
  const uint8_t *payload = reinterpret_cast<const uint8_t*>(buffer);
  uint8_t blank_amount = 32 - length;
  csn(LOW);
  statusOut = SPI.transfer(RF24_W_TX_PAYLOAD); //write payload command
  while(length--) SPI.transfer(*payload++); //transfer payload
  while(blank_amount--) SPI.transfer(0);
  csn(HIGH);
  
  //activate radio, send message
  ce(HIGH);
  delayMicroseconds(15);
  ce(LOW);
  
  return statusOut;
}

/*--------------------------------------------------------*/
