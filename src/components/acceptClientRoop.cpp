#ifndef ACCEPT_CLIENT_ROOP_CPP
#define ACCEPT_CLIENT_ROOP_CPP

#include <iostream>
#include <WinSock2.h>
#include <thread>
#include <vector>

#include "src/classes/clientList.cpp"
#include "src/components/clientHandler.cpp"

using namespace std;

void acceptClientRoop(SOCKET *serverSock, ClientList* clientList) {
    cout << "> Accept Client Roop Started." << endl;
    while (true) {
        sockaddr_in clientSockInfo;
        int clientSize = sizeof(clientSockInfo);

        SOCKET clientSocket = accept(*serverSock, reinterpret_cast<sockaddr*>(&clientSockInfo), &clientSize);
        if (clientSocket == INVALID_SOCKET) {
            int errorCode = WSAGetLastError();

            if(errorCode == WSAEWOULDBLOCK || errorCode == WSAEINTR) continue;
            else throw runtime_error("Fatal Accept Error");
        }

        // Store client data in clientList BEFORE starting thread
        ClientData newClient = clientList->createClient(clientSocket);

        cout << "> Client Handler Thread Started for Client ID: " << newClient.clientID << endl;
        cout << "> Total Connected Clients: " << clientList->size() << endl;

        // Start client handler thread
        thread clientHandlerThread(clientHandler, newClient, clientList);
        clientHandlerThread.detach();
    }
}

#endif