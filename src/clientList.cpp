#ifndef CLIENT_LIST_CPP
#define CLIENT_LIST_CPP

#include <iostream>
#include <vector>
#include <mutex>

#include "clientData.cpp"

using namespace std;

class ClientList
{
private:
    vector<ClientData> clients;
    mutex mtx;
public:
    operator vector<ClientData>&() {
        return clients;
    }

    void addClient(const ClientData& client) {
        lock_guard<mutex> lock(mtx);
        clients.push_back(client);
    }

    void removeClientByID(int clientID) {
        lock_guard<mutex> lock(mtx);
        clients.erase(
            remove_if(clients.begin(), clients.end(),
                      [&](const ClientData& cd) { return cd.clientID == clientID; }),
            clients.end()
        );
    }

    size_t size() {
        lock_guard<mutex> lock(mtx);
        return clients.size();
    }
};

#endif