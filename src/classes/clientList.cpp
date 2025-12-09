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

    int sendAll(const string& message) {
        lock_guard<mutex> lock(mtx);
        for (auto& client : clients) {
            if (client.sendMessage(message) == -1) {
                cerr << "Error sending message to Client ID: " << client.clientID << endl;
                return -1;
            }
        }
        return 0;
    }

    size_t size() {
        lock_guard<mutex> lock(mtx);
        return clients.size();
    }
};

#endif