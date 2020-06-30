void Time4_Config(u16 arr, u16 psc)
{
	RCC->APB2ENR|=1<<0;    //复用时钟使能
	RCC->APB1ENR|=1<<2;    //定时器4时钟使能 
	RCC->APB2ENR|=1<<3;    //GPIOB时钟使能
	  
	GPIOB->CRH&=0XFFFFFFF0;//GPIOB8
	GPIOB->CRH|=0X0000000B;//复用输出     
	GPIOB->ODR|=1<<8;	   //PB8上拉
	
	GPIOB->CRH&=0XFFFFFF0F;//GPIOB9  
	GPIOB->CRH|=0X000000B0;//复用输出    
	GPIOB->ODR|=1<<9;	   //PB9上拉   
	  
	TIM4->ARR=arr;		   //重装载值
	TIM4->PSC=psc;    	   //预分频值
	
	
	TIM4->CCMR2|=6<<4;     //CH3 PWM1模式    
	TIM4->CCMR2|=1<<3;     //CH3 预装载使能 
	TIM4->CCER|=1<<8;      //OC3  输出使能   
	TIM4->CR1=0x0080;      //ARPE使能
	
	TIM4->CCMR2|=6<<12;    //CH4 PWM1模式     
	TIM4->CCMR2|=1<<11;    //CH4 预装载使能    
	TIM4->CCER|=1<<12;     //OC4 输出使能  
	TIM4->CR1=0x0080;      //ARPE 使能
	
	TIM4->CR1|=0x01;       //定时器4使能
	
	
	TIM4->CCR3 = 0;		   //设置CCRx来控制占空比
	TIM4->CCR4 = 0;	
}