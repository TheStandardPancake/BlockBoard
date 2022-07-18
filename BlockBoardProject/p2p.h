/**
*Copyright 2021 Boyd Kirkman
*
*This file is part of BlockBoard.
*
*    BlockBoard is free software: you can redistribute it and/or modify
*    it under the terms of the GNU General Public License as published by
*    the Free Software Foundation, either version 3 of the License, or
*    (at your option) any later version.
*
*    BlockBoard is distributed in the hope that it will be useful,
*    but WITHOUT ANY WARRANTY; without even the implied warranty of
*    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*    GNU General Public License for more details.
*
*    You should have received a copy of the GNU General Public License
*    along with BlockBoard.  If not, see <https://www.gnu.org/licenses/>.
*/

/** THIS HEADER FILE IS USED FOR CONNECTION IN THE P2P NETWORK AND PROVIDES THE FOLLOWING FUNCTIONS:
* searchNodes() --> This will scan through the internet to find any computers acting as nodes in the p2p network,
*                   it will exchange any known other node ip's between both computers.
*
* Client( targetIP(string), serverShare(boolean), blockChain(boolean) ) --> This will take the ip given and attempt to
*                   connect to it on the port <targetPORT>. serverShare dictates if it will share server lists. blockChain is
*                   the state of if the client already has a blockchain or not.
*                   The IP must be given in the form "X.X.X.X" as a string where each X is one or more digits.
*
* Server( ID(string), SERVER_SHARE(boolean), COLLECTING(boolean) ) --> This will setup a server running on the local computer
*                   that accepts incomming connections on port <targetPORT>. ID can be either "MINER" or "NODE",
*                   SERVER_SHARE is wether or not the server is willing to share its other known nodes/miners,
*                   COLLECTING is wether or not the server is both mining, and accepting transactions to form blocks.
*/

#include <iostream>
#include <WS2tcpip.h>
#include <string>
#include <vector>
#include "pystring.h"
#include "fileRead.h"

using namespace std;

#ifndef P2P_H
#define P2P_H

namespace p2p
{
int targetPORT = 8334; //this will be the port that the network communicaes on

//--------------------------------------------------------------search for other nodes on the p2p network~~~~~~~~~~~~~~~~~~~~~~~~~~~<<

void searchNodes()
{
    //iterate through the internet looking for computers with port 8334 open

    //send a message like "Vibe Check" and if it gets a response of "Vibing" then it is a node of the p2p network

}

//--------------------------------------------------------------create a connection to a tcp server~~~~~~~~~~~~~~~~~~~~~~~~~~~<<

//$$$$$$$$$$$$$$$$$$$$$$$$$$$$ Anoying re-implementation of a library that isn't working (inet_pton isn't working from <WS2tcpip.h>) $$$$$$$$$$$$$$$$$$$$$$$$$$$$$
#define NS_INADDRSZ  4
#define NS_IN6ADDRSZ 16
#define NS_INT16SZ   2

int inet_pton4(const char *src, in_addr *dst)
{
    uint8_t tmp[NS_INADDRSZ], *tp;

    int saw_digit = 0;
    int octets = 0;
    *(tp = tmp) = 0;

    int ch;
    while ((ch = *src++) != '\0')
    {
        if (ch >= '0' && ch <= '9')
        {
            uint32_t n = *tp * 10 + (ch - '0');

            if (saw_digit && *tp == 0)
                return 0;

            if (n > 255)
                return 0;

            *tp = n;
            if (!saw_digit)
            {
                if (++octets > 4)
                    return 0;
                saw_digit = 1;
            }
        }
        else if (ch == '.' && saw_digit)
        {
            if (octets == 4)
                return 0;
            *++tp = 0;
            saw_digit = 0;
        }
        else
            return 0;
    }
    if (octets < 4)
        return 0;

    memcpy(dst, tmp, NS_INADDRSZ);

    return 1;
}

//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ End of ugly code grafting $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$


void Client(string targetIP, boolean shareServers, boolean blockChain)
{
    // Initialize WinSock
    WSAData data;
    WORD ver = MAKEWORD(2, 2);
    int wsResult = WSAStartup(ver, &data);
    if (wsResult != 0)
    {
        cerr << "Can't start Winsock" << wsResult << endl;
        return;
    }

    // Create socket
    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET)
    {
        cerr << "Can't create socket" << WSAGetLastError() << endl;
        WSACleanup();
        return;
    }

