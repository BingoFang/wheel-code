float SHT2x_GetTempPoll(void)
{
    float TEMP;
    u8 ack, tmp1,tmp2,tmp3;
    u16 ST;
    u16 i=0;

    i2c_master_start();                             //发送IIC开始信号
    i2c_master_writeByte(SHT20_SlaveAddress+0);     //IIC发送一个字节
    if(!i2c_master_checkAck())
    {
        i2c_master_stop();
        return 1;
    }

    i2c_master_writeByte(SHT20_T_MEASURE_NOHOLD_MASTER);
    if(!i2c_master_checkAck())
    {
        i2c_master_stop();
        return 1;
    }

    iic_delay_ms(100);                          //等待测量完成延时

    do {
        i2c_master_start();                     //发送IIC开始信号

        i2c_master_writeByte(SHT20_SlaveAddress+1);

        i++;
        if(i==1000)break;
    } while(!i2c_master_checkAck());            //等待SDA应答表明内部测量结束

    tmp1 = i2c_master_readByte();
    i2c_master_send_ack();

    tmp2 = i2c_master_readByte();
    i2c_master_send_ack();

    tmp3 = i2c_master_readByte();
    i2c_master_send_nack();

    i2c_master_stop();

    os_printf("temp1:0x%x\r\n",tmp1);
    os_printf("temp2:0x%x\r\n",tmp2);
    os_printf("temp3:0x%x\r\n",tmp3);

    ST = (tmp1 << 8) | (tmp2 << 0);

    ST &= ~0x0003;
    TEMP = ((float)ST * 0.00268127) - 46.85;

    return (TEMP);
}

float SHT2x_GetHumiPoll(void)
{
    float HUMI;
    u8 ack, tmp1,tmp2,tmp3;
    u16 SRH;
    u16 i=0;

    i2c_master_start();                             //发送IIC开始信号

    i2c_master_writeByte(SHT20_SlaveAddress+0);     //IIC发送一个字节
    if(!i2c_master_checkAck())
    {
        i2c_master_stop();
        return 1;
    }

    i2c_master_writeByte(SHT20_RH_MEASURE_NOHOLD_MASTER);
    if(!i2c_master_checkAck())
    {
        i2c_master_stop();
        return 1;
    }

    iic_delay_ms(100);

    do {
       i2c_master_start();                              //发送IIC开始信号

       i2c_master_writeByte(SHT20_SlaveAddress+1);

       os_printf("noAck Cnt:%d\r\n",i);
       i++;
       if(i==100)break;
    } while(!i2c_master_checkAck());

    tmp1 = i2c_master_readByte();
    i2c_master_send_ack();

    tmp2 = i2c_master_readByte();
    i2c_master_send_ack();

    tmp3 = i2c_master_readByte();
    i2c_master_send_nack();

    i2c_master_stop();

    os_printf("Humi1:0x%x\r\n",tmp1);
    os_printf("Humi2:0x%x\r\n",tmp2);
    os_printf("Humi3:0x%x\r\n",tmp3);

    SRH = (tmp1 << 8) | (tmp2 << 0);
    SRH &= ~0x0003;
    HUMI = ((float)SRH * 0.00190735) - 6;

    return (HUMI);
}