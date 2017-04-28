#include "NamePrompt.h"

NamePrompt::NamePrompt() : 
	prompt(sf::VideoMode((unsigned int)(500.f - 12.5f), 90U), "Set Name", sf::Style::Close),
	nameBox(sf::Vector2f(25.f, 25.f), sf::Vector2f(300.f, 40.f), "Name...", sf::Color::Black, sf::Color::White, &prompt),
	okButton("Ok", sf::Vector2f(125.f, 40.f), sf::Vector2f(350.f - 12.5f, 25.f), sf::Color(0, 155, 0), sf::Color::Black, &prompt, 25)
{
	prompt.setFramerateLimit(60);
	nameBox.set_maxChars(MAX_NAME_LENGTH);

	sf::Image icon;
	if (icon.loadFromFile("res\\AppIcon.png"))
		prompt.setIcon(626, 626, icon.getPixelsPtr());
	else
		own_log::AppendToLog("Icon could not be loaded!");
}

NamePrompt::~NamePrompt()
{

}

int NamePrompt::run()
{
	//TODO: Select TextBox on open
	nameBox.Select();
	bool run = true;
	while (run && prompt.isOpen())
	{
		sf::Event evnt;
		while (prompt.pollEvent(evnt))
		{
			switch (evnt.type)
			{
			case sf::Event::Closed:
				prompt.close();
				return 1;
				break;
			case sf::Event::TextEntered:
				if (evnt.text.unicode != 13)
					nameBox.Update(evnt.text.unicode);
				else
				{
					nameBox.Unselect();
					if (nameBox.wasChanged() && nameBox.Text() != "")
					{
						name = nameBox.Text();
						prompt.close();
						return 0;
					}
				}
				break;
			case sf::Event::MouseButtonPressed:
				if (evnt.mouseButton.button == sf::Mouse::Left)
				{
					if (nameBox.wasChanged() && nameBox.Text() != "")
					{
						if(okButton.validClick(true))
						{
							name = nameBox.Text();
							prompt.close();
							return 0;
						}
					}
					nameBox.SelectOrUnselect();
				}
				break;
			}
		}
		display();
	}
	return 2;
}

void NamePrompt::display()
{
	prompt.clear(sf::Color(100, 100, 100));

	nameBox.display();
	okButton.display();

	prompt.display();
}
