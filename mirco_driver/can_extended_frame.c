/*************************************************************************
 * 文    件 : can_extended_frame.c
 * 编 写 人	：youfang
 * 描    述	：打印can扩展帧对应格式上字段数据
 * 编写时间	：2020-03-12
 * 版    本	：v1.0
**************************************************************************/
/*
## can 29位ID分配 ##

    Protocol Version：2bit
    Profile ID：1bit，0：Can Device Profile；1：Can Cluster Library
    Destination Address Field：8bit[广播0xff
    网关默认地址0x00
    设备可用地址：1~254]
    Source Address Field：8bit
    Transaction sequence number：4bit
    Fragmentation ：2bit
    	00：Transmission is not fragmented.
    	01：Frame is first fragment of a fragmented transmission.(分包传输的第一包)
    	10：Frame is part of a fragmented transmission but not the first part.（分包，非第一包）
    	11：预留	
    Block Number：2bit
    	If the fragmentation sub-field is set to 01, then the block number sub-field indicate the number of blocks in the fragmented transmission. If the fragmentation sub-field is set to 10, then the block number sub-field indicate which block number of the transmission the current frame represents, taking the value 0x01 for the second fragment, 0x02 for the third, etc
    预留：2bit


|      | bit:27~28    | bit:25~26     | bit:23~24                   | bit:19~22      | bit:11~18           | bit:3~10   | bit:2            | bit:0~1 |
| ---- | ------------ | ------------- | --------------------------- | -------------- | ------------------- | ---------- | ---------------- | ------- |
| 预留 | Block Number | Fragmentation | Transaction sequence number | Source Address | Destination Address | Profile ID | Protocol Version |         |
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* run this program using the console pauser or add your own getch, system("pause") or input loop */
typedef unsigned           char uint8_t;
typedef unsigned short     int uint16_t;
typedef unsigned           int uint32_t;

typedef struct
{
    uint32_t ProtocolVersion : 2;
    uint32_t ProfileID : 1;
    uint32_t dstAddr : 8;
    uint32_t srcAddr : 8;
    uint32_t transSeqNum : 4;
    uint32_t Fragmentation : 2;
    uint32_t BlkNumber : 4;
    uint32_t reserved : 3;
} ExtIdFormat_t;

int main(int argc, char *argv[]) {
	ExtIdFormat_t value = {0};
	uint32_t temp = 0x001000AC ;
	memcpy(&value, &temp, sizeof(temp));
	
//	value.ProtocolVersion = 0;
//	value.ProfileID = 1;
//	value.dstAddr = 0x00;
//	value.srcAddr = 0x10;
//	value.transSeqNum = 2;
//	value.Fragmentation = 2;
//	value.BlkNumber = 0;
//	value.reserved = 0;
	
	printf("value.ProtocolVersion = 0x%x\n", value.ProtocolVersion); 
	printf("value.ProfileID = 0x%x\n", value.ProfileID); 
	printf("value.dstAddr = 0x%x\n", value.dstAddr); 
	printf("value.srcAddr = 0x%x\n", value.srcAddr); 
	printf("value.transSeqNum = 0x%x\n", value.transSeqNum); 
	printf("value.Fragmentation = 0x%x\n", value.Fragmentation); 
	printf("value.BlkNumber = 0x%x\n", value.BlkNumber); 
	printf("value.reserved = 0x%x\n", value.reserved); 
	return 0;
}