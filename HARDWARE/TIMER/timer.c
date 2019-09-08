#include "timer.h"
#include "led.h"
#include "lcd.h"
#include "delay.h"
#include "math.h"
#include "key.h"
#include "dac.h"
#include "adc.h"

//ȫ�ֱ���
u16 Global_delay_time = 5000; //��ʱʱ��
u16 Global_key_num = 0;		  //����
u16 Global_press_time = 0;	//����ʱ��
u16 last_press = 0;			  //�������Ʊ���
int Global_lcd_x = 0;		  //LCDX�����
int Global_lcd_y = 0;		  //LCDY�����
u16 time = 0;				  // ʱ���¼��
u16 count = 0;				  // ��ͼ���Ʊ���
int xdir = 1;				  //x����������
int ydir = 1;				  //y����������
int xdir_dac = 1;
int ydir_dac = 1;
double temp = 0;			  // sin���� 	 
int counter = 0;
int Global_lcd_x_dac = 0;		  
int Global_lcd_y_dac = 0;		  

// ���ǲ�
void triangle_wave(void);
// ����
void square_wave(void);
// ���Ҳ�
void sin_wave(void);	 

// ���ǲ�
void triangle_wave_dac(void);
// ����
void square_wave_dac(void);
// ���Ҳ�
void sin_wave_dac(void);

//ͨ�ö�ʱ��3�жϳ�ʼ��
//����ʱ��ѡ��ΪAPB1��2������APB1Ϊ36M
//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��
//����ʹ�õ��Ƕ�ʱ��3!




void TIM3_Int_Init(u16 arr,u16 psc)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //ʱ��ʹ��
	
	//��ʱ��TIM3��ʼ��
	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //����ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
 
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE ); //ʹ��ָ����TIM3�ж�,��������ж�

	//�ж����ȼ�NVIC����
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //�����ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //��ʼ��NVIC�Ĵ���

	TIM_Cmd(TIM3, ENABLE);  //ʹ��TIMx					 
}


void TIM2_Int_Init(u16 arr,u16 psc)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); //ʱ��ʹ��
	
	//��ʱ��TIM2��ʼ��
	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //����ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
 
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE ); //ʹ��ָ����TIM3�ж�,��������ж�

	//�ж����ȼ�NVIC����
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;  //TIM2�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //�����ȼ�2��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //��ʼ��NVIC�Ĵ���

	TIM_Cmd(TIM2, ENABLE);  //ʹ��TIMx					 
}

//��ʱ��3�жϷ������
int flag = 1;
int count3 = 0;
void TIM3_IRQHandler(void)   //TIM3�ж�
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) //���ָ����TIM�жϷ������:TIM �ж�Դ
	{
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update); //���TIMx���жϴ�����λ:TIM �ж�Դ
		
		// ����ֵ
		Global_key_num = KEY_Scan(1);
		// ʱ���¼��
		//time++;

		if (Global_key_num)
		{
			switch(Global_key_num)
			{
				case WKUP_PRES:
					//Adc_Init();		  		//ADC��ʼ��
	        //Dac1_Init();
					counter = 1;
				  Global_lcd_x = 0;		  
					Global_lcd_y = 0;
					Global_lcd_x_dac = 0;		  
					Global_lcd_y_dac = 0;
				  LCD_Clear(WHITE);
				  break;
				case KEY0_PRES:
					//Adc_Init();		  		//ADC��ʼ��
	        //Dac1_Init();
					counter = 2;
				  Global_lcd_x = 0;		  
					Global_lcd_y = 0;
					Global_lcd_x_dac = 0;		  
					Global_lcd_y_dac = 0;
				  //LCD_Init();
				  LCD_Clear(WHITE);
				  //Dac1_Init();
				  break;
				case KEY1_PRES:
					//Adc_Init();		  		//ADC��ʼ��
					//Dac1_Init();
					counter = 3;
				  Global_lcd_x = 0;		  
					Global_lcd_y = 0;
					Global_lcd_x_dac = 0;		  
					Global_lcd_y_dac = 0;
				  //LCD_Init();
				  LCD_Clear(WHITE);
				  break;
				default:
					counter = 1;
				  break;
				
			}
		}
		
		if (counter == 1)
		{
			//LCD_ShowxNum(124,150,(arr+1)/10,4,16,0);
			sin_wave();
			sin_wave_dac();
			LCD_DrawPoint(Global_lcd_x, Global_lcd_y + 60);
		  LCD_DrawPoint(Global_lcd_x_dac, Global_lcd_y_dac + 360);
		}
		else if (counter == 2)
		{
			//LCD_ShowxNum(124,150,(arr+1)/10,4,16,0);
			square_wave();
			square_wave_dac();
			LCD_DrawPoint(Global_lcd_x, Global_lcd_y + 60);
		  LCD_DrawPoint(Global_lcd_x_dac, Global_lcd_y_dac + 360);
		}
		else if (counter == 3)
		{
			//LCD_ShowxNum(124,150,(arr+1)/10,4,16,0);
			triangle_wave();
			triangle_wave_dac();
			LCD_DrawPoint(Global_lcd_x, Global_lcd_y + 60);
		  LCD_DrawPoint(Global_lcd_x_dac, Global_lcd_y_dac + 360);
		}
		//LCD_DrawPoint(Global_lcd_x, Global_lcd_y + 60);
