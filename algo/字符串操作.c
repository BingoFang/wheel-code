/******************************
*���ã������ַ������Ƿ���ĳ���ַ�
*���أ�1������ 0��������
*******************************/
#include <stdio.h>
#define ture  1
#define flase 0
int find_char(char **strings,char value)
{
	char *string;
	while((string = *strings++) != NULL)
	{
		while(*string != '\0')
		{
			if(*string++ == value)
				return ture;
			else
				return flase;
		}
	}

} 

/* ���ַ�����ɾ��һ���ַ� */
char delete_char(char *str,char c)
{
	char *p = NULL;
	if(str == NULL)
	{
		return -1;
	}
	p = str;
	while(*p)
	{
		if(*p != c)
		{
			*str++ = *p;
		}
		p++;
	}
	*str = '\0';
	return 0;
}

/* ���ַ������ҵ��Ӵ� */
char * my_strstr(const char *s1, const char *s2)
{
    size_t l1, l2; 
 
    l2 = strlen(s2);
    if (!l2)
        return (char *)s1;
    l1 = strlen(s1);
    while (l1 >= l2) 
   {
        l1--;
        if (!memcmp(s1, s2, l2))
            return (char *)s1;
        s1++;
    }
    return NULL;
}

/* �ַ����ȽϺ��� */
char my_strncmp(const char *ptr,const char *str,char len)
{
	if (*str == '\0' || len == 0) return 0;

	while (len--)
	{
		if(*ptr != *str) return 0;
		ptr++;
		str++;
	}
	return 1;
}

int main()
{
	int result;
	char array[6] = {'v','a','l','u','e'};
	char *pstr = array;         //ʹ�ö���ָ����Ҫ�ȶ���һ��һ��ָ�룬Ȼ����ȡ&
	result = find_char(&pstr,'v');
	printf("%d\n",result);
}

/**********************************************
��Ŀ��Դ����C��ָ�롷
��Ŀ��������һ���ַ����н�������������������һ�������ַ������г��ֵ��ַ����ʹ�ӡ��*str
************************************************/
#include <stdio.h>
#define NULL 0
char *find_char(char const *str,char const *chars)
{
	char *cp;
	if(str != NULL && chars != NULL)
	{
		for(;*str != '\0';str++)
		{
				for(cp = chars;*cp != '\0';cp++)
				{
					if(*str == *cp)
						return str;
				}
		}
		return NULL;
	}
}
int main()
{
	char str[6] = {'c','h','i','n','a'};
	char chars[6] = {'c'};
	char *result;  //���ú������ش�ָ�룬���ܱ���Ҳ��Ҫ�Ǵ�ָ��Ĳ���ƥ��
	result = find_char(str,chars);
	printf("%s\n",result);  //����*�ı�ʾָ�룬�����ַ
	return 0;
}

/******************************************************
*����������C++���򣬴�һ���ַ�����ɾ����һ���ַ���������hash + ˫ָ��
*******************************************************/
#include <iostream>
#include <string>
using namespace std;

void getHash(const string &str, int arr[]){
    if(str.empty())
        return ;
    for(int i = 0; i < str.length(); i++){
        arr[str[i]] = 1;
    }
}

void deleteStr(string &str, const int arr[]){
    if(str.empty())
        return ;
    char *fast = &str[0];
    char *slow = &str[0];

    while(*fast != '\0'){
        if(arr[*fast] == 0){   //��ƥ��ʱ����ָ�붼��ǰ��һ��
            *slow++ = *fast;
        }
        fast++;								//ƥ����ͬʱ����fastָ����ǰ��һ��
    }
    *slow = '\0';
}

int main(){
    int hashArr[128];
    memset(hashArr, 0, sizeof(hashArr));
    string pattern = "ao";
    string str = "I am a boy!";

    cout << str << endl;
    getHash(pattern, hashArr);
    deleteStr(str, hashArr);

    cout<< str.data() << endl;
    return 0;
}

/******************************************************
*������������һ���ַ�����ɾ����һ���ַ�����
˼·���Ƚ���ͬ�Ļ���strָ����ǰ��һ��������ҵ�ƥ���ַ�ʱ���ͷ���ɾ����ʱ����ַ�����ַ���
ע��ֻɾ����һ����ȵ���ͬ���ַ���ʣ�µľͶ���ӡ����
*******************************************************/
#include "stdio.h"
#define NULL  0
#define NUL  '\0'
#define TURE  1
#define FLASE 0
char *match(char *str,char *want)   //�ҵ�ƥ��һ�����ַ���
{
	while(*want != NUL)
	{
		if(*str++ != *want++)
			return NULL;
		return str;	
	}	
}
int del_substr(char *str,char const *substr)  //ɾ��һ�����ַ���
{
	char *next;
	while(*str != NUL)
	{
		next = match(str,substr);
		if(next != NULL)
			break;   //��⵽�����Ƶ��ַ���������ȥ
		str++;
	}
	if(*str == NUL)  //�����������û���ҵ��򷵻�FLASE
		return FLASE;  
	while( *str++ = *next++);
			return TURE;    //��ɾ�����ַ������ֽ���str�����ӡ���������ַ���
}
int main()
{
	int  result;
	char str[6] = {'c','h','i','n','a'};
	char substr[4] = {'i'};
	result = del_substr(str,substr);
	printf("�Ƿ�ɾ���ɹ���%d\n",result);   //1Ϊ�ɹ���0Ϊʧ��
	printf("ɾ������ַ���Ϊ��%s\n",str);
	return 0;
}

