#ifndef CLIENT_HANDLER_CPP
#define CLIENT_HANDLER_CPP

#include <iostream>
#include <vector>
#include <WinSock2.h>

#include "src/classes/clientList.cpp"

using namespace std;

int clientHandler(ClientData clientData, ClientList* clientList) {
    cout << "> Client Handler Started." << endl;

    // Receive username at first
    string username;
    if (clientData.recvMessage(&username, 256) == -1) {
        cerr << "Error receiving username or client disconnected." << endl;
        closesocket(clientData.clientSocket);
        clientList->removeClientByID(clientData.clientID);
        return -1;
    }
    cout << "> Client Username: " << username << endl;

    while (true) {
        string message;
        if (clientData.recvMessage(&message) == -1) break;
        cout << "> [" << username << "," << clientData.clientID << "] " << ": " << message << endl;
        string response = "["+username+"] " + message;
        if (clientList->sendAll(response) == -1) break;
    }

    cout << "> Client ID " << clientData.clientID << " disconnected." << endl;
    closesocket(clientData.clientSocket);
    clientList->removeClientByID(clientData.clientID);

    return 0;
}

#endif