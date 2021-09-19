//
// 这一块 由于各个系统c++的网络库不一样  所以通讯核心要分开写
//

#ifndef CPPSUPPORT_SOCKET_ENGINE_ANDROID_H
#define CPPSUPPORT_SOCKET_ENGINE_ANDROID_H

#include <sys/types.h>
#include <sys/socket.h>

//sockaddr_un
#include <sys/un.h>

//htons,sockaddr_in
#include <netinet/in.h>
//inet_ntop
#include <arpa/inet.h>
//close,unlink
#include <unistd.h>
#include "../utils/utils_android.h"

//sock用到的一些公用方法
//创建一个socket:socket()
static int NewTcpSocket() {

    LogMessage("Constructing a new TCP socket...");
    int tcpSocket = socket(PF_INET, SOCK_STREAM, 0);
    LogMessage("Socket is %hu.",tcpSocket);
    if (-1 == tcpSocket) {
        ThrowErrnoException("java/io/IOException", errno);
    }

    return tcpSocket;
}

//绑定 bind()
static void BindSocketToPort( int sd,
                             unsigned short port) {
    struct sockaddr_in address;
    //清空结构体
    memset(&address, 0, sizeof(address));

    address.sin_family = PF_INET;
    //Bind to all address
    address.sin_addr.s_addr = htonl(INADDR_ANY);
    //Convert port to network byte order
    address.sin_port = htons(port);
    //Bind socket
    LogMessage( "Binding to port %hu.", port);
    //sockaddr方便函数传递, sockaddr_in方便用户设定, 所以需要的时候在这2者之间进行转换
    if (-1 == bind(sd, (struct sockaddr*) &address, sizeof(address))) {
        ThrowErrnoException("java/io/IOException", errno);
    }

}
//返回当前socket绑定的端口
static unsigned short GetSocketPort(int sd) {
    unsigned short port = 0;
    struct sockaddr_in address;
    socklen_t addressLength = sizeof(address);
    if (-1 == getsockname(sd, (struct sockaddr*) &address, &addressLength)) {
        ThrowErrnoException( "java/io/IOException", errno);
    } else {
        port = ntohs(address.sin_port);
        LogMessage("Binding to the random port %hu.", port);
    }
    return port;
}

//监听 listen()
static void ListenOnSocket( int sd, int backlog) {
    LogMessage("Listening on socket with a baklog of  %d pending connections.",backlog);

    //listen()用来等待参数s 的socket 连线. 参数backlog 指定同时能处理的最大连接要求,
    //如果连接数目达此上限则client 端将收到ECONNREFUSED 的错误.
    //Listen()并未开始接收连线, 只是设置socket 为listen 模式, 真正接收client 端连线的是accept().
    //通常listen()会在socket(), bind()之后调用, 接着才调用accept().

    if (-1 == listen(sd, backlog)) {
        ThrowErrnoException( "java/io/IOException", errno);
    }

}

//根据地址打印IP和端口
static void LogAddress(const char* message,
                       const struct sockaddr_in* address) {
    char ip[INET_ADDRSTRLEN];

    if (NULL == inet_ntop(PF_INET, &(address->sin_addr), ip, INET_ADDRSTRLEN)) {
        ThrowErrnoException("java/io/IOException", errno);
    } else {
        unsigned short port = ntohs(address->sin_port);
        LogMessage("%s %s:%hu", message, ip, port);
    }
}

//accept()
static int AcceptOnSocket(int sd) {
    struct sockaddr_in address;
    socklen_t addressLength = sizeof(address);
    LogMessage( "Waiting for a client connection...");
    int clientSocket = accept(sd, (struct sockaddr*) &address, &addressLength);
    if (-1 == clientSocket) {
        ThrowErrnoException("java/io/IOException", errno);
    } else {
        LogAddress("Client connection from ", &address);
    }
    return clientSocket;
}

//接收 recv()
static ssize_t ReceiveFromSocket(int sd, char* buffer,
                                 size_t bufferSize) {
    LogMessage( "Receiving from the socket... ");
    ssize_t recvSize = recv(sd, buffer, bufferSize - 1, 0);

    if (-1 == recvSize) {
        ThrowErrnoException("java/io/IOException", errno);
    } else {
        //字符串截断
        buffer[recvSize] = NULL;

        if (recvSize > 0) {
            //接收成功，打印
            LogMessage( "Received %d bytes:%s", bufferSize, buffer);
        } else {
            LogMessage("Client disconnected.");
        }
    }

    return recvSize;
}

//发送消息:send()
static ssize_t SendToSocket( int sd,
                            const char* buffer, size_t bufferSize) {
    LogMessage("Sending to the socket... ");
    ssize_t sentSize = send(sd, buffer, bufferSize, 0);

    if (-1 == sentSize) {
        ThrowErrnoException("java/io/IOException", errno);
    } else {
        if (sentSize > 0) {
            LogMessage( "Send %d bytes: %s", sentSize, buffer);
        } else {
            LogMessage("Client disconnected.");
        }
    }

    return sentSize;
}

//链接到服务器 connect()
static void ConnectToAddress(int sd, const char*ip,
                             unsigned short port) {
    LogMessage("Connecting to %s:%hu...", ip, port);

    struct sockaddr_in address;

    memset(&address, 0, sizeof(address));
    address.sin_family = PF_INET;

    //转换ip
    if (0 == inet_aton(ip, &(address.sin_addr))) {
        ThrowErrnoException("java/io/IOException", errno);
    } else {
        address.sin_port = htons(port);
    }

    if (-1 == connect(sd, (const sockaddr*) &address, sizeof(address))) {
        ThrowErrnoException("java/io/IOException", errno);
    } else {
        LogMessage("Connected.");
    }

}

//----------------udp

//创建udp socket
static int NewUdpSocket() {

    LogMessage("Constructing a new UDP socket...");
    int udpSocket = socket(PF_INET, SOCK_DGRAM, 0);

    if (-1 == udpSocket) {
        ThrowErrnoException("java/io/IOException", errno);
    }

    return udpSocket;
}




#endif //CPPSUPPORT_SOCKET_ENGINE_ANDROID_H
