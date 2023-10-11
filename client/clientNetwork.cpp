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


void Client::recivePacket(sf::TcpSocket *socket)
{
    while (true)
     {
          if (socket->receive(lastPacket) == sf::Socket::Done)
          {
               std::string receivedString;
               std::string senderSddress;
               unsigned short sender_port;
               lastPacket >> receivedString >> senderAddress >> senderPort;
               std::cout << "From (" << senderAddress << ":" << senderPort << "): " << receivedString  << std::endl;
          }

          std::this_thread::sleep_for((std::chrono::milliseconds)250);
     }
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
void Client::Run()
{
    std::thread receptionThread(&Client::recivePacket, this, &clientsSocket);

    while(true)
    {
        if(isConnected)
        {
            std::string message;
            std::getline(std::cin, message);

            if(message.length() == 0)
                continue;
            sf::Packet messagePacket;
            messagePacket << message;
            sendPacket(messagePacket);
        }
    }
}
