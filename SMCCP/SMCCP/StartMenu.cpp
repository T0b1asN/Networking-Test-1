#include "StartMenu.h"

const std::string StartMenu::serverButton_id = "startMenu_serverB";
const std::string StartMenu::clientButton_id = "startMenu_clientB";

StartMenu::StartMenu() :
	callback_id("startMenu"),
	serverButton(serverButton_id, "Server", sf::Vector2f(200.0f, 100.0f), sf::Vector2f(cr::winWidth() / 4.0f, 75.f), sf::Color::White, sf::Color::Black, &cr::currWin(), 25U),
	clientButton(clientButton_id, "Client", sf::Vector2f(200.f, 100.f), sf::Vector2f(cr::winWidth()/4.f*3.f, 75.f), sf::Color::Black, sf::Color::White, &cr::currWin(), 25U),
	ipBox(sf::Vector2f(25.f, 135.f), sf::Vector2f(312.5f, 40.f), sf::IpAddress::getLocalAddress().toString()),
	portBox(sf::Vector2f(350.f, 135.f), sf::Vector2f(125.f, 40.f), std::to_string(1234))
{
	serverButton.SetOrigin(serverButton.GetSize() / 2.0f);
	clientButton.SetOrigin(clientButton.GetSize() / 2.0f);
	ipBox.set_deleteStdMsg(false);
	portBox.set_deleteStdMsg(false);

	ipBox.set_canReturnStdText(true);
	portBox.set_canReturnStdText(true);

	ipBox.set_maxChars(15);
	portBox.set_maxChars(5);

	initCallbacks();
}

StartMenu::~StartMenu()
{

}

void StartMenu::initCallbacks()
{
	input::addLeftMouseCallback(
		std::bind(
			&StartMenu::leftMouseDown, this,
			std::placeholders::_1, std::placeholders::_2),
		callback_id);
	input::addCloseCallback(
		std::bind(&StartMenu::close, this),
		callback_id);
	input::addTextEnteredCallback(
		std::bind(&StartMenu::textEntered, this, std::placeholders::_1),
		callback_id);
}

StartMenu::Result StartMenu::open()
{
	while (cr::currWin().isOpen() && returnVal == Result::None)
	{
		display();
		cr::updateUIElements();
		input::handleInput();
	}
	return returnVal;
}

void StartMenu::leftMouseDown(int x, int y)
{
	//std::cout << "Test" << std::endl;
	//system("pause");
	//TODO something is going VERY wrong -> see nextWindow
	/**/
	if (serverButton.validClick(true))
	{
		adress = sf::IpAddress::getLocalAddress();

		try
		{
			port = std::stoi(portBox.Text().toAnsiString());
			nextWindow();
			returnVal = StartMenu::Result::Server;
		}
		catch (std::invalid_argument inv_arg)
		{
			debug::log("Invalid Port");
		}
		catch (std::out_of_range oor)
		{
			debug::log("Port out of Range");
		}
	}
	else if (clientButton.validClick(true))
	{
		if (network::validIp(ipBox.Text()))
		{
			adress = sf::IpAddress(ipBox.Text());

			try
			{
				port = std::stoi(portBox.Text().toAnsiString());
				nextWindow();
				returnVal = StartMenu::Result::Client;
			}
			catch (std::invalid_argument inv_arg)
			{
				debug::log("Invalid Port");
			}
			catch (std::out_of_range oor)
			{
				debug::log("Port out of Range");
			}
		}
	}
	/**/
	std::cout << "Test" << std::endl;
	ipBox.SelectOrUnselect(x, y);
	portBox.SelectOrUnselect(x, y);
}

void StartMenu::textEntered(sf::Event::TextEvent text)
{
	if (text.unicode != 13)
	{
		ipBox.Update(text.unicode);
		portBox.Update(text.unicode);
	}
	else
	{
		ipBox.Unselect();
		portBox.Unselect();
	}
}

void StartMenu::display()
{
	cr::currWin().clear(sf::Color(100, 100, 100));

	serverButton.display();
	clientButton.display();

	ipBox.display();
	portBox.display();

	cr::currWin().display();
}

void StartMenu::nextWindow()
{
	serverButton.cleanup();
	clientButton.cleanup();

	input::deleteLMouseCallback(callback_id);
	input::deleteTextEnteredCallback(callback_id);
	input::deleteCloseCallback(callback_id);
}

void StartMenu::close()
{
	returnVal = Result::Close;
	nextWindow();
	cr::currWin().close();
}
