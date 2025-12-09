#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <WinSock2.h>
using namespace std;

#pragma comment (lib, "ws2_32.lib")

void receiveMessages(SOCKET clientSocket) {
    char buffer[512];
    while (true) {
        int bytesReceived = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
        if (bytesReceived > 0) {
            buffer[bytesReceived] = '\0';
            cout << "[Server] " << buffer << endl;
        } else if (bytesReceived == 0) {
            cout << "Connection closed by server." << endl;
            break;
        } else {
            cerr << "Receive error." << endl;
            break;
        }
    }
}

int main() {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        cerr << "WSAStartup failed." << endl;
        return -1;
    }
    cout << "Sample Client Application Started." << endl;

    // Get username from user
    string username;
    cout << "Enter your username: ";
    getline(cin, username);

    // Create client socket
    SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == INVALID_SOCKET) {
        cerr << "Socket creation failed." << endl;
        WSACleanup();
        return -1;
    }

    cout << "Connecting to server..." << endl;

    // Define server address
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(9999);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    // Connect to server
    if (connect(clientSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        cerr << "Connection to server failed." << endl;
        closesocket(clientSocket);
        WSACleanup();
        return -1;
    }

    // Send username to server
    int bytesSent = send(clientSocket, username.c_str(), username.size(), 0);
    if (bytesSent == SOCKET_ERROR) {
        cerr << "Sending username failed." << endl;
        closesocket(clientSocket);
        WSACleanup();
        return -1;
    }

    cout << "Connected to server as " << username << "." << endl;

    // Start thread to receive messages from server
    thread receiverThread(receiveMessages, clientSocket);
    receiverThread.detach();

    while (true) {
        string message;
        getline(cin, message);
        if (message == "exit") break;

        send(clientSocket, message.c_str(), message.size(), 0);
    }

    closesocket(clientSocket);
    WSACleanup();
    return 0;
}