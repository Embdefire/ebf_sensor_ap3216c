/**
  ******************************************************************************
  * @file    bsp_lpi2c.c
  * @author  fire
  * @version V1.0
  * @date    2019-xx-xx
  * @brief   iic驱动
  ******************************************************************************
  * @attention
  *
  * 实验平台:野火  i.MXRT1052开发板 
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :https://fire-stm32.taobao.com
  *
  ******************************************************************************
  */ 
#include "fsl_debug_console.h"  

#include "fsl_iomuxc.h"
#include "fsl_gpio.h"
#include "fsl_lpi2c.h"

#include "./delay/core_delay.h"   
#include "./lpi2c/bsp_lpi2c.h"
#include "./pad_config.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/* I2C的SCL和SDA引脚使用同样的PAD配置 */
#define I2C_PAD_CONFIG_DATA         (SRE_0_SLOW_SLEW_RATE| \
                                        DSE_6_R0_6| \
                                        SPEED_1_MEDIUM_100MHz| \
                                        ODE_1_OPEN_DRAIN_ENABLED| \
                                        PKE_1_PULL_KEEPER_ENABLED| \
                                        PUE_0_KEEPER_SELECTED| \
                                        PUS_3_22K_OHM_PULL_UP| \
                                        HYS_0_HYSTERESIS_DISABLED)   
    /* 配置说明 : */
    /* 转换速率: 转换速率慢
        驱动强度: R0/6 
        带宽配置 : medium(100MHz)
        开漏配置: 使能 
        拉/保持器配置: 使能
        拉/保持器选择: 保持器
        上拉/下拉选择: 22K欧姆上拉(选择了保持器此配置无效)
        滞回器配置: 禁止 */ 
              
/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static void I2C_IOMUXC_MUX_Config(void);
static void I2C_IOMUXC_PAD_Config(void);
static void I2C_ModeInit(void);

/*******************************************************************************
 * Variables
 ******************************************************************************/


/*******************************************************************************
 * Code
 ******************************************************************************/


/**
* @brief  初始化硬件IIC相关IOMUXC的MUX复用配置
* @param  无
* @retval 无
*/
static void I2C_IOMUXC_MUX_Config(void)
{
  /* SCL和SDA引脚，需要使能SION以接收数据 */
  IOMUXC_SetPinMux(SCL_IOMUXC, 1U);                                   
  IOMUXC_SetPinMux(SDA_IOMUXC, 1U);    
}

/**
* @brief  初始化硬件IIC相关IOMUXC的PAD属性配置
* @param  无
* @retval 无
*/
static void I2C_IOMUXC_PAD_Config(void)
{
  /* SCL和SDA引脚 */
  IOMUXC_SetPinConfig(SCL_IOMUXC, I2C_PAD_CONFIG_DATA);                               
  IOMUXC_SetPinConfig(SDA_IOMUXC, I2C_PAD_CONFIG_DATA);   
}


/**
* @brief  初始化硬件IIC使用的I2C外设
* @param  无
* @retval 无
*/  
static void I2C_ModeInit(void)
{
  lpi2c_master_config_t masterConfig; 
  
  /* 配置时钟 LPI2C */
  CLOCK_SetMux(kCLOCK_Lpi2cMux, LPI2C_CLOCK_SOURCE_SELECT);
  CLOCK_SetDiv(kCLOCK_Lpi2cDiv, LPI2C_CLOCK_SOURCE_DIVIDER);
  
  /*给masterConfig赋值为以下默认配置*/
  /*
   * masterConfig.debugEnable = false;
   * masterConfig.ignoreAck = false;
   * masterConfig.pinConfig = kLPI2C_2PinOpenDrain;
   * masterConfig.baudRate_Hz = 100000U;
   * masterConfig.busIdleTimeout_ns = 0;
   * masterConfig.pinLowTimeout_ns = 0;
   * masterConfig.sdaGlitchFilterWidth_ns = 0;
   * masterConfig.sclGlitchFilterWidth_ns = 0;
   */
  LPI2C_MasterGetDefaultConfig(&masterConfig);

  /* 把默认波特率改为I2C_BAUDRATE */
  masterConfig.baudRate_Hz = I2C_BAUDRATE;

  /*  使用以上配置初始化 LPI2C 外设 */
  LPI2C_MasterInit(I2C_MASTER, &masterConfig, LPI2C_MASTER_CLOCK_FREQUENCY);

}


/**
  * @brief   读取数据
  * @param   reg_add:寄存器地址
	* @param reg_data:要写入的数据
  * @retval  
  */
uint32_t Sensor_Read_hardware(uint8_t reg_add,unsigned char* Read,uint8_t num)
{ 
	lpi2c_master_transfer_t masterXfer = {0};
  status_t reVal = kStatus_Fail;

  /* subAddress = ReadAddr, data = pBuffer 自从机处接收
    起始信号start + 设备地址slaveaddress(w 写方向) + 子地址subAddress + 
    重复起始信号repeated start + 设备地址slaveaddress(r 读方向) + 
    接收缓冲数据rx data buffer + 停止信号stop */
  masterXfer.slaveAddress = IIC_ADDRESS;
  masterXfer.direction = kLPI2C_Read;
  masterXfer.subaddress = (uint32_t)reg_add;//要读数据的寄存器地址
  masterXfer.subaddressSize = 1;
  masterXfer.data = Read;						//数据缓冲区
  masterXfer.dataSize = num;			//读取数据的个数
  masterXfer.flags = kLPI2C_TransferDefaultFlag;
  reVal = LPI2C_MasterTransferBlocking(I2C_MASTER, &masterXfer);//I2C_MASTER
  if (reVal != kStatus_Success)
  {
      return 1;
  }
  
  return 0;
}

/**
  * @brief   写数据到传感器的寄存器
  * @param   reg_add:寄存器地址
	* @param reg_data:要写入的数据
  * @retval  
  */
uint32_t Sensor_write_hardware(uint8_t reg_add,uint8_t reg_dat)
{ 
  lpi2c_master_transfer_t masterXfer = {0};
  status_t reVal = kStatus_Fail;
   
  masterXfer.slaveAddress = IIC_ADDRESS;//IIC从机地址 MPU6050_ADDRESS
  masterXfer.direction = kLPI2C_Write;			//写入数据
  masterXfer.subaddress = reg_add;				//读取寄存器地址
  masterXfer.subaddressSize = 1;						//长度默认一个字节
  masterXfer.data = &reg_dat;								//要写入的数据
  masterXfer.dataSize = 1;									//长度默认一个字节
  masterXfer.flags = kLPI2C_TransferDefaultFlag;
  
  reVal = LPI2C_MasterTransferBlocking(I2C_MASTER, &masterXfer);
  
  if (reVal != kStatus_Success)
  {
      return 1;
  }
  
  return 0;

}

/**
* @brief  硬件I2C初始化
* @param  无
* @retval 无
*/
void I2C_Init_Hard(void)
{

  /* 初始化各引脚IOMUXC相关 */
  I2C_IOMUXC_MUX_Config();
  I2C_IOMUXC_PAD_Config();
  /* 初始化I2C外设工作模式 */
  I2C_ModeInit(); 
}

/*********************************************END OF FILE**********************/