/******************************************************
*������������һ���ַ�����ɾ����һ���ַ�����˫ָ��
*******************************************************/
#include <stdio.h>  
#include <string.h>  

void squeeze2(char *s1,const char *s2)  
{  
	/*˫ָ��*/
	int k;
	int instr2 = 0;     //���ã���⵽��ͬʱ���ܹ�ֹͣforѭ����ָ��fast��1�ٽ��м��
	char *fast = s1;
	char *slow = s1;
	while(*fast != '\0')
	{
		instr2 = 0;
		for(k = 0;s2[k] != '\0' && !instr2;k++)
		{
			if(*fast == s2[k])  //��������ͬʱ��ֻ��ָ��fast��1
			{
				instr2 = 1;	
				fast++;
			}
		}
		if(!instr2)
		{
			*slow++ = *fast++;	//�����߲�ͬʱ��ָ��fast��slowͬʱ��1
		}
	}
	*slow = '\0';


/*
//������ʽ
  int i, j, k;   
  int instr2 = 0;
  for(i = j = 0; s1[i] != '\0'; i++)  
  {   
	instr2 = 0;
    for(k = 0; s2[k] != '\0' && !instr2; k++)  
    {  
      if(s2[k] == s1[i])  
      {  
        instr2 = 1;
      }  
    } 
	if(!instr2)
	{
		s1[j++] = s1[i];	
	}
  }  
  s1[j] = '\0';  
*/
}  

main()   
{   
	char a[] = "hello world";
	char b[] = "er";
 
	squeeze2(&a,&b);
	printf("%s",a);
} 

/****************************************************************
 *�����������ݹ飬�������򣬶��ַ���ѯ��
*****************************************************************/
#include "stdio.h"
#include "stdlib.h"

/*  �׳˵ݹ�
int factorial(int n)
{
	if(n == 1)
	{
		return 1;
	}
	return n * factorial(n - 1);
}
*/

void selectionSort(int data[], int count)  //������������
{
    int i, j, min, temp;
    for(i = 0; i < count; i ++) {
        /*find the minimum*/
        min = i;
        for(j = i + 1; j < count; j ++)
            if(data[j] < data[min])
                min = j;
        temp = data[i];
        data[i] = data[min];
        data[min] = temp;
    }
}

int binary_search(int *data, int n, int key)  //�������֣��������ַ���1��û�ҵ�����0
{
    int mid;
    if(n == 1){     //ֹͣ����
        return (data[0] == key);   
    }
		else{
        mid = n/2;
        printf("mid=%d\n", data[mid]);
        if(data[mid-1] == key)
            return 1;        
        else if(data[mid-1] > key)
        {
            printf("key %d �� data[mid-1] %d С��ȡǰ��� \n", key, data[mid-1]);
            return binary_search(&data[0], mid, key);
        }
        else
        {
            printf("key %d �� data[mid-1] %d ��ȡ���� \n", key, data[mid-1]);
            return binary_search(&data[mid], n - mid, key);
        }
    }
}

void main()
{
    int i, key, rs;
    int arr[10];
    int count;

    printf("����ǰ����Ϊ��");
    srand((int)time(0));
	for(i=0; i < 10; i++)
	{
	    arr[i] = rand()%100;    //ѡȡ�����
	    printf("%d ",arr[i]);
	}

    count = sizeof(arr)/sizeof(arr[0]);
    selectionSort(arr, count);

    printf("\n���������Ϊ��");
    for(i=0; i < 10; i++)
	{
	    printf("%d ", arr[i]);
	}

    printf("\n������Ҫ���ҵ����֣�");
    scanf("%d",&key);

    rs = binary_search(arr, 10, key);
    printf("%d ", rs);
}

/****************************************************************
 *�������������ļ��
*****************************************************************/
#include <stdio.h>
#include <string.h>
int huiwen(char *string)
{
	int flag = 1;
	char *str;
//	str = string + strlen(string)-1;
	for(str = string;*str != '\0';*str++);
	*str--;
	while(string <= str)
	{
		if(*string != *str)
		{
			flag = 0;
			break;
		}		
		string++;
		str--;
	}
	return  flag;

}
int main()
{
	int result;
	char string[10] = {1,2,3,4,5,4,3,2,1};
	result = huiwen(string);
	if(result)
		printf("yes");
	else
		printf("no");
	return 0;
}

/****************************************************************
 *����������ָ�뷴������
*****************************************************************/
#include "stdio.h"
void reverse_string(char *str)
{
	char *last_str;    //����һ������ָ��
	for(last_str = str;*last_str != '\0';last_str++)
		;           //��ѭ�����ȵ�last_strָ�����һλ
	last_str--;     //�ȼ���һ����\0��
	while(str < last_str)   //������û����ʱ����
	{
		char temp;
		temp = *str;   
		*str++ = *last_str;
		*last_str-- = temp;

	}
}
int main()
{
	char str[6] = {'1','2','3','4','5'};
	reverse_string(str);
	printf("��������Ϊ��%s\n",str);
	return 0;
}

