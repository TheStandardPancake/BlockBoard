#include <iostream>
#include <WS2tcpip.h>
#include <string>

#pragma comment (lib, "ws2_32")

using namespace std;

#ifndef P2P_H
#define P2P_H


/** THIS HEADER FILE IS USED FOR CONNECTION IN THE P2P NETWORK AND PROVIDES THE FOLLOWING FUNCTIONS
* searchNodes() --> This will scan through the internet to find any computers acting as nodes in the p2p network, it will exchange any known other node ip's between both computers.
* Client( targetIP(string) ) --> This will take the ip given and attempt to connect to it on the port <targetPORT>. The IP must be given in the form "X.X.X.X" as a string where each X is one or more digits.
* Server() --> This will setup a server running on the local computer that accepts incomming connections on port <targetPORT>.
*/




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

void Client(string targetIP)
{
    // Initialize WinSock
    WSAData data;
    WORD ver = MAKEWORD(2, 2);
    int wsResult = WSAStartup(ver, &data);
    if (wsResult != 0)
    {
        cerr << "Can't start Winsock, Err #" << wsResult << endl;
        return;
    }

    // Create socket
    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET)
    {
        cerr << "Can't create socket, Err #" << WSAGetLastError() << endl;
        WSACleanup();
        return;
    }

    // Fill in a hint structure
    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(targetPORT);
    inet_pton(AF_INET, targetIP.c_str(), &hint.sin_addr);

    // Connect to server
    int connResult = connect(sock, (sockaddr*)&hint, sizeof(hint));
    if (connResult == SOCKET_ERROR)
    {
        cerr << "Can't connect to server, Err #" << WSAGetLastError() << endl;
        closesocket(sock);
        WSACleanup();
        return;
    }

    // Do-while loop to send and receive data
    char buf[4096];
    string userInput;

    do
    {
        // Prompt the user for some text
        cout << "> ";
        getline(cin, userInput);

        if (userInput.size() > 0)		// Make sure the user has typed in something
        {
            // Send the text
            int sendResult = send(sock, userInput.c_str(), userInput.size() + 1, 0);
            if (sendResult != SOCKET_ERROR)
            {
                // Wait for response
                ZeroMemory(buf, 4096);
                int bytesReceived = recv(sock, buf, 4096, 0);
                if (bytesReceived > 0)
                {
                    // Echo response to console
                    cout << "SERVER> " << string(buf, 0, bytesReceived) << endl;
                }
            }
        }

    }
    while (userInput.size() > 0);

    // Gracefully close down everything
    closesocket(sock);
    WSACleanup();
}

//--------------------------------------------------------------host a tcp server~~~~~~~~~~~~~~~~~~~~~~~~~~~<<

void Server()
{
    // Initialze winsock
    WSADATA wsData;
    WORD ver = MAKEWORD(2, 2);

    int wsOk = WSAStartup(ver, &wsData);
    if (wsOk != 0)
    {
        cerr << "Can't Initialize winsock! Quitting" << endl;
        return;
    }

    // Create a socket
    SOCKET listening = socket(AF_INET, SOCK_STREAM, 0);
    if (listening == INVALID_SOCKET)
    {
        cerr << "Can't create a socket! Quitting" << endl;
        return;
    }

    // Bind the ip address and port to a socket
    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(targetPORT);
    hint.sin_addr.S_un.S_addr = INADDR_ANY; // Could also use inet_pton ....

    bind(listening, (sockaddr*)&hint, sizeof(hint));

    // Tell Winsock the socket is for listening
    listen(listening, SOMAXCONN);

    // Wait for a connection
    sockaddr_in client;
    int clientSize = sizeof(client);

    SOCKET clientSocket = accept(listening, (sockaddr*)&client, &clientSize);

    char host[NI_MAXHOST];		// Client's remote name
    char service[NI_MAXSERV];	// Service (i.e. port) the client is connect on

    ZeroMemory(host, NI_MAXHOST); // same as memset(host, 0, NI_MAXHOST);
    ZeroMemory(service, NI_MAXSERV);

    if (getnameinfo((sockaddr*)&client, sizeof(client), host, NI_MAXHOST, service, NI_MAXSERV, 0) == 0)
    {
        cout << host << " connected on port " << service << endl;
    }

    // Close listening socket
    closesocket(listening);

    // While loop: accept and echo message back to client
    char buf[4096];

    while (true)
    {
        ZeroMemory(buf, 4096);

        // Wait for client to send data
        int bytesReceived = recv(clientSocket, buf, 4096, 0);
        if (bytesReceived == SOCKET_ERROR)
        {
            cerr << "Error in recv(). Quitting" << endl;
            break;
        }

        if (bytesReceived == 0)
        {
            cout << "Client disconnected " << endl;
            break;
        }

        cout << string(buf, 0, bytesReceived) << endl;

        // Echo message back to client
        send(clientSocket, buf, bytesReceived + 1, 0);

    }

    // Close the socket
    closesocket(clientSocket);

    // Cleanup winsock
    WSACleanup();
}
} //namespace p2p

#endif // P2P_H
