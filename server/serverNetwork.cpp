#include "serverNetwork.hpp"
#include <iostream>
#include <thread>

Server::Server(unsigned short  p)
{
    std::cout << "Chat Server start" << std::endl;
    port = p;
    if(listener.listen(port) != sf::Socket::Done)
        std::cout << "Could not listen" << std::endl;
}


void Server::connectedClient(std::vector<sf::TcpSocket*>* sockets)
{
    while(true)
    {
        sf::TcpSocket* newClient = new sf::TcpSocket();
        if(listener.accept(*newClient) ==  sf::Socket::Done)
        {
            newClient->setBlocking(false);
            sockets->push_back(newClient);
            std::cout << newClient->getRemoteAddress() << ":" << newClient->getRemotePort() << " on slot " << clientsSockets.size() - 1 << std::endl;

        }
        else
        {
            std::cout << "Server listener error" << std::endl;
            delete(newClient);
            break;
        }
    }
}

void Server::disconectClient(sf::TcpSocket* socketPtr, size_t iter)
{
     std::cout << "Client " << socketPtr->getRemoteAddress() << ":" << socketPtr->getRemotePort() << " disconnected, removing" << std::endl;
     socketPtr->disconnect();
     delete(socketPtr);
     clientsSockets.erase(clientsSockets.begin() + iter);
}


void Server::recivePacket(sf::TcpSocket* socket, size_t iter)
{
    sf::Packet packet;
    if(socket->receive(packet) == sf::Socket::Disconnected)
    {
        disconectClient(socket, iter);
        return;
    }
    if (packet.getDataSize() > 0)
     {
          std::string message;
          packet >> message;
          packet.clear();

          packet << message << socket->getRemoteAddress().toString() << socket->getRemotePort();

          broadcastPacket(packet, socket->getRemoteAddress(), socket->getRemotePort());
          std::cout << socket->getRemoteAddress().toString() << ":" << socket->getRemotePort() << " '" << message << "'" << std::endl;
    }
}


void Server::broadcastPacket(sf::Packet& packet, sf::IpAddress addres, unsigned short port)
{
    for(size_t i = 0; i < clientsSockets.size(); i++)
    {
        sf::TcpSocket* client = clientsSockets[i];
         if (client->getRemoteAddress() != addres || client->getRemotePort() != port)
               if (client->send(packet) != sf::Socket::Done)
                    std::cout << "Could not send packet on broadcast" << std::endl;
    }
}

void Server::managePackets()
{
    while(true)
    {
        for(size_t it = 0; it < clientsSockets.size(); it++)
                recivePacket(&clientsSockets[it], it);
    }
}

void Server::Run()
{
    std::thread connectionThread(&Server::connectedClient, this, &clientsSockets);
    managePackets();

}
