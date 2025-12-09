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
    int nextClientID = 1;
public:
    operator vector<ClientData>&() {
        return clients;
    }

    // Can rejected duplicate clientID
    int addClient(const ClientData& client) {
        lock_guard<mutex> lock(mtx);
        for(auto& existingClient : clients) {
            if(existingClient.clientID == client.clientID) return -1;
        }
        clients.push_back(client);
        return 0;
    }

    ClientData createClient(SOCKET clientSocket) {
        lock_guard<mutex> lock(mtx);
        int clientID = nextClientID++;
        ClientData newClient(clientID, clientSocket);
        clients.push_back(newClient);
        return newClient;
    }

    int removeClientByID(int clientID) {
        lock_guard<mutex> lock(mtx);
        for (auto it = clients.begin(); it != clients.end(); ++it) {
            if (it->clientID == clientID) {
                clients.erase(it);
                return 0;
            }
        }
        return -1;
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