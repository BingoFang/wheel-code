//SystemTickCnt���ɶ�ʱ���趨1ms�ж��ۼӣ�
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
***�������ܣ����ñȽ�ʱ������ʱ
***����������timeΪĳһʱ�̱����ʼʱ��,��¼��ʱ��SysTemTickCnt��nΪ�Ƚϵ�ʱ���С
***�������أ�1��ʱ���㹻  2��ʱ�䲻��
***����˵����SystemTickCnt��Ϊȫ�ֱ���
******************************************/
bool CheckSystemTick(unsigned int time,unsigned int n)
{
	if((SystemTickCnt - time) >= n)
	{
		return TRUE;
	}
	return FALSE;
}