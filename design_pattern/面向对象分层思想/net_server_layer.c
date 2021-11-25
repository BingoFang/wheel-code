#include "net_server_layer.h"

/* 注册网络服务 */
void NetServerRegister(PNetServer this)
{
    this->socketFd = NONE;
    this->socketType = NONE;
    str_init(this->serverIp, 10);
    this->serverPort = NONE;

    this->init = NetServerInit;
    this->sendData = NetServerSendData;
    this->recvData = NetServerRecvData;
    this->setHandleDataEntry = NetServerSetHandleDataEntry;
}

/* socket连接tcp/udp服务 */
ret_t NetServerInit(PNetServer this, int type, char *ip, int port)
{
    tcp_connect(ip, port);
}

/* socket发送数据 */
void NetServerSendData(PNetServer this, int *buff, int len)
{
    sendto();
}

/* 创建接收线程，开始接收数据 */
void NetServerRecvData(PNetServer this, int *buff, int len)
{
    tk_thread_create(tcpRecvThread,this);
    tk_thread_start();
}

/* 设置数据处理入口函数 */
void NetServerSetHandleDataEntry(PNetServer this, ret_t(*handleData)(const idle_info_t *idle))
{
	if (this && handleData == NULL)
	{
		return;
	}
	this->handleData = handleData;
}

/* 线程接收数据，向主循环的事件队列发送idle请求，则回调tDataProcess.TcpDataProcessEntry */
void *tcpRecvThread(void* args)
{
    idle_queue(this->handleData, data);
}