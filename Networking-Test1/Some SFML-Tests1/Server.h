#pragma once
#include "SFML.h"
#include "Defines.h"
#include "curr.h"

#include "TextBox.h"

#include <iostream>
#include <string>
#include <vector>

#include <Windows.h>

#include <memory>
//TODO: write a proper function for disconnecting a socket
//TODO: implement names
//TODO: Helper function, that displays a message
class Server
{
	//Networking stuff
private:
	//the listener of the server
	sf::TcpListener listener;

	//the port on which the listener listens
	unsigned int port;
	//the max number of clients allowed
	unsigned int max_Clients;

	//just relevant for the info of the server
	sf::IpAddress ip;
	//the name of the server
	std::string name;

	//the socket selector
	sf::SocketSelector selector;
	//the vector of sockets
	//I didn't find another way of storing them :/
	std::vector<std::unique_ptr<sf::TcpSocket>> sockets;

	//a packet, where you can store the data to send
	sf::Packet sendData;
	//a packet, where the received data is stored
	sf::Packet receiveData;

	//the newest message that is received
	//it has some stuff in it that you should exclude from the message
	//like the ExcludeChar
	sf::String lastMsg;

	//maximum amount of messages shown and saved
	unsigned int maxMsgs = 15;
	//vector for the messages that are received
	//if there are to many (over the limit set by maxMsgs)
	//the old ones will be deleted
	std::vector<sf::String> msgs;
	
	//determines if the components should block or not
	bool block;

	//pushes new socket to the socket vector
	void pushNewSocket();

	//current number of sockets connected
	int socketsConnected = 0;
public:
	//////////////////////////////////////////////
	///pName: name of the server
	///pBlock: should the components block
	///pPort: the port of the server (standard 53000)
	///pMax_Clients: max clients of the server (standard 10)
	///adress: IpAdress of the server (WARNING: standard not tested)
	//////////////////////////////////////////////
	Server(std::string pName, bool pBlock, unsigned int pPort = 53000, unsigned int pMax_Clients = 10, sf::IpAddress adress = sf::IpAddress::getPublicAddress());
	~Server();

	//returns the information of the server in one string
	std::string getInfo();
	//returns the name of the server
	//DISCLAIMER: no real use yet
	std::string getName() { return name; }

	//Call this before using the server, it sets up the listener
	//TODO: let it return an error code
	void setup();
	//looks for new clients that try to connect
	//if a new client wants to connect it pushes a new socket in the sockets array
	//this socket is the last socket in the vector
	void connectToClient();

	//Send a string to all connected sockets
	void SendString(sf::String msg);
	//Send a string to all connected sockets except the
	//socket at the index 'exclude'
	void SendString(sf::String msg, int exclude);

	//Graphics Stuff
private:
	//Text, where the server information is displayed
	sf::Text nameText;
	//Textbox, where the server enters his text
	TextBox textBox;
	//Text, where the messages are displayed
	sf::Text msgText;

	//Draw the server interface
	void Draw();

	//Function, which is called, when the enter key is pressed
	void Enter();

	//initializes the info and message text
	void initGraphics();
public:
	
	//General
public:
	//one cycle of the server
	void Update();
	//Loop that keeps the server running
	void Run();

};

