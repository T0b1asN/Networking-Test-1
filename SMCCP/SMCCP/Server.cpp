#define _CRT_SECURE_NO_WARNINGS
#include "Server.h"

#pragma region Con-/Destructors
Server::Server(std::string pName, bool pBlock, unsigned int pPort, unsigned int pMax_Clients) :
	textBox(sf::Vector2f(10.0f, cr::winHeight() - 50.0f), sf::Vector2f(350.0f, 40.0f)),
	muteBox(sf::Vector2f(480.f, cr::winHeight() - 30.f), 40.f),
	sendButton("Send", sf::Vector2f(80.f, 40.f), sf::Vector2f(370.f, cr::winHeight() - 50.f))
{
	port = pPort;
	max_Clients = pMax_Clients;
	name = pName;
	block = pBlock;

	sendButton.setCharSize(25U);

	muteBox.setInfo("mute sounds");
	muted = muteBox.isChecked();

	initGraphics();
	initCallbacks();

	run = true;
}

Server::~Server()
{

}
#pragma endregion

#pragma region Networking
void Server::pushNewSocket()
{
	sockets.push_back((std::unique_ptr<sf::TcpSocket>)(new sf::TcpSocket()));
	selector.add((*sockets.back().get()));
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
	own_log::append("\nServer session\n-------------------------------------------------------------", false);
	own_log::append("Setting up server on port " + std::to_string(port) + " with name " + name);
	listener.setBlocking(block);
	if (listener.listen(port) != sf::Socket::Status::Done)
	{
		debug::log("Error - Could not set up server!");
		own_log::append("-------------------------------------------------------------\n", false);
		return 1;
	}
	debug::log("Connected... Port: " + std::to_string(port));
	debug::log("Generating Key...");
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
	debug::log("Generated key with bitcount " + std::to_string(key_bitcount));
	own_log::append("Server setup finished");
	return 0;
}

void Server::connectToClient()
{
	if (socketsConnected < (int)max_Clients)
	{
		pushNewSocket();

		if (listener.accept(*(sockets.back().get())) != sf::Socket::Status::Done)
		{
			//most of the time, if no client tries to connect
			sockets.pop_back();
			return;
		}
		// connected Socket is last in vector
		// first send public key
		std::string key_msg = prot::rsa_key;
		key_msg += " " + mpir_helper::str(key.pubKey.N);
		key_msg += " " + mpir_helper::str(key.pubKey.e);
		//send msg to new socket
		//receive public key from socket and save it in new vector
		RSA::PublicKey otherKey = RSA::PublicKey();
		// fill Key
		//fill rec with received message from client
		std::string rec = "";
		std::vector<std::string> args = str::split(rec, ' ');
		if (args.size() < 3)
			; //retry receivingkey
		if (args.front() != "RSA")
			; //retry receiving Key
		mpir_helper::fill(otherKey.N, args.at(1));
		mpir_helper::fill(otherKey.e, args.at(2));

		//from here only send encrypted messages

		// not needed
		int index = sockets.size() - 1;

		socketKeys.push_back(otherKey);
		//receive name from client
		sf::Packet namePacket;
		namePacket.clear();

		sockets.back().get()->receive(namePacket);

		std::string newSocketName;
		namePacket >> newSocketName;

		if (std::find(names.begin(), names.end(), newSocketName) != names.end() || newSocketName == name)
		{
			sf::Packet respPacket;
			respPacket << prot::s::name_resp << " 1";
			(sockets.back().get())->send(respPacket);
			(sockets.back().get())->disconnect();
			sockets.pop_back();
			return;
		}
		else
		{
			sf::Packet respPacket;
			respPacket << prot::s::name_resp << " 0";
			(sockets.back().get())->send(respPacket);
		}

		socketsConnected++;

		names.push_back(newSocketName);

		(*sockets.back().get()).setBlocking(block);
		selector.add((*sockets.back().get()));

		lastMsg = "[" + newSocketName + " connected]";
		debug::log((std::string)lastMsg);

		std::string nsc = NO_SOUND_MSG;
		SendString(nsc + lastMsg, socketsConnected - 1);
		DisplayMessage(lastMsg);

		own_log::append("New client named " + newSocketName + " | Now there are " + std::to_string(socketsConnected) + " sockets connected");
	}
	else
	{
		sf::TcpSocket errorSocket;

		if (listener.accept(errorSocket) != sf::Socket::Status::Done)
			return;

		disconnectSocket(errorSocket, "Server is full!");

		own_log::append("Client tried to connect, even though the server is full");

		return;
	}
}

