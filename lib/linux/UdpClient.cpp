#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include "../../inc/linux/UdpClient.h"

void UdpClient::Connect(char* ipStr, int portNum)
{
    if (this->Active)
    {
        close(this->Client);
        memset(&this->destAddr, 0x0, sizeof(struct sockaddr_in));
    }

    this->destAddr.sin_family      = AF_INET;
    this->destAddr.sin_port        = htons(portNum);
    this->destAddr.sin_addr.s_addr = inet_addr(ipStr);

    if ((this->Client = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
    {
        printf("socket create err!\n");
        close(this->Client);
        return ;
    }

    this->Active = 1;
}

void UdpClient::Send(char* data_addr, int data_len)
{
    if (this->Active < 1)
        return;

    if (sendto(this->Client, (caddr_t)data_addr, data_len, 0, (struct sockaddr *)&this->destAddr, sizeof(this->destAddr)) == -1)
    {
        printf("udp sendto %s @ %d failed!\n", inet_ntoa(this->destAddr.sin_addr), this->destAddr.sin_port);
        perror("");
        close(this->Client);
        return;
    }
}

void UdpClient::Sendto(char* data_addr, int data_len, char* ipStr, int portNum)
{
    struct sockaddr_in addr_dest;
    int reuse_port = 1;

    addr_dest.sin_family      = AF_INET;
    addr_dest.sin_port        = htons(portNum);
    addr_dest.sin_addr.s_addr = inet_addr(ipStr);


    setsockopt(this->Client, SOL_SOCKET, SO_REUSEADDR | SO_BROADCAST, &reuse_port, sizeof(int));

    if (sendto(this->Client, (caddr_t)data_addr, data_len, 0, (struct sockaddr *)&addr_dest, sizeof(addr_dest)) == -1)
    {
        printf("udp sendto %s @ %d failed!\n", inet_ntoa(addr_dest.sin_addr), addr_dest.sin_port);
        perror("");
        close(this->Client);
        return;
    }
}

int UdpClient::Receive(char* data_addr, int data_len)
{
    socklen_t len = sizeof(this->servAddr);
    int ret = 0;

    ret =  recvfrom(this->Client, (caddr_t) data_addr, data_len, 0, (struct sockaddr *)&this->servAddr, (socklen_t*) &len);

    if (ret > 0)
    {
        memcpy(&this->destAddr, &this->servAddr, sizeof(struct sockaddr_in));
        this->Active = 1;
    }

    return ret;
}

#if 0

#endif
