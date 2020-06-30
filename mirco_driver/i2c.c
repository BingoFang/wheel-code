/**********************************************************************
下面主要总结一下spi和iic两种总线的异同点：

1 iic总线不是全双工，2根线SCL SDA。spi总线实现全双工，4根线SCK CS MOSI MISO。

2 iic总线是多主机总线，通过SDA上的地址信息来锁定从设备。spi总线只有一个主设备，主设备通过CS片选来确定从设备。

3 iic总线传输速度在100kbps-4Mbps。spi总线传输速度更快，可以达到30MHZ以上。

4 iic总线空闲状态下SDA SCL都是高电平。spi总线空闲状态MOSI MISO也都是 SCK是由CPOL决定的。

5 iic总线scl高电平时sda下降沿标志传输开始，上升沿标志传输结束。spi总线cs拉低标志传输开始，cs拉高标志传输结束。

6 iic总线是SCL高电平采样。spi总线因为是全双工，因此是沿采样，具体要根据CPHA决定。一般情况下master device是SCK的上升沿发送，下降沿采集。

7 iic总线和spi总线数据传输都是MSB在前，LSB在后（串口是LSB在前）。

8 iic总线和spi总线时钟都是由主设备产生，并且只在数据传输时发出时钟。

9 iic总线读写时序比较固定统一，设备驱动编写方便。spi总线不同从设备读写时序差别比较大，因此必须根据具体的设备datasheet来实现读写，相对复杂一些。
************************************************************************/
/*
	IIC写过程：Start信号->IIC发送(七位地址位+0（W）)->等待应答信号（有应答表明从设备收到，非应答则停止发送）
  	->IIC发送一个字节(数据)->等待应答信号->Stop信号    
  IIC读过程：Start信号->IIC发送(七位地址位+0（W）)->等待应答信号->Staty信号->IIC发送(七位地址位+1（R）)->等待应答信号->读取一个字节数据发送应答信号（如果就一个字节不想读了发送一个非应答信号）->Stop信号
  注：主机每写一个字节都要等待从机是否有应答信号，数据采集在时钟高电平进行。
  
  SPI写过程：先写入8bits（0（W）+七位地址位），根据SDIO的高低电平开始写入16位数据。
  SPI读过程：先写入8bits（1（R）+七位地址位），根据SDIO的高低电平开始读出16位数据。
  注：spi操作写入和读出是靠沿采样，根据不同IC的datasheet编写时序。
*/


/***************************IIC_DRIVER适用于所有iic通讯设备，亲测可行*****************************/
/***
注意点：1、SDA，SCL配置为推挽输出，在通讯中需要切换SDA方向。如果配置为开漏输出，外部需要加上拉电阻，就不需要切换SDA方向。
			  2、该IIC通讯使用的是模拟方式，根据不同IO口修改引脚定义
				3、延时时间可能会导致IIC通讯不成功的一个因素
***/
//IO方向设置
#define SDA_IN()  {GPIOB->CRH&=0XFFFF0FFF;GPIOB->CRH|=8<<12;}
#define SDA_OUT() {GPIOB->CRH&=0XFFFF0FFF;GPIOB->CRH|=3<<12;}

//IO操作函数
#define IIC_SCL    PBout(10) //SCL
#define IIC_SDA    PBout(11) //SDA	 
#define READ_SDA   PBin(11)  //读SDA引脚

//初始化IIC
void IIC_Init(void)
{					     
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOB, ENABLE );	
	   
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10|GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;   //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_SetBits(GPIOB,GPIO_Pin_10|GPIO_Pin_11); 				//PB10,PB11 输出高
  DS1307_SetRtc(Set_Rtc_Code);
}

//产生IIC起始信号
void IIC_Start(void)
{
	SDA_OUT();     //sda线输出
	IIC_SDA=1;	  	  
	IIC_SCL=1;
	delay_us(4);
 	IIC_SDA=0;//START:when CLK is high,DATA change form high to low 
	delay_us(4);
	IIC_SCL=0;//钳住IIC总线，准备发送或接收数据 
}	  