//		if(count3 <= 2)
//		{
//			LCD_DrawPoint(Global_lcd_x, Global_lcd_y + 60);
//		  LCD_DrawPoint(Global_lcd_x_dac, Global_lcd_y_dac + 360);
//			count3 = count3+1;
//		}
//		count3 = 0;
	}
}




//// ���ǲ�
void triangle_wave(void)
{

		// X�Ჽ����
		if (Global_lcd_x == 0)
			xdir = 1;
		// ���Ʒ�Χ
		if (Global_lcd_x >= 240)
		{
			Global_lcd_x = 0;
			Global_lcd_y = 0;
		}
		// Y���ǲ�����
		if (Global_lcd_y == 0)
			ydir = 1;
		if (Global_lcd_y >= 50)
			ydir = -1;

		Global_lcd_x = Global_lcd_x + xdir; //
		Global_lcd_y = Global_lcd_y + ydir; //
		//flag = 1;

}

int pwm = 50;
// ����
void square_wave(void)
{

		count++;
		// ��һ������
		if (count <= 50)
		{
			xdir = 1;
			ydir = 0;
		}
		// ��һ������
		if (count > pwm && count <= 100)
		{
			xdir = 0;
			ydir = 1;
		}
		// �ڶ�������
		if (count > 100 && count <= 150)
		{
			xdir = 1;
			ydir = 0;
		}
		// ��2������
		if (count > 150 && count <= 200)
		{
			xdir = 0;
			ydir = -1;
		}
		// ��3������
		if (count > 200 && count <= 250)
		{
			xdir = 1;
			ydir = 0;
		}
		// ��3������
		if (count > 250 && count <= 300)
		{
			xdir = 0;
			ydir = 1;
		}
		// ��4������
		if (count > 300 && count <= 350)
		{
			xdir = 1;
			ydir = 0;
		}
		// ��4������
		if (count > 350 && count <= 400)
		{
			xdir = 0;
			ydir = -1;
		}
		// ��5������
		if (count > 400 && count <= 450)
		{
			xdir = 1;
			ydir = 0;
		}
		// ����Χ
		if (Global_lcd_x >= 240)
		{
			Global_lcd_x = 0;
			Global_lcd_y = 0;
			count = 0;
		}

		Global_lcd_x = Global_lcd_x + xdir; //
		Global_lcd_y = Global_lcd_y + ydir; //
		
}

// ���Ҳ�

void sin_wave(void)
{


		if (Global_lcd_x == 0)
			xdir = 1;
		if (Global_lcd_x >= 240)
		{
			Global_lcd_x = 0;
			Global_lcd_y = 0;
		}

		Global_lcd_x = Global_lcd_x + xdir; //
		temp = Global_lcd_x;
		if ((int)temp % 2 == 0)
			Global_lcd_y = 30 * sin(3 * temp); //
		  // Global_lcd_y = sin(temp);
		else
			Global_lcd_y = -30 * sin(3 * temp);
		  // Global_lcd_y = -sin(temp);
		
		Global_lcd_y = (int)Global_lcd_y;


}

