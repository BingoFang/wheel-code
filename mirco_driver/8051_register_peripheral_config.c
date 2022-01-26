//ϵͳƵ��11.0592MHZ

//���ö�ʱ0����ʱ100us��������ʽ2(8λ�Զ�����)
sfr AUXR = 0x8E;
void Timer0(void)
{
    AUXR &= 0x7F;  //��������Ĵ���AUXR��12��Ƶ�Ͳ���Ƶ
    TMOD &= 0xF0;
    TMOD |= 0x02; //���úö�ʱ��0�������˹�����ʽ2
    TF0 = 0;      //����жϱ�־λ
    TR0 = 1;      //����ʱ��0��ʼ��ʱ
    TH0 = 0xA3;   //��ʱ100us
    TL0 = 0xA3;
    EA = 1��      //�������ж�
}
void Timer0() interrupt 1   //��ʱ��0���жϴ�����
{

}

//���ö�ʱ1����ʱ100us��������ʽ2��8λ�Զ���װ��
void Timer1Init(void)		//100??@11.0592MHz
{
    AUXR &= 0xBF;		//��ʱ��1ʱ������Ϊ12T
    TMOD &= 0x0F;
    TMOD |= 0x20;		//���ö�ʱ��1�����ù�����ʽ2
    TL1 = 0xA4;			//��ʱ100us
    TH1 = 0xA4;
    TF1 = 0;
    TR1 = 1;
    EA = 1��
}
void Timer1() interrupt 3
{

}

//���ö�ʱ��1->Uart1����9600������
void Uart1_9600(void)
{
    PCON &= 0x3F; //�����ʲ��ӱ�����SCON�е�SM0��SM1������ʽ
    SCON |= 0x50; //�����˹�����ʽ1���������ڽ���
    AUXR &= 0xBF; //��ʱ��1 12��Ƶ�����������������
    AUXR &= 0xFE; //ѡ��ʱ��1��Ϊ�����ʷ�����
    TMOD &= 0x0F;
    TMOD |= 0x20;
    TH1 = 0xFD;
    TL1 = 0xFD;
    ET1 = 0;   	  //��ֹ��ʱ1�ж�
    TR1 = 1;      //������ʱ��1
    ES = 1;       //�������ж�
    EA = 1;
}
void Uart1_Isr() interrupt 4
{
    if (RI)
    {
        RI = 0;
        receive[len] = SBUF;
        len++;
        receive_flag = 1;
    }
}
void UartSelfSend(void)      //�����Է�
{
    if (receive_flag)
    {
        for (i = 0; i < len; i++)
        {
            SBUF = receive[i];
        }
    }
    receive_flag = 0;     //���ܷ��ͽ������־λ���㣬�ȴ���һ�η��ͱ�־λ
    memset(receive, 0, i); //����һ�����ݺ����ԭ��������
    len = 0��             //���ݳ�������
}
void UartSendString(char *str)  //һ��ֻ�ܷ���8λ
{
    while (*str++ != '\0')  //while(*str�� ��ÿ���ֽڽ����ж�
    {
        SendData(*str);
    }
}

void SendData(BYTE data)   //һ�η���һ���ֽ�
{
    ES = 0��       //�رմ����ж�
         SCON &= 0xEF;  //��ֹ���ڽ���

    TI = 0;
    while (!TI);
    SBUF = data;
    TI = 0;    	   //�ֶ�������ͱ�־λ

    ES = 1;
    SCON |= 0x10;  //�����ڽ���
}

#define ADC_POWER  0x80  //����ADCת����Դ
#define ADC_SPEED  0x00  //ADCת���ٶȣ�540��ʱ������ת��һ��
#define ADC_FLAG   0x10  //ADCת��������־λ
#define ADC_START  0x08  //ADCת����ʼ
#define ADC_CHS    0x01  //ѡ��P1.1����ΪAD����ʹ��   
//ע�⣺���û���ⲿ�̶��Ļ�׼�ο���ѹ��û�а취ֱ�Ӳ������ĵ�ѹ
//��Ϊ����ADCΪ��αȽ��ͣ������λ��ʼ����ÿ������ĵ�ѹ������DAת�������ж�αȽϣ�ʹ��������������αƽ�����ģ������Ӧֵ
//ѡȡP1.1��ΪAD��ʵ�ֲɼ�����  10bits AD = 1024 * (Vin / Vcc);
void Delay(WORD n)
{
    WORD x;

    while (n--)
    {
        x = 5000;
        while (x--);
    }
}

