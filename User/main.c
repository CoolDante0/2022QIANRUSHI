/********************************** (C) COPYRIGHT *******************************
* File Name          : main.c
* Author             : WCH
* Version            : V1.0.0
* Date               : 2020/04/30
* Description        : Main program body.
* Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
* SPDX-License-Identifier: Apache-2.0
*******************************************************************************/

/* ͷ�ļ� */
#include "ch32v30x.h"
#include <rtthread.h>
#include <rthw.h>
#include "drivers/pin.h"
#include <rtdevice.h>

#include "debug.h"

static char xj_stack[2048];                    //�߳�ջ
static struct rt_thread xunji_thread;          //�߳̿��ƿ�

rt_int32_t delay1 = 100;

/* ����������Ŷ��� */
#ifndef Left_moto_pwm
    #define Left_moto_pwm            35  //PWM�źŶ�  /* PB0 */  ͨ��A
#endif                                   //���������getpinֱ�ӻ�ȡ���ű�ţ����ù��Ľӵ����ĸ����ţ�ֱ���þ��У���������������ѯ���Ľӵ����ĸ����ŷ�ֹ���ŽӴ�
#ifndef Right_moto_pwm
    #define Right_moto_pwm           36  //PWM�źŶ�  /* PB1 */  ͨ��B
#endif

#ifndef wheel_left_front
    #define wheel_left_front         55  /* PD8 */
#endif
#ifndef wheel_left_back
    #define wheel_left_back          56  /* PD9 */
#endif

#ifndef wheel_right_front
    #define wheel_right_front        57  /* PD10 */
#endif
#ifndef wheel_right_back
    #define wheel_right_back         58  /* PD11 */
#endif

/*ѭ�����Ŷ���*/

#ifndef right_xunji
    #define right_xunji              46  /* PD3 */  //��ѭ��
#endif

#ifndef left_xunji
    #define left_xunji               84  /* PE15 */  //��ѭ��
#endif

/* С��pwm���� */

void Right_moto_go()       //�ҵ����ǰ��
{
    rt_pin_mode(wheel_right_front,PIN_MODE_OUTPUT);
    rt_pin_write(wheel_right_front,PIN_HIGH);
    rt_pin_mode(wheel_right_back,PIN_MODE_OUTPUT);
    rt_pin_write(wheel_right_back,PIN_LOW);
}
void Left_moto_go()        //��ߵ����ǰ��
{
    rt_pin_mode(wheel_left_front,PIN_MODE_OUTPUT);
    rt_pin_write(wheel_left_front,PIN_LOW);
    rt_pin_mode(wheel_left_back,PIN_MODE_OUTPUT);
    rt_pin_write(wheel_left_back,PIN_HIGH);
}
void Right_moto_back()     //�ұߵ�����ת
{
    rt_pin_mode(wheel_right_front,PIN_MODE_OUTPUT);
    rt_pin_write(wheel_right_front,PIN_LOW);
    rt_pin_mode(wheel_right_back,PIN_MODE_OUTPUT);
    rt_pin_write(wheel_right_back,PIN_HIGH);
}
void Left_moto_back()      //��ߵ�������
{
    rt_pin_mode(wheel_left_front,PIN_MODE_OUTPUT);
    rt_pin_write(wheel_left_front,PIN_HIGH);
    rt_pin_mode(wheel_left_back,PIN_MODE_OUTPUT);
    rt_pin_write(wheel_left_back,PIN_LOW);
}
void Right_moto_Stop()     //�ұߵ��ͣת
{
    rt_pin_mode(wheel_right_front,PIN_MODE_OUTPUT);
    rt_pin_write(wheel_right_front,PIN_LOW);
    rt_pin_mode(wheel_right_back,PIN_MODE_OUTPUT);
    rt_pin_write(wheel_right_back,PIN_LOW);
}
void Left_moto_Stop()      //��ߵ��ͣת
{
    rt_pin_mode(wheel_left_front,PIN_MODE_OUTPUT);
    rt_pin_write(wheel_left_front,PIN_LOW);
    rt_pin_mode(wheel_left_back,PIN_MODE_OUTPUT);
    rt_pin_write(wheel_left_back,PIN_LOW);
}
/* �������� */

