//系统频率11.0592MHZ

//利用定时0，定时100us，工作方式2(8位自动重载)
sfr AUXR = 0x8E;
void Timer0(void)
{
    AUXR &= 0x7F;  //新增特殊寄存器AUXR，12分频和不分频
    TMOD &= 0xF0;
    TMOD |= 0x02; //设置好定时器0，设置了工作方式2
    TF0 = 0;      //清楚中断标志位
    TR0 = 1;      //允许定时器0开始计时
    TH0 = 0xA3;   //定时100us
    TL0 = 0xA3;
    EA = 1；      //开启总中断
}
void Timer0() interrupt 1   //定时器0的中断处理函数
{

}

//利用定时1，定时100us，工作方式2（8位自动重装）
void Timer1Init(void)		//100??@11.0592MHz
{
    AUXR &= 0xBF;		//定时器1时钟设置为12T
    TMOD &= 0x0F;
    TMOD |= 0x20;		//设置定时器1，设置工作方式2
    TL1 = 0xA4;			//定时100us
    TH1 = 0xA4;
    TF1 = 0;
    TR1 = 1;
    EA = 1；
}
void Timer1() interrupt 3
{

}

//利用定时器1->Uart1产生9600波特率
void Uart1_9600(void)
{
    PCON &= 0x3F; //波特率不加倍，对SCON中的SM0和SM1工作方式
    SCON |= 0x50; //设置了工作方式1，开启串口接收
    AUXR &= 0xBF; //定时器1 12分频，不允许独立波特率
    AUXR &= 0xFE; //选择定时器1作为波特率发生器
    TMOD &= 0x0F;
    TMOD |= 0x20;
    TH1 = 0xFD;
    TL1 = 0xFD;
    ET1 = 0;   	  //静止定时1中断
    TR1 = 1;      //启动定时器1
    ES = 1;       //允许串口中断
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
void UartSelfSend(void)      //自收自发
{
    if (receive_flag)
    {
        for (i = 0; i < len; i++)
        {
            SBUF = receive[i];
        }
    }
    receive_flag = 0;     //接受发送结束后标志位清零，等待下一次发送标志位
    memset(receive, 0, i); //发送一次数据后清掉原来的数据
    len = 0；             //数据长度清零
}
void UartSendString(char *str)  //一次只能发送8位
{
    while (*str++ != '\0')  //while(*str） 对每个字节进行判断
    {
        SendData(*str);
    }
}

void SendData(BYTE data)   //一次发送一个字节
{
    ES = 0；       //关闭串口中断
         SCON &= 0xEF;  //禁止串口接受

    TI = 0;
    while (!TI);
    SBUF = data;
    TI = 0;    	   //手动清除发送标志位

    ES = 1;
    SCON |= 0x10;  //允许串口接收
}

#define ADC_POWER  0x80  //开启ADC转换电源
#define ADC_SPEED  0x00  //ADC转换速度，540个时钟周期转换一次
#define ADC_FLAG   0x10  //ADC转换结束标志位
#define ADC_START  0x08  //ADC转换开始
#define ADC_CHS    0x01  //选择P1.1口作为AD输入使用   
//注意：如果没有外部固定的基准参考电压，没有办法直接测出所测的电压
//因为这类ADC为逐次比较型，从最高位开始，对每次输入的电压与内置DA转换器进行多次比较，使得所得数字量逐次逼近输入模拟量对应值
//选取P1.1作为AD口实现采集功能  10bits AD = 1024 * (Vin / Vcc);
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
    IE = 0xA0;  	 //EA，EADC中断允许
    P1ASF |= 0x40;   //设置p1.1作为模拟AD口使用
    if (!ADC_POWER)  //确认AD口是否已打开
    {
        delay_ms(3); //初次进行ADC转换需要等待电源稳定延时
        ADC_RES = 0；//ADC转换结果
                  ADC_CONTR = ADC_POWER | ADC_SPEED | ADC_FLAG | ADC_START | ADC_CHS; //datasheet提示直接用mov赋值语句，不要用与或
        Delay(2);  //需要4个系统时钟的延时，才能保证ADC_CONTR读取正确值
    }
}
void ADC_isr()  interrupt 5    //ADC中断接收
{
    ADC_CONTR &= !ADC_FLAG;  //清除ADC中断标志位
    SendData(ADC_CHS);  	 //发送ADC转化通道
    SendData(ADC_RES);  	 //发送ADC转换后的值 high 8 bits
    SendData(ADC_ADC_RESL);  //low 2bits
    ADC_CONTR = ADC_POWER | ADC_SPEED | ADC_FLAG | ADC_START | ADC_CHS;
}

//两路可编程计数器陈列(PCA)模块，用于软件定时，外部脉冲的捕捉，高速输出以及脉冲调制（PWM）

//同步串行外围接口（SPI）
//1.单主机->单从机 2.双器件（器件可互为主机和从机）
void main(void)
{
#ifdef MASTER
    ACC = receUart();
    SPISS = 0;			//设置从SS为低指定从机开始传数据
    SPDAT = ACC;
#endif
}
void SPI_Init(void)  	  //1.单主机——单从机
{
    SPDAT = 0;      	  //初始化SPI data
#ifdef MASTER
    SPCTL = SPEN | MSTR;  //选择主机模式
#else
    SPCTL = SPEN; 		  //选择从机模式
#endif
    SPSTAT |= 0xC0;  	  //clear SPI status
    AUXR1 |= 0x20;  	  //切换SPI的CLK，MISO，MOSI口到P4.3,P4.2,P4.1
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

//相互切换为主从
void main(void)
{
    if (RI) 					 //从->主
    {
        SPCTL = SPEN | MSTR;  //切换为主机
        MSSEL = 1;			  //主机标志位
        ACC = receuart();
        SPISS = 0;			  //将之前的主机设定为从机
        SPDAT = ACC;          //将之前作为从接受到的数据作为主机发送回去
    }
}
void SPI_ISR() interrupt 9
{
    SPSTAT = SPIF | WCOL;
    if (MSSEL)
    {
        SPCTL = SPEN;
        MESSEL = 0;			//主->从
        SPISS = 1；			//将之前是从的强制改变成主机
                SendUart(SPDAT);
    }
    else					//作为从机
    {
        SPDAT = SPDAT;
    }
}