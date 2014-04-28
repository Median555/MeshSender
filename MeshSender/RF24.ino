


#include <SPI.h>

const uint8_t RFcePin = 9, RFcsnPin = 10;


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