//产生IIC停止信号
void IIC_Stop(void)
{
	SDA_OUT();//sda线输出
	IIC_SCL=0;
	IIC_SDA=0;//STOP:when CLK is high DATA change form low to high
 	delay_us(4);
	IIC_SCL=1; 
	IIC_SDA=1;//发送IIC总线结束信号
	delay_us(4);							   	
}

//等待应答信号到来
//返回值：1，接收应答失败
//        0，接收应答成功
u8 IIC_Wait_Ack(void)
{
	u8 ucErrTime=0;
	SDA_IN();      //SDA设置为输入  
	IIC_SDA=1;delay_us(1);	   
	IIC_SCL=1;delay_us(1);	 
	while(READ_SDA)
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			IIC_Stop();
			return 1;
		}
	}
	IIC_SCL=0;//时钟输出0 	   
	return 0;  
} 

//产生ACK应答
void IIC_Ack(void)
{
	IIC_SCL=0;
	SDA_OUT();
	IIC_SDA=0;
	delay_us(2);
	IIC_SCL=1;
	delay_us(2);
	IIC_SCL=0;
}

//不产生ACK应答		    
void IIC_NAck(void)
{
	IIC_SCL=0;
	SDA_OUT();
	IIC_SDA=1;
	delay_us(2);
	IIC_SCL=1;
	delay_us(2);
	IIC_SCL=0;
}				
	 				     
//IIC发送一个字节
//返回从机有无应答
//1，有应答
//0，无应答			  
void IIC_Send_Byte(u8 txd)
{                        
  u8 t;   
	SDA_OUT(); 	    
  IIC_SCL=0;//拉低时钟开始数据传输
  for(t=0;t<8;t++)
  {              
    //IIC_SDA=(txd&0x80)>>7;
		if((txd&0x80)>>7)
			IIC_SDA=1;
		else
			IIC_SDA=0;
		txd<<=1; 	  
		delay_us(2);   //对TEA5767这三个延时都是必须的
		IIC_SCL=1;
		delay_us(2); 
		IIC_SCL=0;	
		delay_us(2);
  }	 
} 	   
 
//读1个字节，ack=1时，发送ACK，ack=0，发送nACK   
u8 IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	SDA_IN();//SDA设置为输入
  for(i=0;i<8;i++ )
	{
    IIC_SCL=0; 
    delay_us(2);
		IIC_SCL=1;
    receive<<=1;
    if(READ_SDA)receive++;   
		delay_us(1); 
  }					 
    if (!ack)
     IIC_NAck();//发送nACK
    else
     IIC_Ack(); //发送ACK   
  return receive;
}

/************************针对不同iic通讯设备的读写时序存在差异，详细见器件数据手册**************************************/
//这里列举了RDA5820读写操作过程
u16 RDA5820_Read_Reg(u8 addr)
{
		u16 res;
		IIC_Start();
		IIC_Send_Byte(RDA5820_write); //发送写命令
		IIC_Wait_Ack();
		IIC_Send_Byte(RDA5820_addr);
		IIC_Wait_Ack();
		
		IIC_Start();
		IIC_Send_Byte(RDA5820_read);  //发送读命令
		IIC_Wait_Ack();
		res = IIC_Read_Byte(1); //read high 8 byte ,send ack
		res <<= 8;
		res |= IIC_Read_Byte(0); //read low 8 byte ,send nack
		IIC_Stop();
		return res;
}

void RDA5820_Write_Reg(u8 addr,u16 data)
{
		IIC_Start();
		IIC_Send_Byte(RDA5820_write);  //发送写命令
		IIC_Wait_Ack();
		IIC_Send_Byte(addr); 
		IIC_Wait_Ack();
		IIC_Send_Byte(data >> 8);   //high byte
		IIC_Wait_Ack();
		IIC_Send_Byte(data & 0xff); //low byte
		IIC_Wait_Ack();
		IIC_Stop();
}