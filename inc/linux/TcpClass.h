#ifndef __INC_LINUX_TCPCLASS__
#define __INC_LINUX_TCPCLASS__

#include <netinet/in.h>

class TcpClient
{
public:
    int   Active;
    int   Available;
    int   Client;
    int   Connected;
    struct sockaddr_in remoteAddr;
    struct sockaddr_in localAddr;

    TcpClient()
    {

    };

    void setClient(int fd)
    {
        this->Client = fd;
    };

    int getClient()
    {
        return this->Client;
    };

    TcpClient(const char* serverIP, int serverPort);

    int  GetError   ( );
    void Connect    (const char* ipStr, int portNum);
    void Disconnect ( );
    int  Send       (char* data_addr, int data_len);
    int  Receive    (char* data_addr, int data_len);
};



class TcpListener
{
private:
    int ListenSock;

public:
    TcpListener(int portNum);
    void Start ();
    void Stop  ();
    TcpClient* AcceptTcpClient ();
};


#endif
