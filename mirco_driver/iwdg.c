IWDG_Init(4,625);			//LSIʱ��40KHZ��64��Ƶ��1Sι��ʱ��

void IWDG_Init(u8 prer, u16 rlr)
{
  	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable); //ʹ�ܶԼĴ���д����
	IWDG_SetPrescaler(prer); //����IWDG��Ԥ��Ƶֵ
	IWDG_SetReload(rlr);	//����IWDG��װ��ֵ
	IWDG_ReloadCounter();	//����IWDG��װ�ؼĴ�����ֵ��װ��IWDG������
	IWDG_Enable(); //ʹ��IWDG
}

//ι�������Ź�
void IWDG_Feed(void)
{   
 	IWDG_ReloadCounter();										   
}