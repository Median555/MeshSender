#include <SPI.h>

const uint8_t RFcePin = 9, RFcsnPin = 10;
uint8_t RFcurrentConfiguration = NRF24_EN_CRC | NRF24_CRCO;

const uint64_t commonAddress = 0xF0F0F0F042LL; //this address is common for all devices


/**************************************************************************/

boolean RFinit()
{
  //init udgangs porte
  pinMode(RFcePin, OUTPUT);
  digitalWrite(RFcePin, LOW); //enable chip
  pinMode(RFcsnPin, OUTPUT);
  digitalWrite(RFcsnPin, HIGH); //suspend the radio
  
  pinMode(SCK, OUTPUT);
  pinMode(MOSI, OUTPUT);
  
  delay(100); //wait for NRF24
  
  //initialize SPI
  SPI.begin();
  SPI.setDataMode(SPI_MODE0);
  SPI.setBitOrder(MSBFIRST);
  SPI.setClockDivider(SPI_CLOCK_DIV16); //slow enough
  
  //clear flags
  if (!RFspiWriteRegister(NRF24_REG_07_STATUS, NRF24_RX_DR | NRF24_TX_DS | NRF24_MAX_RT))
    return false; //return faliure
  
  RFpowerDown();
  
  RFflushTX();
  RFflushRX();
  
  return RFpowerUpRX();
}

/**************************************************************************/

uint8_t RFspiCommand(uint8_t command)
{
  digitalWrite(RFcsnPin, LOW);
  uint8_t status = SPI.transfer(command);
  digitalWrite(RFcsnPin, HIGH);
  return status;
}

/**************************************************************************/

uint8_t RFspiRead(uint8_t command)
{
    digitalWrite(RFcsnPin, LOW);
    SPI.transfer(command); // Send the address, discard status
    uint8_t val = SPI.transfer(0); // The MOSI value is ignored, value is read
    digitalWrite(RFcsnPin, HIGH);
    return val;
}

/**************************************************************************/

uint8_t RFspiWrite(uint8_t command, uint8_t val)
{
    digitalWrite(RFcsnPin, LOW);
    uint8_t status = SPI.transfer(command);
    SPI.transfer(val); // New register value follows
    digitalWrite(RFcsnPin, HIGH);
    return status;
}

/**************************************************************************/

void RFspiBurstRead(uint8_t command, uint8_t* dest, uint8_t len)
{
    digitalWrite(RFcsnPin, LOW);
    SPI.transfer(command); // Send the start address, discard status
    while (len--)
	*dest++ = SPI.transfer(0); // The MOSI value is ignored, value is read
    digitalWrite(RFcsnPin, HIGH);
    // 300 microsecs for 32 octet payload
}

/**************************************************************************/

uint8_t RFspiBurstWrite(uint8_t command, uint8_t* src, uint8_t len)
{
    digitalWrite(RFcsnPin, LOW);
    uint8_t status = SPI.transfer(command);
    while (len--)
	SPI.transfer(*src++);
    digitalWrite(RFcsnPin, HIGH);
    return status;
}

/**************************************************************************/

uint8_t RFspiReadRegister(uint8_t reg)
{
    return RFspiRead((reg & NRF24_REGISTER_MASK) | NRF24_COMMAND_R_REGISTER);
}

/**************************************************************************/

uint8_t RFspiWriteRegister(uint8_t reg, uint8_t val)
{
    return RFspiWrite((reg & NRF24_REGISTER_MASK) | NRF24_COMMAND_W_REGISTER, val);
}

/**************************************************************************/

void RFspiBurstReadRegister(uint8_t reg, uint8_t* dest, uint8_t len)
{
    return RFspiBurstRead((reg & NRF24_REGISTER_MASK) | NRF24_COMMAND_R_REGISTER, dest, len);
}

/**************************************************************************/

uint8_t RFspiBurstWriteRegister(uint8_t reg, uint8_t* src, uint8_t len)
{
    return RFspiBurstWrite((reg & NRF24_REGISTER_MASK) | NRF24_COMMAND_W_REGISTER, src, len);
}

/**************************************************************************/

uint8_t RFstatusRead()
{
    return RFspiReadRegister(NRF24_REG_07_STATUS);
}

/****************************FLUSH COMMANDS********************************/

uint8_t RFflushTX()
{
    return RFspiCommand(NRF24_COMMAND_FLUSH_TX);
}

uint8_t RFflushRX()
{
    return RFspiCommand(NRF24_COMMAND_FLUSH_RX);
}

/**************************************************************************/

boolean RFsetChannel(uint8_t channel)
{
    RFspiWriteRegister(NRF24_REG_05_RF_CH, channel & NRF24_RF_CH);
    return true;
}
boolean RFsetConfiguration(uint8_t configuration)
{
    RFcurrentConfiguration = configuration;
}

/**************************************************************************/

boolean RFsetPipeAddress(uint8_t pipe, uint8_t* address, uint8_t len)
{
  RFspiBurstWriteRegister(NRF24_REG_0A_RX_ADDR_P0 + pipe, address, len);
  return true;
}

/**************************************************************************/

boolean RFsetRetry(uint8_t delay, uint8_t count)
{
  //delay is calculated as: 250 + 250 * delay [µs]
  RFspiWriteRegister(NRF24_REG_04_SETUP_RETR, ((delay << 4) & NRF24_ARD) | (count & NRF24_ARC));
  return true;
}

/**************************************************************************/

boolean RFsetThisAddress(uint8_t* address, uint8_t len)
{
    // Set pipe 1 for this address
    RFsetPipeAddress(1, address, len); 
    // RX_ADDR_P2 is set to RX_ADDR_P1 with the LSbyte set to 0xff, for use as a broadcast address
    return true;
}

