#include <ioCC2530.h>
#include <string.h>
#include <stdio.h>

typedef unsigned char uchar;
typedef unsigned int uint;
typedef unsigned long ulong;

#define LED1 P1_0 //定义 P1.0 口为 LED1 控制端
#define LED2 P1_1 //定义 P1.1 
#define LED3 P1_4 //定义 P1.4，与仿真器共用需拔掉仿真器才起作用
#define KEY1 P0_1 //定义 P0.1
#define ON          0
#define OFF         1
#define UART0_RX    1
#define UART0_TX    2
#define CONTROL_LED 3

#define BUF_SIZE 	20
#define TX_STRING "Hello Zigbee\n"

char tx_buf[BUF_SIZE] = {0};
char rx_buf[BUF_SIZE] = {0};
char uart0_state;
char uart0_temp;
uchar count = 0;

/****************************************************************************
* 名 称: DelayMS()
* 功 能: 以毫秒为单位延时，系统时钟不配置时默认为 16M(用示波器测量相当精确)
* 入口参数: msec 延时参数，值越大，延时越久
* 出口参数: 无
****************************************************************************/
void DelayMS(uint msec)
{
    uint i, j;

    for (i = 0; i < msec; i++)
        for (j = 0; j < 535; j++);
}

/* 统一控制led亮灭 */
void led_on_or_off(uchar mode)
{
    LED1 = mode;
    LED2 = mode;
    LED3 = mode;
}

/* 配置时钟为32MHZ */
void init_clock(void)
{
    CLKCONCMD &= ~0x40;               //设置系统时钟源为32MHZ晶振
    while (CLKCONSTA & 0x40);         //等待晶振稳定为32M
    CLKCONCMD &= ~0x47;               //设置系统主时钟频率为32MHZ
}

/* led灯gpio配置 */
void init_led(void)
{
    P1DIR |= 0x13;     //定义为P1.0，P1.1，P1.3为输出口
    led_on_or_off(1);  //熄灭所有led灯
}

/* KEY按键输入配置 */
void init_key(void)
{
    P0IEN |= 0x02;  //设置P0.1中断输入
    PICTL |= 0x02; //设置下降沿触发中断
    IEN1 |= 0x20;   //使能P0端口中断
    P0IFG = 0x00;   //初始化中断标志位
    EA = 1;         //打开总中断
}
/* 按键扫描 */
uchar key_scan(void)
{
    if (KEY1 == 0)
    {
        DelayMS(10);
        if (KEY1 == 0)
        {
            while (!KEY1); //松手检测
            return 1;
        }
    }
    return 0; //无按键按下
}

/* TIM1功能配置 */
void init_timer1(void)
{
    //  /* 默认不设置时钟为2分频，16MHZ，设定溢出时间约0.52s */
    //  T1CTL = 0x0d; //128分频，从0x0000到0xFFFF反复计数
    //  T1STAT = 0x21;//计数溢出中断，通道0中断有效

    /* 定时器输出pwm */
    CLKCONCMD &= ~0x40;      //设置系统时钟源为32MHZ晶振
    while (CLKCONSTA & 0x40); //等待晶振稳定为32M
    CLKCONCMD &= ~0x07;      //设置系统主时钟频率为32MHZ
    CLKCONCMD |= 0x38;       //时钟速度32 MHz 定时器标记输出设置[5:3]250kHz

    PERCFG |= 0x40;          //定时器1 的IO位置   1:备用位置2
    P2SEL &= ~0x10;          //定时器1优先
    P2DIR |= 0xC0;           //第1优先级：定时器1通道2-3

    P1DIR |= 0xff;           //端口1为输出
    P1SEL |= 0x01;           //timer1 通道2映射口P1_0

    T1CC2H = 0x00;           //20%占空比为200us
    T1CC2L = 0x32;           //修改T1CC2L可调整led的亮度
    T1CC0H = 0x00;           //1ms的周期时钟,频率为976.516HZ
    T1CC0L = 0xff;
    T1CCTL2 = 0x1c;          // 模式选择 通道2比较模式
    T1CTL = 0x02;            //250KHz 1分频

}

/* TIM3功能设置 */
void init_timer3(void)
{
    /* 定时溢出中断 */
    T3CTL |= 0x08;  //开溢出中断
    T3IE = 1;       //开总中断和T3中断
    T3CTL |= 0xE0;  //128分频，设置0.5s溢出，计数值为62500
    T3CTL &= ~0x03; //自由运行，从0x00到0xFF反复计数，62500 / 255 = 245
    T3CTL |= 0x10;  //启动定时器
    EA = 1;         //开启总中断
}