    // Fill in a hint structure
    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(targetPORT);
    inet_pton4(targetIP.c_str(), &hint.sin_addr);

    // Connect to server
    int connResult = connect(sock, (sockaddr*)&hint, sizeof(hint));
    if (connResult == SOCKET_ERROR)
    {
        cerr << "Can't connect to server" << WSAGetLastError() << endl;
        closesocket(sock);
        WSACleanup();
        return;
    }

    char buf[4096];

    //Querying the Server

    //Send an ID_CHECK
    string query = "ID_CHECK";
    send(sock, query.c_str(), query.size() + 1, 0);
    //Wait for response
    ZeroMemory(buf, 4096);
    int bytesReceived = recv(sock, buf, 4096, 0);
    string recievedString = string(buf, 0, bytesReceived);
    if ((recievedString == "NODE") || (recievedString == "MINER"))
    {
        //write IP to the list of known IPs
        fileRead::writeIP(targetIP);
    }

    //Exchange IP lists
    if (shareServers)
    {
        string query = "SERVER_SHARE";
        send(sock, query.c_str(), query.size() + 1, 0);
        //Wait for response
        ZeroMemory(buf, 4096);
        int bytesReceived = recv(sock, buf, 4096, 0);
        string recievedString = string(buf, 0, bytesReceived);
        if (recievedString == "YES")
        {
            //send over the ip's
            vector<string> ipSends = fileRead::nodeIPs;
            string sendingString = "";
            for (vector<string>::iterator ip = ipSends.begin(); ip != ipSends.end(); ip++)
            {
                sendingString = sendingSring+*ip+","
            }
            send(sock, sendingString.c_str(), ID.size() + 1, 0);
        }
    }

    //Exchange the Block Chain
    string query = "CHAIN_EXCH";
    send(sock, query.c_str(), query.size() + 1, 0);
    //Wait for response
    ZeroMemory(buf, 4096);
    int bytesReceived = recv(sock, buf, 4096, 0);
    string recievedString = string(buf, 0, bytesReceived);
    //Send the current block chain (if there is one)
    if (blockChain)
    {
        continue;
    }

    // Gracefully close down everything
    closesocket(sock);
    WSACleanup();
}

//--------------------------------------------------------------host a tcp server~~~~~~~~~~~~~~~~~~~~~~~~~~~<<

