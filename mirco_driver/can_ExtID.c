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