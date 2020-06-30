IWDG_Init(4,625);			//LSI时钟40KHZ，64分频，1S喂狗时间

void IWDG_Init(u8 prer, u16 rlr)
{
  	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable); //使能对寄存器写操作
	IWDG_SetPrescaler(prer); //设置IWDG的预分频值
	IWDG_SetReload(rlr);	//设置IWDG重装载值
	IWDG_ReloadCounter();	//按照IWDG重装载寄存器的值重装载IWDG计数器
	IWDG_Enable(); //使能IWDG
}

//喂独立看门狗
void IWDG_Feed(void)
{   
 	IWDG_ReloadCounter();										   
}