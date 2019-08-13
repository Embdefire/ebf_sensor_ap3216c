#ifndef __BSP_LPI2C_H
#define __BSP_LPI2C_H

#include <stdio.h>
#include "fsl_common.h"
#include "fsl_lpi2c.h"
#include "./ap3216c/bsp_ap3216c.h"

//IIC������ַ
#define IIC_ADDRESS AP3216C_ADDRESS>>1

/*
ѡ��LPI2C��ʱ��Դ
0 derive clock from pll3_60m
1 derive clock from osc_clk
*/
/* ѡ�� USB1 PLL/8 (480/8 = 60MHz) ��Ϊlpi2c����ʱ��Դ, */
#define LPI2C_CLOCK_SOURCE_SELECT     (0U)
/* lpi2c���� ʱ��Դ��ʱ�ӷ�Ƶ���� */
#define LPI2C_CLOCK_SOURCE_DIVIDER    (5U)
/* ��ȡ lpi2c ʱ��Ƶ��LPI2C_CLK_ROOT = 60/(5+1) = 10MHz */
#define LPI2C_CLOCK_FREQUENCY ((CLOCK_GetFreq(kCLOCK_Usb1PllClk) / 8) / (LPI2C_CLOCK_SOURCE_DIVIDER + 1U))

#define LPI2C_MASTER_CLOCK_FREQUENCY  LPI2C_CLOCK_FREQUENCY

/* lpi2c������ */
#define I2C_MASTER_BASE   (LPI2C1_BASE)
#define I2C_MASTER        ((LPI2C_Type *)I2C_MASTER_BASE)
/* lpi2c������ */
#define I2C_BAUDRATE      400000U
//#define I2C_BAUDRATE      100000U

/*! @brief I2C���Ŷ��� */
#define SCL_IOMUXC       IOMUXC_GPIO_AD_B1_00_LPI2C1_SCL
#define SDA_IOMUXC       IOMUXC_GPIO_AD_B1_01_LPI2C1_SDA


//�����ӿ�
int32_t EEPROM_I2C_ReadBytes(uint8_t client_addr, uint8_t *buf, int32_t len);

void I2C_EEPROM_Init(void);
uint32_t I2C_WriteBytes(uint8_t ClientAddr,uint8_t* pBuffer,  uint8_t NumByteToWrite);
uint32_t I2C_ReadBytes(uint8_t ClientAddr,uint8_t* pBuffer, uint16_t NumByteToRead);
                                       

extern uint32_t Sensor_write_hardware(uint8_t reg_add,uint8_t reg_dat);
extern uint32_t Sensor_Read_hardware(uint8_t reg_add,unsigned char* Read,uint8_t num);
																					
extern void I2C_Init_Hard(void);


#endif /* __BSP_LPI2C_H */
