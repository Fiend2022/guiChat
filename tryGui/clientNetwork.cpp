#include "clientNetwork.hpp"
#include <iostream>

void Client::connected(const std::string& adress, unsigned short port)
{
    if(clientsSocket.connect(adress, port) != sf::Socket::Done)
        std::cout << "Could not connect to the server" << std::endl;
    else
    {
        std::cout << "Connected to the server!" << std::endl;
        isConnected = true;
    }

}


sf::Socket::Status Client::recivePacket()
{
  return clientsSocket.receive(lastPacket);
}

void Client::sendPacket(sf::Packet& packet)
{
    if(packet.getDataSize() > 0 && clientsSocket.send(packet) != sf::Socket::Done)
        std::cout << "Could not send packet" << std::endl;
}

sf::Packet& Client::getPacket()
{
    return lastPacket;
}
//void Client::Run()
//{
//    std::thread receptionThread(&Client::recivePacket, this, &clientsSocket);
//
//    while(true)
//    {
//        if(isConnected)
//        {
//            std::string message;
//            std::getline(std::cin, message);
//
//            if(message.length() == 0)
//                continue;
//            sf::Packet messagePacket;
//            messagePacket << message;
//            sendPacket(messagePacket);
//        }
//    }
//}
