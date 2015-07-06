#include "StdAfx.h"
#include "ClientGame.h"


ClientGame::ClientGame(void)
{

    network = new ClientNetwork();

    // send init packet
    const unsigned int packet_size = sizeof(Packet);
    char packet_data[packet_size];

    Packet packet;
    packet.packet_type = INIT_CONNECTION;

    packet.serialize(packet_data);

    NetworkServices::sendMessage(network->ConnectSocket, packet_data, packet_size);
}


ClientGame::~ClientGame(void)
{
}
/*
void ClientGame::sendActionPackets()
{
    // send action packet
    const unsigned int packet_size = sizeof(Packet);
    char packet_data[packet_size] = "moh";

    Packet packet;
    packet.packet_type = ACTION_EVENT;

    // packet.serialize(packet_data);

    NetworkServices::sendMessage(network->ConnectSocket, packet_data, packet_size);
}
*/

void ClientGame::sendPackets(char * packetData)
{
	Packet packet;

	int data_length = sizeof(packetData);

	if ( data_length <= 0 )
	{
		return;
	}

	int sentByteCount = 0;
	if (sentByteCount < data_length) {
		// send action packet
		unsigned int packet_size = sizeof(packetData);// sizeof(Packet);
		//char packet_data[packet_size] = packetData;

		Packet packet;
		packet.packet_type = ACTION_EVENT;

		// packet.serialize(packet_data);

		NetworkServices::sendMessage(network->ConnectSocket, packetData, packet_size);
	}

}
/*
void ClientGame::update()
{
    Packet packet;
    // int data_length = network->receivePackets(network_data);
	int data_length = 4;

    if (data_length <= 0) 
    {
        //no data recieved
        return;
    }

    int i = 0;
    while (i < (unsigned int)data_length) 
    {
        packet.deserialize(&(network_data[i]));
        i += sizeof(Packet);
		sendActionPackets();
        switch (packet.packet_type) {

            case ACTION_EVENT:

                printf("client received action event packet from server\n");

                // sendActionPackets();

                break;

            default:

                printf("error in packet types\n");

                break;
        }
    }
}
*/