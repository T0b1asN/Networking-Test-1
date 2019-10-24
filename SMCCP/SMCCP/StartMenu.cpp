#include "StartMenu.h"

StartMenu::StartMenu() :
	BaseUIWindow(cr::winWidth(), cr::winHeight(), "Start Menu"),
	serverButton("Server", sf::Vector2f(200.0f, 100.0f), sf::Vector2f(cr::winWidth() / 4.0f, 75.f), sf::Color::White, sf::Color::Black, &window, 25),
	clientButton("Client", sf::Vector2f(200.0f, 100.0f), sf::Vector2f(cr::winWidth() / 4.0f * 3.0f, 75.f), sf::Color::Black, sf::Color::White, &window, 25),
	ipBox(sf::Vector2f(25.f, 135.f), sf::Vector2f(312.5f, 40.f), &window, sf::IpAddress::getLocalAddress().toString()),
	portBox(sf::Vector2f(350.f, 135.f), sf::Vector2f(125.f, 40.f), &window, std::to_string(1234))
{
	serverButton.SetOrigin(serverButton.GetSize() / 2.0f);
	clientButton.SetOrigin(clientButton.GetSize() / 2.0f);
	ipBox.set_deleteStdMsg(false);
	portBox.set_deleteStdMsg(false);

	ipBox.set_canReturnStdText(true);
	portBox.set_canReturnStdText(true);

	ipBox.set_maxChars(15);
	portBox.set_maxChars(5);

	port = 1234;
}

StartMenu::~StartMenu()
{

}

StartMenu::Result StartMenu::open()
{
	bool run = true;
	while (run && window.isOpen())
	{
		sf::Event evnt;
		while (window.pollEvent(evnt))
		{
			switch (evnt.type)
			{
			case sf::Event::Closed:
				window.close();
				return StartMenu::Close;
				break;
			case sf::Event::TextEntered:
				if (evnt.text.unicode != 13)
				{
					ipBox.Update(evnt.text.unicode);
					portBox.Update(evnt.text.unicode);
				}
				else
				{
					ipBox.Unselect();
					portBox.Unselect();
				}
				break;
			case sf::Event::MouseButtonPressed:
				if (evnt.mouseButton.button == sf::Mouse::Left)
				{
					if (serverButton.validClick(true))
					{
						adress = sf::IpAddress::getLocalAddress();

						try
						{
							port = std::stoi(portBox.Text().toAnsiString());
							return StartMenu::Server;
						}
						catch (std::invalid_argument inv_arg)
						{
							own_log::pushMsgToCommandIfDebug("Invalid Port");
						}
						catch (std::out_of_range oor)
						{
							own_log::pushMsgToCommandIfDebug("Port out of Range");
						}
					}
					else if (clientButton.validClick(true))
					{
						if (ntwrk::validIp(ipBox.Text()))
						{
							adress = sf::IpAddress(ipBox.Text());

							try
							{
								port = std::stoi(portBox.Text().toAnsiString());
								return StartMenu::Client;
							}
							catch (std::invalid_argument inv_arg)
							{
								own_log::pushMsgToCommandIfDebug("Invalid Port");
							}
							catch (std::out_of_range oor)
							{
								own_log::pushMsgToCommandIfDebug("Port out of Range");
							}
						}
					}

					ipBox.SelectOrUnselect();
					portBox.SelectOrUnselect();
				}
				break;
			}
		}
		display();
	}
	return StartMenu::Default;
}

void StartMenu::display()
{
	window.clear(sf::Color(100, 100, 100));

	serverButton.display();
	clientButton.display();

	ipBox.display();
	portBox.display();

	window.display();
}

void StartMenu::close()
{
	window.close();
}
