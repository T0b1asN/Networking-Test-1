#pragma once
#include "SFML.h"
#include "Defines.h"
#include "curr.h"

#include "BaseUIWindow.h"

#include "InputHandler.h"

#include <iostream>
#include <string>
#include <vector>

#include <map>

#include <Windows.h>

#include <memory>

#include "RSA.h"
#include "Protocol.h"

class Server
{
public:
#pragma region Con-/Destructors
	//////////////////////////////////////////////
	///pName: name of the server
	///pBlock: should the components block the thread when waiting
	///pPort: the port of the server (standard 53000)
	///pMax_Clients: max clients of the server (standard 10)
	//////////////////////////////////////////////
	Server(std::string pName, bool pBlock, unsigned int pPort = 53000, unsigned int pMax_Clients = 10);
	~Server();
#pragma endregion

private:
#pragma region Networking
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
	std::string lastMsg;

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

	//Is the sound muted
	bool muted;
#pragma endregion

public:
#pragma region Networking
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
	__declspec(deprecated) void SendString(sf::String msg);
	//Send a string to all connected sockets except the
	//socket at the index 'exclude'
	//without name
	__declspec(deprecated) void SendString(sf::String msg, int exclude);
	//Send a string to the given socket
	//without name
	__declspec(deprecated) void SendString(sf::String msg, sf::TcpSocket& socket);
	//Send a string without your name
	__declspec(deprecated) void SendStringWithoutName(sf::String msg);

	//TODO check on sf::String
	//Send string to all sockets
	void Send(std::string msg, bool tagIncluded = false, bool encrypt = true);
	//send a string to all sockets except the ssocket at index [exclude]
	void Send(std::string msg, int exclude, bool tagIncluded = false, bool encrypt = true);
	//send a string to a specific socket
	void SendSingle(std::string msg, int socketIndex, bool tagIncluded = false, bool encrypt = true);

	//Shuts down the server
	//The optMsg is displayed after the standardMsg if replaceStd is false
	//it can be used to display errors
	//WARNING: doesn't close window
	void Shutdown(sf::String optMsg = "", bool replaceStd = false);

	//Disconnect the socket at the given index, you can also give a reason
	void disconnectSocket(int index, std::string reason = "");
#pragma endregion

private:
#pragma region Graphics
	//Text, where the server information is displayed
	sf::Text nameText;
	//Textbox, where the server enters his text
	TextBox textBox;
	//Text, where the messages are displayed
	sf::Text msgText;
	//CheckBox to control, if the sound is muted
	CheckBox muteBox;
	//Button that funtions as the enter key
	OwnButton sendButton;

	//Draw the server interface
	void Draw();

	//Function, which is called, when the enter key is pressed
	void Enter();

	//initializes the info and message text
	void initGraphics();

#pragma endregion

public:
#pragma region Graphics
	//Displays message in messages feed (no name, etc.)
	void DisplayMessage(std::string message);
#pragma endregion

private:
#pragma region General
	//one cycle of the server
	void Update();

	//prints all names
	void printNames();

	bool run = true;
#pragma endregion

public:
#pragma region General
	//Loop that keeps the server running
	void Run();
#pragma endregion

private:
#pragma region RSA
	//own RSA-key !!! only share public part!!!
	RSA::Key key;
	//TODO solve differently
	// when adding a new socket, also get a new key
	std::vector<RSA::PublicKey> socketKeys;

	//tries generating a key (filling variable key)
	//returns true if succesful
	bool GenerateKey(int max_errors = 5);
#pragma endregion

public:
#pragma region RSA

#pragma endregion

private:
#pragma region Callbacks
	void initCallbacks();

	void cleanCallbacks();
#pragma endregion

public:
#pragma region Callbacks
	// Callback stuff
	const std::string callback_id = "server";

	void leftMCallback(int x, int y);
	// handle for leftMouseCallback
	input::mouseCallback lMCb =
		std::bind(
			&Server::leftMCallback, this,
			std::placeholders::_1,
			std::placeholders::_2
		);

	void closeCallback();
	// handle for closeCallback
	input::closeCallback cCb =
		std::bind(&Server::closeCallback, this);

	void textEnteredCallback(sf::Event::TextEvent text);
	// handle for textEnteredCallback
	input::textEnteredCallback tECb =
		std::bind(
			&Server::textEnteredCallback, this,
			std::placeholders::_1
		);

#pragma endregion

private:
#pragma region RSA
	RSA::Key ownKey;
#pragma endregion

};

