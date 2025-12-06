#include <WinSock2.h>

int initializeSocket(SOCKET *sock, IPPROTO protocol) {
    // Create Socket
    *sock = socket(AF_INET, SOCK_STREAM, protocol);

    // Catch Exception
    if(*sock == INVALID_SOCKET) return -1;
    return 0;
}

int bindingSocket(SOCKET *sock, u_short port) {
    sockaddr_in hint {};
    hint.sin_family = AF_INET;
    hint.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
    hint.sin_port = htons(port);

    return bind(*sock, reinterpret_cast<sockaddr*>(&hint), sizeof(hint));
}