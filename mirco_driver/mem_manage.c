
#define NULL 0

#define SRAMIN	 0  //�ڲ��ڴ��
#define SRAMEX  1	//�ⲿ�ڴ��

//mem1�ڴ�����趨.mem1��ȫ�����ڲ�SRAM����
#define MEM1_BLOCK_SIZE			32  	  													//�ڴ���СΪ32�ֽ�
#define MEM1_MAX_SIZE			  40*1024  													//�������ڴ� 40K
#define MEM1_ALLOC_TABLE_SIZE	MEM1_MAX_SIZE/MEM1_BLOCK_SIZE 	//�ڴ���С

//mem2�ڴ�����趨.mem2���ڴ�ش����ⲿSRAM����,�����Ĵ����ڲ�SRAM����
#define MEM2_BLOCK_SIZE			32  	  						              //�ڴ���СΪ32�ֽ�
#define MEM2_MAX_SIZE			  200*1024  						            //�������ڴ�200K
#define MEM2_ALLOC_TABLE_SIZE	MEM2_MAX_SIZE/MEM2_BLOCK_SIZE 	//�ڴ���С

//�ڴ��(4�ֽڶ���)
__align(4) u8 mem1base[MEM1_MAX_SIZE];													        //�ڲ�SRAM�ڴ��
__align(4) u8 mem2base[MEM2_MAX_SIZE] __attribute__((at(0X68000000)));	//�ⲿSRAM�ڴ��

//�ڴ�����
u16 mem1mapbase[MEM1_ALLOC_TABLE_SIZE];													                      //�ڲ�SRAM�ڴ��MAP
u16 mem2mapbase[MEM2_ALLOC_TABLE_SIZE] __attribute__((at(0X68000000+MEM2_MAX_SIZE)));	//�ⲿSRAM�ڴ��MAP

//�ڴ�������	   
const u32 memtblsize[2]={MEM1_ALLOC_TABLE_SIZE,MEM2_ALLOC_TABLE_SIZE};		//�ڴ���С
const u32 memblksize[2]={MEM1_BLOCK_SIZE,MEM2_BLOCK_SIZE};								//�ڴ�ֿ��С
const u32 memsize[2]={MEM1_MAX_SIZE,MEM2_MAX_SIZE};												//�ڴ��ܴ�С

//�ڴ���������
struct _m_mallco_dev
{
	void (*init)(u8);    //��ʼ��
	u8 (*perused)(u8);   //�ڴ�ʹ����
	u8 *membase[2];			 //�ڴ�ع���2�������ڴ�
	u16 *memmap[2];			 //�ڴ����״̬��
	u8 memrdy[2];				 //�ڴ�����Ƿ����
};

//�ڴ���������������
struct _m_mallco_dev mallco_dev = 
{
	mem_init,									//�ڴ��ʼ��
	men_perused,							//�ڴ�ʹ����
	mem1base,mem2base,				//�ڴ��
	mem1mapbase,mem2mapbase,	//�ڴ����״̬��
	0,0,											//�ڴ����δ����
};

//�����ڴ�
void mymemset(void *s,u8 c,u32 count)
{
	u8 *xs = s;
	while(count--) *xs ++= c;
}

//�����ڴ�
void mymemcpy(void *des,void *src,u32 n)
{
	u8 *xdes = des;
	u8 *xsrc = src;
	while(n--)*xdes++ = *xsrc++;
}

//�ڴ�����ʼ��
void mem_init(u8 memx)
{
	mymemset(mallco_dev.memmap[memx],0,,memtblsize[memx]*2);  //�ڴ�״̬������
	mymemset(mallco_dev.membase[memx],0,memsize[memx]);  			//�ڴ��������������
	mallco_dev.memrdy[memx] = 1;
}

//��ȡ�ڴ�ʹ����
void mem_perused(u8 memx)
{
	u32 used = 0;
	u32 i;
	for(i = 0;i < memtblsize[memx];i++)
	{
		if(malloc_devmemmap[memx][i]) used++;
	}
	return (used * 100)/(memtblsize[memx]);
}

//�ڴ���䣨�ⲿ���ã�
void *mymalloc(u8 memx,u32 size)
{
	u32 offset;
	offset = men_malloc(memx,size);
	if(offset == 0xFFFFFFFF)return NULL;
	else
		return (void*)((u32)malloc_dev.membase[memx] + offset);
}

//�ڴ���䣨�ڲ����ã�
u32 mem_malloc(u8 memx,u32 size)
{
	signed long offset = 0;
	u16 nmemb;   //��Ҫ���ڴ����
	u16 cmemb; 	 //�������ڴ����
	u32 i;
	if(!mallco_dev.memrdy[memx]) 
		malloc_dev.init(memx); 						 //δ��ʼ������ִ�г�ʼ��
	if(size == 0)return 0xFFFFFFFF;  	//����Ҫ����
	nmemb = size / memblks[memx];   
	if(size % memblksize[memx]) nmemb++;//������һ��
	for(offset = memtblsize[memx] - 1;offset >= 0;offset--)
	{
		if(!malloc_dev.memmap[memx](offset)) cmemb++;  //�������ڴ��������
		else 
			cmemb = 0;  				 //�����ڴ������
		if(cmemb == nmemb)    //�ҵ�������nmemb�����ڴ��
		{
			for(i = 0;i < nmemb;i++)
			{
				malloc_dev.memmap[memx][offset+i] = nmemb;
			}
			return (offset*memblksize[memx]);  //����ƫ�Ƶ�ַ
		}
	}
	return 0xFFFFFFFF;  //δ�ҵ����Ϸ����������ڴ��
}

//�ͷ��ڴ棨�ڲ����ã�
//0,�ͷųɹ�  1���ͷ�ʧ��
u8 mem_free(u8 memx,u32 offset)
{
	int i;
	if(!malloc_dev.memrdy[memx]) //δ��ʼ������ִ�г�ʼ��
	{
		mallco_dev.init(memx);    
    return 1;//δ��ʼ�� 
	}
	if(offset < memsize[memx])  //ƫ�����ڴ����
	{
		int index = offset / memblksize[memx];      //ƫ�������ڴ�����
		int nmemb = mallco_dev.memmap[memx][index]; //�ڴ������
		for(i = 0;i < nmemb;i++)
		{
			mallco_dev,memmap[memx][index + i] = 0;   //�ͷ�����
		}
		return 0��
	}
	else
		return 2;  //ƫ�Ƴ�����
}

//�ͷ��ڴ棨�ⲿ���ã�
void myfree(u8 memx,void *ptr)
{
	u32 offset;
	if(ptr == NULL) return 0;
	offset - (u32)ptr - (u32)mallco_dev.membase[memx];
	mem_free(memx,offset);  //�ͷ��ڴ�
}

//���·����ڴ�
void *myrealloc(u8 memx,void *ptr,u32 size)
{
	u32 offset;
	offset = mem_malloc(memx,size);
	if(offset == 0xFFFFFF) return NULL;
	else
	{
		mymemcpy((void*)((u32)mallco_dev.membase[memx] + offset),ptr,size); //�������ڴ����ݵ����ڴ�����
		myfree(memx,ptr);  //�ͷž��ڴ�
		return (void*)((u32)mallco_dev.membase[memx] + offset); //�������ڴ��׵�ַ	
	}
}