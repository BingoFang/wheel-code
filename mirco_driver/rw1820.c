#include "RW1820.h"
#include "delay.h"

/* 本驱动适用用stm32，时钟频率为72MHZ */
/* 
Single-Line器件的通信工作时钟 T 由配置寄存器的分频系数来设定。
即 T=3x[S2:S0+1]x1.0us，S2:S0默认为4，T为15us。
*/
//复位RW1820
void RW1820_Rst(void)
{
    RW1820_IO_OUT(); //OUTPUT
    RW1820_DQ_OUT=0; //拉低DQ 480~640us
    delay_us(500);    //复位脉冲
    RW1820_DQ_OUT=1; //DQ=1 
    delay_us(30);     //保持高电平2T
}
//等待RW1820的回应
//返回1:未检测到RW1820的存在
//返回0:存在
u8 RW1820_Check(void)     
{
    u8 retry=0;
    RW1820_IO_IN(); 
    while (RW1820_DQ_IN&&retry<120) //8T的应答脉冲
    {
        retry++;
        delay_us(1);
    };
    if(retry>=120) return 1;
    
    RW1820_IO_OUT();
    RW1820_DQ_OUT=1;
    delay_us(120);  //释放总线保持>6T
    return 0;
}
//写一个字节到RW1820
//dat：要写入的字节
void RW1820_Write_Byte(u8 dat)     
{
    u8 j;
    u8 testb;
    RW1820_IO_OUT();
    for (j=1;j<=8;j++) 
    {
        testb=dat&0x01;
        dat=dat>>1;
        /* 单总线器件在2T时刻采样总线电平 */
        if(testb) 
        {
            RW1820_DQ_OUT=0;// Write 1
            delay_us(2);     //>1us                      
            RW1820_DQ_OUT=1;
            delay_us(80);    //写时隙>4T        
        }else 
        {
            RW1820_DQ_OUT=0;// Write 0
            delay_us(80);    //写时隙>4T       
            RW1820_DQ_OUT=1;
            delay_us(2);     //>1us                  
        }
        delay_us(5);         //恢复时间>3us
    }
}
//从RW1820读取一个位
//返回值：1/0
u8 RW1820_Read_Bit(void) 
{
    u8 data;
    RW1820_IO_OUT();
    RW1820_DQ_OUT=0; 
    delay_us(2);     //>1us
    RW1820_DQ_OUT=1;//释放总线
    RW1820_IO_IN();
    if(RW1820_DQ_IN) data=1;//单总线器件保持2T自动释放总线
    else data=0;     
  
    delay_us(80);   //读时隙>4T，含恢复时间>3us
    
    return data;
}
//从RW1820读取一个字节
//返回值：读到的数据
u8 RW1820_Read_Byte(void)    // read one byte
{
    u8 i,j,dat;
    dat=0;
    for (i=1;i<=8;i++) 
    {
        j=RW1820_Read_Bit();
        dat=(j<<7)|(dat>>1);
    }                           
    return dat;
}
     
//开始温度转换
void RW1820_Start(void)// ds1820 start convert
{                                          
    RW1820_Rst();     
    RW1820_Check();     
    RW1820_Write_Byte(0xcc);// skip rom
    RW1820_Write_Byte(0x44);// convert
}

//初始化RW1820的IO口 DQ 同时检测DS的存在
//返回1:不存在
//返回0:存在         
u8 RW1820_Init(void)
{
    GPIO_InitTypeDef GPIO_Initure;
    __HAL_RCC_GPIOB_CLK_ENABLE();           //开启GPIB时钟
    
    GPIO_Initure.Pin=GPIO_PIN_9;            //PB9
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  //推挽输出
    GPIO_Initure.Pull=GPIO_PULLUP;          //上拉
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //高速
    HAL_GPIO_Init(GPIOB,&GPIO_Initure);     //初始化
 
    RW1820_Rst();
    return RW1820_Check();
}

//从RW1820得到温度值.初始读到85，为上电默认值
//精度：0.1C
//返回值：温度值 （-550~1250） 
short RW1820_Get_Temp(void)
{
    u8 temp;
    u8 TL,TH;
    short tem;
    RW1820_Start ();           //开始转换
    RW1820_Rst();
    RW1820_Check();     
    RW1820_Write_Byte(0xcc);   // skip rom
    RW1820_Write_Byte(0xbe);   // convert      
    TL=RW1820_Read_Byte();     // LSB   
    TH=RW1820_Read_Byte();     // MSB   
    if(TH>7)
    {
        TH=~TH;
        TL=~TL; 
        temp=0;//温度为负  
    }else temp=1;//温度为正       
    tem=TH; //获得高八位
    tem<<=8;    
    tem+=TL;//获得底八位
    tem=(double)tem*0.625;//转换     
    if(temp)return tem; //返回温度值
    else return -tem;    
}