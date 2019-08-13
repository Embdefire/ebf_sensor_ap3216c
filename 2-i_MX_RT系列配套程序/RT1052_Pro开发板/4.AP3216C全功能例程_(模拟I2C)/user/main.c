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
#include "./i2c/bsp_i2c.h"
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
		/**/
		float ALS = 0.0;
		uint16_t PS = 0;
		uint16_t IR = 0;
		uint8_t IntStatus;
	
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
		ap3216c_init();
		while(1)
		{
				IntStatus = ap3216c_get_IntStatus();    // �ȶ�״̬λ����ADC����λ�����״̬λ��Ĭ�����ã�
				ALS = ap3216c_read_ambient_light();
				PS = ap3216c_read_ps_data();
				IR = ap3216c_read_ir_data();

				PRINTF("\n����ǿ���ǣ�%.2fLux\n����ǿ���ǣ�%d\n", ALS, IR);

				if (PS == 55555)    // IR ̫ǿ PS ������Ч
					PRINTF("IR ̫ǿ PS ������Ч\n");
				else
				{
					PRINTF("�ӽ������ǣ�%d\n", PS & 0x3FF);
				}
				
				if ((PS >> 15) & 1)
					PRINTF("����ӽ�\n");
				else
					PRINTF("����Զ��\n");
				
				if (IntStatus & 0x1)
					PRINTF("ALS �����ж�\n");
				
				if (IntStatus >> 1 & 0x1)
					PRINTF("PS �����ж�\n");
				RGB_RED_LED_TOGGLE
				CPU_TS_Tmr_Delay_MS(225);// ��С�������225ms
		}

}


/****************************END OF FILE**********************/
