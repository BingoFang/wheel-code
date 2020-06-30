/**********************************************************************
������Ҫ�ܽ�һ��spi��iic�������ߵ���ͬ�㣺

1 iic���߲���ȫ˫����2����SCL SDA��spi����ʵ��ȫ˫����4����SCK CS MOSI MISO��

2 iic�����Ƕ��������ߣ�ͨ��SDA�ϵĵ�ַ��Ϣ���������豸��spi����ֻ��һ�����豸�����豸ͨ��CSƬѡ��ȷ�����豸��

3 iic���ߴ����ٶ���100kbps-4Mbps��spi���ߴ����ٶȸ��죬���Դﵽ30MHZ���ϡ�

4 iic���߿���״̬��SDA SCL���Ǹߵ�ƽ��spi���߿���״̬MOSI MISOҲ���� SCK����CPOL�����ġ�

5 iic����scl�ߵ�ƽʱsda�½��ر�־���俪ʼ�������ر�־���������spi����cs���ͱ�־���俪ʼ��cs���߱�־���������

6 iic������SCL�ߵ�ƽ������spi������Ϊ��ȫ˫����������ز���������Ҫ����CPHA������һ�������master device��SCK�������ط��ͣ��½��زɼ���

7 iic���ߺ�spi�������ݴ��䶼��MSB��ǰ��LSB�ں󣨴�����LSB��ǰ����

8 iic���ߺ�spi����ʱ�Ӷ��������豸����������ֻ�����ݴ���ʱ����ʱ�ӡ�

9 iic���߶�дʱ��ȽϹ̶�ͳһ���豸������д���㡣spi���߲�ͬ���豸��дʱ����Ƚϴ���˱�����ݾ�����豸datasheet��ʵ�ֶ�д����Ը���һЩ��
************************************************************************/
/*
	IICд���̣�Start�ź�->IIC����(��λ��ַλ+0��W��)->�ȴ�Ӧ���źţ���Ӧ��������豸�յ�����Ӧ����ֹͣ���ͣ�
  	->IIC����һ���ֽ�(����)->�ȴ�Ӧ���ź�->Stop�ź�    
  IIC�����̣�Start�ź�->IIC����(��λ��ַλ+0��W��)->�ȴ�Ӧ���ź�->Staty�ź�->IIC����(��λ��ַλ+1��R��)->�ȴ�Ӧ���ź�->��ȡһ���ֽ����ݷ���Ӧ���źţ������һ���ֽڲ�����˷���һ����Ӧ���źţ�->Stop�ź�
  ע������ÿдһ���ֽڶ�Ҫ�ȴ��ӻ��Ƿ���Ӧ���źţ����ݲɼ���ʱ�Ӹߵ�ƽ���С�
  
  SPIд���̣���д��8bits��0��W��+��λ��ַλ��������SDIO�ĸߵ͵�ƽ��ʼд��16λ���ݡ�
  SPI�����̣���д��8bits��1��R��+��λ��ַλ��������SDIO�ĸߵ͵�ƽ��ʼ����16λ���ݡ�
  ע��spi����д��Ͷ����ǿ��ز��������ݲ�ͬIC��datasheet��дʱ��
*/


/***************************IIC_DRIVER����������iicͨѶ�豸���ײ����*****************************/
/***
ע��㣺1��SDA��SCL����Ϊ�����������ͨѶ����Ҫ�л�SDA�����������Ϊ��©������ⲿ��Ҫ���������裬�Ͳ���Ҫ�л�SDA����
			  2����IICͨѶʹ�õ���ģ�ⷽʽ�����ݲ�ͬIO���޸����Ŷ���
				3����ʱʱ����ܻᵼ��IICͨѶ���ɹ���һ������
***/
//IO��������
#define SDA_IN()  {GPIOB->CRH&=0XFFFF0FFF;GPIOB->CRH|=8<<12;}
#define SDA_OUT() {GPIOB->CRH&=0XFFFF0FFF;GPIOB->CRH|=3<<12;}