unsigned char pwm_val_left  =0;
unsigned char push_val_left =0;

/* ��ʼ�� */

/* С����ʻ */

void run(void)
{
    Left_moto_go();        //������ǰ
    Right_moto_go();       //�ҵ����ǰ
    rt_thread_mdelay(delay1);
}
void  backrun(void)
{
     Left_moto_back();      //��������
     Right_moto_back();     //�ҵ������
}

/* С��ת�� */

void  leftrun(void)
{
     Left_moto_back();   //��������
     Right_moto_go();    //�ҵ����ǰ
}
void  rightrun(void)
{
     Right_moto_back();   //�ҵ�����
     Left_moto_go();      //������ǰ
}
void stop(void)
{
    Left_moto_Stop();
    Right_moto_Stop();
}

/* С��pwm���� */

/* ѭ�� */

static int Car_Traction(void)                    // ???`
{
    if(left_xunji==0&&right_xunji==0)           //���Ҿ�δʶ��
    {
        run();
    }
    else if (left_xunji==0&&right_xunji==1)     //�Һ���ת
    {
        rightrun();
    }
    else if (left_xunji==1&&right_xunji==0)     //�����ת
    {
        leftrun();
    }
    else if (left_xunji==1&&right_xunji==1)     //���Ҷ���ͣ��
    {
        stop();
    }
}

/* ������ */

/* �жϷ����� */

/**
  ******************************************************************
  * @file    main.c
  * @author  xy,Benue
  * @version V1.0
  * @date    2022-1-13
  * @brief   ʹ�ð��� Wake_Up ���� LED1 ����
  ******************************************************************
  * @attention
  * VeriMake ����CH32V307����
  ******************************************************************
  */
// ���� CH32V307 ��ͷ�ļ���C ��׼��Ԫ���delay()����
/********************************************************************
* �� �� ��       : GPIO_INIT
* ��������       : ��ʼ�� GPIO
* ��    ��          : ��
* ��    ��          : ��
********************************************************************/
void GPIO_INIT(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure={0};

//    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD,ENABLE);
//    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
//    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
//    GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
//    GPIO_Init(GPIOD, &GPIO_InitStructure);


    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD,ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_Init(GPIOD, &GPIO_InitStructure);

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD,ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_Init(GPIOD, &GPIO_InitStructure);

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD,ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_Init(GPIOD, &GPIO_InitStructure);

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD,ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_Init(GPIOD, &GPIO_InitStructure);

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE,ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_Init(GPIOE, &GPIO_InitStructure);

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE,ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_Init(GPIOE, &GPIO_InitStructure);

//    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE,ENABLE);
//    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
//    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
//    GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
//    GPIO_Init(GPIOE, &GPIO_InitStructure);
}
/********************************************************************
* �� �� ��       : EXTI_INT_INIT
* ��������       : ��ʼ���ⲿ�ж�
* ��    ��          : ��
* ��    ��          : ��
********************************************************************/
void EXTI15_10_INT_INIT(void)
{
   GPIO_InitTypeDef  GPIO_InitStructure={0};
   EXTI_InitTypeDef EXTI_InitStructure={0};
   NVIC_InitTypeDef NVIC_InitStructure={0};

   RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO|RCC_APB2Periph_GPIOE,ENABLE);

   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
   GPIO_Init(GPIOE, &GPIO_InitStructure);

   GPIO_EXTILineConfig(GPIO_PortSourceGPIOE,GPIO_PinSource0);
   EXTI_InitStructure.EXTI_Line=EXTI_Line15;
   EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
   EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;    //Ϊ�ߵ�ƽ����������
   EXTI_InitStructure.EXTI_LineCmd = ENABLE;
   EXTI_Init(&EXTI_InitStructure);

   NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn ;
   NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; //��ռ���ȼ�
   NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;        //�����ȼ�
   NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
   NVIC_Init(&NVIC_InitStructure);
}
void EXTI3_INT_INIT(void)
{
   GPIO_InitTypeDef  GPIO_InitStructure={0};
   EXTI_InitTypeDef EXTI_InitStructure={0};
   NVIC_InitTypeDef NVIC_InitStructure={0};

   RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO|RCC_APB2Periph_GPIOD,ENABLE);

   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
   GPIO_Init(GPIOD, &GPIO_InitStructure);

   GPIO_EXTILineConfig(GPIO_PortSourceGPIOD,GPIO_PinSource0);
   EXTI_InitStructure.EXTI_Line=EXTI_Line3;
   EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
   EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;    //Ϊ�ߵ�ƽ����������
   EXTI_InitStructure.EXTI_LineCmd = ENABLE;
   EXTI_Init(&EXTI_InitStructure);

   NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn ;
   NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; //��ռ���ȼ�
   NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;        //�����ȼ�
   NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
   NVIC_Init(&NVIC_InitStructure);
}

