#include <stdio.h>
#include "../inc/linux/Logger.h"
#include "../inc/linux/UdpClient.h"
#include "../inc/linux/TcpClass.h"

int main(int argc, char const *argv[])
{
    double version = 1.0;
    char data[50];
    //TcpClient *tc = new TcpClient("127.0.0.1", 9000);
    TcpListener *tl = new TcpListener(9000);
    tl->Start();
    TcpClient *tc = tl->AcceptTcpClient();
    while(tc->Receive(data, 50) > 0)
        LogDbg("data %s", data);
    return 0;
}
