#include "exti.h"
#include "led.h"
#include "key.h"
#include "delay.h"
#include "usart.h"
#include "beep.h"
#include "lcd.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEKս��STM32������
//�ⲿ�ж� ��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/9/3
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////   
//�ⲿ�ж�0�������

extern u16 Colors;
extern u16 Hun, Dcd, Unt, NumL;

void EXTIX_Init(void)
{
 
 	EXTI_InitTypeDef EXTI_InitStructure;
 	NVIC_InitTypeDef NVIC_InitStructure;

    KEY_Init();	 //	�����˿ڳ�ʼ��

  	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);	//ʹ�ܸ��ù���ʱ��

    //GPIOE.2 �ж����Լ��жϳ�ʼ������   �½��ش���
  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOE,GPIO_PinSource2);

  	EXTI_InitStructure.EXTI_Line=EXTI_Line2;	//KEY2
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  	EXTI_Init(&EXTI_InitStructure);	 	//����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���

   //GPIOE.3	  �ж����Լ��жϳ�ʼ������ �½��ش��� //KEY1
  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOE,GPIO_PinSource3);
  	EXTI_InitStructure.EXTI_Line=EXTI_Line3;
  	EXTI_Init(&EXTI_InitStructure);	  	//����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���

   //GPIOE.4	  �ж����Լ��жϳ�ʼ������  �½��ش���	//KEY0
  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOE,GPIO_PinSource4);
  	EXTI_InitStructure.EXTI_Line=EXTI_Line4;
  	EXTI_Init(&EXTI_InitStructure);	  	//����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���


   //GPIOA.0	  �ж����Լ��жϳ�ʼ������ �����ش��� PA0  WK_UP
 	 GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource0); 

  	EXTI_InitStructure.EXTI_Line=EXTI_Line0;
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
  	EXTI_Init(&EXTI_InitStructure);		//����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���


  	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;			//ʹ�ܰ���WK_UP���ڵ��ⲿ�ж�ͨ��
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//��ռ���ȼ�2�� 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03;					//�����ȼ�3
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//ʹ���ⲿ�ж�ͨ��
  	NVIC_Init(&NVIC_InitStructure); 

    NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;			//ʹ�ܰ���KEY2���ڵ��ⲿ�ж�ͨ��
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//��ռ���ȼ�2�� 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;					//�����ȼ�2
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//ʹ���ⲿ�ж�ͨ��
  	NVIC_Init(&NVIC_InitStructure);


  	NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn;			//ʹ�ܰ���KEY1���ڵ��ⲿ�ж�ͨ��
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//��ռ���ȼ�2 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;					//�����ȼ�1 
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//ʹ���ⲿ�ж�ͨ��
  	NVIC_Init(&NVIC_InitStructure);  	  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���

	  NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;			//ʹ�ܰ���KEY0���ڵ��ⲿ�ж�ͨ��
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//��ռ���ȼ�2 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;					//�����ȼ�0 
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//ʹ���ⲿ�ж�ͨ��
  	NVIC_Init(&NVIC_InitStructure);  	  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���
 
}

//�ⲿ�ж�0������� 
u16 flagCount = 0;	//�����ۼ�
u8 flagL = 0;				//�»���λ�ã�flagL=0Ϊ��λ��flagL=1Ϊʮλ��flagL=2Ϊ��λ
u8 Start = 1;				//Start = 1Ϊ������Start = 0Ϊֹͣ��

//void EXTI0_IRQHandler(void)
//{
//	if(Start==0){
//		//��ͣ��ſɸ������ֺ��±꣬ȥ���˹��ܽ�ifɾȥ
//		
//		delay_ms(10);//����
//		if(WK_UP==1)	 	 //WK_UP����
//		{
//			//�̰�һ��
//			delay_ms(10);
//			if(flagL==0){
//				Hun++;
//				if(Hun>5){Hun = 0;}
//				if(Hun==0 && Dcd==0 && Unt==0){
//					Hun=0;Dcd=9;Unt=9;
//				}
//			}
//			else if(flagL==1){
//				Dcd++;
//				if(Dcd>9){Dcd = 0;}
//				if(Hun==0 && Dcd==0 && Unt==0){
//					Hun=0;Dcd=9;Unt=9;
//				}
//			}
//			else if(flagL==2){
//				Unt++;
//				if(Unt>9){Unt = 0;}
//				if(Hun==0 && Dcd==0 && Unt==0){
//					Hun=0;Dcd=9;Unt=9;
//				}
//			}
//			
//			//����״̬
//			for(;WK_UP==1;flagCount++){
//				delay_ms(10);
//				if(flagCount>50){
//					delay_ms(100);
//					if(flagL==0){
//						Hun++;
//						if(Hun>5){Hun = 0;}
//						if(Hun==0 && Dcd==0 && Unt==0){
//							Hun=0;Dcd=9;Unt=9;
//						}
//						LCD_ShowNum(100,300,Hun,1,24);
//						LCD_ShowNum(150,300,Dcd,1,24);
//						LCD_ShowNum(200,300,Unt,1,24);
//					}
//					else if(flagL==1){
//						Dcd++;
//						if(Dcd>9){Dcd = 0;}
//						if(Hun==0 && Dcd==0 && Unt==0){
//							Hun=0;Dcd=9;Unt=9;
//						}
//						LCD_ShowNum(100,300,Hun,1,24);
//						LCD_ShowNum(150,300,Dcd,1,24);
//						LCD_ShowNum(200,300,Unt,1,24);
//					}
//					else if(flagL==2){
//						Unt++;
//						if(Unt>9){Unt = 0;}
//						if(Hun==0 && Dcd==0 && Unt==0){
//							Hun=0;Dcd=9;Unt=9;
//						}
//						LCD_ShowNum(100,300,Hun,1,24);
//						LCD_ShowNum(150,300,Dcd,1,24);
//						LCD_ShowNum(200,300,Unt,1,24);
//					}
//					//
//				}
//			}
//		}
//		if(WK_UP==0){
//			flagCount = 0;
//		}
//		
//	}//
//	EXTI_ClearITPendingBit(EXTI_Line0); //���LINE0�ϵ��жϱ�־λ  
//}
// 
////�ⲿ�ж�2�������

