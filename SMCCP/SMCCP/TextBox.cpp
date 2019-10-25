#include "TextBox.h"

TextBox::TextBox(
	sf::Vector2f pPos, sf::Vector2f pSize, std::string pStandardText, 
	sf::Color pBackColor, sf::Color pTextColor, sf::RenderWindow* winPtr)
{
	win = winPtr;
	pos = pPos;
	size = pSize;

	fontSize = (unsigned int)size.y;

	standardText = pStandardText;
	actualText = str::str_to_wstr(standardText);

	text.setFont(cr::currFont());
	text.setFillColor(pTextColor);
	text.setPosition(pos);
	text.setCharacterSize(fontSize);
	text.setString(actualText);

	background = sf::RectangleShape(size);
	background.setPosition(pos);
	background.setFillColor(pBackColor);
}

TextBox::~TextBox()
{

}

void TextBox::display()
{
	win->draw(background);
	win->draw(text);
}

void TextBox::SelectOrUnselect()
{
	if (win->isOpen())
	{
		if (background.getGlobalBounds().contains((sf::Vector2f)sf::Mouse::getPosition(*win)))
		{
			selected = true;
			if (deleteStdMsg)
			{
				if (actualText == str::str_to_wstr(standardText))
					actualText = L"";
				text.setString(actualText);
			}
		}
		else
		{
			selected = false;
			if (actualText == L"")
			{
				actualText = str::str_to_wstr(standardText);
				text.setString(actualText);
			}
		}
	}
}

void TextBox::SelectOrUnselect(int x, int y)
{
	if (win->isOpen())
	{
		if (background.getGlobalBounds().contains(sf::Vector2f((float)x, (float)y)))
		{
			selected = true;
			if (deleteStdMsg)
			{
				if (actualText == str::str_to_wstr(standardText))
					actualText = L"";
				text.setString(actualText);
			}
		}
		else
		{
			selected = false;
			if (actualText == L"")
			{
				actualText = str::str_to_wstr(standardText);
				text.setString(actualText);
			}
		}
	}
}

void TextBox::Select()
{
	selected = true;
	if (deleteStdMsg)
	{
		if (actualText == str::str_to_wstr(standardText))
			actualText = L"";
		text.setString(actualText);
	}
}

void TextBox::Unselect()
{
	selected = false;
	if (actualText == L"")
	{
		actualText = str::str_to_wstr(standardText);
		text.setString(actualText);
	}
}

void TextBox::Update(sf::String add)
{
	if (add != (char)0)
	{
		if (selected)
		{
			if (add == (char)8)
			{
				//actualText = actualText.substr(0, actualText.size() - 1);
				if (actualText.getSize() > 0)
					actualText.erase(actualText.getSize() - 1, 1);
			}
			else if(actualText.getSize() < maxChars)
			{
				actualText += add;
			}
		}
		text.setString(actualText);
	}
}

void TextBox::SetNormal()
{
	actualText = str::str_to_wstr(standardText);
	text.setString(actualText);
}

sf::String TextBox::Text()
{ 
	if (text.getString() == standardText)
	{
		if (canReturnStdText)
			return text.getString();
		return "";
	}
	else
		return text.getString();
}
