#ifndef __INC_LINUX_UDPCLIENT__
#define __INC_LINUX_UDPCLIENT__

#include <netinet/in.h>

class UdpClient
{

private:
    int   Active;
    int   Available;
    int   Client;
    struct sockaddr_in destAddr;
    struct sockaddr_in servAddr;

public:
    UdpClient(int portNum)
    {
        this->servAddr.sin_family      = AF_INET;
        this->servAddr.sin_port        = htons(portNum);
        this->servAddr.sin_addr.s_addr = htonl(INADDR_ANY);

        if ((this->Client = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
        {
            printf("socket create err!\n");
            this->Active = 0;
        }

        if (bind(this->Client, (struct sockaddr *)(&this->servAddr), sizeof(struct sockaddr)) == -1)
        {
            printf("connect socket bind err!\n");
        }

    };

    void Connect(char* ipStr, int portNum);
    void Send   (char* data_addr, int data_len);
    void Sendto (char* data_addr, int data_len, char* ipStr, int portNum);
    int  Receive(char* data_addr, int data_len);
};

#endif
