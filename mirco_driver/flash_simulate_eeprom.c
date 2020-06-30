//解锁STM32的FLASH
void STMFLASH_Unlock(void)
{
	FLASH->KEYR = FLASH_KEY1;
	FLASH->KEYR = FLASH_KEY2;
}

//FLASH上锁
void STMFLASH_Lock(void)
{
	FLASH->CR |= 1 << 7;
}

//检查是否忙
u8 STMFLASH_GetStatus(void)
{
	u32 res;
	res = FLASH->SR;
	if(res & (1 << 0)) return 1;  	//忙
	else if(res & (1 << 2))return 2;//编程错误
	else if(res & (1 << 4))return 3;//写保护错误
	return 0;												//操作完成
}

//等待操作完成
u8 STMFLASH_WaitDone(u16 time)
{
	u8 res;
	do
	{
		res = STMFLASH_GetStatus();
		if(!res) break;   //非忙，且没有其他错误，直接退出
		delay_us(1);
		time--;
	}while(time);
	if(time == 0)res = 0xff;
	return res;
}

//擦除页
u8 STMFLASH_ErasePage(u32 paddr)
{
	u8 res = 0;
	res = STMFLASH_WaitDone(9x5FFF);  //等待上次操作结束，>20ms
	if(res == 0)
	{
		FLASH->CR |= (1<<1);//选择页擦除
		FLASH->AR = paddr;	//设置页地址
		FLASH->CR |= (1<<6);//开始擦除
		res = STMFLASH_WaitDone(0x5FFF)；  //等到操作结束
		if(!res)  //非忙
		{
			FLASH->CR &= ~(1<<1); //清除页擦除标志
		}
	}
	return res;
}

//向指定地址写入半字（16 bit）
u8 STMFLASH_WriteHalfWord(u32 faddr,u16 dat)
{
	u8 res;
	res = STMFLASH_WaitDone(0xFF);
	if(res == 0)
	{
		FLASH->CR |= (1<<0);  //编程使能
		*(vu16*)faddr = dat;  //写入数据
		res = STMFLASH_WaitDone(0xFF);
		if(!res)
		{
			FLASH->CR &= ~(1<<0);  //清除PG位
		}
	}
	return res;
}

//读取指定地址的半字(16 bit)
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

//从指定地址开始读出指定长度的数据
void STMFLASH_Read(u32 ReadAddr,u16 *pBuffer,u16 NumToRead)   	
{
	u16 i;
	for(i=0;i<NumToRead;i++)
	{
		pBuffer[i]=STMFLASH_ReadHalfWord(ReadAddr);//读取2个字节.
		ReadAddr+=2;//偏移2个字节.	
	}
}

//从指定地址开始写入指定长度的数据

#if STM32_FLASH_SIZE <   256
#define STM_SECTOR_SIZE 1024
#else
#define STM_SECTOR_SIZE 2048
#endif
define STM32_FLASH_SIZE 	512 //FLASH的存储容量 
#define STM32_FLASH_BASE 0x8000000
u16 STMFLASH_BUF[STM_SECTOR_SIZE / 2];  //最多是2K字节
void STMFLASH_Write(u32 WriteAddr,u16 *pBuffer,u16 NumToWrite)
{
	u32 secpos;   //扇区地址
	u16 secoff;		//扇区的偏移地址
	u16	secremain; //扇区剩余地址
	u16 i；
	u32 offaddr;   //去掉STMFLASH的起始地址0x80000000
	if(WriteAddr < SIM32_FLASH_BASE || WriteAddr > (STM32_FLASE_BASE + 1024 * define STM32_FLASH_SIZE 	512))
	return；//非法地址
	STMFLASH_Unlock();
	offaddr = WriteAddr - STM32_FLASE_BASE;
	secpos = offaddr / STM_SECTOR_SIZE;
	secoff = (offaddr % STM_SECTOR_SIZE) / 2;//在扇区内2个字节为单位
	secremain = STM_SECTOR_SIZE / 2 - seoff; //扇区剩余大小
	if(NumWrite <= secremain) secremain = NumToWrite;
	while(1)
	{
		STMFLASH_Read(secpos * STM_SECTOR_SIZE + STM32_FLASH_BASE,STMFLASH_BUF,STM_SECTOR_SIZE / 2);
		for(i = 0;i < secremain;i++)  //检验数据
		{
			if(STMFLASH_BUF[secoff]!= 0xFFFF)break; //FLASH编程时，要求写入地址的FLASH是被擦除的了（其值为0xFFFF）
		}
		if(i < seremain)  //需要擦除
		{
			STMFLASH_ErasePage(secpos * STM_SECTOR_SIZE + STM32_FLASH_BASE); //擦除这个扇区
			for(i = 0;i < secremain;i++)
			{
				STMFLASH_BUF[i + secoff] = pBuffer[i];  //复制
			} 
			STMFLASH_Write_NoCheck(secpos*STM_SECTOR_SIZE+STM32_FLASH_BASE,STMFLASH_BUF,STM_SECTOR_SIZE/2);		
		}
		else
			STMFLASH_Write_NoCheck(WriteAddr,pBuffer,secremain);//已擦除的直接写入数据
		
		if(NumToWrite == seremain) break;//写入结束
		else
		{
			secpos++;
			secoff = 0;
			pBuffer += secremain;
			WriteAddr += secremain;
			NumToWrite -= secremain;
			if(NumToWrite > (STM_SECTOR / 2)) secremain = STM_SECTOR_SIZE / 2;
			else
				secremain = NumToWrite;//下一个扇区可以写完
		}
	}
		STMFLASH_Lock();  //上锁
}