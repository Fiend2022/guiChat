#include <SFML/Network.hpp>
#include <iostream>
#include "clientNetwork.hpp"

int main()
{
    Client client;
    client.connected("localhost", 1200);
    client.Run();
    return 0;
}
