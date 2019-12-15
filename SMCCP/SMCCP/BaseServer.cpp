#include "BaseServer.h"

BaseServer::BaseServer(bool pBlock, unsigned int pPort, unsigned int pMaxClients) :
	block(pBlock),
	port(pPort),
	maxClients(pMaxClients),
	msgLogMaxLength(100)
{
	
}

BaseServer::~BaseServer()
{

}

void BaseServer::pushNewSocket()
{
	sockets.push_back((std::unique_ptr<sf::TcpSocket>)(new sf::TcpSocket()));
	selector.add((*sockets.back().get()));
}

BaseServer::SetupResult BaseServer::Setup()
{
	debug::log("------------ Server -----------------");
	own_log::append("\nServer session\n-------------------------------------------------------------", false);
	own_log::append("Setting up server on port " + std::to_string(port));
	listener.setBlocking(block);
	if (listener.listen(port) != sf::Socket::Status::Done)
	{
		debug::log("Error - Could not set up server!");
		own_log::append("-------------------------------------------------------------\n", false);
		return SetupResult::Error;
	}
	debug::log("Connected... Port: " + std::to_string(port));

	own_log::append("Server setup finished");
	return SetupResult::Done;
}

void BaseServer::ConnectClient()
{
	if (socketsConnected < (int)maxClients)
	{
		pushNewSocket();

		if (listener.accept(*(sockets.back().get())) != sf::Socket::Status::Done)
		{
			//most of the time, if no client tries to connect
			sockets.pop_back();
			return;
		}

		int index = sockets.size() - 1;

		//receive name from client
		sf::Packet namePacket;
		namePacket.clear();

		sockets.back().get()->receive(namePacket);

		std::string newSocketName;
		namePacket >> newSocketName;

		std::vector<std::string> splitName = str::split(newSocketName, ' ');
		if (splitName.empty()) return;

		newSocketName = prot::remToken(newSocketName, str::split(newSocketName, ' ').front());

		if (std::find(names.begin(), names.end(), newSocketName) != names.end())
		{
			sf::Packet respPacket;
			std::string resp = str::concat(prot::s::name_resp, " 1");
			debug::log("Resp: " + resp);
			respPacket << resp;
			sockets.back().get()->setBlocking(true);
			(sockets.back().get())->send(respPacket);
			(sockets.back().get())->disconnect();
			sockets.pop_back();
			return;
		}
		else
		{
			sf::Packet respPacket;
			std::string resp = str::concat(prot::s::name_resp, " 0");
			debug::log("Resp: " + resp);
			respPacket << resp;
			(sockets.back().get())->send(respPacket);
		}

		socketsConnected++;

		debug::log("--------- New Socket Name: " + newSocketName);
		names.push_back(newSocketName);

		(*sockets.back().get()).setBlocking(block);
		selector.add((*sockets.back().get()));

		pushNewMsg("[" + newSocketName + " connected]");

		//Send(str::concat(prot::s::msg_nosound, " ", lastMsg), socketsConnected - 1, true);

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

void BaseServer::pushNewMsg(sf::String msg)
{
	msgLog.push_back(msg);
	if (msgLog.size() > msgLogMaxLength)
		msgLog.pop_back();
}

void BaseServer::Send(sf::String msg, bool alreadyHasToken)
{
	if (!alreadyHasToken)
		msg = str::concat(prot::msg, " ", msg);
	for (int i = 0; i < (int)sockets.size(); i++)
	{
		std::string encrypted = msg;
		sendData.clear();
		sendData << encrypted;
		debug::log("Sending: " + encrypted + " (packet size: " + std::to_string((int)sendData.getDataSize()) + ")");
		sf::Socket::Status stat = (*sockets.at(i).get()).send(sendData);
		debug::log("Send status: " + network::statusToString(stat));
	}
}

void BaseServer::Send(sf::String msg, int exclude, bool alreadyHasToken)
{
	for (int i = 0; i < (int)sockets.size(); i++)
	{
		if (i != exclude)
			SendSingle(msg, i, alreadyHasToken);
	}
}

void BaseServer::SendSingle(sf::String msg, int socketIndex, bool alreadyHasToken)
{
	if (!alreadyHasToken)
		msg = str::concat(prot::msg, " ", msg);
	debug::log("MSG before: " 
		+ static_cast<std::string>(msg) 
		+ "| index: " 
		+ std::to_string(socketIndex));
	sendData.clear();
	sendData << msg;
	debug::log("Sending: " 
		+ static_cast<std::string>(msg) 
		+ " (packet size: " 
		+ std::to_string((int)sendData.getDataSize()) + ")");
	(*sockets.at(socketIndex).get()).send(sendData);
}

void BaseServer::Shutdown(sf::String optMsg)
{
	sf::String msg = "";
	if (optMsg == "")
		msg = str::concat(prot::s::msg_nosound, " [Server was shut down]");
	else
		msg = str::concat(prot::s::msg_nosound, " [Server was shut down. Message: ", optMsg, "]");

	Send(prot::s::shutdown);
	for (int i = 0; i < (int)sockets.size(); i++)
		disconnectSocket(i, msg);
}

void BaseServer::disconnectSocket(int index, sf::String reason)
{
	if (reason != "")
		SendSingle("[Disconnected from server. Reason: " + reason + "]", index);
	else
		SendSingle("[Disconnected from server for an unknown reason]", index);
	//wait for sockets to receive message
	Sleep(100);

	sockets.at(index).get()->disconnect();
}

void BaseServer::Update()
{
	//TODO: server should add name, if receiving a message, shouldn't be sent by client
	sf::String lastMsg = "";
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
						std::string token = str::split(lastMsg, ' ').front();
						if (token == prot::msg)
						{
							pushNewMsg(prot::remToken(lastMsg, token));
							OnMessage();
						}
						receiveData.clear();
					}
				}
				else if ((*sockets.at(i).get()).receive(receiveData) == sf::Socket::Disconnected)
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
					//not using send with exclude because socket is deleted
					Send(lastMsg);
					OnDisconnect();
					receiveData.clear();
					//TODO: Disconnect sound
				}
			}

		}
	}
	ConnectClient();
}
