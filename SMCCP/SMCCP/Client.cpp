#include "Client.h"

Client::Client(std::string pName, bool pBlock, int pPort, sf::IpAddress address) :
	textBox(sf::Vector2f(10.0f, cr::winHeight() - 50.0f), sf::Vector2f(350.0f, 40.0f))
{
	port = pPort;
	name = pName;
	ip = address;
	block = pBlock;

	initGraphics();
}

Client::~Client()
{

}

void Client::SendString(sf::String msg)
{
	msg = name + ": " + msg;
	sendData.clear();
	sendData << msg;
	socket.send(sendData);
}

void Client::setup()
{
	own_log::AppendToLogWOTime("\nClient session\n-------------------------------------------------------------");
	own_log::AppendToLog("Trying to connect to " + ip.toString() + " as " + name);
	if (socket.connect(ip, port, sf::seconds(2.f)) != sf::Socket::Done)
	{
		std::cout << "Could not connect" << std::endl;
		own_log::AppendToLog("Could not connect");
		own_log::AppendToLogWOTime("-------------------------------------------------------------\n");
		return;
	}
	sf::Packet namePacket;
	namePacket << name;
	socket.send(namePacket);
	std::cout << "Connected" << std::endl;
	DisplayMessage("[Connected to: " + ip.toString() + "]");
	own_log::AppendToLog("Connected to: " + ip.toString());
	own_log::AppendToLogWOTime("\n--------------------------------\n|	Connected as " + name + "\n--------------------------------\n");
	socket.setBlocking(block);
}

void Client::Update()
{
	newMsg = false;
	lastMsg = "";
	receiveData.clear();

	if (socket.receive(receiveData) == sf::Socket::Done)
	{
		if (!(receiveData >> lastMsg))
		{
			std::cout << "Error in receiving" << std::endl;
			own_log::AppendToLog("Error in receiving");
			return;
		}
		if (lastMsg != "" && lastMsg != SHUTDOWN_MSG)
		{
			if (lastMsg.substring(0, 1) != NO_SOUND_CHAR)
				snd::playSound("incoming_01");
			else
				lastMsg.erase(0);
			DisplayMessage(lastMsg);
		}
		else if (lastMsg == SHUTDOWN_MSG)
		{
			OnServerDisconnect();
		}
	}
	else if (socket.receive(receiveData) == sf::Socket::Disconnected)
	{
		OnServerDisconnect();
	}
	Draw();
}

void Client::Run()
{
	while (cr::currWin().isOpen())
	{
		sf::Event evnt;
		while (cr::currWin().pollEvent(evnt))
		{
			switch (evnt.type)
			{
			case sf::Event::Closed:
				own_log::AppendToLog("Disconnect from server due to closing the window");
				own_log::AppendToLogWOTime("-------------------------------------------------------------\n");
				socket.disconnect();
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
	socket.disconnect();
}

void Client::Draw()
{
	cr::currWin().clear(sf::Color(100, 100, 100));

	cr::currWin().draw(nameText);
	cr::currWin().draw(msgText);
	textBox.display();

	cr::currWin().display();
}

void Client::Enter()
{
	if (textBox.Text() != "" && textBox.Text() != textBox.getStdText())
	{
		this->SendString(textBox.Text());

		sf::String tmpStr = textBox.Text();
		tmpStr = "You: " + tmpStr;

		DisplayMessage(tmpStr);
	}
	textBox.SetNormal();
	snd::playSound("send_01");
}

void Client::initGraphics()
{
	nameText.setString("Name: " + name + "\nRole: Client\nPort: " + std::to_string(port) + 
		"\nVersion: " + VERSION + "\nConnected to " + ip.toString());
	nameText.setFont(cr::currFont());
	nameText.setCharacterSize(14U);

	msgText.setString("Messages:\n");
	msgText.setFont(cr::currFont());
	msgText.setCharacterSize(25U);
	msgText.setPosition(0.0f, 75.0f);

	Draw();
}

void Client::OnServerDisconnect()
{
	own_log::AppendToLog("Disconnected from " + ip.toString() + " due to server");
	own_log::AppendToLogWOTime("-------------------------------------------------------------");
	socket.disconnect();
	snd::playSound("error_01");
	Sleep(1500);
	cr::currWin().close();
}

void Client::DisplayMessage(std::string message)
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
