#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include "../../inc/linux/TcpClass.h"

TcpClient::TcpClient(const char* serverIP, int serverPort)
{
    this->remoteAddr.sin_family = AF_INET;
    inet_pton(AF_INET, serverIP, &this->remoteAddr.sin_addr);
    this->remoteAddr.sin_port = htons(serverPort);

    if ((this->Client = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        printf("socket create err!\n");
        this->Active = 0;
    }

    this->Connect(serverIP, serverPort);
}

void TcpClient::Connect(const char* ipStr, int portNum)
{
    if (this->Connected)
        return;

    this->remoteAddr.sin_family = AF_INET;
    inet_pton(AF_INET, ipStr, &this->remoteAddr.sin_addr);
    this->remoteAddr.sin_port = htons(portNum);

    if (0 == this->Client)
    {
        this->Client = socket(AF_INET, SOCK_STREAM, 0);
    }

    if (0 == connect(this->Client, (struct sockaddr *)&this->remoteAddr, sizeof(this->remoteAddr)))
        this->Connected = 1;
    else
    {
        this->Connected = 0;
        printf("%s: connect to %s at %d failed\n", __func__, inet_ntoa(this->remoteAddr.sin_addr), ntohs(this->remoteAddr.sin_port));
        perror("");
        close(this->Client);
    }
}

void TcpClient::Disconnect()
{
    if ((this != NULL) && (this->Client != 0))
    {
        shutdown(this->Client, SHUT_RDWR);
        close(this->Client);
        this->Client = 0;
        this->Connected = 0;
    }
}

int TcpClient::GetError()
{
    int optval;
    int optlen = sizeof(int);

    getsockopt(this->Client, SOL_SOCKET, SO_ERROR, &optval, (socklen_t*) &optlen);

    return optval;
}

int TcpClient::Send(char* data_addr, int data_len)
{
    int length = 0;

    if (this->GetError())
    {
        this->Connected = 0;
        return -1;
    }

    if (this->Connected)
    {
        length = write(this->Client, data_addr, data_len);

        if (length == data_len)
            return length;
    }

    return -1;
}

int TcpClient::Receive(char* data_addr, int data_len)
{
    int length = 0;

    if (this->GetError())
    {
        this->Connected = 0;
        return -1;
    }

    if (this->Connected)
    {
        length = read(this->Client, data_addr, data_len);

        if (length < 0)
        {
            this->Connected = 0;
            return -1;
        }

        if (length <= data_len)
            return length;
    }

    return -1;
}

TcpListener::TcpListener(int portNum)
{
    struct sockaddr_in SelfAddrIn;

    memset(&SelfAddrIn, 0x0, sizeof(SelfAddrIn));
    SelfAddrIn.sin_family = AF_INET;
    SelfAddrIn.sin_addr.s_addr = htonl(INADDR_ANY);
    SelfAddrIn.sin_port = htons(portNum);

    if ((this->ListenSock = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        printf("socket create err!\n");
    }

    if (bind(this->ListenSock, (struct sockaddr*)&SelfAddrIn, sizeof(struct sockaddr_in))<0)
    {
        printf("socket bind error\n");
        close(this->ListenSock);
    }
}

void TcpListener::Start()
{
    if(listen(this->ListenSock, 10) < 0)
    {
        printf("listen failed\n");
        close(this->ListenSock);
    }
}

void TcpListener::Stop()
{

}

TcpClient* TcpListener::AcceptTcpClient()
{
    TcpClient* tcpCon = new TcpClient();
    socklen_t len = sizeof(struct sockaddr_in);

    tcpCon->setClient(accept(this->ListenSock, (struct sockaddr*)&tcpCon->localAddr, &len));

    if(tcpCon->getClient() < 0)
    {
        printf("accept failed\n");
        return NULL;
    }

    tcpCon->Connected = 1;

    return tcpCon;
}

