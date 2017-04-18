#define _CRT_SECURE_NO_WARNINGS
#include "Server.h"

Server::Server(std::string pName, bool pBlock, unsigned int pPort, unsigned int pMax_Clients, sf::IpAddress address) :
	textBox(sf::Vector2f(10.0f, cr::winHeight() - 50.0f), sf::Vector2f(350.0f, 40.0f))
{
	port = pPort;
	max_Clients = pMax_Clients;
	ip = address;
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
		+ "; Max Clients: " + std::to_string(max_Clients) 
		+ "; Ip: " + ip.toString();
	return out;
}

void Server::setup()
{
	listener.setBlocking(false);
	if (listener.listen(port) != sf::Socket::Status::Done)
	{
		//TODO: Server-Error Could not set up server
		std::cout << "Error - Could not set up server!" << std::endl;
		return;
	}
	std::cout << "Connected... Port: " << port << std::endl;
}

void Server::pushNewSocket()
{
	sockets.push_back((std::unique_ptr<sf::TcpSocket>)(new sf::TcpSocket()));
	selector.add((*sockets.back().get()));
}

void Server::connectToClient()
{
	pushNewSocket();

	if (listener.accept(*(sockets.back().get())) != sf::Socket::Status::Done)
	{
		//std::cout << "Error Could not connect to Client" << std::endl;
		sockets.pop_back();
		return;
	}
	socketsConnected++;
	(*sockets.back().get()).setBlocking(false);
	std::cout << "Client connected" << std::endl;
	selector.add((*sockets.back().get()));
}

void Server::SendString(sf::String msg)
{
	//std::cout << (std::string)msg << std::endl;
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

void Server::Update()
{
	newMsg = false;
	lastMsg = "";
	receiveData.clear();

	if (selector.wait(sf::milliseconds(10)))
	{
		std::cout << "hi" << std::endl;
		for (int i = 0; i < sockets.size(); i++)
		{
			if (selector.isReady((*sockets.at(i).get())))
			{
				(*sockets.at(i).get()).receive(receiveData);
				
				receiveData >> lastMsg;
				if (lastMsg != "")
				{
					lastMsg = "Client " + std::to_string(i) + ": " + lastMsg;
					SendString(ExcludeChar + lastMsg, i);
					msgs.push_back(lastMsg);
					if (msgs.size() > maxMsgs)
						msgs.erase(msgs.begin());

					newMsg = true;

					sf::String complStr = "Messages:\n";
					for (const sf::String msg : msgs)
					{
						complStr += msg + "\n";
					}
					msgText.setString(complStr);

					std::cout << lastMsg.toAnsiString() << std::endl;
				}
				
			}
		
		}
	}
	connectToClient();
	Draw();
}

void Server::initGraphics()
{
	nameText.setString("Server\nPort: " + std::to_string(port) + "\nVersion: " + VERSION);
	nameText.setFont(cr::currFont());
	nameText.setCharacterSize(15U);

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