void EXTI_INT_INIT(void)
{
   GPIO_InitTypeDef  GPIO_InitStructure={0};
   EXTI_InitTypeDef EXTI_InitStructure={0};
   NVIC_InitTypeDef NVIC_InitStructure={0};

   RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO|RCC_APB2Periph_GPIOA,ENABLE);

   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
   GPIO_Init(GPIOA, &GPIO_InitStructure);

   /* GPIOA ----> EXTI_Line0 */
   GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource0);
   EXTI_InitStructure.EXTI_Line=EXTI_Line0;
   EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
   EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising; //����Ϊ�ߵ�ƽ����������
   EXTI_InitStructure.EXTI_LineCmd = ENABLE;
   EXTI_Init(&EXTI_InitStructure);

   NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn ;
   NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; //��ռ���ȼ�
   NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;        //�����ȼ�
   NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
   NVIC_Init(&NVIC_InitStructure);
}

/********************************************************************
* �� �� ��      :  main
* ��������   : ������
* ��    ��         : ��
* ��    ��         : ��
*********************************************************************/
int main(void)
{
    EXTI15_10_INT_INIT(); // ��ʼ���ⲿ�ж�
    GPIO_INIT();     // ��ʼ�� GPIO
    //while(1);        // ��ѭ��

    rt_thread_init(&xunji_thread,
            "xunji",
            Car_Traction,
            RT_NULL,
            &xj_stack[0],
            sizeof(xj_stack),
            10,
            10);
    rt_thread_startup(&xunji_thread);
//    while(1)
//    {
//        run();
//        if(left_xunji==0&&right_xunji==0)           //���Ҿ�δʶ��
//            {
//                run();
//            }
//            else if (left_xunji==0&&right_xunji==1)     //�Һ���ת
//            {
//                rightrun();
//            }
//            else if (left_xunji==1&&right_xunji==0)     //�����ת
//            {
//                leftrun();
//            }
//            else if (left_xunji==1&&right_xunji==1)     //���Ҷ���ͣ��
//            {
//                stop();
//            }
//    }
//    rt_thread_mdelay(4000);
    run();
    rt_thread_mdelay(1000);
    backrun();
    rt_thread_mdelay(1000);
    leftrun();
    rt_thread_mdelay(1000);
    rightrun();
    rt_thread_mdelay(1000);
    stop();
    rt_thread_mdelay(1000);

}

/********************************************************************
* �� �� ��      : EXTI0_IRQHandler
* ��������   : �жϷ������ĺ���
* ��    ��         : ��
* ��    ��         : ��
*********************************************************************/

//������
void EXTI15_10_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
volatile uint16_t LED_Status = 0; // �ж���ʹ�õı����� volatile �ɵ���ȫ�ֱ���
void EXTI15_10_IRQHandler(void)
{
    // ���жϱ�־λΪ��
    EXTI_ClearITPendingBit(EXTI_Line0);
    if(left_xunji==0&&right_xunji==0)           //���Ҿ�δʶ��
        {
            run();
        }
        else if (left_xunji==0&&right_xunji==1)     //�Һ���ת
        {
            rightrun();
        }
        else if (left_xunji==1&&right_xunji==0)     //�����ת
        {
            leftrun();
        }
        else if (left_xunji==1&&right_xunji==1)     //���Ҷ���ͣ��
        {
            stop();
        }
//    LED_Status = !LED_Status ;  // �� LED ״ֵ̬ȡ��
//    GPIO_WriteBit(GPIOE, GPIO_Pin_15, LED_Status); // ���� PE11 (�� LED1) ״̬
}