int temp_t=0;
int count2 = 0;


// ���ǲ�
void triangle_wave_dac(void)
{

		// X�Ჽ����
		if (Global_lcd_x == 0)
			xdir = 1;
		// ���Ʒ�Χ
		if (Global_lcd_x >= 240)
		{
			Global_lcd_x = 0;
			Global_lcd_y = 0;
		}
		// Y���ǲ�����
		if (Global_lcd_y == 0)
			ydir = 1;
		if (Global_lcd_y >= 50)
			ydir = -1;

		Global_lcd_x = Global_lcd_x + xdir; //
		Global_lcd_y = Global_lcd_y + ydir; //
		
		
		DAC_SetChannel1Data(DAC_Align_12b_R, Global_lcd_y);
		//Dac1_Set_Vol(Global_lcd_y);

		Global_lcd_y_dac=DAC_GetDataOutputValue(DAC_Channel_1);
		Global_lcd_y_dac=Get_Adc_Average(ADC_Channel_1,10);
			

		
		//Global_lcd_x_dac=Global_lcd_x_dac/10;
		
		Global_lcd_x_dac = Global_lcd_x;

}

// ����
void square_wave_dac(void)
{
	/////////////////////////////////////
	
	count++;

	// ��һ������
	if (count <= 50)
	{
		xdir = 1;
		ydir = 0;
	}
	// ��һ������
	if (count > 50 && count <= 100)
	{
		xdir = 0;
		ydir = 1;
	}
	// �ڶ�������
	if (count > 100 && count <= 150)
	{
		xdir = 1;
		ydir = 0;
	}
	// ��2������
	if (count > 150 && count <= 200)
	{
		xdir = 0;
		ydir = -1;
	}
	// ��3������
	if (count > 200 && count <= 250)
	{
		xdir = 1;
		ydir = 0;
	}
	// ��3������
	if (count > 250 && count <= 300)
	{
		xdir = 0;
		ydir = 1;
	}
	// ��4������
	if (count > 300 && count <= 350)
	{
		xdir = 1;
		ydir = 0;
	}
	// ��4������
	if (count > 350 && count <= 400)
	{
		xdir = 0;
		ydir = -1;
	}
	// ��5������
	if (count > 400 && count <= 450)
	{
		xdir = 1;
		ydir = 0;
	}
	// ����Χ
	if (Global_lcd_x >= 240)
	{
		Global_lcd_x = 0;
		Global_lcd_y = 0;
		count = 0;
	}

	Global_lcd_x = Global_lcd_x + xdir; 
	Global_lcd_y = Global_lcd_y + ydir; 
	
	DAC_SetChannel1Data(DAC_Align_12b_R, Global_lcd_y*100);
	
	Global_lcd_y_dac=DAC_GetDataOutputValue(DAC_Channel_1);
	Global_lcd_y_dac=Get_Adc_Average(ADC_Channel_1,10);
	
	Global_lcd_x_dac=Global_lcd_x;
		
		


}

// ���Ҳ�

void sin_wave_dac(void)
{

		if (Global_lcd_x == 0)
		{
			xdir = 1;
		}
			
		if (Global_lcd_x >= 240)
		{
			Global_lcd_x = 0;
			Global_lcd_y = 0;
		}

		Global_lcd_x = Global_lcd_x + xdir; //
		temp = Global_lcd_x;
		if ((int)temp % 2 == 0)
			Global_lcd_y = 30 * sin(3 * temp); //
		else
			Global_lcd_y = -30 * sin(3 * temp);
		Global_lcd_y = (int)Global_lcd_y;
		
		DAC_SetChannel1Data(DAC_Align_12b_R, Global_lcd_y);

		Global_lcd_y_dac=DAC_GetDataOutputValue(DAC_Channel_1);
		Global_lcd_y_dac=Get_Adc_Average(ADC_Channel_1,10);
		
		Global_lcd_x_dac = Global_lcd_x;



}









