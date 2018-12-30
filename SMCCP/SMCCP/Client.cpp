#include "Client.h"

#pragma region Con-/Destructor
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

	running = true;
}

Client::~Client()
{

}
#pragma endregion

#pragma region Networking
void Client::onServerDisconnect()
{
	own_log::append("Disconnected from " + ip.toString() + " due to server");
	own_log::append("-------------------------------------------------------------", false);
	socket.disconnect();
	if (!muted)
		snd::playSound("error_01");
	Sleep(1500);
	cr::currWin().close();
}

void Client::SendString(sf::String msg)
{
	msg = name + ": " + msg;
	sendData.clear();
	sendData << msg;
	socket.send(sendData);
}

int Client::Setup()
{
	key = RSA::GenerateKey(key_bitcount);
	int err_count = 0;
	while (key.err && err_count < 5)
	{
		err_count++;
		key = RSA::GenerateKey(key_bitcount);
	}
	if (err_count >= 5)
	{
		debug::log("Error - Could not Generate key!");
		own_log::append("Error - Could not generate key!");
		own_log::append("-------------------------------------------------------------\n", false);
		return 2;
	}

	NamePrompt np;
	if (np.run() == 1)
		return 2;
	name = np.getName();
	input::setFocus(&cr::currWin());

	//Update nameText
	nameText.setString("Name: " + name + "\nRole: Client\nPort: " + std::to_string(port) +
		"\nVersion: " + VERSION + "\nConnected to " + ip.toString());
	draw();

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
		Setup();
		return 3;
	}

	debug::log("Connected");
	DisplayMessage("[Connected to: " + ip.toString() + "]");

	own_log::append("Connected to: " + ip.toString());
	own_log::append("\n--------------------------------\n|	Connected as " + name + "\n--------------------------------\n", false);
	socket.setBlocking(block);
	return 0;
}
#pragma endregion

#pragma region Graphics
void Client::draw()
{
	cr::currWin().clear(sf::Color(100, 100, 100));

	cr::currWin().draw(nameText);
	cr::currWin().draw(msgText);
	textBox.display();
	muteBox.display();
	sendButton.display();

	cr::currWin().display();
}

void Client::onEnter()
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

	draw();
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
#pragma endregion

#pragma region General
void Client::update()
{
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
			onServerDisconnect();
		}
	}
	else if (socket.receive(receiveData) == sf::Socket::Disconnected)
	{
		onServerDisconnect();
	}
	draw();
}

void Client::Run()
{
	textBox.Select();
	while (running && cr::currWin().isOpen())
	{
		input::handleInput();
		textBox.Update((char)0);
		update();
	}
	socket.disconnect();
	cleanCallbacks();
}
#pragma endregion

#pragma region Callbacks
void Client::initCallbacks()
{
	input::addLeftMouseCallback(lMCb, callback_id);
	input::addCloseCallback(cCb, callback_id);
	input::addTextEnteredCallback(tECb, callback_id);
}

void Client::cleanCallbacks()
{
	input::deleteCloseCallback(callback_id);
	input::deleteLMouseCallback(callback_id);
	input::deleteTextEnteredCallback(callback_id);
}

void Client::LeftMCallback(int x, int y)
{
	if (muteBox.CheckClick())
	{
		debug::pause();
		muted = muteBox.isChecked();
	}
	textBox.SelectOrUnselect();
	if (sendButton.validClick(true))
	{
		onEnter();
		textBox.Select();
	}
}

void Client::CloseCallback()
{
	own_log::append("Disconnect from server due to closing the window");
	own_log::append("-------------------------------------------------------------\n", false);
	socket.disconnect();
	running = false;
}

void Client::TextEnteredCallback(sf::Event::TextEvent text)
{
	//std::cout << text.unicode << std::endl;
	if (text.unicode != 13)
	{
		textBox.Update(text.unicode);
	}
	else
	{
		textBox.Unselect();
		onEnter();
		textBox.Select();
	}
}
#pragma endregion
