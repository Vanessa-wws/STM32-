#include "dac.h"

/*********************************************
˼·��
DACʹ��DMA
ADCʹ��DMA
*********************************************/




/******DAC�Ĵ�����ַ����*******/  
#define DAC_DHR12R1    (u32)&(DAC->DHR12R1)   //DACͨ��1�����ַ��PA4��
#define DAC_DHR12R2    (u32)&(DAC->DHR12R2)   //DACͨ��2�����ַ��PA5��

extern u16 SineWave_12_bits_Array[256];
extern u16 Wave_Input_12_bits_Array[256];

//PA4ΪDACͨ��1����ܽ�
void SineWave_Output_TIM2_DAC1_DMA2_Init(void)
{
  
	GPIO_InitTypeDef			GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef		TIM_TimeBaseStructure;
	DAC_InitTypeDef				DAC_InitStructure;
	DMA_InitTypeDef				DMA_InitStructure;
	

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE );	  //ʹ��PORTAͨ��ʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);	  //ʹ��TIM2��ʱ�����ڲ��������¼��Ӷ�����DAC
   	RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE );	  //ʹ��DACͨ��ʱ�� 
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA2, ENABLE);		  //ʹ��DMA2ʱ��
	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;				 // �˿�����
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN; 		 //ģ������
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_SetBits(GPIOA,GPIO_Pin_4)	;//PA.4 �����
					
	
	//1ms����һ�Σ�arr=10��pre=7200,����Ƶ��Ϊ10K����һ������Ҫ1ms��������һ�������Ҳ�������Ҫ256ms����ʵ��֤����������������Σ�
	//ֻ�У�arr=10,pre=720,����Ƶ��Ϊ100K����һ������Ҫ100us��������һ�������Ҳ�������Ҫ256 x 100us = 25.6ms�����Ҳ�Ƶ��Ϊ40Hz���ң�ʵ��39.1Hz��
	//�Լ���arr=10,pre=72,����Ƶ��Ϊ1000K����һ������Ҫ10us��������һ�������Ҳ�������Ҫ256 x 10us = 2.56ms�����Ҳ�Ƶ��Ϊ400Hz���ң�ʵ��391Hz��
	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
    TIM_TimeBaseStructure.TIM_Prescaler = 72-1;
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����Ƶ
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;//���ϼ���
    TIM_TimeBaseStructure.TIM_Period = 10-1;//�������Ƶ��
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
    TIM_SelectOutputTrigger(TIM2, TIM_TRGOSource_Update);//����TIME�������Ϊ����ģʽ
	
	TIM_Cmd(TIM2,ENABLE);
	
	
	DAC_StructInit(&DAC_InitStructure);
	DAC_InitStructure.DAC_Trigger = DAC_Trigger_T2_TRGO;//DAC����Ϊ��ʱ��2����
	DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_None;//����������
	DAC_InitStructure.DAC_LFSRUnmask_TriangleAmplitude=DAC_LFSRUnmask_Bit0;//���Ρ���ֵ����
	DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Disable; //��ʹ���������
    DAC_Init(DAC_Channel_1,&DAC_InitStructure);	 //��ʼ��DACͨ��1

	DAC_Cmd(DAC_Channel_1, ENABLE);    //ʹ��DAC��ͨ��1
	
	DAC_DMACmd(DAC_Channel_1, ENABLE); //ʹ��DACͨ��1��DMA
	
  
    //DAC_SetChannel1Data(DAC_Align_12b_R, 0);  //12λ�Ҷ������ݸ�ʽ����DACֵ����ʼ��Ϊ0
	
	
	DMA_StructInit( &DMA_InitStructure);        //DMA�ṹ���ʼ��
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;//�ӼĴ���������
    DMA_InitStructure.DMA_BufferSize = 256;//�Ĵ�����С
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//�����ַ������
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable; //�ڴ��ַ����
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;//���Ϊ����
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;//���Ϊ����
    DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;//���ȼ��ǳ���
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;//�ر��ڴ浽�ڴ�ģʽ
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;//ѭ������ģʽ
	DMA_InitStructure.DMA_PeripheralBaseAddr = DAC_DHR12R1;//�����ַΪDACͨ��1�ĵ�ַ
	/***************************************************************************************************/
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)SineWave_12_bits_Array;//�������ݱ��ڴ��ַ
	/***************************************************************************************************/
	DMA_Init(DMA2_Channel3, &DMA_InitStructure);//��ʼ��
	
	DMA_Cmd(DMA2_Channel3, ENABLE);//ʹ��DMA2ͨ��3
	
	
	/*************************���Բ������õ�DAC2*******************************/
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;				 // �˿�����
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN; 		 //ģ������
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_SetBits(GPIOA,GPIO_Pin_5)	;//PA.5 �����
	
	DAC_InitStructure.DAC_Trigger = DAC_Trigger_T2_TRGO;//DAC����Ϊ��ʱ��2����
	DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_None;//����������
	DAC_InitStructure.DAC_LFSRUnmask_TriangleAmplitude=DAC_LFSRUnmask_Bit0;//���Ρ���ֵ����
	DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Disable; //��ʹ���������
    DAC_Init(DAC_Channel_2,&DAC_InitStructure);	 //��ʼ��DACͨ��2

	DAC_Cmd(DAC_Channel_2, ENABLE);    //ʹ��DAC��ͨ��2
	
	DAC_DMACmd(DAC_Channel_2, ENABLE); //ʹ��DACͨ��2��DMA
	
	
	
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;//�ӼĴ���������
    DMA_InitStructure.DMA_BufferSize = 256;//�Ĵ�����С
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//�����ַ������
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable; //�ڴ��ַ����
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;//���Ϊ����
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;//���Ϊ����
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;//���ȼ��ǳ���
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;//�ر��ڴ浽�ڴ�ģʽ
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;//ѭ������ģʽ
	DMA_InitStructure.DMA_PeripheralBaseAddr = DAC_DHR12R2;//�����ַΪDACͨ��2�ĵ�ַ
	/***************************************************************************************************/
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)Wave_Input_12_bits_Array;//�������ݱ��ڴ��ַ
	/***************************************************************************************************/
	DMA_Init(DMA2_Channel4, &DMA_InitStructure);//��ʼ��
	
	DMA_Cmd(DMA2_Channel4, ENABLE);//ʹ��DMA2ͨ��4
	
	
	
	

}

//����ͨ��1�����ѹ
//vol:0~3300,����0~3.3V
void Dac1_Set_Vol(u16 vol)
{
	float temp=vol;
	temp/=1000;
	temp=temp*4096/3.3;
	DAC_SetChannel1Data(DAC_Align_12b_R,temp);//12λ�Ҷ������ݸ�ʽ����DACֵ
}






