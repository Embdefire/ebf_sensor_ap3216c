/**
  ******************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2019-xx-xx
  * @brief   AP3216C ������ʵ��
  ******************************************************************
  * @attention
  *
  * ʵ��ƽ̨:Ұ��  i.MXRT1052������ 
  * ��̳    :http://www.firebbs.cn
  * �Ա�    :http://firestm32.taobao.com
  *
  ******************************************************************
  */
#include <stdio.h>

#include "fsl_debug_console.h"
#include "board.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "./lpi2c/bsp_lpi2c.h"
#include "./delay/core_delay.h"   
#include "./ap3216c/bsp_ap3216c.h"
#include "./led/bsp_led.h"   
/*******************************************************************
 * Prototypes
 *******************************************************************/

/*******************************************************************
 * Code
 *******************************************************************/

/**
  * @brief  ������
  * @param  ��
  * @retval ��
  */
int main(void)
{
		static uint16_t ALS_RAW;
		static uint16_t PS_RAW;
		static uint16_t IR_RAW;
		
		float ALSValue;
	
	/* ��ʼ���ڴ汣����Ԫ */
		BOARD_ConfigMPU();
		/* ��ʼ������������ */
		BOARD_InitPins();
		/* ��ʼ��������ʱ�� */
		BOARD_BootClockRUN();
		/* ��ʼ�����Դ��� */
		BOARD_InitDebugConsole();
		/* ��ӡϵͳʱ�� */
		PRINTF("\r\n");
		PRINTF("*****��ӭʹ�� Ұ��i.MX RT1052 ������*****\r\n");
		PRINTF("CPU:             %d Hz\r\n", CLOCK_GetFreq(kCLOCK_CpuClk));
		PRINTF("AHB:             %d Hz\r\n", CLOCK_GetFreq(kCLOCK_AhbClk));
		PRINTF("SEMC:            %d Hz\r\n", CLOCK_GetFreq(kCLOCK_SemcClk));
		PRINTF("SYSPLL:          %d Hz\r\n", CLOCK_GetFreq(kCLOCK_SysPllClk));
		PRINTF("SYSPLLPFD0:      %d Hz\r\n", CLOCK_GetFreq(kCLOCK_SysPllPfd0Clk));
		PRINTF("SYSPLLPFD1:      %d Hz\r\n", CLOCK_GetFreq(kCLOCK_SysPllPfd1Clk));
		PRINTF("SYSPLLPFD2:      %d Hz\r\n", CLOCK_GetFreq(kCLOCK_SysPllPfd2Clk));
		PRINTF("SYSPLLPFD3:      %d Hz\r\n", CLOCK_GetFreq(kCLOCK_SysPllPfd3Clk));	
    PRINTF("\r\n ����һ������һ���մ������������� \r\n");
		PRINTF(" оƬ��ʼ����.....\n");
		LED_GPIO_Config();
		/* ��ȷ��ʱ */
		CPU_TS_TmrInit();
		/* ��ʼ�� ���մ����� */
		AP3216C_Init();
		while(1)
		{
				AP3216CReadALS(&ALS_RAW);
				AP3216CReadPS(&PS_RAW);
				AP3216CReadIR(&IR_RAW);
				ALSValue = ALS_RAW * 0.36;// Lux = 16 bit ALS data * Resolution
				PRINTF("�����⣺%.2flux ",ALSValue);
				PRINTF("�ӽ�ֵ��%d ",PS_RAW);
				PRINTF("����⣺%d\r\n",IR_RAW);
				RGB_RED_LED_TOGGLE
				CPU_TS_Tmr_Delay_MS(225);// ��С�������225ms
		}

}


/****************************END OF FILE**********************/
