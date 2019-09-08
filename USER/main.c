#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "lcd.h"
#include "usart.h"
#include "timer.h"
#include "exti.h"
#include "beep.h"
#include "dac.h"
#include "adc.h"
 
void init(void);

extern int Global_lcd_x;
extern int Global_lcd_y;
extern int Global_lcd_x_dac;
extern int Global_lcd_y_dac;

int arr = 9;
extern int flag;

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
extern int Global_key_num;
extern int counter;

 int main(void)
 {		
	  init();

    while(1)
	{
		
			
	}	 

}

// ��ʼ������
void init(void)
{
	u8 lcd_id[12];   //���LCD ID�ַ���
	delay_init();	//��ʱ������ʼ��
	uart_init(9600); //���ڳ�ʼ��Ϊ9600
	//uart2_init(115200);
	LED_Init();		 //��ʼ����LED���ӵ�Ӳ���ӿ�
	LCD_Init();
	KEY_Init();
	Adc_Init();		  		//ADC��ʼ��
	Dac1_Init();

	POINT_COLOR = RED;
	sprintf((char *)lcd_id, "LCD ID:%04X", lcddev.id); //��LCD ID��ӡ��lcd_id���顣
	

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); // �����ж����ȼ�����2
	TIM3_Int_Init(9, 7199);						// 1Khz�ļ���Ƶ�ʣ�������99Ϊ10ms,��arrΪ9����Ϊ10Khz
	//TIM2_Int_Init(999, 7199);
	LCD_Clear(WHITE);								// ����
}

//void UART_Handle()
//{
//	
//}
