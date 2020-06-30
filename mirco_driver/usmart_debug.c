#include "Usmart.h"
#include "Usmart_str.h"

//�������ƹ�������ʼ��
struct _m_usmart_dev usmart_dev=
{
	usmart_nametab,
	usmart_init,
	usmart_cmd_rec,
	usmart_exe,
	usmart_scan,
	sizeof(usmart_nametab)/sizeof(struct _m_usmart_nametab),
	0,	   //��������
	0,	 	 //����ID
	1,		 //������ʾ���ͣ�0��10���ƣ�1:16����
	0,		 //�������ͣ�0�����֣�1���ַ���	    
	0,	   //ÿ�������ĳ����ݴ����ҪMAX_PARM��0��ʼ��
	0,		 //�����Ĳ�������ҪPARM_LEN��0��ʼ����
};  

void Usmart_Init()
{
	#if USMART_ENTIMX_SCAN == 1
		Timer_100us_Scan();  //ÿ0.1msɨ��Uamart,ʱ��Ƶ��Ҫ����Ϊ10KHZ
	#endif
		usmart_dev.sptype = 1;
}

/*****************
 ��str�л�ȡ��������id���Լ�������Ϣ
 *str�ַ���ָ��
 ����0��ʶ��ɹ����������������
******************/
u8 usmart_cmd_rec(u8 *str)
{
	u8 sta,i,rval;//״̬ 
	u8 rpnum,spnum;
	u8 rfname[MAX_FNAME_LEN];//�ݴ�ռ䣬���ڴ�Ž��ܵĺ�����  
	u8 sfname[MAX_FNAME_LEN];//��ű��غ�����
	sta=usmart_get_fname(str,rfname,&rpnum,&rval);//�õ����ܵ������ݵĺ������Լ���������  
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

