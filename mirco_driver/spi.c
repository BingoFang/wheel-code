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
	I2Cд���̣�Start�ź�->I2C����(��λ��ַλ+0��W��)->�ȴ�Ӧ���źţ���Ӧ��������豸�յ�����Ӧ����ֹͣ���ͣ�
  	->I2C����һ���ֽ�(����)->�ȴ�Ӧ���ź�->Stop�ź�    
  I2C�����̣�Start�ź�->IIC����(��λ��ַλ+0��W��)->�ȴ�Ӧ���ź�->Staty�ź�->IIC����(��λ��ַλ+1��R��)->�ȴ�Ӧ���ź�->��ȡһ���ֽ����ݷ���Ӧ���źţ������һ���ֽڲ�����˷���һ����Ӧ���źţ�->Stop�ź�
  ע������ÿдһ���ֽڶ�Ҫ�ȴ��ӻ��Ƿ���Ӧ���źţ����ݲɼ���ʱ�Ӹߵ�ƽ���С�
  
  SPIд���̣���д��8bits��0��W��+��λ��ַλ��������SDIO�ĸߵ͵�ƽ��ʼд��16λ���ݡ�
  SPI�����̣���д��8bits��1��R��+��λ��ַλ��������SDIO�ĸߵ͵�ƽ��ʼ����16λ���ݡ�
  ע��spi����д��Ͷ����ǿ��ز��������ݲ�ͬIC��datasheet��дʱ��
*/
 
 /****************�оٶ�ȡ1846s��spi��д����*******************************************/ 
 unsigned int 1846sRead(unsigned char regaddr)
{
	unsigned char i = 8;
	unsigend char read_data = 0;
	
	regaddr |= 0x80;                  //R(1)�����λΪ��дλ
	SET_SDIO_OUT();    
	SET_SCK_LOW();		                // ʱ��������ʱ��ʼд����
	SET_NSS_ENABLE();                 // Ƭѡʹ��
	do
	{
		if((regaddr & 0x80) == 0)       //д��8bits ��ַ
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
	SET_SDIO_IN();  				 	        //׼������16bits ����
	NopDelay();
	SET_SCK_HIGH();                   //ʱ��������ʱ��������
	NopDelay();
	do
	{
		read_data <<= 1;
		if(IS_SET_SDIO_HIGH())         //��ȡ���ݴӸ�λ����λ
			read_data |= 0x0001;    
		SET_SCK_LOW();
		NopDelay();
		SET_SCK_HIGH();
		NopDelay();
	}while(--i > 0);
	
	SET_SCK_LOW();
	NopDelay();
	SET_NSS_DISABLE();       	       //Ƭѡʧ��
	NopDelay();          
	return read_data;
}

void 1846sWrite(unsigned char regaddr,unsigned int data)
{
	unsigned char i = 8;
	regaddr &= 0x7F;   //w(0)
	 
	SET_SCK_LOW();         		      //ʱ���ߵͿ�ʼд����
	SET_NSS_ENABLE();
	SET_SDIO_OUT();
	
	//write 8 bits regaddr
	do
	{
			if((regaddr & 0x80) == 0)   //д����Ҳ�ǴӸ�λ����λ
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
	SET_SCL_LOW();					        //ʱ���ߵͿ�ʼд����
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
	
	SET_NSS_DISABLE();              //Ƭѡʧ��
}