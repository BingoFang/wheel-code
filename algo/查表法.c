/* ������ӡ��ÿ�е��������ݣ���end���ֵΪ���� */
int array[][10] = 
{
	0x03,0x00,0x00,0x00,0x00,//��һ�� 
    0x00,0x02,0x00,0x00,0x00,  
      
    0x0C,0x00,0x00,0x00,0x00,//�ڶ���  
    0x00,0x04,0x00,0x00,0x00,  
      
    0x10,0x00,0x00,0x00,0x00,//������  
    0x00,0x08,0x00,0x00,0x00,  
      
    0x60,0x00,0x00,0x00,0x00,//������  
    0x00,0x10,0x00,0x00,0x00,  
      
    0x80,0x01,0x00,0x00,0x00,//������  
    0x00,0x20,0x00,0x00,0x00,  
      
    0xAA,0x55,0x00,0x00,0xC0,//end  
    0x00,0x00,0x00,0x00,0x00,  
};

void designator_LED(int  *array)	//��ӡ������
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
	while(array[cout][0] != 0xAA || array[cout][1] != 0x55)	//ͨ��end�ı��ֵ������λ����
	{
		designator_LED((int *)(&array[0][0] + cout * 10));	//��λ���׵�ַ
		delay_500ms();
		cout++;
	}
	return 0;
}

/*��ѯһ��BYTE��1�ĸ���*/
#include <stdio.h>
#define BYTE unsigned char

/* ʱ�任�ռ�Ĳ�ѯ�� */
int main(int argc, char *argv[])
{
	int i, num = 0;
	BYTE a;
	// �����û����� 
	printf("\nPlease Input a BYTE(0~255):");
	scanf("%d", &a);
	// ���� 1 �ĸ��� 
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
	//...ֱ��num = 8
	}
	printf("\nthe num of 1 in the BYTE is %d", num);
	return 0;
}

/* ���Ǹ����͵Ŀռ任ʱ���㷨����0~255 ��1 �ĸ���ֱ�Ӵ洢�������У��ֽ�a ��Ϊ������±꣬
checknum[a]ֱ�Ӿ��� a �С�1���ĸ������㷨�ĸ��Ӷ����£�
ʱ�临�Ӷȣ� O(1)
�ռ临�Ӷȣ� O(2n) */
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
	/* �����û����� */
	printf("\nPlease Input a BYTE(0~255):");
	scanf("%d", &a);
	/* ���� 1 �ĸ��� */
	/* �� BYTE ֱ����Ϊ������±�ȡ�� 1 �ĸ��������գ� */
	printf("\nthe num of 1 in the BYTE is %d", numTable[a]);
	return 0;
}