#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define FIND_BYTE_LEN 		32
#define APP_BASE_ADDR		0x0800B800
#define OPTION_PARA_ADDR	0x0801D000
#define OPTION_PARA_FLAG	0xB4B4B4B4
#define OPTION_RELATIVE_ADDR (OPTION_PARA_ADDR - APP_BASE_ADDR)

typedef unsigned char uint8;
typedef struct
{
	int flag;
	int addr;
	int len;
}optionPara_t;

optionPara_t optionPara = 
{
	.flag =  OPTION_PARA_FLAG,
	.addr =  OPTION_PARA_ADDR,
}; 

int main(int argc, char **argv[]) {
	int i,j;
	for (i = 0; i < argc; i++){
		printf("Argument %d is %s.\n", i, argv[i]);
	}
	
	FILE *pFile;    
	int iByteNum,iSame;
	uint8 acBuf[128 * 1024];
	uint8 acFindByte = 0xFF;
	uint8 dataFill[FIND_BYTE_LEN];
	memset(dataFill, 0xFF, FIND_BYTE_LEN);
	
	// 打开未分割bin文件 
    pFile = fopen((char *)argv[1],"rb");
    if(pFile==NULL)   
    {  
    	printf ("Open file error!");   
    	exit(0);
    }
    
    // 获取文件大小 
    fseek(pFile, 0, SEEK_END);
    iByteNum = ftell(pFile);
    rewind(pFile);
    optionPara.len = iByteNum - OPTION_RELATIVE_ADDR;
    
    // 将文件内容读到buf 
    fread(acBuf, 1, iByteNum, pFile);
    fclose(pFile);
    
    // 查找连续对应byte 
    for (i = 0; i < iByteNum; i++)
    {
    	if (acBuf[i] == acFindByte)
    	{
    		iSame = 0;
    		for (j = 0; j < FIND_BYTE_LEN; j++)
    		{
    		    if(acBuf[i + j] == acFindByte)
    				iSame++;		
			}
    		if (iSame == FIND_BYTE_LEN)
    		{
    			// 保存分割后bin文件 
    			pFile = fopen((char *)argv[2],"wb+");
    			fwrite(acBuf, 1, i, pFile); //写入app应用数据 
    			fwrite(dataFill, 1, sizeof(dataFill), pFile); //写入填充0xFF，避免截掉应用程序尾处为0xFF 
				fwrite(&acBuf[OPTION_RELATIVE_ADDR], 1, optionPara.len, pFile); //写入配置数据 
				fwrite(&optionPara, 1, sizeof(optionPara), pFile); //写入配置参数 
				fclose(pFile);
    			return 0; 
			}
		}
	}
    return 0;
}