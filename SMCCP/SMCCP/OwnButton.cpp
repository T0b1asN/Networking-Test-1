#include "OwnButton.h"

OwnButton::OwnButton(sf::String pText, sf::Vector2f pSize, sf::Vector2f pPos,
	sf::Color backColor, sf::Color textColor, sf::RenderWindow* winPtr, unsigned int pCharSize)
{
	win = winPtr;

	pos = pPos;
	text = pText;
	size = pSize;

	if (pCharSize == 0)
	{
		charSize = (unsigned int)std::abs(size.y) / 4U;
	}
	else
	{
		charSize = pCharSize;
	}

	rect = sf::FloatRect(pos, size);

	textField = sf::Text(text, cr::currFont(), charSize);
	textField.setFillColor(textColor);
	textField.setOrigin(textField.getLocalBounds().width / 2.0f, textField.getLocalBounds().height / 2.0f);
	textField.setPosition(pos + (size / 2.0f));

	field = sf::RectangleShape(size);
	field.setPosition(pos);
	field.setFillColor(backColor);

	offset = sf::Vector2f(0.0f, 0.0f);

	addToList();
}

OwnButton::~OwnButton()
{
	deleteFromList();
}

bool OwnButton::checkClick()
{
	if (win->isOpen())
	{
		if (rect.contains((sf::Vector2f)sf::Mouse::getPosition(*win)))
		{
			return true;
		}
	}
	return false;
}

void OwnButton::display()
{
	win->draw(field);
	win->draw(textField);
}

bool OwnButton::validClick(bool click)
{
	if (click)
	{
		if (win->isOpen())
		{
			if (rect.contains((sf::Vector2f)sf::Mouse::getPosition(*win)))
			{
				if (callback)
					callback();
				else
					own_log::append("Button: callback not set!");
				return true;
			}
		}
	}
	return false;
}

void OwnButton::SetOrigin(sf::Vector2f newOffset)
{
	offset = -newOffset;
	field.setPosition(pos + offset);
	textField.setPosition(pos + (size / 2.0f) + offset);
	rect = sf::FloatRect(pos + offset, size);
}

void OwnButton::SetPos(sf::Vector2f newPos)
{
	pos = newPos;
	field.setPosition(pos + offset);
	textField.setPosition(pos + (size / 2.0f) + offset);
	rect = sf::FloatRect(pos + offset, size);
}

void OwnButton::SetSize(sf::Vector2f newSize)
{
	size = newSize;
	field.setSize(size);
	offset = -(size / 2.0f);

	field.setPosition(pos + offset);
	textField.setPosition(pos + (size / 2.0f) + offset);
	rect = sf::FloatRect(pos + offset, size);
}

void OwnButton::SetText(std::string newText)
{
	text = newText;
	textField.setString(text);
}

void OwnButton::setCharSize(unsigned int newSize)
{
	charSize = newSize;
	textField.setCharacterSize(newSize);
	textField.setOrigin(textField.getLocalBounds().width / 2.0f, textField.getLocalBounds().height / 2.0f);
	textField.setPosition(pos + (size / 2.0f));
}

void OwnButton::setOnClickCallback(std::function<void()> newCallback)
{
	callback = newCallback;
}

void OwnButton::update()
{

}
