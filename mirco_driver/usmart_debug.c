#include "Usmart.h"
#include "Usmart_str.h"

//函数控制管理器初始化
struct _m_usmart_dev usmart_dev=
{
	usmart_nametab,
	usmart_init,
	usmart_cmd_rec,
	usmart_exe,
	usmart_scan,
	sizeof(usmart_nametab)/sizeof(struct _m_usmart_nametab),
	0,	   //参数数量
	0,	 	 //函数ID
	1,		 //函数显示类型，0：10进制，1:16进制
	0,		 //参数类型，0：数字，1：字符串	    
	0,	   //每个参数的长度暂存表，需要MAX_PARM个0初始化
	0,		 //函数的参数，需要PARM_LEN个0初始化ˉ
};  

void Usmart_Init()
{
	#if USMART_ENTIMX_SCAN == 1
		Timer_100us_Scan();  //每0.1ms扫描Uamart,时钟频率要设置为10KHZ
	#endif
		usmart_dev.sptype = 1;
}

/*****************
 从str中获取函数名，id，以及参数信息
 *str字符串指针
 返回0：识别成功，其他：代码错误
******************/
u8 usmart_cmd_rec(u8 *str)
{
	u8 sta,i,rval;//状态 
	u8 rpnum,spnum;
	u8 rfname[MAX_FNAME_LEN];//暂存空间，用于存放接受的函数名  
	u8 sfname[MAX_FNAME_LEN];//存放本地函数名
	sta=usmart_get_fname(str,rfname,&rpnum,&rval);//得到接受到的数据的函数名以及函数个数  
	if(sta)return sta; //
	for(i=0;i<usmart_dev.fnum;i++)
	{
		sta=usmart_get_fname((u8*)usmart_dev.funs[i].name,sfname,&spnum,&rval);/
		if(sta)return sta;  
		if(usmart_strcmp(sfname,rfname)==0)
		{
			if(spnum>rpnum)return USMART_PARMERR;
			usmart_dev.id=i;
			break;
		}	
	}
	if(i==usmart_dev.fnum)return USMART_NOFUNCFIND;	
 	sta=usmart_get_fparam(str,&i);						
	if(sta)return sta;								
	usmart_dev.pnum=i;								
  return USMART_OK;
}