//void EXTI2_IRQHandler(void)
//{
//	delay_ms(10);//����
//	if(KEY2==0)	  //����KEY2
//	{
//	  //�̰�һ��
//		delay_ms(100);
//		Start = !Start;
//		//
//		for(;KEY2==0;flagCount++){
//			delay_ms(10);
//			if(flagCount>50){

//				//
//			}
//		}
//	}
//	if(KEY2==1){
//		flagCount = 0;
//	}
//	
//	EXTI_ClearITPendingBit(EXTI_Line2);  //���LINE2�ϵ��жϱ�־λ  
//}
////�ⲿ�ж�3�������
//void EXTI3_IRQHandler(void)
//{
//	if(Start==0){
//		//��ͣ��ſɸ������ֺ��±�
//		
//		delay_ms(10);//����
//		if(KEY1==0)	 //����KEY1
//		{
//			//�̰�һ��
//			if(NumL == 100){
//				POINT_COLOR = WHITE;
//				LCD_DrawLine(NumL,340,NumL+10,340);
//				NumL = 150;
//				POINT_COLOR = RED;
//				LCD_DrawLine(NumL,340,NumL+10,340);
//				LED1=!LED1;
//				flagL = 1;
//			}
//			else if(NumL == 150){
//				POINT_COLOR = WHITE;
//				LCD_DrawLine(NumL,340,NumL+10,340);
//				NumL = 200;
//				POINT_COLOR = RED;
//				LCD_DrawLine(NumL,340,NumL+10,340);
//				flagL = 2;
//			}
//			else if(NumL == 200){
//				POINT_COLOR = WHITE;
//				LCD_DrawLine(NumL,340,NumL+10,340);
//				NumL = 100;
//				POINT_COLOR = RED;
//				LCD_DrawLine(NumL,340,NumL+10,340);
//				flagL = 0;
//			}
//			delay_ms(10);
//			//
//			for(;KEY1==0;flagCount++){
//				delay_ms(10);
//				if(flagCount>50){
//					delay_ms(100);
//					
//					//
//				}
//			}
//		}
//		if(KEY1==1){
//			flagCount = 0;
//		}
//		
//	}
//	EXTI_ClearITPendingBit(EXTI_Line3);  //���LINE3�ϵ��жϱ�־λ  
//}

//u8 colorFlag = 0;
//void EXTI4_IRQHandler(void)
//{
//	delay_ms(10);//����
//	if(KEY0==0)	 //����KEY0
//	{
//	  //�̰�һ��
//		//���Ļ������ɫ
//		delay_ms(10);
//		colorFlag++;
//		if(colorFlag>3)colorFlag = 0;
//		switch(colorFlag)
//		{
//			case 0:Colors=RED;break;
//			case 1:Colors=BLACK;break;
//			case 2:Colors=BLUE;break;
//			case 3:Colors=GREEN;break;
//		}
//	  //����״̬
//		for(;KEY0==0;flagCount++){
//			delay_ms(10);
//			if(flagCount>50){
//			  delay_ms(100);
//				colorFlag++;
//				if(colorFlag>3)colorFlag = 0;
//				switch(colorFlag)
//				{
//					case 0:Colors=RED;break;
//					case 1:Colors=BLACK;break;
//					case 2:Colors=BLUE;break;
//					case 3:Colors=GREEN;break;
//				}
//				//�������Ļ������ɫ
//			}
//		}
//	}
//	if(KEY0==1){
//		flagCount = 0;
//	}
//	
//	EXTI_ClearITPendingBit(EXTI_Line4);  //���LINE4�ϵ��жϱ�־λ  
//}
 
