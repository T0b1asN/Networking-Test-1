#include "StartMenu.h"

StartMenu::StartMenu() :
	serverButton("Server", sf::Vector2f(200.0f, 100.0f), sf::Vector2f(cr::winWidth() / 4.0f, cr::winHeight() / 2.0f), sf::Color::White, sf::Color::Black, 25),
	clientButton("Client", sf::Vector2f(200.0f, 100.0f), sf::Vector2f(cr::winWidth() / 4.0f * 3.0f, cr::winHeight() / 2.0f), sf::Color::Black, sf::Color::White, 25)
{
	serverButton.SetOrigin(serverButton.GetSize() / 2.0f);
	clientButton.SetOrigin(clientButton.GetSize() / 2.0f);

	std::cout << serverButton.GetPos().x << "|" << serverButton.GetPos().y << std::endl;
}

StartMenu::~StartMenu()
{

}

StartMenu::Result StartMenu::open()
{
	bool run = true;
	while (run && cr::currWin().isOpen())
	{
		sf::Event evnt;
		while (cr::currWin().pollEvent(evnt))
		{
			switch (evnt.type)
			{
			case sf::Event::Closed:
				cr::currWin().close();
				return StartMenu::Close;
				break;
			case sf::Event::MouseButtonPressed:
				if (evnt.mouseButton.button == sf::Mouse::Left)
				{
					if (serverButton.validClick(true))
					{
						return StartMenu::Server;
					}
					else if (clientButton.validClick(true))
					{
						return StartMenu::Client;
					}
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
	cr::currWin().clear(sf::Color(100, 100, 100));

	serverButton.display();
	clientButton.display();

	cr::currWin().display();
}
