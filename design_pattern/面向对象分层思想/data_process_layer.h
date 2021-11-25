#ifndef _DATA_PROCESS_LAYER_H_
#define _DATA_PROCESS_LAYER_H_

#include "net_server_layer.h"

typedef struct DataProcess
{
    NetServer TcpServer;
    NetServer UdpServer;

    void (*TcpSendData)(void *data, int len);
    void (*UdpSendData)(void *data, int len);

    ret_t (*TcpDataProcessEntry)(const idle_info_t *idle);
    ret_t (*UdpDataProcessEntry)(const idle_info_t *idle);

    ret_t (*TcpDataProcessFun)(void * data);
    ret_t (*UdpDataProcessFun)(void * data);

    ret_t (*TcpDataProcessSetFun)(ret_t(*handleData)(void *data));
    ret_t (*UdpDataProcessSetFun)(ret_t(*handleData)(void *data));
}DataProcess, *PDataProcess;

extern DataProcess tDataProcess;

extern ret_t DataProcessTcp(str_t ip, int port);

#endif