void Server(string ID, boolean SERVER_SHARE, boolean COLLECTING)
{
    // Initialze winsock
    WSADATA wsData;
    WORD ver = MAKEWORD(2, 2);

    int wsOk = WSAStartup(ver, &wsData);
    if (wsOk != 0)
    {
        cerr << "Can't Initialize winsock" << endl;
        return;
    }

    // Create a socket
    SOCKET listening = socket(AF_INET, SOCK_STREAM, 0);
    if (listening == INVALID_SOCKET)
    {
        cerr << "Can't create a socket" << endl;
        return;
    }

    // Bind the ip address and port to a socket
    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(targetPORT);
    hint.sin_addr.S_un.S_addr = INADDR_ANY;

    bind(listening, (sockaddr*)&hint, sizeof(hint));

    // Tell Winsock the socket is for listening
    listen(listening, SOMAXCONN);

    // Wait for a connection
    sockaddr_in client;
    int clientSize = sizeof(client);

    SOCKET clientSocket = accept(listening, (sockaddr*)&client, &clientSize);

    char host[NI_MAXHOST];		// Client's remote name
    char service[NI_MAXSERV];	// Service (i.e. port) the client is connect on

    ZeroMemory(host, NI_MAXHOST);
    ZeroMemory(service, NI_MAXSERV);

    if (getnameinfo((sockaddr*)&client, sizeof(client), host, NI_MAXHOST, service, NI_MAXSERV, 0) == 0)
    {
        cout << host << " connected on port " << service << endl;
    }

    // Close listening socket
    closesocket(listening);

    char buf[4096];

    boolean exchangingInfo = true;

    while (exchangingInfo)
    {
        ZeroMemory(buf, 4096);

        // Wait for client to send data
        int bytesReceived = recv(clientSocket, buf, 4096, 0);
        if (bytesReceived == SOCKET_ERROR)
        {
            cerr << "Error in recv()" << endl;
            exchangingInfo = false;
            break;
        }
        if (bytesReceived == 0)
        {
            cout << "Client disconnected" << endl;
            exchangingInfo = false;
            break;
        }

        //Handle the various different requests
        string recievedString = string(buf, 0, bytesReceived);

        if (recievedString == "ID_CHECK")
        {
            //send back as to if this is a "MINER" or a "NODE"
            send(clientSocket, ID.c_str(), ID.size() + 1, 0);
        }
        else if (recievedString == "SERVER_SHARE")
        {
            //send back as to if this server is willing to share its other known nodes -- sends either "YES" or "NO"
            string yes = "YES";
            string no = "NO";
            if (SERVER_SHARE)
            {
                send(clientSocket, yes.c_str(), yes.size() + 1, 0); //send "YES"

                //Await response -- should expect a series of ip addresses
                ZeroMemory(buf, 4096);
                int bytesReceived = recv(clientSocket, buf, 4096, 0);
                if (bytesReceived == SOCKET_ERROR)
                {
                    cerr << "Error in recv()" << endl;
                    exchangingInfo = false;
                    break;
                }
                if (bytesReceived == 0)
                {
                    cout << "Client disconnected" << endl;
                    exchangingInfo = false;
                    break;
                }

                //split the recieved string into a list
                string recievedString = string(buf, 0, bytesReceived);
                vector <string> recievedIPList;
                pystring::split(recievedString, recievedIPList, ",");

                //for each recieved IP check if it is in the list already and if not add it
                for(vector<string>::iterator ip = recievedIPList.begin(); ip != recievedIPList.end(); ip++)
                {
                    if (!fileRead::inIPList(ip))
                    {
                        fileRead::writeIP(ip);
                    }
                }

                //Send a series of ip addresses
                vector<string> ipSends = fileRead::nodeIPs;
                string sendingString = "";
                for (vector<string>::iterator ip = ipSends.begin(); ip != ipSends.end(); ip++)
                {
                    sendingString = sendingSring+*ip+","
                }
                send(clientSocket, sendingString.c_str(), sendingString.size() + 1, 0);
            }
            else
            {
                send(clientSocket, no.c_str(), no.size() + 1, 0); //send "NO"
            }
        }
        else if (recievedString == "CHAIN_EXCH")
        {
            //Send the block chain somehow

            //Recieve the block chain somehow
        }
        else if (recievedString == "TRANSACTION")
        {
            //send back as to if this program is "COLLECTING" transactions or if it is "CLOSED"
            string yes = "COLLECTING";
            string no = "CLOSED";
            if (COLLECTING)
            {
                send(clientSocket, yes.c_str(), yes.size() + 1, 0); //send "COLLECTING"
            }
            else
            {
                send(clientSocket, no.c_str(), no.size() + 1, 0); //send "CLOSED"
            }

            //if "COLLECTING" then recieve the transaction
            ZeroMemory(buf, 4096);
            int bytesReceived = recv(clientSocket, buf, 4096, 0);
            if (bytesReceived == SOCKET_ERROR)
            {
                cerr << "Error in recv()" << endl;
                exchangingInfo = false;
                break;
            }
            if (bytesReceived == 0)
            {
                cout << "Client disconnected" << endl;
                exchangingInfo = false;
                break;
            }
            cout << string(buf, 0, bytesReceived) << endl;
        }
        else if (recievedString == "DISCONNECT")
        {
            exchangingInfo = false;
            break;
        }
        else //handle an invalid request
        {
            string invalid = "UNKOWN_REQUEST";
            send(clientSocket, invalid.c_str(), invalid.size() + 1, 0);
        }

    }

// Close the socket
    closesocket(clientSocket);

// Cleanup winsock
    WSACleanup();
}
} //namespace p2p

#endif // P2P_H
