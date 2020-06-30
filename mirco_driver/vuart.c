u8 co2_RxBuf[32];
u8 co2_TxBuf[16];
u8 co2_RxCnt=0;
u8 co2_20ms=0, co2_RxOk=0;

//////////////���ղ���///////////////
//�ص�����
void co2RxInterrupt(u8 dat)
{
  	if(co2_RxCnt>=sizeof(co2_RxBuf)) return;
	if(co2_RxCnt==0) co2_20ms=0;	//ÿ����������ʱ��60ms
	co2_RxBuf[co2_RxCnt++] = dat;
}
//EXTIģ�⴮�ڽ�������
#pragma vector=8
__interrupt void EXTI_PORTD_IRQHandler(void)
{
	u8 tmp, i=9;
	PD_CR2 &= ~(1<<2);	//��ֹ���ж�
	delay_us(30);
	while(i--)
	{
		tmp>>=1;
		if(PD_IDR&(1<<2)) tmp|=0x80;
		delay_us(VUART_TIMING_DELAY);
	}
	//EXTI_SR |= (1<<2);	//��ͨģʽͨ���˼Ĵ�������жϱ�־λ
	co2RxInterrupt(tmp);	//���뻺����
	PD_CR2 |= 1<<2;	//���´򿪸��ж�
}
//��ʱ������ж�
#pragma vector=0xD
__interrupt void TIM1_UPD_OVF_TRG_BRK_IRQHandler(void)	//20ms�ж�һ��
{
	static u16 tt=0;
	if(co2_RxCnt) co2_20ms++;
	if(co2_20ms>4) { co2_20ms=0; co2_RxOk=1; }
	TIM1_SR1 = (uint8_t)(~(uint8_t)TIM1_IT_UPDATE);
}




//////////////���Ͳ���///////////////
//�����ֽ�
void VUART_SendByte(u8 b)
{
	u8 i;
	VU_TXD_0; //������ʼλ
	delay_us(VUART_TIMING_DELAY);
	for(i=0; i<8; i++)	//����8λ����
	{
		if((b>>i & 0x01) == 0) VU_TXD_0; else VU_TXD_1;	//��λ��ǰ
		delay_us(VUART_TIMING_DELAY);	//0	
	}
	VU_TXD_1;
	delay_us(VUART_TIMING_DELAY);
}
//���ͻ�����
void VUART_SendData(u8 *buf, u8 len)	//������������������Ҳ�ɷ����ַ���
{
	u8 i;
	for(i=0; i<len; i++)
		VUART_SendByte(buf[i]);
}
//���ų�ʼ��
void VUART_Init(void)
{
	PC_DDR |= 1<<4;		//PC4ģ��TXD���������
	PC_CR1 |= 1<<4;
	PD_DDR &= ~(1<<2);	//PD2ģ��RXD���ж���������
	PD_CR1 |= 1<<2;
	PD_CR2 |= 1<<2;
}

////////////��ʱ//////////////
//������ʱ��8MHz
void delay_1us(void)   
{    
    asm("nop"); //һ��asm("nop")��������ʾ�������Դ���100ns   
    asm("nop");
	asm("nop"); //һ��asm("nop")��������ʾ�������Դ���100ns   
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

