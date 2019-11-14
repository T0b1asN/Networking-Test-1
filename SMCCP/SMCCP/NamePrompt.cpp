#include "NamePrompt.h"

const std::string NamePrompt::okButton_id = "namePrompt_okB";

NamePrompt::NamePrompt(bool dontLoseFocus) :
	InputCallbackHandler("namePrompt" + str::createRandom(8)),
	callback_id_base("namePrompt"),
	prompt(sf::VideoMode((unsigned int)(500.f - 12.5f), 90U), "Set Name", sf::Style::Close),
	nameBox(sf::Vector2f(25.f, 25.f), sf::Vector2f(300.f, 40.f), "Name...", sf::Color::Black, sf::Color::White, &prompt),
	okButton(okButton_id, "Ok", sf::Vector2f(125.f, 40.f), sf::Vector2f(350.f - 12.5f, 25.f), sf::Color(0, 155, 0), sf::Color::Black, &prompt, 25)
{
	loseFocus = !dontLoseFocus;

	prompt.setFramerateLimit(60);
	nameBox.set_maxChars(MAX_NAME_LENGTH);

	sf::Image icon;
	if (icon.loadFromFile("res\\AppIcon.png"))
		prompt.setIcon(626, 626, icon.getPixelsPtr());
	else
		own_log::append("Icon could not be loaded!");

	callback_id_own = callback_id_base + str::createRandom(8);
	InputCallbackHandler::initCallbacks();

	input::setFocus(&prompt);
}

NamePrompt::~NamePrompt()
{
	
}

int NamePrompt::run_int()
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

NamePrompt::Result NamePrompt::run()
{
	nameBox.Select();
	while (prompt.isOpen() && returnVal == -1)
	{
		//debug::log("---- Prompt / Update");
		display();
		cr::updateUIElements();
		input::handleInput();
	}
	nextWindow();
	return result;
}

void NamePrompt::display()
{
	prompt.clear(sf::Color(100, 100, 100));

	nameBox.display();
	okButton.display();

	prompt.display();
}

void NamePrompt::LeftMCallback(int x, int y)
{
	if (nameBox.changed() && nameBox.Text() != "")
	{
		if (okButton.validClick(true))
		{
			name = nameBox.Text();
			nextWindow();
			returnVal = 0;
			result = Result::HasName;
			return;
		}
	}
	nameBox.SelectOrUnselect();
}

void NamePrompt::TextEnteredCallback(sf::Event::TextEvent text)
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
			result = Result::HasName;
		}
	}
}

void NamePrompt::LostFocusCallback()
{
	if (!loseFocus)
	{
		prompt.requestFocus();
	}
}

void NamePrompt::nextWindow()
{
	InputCallbackHandler::cleanCallbacks();
	okButton.cleanup();
	prompt.close();
}

void NamePrompt::CloseCallback()
{
	nextWindow();
	returnVal = 1;
	result = Result::Close;
}
