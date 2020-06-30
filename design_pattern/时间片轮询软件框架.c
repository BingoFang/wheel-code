/****************************************************************
 *�������������ں���ָ�룬����ʱ��Ƭ��ѭ����
*****************************************************************/
/* ����ṹ�� */
typedef struct
{
	uint8_t Run;				     //����״̬
	uint8_t Timer;			      	 //��ʱ����ֵ
	uint8_t ltvTime;  				 //��ʱ����ֵ
	void(*TaskHook)(void);  		 //������ָ��
}TASK_COMPONENTS;          			 
	
/* ������������ */
TASK_COMPONENTS TaskComps[] =
{
	{0,50,50,PHTask}				 //PH����ʱ����50ms
	{0,60,60,TemperTask},			 //�¶�����ʱ����60ms
	{0,100,100,HumitureTask},		 //ʪ������ʱ����100ms
};

/* ������ */
typedef enum 
{
	PHTask,   						 
	TemperTask, 					 
	HumitureTask, 					
	TASK_MAX
}TASK_LIST;

/* �����־��������1ms��ϵͳ�δ��жϣ� */
void TaskRemarks(void)
{
	uint8_t i;
	for(i = 0;i < TASKS_MAX;i++)  					
	{
		if(TaskComps[i].Timer)					//�����ж϶�ʱ����ֵ
		{
			TaskComps[i].Timer--;				//�ݼ�һ��ʱ�ӽ���
			if(TaskComps[i].Timer == 0) 		//��������
			{
				TaskComps[i].Timer = TaskComps[i].itvTime;  //�ָ���ʱʱ��
				TaskComps[i].Run = 1;   		//�������Ϊ����̬
			}
		}
	}
}	

/* �������˺���������ѭ���� */
void TaskProcess(void)
{
	uint8_t i;
	for(i = 0;i < TASKS_MAX;i++)
	{
		if(TaskComps[i].Run)  			//��������������
		{
			TaskComps[i].TaskHook();  	//������
			TaskComps[i].Run = 0; 	    //�������Ϊ������̬
		}
	}
}

/****************************************************************
 *����������main������������ѯʹ�ô��ں���������ѯ�����������������ִ�С�
*****************************************************************/
typedef void (*main_handler_func)(void);   //���庯��ָ��

typedef enum		//������
{				   
	MAIN_START_TASK = 0,
	MAIN_RUN_TASK = 1,
	MAIN_END_TASK = 2
}EMAIN_STEP;
extern EMAIN_STEP MainStep;

main_handler_func const MainHanderTable[] =  //����������
{  
	start_task(),
	run_task(),
	end_task(),	
};

void main(void) 
{
	while(1)
	{
	    (*MainHandlerTable[MainStep])(); //���ú���������MainStepΪ��ֵ��ִ�кκ���
	}
}

/****************************************************************
 *����������״̬����ѯ��ѯ���ƣ�ͨ������������ֵ��ִ�к���
*****************************************************************/
typedef struct
{
  uint8_t check_flag;
  uint8_t err_flag;
  uint8_t cnt_200ms;
  uint8_t cnt_500ms;
  uint8_t cnt_1s;
}LOOP;

LOOP loop = {0};


/* �������ֵ��ǣ����ڶ�ʱ�������жϻ�ϵͳ�δ�ʱ��Ϊ������׼ */
void task_times(void)
{
	static uint16_t timetick= 0;
    timetick++;
  
    /* ʱ��Ƭ�����ʱ������λ */
    if((timetick % 20) == 0)
    {
        loop.cnt_200ms = 1;
        if(loop.check_flag == 1) //Ԥ��ʱ����200ms
        {
          loop.err_flag++;       //ÿ�ۼ�һ�Σ�˵����Ԥ�ڵ�ʱ����û������
        }
        else
        {
          loop.check_flag = 1;   //�ñ�־λ��ѭ��β��������
        }
    }
    if((timetick % 50) == 0)
    {
        loop.cnt_500ms = 1;
    }
    if((timetick % 100) == 0)
    {
        loop.cnt_1s = 1;
    }
    if((timetick % 200) == 0)
    {
        loop.cnt_2s = 1;
    }
    if((timetick % 500) == 0)
    {
        loop.cnt_5s = 1;
    }
 
}

/* �ܵĴ���ִ��ʱ����ҪС����Сʱ��Ƭ */
void task_loop(void)   
{
	if(loop.check_flag == 1)
	{
		if(loop.cnt_200ms)
		{
			loop.cnt_200ms = 0;
			task_200ms();
		}
    
		if(loop.cnt_500ms)
		{
			loop.cnt_500ms = 0;
			task_500ms();
		}   
		
		if(loop.cnt_1s)
		{
		    loop.cnt_1s = 0;
		    task_1s();
		}

		loop.check_flag = 0;  //ѭ�������������
	} 
}

