 #include "adc.h"
 #include "delay.h"
 
 /*********************************************
�������գ�
��Ϊ�����ź�Ϊ40Hz��ʵ����ʵ��39.1Hz�������ź�����25ms��һ���ź����ڲ���256������ô��ʱ��Ӧ������Ϊ25ms/256=97.65625Լ����98us���һ��
ADCʹ��DMA
*********************************************/
 
 
u16 Wave_Input_Temp_12_bits_Array[256];
u16 Wave_Input_12_bits_Array[256];

extern u16 SineWave_12_bits_Array[256];
extern u16 SawtoothWave_12_bits_Array[256];
extern u16 SquareWave_12_bits_Array[256];
extern u16 Altered_SquareWave_12_bits_Array[256];

extern enum Show_WaveTypeDef	{Show_SineWave,Show_SawtoothWave,Show_SquareWave,Show_Altered_SquareWave}	show_wave;
 
 
////��ʼ��ADC
////�������ǽ��Թ���ͨ��Ϊ��
////����Ĭ�Ͻ�����ͨ��0~3																	   
//void  Adc_Init(void)
//{ 	
//	ADC_InitTypeDef ADC_InitStructure; 
//	GPIO_InitTypeDef GPIO_InitStructure;

//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA |RCC_APB2Periph_ADC1	, ENABLE );	  //ʹ��ADC1ͨ��ʱ��
// 

//	RCC_ADCCLKConfig(RCC_PCLK2_Div6);   //����ADC��Ƶ����6 72M/6=12,ADC���ʱ�䲻�ܳ���14M

//	//PA1 ��Ϊģ��ͨ����������                         
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		//ģ����������
//	GPIO_Init(GPIOA, &GPIO_InitStructure);	

//	ADC_DeInit(ADC1);  //��λADC1,������ ADC1 ��ȫ���Ĵ�������Ϊȱʡֵ

//	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;	//ADC����ģʽ:ADC1��ADC2�����ڶ���ģʽ
//	ADC_InitStructure.ADC_ScanConvMode = DISABLE;	//ģ��ת�������ڵ�ͨ��ģʽ
//	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;	//ģ��ת�������ڵ���ת��ģʽ
//	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//ת��������������ⲿ��������
//	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	//ADC�����Ҷ���
//	ADC_InitStructure.ADC_NbrOfChannel = 1;	//˳����й���ת����ADCͨ������Ŀ
//	ADC_Init(ADC1, &ADC_InitStructure);	//����ADC_InitStruct��ָ���Ĳ�����ʼ������ADCx�ļĴ���   

//  
//	ADC_Cmd(ADC1, ENABLE);	//ʹ��ָ����ADC1
//	
//	ADC_ResetCalibration(ADC1);	//ʹ�ܸ�λУ׼  
//	 
//	while(ADC_GetResetCalibrationStatus(ADC1));	//�ȴ���λУ׼����
//	
//	ADC_StartCalibration(ADC1);	 //����ADУ׼
// 
//	while(ADC_GetCalibrationStatus(ADC1));	 //�ȴ�У׼����
// 
////	ADC_SoftwareStartConvCmd(ADC1, ENABLE);		//ʹ��ָ����ADC1�����ת����������

//}				  
////���ADCֵ
////ch:ͨ��ֵ 0~3
//u16 Get_Adc(u8 ch)   
//{
//  	//����ָ��ADC�Ĺ�����ͨ����һ�����У�����ʱ��
//	ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_239Cycles5 );	//ADC1,ADCͨ��,����ʱ��Ϊ239.5����	  			    
//  
//	ADC_SoftwareStartConvCmd(ADC1, ENABLE);		//ʹ��ָ����ADC1�����ת����������	
//	 
//	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));//�ȴ�ת������

//	return ADC_GetConversionValue(ADC1);	//�������һ��ADC1�������ת�����
//}

//u16 Get_Adc_Average(u8 ch,u8 times)
//{
//	u32 temp_val=0;
//	u8 t;
//	for(t=0;t<times;t++)
//	{
//		temp_val+=Get_Adc(ch);
//		delay_ms(5);
//	}
//	return temp_val/times;
//} 	 



