/******************************
*作用：查找字符串中是否有某个字符
*返回：1：存在 0：不存在
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

/* 从字符串中删除一个字符 */
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

/* 在字符串中找到子串 */
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

/* 字符串比较函数 */
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
	char *pstr = array;         //使用二级指针需要先定义一个一级指针，然后在取&
	result = find_char(&pstr,'v');
	printf("%d\n",result);
}

/**********************************************
题目来源：《C和指针》
题目：函数在一个字符串中进行搜索，查找所有在一个给定字符集合中出现的字符，就打印出*str
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
	char *result;  //调用函数返回带指针，接受变量也需要是带指针的才能匹配
	result = find_char(str,chars);
	printf("%s\n",result);  //不带*的表示指针，代表地址
	return 0;
}

/******************************************************
*功能描述：C++程序，从一个字符串中删除另一个字符串，利用hash + 双指针
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
        if(arr[*fast] == 0){   //不匹配时，两指针都向前进一步
            *slow++ = *fast;
        }
        fast++;								//匹配相同时，就fast指针向前进一步
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
*功能描述：从一个字符串中删除另一个字符串，
思路：比较相同的话，str指针向前走一步，如果找到匹配字符时，就返回删除此时这个字符后的字符串
注：只删除第一次配比到相同的字符，剩下的就都打印出来
*******************************************************/
#include "stdio.h"
#define NULL  0
#define NUL  '\0'
#define TURE  1
#define FLASE 0
char *match(char *str,char *want)   //找到匹配一样的字符串
{
	while(*want != NUL)
	{
		if(*str++ != *want++)
			return NULL;
		return str;	
	}	
}
int del_substr(char *str,char const *substr)  //删除一样的字符串
{
	char *next;
	while(*str != NUL)
	{
		next = match(str,substr);
		if(next != NULL)
			break;   //检测到有相似的字符串则跳出去
		str++;
	}
	if(*str == NUL)  //如果都检测完后还没有找到则返回FLASE
		return FLASE;  
	while( *str++ = *next++);
			return TURE;    //将删除的字符串部分接上str后面打印出来整个字符串
}
int main()
{
	int  result;
	char str[6] = {'c','h','i','n','a'};
	char substr[4] = {'i'};
	result = del_substr(str,substr);
	printf("是否删除成功：%d\n",result);   //1为成功，0为失败
	printf("删除后的字符串为：%s\n",str);
	return 0;
}

/******************************************************
*功能描述：从一个字符串中删除另一个字符串，双指针
*******************************************************/
#include <stdio.h>  
#include <string.h>  

void squeeze2(char *s1,const char *s2)  
{  
	/*双指针*/
	int k;
	int instr2 = 0;     //作用：检测到相同时，能够停止for循环，指针fast加1再进行检测
	char *fast = s1;
	char *slow = s1;
	while(*fast != '\0')
	{
		instr2 = 0;
		for(k = 0;s2[k] != '\0' && !instr2;k++)
		{
			if(*fast == s2[k])  //当两者相同时，只有指针fast加1
			{
				instr2 = 1;	
				fast++;
			}
		}
		if(!instr2)
		{
			*slow++ = *fast++;	//当两者不同时，指针fast和slow同时加1
		}
	}
	*slow = '\0';


/*
//数组形式
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
 *功能描述：递归，正序排序，二分法查询法
*****************************************************************/
#include "stdio.h"
#include "stdlib.h"

/*  阶乘递归
int factorial(int n)
{
	if(n == 1)
	{
		return 1;
	}
	return n * factorial(n - 1);
}
*/

void selectionSort(int data[], int count)  //进行正序排序
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

int binary_search(int *data, int n, int key)  //查找数字，存在数字返回1，没找到返回0
{
    int mid;
    if(n == 1){     //停止条件
        return (data[0] == key);   
    }
		else{
        mid = n/2;
        printf("mid=%d\n", data[mid]);
        if(data[mid-1] == key)
            return 1;        
        else if(data[mid-1] > key)
        {
            printf("key %d 比 data[mid-1] %d 小，取前半段 \n", key, data[mid-1]);
            return binary_search(&data[0], mid, key);
        }
        else
        {
            printf("key %d 比 data[mid-1] %d 大，取后半段 \n", key, data[mid-1]);
            return binary_search(&data[mid], n - mid, key);
        }
    }
}

void main()
{
    int i, key, rs;
    int arr[10];
    int count;

    printf("排序前数组为：");
    srand((int)time(0));
	for(i=0; i < 10; i++)
	{
	    arr[i] = rand()%100;    //选取随机数
	    printf("%d ",arr[i]);
	}

    count = sizeof(arr)/sizeof(arr[0]);
    selectionSort(arr, count);

    printf("\n排序后数组为：");
    for(i=0; i < 10; i++)
	{
	    printf("%d ", arr[i]);
	}

    printf("\n请输入要查找的数字：");
    scanf("%d",&key);

    rs = binary_search(arr, 10, key);
    printf("%d ", rs);
}

/****************************************************************
 *功能描述：回文检测
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
 *功能描述：指针反向排序
*****************************************************************/
#include "stdio.h"
void reverse_string(char *str)
{
	char *last_str;    //定义一个最后的指针
	for(last_str = str;*last_str != '\0';last_str++)
		;           //空循环，等到last_str指向最后一位
	last_str--;     //先减掉一个‘\0’
	while(str < last_str)   //在两则还没相遇时交换
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
	printf("反向排序为：%s\n",str);
	return 0;
}

