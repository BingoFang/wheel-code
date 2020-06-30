void USART3_IRQHandler(void)
{
	uint8_t sensor_data;
	if(USART_GetITSstatus(USART3,USART_IT_RXNE))
	{
		USART_ClearITPendingBit(USART3,USART_IT_RXNE);
		
		sensor_data = USART3->DR;
		
		Slaver_Data_Receive(sensor_data);
	}
}

/*写法思路：单个字节依次检测,接收响应数据*/
void Slaver_Data_Receive(uint8_t sensor_data)
{
	static uint8_t RxBuffer[30];
	static uint8_t _data_len = 0,_data_cnt = 0;
	static uint8_t state = 0;
	
	if(state == 0 && sensor_data == TxBuffer[0])			//设备地址
	{
		state = 1;
		RxBuffer[0] = sensor_data;
	}
	else if(state == 1 && sensor_data == TxBuffer[1]) //功能码
	{
		state = 2;
	  	RxBuffer[1] = sensor_data;	
	}
	else if(state == 2 && sensor_data < 20)  //数据N字节
	{
		state = 3;
		RxBuffer[2] = sensor_data;
		_data_len = sensor_data - 5;	//减去head function len crc
		_data_cnt = 0;
	}
	else if(state == 3 && _data_len > 0) 	   //传感器数据
	{
		_data_len--;
		RxBuffer[3 + _data_cnt++] = sensor_data;
		if(_data_len == 0)
		state = 4;
	}
	else if(state == 4)     								 //CRC校验位 低八位
  	{
	  	RxBuffer[3 + _data_cnt++] = sensor_data;
	  	state = 5;
  	}
  	else if(state == 5)											//CRC校验位 高八位
  	{
  		state = 0;
  		RxBuffer[3 + _data_cnt++] = sensor_data;
  		Slaver_Data_Prase(RxBuffer,3 + _data_cnt );
  	}
  	else
  		state = 0;
}

void Slaver_Data_Prase(uint8_t *data_buf,uint8_t num)
{
  master_tx_calcrc = CRC_Compute(data_buf,num - 2);
  master_rx_calcrc = (u16)(*(data_buf + num - 1) << 8) | *(data_buf + num - 2);
  if(!(master_tx_calcrc == master_rx_calcrc)) return;
  
  receive_reg_len = *(data_buf + 2) >> 1;  		//接收传感器数据寄存器
  
  for(uint8_t receive_cnt = 0;receive_cnt < receive_reg_len;receive_cnt++)
  {
	  switch(*data_buf)      					//根据设备地址判断
	  {
	  	case AIR_SENSOR_ADDR:
	  	*(Air_ReadReg + receive_cnt++) = (u16)(*(data_buf + 3 + receive_cnt * 2 << 8)) | (*(data_buf + 4 + receive_cnt * 2));
	  	break;
	  	case FLOW_METER_ADDR:
	  	*(Flow_ReadReg + receive_cnt++) = (u16)(*(data_buf + 3 + receive_cnt * 2 << 8)) | (*(data_buf + 4 + receive_cnt * 2));
	  	break;
	  	case TANK_SENSOR_ADDR:
	  	*(Tank_ReadReg + receive_cnt++) = (u16)(*(data_buf + 3 + receive_cnt * 2 << 8)) | (*(data_buf + 4 + receive_cnt * 2));
	  	break;
	  	default:
	  	break;
	  }
	}
}
