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
	listener.setBlocking(block);
	if (listener.listen(port) != sf::Socket::Status::Done)
	{
		std::cout << "Error - Could not set up server!" << std::endl;
		return 1;
	}
	std::cout << "Connected... Port: " << port << std::endl;
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
			//std::cout << "Error Could not connect to Client" << std::endl;
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
		msgs.push_back(lastMsg);
		if (msgs.size() > maxMsgs)
			msgs.erase(msgs.begin());

		sf::String complStr = "Messages:\n";
		for (const sf::String msg : msgs)
		{
			complStr += msg + "\n";
		}
		msgText.setString(complStr);
	}
	else
	{
		sf::TcpSocket errorSocket;

		if (listener.accept(errorSocket) != sf::Socket::Status::Done)
			return;

		std::cout << "New socket detected, even though the server is full!" << std::endl;
		SendString("Server is full!", errorSocket);
		errorSocket.disconnect();

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
	//std::cout << (std::string)msg << std::endl;
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
	//std::cout << (std::string)msg << std::endl;
	sendData.clear();
	sendData << msg;
	socket.send(sendData);
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
						msgs.push_back(lastMsg);
						if (msgs.size() > maxMsgs)
							msgs.erase(msgs.begin());

						sf::String complStr = "Messages:\n";
						for (const sf::String msg : msgs)
						{
							complStr += msg + "\n";
						}
						msgText.setString(complStr);

						std::cout << lastMsg.toAnsiString() << std::endl;
					}
				}
				else
				{
					lastMsg = names.at(i) + " disconnected";
					std::cout << (std::string)lastMsg << std::endl;

					//disconnect and delete socket
					selector.remove((*sockets.at(i).get()));
					(*sockets.at(i).get()).disconnect();
					sockets.erase(sockets.begin() + i);
					names.erase(names.begin() + i);

					//send message to all other sockets
					SendString(lastMsg, i);
					msgs.push_back(lastMsg);
					if (msgs.size() > maxMsgs)
						msgs.erase(msgs.begin());

					sf::String complStr = "Messages:\n";
					for (const sf::String msg : msgs)
					{
						complStr += msg + "\n";
					}
					msgText.setString(complStr);
					socketsConnected--;
				}
			}
		
		}
	}
	connectToClient();
	Draw();
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

void Server::Enter()
{
	if (textBox.Text() != "" && textBox.Text() != textBox.getStdText())
	{
		this->SendString(textBox.Text());

		sf::String tmpStr = textBox.Text();
		tmpStr = "You: " + tmpStr;

		msgs.push_back(tmpStr);
		if (msgs.size() > maxMsgs)
			msgs.erase(msgs.begin());

		sf::String complStr = "Messages:\n";
		for (const sf::String msg : msgs)
		{
			complStr += msg + "\n";
		}
		msgText.setString(complStr);
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
