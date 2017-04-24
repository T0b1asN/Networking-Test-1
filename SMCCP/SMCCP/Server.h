#pragma once
#include "SFML.h"
#include "Defines.h"
#include "curr.h"

#include "TextBox.h"

#include "AudioUtil.h"

#include <iostream>
#include <string>
#include <vector>

#include <map>

#include <Windows.h>

#include <memory>

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
	//all names
	std::vector<std::string> names;
public:
	//////////////////////////////////////////////
	///pName: name of the server
	///pBlock: should the components block
	///pPort: the port of the server (standard 53000)
	///pMax_Clients: max clients of the server (standard 10)
	///adress: IpAdress of the server (WARNING: standard not tested)
	//////////////////////////////////////////////
	Server(std::string pName, bool pBlock, unsigned int pPort = 53000, unsigned int pMax_Clients = 10);
	~Server();

	//returns the information of the server in one string
	std::string getInfo();
	//returns the name of the server
	//DISCLAIMER: no real use yet
	std::string getName() { return name; }

	//Call this before using the server, it sets up the listener
	int setup();
	//looks for new clients that try to connect
	//if a new client wants to connect it pushes a new socket in the sockets array
	//this socket is the last socket in the vector
	void connectToClient();

	//Send a string to all connected sockets
	void SendString(sf::String msg);
	//Send a string to all connected sockets except the
	//socket at the index 'exclude'
	//without name
	void SendString(sf::String msg, int exclude);
	//Send a string to the given socket
	//without name
	void SendString(sf::String msg, sf::TcpSocket& socket);
	//Send a string without your name
	void SendStringWithoutName(sf::String msg);

	//Shuts down the server
	//The optMsg is displayed after the standardMsg if replaceStd is false
	//it can be used to display errors
	//WARNING: doesn't close window
	void Shutdown(std::string optMsg = "", bool replaceStd = false);

	//Disconnect the socket at the given index, you can also give a reason
	void disconnectSocket(int index, std::string reason = "");
	//Disconnect the given socket, you can also give a reason
	void disconnectSocket(sf::TcpSocket& socket, std::string reason = "");

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
	//Displays message in messages feed (no name, etc.)
	void DisplayMessage(std::string message);
	
	//General
public:
	//one cycle of the server
	void Update();
	//Loop that keeps the server running
	void Run();

	//prints all names
	void printNames();
};

