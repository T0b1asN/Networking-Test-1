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
	listener.setBlocking(block);
	if (listener.listen(port) != sf::Socket::Status::Done)
	{
		//TODO: Server-Error Could not set up server
	}
	std::cout << "Connected... Port: " << port << std::endl;
}

void Server::connectToClient()
{
	if (listener.accept(other) != sf::Socket::Status::Done)
	{
		//TODO: Server-Error Could not connect to Client
		return;
	}

	other.setBlocking(block);
	std::cout << "Client connected" << std::endl;
}

void Server::SendString(sf::String msg)
{
	std::cout << (std::string)msg << std::endl;
	sendData.clear();
	sendData << msg;
	other.send(sendData);
}

void Server::Update()
{
	newMsg = false;
	lastMsg = "";
	receiveData.clear();

	if (other.receive(receiveData) == sf::Socket::Done)
	{
		std::cout << "Receiving" << std::endl;
		receiveData >> lastMsg;
		if (lastMsg != "")
		{
			lastMsg = "Client: " + lastMsg;
			newMsg = true;
			std::cout << lastMsg.toAnsiString() << std::endl;
		}
	}
	connectToClient();
	Draw();
}

void Server::initGraphics()
{
	nameText.setString("Server\nPort: " + std::to_string(port));
	nameText.setFont(cr::currFont());
	nameText.setCharacterSize(15U);

	Draw();
}

void Server::Draw()
{
	cr::currWin().clear(sf::Color(100, 100, 100));

	cr::currWin().draw(nameText);
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
				if (evnt.text.unicode < 128 && evnt.text.unicode != 13)
				{
					textBox.Update(evnt.text.unicode);
				}
				else if (evnt.text.unicode == 13)
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
		textBox.Update(0);
		Update();
	}
}

void Server::Enter()
{
	if (textBox.Text() != "")
	{
		this->SendString(textBox.Text());
	}
	textBox.SetNormal();
}
