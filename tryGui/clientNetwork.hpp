#pragma once
#include <SFML/Network.hpp>
#include <thread>
#include <string>

class Client
{
private:
    sf::TcpSocket clientsSocket;
    sf::Packet lastPacket;
    bool isConnected = false;
public:
    void connected(const std::string& adress,unsigned short port);
    void sendPacket(sf::Packet& message);
    sf::Socket::Status recivePacket();
    sf::Packet& getPacket();
};