/**************************************************************************/

boolean RFsetTransmitAddress(uint8_t* address, uint8_t len)
{
    // Set both TX_ADDR and RX_ADDR_P0 for auto-ack with Enhanced shockburst
    RFspiBurstWriteRegister(NRF24_REG_0A_RX_ADDR_P0, address, len);
    RFspiBurstWriteRegister(NRF24_REG_10_TX_ADDR, address, len);
    return true;
}

/**************************************************************************/

boolean RFsetPayloadSize(uint8_t size)
{
    RFspiWriteRegister(NRF24_REG_11_RX_PW_P0, size);
    RFspiWriteRegister(NRF24_REG_12_RX_PW_P1, size);
    return true;
}

/**************************************************************************/

boolean RFsetRF(uint8_t data_rate, uint8_t power)
{
    uint8_t value = (power << 1) & NRF24_PWR;
    // Ugly mapping of data rates to noncontiguous 2 bits:
    if (data_rate == NRF24DataRate250kbps)
	value |= NRF24_RF_DR_LOW;
    else if (data_rate == NRF24DataRate2Mbps)
	value |= NRF24_RF_DR_HIGH;
    // else NRF24DataRate1Mbps, 00
    RFspiWriteRegister(NRF24_REG_06_RF_SETUP, value);
    return true;
}

/**************************************************************************/

boolean RFpowerDown()
{
    RFspiWriteRegister(NRF24_REG_00_CONFIG, RFcurrentConfiguration);
    digitalWrite(RFcePin, LOW);
    return true;
}

/**************************************************************************/

boolean RFpowerUpRX()
{
    boolean status = RFspiWriteRegister(NRF24_REG_00_CONFIG, RFcurrentConfiguration | NRF24_PWR_UP | NRF24_PRIM_RX);
    digitalWrite(RFcePin, HIGH);
    delayMicroseconds(150);
    return status;
}

/**************************************************************************/

boolean RFpowerUpTx()
{
    // Its the pulse high that puts us into TX mode
    digitalWrite(RFcePin, LOW);
    boolean status = RFspiWriteRegister(NRF24_REG_00_CONFIG, RFcurrentConfiguration | NRF24_PWR_UP);
    digitalWrite(RFcePin, HIGH);
    return status;
}

/**************************************************************************/

boolean RFsend(uint8_t* data, uint8_t len, boolean noack)
{
    RFpowerUpTx();
    RFspiBurstWrite(noack ? NRF24_COMMAND_W_TX_PAYLOAD_NOACK : NRF24_COMMAND_W_TX_PAYLOAD, data, len);
    // Radio will return to Standby II mode after transmission is complete
    return true;
}

/**************************************************************************/

boolean RFwaitPacketSent()
{
    // If we are currently in receive mode, then there is no packet to wait for
    if (RFspiReadRegister(NRF24_REG_00_CONFIG) & NRF24_PRIM_RX) return false;

    // Wait for either the Data Sent or Max ReTries flag, signalling the 
    // end of transmission
    uint8_t status;
    while (!((status = RFstatusRead()) & (NRF24_TX_DS | NRF24_MAX_RT)))
	;

    // Must clear NRF24_MAX_RT if it is set, else no further comm
    RFspiWriteRegister(NRF24_REG_07_STATUS, NRF24_TX_DS | NRF24_MAX_RT);
    if (status & NRF24_MAX_RT)
	RFflushTX();
    // Return true if data sent, false if MAX_RT
    //Serial.println(String(status, BIN) + " & " + String(NRF24_TX_DS, BIN));
    return status & NRF24_TX_DS;
}

/**************************************************************************/

boolean RFisSending()
{
    return !(RFspiReadRegister(NRF24_REG_00_CONFIG) & NRF24_PRIM_RX) && !(RFstatusRead() & (NRF24_TX_DS | NRF24_MAX_RT));
}

/**************************************************************************/

boolean RFprintRegisters()
{
    uint8_t registers[] = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0d, 0x0f, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x1c, 0x1d};

    uint8_t i;
    for (i = 0; i < sizeof(registers); i++)
    {
	Serial.print(i, HEX);
	Serial.print(": ");
	Serial.println(RFspiReadRegister(i), HEX);
    }
    return true;
}

/**************************************************************************/

boolean RFavailable()
{
    if (RFspiReadRegister(NRF24_REG_17_FIFO_STATUS) & NRF24_RX_EMPTY)
	return false;
    // Manual says that messages > 32 octets should be discarded
    if (RFspiRead(NRF24_COMMAND_R_RX_PL_WID) > 32)
    {
	RFflushRX();
	return false;
    }
    return true;
}

/**************************************************************************/

void RFwaitAvailable()
{
    RFpowerUpRX();
    while (!RFavailable())
	;
}

/**************************************************************************/

// Blocks until a valid message is received or timeout expires
// Return true if there is a message available
// Works correctly even on millis() rollover
bool RFwaitAvailableTimeout(uint16_t timeout)
{
    RFpowerUpRX();
    unsigned long starttime = millis();
    while ((millis() - starttime) < timeout)
        if (RFavailable())
           return true;
    return false;
}

/**************************************************************************/

boolean RFrecv(uint8_t* buf, uint8_t* len)
{
    // Clear read interrupt
    RFspiWriteRegister(NRF24_REG_07_STATUS, NRF24_RX_DR);

    // 0 microsecs @ 8MHz SPI clock
    if (!RFavailable())
	return false;
    // 32 microsecs (if immediately available)
    *len = RFspiRead(NRF24_COMMAND_R_RX_PL_WID);
    // 44 microsecs
    RFspiBurstRead(NRF24_COMMAND_R_RX_PAYLOAD, buf, *len);
    // 140 microsecs (32 octet payload)

    return true;
}
