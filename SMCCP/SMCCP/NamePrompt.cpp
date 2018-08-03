#include "NamePrompt.h"

NamePrompt::NamePrompt(bool dontLoseFocus) :
	prompt(sf::VideoMode((unsigned int)(500.f - 12.5f), 90U), "Set Name", sf::Style::Close),
	nameBox(sf::Vector2f(25.f, 25.f), sf::Vector2f(300.f, 40.f), "Name...", sf::Color::Black, sf::Color::White, &prompt),
	okButton("Ok", sf::Vector2f(125.f, 40.f), sf::Vector2f(350.f - 12.5f, 25.f), sf::Color(0, 155, 0), sf::Color::Black, &prompt, 25)
{
	loseFocus = !dontLoseFocus;

	prompt.setFramerateLimit(60);
	nameBox.set_maxChars(MAX_NAME_LENGTH);

	sf::Image icon;
	if (icon.loadFromFile("res\\AppIcon.png"))
		prompt.setIcon(626, 626, icon.getPixelsPtr());
	else
		own_log::append("Icon could not be loaded!");

	initCallbacks();

	input::setFocus(&prompt);
}

NamePrompt::~NamePrompt()
{
	
}

void NamePrompt::initCallbacks()
{
	input::addLeftMouseCallback(lMCb, callback_id);
	input::addCloseCallback(cCb, callback_id);
	input::addTextEnteredCallback(tECb, callback_id);
	input::addLostFocusCallback(lFCb, callback_id);
}

void NamePrompt::cleanCallbacks()
{
	input::deleteCloseCallback(callback_id);
	input::deleteLMouseCallback(callback_id);
	input::deleteTextEnteredCallback(callback_id);
	input::deleteLostFocusCallback(callback_id);
}

int NamePrompt::run()
{
	nameBox.Select();
	while (prompt.isOpen() && returnVal == -1)
	{
		display();
		cr::updateUIElements();
		input::handleInput();
	}
	nextWindow();
	return returnVal;
}

void NamePrompt::display()
{
	prompt.clear(sf::Color(100, 100, 100));

	nameBox.display();
	okButton.display();

	prompt.display();
}

void NamePrompt::leftMouseDown(int x, int y)
{
	if (nameBox.changed() && nameBox.Text() != "")
	{
		if (okButton.validClick(true))
		{
			name = nameBox.Text();
			nextWindow();
			returnVal = 0;
			return;
		}
	}
	nameBox.SelectOrUnselect();
}

void NamePrompt::textEntered(sf::Event::TextEvent text)
{
	if (text.unicode != 13)
		nameBox.Update(text.unicode);
	else
	{
		nameBox.Unselect();
		if (nameBox.changed() && nameBox.Text() != "")
		{
			name = nameBox.Text();
			nextWindow();
			returnVal = 0;
		}
	}
}

void NamePrompt::lostFocus()
{
	if (!loseFocus)
	{
		prompt.requestFocus();
	}
}

void NamePrompt::nextWindow()
{
	okButton.~OwnButton();
	cleanCallbacks();
	prompt.close();
}

void NamePrompt::close()
{
	nextWindow();
	returnVal = 1;
}