//IO��������
#define IIC_SCL    PBout(10) //SCL
#define IIC_SDA    PBout(11) //SDA	 
#define READ_SDA   PBin(11)  //��SDA����

//��ʼ��IIC
void IIC_Init(void)
{					     
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOB, ENABLE );	
	   
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10|GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;   //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_SetBits(GPIOB,GPIO_Pin_10|GPIO_Pin_11); 				//PB10,PB11 �����
  DS1307_SetRtc(Set_Rtc_Code);
}

//����IIC��ʼ�ź�
void IIC_Start(void)
{
	SDA_OUT();     //sda�����
	IIC_SDA=1;	  	  
	IIC_SCL=1;
	delay_us(4);
 	IIC_SDA=0;//START:when CLK is high,DATA change form high to low 
	delay_us(4);
	IIC_SCL=0;//ǯסIIC���ߣ�׼�����ͻ�������� 
}	  

//����IICֹͣ�ź�
void IIC_Stop(void)
{
	SDA_OUT();//sda�����
	IIC_SCL=0;
	IIC_SDA=0;//STOP:when CLK is high DATA change form low to high
 	delay_us(4);
	IIC_SCL=1; 
	IIC_SDA=1;//����IIC���߽����ź�
	delay_us(4);							   	
}

//�ȴ�Ӧ���źŵ���
//����ֵ��1������Ӧ��ʧ��
//        0������Ӧ��ɹ�
u8 IIC_Wait_Ack(void)
{
	u8 ucErrTime=0;
	SDA_IN();      //SDA����Ϊ����  
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
	IIC_SCL=0;//ʱ�����0 	   
	return 0;  
} 

//����ACKӦ��
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

//������ACKӦ��		    
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
	 				     
//IIC����һ���ֽ�
//���شӻ�����Ӧ��
//1����Ӧ��
//0����Ӧ��			  
void IIC_Send_Byte(u8 txd)
{                        
  u8 t;   
	SDA_OUT(); 	    
  IIC_SCL=0;//����ʱ�ӿ�ʼ���ݴ���
  for(t=0;t<8;t++)
  {              
    //IIC_SDA=(txd&0x80)>>7;
		if((txd&0x80)>>7)
			IIC_SDA=1;
		else
			IIC_SDA=0;
		txd<<=1; 	  
		delay_us(2);   //��TEA5767��������ʱ���Ǳ����
		IIC_SCL=1;
		delay_us(2); 
		IIC_SCL=0;	
		delay_us(2);
  }	 
} 	   
 
//��1���ֽڣ�ack=1ʱ������ACK��ack=0������nACK   
u8 IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	SDA_IN();//SDA����Ϊ����
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
     IIC_NAck();//����nACK
    else
     IIC_Ack(); //����ACK   
  return receive;
}

/************************��Բ�ͬiicͨѶ�豸�Ķ�дʱ����ڲ��죬��ϸ�����������ֲ�**************************************/
//�����о���RDA5820��д��������
u16 RDA5820_Read_Reg(u8 addr)
{
		u16 res;
		IIC_Start();
		IIC_Send_Byte(RDA5820_write); //����д����
		IIC_Wait_Ack();
		IIC_Send_Byte(RDA5820_addr);
		IIC_Wait_Ack();
		
		IIC_Start();
		IIC_Send_Byte(RDA5820_read);  //���Ͷ�����
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
		IIC_Send_Byte(RDA5820_write);  //����д����
		IIC_Wait_Ack();
		IIC_Send_Byte(addr); 
		IIC_Wait_Ack();
		IIC_Send_Byte(data >> 8);   //high byte
		IIC_Wait_Ack();
		IIC_Send_Byte(data & 0xff); //low byte
		IIC_Wait_Ack();
		IIC_Stop();
}