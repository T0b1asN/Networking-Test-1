#include "Client.h"

Client::Client(bool pBlock, int pPort, sf::IpAddress address) :
	textBox(sf::Vector2f(10.0f, cr::winHeight() - 50.0f), sf::Vector2f(350.0f, 40.0f)),
	muteBox(sf::Vector2f(480.f, cr::winHeight() - 30.f), 40.f),
	sendButton("Send", sf::Vector2f(80.f, 40.f), sf::Vector2f(370.f, cr::winHeight() - 50.f))
{
	port = pPort;
	name = "";
	ip = address;
	block = pBlock;

	sendButton.setCharSize(25U);

	muteBox.setInfo("mute sounds");
	muted = muteBox.isChecked();

	initGraphics();
	initCallbacks();

	run = true;
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

int Client::setup()
{
	NamePrompt np;
	if(np.run() == 1)
		return 2;
	name = np.getName();
	input::setFocus(&cr::currWin());
	
	//Update nameText
	nameText.setString("Name: " + name + "\nRole: Client\nPort: " + std::to_string(port) +
		"\nVersion: " + VERSION + "\nConnected to " + ip.toString());
	Draw();

	own_log::append("\nClient session\n-------------------------------------------------------------", false);
	own_log::append("Trying to connect to " + ip.toString() + " as " + name);

	if (socket.connect(ip, port, sf::seconds(2.f)) != sf::Socket::Done)
	{
		debug::log("Could not connect");
		own_log::append("Could not connect");
		own_log::append("-------------------------------------------------------------\n", false);
		return 1;
	}

	sf::Packet namePacket;
	namePacket << name;
	socket.send(namePacket);

	sf::Packet respPacket;
	socket.receive(respPacket);
	std::string resp;
	respPacket >> resp;
	
	if (resp == "1")
	{
		// Name already exists on Server, disconnect and ask for another name
		socket.disconnect();
		setup();
		return 3;
	}

	debug::log("Connected");
	DisplayMessage("[Connected to: " + ip.toString() + "]");

	own_log::append("Connected to: " + ip.toString());
	own_log::append("\n--------------------------------\n|	Connected as " + name + "\n--------------------------------\n", false);
	socket.setBlocking(block);
	return 0;
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
			debug::log("Error in receiving");
			own_log::append("Error in receiving");
			return;
		}
		if (lastMsg != "" && lastMsg != SHUTDOWN_MSG)
		{
			if (lastMsg.substring(0, NO_SOUND_MSG_LENGTH) != NO_SOUND_MSG)
			{
				if (!muted)
					snd::playSound("incoming_01");
			}
			else
			{
				lastMsg.erase(0, NO_SOUND_MSG_LENGTH);
			}
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
	textBox.Select();
	while (cr::currWin().isOpen() && run)
	{
		input::handleInput();
		textBox.Update((char)0);
		Update();
	}
	socket.disconnect();
}

void Client::initCallbacks()
{
	input::addLeftMouseCallback(lMCb, callback_id);
	input::addCloseCallback(cCb, callback_id);
	input::addTextEnteredCallback(tECb, callback_id);
}

void Client::leftMCallback(int x, int y)
{
	if (muteBox.CheckClick())
	{
		debug::pause();
		muted = muteBox.isChecked();
	}
	textBox.SelectOrUnselect();
	if (sendButton.validClick(true))
	{
		Enter();
		textBox.Select();
	}
}

void Client::closeCallback()
{
	own_log::append("Disconnect from server due to closing the window");
	own_log::append("-------------------------------------------------------------\n", false);
	socket.disconnect();
	run = false;
	debug::pause();
}

void Client::textEnteredCallback(sf::Event::TextEvent text)
{
	//std::cout << text.unicode << std::endl;
	if (text.unicode != 13)
	{
		textBox.Update(text.unicode);
	}
	else
	{
		textBox.Unselect();
		Enter();
		textBox.Select();
	}
}

void Client::Draw()
{
	cr::currWin().clear(sf::Color(100, 100, 100));
	
	cr::currWin().draw(nameText);
	cr::currWin().draw(msgText);
	textBox.display();
	muteBox.display();
	sendButton.display();

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
		if (!muted)
			snd::playSound("send_01");
	}
	textBox.SetNormal();
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
	own_log::append("Disconnected from " + ip.toString() + " due to server");
	own_log::append("-------------------------------------------------------------", false);
	socket.disconnect();
	if (!muted)
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
