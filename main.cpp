#include <iostream>
#include <vector>
#include <thread>
#include <WinSock2.h>

using namespace std;

#include "src/classes/clientList.cpp"
#include "src/utils/socketInitializer.cpp"
#include "src/components/acceptClientRoop.cpp"

#pragma comment (lib, "ws2_32.lib")

const u_short SERVER_PORT = 9999;

int main(int argc, char *argv[]) {
    #pragma region WSASetup
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        cerr << "WSA Setup has Error." << endl;
        return -1;
    }
    #pragma endregion

    cout << "> Starting Server..." << endl;

    SOCKET serverSocket;
    ClientList clientSockets;

    try {
        if (initializeSocket(&serverSocket, IPPROTO_TCP) == -1)         throw runtime_error("Can't Create Socket.");
        if (bindingSocket(&serverSocket, SERVER_PORT) == SOCKET_ERROR)  throw runtime_error("Can't Binding Socket.");
        if (listen(serverSocket, SOMAXCONN) == SOCKET_ERROR)            throw runtime_error("Can't Listen a Socket.");

        thread acceptClientThread(acceptClientRoop, &serverSocket, &clientSockets);
        acceptClientThread.join();

        cout << "The Program has Ended." << endl;
        closesocket(serverSocket);
        WSACleanup();
        return 0;

    }
    catch (const exception& e) {
        cerr << "Caught Exception: " << e.what() << endl;
        closesocket(serverSocket);
        WSACleanup();
        return -1;
    }
}