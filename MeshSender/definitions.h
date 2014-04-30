#define LCD_RST 12
#define LCD_SCE 11
#define LCD_DC 10
#define LCD_DN 9
#define LCD_SCLK 8
#define LCD_BACKLIGHT 6
#define DELAY 50
#define LCD_WIDTH 84
#define LCD_HEIGHT 48


//"Borrowed definetions"
<<<<<<< HEAD
/* Memory Map */
#define RF24_CONFIG      0x00
#define RF24_EN_AA       0x01
#define RF24_EN_RXADDR   0x02
#define RF24_SETUP_AW    0x03
#define RF24_SETUP_RETR  0x04
#define RF24_RF_CH       0x05
#define RF24_RF_SETUP    0x06
#define RF24_STATUS      0x07
#define RF24_OBSERVE_TX  0x08
#define RF24_CD          0x09
#define RF24_RX_ADDR_P0  0x0A
#define RF24_RX_ADDR_P1  0x0B
#define RF24_RX_ADDR_P2  0x0C
#define RF24_RX_ADDR_P3  0x0D
#define RF24_RX_ADDR_P4  0x0E
#define RF24_RX_ADDR_P5  0x0F
#define RF24_TX_ADDR     0x10
#define RF24_RX_PW_P0    0x11
#define RF24_RX_PW_P1    0x12
#define RF24_RX_PW_P2    0x13
#define RF24_RX_PW_P3    0x14
#define RF24_RX_PW_P4    0x15
#define RF24_RX_PW_P5    0x16
#define RF24_FIFO_STATUS 0x17
#define RF24_DYNPD	    0x1C
#define RF24_FEATURE	    0x1D

/* Bit Mnemonics */
#define RF24_MASK_RX_DR  6
#define RF24_MASK_TX_DS  5
#define RF24_MASK_MAX_RT 4
#define RF24_EN_CRC      3
#define RF24_CRCO        2
#define RF24_PWR_UP      1
#define RF24_PRIM_RX     0
#define RF24_ENAA_P5     5
#define RF24_ENAA_P4     4
#define RF24_ENAA_P3     3
#define RF24_ENAA_P2     2
#define RF24_ENAA_P1     1
#define RF24_ENAA_P0     0
#define RF24_ERX_P5      5
#define RF24_ERX_P4      4
#define RF24_ERX_P3      3
#define RF24_ERX_P2      2
#define RF24_ERX_P1      1
#define RF24_ERX_P0      0
#define RF24_AW          0
#define RF24_ARD         4
#define RF24_ARC         0
#define RF24_PLL_LOCK    4
#define RF24_RF_DR       3
#define RF24_RF_PWR      6
#define RF24_RX_DR       6
#define RF24_TX_DS       5
#define RF24_MAX_RT      4
#define RF24_RX_P_NO     1
#define RF24_TX_FULL     0
#define RF24_PLOS_CNT    4
#define RF24_ARC_CNT     0
#define RF24_TX_REUSE    6
#define RF24_FIFO_FULL   5
#define RF24_TX_EMPTY    4
#define RF24_RX_FULL     1
#define RF24_RX_EMPTY    0
#define RF24_DPL_P5	    5
#define RF24_DPL_P4	    4
#define RF24_DPL_P3	    3
#define RF24_DPL_P2	    2
#define RF24_DPL_P1	    1
#define RF24_DPL_P0	    0
#define RF24_EN_DPL	    2
#define RF24_EN_ACK_PAY  1
#define RF24_EN_DYN_ACK  0

/* Instruction Mnemonics */
#define RF24_R_REGISTER    0x00
#define RF24_W_REGISTER    0x20
#define RF24_REGISTER_MASK 0x1F
#define RF24_ACTIVATE      0x50
#define RF24_R_RX_PL_WID   0x60
#define RF24_R_RX_PAYLOAD  0x61
#define RF24_W_TX_PAYLOAD  0xA0
#define RF24_W_ACK_PAYLOAD 0xA8
#define RF24_FLUSH_TX      0xE1
#define RF24_FLUSH_RX      0xE2
#define RF24_REUSE_TX_PL   0xE3
#define RF24_NOP           0xFF

