#define _CRT_SECURE_NO_WARNINGS
#include "Server.h"

Server::Server(std::string pName, bool pBlock, unsigned int pPort, unsigned int pMax_Clients) :
	textBox(sf::Vector2f(10.0f, cr::winHeight() - 50.0f), sf::Vector2f(350.0f, 40.0f))
{
	port = pPort;
	max_Clients = pMax_Clients;
	name = pName;
	block = pBlock;

	initGraphics();
}

Server::~Server()
{

}

std::string Server::getInfo()
{
	std::string out;
	out += "Name: " + name
		+ "; Port: " + std::to_string(port)
		+ "; Max Clients: " + std::to_string(max_Clients);
	return out;
}

int Server::setup()
{
	own_log::AppendToLogWOTime("\nServer session\n-------------------------------------------------------------");
	own_log::AppendToLog("Setting up server on port " + std::to_string(port) + " with name " + name);
	listener.setBlocking(block);
	if (listener.listen(port) != sf::Socket::Status::Done)
	{
		std::cout << "Error - Could not set up server!" << std::endl;
		own_log::AppendToLogWOTime("-------------------------------------------------------------\n");
		return 1;
	}
	std::cout << "Connected... Port: " << port << std::endl;
	own_log::AppendToLog("Server setup finished");
	return 0;
}

void Server::pushNewSocket()
{
	sockets.push_back((std::unique_ptr<sf::TcpSocket>)(new sf::TcpSocket()));
	selector.add((*sockets.back().get()));
}

void Server::connectToClient()
{
	if (socketsConnected < (int)max_Clients)
	{
		pushNewSocket();

		if (listener.accept(*(sockets.back().get())) != sf::Socket::Status::Done)
		{
			//most of the time, iff no client tries to connect
			sockets.pop_back();
			return;
		}
		//receive name from client
		sf::Packet namePacket;
		namePacket.clear();

		(sockets.back().get())->receive(namePacket);

		std::string newSocketName;
		namePacket >> newSocketName;

		socketsConnected++;

		names.push_back(newSocketName);

		(*sockets.back().get()).setBlocking(block);
		selector.add((*sockets.back().get()));

		lastMsg = "[" + newSocketName + " connected]";
		std::cout << (std::string)lastMsg << std::endl;

		//send message to all other sockets
		SendString(lastMsg, socketsConnected - 1);
		DisplayMessage(lastMsg);

		own_log::AppendToLog("New client named " + newSocketName + " | Now there are " + std::to_string(socketsConnected)  + " sockets connected");
	}
	else
	{
		sf::TcpSocket errorSocket;

		if (listener.accept(errorSocket) != sf::Socket::Status::Done)
			return;

		disconnectSocket(errorSocket, "Server is full!");

		own_log::AppendToLog("Client tried to connect, even though the server is full");

		return;
	}
}

void Server::SendString(sf::String msg)
{
	//std::cout << (std::string)msg << std::endl;
	msg = name + ": " + msg;
	sendData.clear();
	sendData << msg;
	for (int i = 0; i < (int)sockets.size(); i++)
	{
		(*sockets.at(i).get()).send(sendData);
	}
}

void Server::SendString(sf::String msg, int exclude)
{
	sendData.clear();
	sendData << msg;
	for (int i = 0; i < (int)sockets.size(); i++)
	{
		if(i != exclude)
			(*sockets.at(i).get()).send(sendData);;
	}
}

void Server::SendString(sf::String msg, sf::TcpSocket& socket)
{
	sendData.clear();
	sendData << msg;
	socket.send(sendData);
}

void Server::SendStringWithoutName(sf::String msg)
{
	sendData.clear();
	sendData << msg;
	for (int i = 0; i < (int)sockets.size(); i++)
	{
		(*sockets.at(i).get()).send(sendData);
	}
}

void Server::initGraphics()
{
	nameText.setString("Your nickname: " + name + "\nRole: Server\nPort: " + std::to_string(port) + 
		"\nVersion: " + VERSION + "\nIp: " + sf::IpAddress::getLocalAddress().toString());
	nameText.setFont(cr::currFont());
	nameText.setCharacterSize(14U);

	msgText.setString("Messages:\n");
	msgText.setFont(cr::currFont());
	msgText.setCharacterSize(25U);
	msgText.setPosition(0.0f, 75.0f);

	Draw();
}

