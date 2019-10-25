#define _CRT_SECURE_NO_WARNINGS
#include "Server.h"

const std::string Server::sendButton_id = "server_sendB";

#pragma region Con-/Destructors
Server::Server(std::string pName, bool pBlock, unsigned int pPort, unsigned int pMax_Clients) :
	textBox(sf::Vector2f(10.0f, cr::winHeight() - 50.0f), sf::Vector2f(350.0f, 40.0f)),
	muteBox(sf::Vector2f(480.f, cr::winHeight() - 30.f), 40.f),
	sendButton(sendButton_id, "Send", sf::Vector2f(80.f, 40.f), sf::Vector2f(370.f, cr::winHeight() - 50.f))
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
	if (!GenerateKey())
	{
		debug::log("Error - Could not Generate key!");
		own_log::append("Error - Could not generate key!");
		own_log::append("-------------------------------------------------------------\n", false);
		return 2;
	}
	debug::log("Generated key with bitcount " + std::to_string(prot::rsa::key_bitcnt));

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
		//receive public key from socket and save it in new vector
		RSA::PublicKey otherKey = RSA::PublicKey();
		// fill Key
		//fill rec with received message from client
		sf::Packet rsaPacket;
		sockets.back().get()->receive(rsaPacket);
		//TODO receive Key here
		std::string rec = "";
		rsaPacket >> rec;
		std::vector<std::string> args = str::split(rec, ' ');
		if (args.size() < 3)
			; //retry receivingkey
		if (args.front() != prot::rsa_key)
			; //retry receiving Key
		mpir_helper::fill(otherKey.N, args.at(1), RSA::ENC_BASE);
		mpir_helper::fill(otherKey.e, args.at(2), RSA::ENC_BASE);
		socketKeys.push_back(otherKey);
		debug::log("Client " + std::to_string(sockets.size() - 1) + " key: " + RSA::keyToStr(otherKey));

		// connected Socket is last in vector
		// first send public key
		std::string key_msg = str::concat(prot::rsa_key, " ", mpir_helper::str(key.pubKey.N), " ", mpir_helper::str(key.pubKey.e));
		sf::Packet ownRsaP;
		ownRsaP.clear();
		ownRsaP << key_msg;
		sockets.back().get()->send(ownRsaP);
		
		//send msg to new socket


		//from here only send encrypted messages

		// not needed
		int index = sockets.size() - 1;

		//receive name from client
		sf::Packet namePacket;
		namePacket.clear();

		sockets.back().get()->receive(namePacket);

		std::string newSocketName;
		namePacket >> newSocketName;
		newSocketName = RSA::Decrypt(newSocketName, key.privKey);
		newSocketName = prot::remToken(newSocketName, str::split(newSocketName, ' ').front());
		if (std::find(names.begin(), names.end(), newSocketName) != names.end() || newSocketName == name)
		{
			sf::Packet respPacket;
			std::string resp = str::concat(prot::s::name_resp, " 1");
			resp = RSA::Encrypt(resp, socketKeys.back());
			debug::log("Resp: " + resp);
			respPacket << resp;
			sockets.back().get()->setBlocking(true);
			(sockets.back().get())->send(respPacket);
			(sockets.back().get())->disconnect();
			sockets.pop_back();
			socketKeys.pop_back();
			socketKeys.pop_back();
			return;
		}
		else
		{
			sf::Packet respPacket;
			std::string resp = str::concat(prot::s::name_resp, " 0");
			resp = RSA::Encrypt(resp, socketKeys.back());
			debug::log("Resp: " + resp);
			respPacket << resp;
			(sockets.back().get())->send(respPacket);
		}

		socketsConnected++;

		names.push_back(newSocketName);

		(*sockets.back().get()).setBlocking(block);
		selector.add((*sockets.back().get()));

		lastMsg = "[" + newSocketName + " connected]";
		debug::log((std::string)lastMsg);
		
		Send(str::concat(prot::s::msg_nosound, " ", lastMsg), socketsConnected - 1, true);
		DisplayMessage(lastMsg);

		own_log::append("New client named " + newSocketName + " | Now there are " + std::to_string(socketsConnected) + " sockets connected");
	}
	else
	{
		sf::TcpSocket errorSocket;
		errorSocket.setBlocking(false);

		if (listener.accept(errorSocket) != sf::Socket::Status::Done)
			return;

		//TODO do manually
		std::string msg = "[Disconnected from server.Reason: Server is full!]";
		sf::Packet errorPkt;
		errorPkt.clear();
		errorPkt << msg;
		errorSocket.send(errorPkt);
		Sleep(100);

		errorSocket.disconnect();

		own_log::append("Client tried to connect, even though the server is full");
	}

	receiveData.clear();
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

void Server::Send(std::string msg, bool tagIncluded, bool encrypt)
{
	if(!tagIncluded)
		msg = str::concat(prot::msg, " ", msg);
	for (int i = 0; i < (int)sockets.size(); i++)
	{
		std::string encrypted = msg;
		if (encrypt)
			encrypted = RSA::Encrypt(msg, socketKeys.at(i), prot::rsa::chunkSize);
		sendData.clear();
		sendData << encrypted;
		debug::log("Sending: " + encrypted + " (packet size: " + std::to_string((int)sendData.getDataSize()) + ")");
		sf::Socket::Status stat = (*sockets.at(i).get()).send(sendData);
		debug::log("Send status: " + network::statusToString(stat));
	}
}