/* 串口配置 */
void init_uart(void)
{
    PERCFG = 0x00;  //USART的I/O置位
    P0SEL = 0x0c;   //P0.2,P0.3用作串口
    P2DIR &= ~0xc0; //P0优先作为Uart0

    U0CSR |= 0x80;  //UART模式
    U0GCR |= 11;
    U0BAUD |= 215;   //波特率115200
    UTX0IF = 0;     //UART0 TX中断标志位置位
    U0CSR |= 0x40;  //允许接收
    IEN0 |= 0x84;   //UART0接收使能
}

/* 串口发送函数 */
void uart0_send_string(char *data, int len)
{
    uint i;

    for (i = 0; i < len; i++)
    {
        U0DBUF = *data++;
        while (UTX0IF == 0);  //发送完毕后置位
        UTX0IF = 0;   //清除标志位
    }
}

/* 进入休眠模式 */
void sys_power_mode(uchar mode)
{
    if (mode > 0 && mode < 4)
    {
        SLEEPCMD |= mode;
        PCON = 0x01;  //进入休眠模式
    }
    else
        PCON = 0x00;  //主动/空闲模式，通过按钮中断触发唤醒
}

/* 设置连接片上温度传感器AD采样 */
#define DISABLE_ALL_INTERRUPTS() (IEN0 = IEN1 = IEN2 = 0x00)//三个
void init_sensor(void)
{
    DISABLE_ALL_INTERRUPTS(); //关闭所有中断

    init_clock();

    TR0 = 0x01; //设置1连接片上温度传感器

    ATEST = 0x01; //使能温度传感器
}

/* 获取温度传感器AD值 */
float get_temperature(void)
{
    uint value;

    ADCCON3 = (0x3E); //内部参考电压，12位ENOB,对片内温度传感器采样
    ADCCON1 |= 0x30;  //启动选择
    ADCCON1 |= 0x40;  //启动AD转化
    while (!(ADCCON1 & 0x80));  //等待AD转化完成
    value = ADCL >> 4; //12位有效数据，选取高4位
    value |= ((uint)ADCH) << 4;

    return (value - 1367.5) / 4.5 - 5;
}

/* 睡眠定时器配置 */
void init_sleep_timer(void)
{
    ST2 = 0x00;
    ST1 = 0x00;
    ST0 = 0x00;
    EA = 1;

    STIE = 1;   //睡眠定时器中断使能 0： 中断禁止     1： 中断使能
    STIF = 0;   //睡眠定时器中断标志 0： 无中断未决   1： 中断未决
}

/* 设置定时睡眠时间 */
void set_st_period(uint sec)
{
    ulong sleepTimer = 0;

    sleepTimer |= ST0;
    sleepTimer |= (ulong)ST1 <<  8;
    sleepTimer |= (ulong)ST2 << 16;
    sleepTimer += ((ulong)sec * (ulong)32768);
    ST2 = (uchar)(sleepTimer >> 16);
    ST1 = (uchar)(sleepTimer >> 8);
    ST0 = (uchar) sleepTimer;
}

/* 看门狗配置 */
void init_watchdog(void)
{
    WDCTL = 0x00; //打开IDLE才能设置看门狗
    WDCTL |= 0x08;  //开启看门狗,定时1s间隔
}
/* 喂狗 */
void feetdog(void)
{
    WDCTL = 0xa0;//当写入0xa0后继续写入0x50，定时器被清除
    WDCTL = 0x50;
    LED2 = 0;
}

/* 设置led灯亮度，延时模拟pwm */
void Delay(unsigned int t)
{
    while (--t);
}

void led_event(uint bright, int cycle)
{
    LED1 = 0;
    Delay(bright);
    LED1 = 1;
    Delay(cycle - bright);
}

/****************************************************************************/

/* 串口接收中断服务函数 */
#pragma vector = URX0_VECTOR
__interrupt void UART0_ISR(void)
{
    URX0IF = 0;   //清中断标志
    uart0_temp = U0DBUF;
}

/* TIM3定时溢出中断服务函数 */
#pragma vector = T3_VECTOR
__interrupt void T2_ISR(void)
{
    IRCON = 0x00; //清除中断标志

    if (count++ > 244)  //大约0.5秒LED翻转一次
    {
        count = 0;
        LED1 = ~LED1;
    }
}

/* P0输入触发中断服务函数 */
#pragma vector = P0INT_VECTOR
__interrupt void P0_ISR(void)
{
    if (P0IFG > 0)
    {
        P0IFG = 0;  //清中断标志
    }

    P0IF = 0;   //清端口0中断标志
    sys_power_mode(4); //正常工作
}

/****************************************************************************
* 程序入口函数
****************************************************************************/
void main(void)
{
    /* 外设配置 */

    while (1)//主循环
    {
    }
}