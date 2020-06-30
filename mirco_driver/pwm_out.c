void Time4_Config(u16 arr, u16 psc)
{
	RCC->APB2ENR|=1<<0;    //����ʱ��ʹ��
	RCC->APB1ENR|=1<<2;    //��ʱ��4ʱ��ʹ�� 
	RCC->APB2ENR|=1<<3;    //GPIOBʱ��ʹ��
	  
	GPIOB->CRH&=0XFFFFFFF0;//GPIOB8
	GPIOB->CRH|=0X0000000B;//�������     
	GPIOB->ODR|=1<<8;	   //PB8����
	
	GPIOB->CRH&=0XFFFFFF0F;//GPIOB9  
	GPIOB->CRH|=0X000000B0;//�������    
	GPIOB->ODR|=1<<9;	   //PB9����   
	  
	TIM4->ARR=arr;		   //��װ��ֵ
	TIM4->PSC=psc;    	   //Ԥ��Ƶֵ
	
	
	TIM4->CCMR2|=6<<4;     //CH3 PWM1ģʽ    
	TIM4->CCMR2|=1<<3;     //CH3 Ԥװ��ʹ�� 
	TIM4->CCER|=1<<8;      //OC3  ���ʹ��   
	TIM4->CR1=0x0080;      //ARPEʹ��
	
	TIM4->CCMR2|=6<<12;    //CH4 PWM1ģʽ     
	TIM4->CCMR2|=1<<11;    //CH4 Ԥװ��ʹ��    
	TIM4->CCER|=1<<12;     //OC4 ���ʹ��  
	TIM4->CR1=0x0080;      //ARPE ʹ��
	
	TIM4->CR1|=0x01;       //��ʱ��4ʹ��
	
	
	TIM4->CCR3 = 0;		   //����CCRx������ռ�ձ�
	TIM4->CCR4 = 0;	
}