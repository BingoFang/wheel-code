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
	I2C写过程：Start信号->I2C发送(七位地址位+0（W）)->等待应答信号（有应答表明从设备收到，非应答则停止发送）
  	->I2C发送一个字节(数据)->等待应答信号->Stop信号    
  I2C读过程：Start信号->IIC发送(七位地址位+0（W）)->等待应答信号->Staty信号->IIC发送(七位地址位+1（R）)->等待应答信号->读取一个字节数据发送应答信号（如果就一个字节不想读了发送一个非应答信号）->Stop信号
  注：主机每写一个字节都要等待从机是否有应答信号，数据采集在时钟高电平进行。
  
  SPI写过程：先写入8bits（0（W）+七位地址位），根据SDIO的高低电平开始写入16位数据。
  SPI读过程：先写入8bits（1（R）+七位地址位），根据SDIO的高低电平开始读出16位数据。
  注：spi操作写入和读出是靠沿采样，根据不同IC的datasheet编写时序。
*/
 
 /****************列举读取1846s的spi读写过程*******************************************/ 
 unsigned int 1846sRead(unsigned char regaddr)
{
	unsigned char i = 8;
	unsigend char read_data = 0;
	
	regaddr |= 0x80;                  //R(1)，最高位为读写位
	SET_SDIO_OUT();    
	SET_SCK_LOW();		                // 时钟线拉低时开始写数据
	SET_NSS_ENABLE();                 // 片选使能
	do
	{
		if((regaddr & 0x80) == 0)       //写入8bits 地址
			SET_SDIO_LOW();
		else
			SET_SDIO_HIGH();
		regaddr <<= 1;
		NopDelay();
		SET_SCK_HIGH();
		NopDelay();
		SET_SCK_LOW();
		NopDelay();
	}while(--i > 0);
	
	i = 16;
	SET_SDIO_IN();  				 	        //准备读出16bits 数据
	NopDelay();
	SET_SCK_HIGH();                   //时钟线拉高时，读数据
	NopDelay();
	do
	{
		read_data <<= 1;
		if(IS_SET_SDIO_HIGH())         //读取数据从高位到低位
			read_data |= 0x0001;    
		SET_SCK_LOW();
		NopDelay();
		SET_SCK_HIGH();
		NopDelay();
	}while(--i > 0);
	
	SET_SCK_LOW();
	NopDelay();
	SET_NSS_DISABLE();       	       //片选失能
	NopDelay();          
	return read_data;
}

void 1846sWrite(unsigned char regaddr,unsigned int data)
{
	unsigned char i = 8;
	regaddr &= 0x7F;   //w(0)
	 
	SET_SCK_LOW();         		      //时钟线低开始写数据
	SET_NSS_ENABLE();
	SET_SDIO_OUT();
	
	//write 8 bits regaddr
	do
	{
			if((regaddr & 0x80) == 0)   //写数据也是从高位到低位
					SET_SDIO_LOW();
			else
					SET_SDIO_HIGH();
			regaddr <<= 1
			SET_SCK_HIGH();
			NopDelay();
			SET_SCK_LOW();
			NopDelay();
	}while(--i > 0);
	
	//write 16 bits data
	i = 16;
	NopDelay();
	SET_SCL_LOW();					        //时钟线低开始写数据
	do
	{
		if((data & 0x8000) == 0) 
			SET_SDIO_LOW();				
		else
			SET_SDIO_HIGH();
		data <<= 1;
		NopDelay();
		SET_SCK_HIGH();
		NopDelay();
		SET_SCK_LOW();
		NopDelay();
	}while(--i > 0);
	
	SET_NSS_DISABLE();              //片选失能
}