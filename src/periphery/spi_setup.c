#include "periphery.h"
#define AD7739_REG_IOPort                  0x01UL
#define AD7739_REG_Revision                0x02UL
#define AD7739_REG_ADCStatus               0x04UL
#define AD7739_REG_ChannelData             0x08UL
#define AD7739_REG_ChannelStatus           0x20UL // 0 channel, 0x21 - 1 channel ...etc.
#define AD7739_REG_ChannelSetup            0x28UL // 0 channel, 0x29 - 1 channel ...etc.
#define AD7739_REG_ChannelConvTime         0x30UL // 0 channel, 0x31 - 1 channel ...etc.
#define AD7739_REG_Mode                    0x38UL

#define AD7739_Comm_Write                  0x0UL
#define AD7739_Comm_Read                   (0x01UL << 0x06UL)

#define AD7739_CHANNEL_0                   0x00UL
#define AD7739_CHANNEL_1                   0x01UL
#define AD7739_CHANNEL_2                   0x02UL
#define AD7739_CHANNEL_3                   0x03UL
#define AD7739_CHANNEL_4                   0x04UL
#define AD7739_CHANNEL_5                   0x05UL
#define AD7739_CHANNEL_6                   0x06UL
#define AD7739_CHANNEL_7                   0x07UL

#define AD7739_IOPort_Sync                 (0x1UL << 0UL)
#define AD7739_IOPort_RedPwr               (0x1UL << 2UL)
#define AD7739_IOPort_RDYFN                (0x1UL << 3UL)
#define AD7739_IOPort_P0DIR                (0x1UL << 4UL)
#define AD7739_IOPort_P1DIR                (0x1UL << 5UL)
#define AD7739_IOPort_P0                   (0x1UL << 6UL)
#define AD7739_IOPort_P1                   (0x1UL << 7UL)

#define AD7739_ChannelSetup_RNG0           (0x1UL << 0UL)
#define AD7739_ChannelSetup_RNG1           (0x1UL << 1UL)
#define AD7739_ChannelSetup_RNG2           (0x1UL << 2UL)
#define AD7739_ChannelSetup_RNG_Pos        0UL
#define AD7739_ChannelSetup_RNG_Msk        (0x07UL << AD7739_ChannelSetup_RNG_Pos)
#define AD7739_ChannelSetup_EN             (0x1UL << 3UL)
#define AD7739_ChannelSetup_StatOPT        (0x1UL << 4UL)
#define AD7739_ChannelSetup_COM0           (0x1UL << 5UL)
#define AD7739_ChannelSetup_COM1           (0x1UL << 6UL)
#define AD7739_ChannelSetup_COM_Pos        5UL
#define AD7739_ChannelSetup_COM_Msk        (0x03UL << AD7739_ChannelSetup_COM_Pos)
#define AD7739_ChannelSetup_BUFOFF         (0x1UL << 7UL)
#define AD7739_ChannelSetup_RNG_DUAL_2_5   (AD7739_ChannelSetup_RNG2)
#define AD7739_ChannelSetup_RNG_SING_2_5   (AD7739_ChannelSetup_RNG2 | AD7739_ChannelSetup_RNG0)
#define AD7739_ChannelSetup_RNG_DUAL_1_25  0x00UL
#define AD7739_ChannelSetup_RNG_SING_1_25  (AD7739_ChannelSetup_RNG0)
#define AD7739_ChannelSetup_RNG_DUAL_0_625 (AD7739_ChannelSetup_RNG1)
#define AD7739_ChannelSetup_RNG_SING_0_625 (AD7739_ChannelSetup_RNG1 | AD7739_ChannelSetup_RNG0)

#define AD7739_ChannelConvTime_CHOP        (0x01UL << 0x07UL)

#define AD7739_Mode_Clamp                  (0x01UL << 0x00UL)
#define AD7739_Mode_DataWidth              (0x01UL << 0x01UL)
#define AD7739_Mode_ContRD                 (0x01UL << 0x02UL)
#define AD7739_Mode_Dump                   (0x01UL << 0x03UL)
#define AD7739_Mode_Clkdis                 (0x01UL << 0x04UL)
#define AD7739_Mode_MD0                    (0x01UL << 0x05UL)
#define AD7739_Mode_MD1                    (0x01UL << 0x06UL)
#define AD7739_Mode_MD2                    (0x01UL << 0x07UL)
#define AD7739_Mode_MD_Idle                (0x00UL)
#define AD7739_Mode_MD_ContConv            (AD7739_Mode_MD0)
#define AD7739_Mode_MD_SingConv            (AD7739_Mode_MD1)
#define AD7739_Mode_MD_PD                  (AD7739_Mode_MD1 | AD7739_Mode_MD0)
#define AD7739_Mode_MD_ZSCall              (AD7739_Mode_MD2)
#define AD7739_Mode_MD_FSCall              (AD7739_Mode_MD2 | AD7739_Mode_MD0)
#define AD7739_Mode_MD_ChZSCall            (AD7739_Mode_MD2 | AD7739_Mode_MD1)
#define AD7739_Mode_MD_ChFSCall            (AD7739_Mode_MD2 | AD7739_Mode_MD0 | AD7739_Mode_MD0)

#define ADC_DATA_SIZE                      (8)

void SPI1SetUp(void)
{
    SPI1->CR1 = SPI_CR1_SSM | SPI_CR1_SSI | SPI_CR1_DFF | SPI_CR1_MSTR |
                // SPI_CR1_RXONLY |
                SPI_CR1_CPOL | SPI_CR1_CPHA | SPI_CR1_BR_2 | SPI_CR1_BR_0 | SPI_CR1_BR_1;
}