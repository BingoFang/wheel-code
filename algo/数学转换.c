/* 求最大公约数 */
int gcd(int m,int n)
{
	int r;
	if(m <= 0 || n <= 0)
		return 0;
	r = m % n;
	return r > 0 ? gcd(n,r):n;
}

/* 利用二进制转化成ASCII */
void binary_to_ascii(unsigned int data)
{
	unsigned int quotient;
	quotient = data / 10;
	if(quotient != 0)
		binary_to_ascii(quotient);
	putchar(data % 10 + '0');
}

/* 利用迭代将字符串转化成一个整形 */
int ascii_to_integer(char *string)
{
	int data = 0;
	while(*string >= '0' && *string <= '9')
	{
		data *= 10;
		data += *string - '0';  //将字符串转化为整形 - ‘0’
		string++;
	}
	if(*string != '\0')  //如果遇到一个非数字字符的返回0
		data = 0;
	return data;
}

/***********************************************************
 * 函数名称：Hex2Str
 * 功能描述：十六进制转换为字符串
 * 输入参数：pbDest：存放数据，pbSrc：输入数据，nLen：输入数据长度
 * 输出参数：无
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
 * 函数名称：Byte2Dword
 * 功能描述：一个字节型数组拆分重组成4个字节数组
 * 输入参数：pbDest：存储数据，pbSrc：输入数据，nLen：输入数据长度
 * 输出参数：无
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
 * 函数名称：Dword2Byte
 * 功能描述：4个字节数组拆分成一个字节数组
 * 输入参数：pbDest：存储数据，pbSrc：输入数据，nLen：输入数据长度
 * 输出参数：无
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

/* 字符串转换整形数值 */
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

/* 寻找最大值，必须在一个结尾符为负值结束 */
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
	   else        //当遇到结尾为负值时结束循环，返回最大值max
		return max;
}


/* 去极值求平均值 */
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

/* 滑动均值滤波 */
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



