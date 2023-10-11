#pragma once

#include <SFML/Network.hpp>
#include <vector>
class Server
{
private:
    sf::TcpListener listener;
    std::vector<sf::TcpSocket*> clientsSockets;
    unsigned short port;
public:
    Server(unsigned short);
    void connectedClient(std::vector<sf::TcpSocket*>* sockets);
    void disconectClient(sf::TcpSocket* socketPtr, size_t iter);
    void recivePacket(sf::TcpSocket* socketPtr, size_t iter);
    void broadcastPacket(sf::Packet& packet, sf::IpAddress addres, unsigned short port);
    void managePackets();
    void Run();
};
