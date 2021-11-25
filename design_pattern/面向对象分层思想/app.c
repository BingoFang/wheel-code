#include "data_process_layer.h"

/* 解析json语句，控制控件 */
ret_t onHandleData(void * data)
{
    widget_set_style_color();
}

/* 按下按钮连接tcp服务 */
ret_t  on_connectBtn_click(void* ctx, event_t* e)
{
    DataProcessTcp("192.168.2.105", 8080); 
    tDataProcess.TcpDataProcessSetFun(onHandleData);
}

/* 按下按钮发送tcp数据 */
ret_t  on_openOneBtn_click(void* ctx, event_t* e)
{
    tDataProcess.TcpSendData(out, strlen(out));
}

