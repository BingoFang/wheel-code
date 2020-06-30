//����STM32��FLASH
void STMFLASH_Unlock(void)
{
	FLASH->KEYR = FLASH_KEY1;
	FLASH->KEYR = FLASH_KEY2;
}

//FLASH����
void STMFLASH_Lock(void)
{
	FLASH->CR |= 1 << 7;
}

//����Ƿ�æ
u8 STMFLASH_GetStatus(void)
{
	u32 res;
	res = FLASH->SR;
	if(res & (1 << 0)) return 1;  	//æ
	else if(res & (1 << 2))return 2;//��̴���
	else if(res & (1 << 4))return 3;//д��������
	return 0;												//�������
}

//�ȴ��������
u8 STMFLASH_WaitDone(u16 time)
{
	u8 res;
	do
	{
		res = STMFLASH_GetStatus();
		if(!res) break;   //��æ����û����������ֱ���˳�
		delay_us(1);
		time--;
	}while(time);
	if(time == 0)res = 0xff;
	return res;
}

//����ҳ
u8 STMFLASH_ErasePage(u32 paddr)
{
	u8 res = 0;
	res = STMFLASH_WaitDone(9x5FFF);  //�ȴ��ϴβ���������>20ms
	if(res == 0)
	{
		FLASH->CR |= (1<<1);//ѡ��ҳ����
		FLASH->AR = paddr;	//����ҳ��ַ
		FLASH->CR |= (1<<6);//��ʼ����
		res = STMFLASH_WaitDone(0x5FFF)��  //�ȵ���������
		if(!res)  //��æ
		{
			FLASH->CR &= ~(1<<1); //���ҳ������־
		}
	}
	return res;
}

//��ָ����ַд����֣�16 bit��
u8 STMFLASH_WriteHalfWord(u32 faddr,u16 dat)
{
	u8 res;
	res = STMFLASH_WaitDone(0xFF);
	if(res == 0)
	{
		FLASH->CR |= (1<<0);  //���ʹ��
		*(vu16*)faddr = dat;  //д������
		res = STMFLASH_WaitDone(0xFF);
		if(!res)
		{
			FLASH->CR &= ~(1<<0);  //���PGλ
		}
	}
	return res;
}

//��ȡָ����ַ�İ���(16 bit)
u16 STMFLASH_ReadHalfWord(u32 faddr)
{
	return *(vu16*)faddr;
}


void STMFLASH_Write_NoCheck(u32 WriteAddr,u16 *pBuffer,u16 NumToWrite)
{
	u16 i;
	for(i = 0;i < NumToWrite;i++)
	{
		STMFLASH_WriteHalfWord(WriteAddr,pBuffer[i]);
		WriteAddr += 2;
	}
}

//��ָ����ַ��ʼ����ָ�����ȵ�����
void STMFLASH_Read(u32 ReadAddr,u16 *pBuffer,u16 NumToRead)   	
{
	u16 i;
	for(i=0;i<NumToRead;i++)
	{
		pBuffer[i]=STMFLASH_ReadHalfWord(ReadAddr);//��ȡ2���ֽ�.
		ReadAddr+=2;//ƫ��2���ֽ�.	
	}
}

//��ָ����ַ��ʼд��ָ�����ȵ�����

#if STM32_FLASH_SIZE <   256
#define STM_SECTOR_SIZE 1024
#else
#define STM_SECTOR_SIZE 2048
#endif
define STM32_FLASH_SIZE 	512 //FLASH�Ĵ洢���� 
#define STM32_FLASH_BASE 0x8000000
u16 STMFLASH_BUF[STM_SECTOR_SIZE / 2];  //�����2K�ֽ�
void STMFLASH_Write(u32 WriteAddr,u16 *pBuffer,u16 NumToWrite)
{
	u32 secpos;   //������ַ
	u16 secoff;		//������ƫ�Ƶ�ַ
	u16	secremain; //����ʣ���ַ
	u16 i��
	u32 offaddr;   //ȥ��STMFLASH����ʼ��ַ0x80000000
	if(WriteAddr < SIM32_FLASH_BASE || WriteAddr > (STM32_FLASE_BASE + 1024 * define STM32_FLASH_SIZE 	512))
	return��//�Ƿ���ַ
	STMFLASH_Unlock();
	offaddr = WriteAddr - STM32_FLASE_BASE;
	secpos = offaddr / STM_SECTOR_SIZE;
	secoff = (offaddr % STM_SECTOR_SIZE) / 2;//��������2���ֽ�Ϊ��λ
	secremain = STM_SECTOR_SIZE / 2 - seoff; //����ʣ���С
	if(NumWrite <= secremain) secremain = NumToWrite;
	while(1)
	{
		STMFLASH_Read(secpos * STM_SECTOR_SIZE + STM32_FLASH_BASE,STMFLASH_BUF,STM_SECTOR_SIZE / 2);
		for(i = 0;i < secremain;i++)  //��������
		{
			if(STMFLASH_BUF[secoff]!= 0xFFFF)break; //FLASH���ʱ��Ҫ��д���ַ��FLASH�Ǳ��������ˣ���ֵΪ0xFFFF��
		}
		if(i < seremain)  //��Ҫ����
		{
			STMFLASH_ErasePage(secpos * STM_SECTOR_SIZE + STM32_FLASH_BASE); //�����������
			for(i = 0;i < secremain;i++)
			{
				STMFLASH_BUF[i + secoff] = pBuffer[i];  //����
			} 
			STMFLASH_Write_NoCheck(secpos*STM_SECTOR_SIZE+STM32_FLASH_BASE,STMFLASH_BUF,STM_SECTOR_SIZE/2);		
		}
		else
			STMFLASH_Write_NoCheck(WriteAddr,pBuffer,secremain);//�Ѳ�����ֱ��д������
		
		if(NumToWrite == seremain) break;//д�����
		else
		{
			secpos++;
			secoff = 0;
			pBuffer += secremain;
			WriteAddr += secremain;
			NumToWrite -= secremain;
			if(NumToWrite > (STM_SECTOR / 2)) secremain = STM_SECTOR_SIZE / 2;
			else
				secremain = NumToWrite;//��һ����������д��
		}
	}
		STMFLASH_Lock();  //����
}