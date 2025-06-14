
#define NULL 0

#define SRAMIN	 0  //内部内存池
#define SRAMEX  1	//外部内存池

//mem1内存参数设定.mem1完全处于内部SRAM里面
#define MEM1_BLOCK_SIZE			32  	  													//内存块大小为32字节
#define MEM1_MAX_SIZE			  40*1024  													//最大管理内存 40K
#define MEM1_ALLOC_TABLE_SIZE	MEM1_MAX_SIZE/MEM1_BLOCK_SIZE 	//内存表大小

//mem2内存参数设定.mem2的内存池处于外部SRAM里面,其他的处于内部SRAM里面
#define MEM2_BLOCK_SIZE			32  	  						              //内存块大小为32字节
#define MEM2_MAX_SIZE			  200*1024  						            //最大管理内存200K
#define MEM2_ALLOC_TABLE_SIZE	MEM2_MAX_SIZE/MEM2_BLOCK_SIZE 	//内存表大小

//内存池(4字节对齐)
__align(4) u8 mem1base[MEM1_MAX_SIZE];													        //内部SRAM内存池
__align(4) u8 mem2base[MEM2_MAX_SIZE] __attribute__((at(0X68000000)));	//外部SRAM内存池

//内存管理表
u16 mem1mapbase[MEM1_ALLOC_TABLE_SIZE];													                      //内部SRAM内存池MAP
u16 mem2mapbase[MEM2_ALLOC_TABLE_SIZE] __attribute__((at(0X68000000+MEM2_MAX_SIZE)));	//外部SRAM内存池MAP

//内存管理参数	   
const u32 memtblsize[2]={MEM1_ALLOC_TABLE_SIZE,MEM2_ALLOC_TABLE_SIZE};		//内存表大小
const u32 memblksize[2]={MEM1_BLOCK_SIZE,MEM2_BLOCK_SIZE};								//内存分块大小
const u32 memsize[2]={MEM1_MAX_SIZE,MEM2_MAX_SIZE};												//内存总大小

//内存管理控制器
struct _m_mallco_dev
{
	void (*init)(u8);    //初始化
	u8 (*perused)(u8);   //内存使用率
	u8 *membase[2];			 //内存池管理2个区域内存
	u16 *memmap[2];			 //内存管理状态表
	u8 memrdy[2];				 //内存管理是否就绪
};

//内存管理控制器任务函数
struct _m_mallco_dev mallco_dev = 
{
	mem_init,									//内存初始化
	men_perused,							//内存使用率
	mem1base,mem2base,				//内存池
	mem1mapbase,mem2mapbase,	//内存管理状态表
	0,0,											//内存管理未就绪
};

//设置内存
void mymemset(void *s,u8 c,u32 count)
{
	u8 *xs = s;
	while(count--) *xs ++= c;
}

//复制内存
void mymemcpy(void *des,void *src,u32 n)
{
	u8 *xdes = des;
	u8 *xsrc = src;
	while(n--)*xdes++ = *xsrc++;
}

//内存管理初始化
void mem_init(u8 memx)
{
	mymemset(mallco_dev.memmap[memx],0,,memtblsize[memx]*2);  //内存状态表清零
	mymemset(mallco_dev.membase[memx],0,memsize[memx]);  			//内存池所有数据清零
	mallco_dev.memrdy[memx] = 1;
}

//获取内存使用率
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

//内存分配（外部调用）
void *mymalloc(u8 memx,u32 size)
{
	u32 offset;
	offset = men_malloc(memx,size);
	if(offset == 0xFFFFFFFF)return NULL;
	else
		return (void*)((u32)malloc_dev.membase[memx] + offset);
}

//内存分配（内部调用）
u32 mem_malloc(u8 memx,u32 size)
{
	signed long offset = 0;
	u16 nmemb;   //需要的内存块数
	u16 cmemb; 	 //连续空内存块数
	u32 i;
	if(!mallco_dev.memrdy[memx]) 
		malloc_dev.init(memx); 						 //未初始化，先执行初始化
	if(size == 0)return 0xFFFFFFFF;  	//不需要分配
	nmemb = size / memblks[memx];   
	if(size % memblksize[memx]) nmemb++;//多分配出一块
	for(offset = memtblsize[memx] - 1;offset >= 0;offset--)
	{
		if(!malloc_dev.memmap[memx](offset)) cmemb++;  //连续空内存块数增加
		else 
			cmemb = 0;  				 //连续内存块清零
		if(cmemb == nmemb)    //找到了连续nmemb个空内存块
		{
			for(i = 0;i < nmemb;i++)
			{
				malloc_dev.memmap[memx][offset+i] = nmemb;
			}
			return (offset*memblksize[memx]);  //返回偏移地址
		}
	}
	return 0xFFFFFFFF;  //未找到符合分配条件的内存块
}

//释放内存（内部调用）
//0,释放成功  1，释放失败
u8 mem_free(u8 memx,u32 offset)
{
	int i;
	if(!malloc_dev.memrdy[memx]) //未初始化，先执行初始化
	{
		mallco_dev.init(memx);    
    return 1;//未初始化 
	}
	if(offset < memsize[memx])  //偏移在内存池内
	{
		int index = offset / memblksize[memx];      //偏移所在内存块号码
		int nmemb = mallco_dev.memmap[memx][index]; //内存块数量
		for(i = 0;i < nmemb;i++)
		{
			mallco_dev,memmap[memx][index + i] = 0;   //释放清零
		}
		return 0；
	}
	else
		return 2;  //偏移超区了
}

//释放内存（外部调用）
void myfree(u8 memx,void *ptr)
{
	u32 offset;
	if(ptr == NULL) return 0;
	offset - (u32)ptr - (u32)mallco_dev.membase[memx];
	mem_free(memx,offset);  //释放内存
}

//重新分配内存
void *myrealloc(u8 memx,void *ptr,u32 size)
{
	u32 offset;
	offset = mem_malloc(memx,size);
	if(offset == 0xFFFFFF) return NULL;
	else
	{
		mymemcpy((void*)((u32)mallco_dev.membase[memx] + offset),ptr,size); //拷贝旧内存内容到新内存内容
		myfree(memx,ptr);  //释放旧内存
		return (void*)((u32)mallco_dev.membase[memx] + offset); //返回新内存首地址	
	}
}