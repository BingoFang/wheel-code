u8 co2_RxBuf[32];
u8 co2_TxBuf[16];
u8 co2_RxCnt=0;
u8 co2_20ms=0, co2_RxOk=0;

//////////////接收部分///////////////
//回调函数
void co2RxInterrupt(u8 dat)
{
  	if(co2_RxCnt>=sizeof(co2_RxBuf)) return;
	if(co2_RxCnt==0) co2_20ms=0;	//每条命令生存时间60ms
	co2_RxBuf[co2_RxCnt++] = dat;
}
//EXTI模拟串口接收引脚
#pragma vector=8
__interrupt void EXTI_PORTD_IRQHandler(void)
{
	u8 tmp, i=9;
	PD_CR2 &= ~(1<<2);	//禁止该中断
	delay_us(30);
	while(i--)
	{
		tmp>>=1;
		if(PD_IDR&(1<<2)) tmp|=0x80;
		delay_us(VUART_TIMING_DELAY);
	}
	//EXTI_SR |= (1<<2);	//普通模式通过此寄存器清除中断标志位
	co2RxInterrupt(tmp);	//放入缓冲区
	PD_CR2 |= 1<<2;	//重新打开该中断
}
//定时器溢出中断
#pragma vector=0xD
__interrupt void TIM1_UPD_OVF_TRG_BRK_IRQHandler(void)	//20ms中断一次
{
	static u16 tt=0;
	if(co2_RxCnt) co2_20ms++;
	if(co2_20ms>4) { co2_20ms=0; co2_RxOk=1; }
	TIM1_SR1 = (uint8_t)(~(uint8_t)TIM1_IT_UPDATE);
}




//////////////发送部分///////////////
//发送字节
void VUART_SendByte(u8 b)
{
	u8 i;
	VU_TXD_0; //发送起始位
	delay_us(VUART_TIMING_DELAY);
	for(i=0; i<8; i++)	//发送8位数据
	{
		if((b>>i & 0x01) == 0) VU_TXD_0; else VU_TXD_1;	//低位在前
		delay_us(VUART_TIMING_DELAY);	//0	
	}
	VU_TXD_1;
	delay_us(VUART_TIMING_DELAY);
}
//发送缓冲区
void VUART_SendData(u8 *buf, u8 len)	//发送数组最好用这个，也可发送字符串
{
	u8 i;
	for(i=0; i<len; i++)
		VUART_SendByte(buf[i]);
}
//引脚初始化
void VUART_Init(void)
{
	PC_DDR |= 1<<4;		//PC4模拟TXD，推挽输出
	PC_CR1 |= 1<<4;
	PD_DDR &= ~(1<<2);	//PD2模拟RXD，中断上拉输入
	PD_CR1 |= 1<<2;
	PD_CR2 |= 1<<2;
}

////////////延时//////////////
//适用于时钟8MHz
void delay_1us(void)   
{    
    asm("nop"); //一个asm("nop")函数经过示波器测试代表100ns   
    asm("nop");
	asm("nop"); //一个asm("nop")函数经过示波器测试代表100ns   
    asm("nop");
}   

void delay_us(u16 n)
{
	n=9*n/11 -9;
	while(n--)
		asm("nop");
}

void delay_ms(u16 n)   
{
    unsigned int i;   
    while(n--)     
    	for(i=900; i>0; i--)   
    		delay_1us();    
}

