/* �����Լ�� */
int gcd(int m,int n)
{
	int r;
	if(m <= 0 || n <= 0)
		return 0;
	r = m % n;
	return r > 0 ? gcd(n,r):n;
}

/* ���ö�����ת����ASCII */
void binary_to_ascii(unsigned int data)
{
	unsigned int quotient;
	quotient = data / 10;
	if(quotient != 0)
		binary_to_ascii(quotient);
	putchar(data % 10 + '0');
}

/* ���õ������ַ���ת����һ������ */
int ascii_to_integer(char *string)
{
	int data = 0;
	while(*string >= '0' && *string <= '9')
	{
		data *= 10;
		data += *string - '0';  //���ַ���ת��Ϊ���� - ��0��
		string++;
	}
	if(*string != '\0')  //�������һ���������ַ��ķ���0
		data = 0;
	return data;
}

/***********************************************************
 * �������ƣ�Hex2Str
 * ����������ʮ������ת��Ϊ�ַ���
 * ���������pbDest��������ݣ�pbSrc���������ݣ�nLen���������ݳ���
 * �����������
***********************************************************/
void Hex2Str(uint8_t *pbDest,uint8_t *pbSrc,uint8_t nLen)
{
    uint8_t ddh,ddl;
    uint8_t i;

    for(i = 0;i < nLen;i++)
    {
        ddh = 48 + pbSrc[i] / 16;
        ddl = 48 + pbSrc[i] % 16;
        if(ddh > 57) ddh += 7;
        if(ddl > 57) ddl += 7;
        pbDest[i * 2] = ddh;
        pbDest[i * 2 + 1] = ddl;
    }

    pbDest[nLen * 2] = '\0';
}

/***********************************************************
 * �������ƣ�Byte2Dword
 * ����������һ���ֽ��������������4���ֽ�����
 * ���������pbDest���洢���ݣ�pbSrc���������ݣ�nLen���������ݳ���
 * �����������
***********************************************************/
void Byte2Dword(uint32 *pbDest,uint8_t *pbSrc,uint8_t nlen)
{
    uint8_t i = 0,j = 0;
    uint8_t cnt = 4;
    for(i = 0;i < nlen;i++)
    {
        cnt--;
        *((char *)(pbDest + j) + (cnt)) =  *(pbSrc + i);
        if(!cnt)
        {
            cnt = 4;
            j++;
        }
    }
}

/***********************************************************
 * �������ƣ�Dword2Byte
 * ����������4���ֽ������ֳ�һ���ֽ�����
 * ���������pbDest���洢���ݣ�pbSrc���������ݣ�nLen���������ݳ���
 * �����������
***********************************************************/
void Dword2Byte(uint8_t *pbDest,uint32 *pbSrc,uint8_t len)
{
    uint8_t i = 0,j = 0;
    uint8_t cnt = 4;
    for(i = 0;i < 4 * len;i++)
    {
        cnt--;
        *(pbDest + i) = *((char *)(pbSrc + j) + (cnt));
        if(!cnt)
        {
            cnt = 4;
            j++;
        }
    }
}

/* �ַ���ת��������ֵ */
int Str2Int(char *str)
{
	int value = 0;
	int sign = 1;
	int result = 0;
	if(NULL == str)
	{
		return -1;
	}
	if('-' == *str)
	{
		sign = -1;
		str++;
	}
	while(*str)
	{
		value = value * 10 + *str - '0';
		str++;
	}
	result = sign * value;
	return result;
}

/* Ѱ�����ֵ��������һ����β��Ϊ��ֵ���� */
int max_list(int  string[])
{
	int i;
	int max = string[0];
	for(i = 1;i < 4;i++)
		if(string[i] > 0)
		{
			if(string[i] > max)
			max = string[i];
		}
	   else        //��������βΪ��ֵʱ����ѭ�����������ֵmax
		return max;
}


/* ȥ��ֵ��ƽ��ֵ */
u16 deex_average_filter(u16 *data,uint8_t cnt)
{
	uint8_t i,j;
	u16 temp,temp_sum = 0;
	for(i = 0;i < cnt - 1;i++)
	{
		for(j = i + 1;j < cnt;j++)
		{
			if(data[i] > data[j])
			{
				temp = data[i];
				data[i] = data[j];
				data[j] = temp;
			}
		}
	}
	for(i = 1;i < cnt - 1;i++)
	{
		temp_sum += data[i];
	}
	return temp_sum / (cnt - 2);
}

/* ������ֵ�˲� */
u16 Filter_Buf[100] = {0};    
float PH_data[10];    			 
float move_filter(float data,uint8_t Len)
{
	float Filter_Sum=0;
	for (uint8_t i = 0; i < Len - 1; i++) 
	{
		Filter_Buf[i]=Filter_Buf[i+1];
	}
	Filter_Buf[Len-1]=data;
	for (uint8_t i = 0; i < Len; i++) 
	{
		Filter_Sum += Filter_Buf[i];
	}
	return Filter_Sum / Len;
}



