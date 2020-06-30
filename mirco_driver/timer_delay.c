//SystemTickCnt是由定时器设定1ms中断累加，
/**
  * @brief TIM1 Update/Overflow/Trigger/Commutation Interrupt routine.
  * @param  None
  * @retval None
  */
INTERRUPT_HANDLER(TIM1_UPD_OVF_TRG_COM_IRQHandler,23)
{
    /* In order to detect unexpected events during development,
       it is recommended to set a breakpoint on the following instruction.
    */

	TIM1->SR1 &= ~0x01;
	++StmSysTickCnt;
}

/******************************************
***函数功能：利用比较时钟来定时
***函数参数：time为某一时刻标记起始时间,记录当时的SysTemTickCnt，n为比较的时间大小
***函数返回：1：时间足够  2：时间不足
***函数说明：SystemTickCnt作为全局变量
******************************************/
bool CheckSystemTick(unsigned int time,unsigned int n)
{
	if((SystemTickCnt - time) >= n)
	{
		return TRUE;
	}
	return FALSE;
}