#include "RW1820.h"
#include "delay.h"

/* ������������stm32��ʱ��Ƶ��Ϊ72MHZ */
/* 
Single-Line������ͨ�Ź���ʱ�� T �����üĴ����ķ�Ƶϵ�����趨��
�� T=3x[S2:S0+1]x1.0us��S2:S0Ĭ��Ϊ4��TΪ15us��
*/
//��λRW1820
void RW1820_Rst(void)
{
    RW1820_IO_OUT(); //OUTPUT
    RW1820_DQ_OUT=0; //����DQ 480~640us
    delay_us(500);    //��λ����
    RW1820_DQ_OUT=1; //DQ=1 
    delay_us(30);     //���ָߵ�ƽ2T
}
//�ȴ�RW1820�Ļ�Ӧ
//����1:δ��⵽RW1820�Ĵ���
//����0:����
u8 RW1820_Check(void)     
{
    u8 retry=0;
    RW1820_IO_IN(); 
    while (RW1820_DQ_IN&&retry<120) //8T��Ӧ������
    {
        retry++;
        delay_us(1);
    };
    if(retry>=120) return 1;
    
    RW1820_IO_OUT();
    RW1820_DQ_OUT=1;
    delay_us(120);  //�ͷ����߱���>6T
    return 0;
}
//дһ���ֽڵ�RW1820
//dat��Ҫд����ֽ�
void RW1820_Write_Byte(u8 dat)     
{
    u8 j;
    u8 testb;
    RW1820_IO_OUT();
    for (j=1;j<=8;j++) 
    {
        testb=dat&0x01;
        dat=dat>>1;
        /* ������������2Tʱ�̲������ߵ�ƽ */
        if(testb) 
        {
            RW1820_DQ_OUT=0;// Write 1
            delay_us(2);     //>1us                      
            RW1820_DQ_OUT=1;
            delay_us(80);    //дʱ϶>4T        
        }else 
        {
            RW1820_DQ_OUT=0;// Write 0
            delay_us(80);    //дʱ϶>4T       
            RW1820_DQ_OUT=1;
            delay_us(2);     //>1us                  
        }
        delay_us(5);         //�ָ�ʱ��>3us
    }
}
//��RW1820��ȡһ��λ
//����ֵ��1/0
u8 RW1820_Read_Bit(void) 
{
    u8 data;
    RW1820_IO_OUT();
    RW1820_DQ_OUT=0; 
    delay_us(2);     //>1us
    RW1820_DQ_OUT=1;//�ͷ�����
    RW1820_IO_IN();
    if(RW1820_DQ_IN) data=1;//��������������2T�Զ��ͷ�����
    else data=0;     
  
    delay_us(80);   //��ʱ϶>4T�����ָ�ʱ��>3us
    
    return data;
}
//��RW1820��ȡһ���ֽ�
//����ֵ������������
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
     
//��ʼ�¶�ת��
void RW1820_Start(void)// ds1820 start convert
{                                          
    RW1820_Rst();     
    RW1820_Check();     
    RW1820_Write_Byte(0xcc);// skip rom
    RW1820_Write_Byte(0x44);// convert
}

//��ʼ��RW1820��IO�� DQ ͬʱ���DS�Ĵ���
//����1:������
//����0:����         
u8 RW1820_Init(void)
{
    GPIO_InitTypeDef GPIO_Initure;
    __HAL_RCC_GPIOB_CLK_ENABLE();           //����GPIBʱ��
    
    GPIO_Initure.Pin=GPIO_PIN_9;            //PB9
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  //�������
    GPIO_Initure.Pull=GPIO_PULLUP;          //����
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //����
    HAL_GPIO_Init(GPIOB,&GPIO_Initure);     //��ʼ��
 
    RW1820_Rst();
    return RW1820_Check();
}

//��RW1820�õ��¶�ֵ.��ʼ����85��Ϊ�ϵ�Ĭ��ֵ
//���ȣ�0.1C
//����ֵ���¶�ֵ ��-550~1250�� 
short RW1820_Get_Temp(void)
{
    u8 temp;
    u8 TL,TH;
    short tem;
    RW1820_Start ();           //��ʼת��
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
        temp=0;//�¶�Ϊ��  
    }else temp=1;//�¶�Ϊ��       
    tem=TH; //��ø߰�λ
    tem<<=8;    
    tem+=TL;//��õװ�λ
    tem=(double)tem*0.625;//ת��     
    if(temp)return tem; //�����¶�ֵ
    else return -tem;    
}