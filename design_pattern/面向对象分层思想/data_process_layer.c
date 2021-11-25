#include "data_process_layer.h"
#include "net_server_layer.h"

DataProcess tDataProcess;

ret_t DataProcessTcp(str_t ip, int port)
{
    ret_t ret;

    tDataProcess.TcpSendData = TcpSendData; 
    tDataProcess.TcpDataProcessEntry = TcpDataProcessEntry;
    tDataProcess.TcpDataProcessFun = NULL;
    tDataProcess.TcpDataProcessSetFun = TcpDataProcessSetFun;

    NetServerRegister(&tDataProcess.TcpServer); /* 注册tcp服务 */

    ret = tDataProcess.TcpServer.init(&tDataProcess.TcpServer, TCP, ip, port); /* 连接tcp服务 */
    if (ret != RET_OK)
    {
        reutrn ret;
    }

    tDataProcess.TcpServer.setHandleDataEntry(&tDataProcess.TcpServer, tDataProcess.TcpDataProcessEntry); /* 设置tcp接收数据入口函数 */

    tDataProcess.TcpServer.recvData(&tDataProcess.TcpServer); /* 创建tcp接收线程，开始接收数据 */
}

/* tcp发送数据，调用net_server层发送函数 */
void TcpSendData(void *data, int len)
{
    tDataProcess.TcpServer.sendData(&tDataProcess.TcpServer, data, len);
}

/* 封装一层提供给应用层设置处理函数 */
ret_t TcpDataProcessSetFun(ret_t(*handleData)(void *data))
{
    if (handleData != NULL)
    {
        tDataProcess.TcpDataProcessFun = handleData;
    }
}

/* 接收主循环事件队列发来的idle请求，进行接收数据解密 */
ret_t TcpDataProcessEntry(const idle_info_t *idle)
{
	cRecvData* recvData = (cRecvData*)idle->ctx;
	int nDataLen = recvData->nDataLen;
	void* data = recvData->strData;

    tDataProcess.TcpDataProcessFun(data); /* 解密后数据 */
}