/****************************************************************
 *功能描述：窗口函数指针，利用时间片来循环。
*****************************************************************/
/* 任务结构体 */
typedef struct
{
	uint8_t Run;				     //运行状态
	uint8_t Timer;			      	 //定时计数值
	uint8_t ltvTime;  				 //定时周期值
	void(*TaskHook)(void);  		 //任务函数指针
}TASK_COMPONENTS;          			 
	
/* 定义三个任务 */
TASK_COMPONENTS TaskComps[] =
{
	{0,50,50,PHTask}				 //PH任务定时周期50ms
	{0,60,60,TemperTask},			 //温度任务定时周期60ms
	{0,100,100,HumitureTask},		 //湿度任务定时周期100ms
};

/* 任务编号 */
typedef enum 
{
	PHTask,   						 
	TemperTask, 					 
	HumitureTask, 					
	TASK_MAX
}TASK_LIST;

/* 任务标志处理（放在1ms的系统滴答中断） */
void TaskRemarks(void)
{
	uint8_t i;
	for(i = 0;i < TASKS_MAX;i++)  					
	{
		if(TaskComps[i].Timer)					//遍历判断定时计数值
		{
			TaskComps[i].Timer--;				//递减一个时钟节拍
			if(TaskComps[i].Timer == 0) 		//计数到点
			{
				TaskComps[i].Timer = TaskComps[i].itvTime;  //恢复定时时间
				TaskComps[i].Run = 1;   		//标记任务为运行态
			}
		}
	}
}	

/* 任务处理，此函数放至主循环中 */
void TaskProcess(void)
{
	uint8_t i;
	for(i = 0;i < TASKS_MAX;i++)
	{
		if(TaskComps[i].Run)  			//遍历可运行任务
		{
			TaskComps[i].TaskHook();  	//任务函数
			TaskComps[i].Run = 0; 	    //标记任务为非运行态
		}
	}
}

/****************************************************************
 *功能描述：main函数中任务轮询使用窗口函数进行轮询，利用任务号来调用执行。
*****************************************************************/
typedef void (*main_handler_func)(void);   //定义函数指针

typedef enum		//任务编号
{				   
	MAIN_START_TASK = 0,
	MAIN_RUN_TASK = 1,
	MAIN_END_TASK = 2
}EMAIN_STEP;
extern EMAIN_STEP MainStep;

main_handler_func const MainHanderTable[] =  //定义任务函数
{  
	start_task(),
	run_task(),
	end_task(),	
};

void main(void) 
{
	while(1)
	{
	    (*MainHandlerTable[MainStep])(); //调用函数中设置MainStep为何值便执行何函数
	}
}

/****************************************************************
 *功能描述：状态机轮询查询机制，通过任务计数标记值来执行函数
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


/* 任务计数值标记，放在定时器更新中断或系统滴答时钟为计数基准 */
void task_times(void)
{
	static uint16_t timetick= 0;
    timetick++;
  
    /* 时间片任务计时变量置位 */
    if((timetick % 20) == 0)
    {
        loop.cnt_200ms = 1;
        if(loop.check_flag == 1) //预期时间以200ms
        {
          loop.err_flag++;       //每累加一次，说明在预期的时间内没有跑完
        }
        else
        {
          loop.check_flag = 1;   //该标志位在循环尾部被清零
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

/* 总的代码执行时间需要小于最小时间片 */
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

		loop.check_flag = 0;  //循环运行完毕清零
	} 
}

