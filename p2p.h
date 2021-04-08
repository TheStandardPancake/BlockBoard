#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string>

#ifndef P2P_H
#define P2P_H


namespace p2p
{
class P2p()
{
public:
    int targetIP; //The IP it will attempt to connect to first
    int targetPORT = 8334; //this will be the port that the network communicaes on

    //tcp socket setup
    int socketfd = socket();

    //search for other nodes on the p2p network
    void searchNodes()
    {
        asd
    }

    //create a connection to tcp server

    //create a hosting tcp server

    //disconnect connection to tcp server

    //shutdown hosting tcp server
};
} //namespace p2p

#endif // P2P_H