/* Non-P omissions */
#define RF24_LNA_HCURR   0

/* P model memory Map */
#define RF24_RPD         0x09

/* P model bit Mnemonics */
#define RF24_RF_DR_LOW   5
#define RF24_RF_DR_HIGH  3
#define RF24_RF_PWR_LOW  1
#define RF24_RF_PWR_HIGH 2
=======
// This is the bit in the SPI address that marks it as a write
#define NRF24_SPI_WRITE_MASK 0x80

// This is the maximum message length that can be supported by this library. Limited by
// the suported message lengths oin the nRF24
// Can be pre-defined to a smaller size (to save SRAM) prior to including this header
#ifndef NRF24_MAX_MESSAGE_LEN
#define NRF24_MAX_MESSAGE_LEN 32
#endif

// Keep track of the mode the NRF24 is in
#define NRF24_MODE_IDLE         0
#define NRF24_MODE_RX           1
#define NRF24_MODE_TX           2

// These values we set for FIFO thresholds are actually the same as the POR values
#define NRF24_TXFFAEM_THRESHOLD 4
#define NRF24_RXFFAFULL_THRESHOLD 55

// This is the default node address,
#define NRF24_DEFAULT_NODE_ADDRESS 0x00000000

// This address in the TO addreess signifies a broadcast
#define NRF24_BROADCAST_ADDRESS 0xffffffffff

// SPI Command names
#define NRF24_COMMAND_R_REGISTER                        0x00
#define NRF24_COMMAND_W_REGISTER                        0x20
#define NRF24_COMMAND_R_RX_PAYLOAD                      0x61
#define NRF24_COMMAND_W_TX_PAYLOAD                      0xa0
#define NRF24_COMMAND_FLUSH_TX                          0xe1
#define NRF24_COMMAND_FLUSH_RX                          0xe2
#define NRF24_COMMAND_REUSE_TX_PL                       0xe3
#define NRF24_COMMAND_R_RX_PL_WID                       0x60
#define NRF24_COMMAND_W_ACK_PAYLOAD(pipe)               (0xa8|(pipe&0x7))
#define NRF24_COMMAND_W_TX_PAYLOAD_NOACK                0xb0
#define NRF24_COMMAND_NOP                               0xff

// Register names
#define NRF24_REGISTER_MASK                             0x1f
#define NRF24_REG_00_CONFIG                             0x00
#define NRF24_REG_01_EN_AA                              0x01
#define NRF24_REG_02_EN_RXADDR                          0x02
#define NRF24_REG_03_SETUP_AW                           0x03
#define NRF24_REG_04_SETUP_RETR                         0x04
#define NRF24_REG_05_RF_CH                              0x05
#define NRF24_REG_06_RF_SETUP                           0x06
#define NRF24_REG_07_STATUS                             0x07
#define NRF24_REG_08_OBSERVE_TX                         0x08
#define NRF24_REG_09_RPD                                0x09
#define NRF24_REG_0A_RX_ADDR_P0                         0x0a
#define NRF24_REG_0B_RX_ADDR_P1                         0x0b
#define NRF24_REG_0C_RX_ADDR_P2                         0x0c
#define NRF24_REG_0D_RX_ADDR_P3                         0x0d
#define NRF24_REG_0E_RX_ADDR_P4                         0x0e
#define NRF24_REG_0F_RX_ADDR_P5                         0x0f
#define NRF24_REG_10_TX_ADDR                            0x10
#define NRF24_REG_11_RX_PW_P0                           0x11
#define NRF24_REG_12_RX_PW_P1                           0x12
#define NRF24_REG_13_RX_PW_P2                           0x13
#define NRF24_REG_14_RX_PW_P3                           0x14
#define NRF24_REG_15_RX_PW_P4                           0x15
#define NRF24_REG_16_RX_PW_P5                           0x16
#define NRF24_REG_17_FIFO_STATUS                        0x17
#define NRF24_REG_1C_DYNPD                              0x1c
#define NRF24_REG_1D_FEATURE                            0x1d