void Server::Draw()
{
	cr::currWin().clear(sf::Color(100, 100, 100));

	cr::currWin().draw(nameText);
	cr::currWin().draw(msgText);
	textBox.display();

	cr::currWin().display();
}

void Server::Run()
{
	while (cr::currWin().isOpen())
	{
		sf::Event evnt;
		while (cr::currWin().pollEvent(evnt))
		{
			switch (evnt.type)
			{
			case sf::Event::Closed:
				own_log::AppendToLog("You shut down the server, by closing the window");
				own_log::AppendToLogWOTime("-------------------------------------------------------------\n");
				Shutdown("Host closed the window!", true);
				return;
				break;
			case sf::Event::TextEntered:
				if (evnt.text.unicode != 13)
				{
					textBox.Update(evnt.text.unicode);
				}
				else
				{
					textBox.Unselect();
					Enter();
				}
				break;
			case sf::Event::MouseButtonPressed:
				if (evnt.mouseButton.button == sf::Mouse::Left)
				{
					textBox.SelectOrUnselect();
				}
				break;
			}
		}
		textBox.Update((char)0);
		Update();
	}
}

void Server::Update()
{
	lastMsg = "";
	receiveData.clear();

	if (selector.wait(sf::milliseconds(10)) && socketsConnected > 0)
	{
		for (int i = 0; i < (int)sockets.size(); i++)
		{
			if (selector.isReady((*sockets.at(i).get())))
			{
				if ((*sockets.at(i).get()).receive(receiveData) != sf::Socket::Disconnected)
				{
					receiveData >> lastMsg;
					if (lastMsg != "")
					{
						SendString(lastMsg, i);
						DisplayMessage(lastMsg);
					}
				}
				else
				{
					lastMsg = "[" + names.at(i) + " disconnected]";
					std::cout << (std::string)lastMsg << std::endl;

					//disconnect and delete socket
					selector.remove((*sockets.at(i).get()));
					(*sockets.at(i).get()).disconnect();
					sockets.erase(sockets.begin() + i);
					socketsConnected--;
					own_log::AppendToLog(names.at(i) + " (Place " + std::to_string(i) +
						") disconnected | Now there are " + std::to_string(socketsConnected) + " sockets connected");
					names.erase(names.begin() + i);

					//send message to all other sockets
					SendString(lastMsg, i);
					DisplayMessage(lastMsg);
				}
			}
		
		}
	}
	connectToClient();
	Draw();
}

void Server::Enter()
{
	if (textBox.Text() != "" && textBox.Text() != textBox.getStdText())
	{
		this->SendString(textBox.Text());

		sf::String tmpStr = textBox.Text();
		tmpStr = "You: " + tmpStr;

		DisplayMessage(tmpStr);
	}
	textBox.SetNormal();
}

void Server::printNames()
{
	for (int i = 0; i < (int)names.size(); i++)
	{
		std::cout << "Slot " << i << ": " << names.at(i) << std::endl;
	}
}

void Server::Shutdown(std::string optMsg, bool replaceOld)
{
	if (optMsg == "")
		SendStringWithoutName("[Server was shut down]");
	else if(!replaceOld)
		SendStringWithoutName("[Server was shut down. Message: " + optMsg + "]");
	else
		SendStringWithoutName("[" + optMsg + "]");

	SendStringWithoutName(SHUTDOWN_MSG);
	for (int i = 0; i < (int)sockets.size(); i++)
		disconnectSocket(i);
}

void Server::disconnectSocket(int index, std::string reason)
{
	if (reason != "")
		SendString("[Disconnected from server. Reason: " + reason + "]", (*sockets.at(index).get()));
	else
		SendString("[Disconnected from server for an unknown reason]", (*sockets.at(index).get()));
	Sleep(100);

	sockets.at(index).get()->disconnect();
}

void Server::disconnectSocket(sf::TcpSocket& socket, std::string reason)
{
	if (reason != "")
		SendString("[Disconnected from server. Reason: " + reason + "]", socket);
	else
		SendString("[Disconnected from server for an unknown reason]", socket);
	Sleep(100);

	socket.disconnect();
}

void Server::DisplayMessage(std::string message)
{
	msgs.push_back(message);
	if (msgs.size() > maxMsgs)
		msgs.erase(msgs.begin());

	sf::String complStr = "Messages:\n";
	for (const sf::String msg : msgs)
	{
		complStr += msg + "\n";
	}
	msgText.setString(complStr);
}
