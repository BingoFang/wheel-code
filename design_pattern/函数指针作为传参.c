/****************************************************************
 *功能描述：函数指针传参进行函数回调
*****************************************************************/
#include <stdlib.h>  
#include <stdio.h>  
  
int Test1()  
{  
   int i;  
   for (i=0; i<5; i++)  
   {  
     printf("The %d th charactor is: %c\n", i, (char)('a' + i%26));  
    }  
   return 0;  
}  

int Test2(int num)  
{  
   int i;  
   for (i=0; i<num; i++)  
   {  
    printf("The %d th charactor is: %c\n", i, (char)('a' + i%26));  
    }  
   return 0;  
}  
  
void Caller1(void (*ptr)())//指向函数的指针作函数参数  
{  
   (*ptr)();  
}  

void Caller2(int n, int (*ptr)())//指向函数的指针作函数参数,这里第一个参数是为指向函数的指针服务的，  
{                                //不能写成void Caller2(int (*ptr)(int n))，这样的定义语法错误。  
   (*ptr)(n);  
   return;  
}  

int main()  
{  
   printf("************************\n");  
   Caller1(Test1); //相当于调用Test1();  
   printf("************************\n");  
   Caller2(3, Test2); //相当于调用Test2(3);  
   return 0;  
}  
