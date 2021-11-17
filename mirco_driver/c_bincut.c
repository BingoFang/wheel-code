#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FIND_BYTE_LEN 32

int main(int argc, char **argv[]) {
	int i,j;
	for (i = 0; i < argc; i++){
		printf("Argument %d is %s.\n", i, argv[i]);
	}
	
	FILE *pFile;    
	int iByteNum,iSame;
	uint8 acBuf[128 * 1024];
	uint8 acFindByte = 0xFF;
	
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
    			fwrite(acBuf, 1, i, pFile);
				fclose(pFile);
    			return 0; 
			}
		}
	}
    return 0;
}