void SineWave_Input_TIM3_ADC1_DMA1_Init(void)//ADC1_Mode_Config(uint32_t ADC_ConvertedVal)
{
        
	GPIO_InitTypeDef			GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef		TIM_TimeBaseStructure;
	ADC_InitTypeDef				ADC_InitStructure;
	DMA_InitTypeDef				DMA_InitStructure;
	NVIC_InitTypeDef			NVIC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE );	  //ʹ�ܣ�ADC��ͨ��1��PORTAͨ��ʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);	  //ʹ��TIM2��ʱ�����ڲ��������¼��Ӷ���ʱ����ADC
   	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE );	  //ʹ��ADC1ͨ��ʱ��
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);		  //ʹ��DMA1ʱ��
	
	
	/***********GPIO***************/	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	
	/***********TIM***************/
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_Period = 10-1;
	TIM_TimeBaseStructure.TIM_Prescaler = 72-1;//��ʱ������100us����һ����
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseStructure);

	TIM_SelectOutputTrigger(TIM3,TIM_TRGOSource_Update); //TRGO����ADC1
	TIM_SelectMasterSlaveMode(TIM3, TIM_MasterSlaveMode_Enable);

	TIM_Cmd(TIM3,ENABLE); 
	
	
	/***********ADC***************/
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);//����ADC��Ƶ����6 72M/6=12,ADC���ʱ�䲻�ܳ���14M
	ADC_DeInit(ADC1); 
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent; 
	ADC_InitStructure.ADC_NbrOfChannel = 1; 
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE; 
	ADC_InitStructure.ADC_ScanConvMode = DISABLE; 
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T3_TRGO;//TIM3��TRGO�����¼�����ADC 
	ADC_InitStructure.ADC_DataAlign =ADC_DataAlign_Right; 
	ADC_Init(ADC1,&ADC_InitStructure); 
	
	ADC_ExternalTrigConvCmd(ADC1,ENABLE);//ʹ���ⲿ����
	
	ADC_RegularChannelConfig(ADC1,ADC_Channel_1,1,ADC_SampleTime_55Cycles5);//ADC1��Channel1
	
	ADC_DMACmd(ADC1, ENABLE);/***********ADC��DMA****************/
	
	ADC_Cmd(ADC1,ENABLE); 

	ADC_ResetCalibration(ADC1);
	while(ADC_GetResetCalibrationStatus(ADC1)); 
	ADC_StartCalibration(ADC1); 
	while(ADC_GetCalibrationStatus(ADC1));
	
	
	
	/***********DMA***************/
	DMA_InitStructure.DMA_PeripheralBaseAddr = ( uint32_t ) ( & ( ADC1->DR ) );// �����ַΪ��ADC ���ݼĴ�����ַ
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)Wave_Input_Temp_12_bits_Array;// �洢����ַ��ʵ���Ͼ���һ���ڲ�SRAM�ı���
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	DMA_InitStructure.DMA_BufferSize = 256;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;//�ڴ��ַ���� 
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;/****ѭ������ģʽ****/ 	
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;// DMA ����ͨ�����ȼ�Ϊ�ߣ���ʹ��һ��DMAͨ��ʱ�����ȼ����ò�Ӱ��
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;// ��ֹ�洢�����洢��ģʽ����Ϊ�Ǵ����赽�洢��
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);//DMA1��Channel1����ADC1

	DMA_Cmd(DMA1_Channel1 , ENABLE);
	
	DMA_ITConfig(DMA1_Channel1,DMA_IT_TC,ENABLE); //��������ж�
	
	
	/***********NVIC***************/
	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	

}


void DMA1_Channel1_IRQHandler(void)
{        
	int i=0;
	
	
	for(i=0;i<256;i++)
	{
		Wave_Input_12_bits_Array[i] = Wave_Input_Temp_12_bits_Array[i];//�ѻ���������ʱֵ�ŵ��ȶ�����
	}        
	
	if(show_wave == Show_SineWave)
		DMA2_Channel3->CMAR = (uint32_t)SineWave_12_bits_Array;
	if(show_wave == Show_SawtoothWave)
		DMA2_Channel3->CMAR = (uint32_t)SawtoothWave_12_bits_Array;
	if(show_wave == Show_SquareWave)
		DMA2_Channel3->CMAR = (uint32_t)SquareWave_12_bits_Array;
	
	
	if(show_wave == Show_Altered_SquareWave)
		DMA2_Channel3->CMAR = (uint32_t)Altered_SquareWave_12_bits_Array;
	
	DMA_ClearITPendingBit(DMA1_IT_TC1);
	DMA_ClearFlag(DMA1_FLAG_TC1);
}





























