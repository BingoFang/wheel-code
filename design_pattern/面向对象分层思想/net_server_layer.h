#ifndef _NET_SERVER_LAYER_H
#define _NET_SERVER_LAYER_H

typedef enum
{
    NONE = -1,
    TCP,
    UDP
}SOCKET_TYPE;

/* 抽象网络服务结构体 */
typedef struct NetServer
{
    int socketFd;
    int socketType;
    str_t *serverIp;
    int serverPort;

    ret_t (*init)(PNetServer this, int type, char *ip, int port);
    void  (*sendData)(PNetServer this, int *buff, int len);
    void  (*recvData)(PNetServer this);

    ret_t (*handleData)(const idle_info_t *idle);
    void  (*setHandleDataEntry)(PNetServer this, ret_t (*handleData)(const idle_info_t *idle));
}NetServer, *PNetServer;

void NetServerRegister(PNetServer this);

#endif