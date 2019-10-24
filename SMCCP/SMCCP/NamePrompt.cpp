#include "NamePrompt.h"

NamePrompt::NamePrompt() :
	BaseUIWindow((unsigned int)(500.f - 12.5f), 90U, "Name Prompt"),
	nameBox(sf::Vector2f(25.f, 25.f), sf::Vector2f(300.f, 40.f), &window, "Name..."),
	okButton("Ok", sf::Vector2f(125.f, 40.f), sf::Vector2f(350.f - 12.5f, 25.f), sf::Color(0, 155, 0), sf::Color::Black, &window, 25)
{
	nameBox.set_maxChars(MAX_NAME_LENGTH);
}

NamePrompt::~NamePrompt()
{

}

int NamePrompt::run()
{
	nameBox.Select();
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
						window.close();
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
							window.close();
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
	window.clear(sf::Color(100, 100, 100));

	nameBox.display();
	okButton.display();

	window.display();
}
