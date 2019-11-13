#include "Client.h"

const std::string Client::sendButton_id = "client_sendB";

#pragma region Con-/Destructor
Client::Client(bool pBlock, int pPort, sf::IpAddress address) :
	textBox(sf::Vector2f(10.0f, cr::winHeight() - 50.0f), sf::Vector2f(350.0f, 40.0f)),
	muteBox(sf::Vector2f(480.f, cr::winHeight() - 30.f), 40.f),
	sendButton(sendButton_id, "Send", sf::Vector2f(80.f, 40.f), sf::Vector2f(370.f, cr::winHeight() - 50.f))
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

Client::SetupResult Client::Setup()
{
	debug::log("------------ Client -----------------");
	socket.setBlocking(true);
	//TODO: make that key isn't generated twice for the client, even if you need a new name
	//		should also resend key, but not regenerate
	//key generation
	if (!GenerateKey())
	{
		debug::log("Error - Could not Generate key!");
		own_log::append("Error - Could not generate key!");
		own_log::append("-------------------------------------------------------------\n", false);
		return SetupResult::Error;
	}

	if (socket.connect(ip, port, sf::seconds(2.f)) != sf::Socket::Done)
	{
		debug::log("Could not connect");
		own_log::append("Could not connect");
		own_log::append("-------------------------------------------------------------\n", false);
		return SetupResult::Error;
	}

	//send key to server
	std::string keyStr = str::concat(
		prot::rsa_key, " ", mpir_helper::str(key.pubKey.N), " ", mpir_helper::str(key.pubKey.e)
	);
	this->Send(keyStr, true, false);

	NamePrompt np(true);
	NamePrompt::Result np_res = np.run();
	if (np_res == NamePrompt::Result::Close)
		return SetupResult::Close;
	else if (np_res == NamePrompt::Result::Default)
		return SetupResult::Error;
	else if (np_res == NamePrompt::Result::Unexpected)
		return SetupResult::Error;

	name = np.getName();
	np.close();

	//Update nameText
	nameText.setString("Name: " + name + "\nRole: Client\nPort: " + std::to_string(port) +
		"\nVersion: " + VERSION + "\nConnected to " + ip.toString());
	draw();

	own_log::append("\nClient session\n-------------------------------------------------------------", false);
	own_log::append("Trying to connect to " + ip.toString() + " as " + name);

	//receive key
	sf::Packet sKeyPacket;
	socket.receive(sKeyPacket);
	std::string sKey;
	sKeyPacket >> sKey;

	if (str::split(sKey, ' ').front() != prot::rsa_key)
	{
		debug::log(str::concat("Error: Awaited ", prot::rsa_key,
			"and received different Token -> Exiting..."));
		own_log::append(str::concat("Error: Awaited ", prot::rsa_key,
			"and received different Token -> Exiting...")); //awaited name response
		return SetupResult::Error;
	}
	if (str::split(sKey, ' ').size() < 3)
	{
		debug::log(str::concat("Error: Didn't receive enough arguments from server -> Exiting..."));
		own_log::append(str::concat("Error: Didn't receive enough arguments from server -> Exiting...")); //awaited name response
		return SetupResult::Error;
	}
	mpir_helper::fill(serverKey.N, str::split(sKey, ' ').at(1), RSA::ENC_BASE);
	mpir_helper::fill(serverKey.e, str::split(sKey, ' ').at(2), RSA::ENC_BASE);

	this->Send(str::concat(prot::c::name, " ", name), true);

	sf::Packet respPacket;
	socket.receive(respPacket);
	std::string resp;
	respPacket >> resp;
	resp = RSA::Decrypt(resp, key.privKey);
	debug::log("Resp: " + resp);

	if (str::split(resp, ' ').front() != prot::s::name_resp)
	{
		debug::log(str::concat("Error: Awaited ", prot::s::name_resp,
			"and received different Token -> Exiting..."));
		own_log::append(str::concat("Error: Awaited ", prot::s::name_resp,
			"and received different Token -> Exiting...")); //awaited name response
		return SetupResult::Error;
	}

	if (str::split(resp, ' ').at(1) == "1")
	{
		// Name already exists on Server, disconnect and ask for another name
		socket.disconnect();
		own_log::append("Name exists on server, retrying setup");
		return SetupResult::RetrySetup;
	}

	input::setFocus(&cr::currWin());

	debug::log("Connected");
	DisplayMessage("[Connected to: " + ip.toString() + "]");

	own_log::append("Connected to: " + ip.toString());
	own_log::append("\n--------------------------------\n|	Connected as " + name + "\n--------------------------------\n", false);
	socket.setBlocking(block);
	return SetupResult::Done;
}

void Client::Send(std::string msg, bool tagIncluded, bool encrypt)
{
	//if tag is not included in the message, add standard msg tag
	if (!tagIncluded)
		msg = str::concat(prot::msg, " ", msg);
	if (encrypt)
		msg = RSA::Encrypt(msg, serverKey, prot::rsa::chunkSize);
	sendData.clear();
	sendData << msg;
	socket.send(sendData);
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
		this->Send(textBox.Text());
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
	//TODO: Work here
	if (socket.receive(receiveData) == sf::Socket::Done)
	{
		if (!(receiveData >> lastMsg))
		{
			debug::log("Error in receiving");
			own_log::append("Error in receiving");
			return;
		}
		lastMsg = RSA::Decrypt(lastMsg, key.privKey);
		debug::log("lastMsg: " + lastMsg);
		std::string token = str::split(lastMsg, ' ').front();
		//remove token from message
		lastMsg = prot::remToken(lastMsg, token);
		if (token == prot::msg)
		{
			//normal message
			if (!muted)
				snd::playSound("incoming_01");
			DisplayMessage(lastMsg);
		}
		else if (token == prot::s::msg_nosound)
		{
			//message with no sound
			DisplayMessage(lastMsg);
		}
		else if (token == prot::s::shutdown)
		{
			//server is shutting down
			onServerDisconnect();
		}
		else if (token == prot::error)
		{
			//error
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
		update();
	}

	socket.disconnect();
	cleanCallbacks();

	sendButton.cleanup();
}
#pragma endregion

#pragma region RSA
bool Client::GenerateKey(int max_errors)
{
	key = RSA::GenerateKey(prot::rsa::key_bitcnt);
	int err_count = 0;
	while (key.err && err_count < 5)
	{
		err_count++;
		key = RSA::GenerateKey(prot::rsa::key_bitcnt);
	}
	return !(err_count >= max_errors);
}
#pragma endregion

#pragma region Callbacks
void Client::initCallbacks()
{
	input::addLeftMouseCallback(lMCb, callback_id);
	input::addCloseCallback(cCb, callback_id);
	input::addTextEnteredCallback(tECb, callback_id);

	sendButton.setOnClickCallback(bCallback);
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
}

void Client::CloseCallback()
{
	own_log::append("Disconnect from server due to closing the window");
	own_log::append("-------------------------------------------------------------\n", false);
	debug::log("Client closed window");
	debug::pause();
	socket.disconnect();
	running = false;
}

void Client::TextEnteredCallback(sf::Event::TextEvent text)
{
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

void Client::buttonCallback(std::string id)
{
	onEnter();
	textBox.Select();
}
#pragma endregion