void Server::SendString(sf::String msg)
{
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
		if (i != exclude)
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

void Server::Shutdown(sf::String optMsg, bool replaceOld)
{
	sf::String nsc = NO_SOUND_MSG;
	std::wcout << nsc.toWideString() << std::endl;
	if (optMsg == "")
		SendStringWithoutName(nsc + L"[Server was shut down]");
	else if (!replaceOld)
		SendStringWithoutName(nsc + L"[Server was shut down. Message: " + optMsg + L"]");
	else
		SendStringWithoutName(nsc + L"[" + optMsg + L"]");

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
#pragma endregion

#pragma region Graphics
void Server::Draw()
{
	cr::currWin().clear(sf::Color(100, 100, 100));

	cr::currWin().draw(nameText);
	cr::currWin().draw(msgText);
	textBox.display();
	muteBox.display();
	sendButton.display();

	cr::currWin().display();
}

void Server::Enter()
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
#pragma endregion

#pragma region General
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
						if (!muted)
							snd::playSound("incoming_01");
						SendString(lastMsg, i);
						DisplayMessage(lastMsg);
					}
				}
				else
				{
					lastMsg = "[" + names.at(i) + " disconnected]";
					debug::log((std::string)lastMsg);

					//disconnect and delete socket
					selector.remove((*sockets.at(i).get()));
					(*sockets.at(i).get()).disconnect();
					sockets.erase(sockets.begin() + i);
					socketsConnected--;
					own_log::append(names.at(i) + " (Place " + std::to_string(i) +
						") disconnected | Now there are " + std::to_string(socketsConnected) + " sockets connected");
					names.erase(names.begin() + i);

					//send message to all other sockets
					SendString(lastMsg, i);
					DisplayMessage(lastMsg);

					//TODO: Disconnect sound
				}
			}

		}
	}
	connectToClient();
	Draw();
}

void Server::Run()
{
	textBox.Select();

	// Generate RSA key

	while (cr::currWin().isOpen() && run)
	{
		input::handleInput();
		textBox.Update((char)0);
		Update();
	}
	cleanCallbacks();
}

void Server::printNames()
{
	for (int i = 0; i < (int)names.size(); i++)
	{
		debug::log("Slot " + std::to_string(i) + ": " + names.at(i));
	}
}
#pragma endregion

#pragma region Callbacks
void Server::initCallbacks()
{
	input::addLeftMouseCallback(lMCb, callback_id);
	input::addCloseCallback(cCb, callback_id);
	input::addTextEnteredCallback(tECb, callback_id);
}

void Server::cleanCallbacks()
{
	input::deleteCloseCallback(callback_id);
	input::deleteLMouseCallback(callback_id);
	input::deleteTextEnteredCallback(callback_id);
}

void Server::leftMCallback(int x, int y)
{
	if (muteBox.CheckClick())
		muted = muteBox.isChecked();
	textBox.SelectOrUnselect();
	if (sendButton.validClick(true))
	{
		Enter();
		textBox.Select();
	}
}

void Server::closeCallback()
{
	own_log::append("You shut down the server, by closing the window");
	own_log::append("-------------------------------------------------------------\n", false);
	Shutdown("Host closed the window!", true);
	run = false;
}

void Server::textEnteredCallback(sf::Event::TextEvent text)
{
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
#pragma endregion
