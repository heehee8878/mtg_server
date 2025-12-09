#ifndef CLIENT_DATA_CPP
#define CLIENT_DATA_CPP

#include <WinSock2.h>
#include <thread>
#include <string>
#include <vector>

using namespace std;

class ClientData {
public:
    int clientID;
    SOCKET clientSocket;

    string username;

    ClientData(int id, SOCKET sock) {
        clientID = id;
        clientSocket = sock;
    }

    int setUsername(const string& name) {
        if(username.empty()) {
            username = name;
            return 0;
        } else return -1;
    }

    int recvMessage(string* message, int bufferSize = 512) {
        vector<char> buffer(bufferSize);
        int bytesReceived = recv(clientSocket, buffer.data(), buffer.size() - 1, 0);
        if (bytesReceived == SOCKET_ERROR || bytesReceived == 0) return -1;

        buffer[bytesReceived] = '\0';
        *message = string(buffer.data());
        return 0;
    }

    int sendMessage(const string& message) {
        int bytesSent = send(clientSocket, message.c_str(), message.size(), 0);
        if (bytesSent == SOCKET_ERROR) return -1;
        return 0;
    }
};

#endif