void AD_Init()
{
    IE = 0xA0;  	 //EA��EADC�ж�����
    P1ASF |= 0x40;   //����p1.1��Ϊģ��AD��ʹ��
    if (!ADC_POWER)  //ȷ��AD���Ƿ��Ѵ�
    {
        delay_ms(3); //���ν���ADCת����Ҫ�ȴ���Դ�ȶ���ʱ
        ADC_RES = 0��//ADCת�����
                  ADC_CONTR = ADC_POWER | ADC_SPEED | ADC_FLAG | ADC_START | ADC_CHS; //datasheet��ʾֱ����mov��ֵ��䣬��Ҫ�����
        Delay(2);  //��Ҫ4��ϵͳʱ�ӵ���ʱ�����ܱ�֤ADC_CONTR��ȡ��ȷֵ
    }
}
void ADC_isr()  interrupt 5    //ADC�жϽ���
{
    ADC_CONTR &= !ADC_FLAG;  //���ADC�жϱ�־λ
    SendData(ADC_CHS);  	 //����ADCת��ͨ��
    SendData(ADC_RES);  	 //����ADCת�����ֵ high 8 bits
    SendData(ADC_ADC_RESL);  //low 2bits
    ADC_CONTR = ADC_POWER | ADC_SPEED | ADC_FLAG | ADC_START | ADC_CHS;
}

//��·�ɱ�̼���������(PCA)ģ�飬���������ʱ���ⲿ����Ĳ�׽����������Լ�������ƣ�PWM��

//ͬ��������Χ�ӿڣ�SPI��
//1.������->���ӻ� 2.˫�����������ɻ�Ϊ�����ʹӻ���
void main(void)
{
#ifdef MASTER
    ACC = receUart();
    SPISS = 0;			//���ô�SSΪ��ָ���ӻ���ʼ������
    SPDAT = ACC;
#endif
}
void SPI_Init(void)  	  //1.�������������ӻ�
{
    SPDAT = 0;      	  //��ʼ��SPI data
#ifdef MASTER
    SPCTL = SPEN | MSTR;  //ѡ������ģʽ
#else
    SPCTL = SPEN; 		  //ѡ��ӻ�ģʽ
#endif
    SPSTAT |= 0xC0;  	  //clear SPI status
    AUXR1 |= 0x20;  	  //�л�SPI��CLK��MISO��MOSI�ڵ�P4.3,P4.2,P4.1
}

void SPI_ISR()  interrupt 9
{
    SPSTAT |= 0xC0;  	  //clear SPI status
#ifdef MASTER
    SPISS = 1;
    SendUart(SPDAT);
#else
    SPDAT = SPDAT;
#endif
}

//�໥�л�Ϊ����
void main(void)
{
    if (RI) 					 //��->��
    {
        SPCTL = SPEN | MSTR;  //�л�Ϊ����
        MSSEL = 1;			  //������־λ
        ACC = receuart();
        SPISS = 0;			  //��֮ǰ�������趨Ϊ�ӻ�
        SPDAT = ACC;          //��֮ǰ��Ϊ�ӽ��ܵ���������Ϊ�������ͻ�ȥ
    }
}
void SPI_ISR() interrupt 9
{
    SPSTAT = SPIF | WCOL;
    if (MSSEL)
    {
        SPCTL = SPEN;
        MESSEL = 0;			//��->��
        SPISS = 1��			//��֮ǰ�Ǵӵ�ǿ�Ƹı������
                SendUart(SPDAT);
    }
    else					//��Ϊ�ӻ�
    {
        SPDAT = SPDAT;
    }
}