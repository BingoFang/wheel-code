/* 遍历打印出每行的所有数据，以end标记值为结束 */
int array[][10] = 
{
	0x03,0x00,0x00,0x00,0x00,//第一行 
    0x00,0x02,0x00,0x00,0x00,  
      
    0x0C,0x00,0x00,0x00,0x00,//第二行  
    0x00,0x04,0x00,0x00,0x00,  
      
    0x10,0x00,0x00,0x00,0x00,//第三行  
    0x00,0x08,0x00,0x00,0x00,  
      
    0x60,0x00,0x00,0x00,0x00,//第四行  
    0x00,0x10,0x00,0x00,0x00,  
      
    0x80,0x01,0x00,0x00,0x00,//第五行  
    0x00,0x20,0x00,0x00,0x00,  
      
    0xAA,0x55,0x00,0x00,0xC0,//end  
    0x00,0x00,0x00,0x00,0x00,  
};

void designator_LED(int  *array)	//打印列数据
{
	int i;
	for(i = 0;i < 10;i++)
	{
		printf("%3d\r\n",*(array + i));
	}
}

void delay_500ms(void)
{
	Sleep(500);
}

void main(void)
{
	int cout = 0;
	while(array[cout][0] != 0xAA || array[cout][1] != 0x55)	//通过end的标记值遍历定位行数
	{
		designator_LED((int *)(&array[0][0] + cout * 10));	//定位行首地址
		delay_500ms();
		cout++;
	}
	return 0;
}

/*查询一个BYTE中1的个数*/
#include <stdio.h>
#define BYTE unsigned char

/* 时间换空间的查询法 */
int main(int argc, char *argv[])
{
	int i, num = 0;
	BYTE a;
	// 接收用户输入 
	printf("\nPlease Input a BYTE(0~255):");
	scanf("%d", &a);
	// 计算 1 的个数 
	switch (a)
	{
		case 0x0:
			num = 0;
			break;
		case 0x1:
		case 0x2:
		case 0x4:
		case 0x8:
		case 0x10:
		case 0x20:
		case 0x40:
		case 0x80:
			num = 1;
			break;
		case 0x3:
		case 0x6:
		case 0xc:
		case 0x18:
		case 0x30:
		case 0x60:
		case 0xc0:
			num = 2;
			break;
	//...直至num = 8
	}
	printf("\nthe num of 1 in the BYTE is %d", num);
	return 0;
}

/* 这是个典型的空间换时间算法，把0~255 中1 的个数直接存储在数组中，字节a 作为数组的下标，
checknum[a]直接就是 a 中“1”的个数！算法的复杂度如下：
时间复杂度： O(1)
空间复杂度： O(2n) */
BYTE numTable[256] =
{
	0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 4, 1, 2, 2, 3, 2, 3, 3, 4, 2, 3,
	3, 4, 3, 4, 4, 5, 1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5, 2, 3, 3,
	4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4,
	3, 4, 4, 5, 2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 2, 3, 3, 4, 3,
	4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6,
	6, 7, 1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5, 2, 3, 3, 4, 3, 4, 4,
	5, 3, 4, 4, 5, 4, 5, 5, 6, 2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
	3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7, 2, 3, 3, 4, 3, 4, 4, 5, 3,
	4, 4, 5, 4, 5, 5, 6, 3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7, 3, 4,
	4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7, 4, 5, 5, 6, 5, 6, 6, 7, 5, 6, 6,
	7, 6, 7, 7, 8
};

int main(int argc, char *argv[])
{
	int i, num = 0;
	BYTE a = 0;
	/* 接收用户输入 */
	printf("\nPlease Input a BYTE(0~255):");
	scanf("%d", &a);
	/* 计算 1 的个数 */
	/* 用 BYTE 直接作为数组的下标取出 1 的个数，妙哉！ */
	printf("\nthe num of 1 in the BYTE is %d", numTable[a]);
	return 0;
}