void Server::Send(std::string msg, int exclude, bool tagIncluded, bool encrypt)
{
	for (int i = 0; i < (int)sockets.size(); i++)
	{
		if (i != exclude)
			SendSingle(msg, i, tagIncluded, encrypt);
	}
}

void Server::SendSingle(std::string msg, int socketIndex, bool tagIncluded, bool encrypt)
{
	if (!tagIncluded)
		msg = str::concat(prot::msg, " ", msg);
	debug::log("MSG before: " + msg + "| index: " + std::to_string(socketIndex));
	if (encrypt)
		msg = RSA::Encrypt(msg, socketKeys.at(socketIndex), prot::rsa::chunkSize);
	sendData.clear();
	sendData << msg;
	debug::log("Sending: " + msg + " (packet size: " + std::to_string((int)sendData.getDataSize()) + ")");
	(*sockets.at(socketIndex).get()).send(sendData);
}

void Server::Shutdown(sf::String optMsg, bool replaceOld)
{
	if (optMsg == "")
		Send(str::concat(prot::s::msg_nosound, " [Server was shut down]"));
	else if (!replaceOld)
		Send(str::concat(prot::s::msg_nosound, " [Server was shut down. Message: ", optMsg, "]"));
	else
		Send(str::concat(prot::s::msg_nosound, "[", optMsg, "]"));

	Send(prot::s::shutdown);
	for (int i = 0; i < (int)sockets.size(); i++)
		disconnectSocket(i);
}

void Server::disconnectSocket(int index, std::string reason)
{
	if (reason != "")
		SendSingle("[Disconnected from server. Reason: " + reason + "]", index);
	else
		SendSingle("[Disconnected from server for an unknown reason]", index);
	//wait for sockets to receive message
	Sleep(100);

	sockets.at(index).get()->disconnect();
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
		this->Send(name + ": " + textBox.Text());

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
	//TODO: server should add name, if receiving a message, shouldn't be sent by client
	lastMsg = "";
	//receiveData.clear();

	//TODO adhere to protocol

	if (selector.wait(sf::milliseconds(10)) && socketsConnected > 0)
	{
		for (int i = 0; i < (int)sockets.size(); i++)
		{
			if (selector.isReady((*sockets.at(i).get())))
			{
				if ((*sockets.at(i).get()).receive(receiveData) == sf::Socket::Done)
				{
					//TODO convert lastMsg to sf::String
					if (!(receiveData >> lastMsg))
					{
						debug::log("Error in receiving");
					}
					else
					{
						int dataSize = (int)receiveData.getDataSize();
						debug::log("data size: " + std::to_string(dataSize));
						debug::log("lastMsg (no decrypt): " + lastMsg);
						lastMsg = RSA::Decrypt(lastMsg, key.privKey);
						debug::log("lastMsg: " + lastMsg);
						std::string token = str::split(lastMsg, ' ').front();
						if (token == prot::msg)
						{
							debug::log("socket which send index: " + std::to_string(i));
							lastMsg = prot::remToken(lastMsg, token);
							if (!muted)
								snd::playSound("incoming_01");
							Send(names.at(i) + ": " + lastMsg, i);
							DisplayMessage(names.at(i) + ": " + lastMsg);
						}
						receiveData.clear();
					}
				}
				else if((*sockets.at(i).get()).receive(receiveData) == sf::Socket::Disconnected)
				{
					lastMsg = "[" + names.at(i) + " disconnected]";
					debug::log((std::string)lastMsg);

					//disconnect and delete socket
					selector.remove((*sockets.at(i).get()));
					(*sockets.at(i).get()).disconnect();

					sockets.erase(sockets.begin() + i);
					socketKeys.erase(socketKeys.begin() + i);

					socketsConnected--;
					own_log::append(names.at(i) + " (Place " + std::to_string(i) +
						") disconnected | Now there are " + std::to_string(socketsConnected) + " sockets connected");
					names.erase(names.begin() + i);

					//send message to all other sockets
					//not using send with exclude because socket is deleted
					Send(lastMsg);
					DisplayMessage(lastMsg);
					receiveData.clear();
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
	//server is terminated, clean up stuff here
	cleanCallbacks();

	sendButton.cleanup();
}

void Server::printNames()
{
	for (int i = 0; i < (int)names.size(); i++)
	{
		debug::log("Slot " + std::to_string(i) + ": " + names.at(i));
	}
}
#pragma endregion

#pragma region RSA
bool Server::GenerateKey(int max_errors)
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
void Server::initCallbacks()
{
	input::addLeftMouseCallback(lMCb, callback_id);
	input::addCloseCallback(cCb, callback_id);
	input::addTextEnteredCallback(tECb, callback_id);

	sendButton.setOnClickCallback(bCallback);
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
void Server::buttonCallback(std::string id)
{
	if (id == sendButton_id)
	{
		Enter();
		textBox.Select();
	}
}
#pragma endregion
