#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string>

#ifndef P2P_H
#define P2P_H

/** THIS HEADER FILE IS USED FOR CONNECTION IN THE P2P NETWORK AND PROVIDES THE FOLLOWING FUNCTIONS
* searchNodes() --> This will scan through the internet to find any computers acting as nodes in the p2p network, it will exchange any known other node ip's between both computers.
* Client( targetIP(string) ) --> This will take the ip given and attempt to connect to it on the port <targetPORT>. The IP must be given in the form "X.X.X.X" as a string where each X is one or more digits.
* Server( targetIP(string) ) --> This will setup a server running on the local computer that accepts incomming connections on port <targetPORT>. The IP is that of the local computers and must also be a string like "X.X.X.X".
*/




namespace p2p
{
class P2p()
{
public:
    final int targetPORT = 8334; //this will be the port that the network communicaes on

    //--------------------------------------------------------------search for other nodes on the p2p network~~~~~~~~~~~~~~~~~~~~~~~~~~~<<
    void searchNodes()
    {
        //iterate through the internet looking for computers with port 8334 open

        //send a message like "Vibe Check" and if it gets a response of "Vibing" then it is a node of the p2p network

    }

    //--------------------------------------------------------------create a connection to a tcp server~~~~~~~~~~~~~~~~~~~~~~~~~~~<<
    void Client(String targetIP)
    {
        //setup all the stuffs

        //close connection once complete
    }

    //--------------------------------------------------------------host a tcp server~~~~~~~~~~~~~~~~~~~~~~~~~~~<<
    void Server(String targetIP)
    {
        //request WinSock

        //Create socket

        //set socket to listening

        //wait for incoming connections

        //connect

        //while connected, send messages back and forth

        //close connection once complete

    }

};
} //namespace p2p

#endif // P2P_H
