#ifndef ACCEPT_CLIENT_ROOP_CPP
#define ACCEPT_CLIENT_ROOP_CPP

#include <iostream>
#include <WinSock2.h>
#include <thread>
#include <vector>

#include "clientList.cpp"
#include "clientHandler.cpp"

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
        int clientID = static_cast<int>(clientList->size()) + 1;
        ClientData newClient(clientID, clientSocket);
        clientList->addClient(newClient);

        // Start client handler thread - pass by value (copy is safe)
        thread clientHandlerThread(clientHandler, newClient, clientList);
        clientHandlerThread.detach();

        cout << "> Client Handler Thread Started for Client ID: " << clientID << endl;
        cout << "> Total Connected Clients: " << clientList->size() << endl;
    }
}

#endif