// These register masks etc are named wherever possible
// corresponding to the bit and field names in the nRF24L01 Product Specification
// #define NRF24_REG_00_CONFIG                             0x00
#define NRF24_MASK_RX_DR                                0x40
#define NRF24_MASK_TX_DS                                0x20
#define NRF24_MASK_MAX_RT                               0x10
#define NRF24_EN_CRC                                    0x08
#define NRF24_CRCO                                      0x04
#define NRF24_PWR_UP                                    0x02
#define NRF24_PRIM_RX                                   0x01

// #define NRF24_REG_01_EN_AA                              0x01
#define NRF24_ENAA_P5                                   0x20
#define NRF24_ENAA_P4                                   0x10
#define NRF24_ENAA_P3                                   0x08
#define NRF24_ENAA_P2                                   0x04
#define NRF24_ENAA_P1                                   0x02
#define NRF24_ENAA_P0                                   0x01

// #define NRF24_REG_02_EN_RXADDR                          0x02
#define NRF24_ERX_P5                                    0x20
#define NRF24_ERX_P4                                    0x10
#define NRF24_ERX_P3                                    0x08
#define NRF24_ERX_P2                                    0x04
#define NRF24_ERX_P1                                    0x02
#define NRF24_ERX_P0                                    0x01

// #define NRF24_REG_03_SETUP_AW                           0x03
#define NRF24_AW_3_BYTES                                0x01
#define NRF24_AW_4_BYTES                                0x02
#define NRF24_AW_5_BYTES                                0x03

// #define NRF24_REG_04_SETUP_RETR                         0x04
#define NRF24_ARD                                       0xf0
#define NRF24_ARC                                       0x0f

// #define NRF24_REG_05_RF_CH                              0x05
#define NRF24_RF_CH                                     0x7f

// #define NRF24_REG_06_RF_SETUP                           0x06
#define NRF24_CONT_WAVE                                 0x80
#define NRF24_RF_DR_LOW                                 0x20
#define NRF24_PLL_LOCK                                  0x10
#define NRF24_RF_DR_HIGH                                0x08
#define NRF24_PWR                                       0x06
#define NRF24_PWR_m18dBm                                0x00
#define NRF24_PWR_m12dBm                                0x02
#define NRF24_PWR_m6dBm                                 0x04
#define NRF24_PWR_0dBm                                  0x06

// #define NRF24_REG_07_STATUS                             0x07
#define NRF24_RX_DR                                     0x40
#define NRF24_TX_DS                                     0x20
#define NRF24_MAX_RT                                    0x10
#define NRF24_RX_P_NO                                   0x0e
#define NRF24_STATUS_TX_FULL                            0x01


// #define NRF24_REG_08_OBSERVE_TX                         0x08
#define NRF24_PLOS_CNT                                  0xf0
#define NRF24_ARC_CNT                                   0x0f

// #define NRF24_REG_09_RPD                                0x09
#define NRF24_RPD                                       0x01

// #define NRF24_REG_17_FIFO_STATUS                        0x17
#define NRF24_TX_REUSE                                  0x40
#define NRF24_TX_FULL                                   0x20
#define NRF24_TX_EMPTY                                  0x10
#define NRF24_RX_FULL                                   0x02
#define NRF24_RX_EMPTY                                  0x01

// #define NRF24_REG_1C_DYNPD                              0x1c
#define NRF24_DPL_P5                                    0x20
#define NRF24_DPL_P4                                    0x10
#define NRF24_DPL_P3                                    0x08
#define NRF24_DPL_P2                                    0x04
#define NRF24_DPL_P1                                    0x02
#define NRF24_DPL_P0                                    0x01

// #define NRF24_REG_1D_FEATURE                            0x1d
#define NRF24_EN_DPL                                    0x04
#define NRF24_EN_ACK_PAY                                0x02
#define NRF24_EN_DYN_ACK                                0x01
>>>>>>> origin/RF-devlopment
