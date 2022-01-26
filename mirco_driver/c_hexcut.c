/*************************************************************************
 * 文    件 : c_hexcut.c
 * 编 写 人	：youfang
 * 描    述	：去除boot程序的hex尾部2行校验值，避免同app程序合并后固件使用量产工具烧录失败
 * 编写时间	：2020-03-12
 * 版    本	：v1.0
**************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv[]) {
    int i,j;
    for (i = 0; i < argc; i++){
        printf("Argument %d is %s.\n", i, argv[i]);
    }
    
    FILE *inFile;    
    FILE *outFile;
    int c;
    int rlines = 0;
    int wlines = 0;
    
    // 打开hex文件,读取文件行数 
    inFile = fopen((char *)argv[1],"r");
    if (inFile == NULL)   
    {  
        printf("Open in file error!");   
        exit(0);
    }
    if (inFile)
    {
        while ((c = fgetc(inFile)) != EOF)
            if (c == '\n') rlines++;
        printf("rlines:%d\n", rlines);  
    }
    
    // 写入除最后两行数据到hex文件 
    outFile = fopen((char *)argv[2], "w");
    if (outFile == NULL)
    {  
        printf("Open out file error!");   
        exit(0);
    }
    // 从文件开头查找 
    fseek(inFile, 0, SEEK_SET);
    while(1)
    {
        c = fgetc(inFile);
        if (c == '\n') wlines++;
        if (wlines == (rlines - 2))
        { 
            fputc(c, outFile);
            break;
        } 
        
        fputc(c, outFile);
    }
    
    fclose(inFile); 
    fclose(outFile); 
    return 0;
}