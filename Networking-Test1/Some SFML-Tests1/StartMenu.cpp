#include "StartMenu.h"

StartMenu::StartMenu() :
	serverButton("Server", sf::Vector2f(200.0f, 100.0f), sf::Vector2f(cr::winWidth() / 4.0f, cr::winHeight() / 4.0f * 2.5f), sf::Color::White, sf::Color::Black, 25),
	clientButton("Client", sf::Vector2f(200.0f, 100.0f), sf::Vector2f(cr::winWidth() / 4.0f * 3.0f, cr::winHeight() / 4.0f * 2.5f), sf::Color::Black, sf::Color::White, 25),
	nameBox(sf::Vector2f(25.0f, 25.0f), sf::Vector2f(450.f, 40.0f), "Input name...")
{
	serverButton.SetOrigin(serverButton.GetSize() / 2.0f);
	clientButton.SetOrigin(clientButton.GetSize() / 2.0f);
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
				name = nameBox.Text();
				cr::currWin().close();
				return StartMenu::Close;
				break;
			case sf::Event::TextEntered:
				if (evnt.text.unicode != 13)
				{
					nameBox.Update(evnt.text.unicode);
				}
				else
				{
					nameBox.Unselect();
				}
				break;
			case sf::Event::MouseButtonPressed:
				if (evnt.mouseButton.button == sf::Mouse::Left)
				{
					if (serverButton.validClick(true))
					{
						name = nameBox.Text();
						return StartMenu::Server;
					}
					else if (clientButton.validClick(true))
					{
						name = nameBox.Text();
						return StartMenu::Client;
					}
					nameBox.SelectOrUnselect();
				}
				break;
			}
		}
		nameBox.Update((char)0);
		display();
	}
	return StartMenu::Default;
}

void StartMenu::display()
{
	cr::currWin().clear(sf::Color(100, 100, 100));

	serverButton.display();
	clientButton.display();
	nameBox.display();

	cr::currWin().display();
}
